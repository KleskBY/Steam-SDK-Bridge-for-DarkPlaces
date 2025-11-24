#pragma once

extern bool g_steamInitialized;
extern void (*g_ExecFromEngine)(const char* cmd);

bool Steam_Init();
void Steam_Frame();
void Steam_Shutdown();
char* Steam_GetSteamIDStr();
void Steam_Connect(const char* addr);