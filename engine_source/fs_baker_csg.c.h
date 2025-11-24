// fs_baker_csg.c.h
// CSG PROCESS IS HERE
RELATED_ (Sys_Console_Init_HOutput_WIN32)
void Perform_Exit_1_Error (ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	Con_PrintLinef ("************ ERROR ************" NEWLINE "%s", text);
	VA_EXPAND_ALLOC_FREE (text);
//	extern int super_exit_code;
//	super_exit_code = 2; // This is all failing.  Is it because SDL?
//	Con_PrintLinef ("Setting ExitProcess 2");
//	ExitProcess(2);
#ifdef _WIN32
	//FreeConsole(); // It's a visual issue.  You have to press enter once.
#endif
	exit (2); // Supposedly this is a bit "bad" but whatever ...
}

#define CSG_REWRITE_RELOAD_ \
	{ /* RE-WRITE */ \
		Con_PrintLinef ("Rewriting / reparsing .map ..."); \
		baker_string_t *bsout = entitylist_maptext_bsalloc (&list_map1); if (!bsout) Perform_Exit_1_Error("generated string using entitylist_maptext_bsalloc failed"); \
		int isok2 = FS_SaveFileString_Is_Ok_Unsafe (mappath, bsout->string); \
		BakerString_Destroy_And_Null_It (&bsout); \
		if (isok2 == false) Perform_Exit_1_Error("Save file string failed."); \
	} \
	{ /* REFRESH SOURCE Free the map string -- then reload it */ \
		Mem_FreeNull_ (sinza); sinza = FS_LoadFileString_Unsafe_ZAlloc (mappath); if (!sinza) Perform_Exit_1_Error("Reload FS_LoadFileString_Unsafe_ZAlloc sinza failed"); \
	} // Ender

void CSG_Perform (int csg_argnum)
{
#ifdef _WIN32
	Sys_Console_Init_HOutput_WIN32 (); // Activate HOUTPUT we hope
	const char *s_cwd_ta = FS_Getcwd_No_Trailing_Slash_Unsafe_TempAlloc ();
	Con_PrintLinef ("CSG: Startup: %s", s_cwd_ta);
	Mem_FreeNull_ (s_cwd_ta);
#endif

	// Baker: With -csg
	char mappath[MAX_OSPATH];
    //#pragma message ("Is PathWork_CSG WTF-8?")  It's fine AFAIK.
	PathWork_CSG (csg_argnum, mappath, sizeof(mappath) ); // "C:/galaxy/zircon/maps/elder_quad_shrine.map" from sys.argv

	// Gen timestring // "_brushfacer" "0 - 20241005 10:12 AM"
	char timestring[64]; TimeString_Fill_YYYYMMDD_HHMMSSAM (timestring, sizeof(timestring));
	va_super (timestring_newval, 128, "0 - %s", timestring);  // looks like "0 - 20241005 10:12 AM" //char *snewval_za = Z_StrDupf ("0 - %s", timestring); 
	Con_PrintLinef ("CSG: Processing map %s", mappath);
	// Do the command here and exit?
	//fs_offset_t bytes = 0;
	char *sinza = NULL;
	//char *sinza = (char *)File_To_Memory_Alloc (mappath, &bytes); // mappath	0x0019b55c "C:/galaxy/zircon/maps/aafter.map"	char [260]
	sinza = FS_LoadFileString_Unsafe_ZAlloc (mappath);
	
	if (!sinza) {
		Perform_Exit_1_Error("Unable to load file %s", mappath); // Couldn't open map
	}
	Con_PrintLinef ("Loaded from file ok.");

	// Baker: We are assuming the decal process applies.

commands:

	// COMMANDS - The process and then exit immediately.
	#include "fs_baker_csg_test.c.h"				// "_csg_test" ==> parses the map and then rewrites it verifying the parse outputs what it inputs
	#include "fs_baker_csg_brushfacer.c.h"			// "_brushfacer" "1" (missing or <= 0 will not run)	
	#include "fs_baker_csg_floor_everything.c.h"	// "_floor_everything" "1" (missing or <= 0 will not run)

	//
	// MODIFICATIONS
	// 

novel:
	#include "fs_baker_csg_clone_trigger.c.h"		// "_clone_trigger" "trigger_push" .. makes entity with same brushes composed of common/trigger and the classname is "trigger_push"
	#include "fs_baker_csg_rename.c.h"				// "_rename" "misc_model_torch=misc_model,misc_mdl_entity_kickable=misc_mdl_entity"

serious:
	// _layout_to_boxes must run before "_clone_classname"
	#include "fs_baker_csg_layout_to_boxes.c.h"		// "_layout_to_boxes" "1" (MUST RUN BEFORE "_clone_classname")

	// "_layout_to_boxes" may eventually spawn random entities.
	// How will cabinents and such be renamed?  We have room numbers, we should mydoor ===> myroom_room_1

	#include "fs_baker_csg_clone_classname.c.h"		// "_clone_classname" "light_csqc_emitter;origin_z += 81;spawnflags = 1"
	#include "fs_baker_csg_atomize.c.h"				// "_atomize" "1"
	#include "fs_baker_csg_originmake.c.h"			// "_originmake" "1" (MUST RUN AFTER "_atomize")
	#include "fs_baker_csg_collision_body.c.h"		// "_collision_body" "models/vehicles/psx/4door_car01.md3 184.5 76.703125 65.21875"
	#include "fs_baker_csg_floorage.c.h"			// "_floorage" "1" // Run before encase
	#include "fs_baker_csg_heightmap.c.h"			// "_heightmap" "0 to 384"
	#include "fs_baker_csg_sky_encase.c.h"			// Let's run this before inclusions

	// "info_decal_start" .. "info_decal_point" ..."info_decal_point" until the chain ends 
	// ... in theory.  In practice .. I don't recall making a road with more than 2 points.
	// WEAKNESS: big brushes don't seem to always decal

	#include "fs_baker_csg_decals.c.h"				// "info_decal_start" .. "info_decal_point" ... until the chain ends ... in theory.  In practice

	#include "fs_baker_csg__decal__automatic.c.h"	//
              
	//"map_include_prefix_start_end" "around 2 16"
	//"map_include_move" "8192 0 0"
	//"map_include_step" "2048"
	//"map_include_set" "invaders_bossname invaders1"
	//"map_include_change" "invaders_round"
	//"map_include_name_fields" "door_name attach_to controller"

	//"model_gen" "models/vehicles/psx/4door_car01.md3 184.5 76.703125 65.21875"
	//"model_gen1" "models/vehicles/psx/armored_car08.md3 206.75 94.953125 97.03125"
	//"model_gen2" "models/vehicles/psx/compact_car03.md3 168.3125 79.4375 64.3125"
	//"model_gen3" "models/vehicles/psx/oldcar_car07.md3 173.4375 72.78125 85.875"
	//"model_gen4" "models/vehicles/psx/police_car05b.md3 220.96875 83.40625 67.34375"
	//"model_gen5" "models/vehicles/psx/scrap_car06.md3 190.78125 74.40625 50.03125"
	//"model_gen6" "models/vehicles/psx/sedan_car02.md3 195.96875 86.9375 65.9375"
	//"model_gen7" "models/vehicles/psx/sedan_car05.md3 220.96875 83.40625 61.375"
	//"model_gen8" "models/vehicles/psx/taxi_car05c.md3 219.03125 83.40625 67.34375"
	//"model_gen9" "models/vehicles/psx/van_car04.md3 188.9375 101.8125 82.25"
	// Creates ultimately ...
		//{
		//"model" "*1"
		//"origin" "94.469002 50.906502 41.125000"
		//"size" "188.938 101.813 82.25"
		//"mdl" "models/vehicles/psx/van_car04.md3"
		//"classname" "func_collision"
		//}
	

	#include "fs_baker_csg_map_include_prefix_start_end.c.h"	// 

	Mem_FreeNull_ (sinza); // Free the map data -- then reload it
	Con_PrintLinef ("CSG: Ending");
	exit (0); // success
	Con_PrintLinef ("CSG: Over");
}

// Q13
// Baker: Q1 to Q3 map ...
// 1.  sky anything must become something else
// 2.  Lights will need the radius flag.
// 3.  star anything must become # something

void Q13_Perform (int j)
{
	char mappath[MAX_OSPATH];
	PathWork_CSG (j, mappath, sizeof(mappath) );

	// We ONLY support Valve 220.  Have JACK save it first.

	char *sinza = FS_LoadFileString_Unsafe_ZAlloc (mappath); // mappath	0x0019b55c "C:/galaxy/zircon/maps/aafter.map"	char [260]

	if (!sinza) exit (0); // Couldn't open map

	File_URL_Edit_Remove_Extension (mappath); // May 24 2025: Should be unicode safe.  All unicode >= char 128 = no collision.
	c_strlcat (mappath, "_out");
	c_strlcat (mappath, ".map");

	entitylist_t list_map1 = {0};	// aafter.map

	char timestring [64]; TimeString_Fill_YYYYMMDD_HHMMSSAM (timestring, sizeof(timestring));
	char *snewval_za = Z_StrDupf ("0 - %s", timestring); // looks like "0 - 20241005 10:12 AM"

	// PARSE THE MAP
	int isok = entitylist_parsemaptxt (&list_map1, sinza);
	if (isok == false) Perform_Exit_1_Error("q13 entitylist_parsemaptxt failed"); // failed to parse

	entitylist_change_textures_q1 (&list_map1, "skies/purplenebula_sky_low", "id1wad");

	baker_string_t *bsout = entitylist_maptext_bsalloc (&list_map1);
	if (!bsout) Perform_Exit_1_Error("generated string using entitylist_maptext_bsalloc failed"); // This would be bad.

	// RE-WRITE
	int isok2 = FS_SaveFileString_Is_Ok_Unsafe (mappath, bsout->string);
	BakerString_Destroy_And_Null_It (&bsout);

	if (isok2 == false) Perform_Exit_1_Error("Save file string failed."); // Couldn't write file

	Mem_FreeNull_ (snewval_za);
	entitylistfreecontents	(&list_map1);

	Mem_FreeNull_ (sinza); // Free the map data -- then reload it
	exit (0); // success
}


