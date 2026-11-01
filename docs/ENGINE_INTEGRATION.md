# Engine integration

What the engine side of SteamBridge does, where it hooks in, and what it adds
for players and QuakeC. File and function names are Zircon's; other DarkPlaces
trees use the same names unless noted.

## Which copy to use

**Current Zircon already contains the integration**: `steam.c`, `steam.h`,
`steam_net.c`, `steam_net.h` and `prvm_steam.h` sit at the root of the engine
tree, and the Visual Studio projects with `steam` in their name define the
switches below. Use those. They load the DLL through the engine's own
`Sys_LoadDependency` with a `dllfunction_t` table, and they check
`steam_appid.txt` before loading anything.

`engine_source/kleskby/` in this repository is the original patch against Zircon
Beta 20251028 (`LoadLibraryA` + `GetProcAddress`, one call per symbol). It is
kept as a reference for porting the integration into an older or a different
DarkPlaces tree.

## Build switches

`steam.c` (start-up, frame, shutdown, achievements, URL) is part of every Zircon
project, Steam or not; without the DLL and `steam_appid.txt` it does nothing.

| Define | Effect |
| --- | --- |
| `STEAM_NETWORKING` | Turns on the networking hooks in `netconn.c`, `lhnet.c` and `cl_main.c`, and `steam_net.c` is added to the project. Without it the engine never opens a FakeIP socket. Set by the Steam-enabled projects, e.g. `#zircon_libre_life_steam_visual_studio_2022.vcxproj`. |
| `STEAM` | Set alongside it by the Visual Studio projects. Current sources never test it. |

Neither adds a library to link. The executable stays free of Steam symbols
whether or not the DLL is present.

## Hook points

| Where | Calls | Purpose |
| --- | --- | --- |
| `NetConn_InitOnce` | `SteamNet_Init` → `Steam_Init` | Runs first, because networking starts before the rest of the host. `Steam_Init` checks `steam_appid.txt`, loads the DLL, resolves the 19 `SB_*` functions, calls `SB_Init`, registers the console-command callback, and sets `net_sourceaddresscheck 0`. `SteamNet_Init` then registers the cvars and commands below. |
| end of `Host_InitOnce` | `Steam_Init` | Second call; returns at once if Steam is already up. |
| `Host_Frame`, first line | `Steam_Frame` → `SB_Frame` | Runs Steam callbacks every frame. |
| `NetConn_OpenServerPorts` | `NetConn_OpenServerPort(…, LHNETADDRESSTYPE_STEAM_FAKEIP, port + 1)` then `SteamNet_ServerStart` | A server with network ports (`maxplayers` > 1) listens on Steam alongside UDP. |
| `NetConn_CloseServerPorts` | `SteamNet_ServerStop` | |
| `NetConn_OpenClientPorts` | `NetConn_OpenClientPort(…, LHNETADDRESSTYPE_STEAM_FAKEIP, …)` | Gives the client a FakeIP-typed socket. |
| `cl_main.c`, connect | `SteamNet_ClientConnectFakeIP` | When the target address is a FakeIP. |
| `NetConn_Close` | `SteamNet_ClientDisconnect` | |
| `LHNETADDRESS_FromString` (`lhnet.c`) | `SteamNet_IsFakeIPAddr` | A typed `connect a.b.c.d:port` whose IP is in Steam's FakeIP range is retagged `LHNETADDRESSTYPE_STEAM_FAKEIP`. |
| `LHNET_Read` / `LHNET_Write` | `SteamNet_Read` / `SteamNet_Write` | Packets on a FakeIP socket go through Steam instead of `recvfrom` / `sendto`. |
| `NetConn_Shutdown` | `SteamNet_Shutdown` | |
| `Host_Shutdown` | `Steam_Shutdown` | |

`net_sourceaddresscheck` is forced to 0 because replies over the relay do not
come from the address the client dialled.

## Console variables

| Cvar | Flags | Meaning |
| --- | --- | --- |
| `steam_steamid` | read-only | Your SteamID64. **Empty unless Steam started successfully**, which makes it the test for "is Steam up" in QuakeC and configs. |
| `steam_fakeip` | read-only, server | This server's FakeIP `a.b.c.d:port` once the relay hands one out; empty when not hosting. |
| `steam_networking` | | Default `1`. Registered for configs and menus; current Zircon does not read it. |
| `developer_steam` | | `1` prints the networking library's messages (the `steam_echo` command). |

All but `developer_steam` exist only after `SteamNet_Init` succeeds, so on an
engine or install without Steam they read as empty / 0.

## Console commands

| Command | Does |
| --- | --- |
| `steam_invite` | Opens the overlay's invite dialog for the current server. |
| `steam_invite <steamid64>` | Sends a game invite to that friend directly. |
| `steam_drop <hConn> "<reason>"` | Sent by the bridge when a Steam peer disconnects; drops the client on that connection. Not meant for players. |
| `steam_echo <text>` | Sent by the bridge; prints only with `developer_steam 1`. |

Players connect to a Steam-hosted server with plain `connect`, using the address
in the host's `steam_fakeip`. Friends can also use "Join game" in the Steam
friends list, or accept an invite; both arrive as a `connect` the bridge issues.

## QuakeC builtins

Registered in all three VMs (server, client, menu) at the same numbers. The
extension name `KLESKBY_STEAM` is advertised to the server and client VMs only;
the menu VM's extension list does not include it.

| # | Prototype | Works in |
| --- | --- | --- |
| 645 | `float(string achievement_name) steam_achievement_unlock` | all three |
| 646 | `float(string achievement_name) steam_achievement_lock` | all three |
| 647 | `void(string url) steam_open_url` | builds with `CONFIG_MENU`; a no-op otherwise |

`steam_open_url` in current Zircon opens the page with the system browser
(`Sys_Open_URL`), not the Steam overlay. The bridge's `SB_OpenUrl` export can do
the overlay version if an engine wants it.

In server and client QC, `checkextension("KLESKBY_STEAM")` is true on any engine
compiled with the builtins, **whether or not Steam started**. Combine it with
the `steam_steamid` test; in menu QC, use the `steam_steamid` test alone. See
[QC.md](QC.md) for usage and examples.

## Porting to another DarkPlaces tree

1. Copy `steam.c`/`.h`, `steam_net.c`/`.h` and `prvm_steam.h` from a current
   Zircon tree (or `engine_source/kleskby/` for the older loader style).
2. Add the hook calls from the table above. Each one in Zircon is wrapped in
   `#ifdef STEAM_NETWORKING`; search the Zircon tree for `KLESKBY` and
   `STEAM_NETWORKING` to find them.
3. Add `LHNETADDRESSTYPE_STEAM_FAKEIP` to the end of `lhnetaddresstype_t`, and
   make the engine's native address struct match the layout in
   [API.md](API.md#lhnetaddress_t-is-shared-memory-layout).
4. Register builtins 645–647 in `svvm_cmds_table.h`, `clvm_cmds_table.h` and
   `mvm_cmds_table.h`, and add `KLESKBY_STEAM` to `vm_sv_extensions` in
   `svvm_cmds.c`.
5. Define `STEAM_NETWORKING` in the project, build, and deploy the three files
   listed in the [README](../README.md#deploying-with-a-game).
