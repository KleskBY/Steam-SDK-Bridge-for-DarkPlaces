#include "steam.h"

#include <public/steam/steam_api.h>
#include "main.h"

bool g_steamInitialized = false;

void (*g_ExecFromEngine)(const char* cmd) = nullptr;

bool Steam_Init()
{
	if (!SteamAPI_Init())
	{
		log("SteamAPI_Init error");
		return false;
	}

	if (!SteamUser()->BLoggedOn())
	{
		log("Steam: Steam user is not logged in");
		return false;
	}

	log("Steam: successfully initialized");

	g_steamInitialized = true;
	return true;
}

void Steam_Frame()
{
	if (!g_steamInitialized) return;
	SteamAPI_RunCallbacks();
}

void Steam_Shutdown()
{
	if (!g_steamInitialized) return;
	SteamAPI_Shutdown();
	g_steamInitialized = false;
}

char* Steam_GetSteamIDStr()
{
	static char buffer[32] = "";  // static so it stays alive
	buffer[0] = '\0';

	if (!g_steamInitialized || !SteamUser())
		return buffer;

	uint64 id64 = SteamUser()->GetSteamID().ConvertToUint64();
	snprintf(buffer, sizeof(buffer), "%llu", (unsigned long long)id64);
	return buffer;
}

void Steam_Connect(const char* addr)
{
	if (!g_ExecFromEngine || !addr || !addr[0])
		return;

	char cmd[128];
	snprintf(cmd, sizeof(cmd), "connect %s", addr);
	g_ExecFromEngine(cmd);
}

void Steam_OpenUrl(const char* url)
{
#ifdef _WIN32
	if (!url || !url[0])
		return;
	if (!SteamFriends()) return;
	SteamFriends()->ActivateGameOverlayToWebPage(url, k_EActivateGameOverlayToWebPageMode_Default);
#endif
}