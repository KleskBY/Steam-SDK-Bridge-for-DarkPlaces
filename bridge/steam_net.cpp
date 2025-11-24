#include "steam_net.h"
#include "steam.h"
#include "steam_lobby.h"
#include "main.h"

#include <public/steam/steam_api.h>
#include <public/steam/isteamnetworkingsockets.h>
#include <public/steam/isteamnetworkingutils.h>
#include <public/steam/steamnetworkingfakeip.h>

#include <stdio.h>
#include <string.h>

// ----- shared state -----
static bool g_gnsInit = false;
static ISteamNetworkingSockets* g_sockets = nullptr;

// ----- server state -----
static HSteamListenSocket       g_hListenSocket = k_HSteamListenSocket_Invalid;
static HSteamNetPollGroup       g_hPollGroup = k_HSteamNetPollGroup_Invalid;
static SteamNetworkingIPAddr    g_serverFakeIP;

// ----- client state -----
static HSteamNetConnection      g_hClientConn = k_HSteamNetConnection_Invalid;


// -------- peers ---------
struct SteamPeer
{
    HSteamNetConnection conn;
};
static SteamPeer g_serverPeers[64];

static int AllocServerPeer(HSteamNetConnection c)
{
    for (int i = 0; i < (int)(sizeof(g_serverPeers) / sizeof(g_serverPeers[0])); ++i)
    {
        if (g_serverPeers[i].conn == k_HSteamNetConnection_Invalid)
        {
            g_serverPeers[i].conn = c;
            return i;
        }
    }
    return -1;
}

static int FindServerPeer(HSteamNetConnection c)
{
    for (int i = 0; i < (int)(sizeof(g_serverPeers) / sizeof(g_serverPeers[0])); ++i)
        if (g_serverPeers[i].conn == c)
            return i;
    return -1;
}

static void ResetPeers()
{
    memset(g_serverPeers, 0, sizeof(g_serverPeers));
    for (int i = 0; i < (int)(sizeof(g_serverPeers) / sizeof(g_serverPeers[0])); ++i)
        g_serverPeers[i].conn = k_HSteamNetConnection_Invalid;
}

void SteamNet_DropServerConnection(HSteamNetConnection hConn, const char* reason)
{
    int peerIdx = FindServerPeer(hConn);
    if (peerIdx >= 0)
        g_serverPeers[peerIdx].conn = k_HSteamNetConnection_Invalid;

    if (g_ExecFromEngine)
    {
        std::string cmd = "steam_drop " + std::to_string(hConn) + " \"" + std::string(reason ? reason : "") + "\"";
        g_ExecFromEngine(cmd.c_str());
    }

    if (g_sockets)
        g_sockets->CloseConnection(hConn, 0, nullptr, false);
}

// Connection status callback class
class SteamNetCallbacks
{
public:
    SteamNetCallbacks() : m_connStatusChanged(this, &SteamNetCallbacks::OnConnectionStatusChanged)
    {
    }

private:
    STEAM_CALLBACK(SteamNetCallbacks, OnConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t, m_connStatusChanged);
};

static SteamNetCallbacks g_callbacks; // static so callback is alive

void SteamNetCallbacks::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
{
    switch (info->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_Connecting:
    {
        // Incoming connections have a non-invalid listen socket handle.
        if (info->m_info.m_hListenSocket != k_HSteamListenSocket_Invalid && info->m_info.m_hListenSocket == g_hListenSocket)
        {
            // *** SERVER SIDE: NEW INCOMING CLIENT ***
            if (g_sockets->AcceptConnection(info->m_hConn) == k_EResultOK)
            {
                g_sockets->SetConnectionPollGroup(info->m_hConn, g_hPollGroup);
                int idx = AllocServerPeer(info->m_hConn);
                log("SteamNet: incoming connection accepted (conn " + std::to_string(info->m_hConn) + ", slot " + std::to_string(idx) + ")");
            }
            else
            {
                g_sockets->CloseConnection(info->m_hConn, 0, nullptr, false);
            }
        }
        else //Client side (outgoing)
        {
            log("SteamNet: outgoing connection " + std::to_string(info->m_hConn) + " is connecting");
        }
        break;
    }
    case k_ESteamNetworkingConnectionState_Connected:
        log("SteamNet: connection " + std::to_string(info->m_hConn) + " now connected");
        break;
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        SteamNet_DropServerConnection(info->m_hConn, info->m_info.m_szEndDebug);
    }
    break;

    default:
        break;
    }
}

// ----- shared bootstrap -----

void SteamNet_Debug(ESteamNetworkingSocketsDebugOutputType nType, const char* msg)
{
    log(msg);
}

int SteamNet_Init(void)
{
    if (g_gnsInit && g_sockets)
        return 1;

    if (!g_steamInitialized)
    {
        if (!Steam_Init())
        {
            log("SteamNet_Init: Steam_Init failed");
            return 0;
        }
    }

    g_sockets = SteamNetworkingSockets();
    if (!g_sockets)
    {
        log("SteamNet_Init: SteamNetworkingSockets() returned null");
        return 0;
    }

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, SteamNet_Debug);
    SteamNetworkingUtils()->InitRelayNetworkAccess();
    ResetPeers();
    if (!g_sockets->BeginAsyncRequestFakeIP(1))
    {
        log("SteamNet: BeginAsyncRequestFakeIP failed or already in progress");
    }

    g_gnsInit = true;
    return 1;
}

// Stop listening for Steam/SDR connections and close server-side peers,
// but keep the Steam API and any client connection alive.
void SteamNet_ServerStop(void)
{
    if (!g_sockets) return;
    if(SteamFriends()) SteamFriends()->ClearRichPresence();

    // Close all server-side connections
    for (int i = 0; i < (int)(sizeof(g_serverPeers) / sizeof(g_serverPeers[0])); ++i)
    {
        if (g_serverPeers[i].conn != k_HSteamNetConnection_Invalid)
        {
            g_sockets->CloseConnection(g_serverPeers[i].conn, 0, nullptr, false);
            g_serverPeers[i].conn = k_HSteamNetConnection_Invalid;
        }
    }

    // Destroy poll group used by the listen socket
    if (g_hPollGroup != k_HSteamNetPollGroup_Invalid)
    {
        g_sockets->DestroyPollGroup(g_hPollGroup);
        g_hPollGroup = k_HSteamNetPollGroup_Invalid;
    }

    // Close the listen socket itself
    if (g_hListenSocket != k_HSteamListenSocket_Invalid)
    {
        g_sockets->CloseListenSocket(g_hListenSocket);
        g_hListenSocket = k_HSteamListenSocket_Invalid;
    }
    log("SteamNet Server shutdown");
}


// ----- server side -----

int SteamNet_ServerStart()
{
    if (!SteamNet_Init()) return 0;
    if (g_hListenSocket != k_HSteamListenSocket_Invalid)
    {
        log("SteamNet: SteamNet_ServerStart is already running");
        return 1;
    }

    lhnetaddress_t fake_addr;
    if (!SteamNet_ServerGetFakeIP(&fake_addr))
    {
        if (!g_sockets->BeginAsyncRequestFakeIP(1)) // Ask Steam for 1 FakeIP port (index 0)
        {
            log("SteamNet: BeginAsyncRequestFakeIP failed or already in progress");
        }
    }

    // Create listen socket bound to FakeIP port index 0
    g_hListenSocket = g_sockets->CreateListenSocketP2PFakeIP(0, /* idxFakePort */ 0, /* nOptions */ nullptr);
    if (g_hListenSocket == k_HSteamListenSocket_Invalid)
    {
        log("SteamNet: CreateListenSocketP2PFakeIP failed");
        return 0;
    }

    g_hPollGroup = g_sockets->CreatePollGroup();
    if (g_hPollGroup == k_HSteamNetPollGroup_Invalid)
    {
        log("SteamNet: CreatePollGroup failed");
        g_sockets->CloseListenSocket(g_hListenSocket);
        g_hListenSocket = k_HSteamListenSocket_Invalid;
        return 0;
    }

    SteamNet_CreateLobby();
    return 1;
}

int SteamNet_ServerGetFakeIP(lhnetaddress_t* out_addr)
{
    if (!g_sockets || !out_addr) return 0;

    SteamNetworkingFakeIPResult_t info{};
    int idxFirstPort = 0; // since we requested 1 port
    g_sockets->GetFakeIP(idxFirstPort, &info);
    if (info.m_eResult != k_EResultOK)
    {
        // k_EResultBusy, k_EResultNoMatch, etc. – just report "not ready" for now
        return 0;
    }

    uint32 ip = info.m_unIP;
    uint16 port = info.m_unPorts[0]; // first FakeIP port we requested

    lhnetaddressnative_t* addr = (lhnetaddressnative_t*)out_addr;
    memset(addr, 0, sizeof(*addr));

    addr->addresstype = LHNETADDRESSTYPE_STEAM_FAKEIP;
    addr->port = port;
    addr->addr.in.sin_family = AF_INET;
    addr->addr.in.sin_port = htons(port);
    addr->addr.in.sin_addr.s_addr = htonl(ip);

    char fakeip_str[64];
    snprintf(fakeip_str, sizeof(fakeip_str), "%u.%u.%u.%u:%hu", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF, port);
    log("connect " + std::string(fakeip_str));
    return 1;
}
const char* SteamNet_ServerGetFakeIP_String()
{
    static char fakeip_str[64];   // returned string stays valid across calls

    if (!g_sockets) return NULL;

    SteamNetworkingFakeIPResult_t info{};
    g_sockets->GetFakeIP(0, &info);

    if (info.m_eResult != k_EResultOK)
        return NULL;

    uint32 ip = info.m_unIP;
    uint16 port = info.m_unPorts[0];

    snprintf(fakeip_str, sizeof(fakeip_str), "%u.%u.%u.%u:%hu", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF, port);
    log("connect " + std::string(fakeip_str));
   
    if (SteamFriends())
    {
        char connect_string[128];
        snprintf(connect_string, sizeof(connect_string), "+connect %s", fakeip_str);
        SteamFriends()->SetRichPresence("connect", connect_string);
        SteamFriends()->SetRichPresence("status", "On server");
    }
    return fakeip_str;
}

// ----- client side -----

void SteamNet_ClientDisconnect(void)
{
    if (g_sockets && g_hClientConn != k_HSteamNetConnection_Invalid)
    {
        g_sockets->CloseConnection(g_hClientConn, 0, nullptr, false);
        g_hClientConn = k_HSteamNetConnection_Invalid;
    }
}

int SteamNet_ClientConnectFakeIP(const lhnetaddress_t* server_addr)
{
    if (!server_addr) return 0;
    if (!SteamNet_Init()) return 0;

    const lhnetaddressnative_t* addr = (const lhnetaddressnative_t*)server_addr;
    if (addr->addresstype != LHNETADDRESSTYPE_STEAM_FAKEIP && addr->addresstype != LHNETADDRESSTYPE_INET4)
    {
        log("SteamNet_ClientConnectFakeIP: expected STEAM_FAKEIP/INET4 address");
        return 0;
    }

    uint32 ip_host = ntohl(addr->addr.in.sin_addr.s_addr);
    uint16 port = (uint16)addr->port;

    SteamNetworkingIPAddr saddr;
    saddr.Clear();
    saddr.SetIPv4(ip_host, port);

    SteamNet_ClientDisconnect();

    g_hClientConn = g_sockets->ConnectByIPAddress(saddr, 0, nullptr);
    if (g_hClientConn == k_HSteamNetConnection_Invalid)
    {
        log("SteamNet: ConnectByIPAddress failed");
        return 0;
    }

    log("SteamNet: connecting to FakeIP via SDR");
    return 1;
}

void SteamNet_Shutdown(void)
{
    if (!g_sockets)
    {
        g_gnsInit = false;
        return;
    }
    SteamNet_ServerStop();
    SteamNet_ClientDisconnect();
    g_sockets = nullptr;
    g_gnsInit = false;
}


bool SteamNet_IsActive(void)
{
    if (!g_sockets) return false;

    // 1) Client -> server Steam connection active?
    if (g_hClientConn != k_HSteamNetConnection_Invalid) return true;

    // 2) Server is listening using Steam?
    if (g_hListenSocket != k_HSteamListenSocket_Invalid) return true;

    // 3) Any accepted Steam peers connected to server?
    for (int i = 0; i < (int)(sizeof(g_serverPeers) / sizeof(g_serverPeers[0])); ++i)
    {
        if (g_serverPeers[i].conn != k_HSteamNetConnection_Invalid)
            return true;
    }

    return false;
}


int SteamNet_Read(lhnetsocket_t* mysocket, void* data, int maxlength, lhnetaddress_t* peeraddress)
{
    ISteamNetworkingMessage* pMsg = nullptr;
    int num = -1;
    if (SteamNet_IsClient())
    {
        num = g_sockets->ReceiveMessagesOnConnection(g_hClientConn, &pMsg, 1);
    }
    else num = g_sockets->ReceiveMessagesOnPollGroup(g_hPollGroup, &pMsg, 1);

    if (num <= 0) return 0;

    int len = pMsg->GetSize();
    if (len > maxlength) len = maxlength;
    memcpy(data, pMsg->GetData(), len);

    lhnetaddressnative_t* a = (lhnetaddressnative_t*)peeraddress;
    memset(a, 0, sizeof(*a));

    a->addresstype = LHNETADDRESSTYPE_STEAM_FAKEIP;
    a->port = 0; // not really used directly
    a->addr.in.sin_family = AF_INET;

    // Optional but recommended: fill FakeIP for pretty logging
    SteamNetConnectionInfo_t info;
    if (g_sockets->GetConnectionInfo(pMsg->m_conn, &info))
    {
        SteamNetworkingIPAddr ip;
        if (g_sockets->GetRemoteFakeIPForConnection(pMsg->m_conn, &ip))
        {
            uint32 ipNetOrder = ip.GetIPv4(); // network order
            a->addr.in.sin_addr.s_addr = ipNetOrder;

            uint16 fakePort = ip.m_port; // already in host order
            a->addr.in.sin_port = htons(fakePort);
            a->port = fakePort;
        }
    }

    // Store Steam info for writes
    a->hConn = pMsg->m_conn;
    a->steamid = pMsg->m_identityPeer.GetSteamID64();
    pMsg->Release();
    return len;
}

int SteamNet_Write(lhnetsocket_t* mysocket, const void* content, int contentlength, const lhnetaddress_t* vaddress)
{
    int value = -1;
    if (!mysocket || !vaddress || !content || contentlength <= 0) return -1;
    if (!g_sockets)  return -1;

    HSteamNetConnection hConn = k_HSteamNetConnection_Invalid;
    const lhnetaddressnative_t* a = (const lhnetaddressnative_t*)vaddress;
    if (g_hClientConn != k_HSteamNetConnection_Invalid) // 
    {
        hConn = g_hClientConn;
        if (vaddress->addresstype == LHNETADDRESSTYPE_INET4 || vaddress->addresstype == LHNETADDRESSTYPE_STEAM_FAKEIP) // if we can pass a steam address with per-peer hConn
        {
            if ((HSteamNetConnection)a->hConn != k_HSteamNetConnection_Invalid) hConn = (HSteamNetConnection)a->hConn;
        }
    }
    else
    {
        hConn = (HSteamNetConnection)a->hConn;
    }

    if (hConn == k_HSteamNetConnection_Invalid)
    {
        return -1;
    }

    EResult res = g_sockets->SendMessageToConnection(hConn, content, contentlength, k_nSteamNetworkingSend_UnreliableNoNagle, nullptr);
    switch (res)
    {
    case k_EResultOK:
    case k_EResultNoConnection: // queued but connection closing; UDP = fire-and-forget
    case k_EResultPending:
        value = contentlength;
        break;
    case k_EResultLimitExceeded:
        value = 0; // emulate "send buffer full" / EWOULDBLOCK
        break;
    default:
        log("SteamNet_Write: SendMessageToConnection failed. Error code: " + std::to_string((int)res));
        value = -1;
        break;
    }
    return value;
}


void SteamNet_Frame()
{
    if (!g_sockets) return;
    SteamAPI_RunCallbacks();
}

bool SteamNet_IsFakeIP(unsigned __int32 ipHostOrder)
{
    return SteamNetworkingUtils() && SteamNetworkingUtils()->IsFakeIPv4(ipHostOrder);
}

bool SteamNet_IsClient(void)
{
    return g_hClientConn != k_HSteamNetConnection_Invalid;
}

bool SteamNet_IsFakeIPAddr(const lhnetaddress_t* a)
{
    if (!a || !SteamNetworkingUtils()) return false;
    const lhnetaddressnative_t* n = (const lhnetaddressnative_t*)a;
    if (n->addresstype != LHNETADDRESSTYPE_INET4 && n->addresstype != LHNETADDRESSTYPE_STEAM_FAKEIP) return false;
    uint32 ip_host = ntohl(n->addr.in.sin_addr.s_addr);
    return SteamNetworkingUtils()->IsFakeIPv4(ip_host);
}



