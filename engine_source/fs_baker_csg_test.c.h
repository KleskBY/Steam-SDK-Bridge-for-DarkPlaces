// fs_baker_csg_test.c.h

// WORLD:	"_csg_text" "1"
// ACTION:	Read .map and then write .map from parser
//			It should be identical except for whitespace
// PURPOSE: Verifying the parse outputs exactly what it inputs.

#define COMMAND_NAME "_csg_test"
#define COMMAND_DESC "Read .map, parse .map, rewrite .map."

	// Baker: May 30 2025 - This was a FULL PASS with 0 differences compared to J.A.C.K. (except whitespace stuff)
	while (String_Contains (sinza, "_csg_test")) {
		entitylist_t list_map1 = {0};	// aafter.map

		Con_PrintLinef ("%s requested ...", COMMAND_NAME);
		Con_PrintLinef ("Description: %s", COMMAND_DESC);

		// PARSE THE MAP
		Con_PrintLinef ("Parse map ..");
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse
		Con_PrintLinef ("Parse map: ok!");
		// DO NOTHING
		
		// RE-WRITE IF ANYTHING HAPPENED
		if (1) {
			Con_PrintLinef ("Save .map file ..");
			{ // RE-WRITE
				baker_string_t *bsout = entitylist_maptext_bsalloc (&list_map1); if (!bsout) Perform_Exit_1_Error("generated string using entitylist_maptext_bsalloc failed"); // This would be bad.
				int isok2 = FS_SaveFileString_Is_Ok_Unsafe (mappath, bsout->string);
				BakerString_Destroy_And_Null_It (&bsout);
				if (isok2 == false) Perform_Exit_1_Error("Save .map file failed."); // Couldn't write file
			}
#if 0 // No reason to do this ..
			{ // REFRESH SOURCE Free the map string -- then reload it
				Mem_FreeNull_ (sinza); sinza = FS_LoadFileString_Unsafe_ZAlloc (mappath); if (!sinza) Perform_Exit_1_Error("Reload FS_LoadFileString_Unsafe_ZAlloc sinza failed"); // Couldn't reopen map
			}
#endif
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		Mem_FreeNull_ (sinza); 

		// THE ENTIRE POINT OF CSG TEST IS TO DO NOTHING, WRITE and EXIT

		Perform_Exit_1_Error("%s: Finished ok!  Exiting with error code to stop compile process.", COMMAND_NAME);
		
		// unreachable ...

		break; 
	} // while


#undef COMMAND_NAME 
#undef COMMAND_DESC
