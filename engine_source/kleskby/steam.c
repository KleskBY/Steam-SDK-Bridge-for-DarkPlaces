

#include "../darkplaces.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "steam.h"

#include <Windows.h>


HMODULE SteamDLL;
PFN_SB_Init     pSB_Init;
PFN_SB_Exec     pSB_Exec;
PFN_SB_Frame    pSB_Frame;
PFN_SB_Shutdown pSB_Shutdown;
PFN_SB_GetSteamIDStr pSB_GetSteamIDStr;
PFN_SB_Achivement_Unlock pSB_Achivement_Unlock;
PFN_SB_Achivement_Lock pSB_Achivement_Lock;

PFN_SB_SteamNet_Init         pSB_SteamNet_Init;
PFN_SB_SteamNet_ServerStop   pSB_SteamNet_ServerStop;
PFN_SB_SteamNet_ServerStart  pSB_SteamNet_ServerStart;
PFN_SB_SteamNet_ServerGetFakeIP_String pSB_SteamNet_ServerGetFakeIP_String;
PFN_SB_SteamNet_ClientDisconnect       pSB_SteamNet_ClientDisconnect;
PFN_SB_SteamNet_ClientConnectFakeIP    pSB_SteamNet_ClientConnectFakeIP;
PFN_SB_SteamNet_Shutdown               pSB_SteamNet_Shutdown;
PFN_SB_SteamNet_Read   pSB_SteamNet_Read;
PFN_SB_SteamNet_Write  pSB_SteamNet_Write;
PFN_SB_SteamNet_IsFakeIPAddr pSB_SteamNet_IsFakeIPAddr;
PFN_SB_SteamNet_InviteOverlay pSB_SteamNet_InviteOverlay;
PFN_SB_SteamNet_InviteFriendToCurrentServer pSB_SteamNet_InviteFriendToCurrentServer;
PFN_SB_RegisterEngineCmdExec pSB_RegisterEngineCmdExec;

static void SteamBridge_ExecFromDLL(const char* text)
{
	if (!text || !text[0]) return;
	Cmd_ExecuteString(cmd_local, text, src_local, false);
}

static bool LoadSteamFunc(void** out, const char* name)
{
	*out = (void*)GetProcAddress(SteamDLL, name);
	if (!*out)
	{
		Con_PrintLinef(CON_ERROR "Steam: missing required symbol '%s'", name);
		return false;
	}
	return true;
}

bool Steam_Init()
{
	if (g_steamInitialized) return true;

	const char* library_name = "steambridge.dll";
#ifdef _WIN64
	library_name = "steambridge64.dll";
#endif

	if (!FS_SysFileExists(library_name))
	{
		Con_PrintLinef("Steam: missing steam files (%s), skipping steam.", library_name);
		return false;
	}
	if (!FS_SysFileExists("steam_appid.txt"))
	{
		Con_PrintLinef("Steam: missing steam_appid.txt, skipping steam.");
		return false;
	}

	SteamDLL = LoadLibraryA(library_name);
	if (!SteamDLL)
	{
		Con_PrintLinef("Steam: Error loading bridge %d", GetLastError());
		return false;
	}

	
	bool ok = true;
	ok &= LoadSteamFunc((void**)&pSB_Init, "SB_Init");
	ok &= LoadSteamFunc((void**)&pSB_Frame, "SB_Frame");
	ok &= LoadSteamFunc((void**)&pSB_Shutdown, "SB_Shutdown");
	ok &= LoadSteamFunc((void**)&pSB_GetSteamIDStr, "SB_GetSteamIDStr");
	ok &= LoadSteamFunc((void**)&pSB_RegisterEngineCmdExec, "SB_RegisterEngineCmdExec");
	ok &= LoadSteamFunc((void**)&pSB_Achivement_Unlock, "SB_Achivement_Unlock");
	ok &= LoadSteamFunc((void**)&pSB_Achivement_Lock, "SB_Achivement_Lock");

	// networking
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_Init, "SB_SteamNet_Init");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_ServerStop, "SB_SteamNet_ServerStop");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_ServerStart, "SB_SteamNet_ServerStart");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_ServerGetFakeIP_String, "SB_SteamNet_ServerGetFakeIP_String");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_ClientDisconnect, "SB_SteamNet_ClientDisconnect");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_ClientConnectFakeIP, "SB_SteamNet_ClientConnectFakeIP");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_Shutdown, "SB_SteamNet_Shutdown");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_Read, "SB_SteamNet_Read");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_Write, "SB_SteamNet_Write");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_IsFakeIPAddr, "SB_SteamNet_IsFakeIPAddr");

	// overlay / invites – probably optional
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_InviteOverlay, "SB_SteamNet_InviteOverlay");
	ok &= LoadSteamFunc((void**)&pSB_SteamNet_InviteFriendToCurrentServer, "SB_SteamNet_InviteFriendToCurrentServer");
	if (!ok)
	{
		Con_PrintLinef(CON_ERROR "Steam: missing required Steam bridge functions, aborting init");
		return false;
	}

	if (!pSB_Init())
	{
		Con_PrintLinef("Steam: Error initializing steam");
		return false;
	}

	if (pSB_RegisterEngineCmdExec) pSB_RegisterEngineCmdExec(SteamBridge_ExecFromDLL);

	Con_PrintLinef("Steam: successfully initialized");
	g_steamInitialized = true;
	return true;
}

void Steam_Frame()
{
	if (!g_steamInitialized) return;
	pSB_Frame(); //SteamAPI_RunCallbacks();
}

void Steam_Shutdown()
{
	if (!g_steamInitialized) return;
	pSB_Shutdown(); // SteamAPI_Shutdown();
	g_steamInitialized = false;
}

int Steam_Achivment_Unlock(const char* name)
{
	if (!name || !name[0]) return 0;
	if (!pSB_Achivement_Unlock) return 0;
	return pSB_Achivement_Unlock(name);
}

int Steam_Achivment_Lock(const char* name)
{
	if (!name || !name[0]) return 0;
	if (!pSB_Achivement_Lock) return 0;
	return pSB_Achivement_Lock(name);
}

void Steam_OpenUrl(const char* url)
{
#ifdef _WIN32
	if (!url || !url[0])
		return;
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#endif
}