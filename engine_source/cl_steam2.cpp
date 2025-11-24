// cl_steam2.cpp

//		#include <GL/gl.h>
//		#include <GL/glu.h>


// C/C++ Includes
#include <cstdlib>
#include <iostream>

#include <stdlib.h>
#include <fcntl.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "quakedef.h"
#include "common.h" // Courtesy
#include "utf8lib.h"

#undef snprintf
#define snprintf dpsnprintf

#pragma comment (lib, "steam_lib/steam_api.lib")


// STEAMWORKS API
#include <steam/steam_api.h>
//#include <steam/steam_gameserver.h> // Only include this if we are using the GameServer API

// Program Entry Point
int main()
{
    // Init SteamAPI
    if (SteamAPI_Init() == false)
    {
        // Send a message to the console
        std::cout << "ERROR: Steam must be open and logged in to run this game" << std::endl;
        return EXIT_FAILURE;
    }


    // Grab SteamFriends
    ISteamFriends* pSteamFriends = SteamFriends();
    // Send a message to the Console
    std::cout << pSteamFriends->GetPersonaName() << std::endl;


    // Shutdown SteamAPI
    SteamAPI_Shutdown();

    return EXIT_SUCCESS;
}