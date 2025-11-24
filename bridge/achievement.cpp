#include "achievement.h"
#include "main.h"

namespace achievement
{
	bool Unlock(std::string ach)
	{
		bool ok = SteamUserStats()->SetAchievement(ach.c_str());
		if (!ok) { log("SetAchievement failed: " + ach); return false; }
		SteamUserStats()->StoreStats();
		log("Achievement unlocked: " + ach);
		return true;
	}

	bool Lock(std::string ach)
	{
		bool ok = SteamUserStats()->ClearAchievement(ach.c_str());
		if (!ok) { log("ClearAchievement failed: " + ach); return false; }
		SteamUserStats()->StoreStats();
		log("Achievement cleared: " + ach);
		return true;
	}
}