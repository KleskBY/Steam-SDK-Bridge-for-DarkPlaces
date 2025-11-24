#include "../darkplaces.h"
#include "../lhnet.h"
#include "../netconn.h"
#include "../console.h"
#include "steam.h"
#include "steam_net.h"


// ----- shared state -----
bool g_steamNetInitialized = false;


// CVARS
cvar_t steam_fakeip = { CF_SERVER | CF_READONLY | CF_NOTIFY, "steam_fakeip", "", "Fake ip received from Steam SDK (BeginAsyncRequestFakeIP). A FakeIP is an IP that looks like a valid IPv4 address for most purposes, but comes from a reserved address space that is not used on the Internet." };
cvar_t steam_steamid = { CF_BOTH | CF_READONLY | CF_NOTIFY, "steam_steamid", "", "This is your current SteamID64." };
cvar_t steam_networking = { CF_BOTH, "steam_networking", "1", "Indicates if using Steam Datagram Relay service." };

void SteamNet_RegisterCvars()
{
    Cvar_RegisterVariable(&steam_fakeip);
    Cvar_RegisterVariable(&steam_steamid);
    Cvar_RegisterVariable(&steam_networking);

    char* buf = pSB_GetSteamIDStr();
    Cvar_SetQuick(&steam_steamid, buf);

    Cmd_AddCommand(CF_SHARED, "steam_invite", Steam_InviteFriend_f, "Invites a friend to a multiplayer game thru overlay or by their SteamID64");
    Cmd_AddCommand(CF_SHARED, "steam_drop", Net_DropSteamConnection_f, "Drops steam client from a server");
}

// ----- shared bootstrap -----

int SteamNet_Init(void) 
{
    if (!g_steamInitialized)
    {
        if (!Steam_Init()) return 0;
    }
    if (pSB_SteamNet_Init && pSB_SteamNet_Init())
    {
        g_steamNetInitialized = true;
        SteamNet_RegisterCvars();
        return 1;
    }
    return false;
}

// Stop listening for Steam/SDR connections and close server-side peers
void SteamNet_ServerStop(void)
{
    Cvar_SetQuick(&steam_fakeip, "");
    if(pSB_SteamNet_ServerStop) pSB_SteamNet_ServerStop();
}


// ----- server side -----

int SteamNet_ServerStart()
{
    int i = pSB_SteamNet_ServerStart();
    char* str = pSB_SteamNet_ServerGetFakeIP_String();
    if(str && str[0]) Cvar_SetQuick(&steam_fakeip, str);
    return i;
}



// ----- client side -----

void SteamNet_ClientDisconnect(void) //Call in NetConn_Close
{
    pSB_SteamNet_ClientDisconnect();
}

int SteamNet_ClientConnectFakeIP(const lhnetaddress_t* server_addr)
{
    return pSB_SteamNet_ClientConnectFakeIP(server_addr);
}

void SteamNet_Shutdown(void)
{
    pSB_SteamNet_Shutdown();
}

int SteamNet_Read(lhnetsocket_t* mysocket, void* data, int maxlength, lhnetaddress_t* peeraddress)
{
    return pSB_SteamNet_Read(mysocket, data, maxlength, peeraddress);
}

int SteamNet_Write(lhnetsocket_t* mysocket, const void* content, int contentlength, const lhnetaddress_t* vaddress)
{
    return pSB_SteamNet_Write(mysocket, content, contentlength, vaddress);
}


void SteamNet_Frame() // Call in NetConn_ClientFrame and NetConn_ServerFrame or better 1 call in Host_Frame;
{
    if (!g_steamInitialized) return;
    pSB_Frame();
}

bool SteamNet_IsFakeIPAddr(const lhnetaddress_t* a)
{
    if (!pSB_SteamNet_IsFakeIPAddr) return false;
    return pSB_SteamNet_IsFakeIPAddr(a);
}


void Steam_InviteFriend_f(cmd_state_t* cmd)
{
    if (Cmd_Argc(cmd) < 2)
    {
        pSB_SteamNet_InviteOverlay();
        return;
    }
    const char* str = Cmd_Argv(cmd, 1);
    pSB_SteamNet_InviteFriendToCurrentServer(str);
}
