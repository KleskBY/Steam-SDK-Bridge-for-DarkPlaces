// fs_baker_csg_floor_everything.c.h

// WORLD:	"_floor_everything" "1" (any non-zero value will run)
// ACTION:	The top of all brushes will become 0.
// PURPOSE: Creating a 2D layout.

#define COMMAND_NAME "_floor_everything"
#define COMMAND_DESC "Set top of all brushes to 0"

	while (String_Contains (sinza, "_floor_everything")) {
		Con_PrintLinef ("Detected " "_floor_everything" " in .map, checking ...");
		entitylist_t list_map1 = {0};	// dw_quadshrine.map

		// PARSE THE MAP
		Con_PrintLinef ("Parse map ..");
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse
		Con_PrintLinef ("Parse map: ok!");

		ccs *val = entitykeys_find_value (&list_map1.entity[ENTITY_WORLD_0], "_floor_everything");
		if (!val || atof(val) == 0) {
			Con_PrintLinef ("No action: _floor_everything only works on world and it has value of 0.");
			entitylistfreecontents	(&list_map1);
			break;
		}

		int num_moved = entitylist_perform_floor_everything_return_num_moved (&list_map1, val);
		Con_PrintVarInt (num_moved);

		entitylist_key_set_replace_val (&list_map1, ENTITY_WORLD_0, "_floor_everything", timestring_newval);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_moved) {
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
