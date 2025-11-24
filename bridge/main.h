#pragma once
#include <Windows.h>
#include <iostream>
#include <public/steam/steam_api.h>
#include <public/steam/isteamclient.h>
#include <public/steam/isteamugc.h>
#include <public/steam/isteamnetworkingsockets.h>
#include <public/steam/isteamnetworkingutils.h>
#include <filesystem>
#include <fstream>
#include <chrono>


namespace fs = std::filesystem;

#include "steam.h"
static void log(const std::string& s)
{
    if (g_ExecFromEngine) g_ExecFromEngine(("echo " + s + "\n").c_str());
    else std::cout << s << std::endl;
}
