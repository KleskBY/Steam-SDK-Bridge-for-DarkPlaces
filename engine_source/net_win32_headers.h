#pragma once


extern "C" {

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

	typedef enum lhnetaddresstype_e
	{
		LHNETADDRESSTYPE_NONE,
		LHNETADDRESSTYPE_LOOP,
		LHNETADDRESSTYPE_INET4,
		LHNETADDRESSTYPE_INET6
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
#ifndef NOSUPPORTIPV6
			struct sockaddr_in6 in6;
#endif
		}
		addr;
	}
	lhnetaddressnative_t;
}