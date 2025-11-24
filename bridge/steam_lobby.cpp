#include "steam_lobby.h"
#include "steam_net.h"
#include "steam.h"
#include "main.h"

#include <public/steam/steam_api.h>
#include <public/steam/isteamnetworkingsockets.h>
#include <public/steam/isteamnetworkingutils.h>
#include <public/steam/steamnetworkingfakeip.h>

///////////////////////////
//////// FAKE LOBBY ///////
///////////////////////////
CSteamID g_currentLobby;

void SteamNet_CreateLobby()
{
    if (!SteamMatchmaking()) return;
    SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 16);
}

void SteamNet_DestroyLobby()
{
    if (!SteamMatchmaking()) return;
    if (g_currentLobby.IsValid())
        SteamMatchmaking()->LeaveLobby(g_currentLobby);
    g_currentLobby.Clear();
}

void SteamNet_InviteOverlay()
{
    if (!SteamFriends()) return;
    if (!g_currentLobby.IsValid())
    {
        log("No valid lobby\n");
        return;
    }
    SteamFriends()->ActivateGameOverlayInviteDialog(g_currentLobby);
}


class CLobbyCallback
{
public:
    CLobbyCallback() : m_LobbyCreated(this, &CLobbyCallback::OnLobbyCreated)
    {
    }
    STEAM_CALLBACK(CLobbyCallback, OnLobbyCreated, LobbyCreated_t, m_LobbyCreated);
};

void CLobbyCallback::OnLobbyCreated(LobbyCreated_t* p)
{
    if (p->m_eResult == k_EResultOK)
    {
        CSteamID lobbyID = p->m_ulSteamIDLobby;
        // Save it globally
        g_currentLobby = lobbyID;

        // Build connect string
        const char* addr = SteamNet_ServerGetFakeIP_String();
        if (!addr || !addr[0])
        {
            log("steam_invite: steam_fakeip is empty");
            return;
        }

        printf("Lobby created: id=%llu\n", (unsigned long long)lobbyID.ConvertToUint64());
        SteamMatchmaking()->SetLobbyData(lobbyID, "connect", addr);
    }
}

static CLobbyCallback g_LobbyCallback;





class CJoinRequestHandler
{
public:
    CJoinRequestHandler()
        : m_OnGameLobbyJoinRequested(this, &CJoinRequestHandler::OnGameLobbyJoinRequested)
        , m_OnLobbyEnter(this, &CJoinRequestHandler::OnLobbyEnter)
    {
    }
    STEAM_CALLBACK(CJoinRequestHandler, OnGameLobbyJoinRequested, GameLobbyJoinRequested_t, m_OnGameLobbyJoinRequested);
    STEAM_CALLBACK(CJoinRequestHandler, OnLobbyEnter, LobbyEnter_t, m_OnLobbyEnter);
};

void CJoinRequestHandler::OnGameLobbyJoinRequested(GameLobbyJoinRequested_t* p)
{
    CSteamID lobby(p->m_steamIDLobby);
    printf("Steam: GameLobbyJoinRequested for lobby %llu\n", (unsigned long long)lobby.ConvertToUint64());
    SteamMatchmaking()->RequestLobbyData(p->m_steamIDLobby);
    const char* addr = SteamMatchmaking()->GetLobbyData(p->m_steamIDLobby, "connect");
    if (!addr || !addr[0])
    {
        SteamMatchmaking()->JoinLobby(lobby);
    }
    else
    {
        Steam_Connect(addr);
    }
}
void CJoinRequestHandler::OnLobbyEnter(LobbyEnter_t* p)
{
    CSteamID lobby(p->m_ulSteamIDLobby);
    if (p->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
    {
        printf("Steam: failed to enter lobby %llu (resp %d)\n", (unsigned long long)lobby.ConvertToUint64(), (int)p->m_EChatRoomEnterResponse);
        return;
    }

    CSteamID me = SteamUser()->GetSteamID();
    CSteamID owner = SteamMatchmaking()->GetLobbyOwner(lobby);
    if (me == owner)
    {
        log("Steam: entered own lobby not auto-connecting");
        return;
    }

    // Now we're a client joining somebody else's lobby: grab the connect string.
    const char* addr = SteamMatchmaking()->GetLobbyData(lobby, "connect");
    Steam_Connect(addr);
}

static CJoinRequestHandler g_JoinRequest;









//Not a lobby, direct invite

class CSteamInviteHandler
{
public:
    CSteamInviteHandler() : m_GameJoinRequested(this, &CSteamInviteHandler::OnGameRichPresenceJoinRequested)
    {
    }

private:
    STEAM_CALLBACK(CSteamInviteHandler, OnGameRichPresenceJoinRequested, GameRichPresenceJoinRequested_t, m_GameJoinRequested);
};

static CSteamInviteHandler g_SteamInviteHandler;

void CSteamInviteHandler::OnGameRichPresenceJoinRequested(GameRichPresenceJoinRequested_t* pParam)
{
    const char* pchConnect = pParam->m_rgchConnect; // connect string sender used
    uint64 id64 = pParam->m_steamIDFriend.ConvertToUint64();
    log("Steam invite accepted, connect string: " + std::string(pchConnect));
    Steam_Connect(pchConnect);
}




void SteamNet_InviteFriendToCurrentServer(const char* str)
{
    if (!str || !*str)
    {
        log("steam_invite: empty SteamID string");
        return;
    }

    if (!SteamAPI_IsSteamRunning() || !SteamUser() || !SteamFriends())
    {
        log("steam_invite: Steam API not available");
        return;
    }

    // Parse SteamID64
    char* end = NULL;
    uint64 id64 = strtoull(str, &end, 10);
    if (!end || *end != '\0' || id64 == 0)
    {
        log("steam_invite: (" + std::string(str) + ") is not a valid steamid64");
        return;
    }

    CSteamID steamid(id64);
    if (!steamid.IsValid())
    {
        log("steam_invite: invalid SteamID64 " + std::string(str));
        return;
    }

    const char* addr = SteamNet_ServerGetFakeIP_String();
    if (!addr || !addr[0])
    {
        log("steam_invite: steam_fakeip is empty");
        return;
    }
    // Send invite
    bool ok = SteamFriends()->InviteUserToGame(steamid, addr);
}