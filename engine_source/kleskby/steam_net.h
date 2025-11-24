#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif
	extern bool g_steamNetInitialized;
	extern struct cvar_s steam_fakeip;
	extern struct cvar_s steam_steamid;
	extern struct cvar_s steam_networking;

	// ---- shared bootstrap ----
	int SteamNet_Init(void);           // Call in NetConn_InitOnce or LHnet_Init
	void SteamNet_Shutdown(void);      // Call in NetConn_Shutdown or LHNET_Shutdown
	int SteamNet_Read(lhnetsocket_t* lhnetsocket, void* content, int maxcontentlength, lhnetaddress_t* vaddress);
	int SteamNet_Write(lhnetsocket_t* lhnetsocket, const void* content, int contentlength, const lhnetaddress_t* vaddress);
	void SteamNet_Frame();
	bool SteamNet_IsFakeIPAddr(const lhnetaddress_t* vaddress);

	// ---- server side ----

	// Start a Steam/SDR server, request FakeIP, create listen socket.
	// "port_hint" is your classic Quake port (e.g. 26000) you want to *semantically* use.
	// Returns 1 on success, 0 on failure.
	int SteamNet_ServerStart(); // Call in NetConn_OpenServerPorts

	void SteamNet_ServerStop(void); // Call in NetConn_CloseServerPorts

	// ---- client side ----

	// Connect to a Steam server given its FakeIP (in lhnetaddress_t form, as an IPv4).
	// Returns 1 on success (connection starting), 0 on failure.
	int SteamNet_ClientConnectFakeIP(const lhnetaddress_t* server_addr);

	// Disconnect client from Steam server (if connected).
	void SteamNet_ClientDisconnect(void);
	void Steam_InviteFriend_f(cmd_state_t* cmd);
	void SteamNet_RegisterCvars();


	//CALLS
	//SteamNet_Init
	//SteamNet_Shutdown
	//SteamNet_ServerStop
	//SteamNet_ServerStart -> NetConn_OpenServerPorts

#ifdef __cplusplus
}
#endif

