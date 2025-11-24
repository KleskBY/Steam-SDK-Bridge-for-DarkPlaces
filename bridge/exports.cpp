#include "exports.h"
#include "steam.h"
#include "steam_net.h"
#include "steam_lobby.h"
#include "achievement.h"
extern "C" 
{
    //BASE
    SB_API int SB_Init() 
    {
        return Steam_Init();
    }

    SB_API void SB_Frame(void) 
    {
        Steam_Frame();
    }

    SB_API void SB_Shutdown(void) 
    {
        Steam_Shutdown();
    }

    SB_API char* SB_GetSteamIDStr(void)
    {
        return Steam_GetSteamIDStr();
    }

    SB_API void SB_RegisterEngineCmdExec(void (*fn)(const char*))
    {
        g_ExecFromEngine = fn;
    }


    //Achivement
    SB_API bool SB_Achivement_Unlock(const char* line)
    {
        return achievement::Unlock(line);
    }

    SB_API bool SB_Achivement_Lock(const char* line)
    {
        return achievement::Lock(line);
    }



    //NETWORK
    SB_API int SB_SteamNet_Init(void)
    {
        return SteamNet_Init();
    }

    SB_API void SB_SteamNet_ServerStop(void)
    {
        SteamNet_ServerStop();
    }

    SB_API int SB_SteamNet_ServerStart(void)
    {
        return SteamNet_ServerStart();
    }

    SB_API const char* SB_SteamNet_ServerGetFakeIP_String()
    {
        return SteamNet_ServerGetFakeIP_String();
    }

    SB_API void SB_SteamNet_ClientDisconnect(void)
    {
        SteamNet_ClientDisconnect();
    }

    SB_API int SB_SteamNet_ClientConnectFakeIP(const lhnetaddress_t* server_addr)
    {
        return SteamNet_ClientConnectFakeIP(server_addr);
    }
    
    SB_API void SB_SteamNet_Shutdown(void)
    {
        SteamNet_Shutdown();
    }

    SB_API int SB_SteamNet_Read(lhnetsocket_t* mysocket, void* data, int maxlength, lhnetaddress_t* peeraddress)
    {
        return SteamNet_Read(mysocket, data, maxlength, peeraddress);
    }

    SB_API int SB_SteamNet_Write(lhnetsocket_t* mysocket, const void* content, int contentlength, const lhnetaddress_t* vaddress)
    {
        return SteamNet_Write(mysocket, content, contentlength, vaddress);
    }

    SB_API bool SB_SteamNet_IsFakeIPAddr(const lhnetaddress_t* a)
    {
        return SteamNet_IsFakeIPAddr(a);
    }

    SB_API void SB_SteamNet_InviteOverlay()
    {
        SteamNet_InviteOverlay();
    }
    
    SB_API void SB_SteamNet_InviteFriendToCurrentServer(const char* str)
    {
        SteamNet_InviteFriendToCurrentServer(str);
    }

};