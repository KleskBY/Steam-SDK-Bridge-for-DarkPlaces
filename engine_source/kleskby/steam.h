#pragma once
#ifndef KLESKBY
#define KLESKBY

static bool g_steamInitialized = false;
bool Steam_Init(); // Called once in Host_InitOnce 
void Steam_Frame();
void Steam_Shutdown();

int Steam_Achivment_Unlock(const char* name);
int Steam_Achivment_Lock(const char* name);
void Steam_OpenUrl(const char* url);

typedef int(__cdecl* PFN_SB_Init)(void);
typedef void(__cdecl* PFN_SB_Exec)(const char*);
typedef void(__cdecl* PFN_SB_Frame)(void);
typedef void(__cdecl* PFN_SB_Shutdown)(void);
typedef char*(__cdecl* PFN_SB_GetSteamIDStr)(void);
typedef int(__cdecl* PFN_SB_SteamNet_Init)(void);
typedef void(__cdecl* PFN_SB_SteamNet_ServerStop)(void);
typedef int(__cdecl* PFN_SB_SteamNet_ServerStart)(void);
typedef char* (__cdecl* PFN_SB_SteamNet_ServerGetFakeIP_String)(void);
typedef void(__cdecl* PFN_SB_SteamNet_ClientDisconnect)(void);
typedef int(__cdecl* PFN_SB_SteamNet_ClientConnectFakeIP)(const lhnetaddress_t* server_addr);
typedef void(__cdecl* PFN_SB_SteamNet_Shutdown)(void);
typedef int(__cdecl* PFN_SB_SteamNet_Read)(lhnetsocket_t* mysocket, void* data, int maxlength, lhnetaddress_t* peeraddress);
typedef int(__cdecl* PFN_SB_SteamNet_Write)(lhnetsocket_t* mysocket, const void* content, int contentlength, const lhnetaddress_t* vaddress);
typedef int(__cdecl* PFN_SB_SteamNet_IsFakeIP)(unsigned __int32 ipHostOrder);
typedef int(__cdecl* PFN_SB_SteamNet_IsFakeIPAddr)(const lhnetaddress_t* a);
typedef void(__cdecl* PFN_SB_SteamNet_InviteOverlay)(void);
typedef void(__cdecl* PFN_SB_SteamNet_InviteFriendToCurrentServer)(const char* str);
typedef void(__cdecl* PFN_SB_RegisterEngineCmdExec)(void (*fn)(const char*));
typedef int(__cdecl* PFN_SB_Achivement_Unlock)(const char* line);
typedef int(__cdecl* PFN_SB_Achivement_Lock)(const char* line);

extern HMODULE SteamDLL;
extern PFN_SB_Init     pSB_Init;
extern PFN_SB_Exec     pSB_Exec;
extern PFN_SB_Frame    pSB_Frame;
extern PFN_SB_Shutdown pSB_Shutdown;
extern PFN_SB_GetSteamIDStr pSB_GetSteamIDStr;
extern PFN_SB_Achivement_Unlock pSB_Achivement_Unlock;
extern PFN_SB_Achivement_Unlock pSB_Achivement_Lock;

extern PFN_SB_SteamNet_Init         pSB_SteamNet_Init;
extern PFN_SB_SteamNet_ServerStop   pSB_SteamNet_ServerStop;
extern PFN_SB_SteamNet_ServerStart  pSB_SteamNet_ServerStart;
extern PFN_SB_SteamNet_ServerGetFakeIP_String pSB_SteamNet_ServerGetFakeIP_String;
extern PFN_SB_SteamNet_ClientDisconnect       pSB_SteamNet_ClientDisconnect;
extern PFN_SB_SteamNet_ClientConnectFakeIP    pSB_SteamNet_ClientConnectFakeIP;
extern PFN_SB_SteamNet_Shutdown               pSB_SteamNet_Shutdown;
extern PFN_SB_SteamNet_Read   pSB_SteamNet_Read;
extern PFN_SB_SteamNet_Write  pSB_SteamNet_Write;
extern PFN_SB_SteamNet_IsFakeIPAddr pSB_SteamNet_IsFakeIPAddr;
extern PFN_SB_SteamNet_InviteOverlay pSB_SteamNet_InviteOverlay;
extern PFN_SB_SteamNet_InviteFriendToCurrentServer pSB_SteamNet_InviteFriendToCurrentServer;
extern PFN_SB_RegisterEngineCmdExec pSB_RegisterEngineCmdExec;

#endif