// fs_baker_csg_floorage.c.h

// WORLD:	"_floorage" "maps_sh/fang_grass_only" (missing or <= 0 will not run)
// ACTION:	makes floor with specified texture
//          colliding against func_group_terrain*
//          the has bbmins <= 0
// PURPOSE: Automatic floor

	while (String_Contains (sinza, "_floorage")) {
		Con_PrintLinef ("Detected " "_floorage" " in .map, checking ...");
		entitylist_t list_map1 = {0};	// pac3.map

		// PARSE THE MAP
		int isok = entitylist_parsemap_metric(&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		ccs *val = entitykeys_find_value (&list_map1.entity[ENTITY_WORLD_0], "_floorage");
		if (!val || String_Starts_With (val, "0 -")) {
			entitylistfreecontents	(&list_map1);
			Con_PrintLinef ("No action: _floorage has %s", !val ? "no value" : "value of leading 0");
			break;
		}

		int num_floor_tiles = entitylist_perform_floorage_world_only (&list_map1);
		Con_PrintVarInt (num_floor_tiles);
		
		if (num_floor_tiles == 0) {
			entitylistfreecontents	(&list_map1);
			Con_PrintLinef ("No action: No action.");
			break;
		}

		entitylist_key_set_replace_val (&list_map1, ENTITY_WORLD_0, "_floorage", timestring_newval);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_floor_tiles) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


