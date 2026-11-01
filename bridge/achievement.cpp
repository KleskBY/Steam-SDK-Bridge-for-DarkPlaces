#include "achievement.h"
#include "main.h"

namespace achievement
{
	static bool Ready(const std::string& ach)
	{
		if (g_steamInitialized && SteamUserStats()) return true;
		log("Steam not initialized, ignoring achievement: " + ach);
		return false;
	}

	bool Unlock(std::string ach)
	{
		if (!Ready(ach)) return false;
		bool ok = SteamUserStats()->SetAchievement(ach.c_str());
		if (!ok) { log("SetAchievement failed: " + ach); return false; }
		SteamUserStats()->StoreStats();
		log("Achievement unlocked: " + ach);
		return true;
	}

	bool Lock(std::string ach)
	{
		if (!Ready(ach)) return false;
		bool ok = SteamUserStats()->ClearAchievement(ach.c_str());
		if (!ok) { log("ClearAchievement failed: " + ach); return false; }
		SteamUserStats()->StoreStats();
		log("Achievement cleared: " + ach);
		return true;
	}
}