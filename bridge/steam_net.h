#pragma once
#include <string>
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

typedef enum lhnetaddresstype_e
{
	LHNETADDRESSTYPE_NONE,
	LHNETADDRESSTYPE_LOOP,
	LHNETADDRESSTYPE_INET4,
	LHNETADDRESSTYPE_INET6,
	LHNETADDRESSTYPE_STEAM_FAKEIP,
}
lhnetaddresstype_t;

typedef struct lhnetaddressnative_s
{
	lhnetaddresstype_t addresstype;
	int port;
	union
	{
		struct sockaddr sock;
		struct sockaddr_in in;
	}
	addr;

	uintptr_t hConn; //HSteamNetConnection hConn; 
	unsigned long long steamid; //uint64 steamID;
}
lhnetaddressnative_t;

typedef struct lhnetaddress_s
{
	lhnetaddresstype_t addresstype;
	int port; // used by LHNETADDRESSTYPE_LOOP
	unsigned char storage[256]; // sockaddr_in or sockaddr_in6
} __lhnetaddress_t;

#define lhnetaddress_t struct lhnetaddress_s

typedef struct llist_s
{
	struct llist_s* prev;
	struct llist_s* next;
} llist_t;

typedef struct lhnetsocket_s
{
	lhnetaddress_t address;
	int inetsocket;
	llist_t list;
}
lhnetsocket_t;

#include <public/steam/steam_api.h>
extern CSteamID g_currentLobby;

int SteamNet_Init(void);
bool SteamNet_IsClient(void);
void SteamNet_ServerStop(void);
int SteamNet_ServerStart(void);
int SteamNet_ServerGetFakeIP(lhnetaddress_t* out_addr);
const char* SteamNet_ServerGetFakeIP_String();
void SteamNet_ClientDisconnect();
int SteamNet_ClientConnectFakeIP(const lhnetaddress_t* server_addr);
void SteamNet_Shutdown(void);
int SteamNet_Read(lhnetsocket_t* mysocket, void* data, int maxlength, lhnetaddress_t* peeraddress);
int SteamNet_Write(lhnetsocket_t* mysocket, const void* content, int contentlength, const lhnetaddress_t* vaddress);
bool SteamNet_IsFakeIP(unsigned __int32 ipHostOrder);
bool SteamNet_IsFakeIPAddr(const lhnetaddress_t* a);
void SteamNet_InviteFriendToCurrentServer(const char* str);