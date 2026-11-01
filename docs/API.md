# Exported API

Every function `steambridge64.dll` exports, from `bridge/exports.cpp`. All are
`extern "C"`, `__cdecl` (the only convention on x64), and resolved by the engine
by exact name, so **the names are a contract**: the misspelling `Achivement` is
load-bearing. Renaming an export breaks every engine built against it, with no
link error, only `missing function` at load time.

The engine resolves 19 of the 20. `SB_OpenUrl` is optional; current Zircon opens
URLs through its own `Sys_Open_URL` instead.

## Call order

```
SB_Init()                       once, at start-up; nothing else is safe before it succeeds
SB_RegisterEngineCmdExec(fn)    right after, so bridge output reaches the console
SB_SteamNet_Init()              once, if the engine wants Steam networking
  per frame:  SB_Frame()        drives every Steam callback below
  server:     SB_SteamNet_ServerStart() ... SB_SteamNet_ServerStop()
  client:     SB_SteamNet_ClientConnectFakeIP(addr) ... SB_SteamNet_ClientDisconnect()
SB_SteamNet_Shutdown()
SB_Shutdown()                   once, at exit
```

All calls must come from the engine's main thread. Steam callbacks (lobby
created, invite accepted, peer dropped) only fire inside `SB_Frame`, so their
effects arrive as console commands during that call.

## Core

| Export | Returns | Behaviour |
| --- | --- | --- |
| `int SB_Init(void)` | 1 ok, 0 fail | `SteamAPI_Init()`, then requires `SteamUser()->BLoggedOn()`. Fails when the Steam client is not running, the user is offline, or `steam_appid.txt` names an app the account cannot run. Idempotent only on the engine side (the engine skips it once initialised). |
| `void SB_Frame(void)` | | `SteamAPI_RunCallbacks()` if initialised, else nothing. Call every host frame. |
| `void SB_Shutdown(void)` | | `SteamAPI_Shutdown()` if initialised. |
| `char *SB_GetSteamIDStr(void)` | SteamID64 as decimal text | Points at a static buffer inside the DLL; copy it before the next call. Empty string when not initialised. |
| `void SB_RegisterEngineCmdExec(void (*fn)(const char *))` | | Gives the bridge a way to run console commands in the engine. See [Commands the bridge sends](#commands-the-bridge-sends). Until it is set, bridge messages go to `stdout`. |
| `void SB_OpenUrl(const char *url)` | | Opens `url` in the Steam overlay's browser (`ActivateGameOverlayToWebPage`). No-op for an empty string. |

## Achievements

| Export | Returns | Behaviour |
| --- | --- | --- |
| `int SB_Achivement_Unlock(const char *name)` | 1 if set, else 0 | `SetAchievement(name)` then `StoreStats()`, so the unlock is uploaded and the overlay toast shows immediately. Logs `Achievement unlocked: NAME` or `SetAchievement failed: NAME`. Returns 0 for a NULL or empty name, and returns 0 with `Steam not initialized, ignoring achievement: NAME` if `SB_Init` never succeeded. |
| `int SB_Achivement_Lock(const char *name)` | 1 if cleared, else 0 | `ClearAchievement(name)` then `StoreStats()`. For testing; logs `Achievement cleared: NAME`. Same guards as unlock. |

`name` is the achievement's **API name** as configured for the app in
Steamworks, not its display name. A name the app does not define fails.

## Networking (Steam Datagram Relay, FakeIP)

The engine keeps using its own UDP code paths. A socket whose address type is
`LHNETADDRESSTYPE_STEAM_FAKEIP` is routed to `SB_SteamNet_Read` /
`SB_SteamNet_Write` instead of `recvfrom` / `sendto`, so the netcode above it
does not know the difference.

| Export | Returns | Behaviour |
| --- | --- | --- |
| `int SB_SteamNet_Init(void)` | 1 ok, 0 fail | Calls `Steam_Init` if needed, gets `SteamNetworkingSockets()`, starts relay access (`InitRelayNetworkAccess`), asks Steam for one FakeIP port (`BeginAsyncRequestFakeIP(1)`). The FakeIP arrives asynchronously, a few frames later. |
| `int SB_SteamNet_ServerStart(void)` | 1 listening, 0 fail | Creates the listen socket on FakeIP port index 0 (`CreateListenSocketP2PFakeIP`) and a poll group, then creates a public lobby (16 slots) whose `connect` data is the FakeIP. Incoming connections are accepted automatically; the bridge tracks up to 64 peers. Returns 1 without change if already listening. |
| `const char *SB_SteamNet_ServerGetFakeIP_String(void)` | `"a.b.c.d:port"` or NULL | NULL until the FakeIP request completes. When it returns a string it also sets rich presence `connect` = `+connect a.b.c.d:port` and `status` = `On server`, which is what makes "Join game" work from the friends list. Static buffer. |
| `void SB_SteamNet_ServerStop(void)` | | Clears rich presence, closes every server-side peer, destroys the poll group and listen socket. Keeps the Steam API and any client connection alive. |
| `int SB_SteamNet_ClientConnectFakeIP(const lhnetaddress_t *addr)` | 1 connecting, 0 fail | Accepts an `INET4` or `STEAM_FAKEIP` address, closes any previous client connection, then `ConnectByIPAddress`. The connection completes in later frames. |
| `void SB_SteamNet_ClientDisconnect(void)` | | Closes the client connection, if any. |
| `int SB_SteamNet_Read(lhnetsocket_t *sock, void *data, int maxlen, lhnetaddress_t *peer)` | bytes read, 0 if none | Receives **one** message (from the client connection when connected as a client, else from the server poll group), truncating to `maxlen`. Fills `peer` with the sender's FakeIP and stashes the connection handle and SteamID64 in it for the reply. |
| `int SB_SteamNet_Write(lhnetsocket_t *sock, const void *data, int len, const lhnetaddress_t *peer)` | `len` sent, 0 buffer full, -1 error | Unreliable, no Nagle: the engine's own netcode handles reliability, exactly as over UDP. Uses the client connection when connected as a client, else the connection handle stored in `peer` by `SB_SteamNet_Read`. |
| `int SB_SteamNet_IsFakeIPAddr(const lhnetaddress_t *addr)` | 1 or 0 | 1 if `addr` is an `INET4`/`STEAM_FAKEIP` address in Steam's FakeIP range. The engine uses it to retag a typed `connect a.b.c.d:port` as a FakeIP. |
| `void SB_SteamNet_Shutdown(void)` | | `ServerStop` + `ClientDisconnect`, then forgets the sockets interface. |

### `lhnetaddress_t` is shared memory layout

The networking calls take the engine's own `lhnetaddress_t` / `lhnetsocket_t`
by pointer, and `bridge/steam_net.h` re-declares those structs. The bridge reads
and writes the address through this "native" view, inside what the engine sees
as opaque `storage` bytes:

| Offset (x64) | Field |
| --- | --- |
| 0 | `addresstype` (enum, order `NONE, LOOP, INET4, INET6, STEAM_FAKEIP`) |
| 4 | `port` |
| 8 | 16-byte union of `sockaddr` / `sockaddr_in` |
| 24 | `hConn`, the Steam connection handle (`uintptr_t`) |
| 32 | `steamid`, the peer's SteamID64 |

On the engine side the matching declaration is `lhnetaddressnative_t` in
`netconn.c`, whose union drops `sockaddr_in6` when `STEAM_NETWORKING` is
defined precisely so `hConn` lands at offset 24. (`lhnet.c` has its own copy
with `sockaddr_in6` still in the union; it never touches the two Steam fields,
so the difference is harmless today, but do not start reading them there.)
Change the enum order or either struct and the bridge must be rebuilt to match.

## Overlay and invites

| Export | Behaviour |
| --- | --- |
| `void SB_SteamNet_InviteOverlay(void)` | Opens the overlay's invite dialog for the current lobby. Logs `No valid lobby` when not hosting. |
| `void SB_SteamNet_InviteFriendToCurrentServer(const char *steamid64)` | Sends a game invite whose connect string is this server's FakeIP. Validates the SteamID64 and requires a FakeIP (`steam_invite: steam_fakeip is empty` otherwise). |

How a friend gets in, all handled inside the bridge:

- **Invite accepted** (`GameRichPresenceJoinRequested_t`): runs `connect <fakeip>`.
- **"Join game" from the friends list** (`GameLobbyJoinRequested_t`): reads the
  lobby's `connect` data and runs `connect <fakeip>`, or joins the lobby first
  if the data has not arrived.
- **Lobby entered** (`LobbyEnter_t`) as a non-owner: runs `connect <fakeip>`.
- Launching the game from an invite while it is closed passes
  `+connect a.b.c.d:port` on the command line, from the rich presence above.

## Commands the bridge sends

Through the `SB_RegisterEngineCmdExec` callback, the bridge asks the engine to
run console commands. An engine integration must provide the last two.

| Command | When |
| --- | --- |
| `echo <text>` | Every bridge log message. |
| `connect <a.b.c.d:port>` | A friend's invite or lobby was accepted. |
| `steam_drop <hConn> "<reason>"` | A server-side peer closed or timed out. The engine drops the client bound to that connection. |
| `steam_echo <text>` | Warnings (and, in Debug builds, messages) from the Steam networking library. Engines print these only when `developer_steam` is on. |

## Caveats for callers

- **Every flag result is a full `int`.** Builds before 2026-10-29 returned C++
  `bool` from `SB_Achivement_Unlock`, `SB_Achivement_Lock` and
  `SB_SteamNet_IsFakeIPAddr`. On x64 a `bool` defines only the low byte of
  `eax`, while the engine declares these as returning `int`, so a failed unlock
  could read as nonzero. If you must support an old DLL, read only the low byte.
- **The engine calls in before Steam is up.** Its function pointers are set when
  the DLL loads, not when `SB_Init` succeeds, so with Steam closed the engine
  still forwards QuakeC achievement calls. The achievement exports check for
  this and return 0. Builds before 2026-10-29 did not, and crashed on a null
  `SteamUserStats()`. QuakeC should still test `steam_steamid` first (see
  [QC.md](QC.md)), which also keeps the call quiet.
- **Static buffers.** `SB_GetSteamIDStr` and `SB_SteamNet_ServerGetFakeIP_String`
  return pointers into the DLL. Copy the text; do not free it.
