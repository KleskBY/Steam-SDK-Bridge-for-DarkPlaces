# SteamBridge – Steamworks Integration for DarkPlaces-Based Engines

**SteamBridge** is a clean, GPL-compatible integration layer that allows DarkPlaces-derived engines (based on Zircon Beta 20251028) to use **Steamworks SDK features** such as:
-   Steam API initialization
-   Steam Networking (Steam Datagram Relay – SDR)
-   FakeIP and P2P connection
-   Achievements
-   Invites to current server

SteamBridge is designed specifically to **avoid GPL license conflicts** by isolating all Steam SDK calls inside a **separate proprietary DLL**, while the engine communicates with that DLL only through **pure C function pointers**.

This repository contains:
-   `engine_code` – GPL side of the integration (GPL 2)
-   `bridge` – Steam integration


----------

# How the Pipeline Works

Steam SDK cannot be included inside a GPL-licensed engine due to incompatible license terms.  
**SteamBridge solves this with a legally compliant pipeline:**
DarkPlaces Engine (C code) <---> SteamBridge DLL (C++ code) <---> Steamworks SDK (Valve)

### ENGINE SIDE (GPL)
-   Loads `steambridge.dll` dynamically with `LoadLibrary()`
-   Retrieves allowed function pointers via `GetProcAddress()`
-   Makes **no direct reference to Steam SDK symbols**
-   Contains **no Steam headers**, **no Steam types**, **no linking**
-   Can be legally distributed as GPL-licensed

### STEAMBRIDGE DLL (Proprietary, NOT GPL)
-   Compiled separately
-   Links to Steamworks SDK
-   Exports a tiny API of clean C functions
-   Handles SteamAPI_Init, callbacks, networking, FakeIP, achievements, etc.