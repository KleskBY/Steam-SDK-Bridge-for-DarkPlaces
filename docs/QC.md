# Using Steam from QuakeC

The engine exposes three builtins and a handful of cvars and commands. This page
shows how a game uses them; the examples are trimmed from a shipped game
(ObjectN) and from Libre-life.

## Declarations

Put these in a defs file included by every VM that calls them (server, client,
menu):

```c
float(string achievement_name) steam_achievement_unlock = #645;
float(string achievement_name) steam_achievement_lock = #646;
void(string url) steam_open_url = #647;
```

Declaring them is harmless on an engine without the builtins, as long as the
code never **calls** them there: calling an unbound builtin number is a fatal
error, not a no-op.

## Is Steam up?

Two separate questions, and you usually need both:

| Question | Test | Why |
| --- | --- | --- |
| Does this engine have the builtins? | `cvar("pr_checkextension") && checkextension("KLESKBY_STEAM")` | Protects you on stock DarkPlaces or FTEQW. Server and client VMs only; the menu VM does not advertise the extension. |
| Did Steam actually start? | `strlen(cvar_string("steam_steamid")) >= 3` | The cvar is empty unless the bridge loaded and logged in. The extension test alone is **not** enough: the builtins exist even when Steam is closed. A current bridge then just returns 0 and logs a line, but bridges built before 2026-10-29 crash on an achievement call without Steam, so keep this test. |

```c
float Steam_Available()
{
	if (strlen(cvar_string("steam_steamid")) < 3)
		return FALSE;
#ifndef MENUQC
	if (!cvar("pr_checkextension"))
		return FALSE;
	if (!checkextension("KLESKBY_STEAM"))
		return FALSE;
#endif
	return TRUE;
}
```

Each test is its own `if` on purpose. An engine without Steam support has no
`steam_steamid` cvar, so it returns before the extension check; and
`checkextension` itself must not run on an engine whose `pr_checkextension` is
0. Folding them into one `&&` / `||` is only safe if your compiler
short-circuits, and fteqcc does not by default.

## Achievements

Unlock once, and remember it, so the call is not repeated every time the
condition is met. ObjectN keeps a saved cvar per achievement:

```c
void Steam_AchievementUnlock(string name)
{
	if (!Steam_Available())
		return;

	string latch = strtolower(name);	// e.g. "ach_finish_e1m1"
	if (cvar(latch))
		return;				// already unlocked on this machine

	if (steam_achievement_unlock(name))
		localcmd(strcat("seta ", latch, " 1\n"));
}
```

`name` is the achievement's **API name** from the app's Steamworks settings.
With the test app ID 480 only Spacewar's achievements exist (`ACH_WIN_ONE_GAME`,
`ACH_WIN_100_GAMES`, `ACH_HEAVY_FIRE`, `ACH_TRAVEL_FAR_ACCUM`,
`ACH_TRAVEL_FAR_SINGLE`), so use one of those to test the pipeline end to end.

### Awarding at the end of a level (CSQC)

```c
float achievements_done;

void Steam_CheckLevelEnd()			// called from CSQC_UpdateView
{
	if (!intermission || achievements_done)
		return;
	achievements_done = TRUE;

	if (mapname == "maps/e1m1.bsp")
		Steam_AchievementUnlock("ACH_FINISH_E1M1");

	if (mapname == "maps/e1m2.bsp")
	if (getstati(STAT_SECRETS) == getstati(STAT_TOTALSECRETS))
		Steam_AchievementUnlock("ACH_ALL_SECRETS_E1M2");

	if (getstati(STAT_SHOTS) >= 5)
	if (getstati(STAT_HITS) * 100 >= getstati(STAT_SHOTS) * 95)
		Steam_AchievementUnlock("ACH_ACCURACY");
}
```

### Awarding from a console command

Any client-side command can award one; ObjectN rewards opening a page and
quick-saving too often:

```c
float CSQC_ConsoleCommand(string cmd)
{
	if (cmd == "page")
	{
		Steam_AchievementUnlock("ACH_PAGE");
		Page_Toggle();
		return TRUE;
	}
	return FALSE;
}
```

### Resetting achievements while testing

Register the command once at start-up, then handle it:

```c
// CSQC_Init
registercommand("steam_resetachievements");

// CSQC_ConsoleCommand
if (cmd == "steam_resetachievements")
{
	steam_achievement_lock("ACH_FINISH_E1M1");
	steam_achievement_lock("ACH_ALL_SECRETS_E1M2");
	steam_achievement_lock("ACH_PAGE");
	localcmd("seta ach_finish_e1m1 0; seta ach_all_secrets_e1m2 0; seta ach_page 0\n");
	return TRUE;
}
```

Clearing the latch cvars as well matters; otherwise the unlock helper above
never calls Steam again on this machine.

## Menus

### Open a web page

```c
void Menu_OpenDiscord()
{
	steam_open_url("https://discord.gg/your-invite");
}
```

In current Zircon this opens the system browser and needs no Steam at all, so it
does not need the availability test.

### Invite a friend while hosting

`steam_fakeip` is set only while this machine hosts a server through the relay:

```c
if (strlen(cvar_string("steam_fakeip")) > 3 && clientcount() > 1)
	UI_Button("Invite friend", pos, UI_Cmd, "steam_invite\n");
```

`steam_invite` with no argument opens the overlay's invite dialog;
`steam_invite <steamid64>` invites one friend directly.

### "Do I need to forward a port?"

With Steam up, a hosted game is reachable through the relay at `steam_fakeip`.
Without it, players must forward the UDP port. Libre-life's Create Server page
shows the hint only in the second case:

```c
if (cvar("ui_cs_public") && strlen(cvar_string("steam_steamid")) < 3)
{
	UI_Text(strcat("Forward UDP port ", ftos(cvar("port"))), pos, UI_TEXT_INACTIVE);
	UI_Text("to be listed online.", pos + [0, lineHeight, 0], UI_TEXT_INACTIVE);
}
```

### Show the address to share

```c
string addr = cvar_string("steam_fakeip");
if (addr != "")
	UI_Text(strcat("Friends can connect to ", addr), pos, UI_TEXT_NORMAL);
```

## From server QC

The server VM has the same builtins, but achievements belong to the player at
the keyboard, and in a listen server or dedicated server the server process is
not that player. Award achievements from client QC (or tell the client to, with
`stuffcmd` and a client-side command as above), not by calling
`steam_achievement_unlock` in server code.
