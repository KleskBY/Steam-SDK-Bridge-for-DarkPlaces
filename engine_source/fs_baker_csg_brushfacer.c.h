// fs_baker_csg_brushfacer.c.h

// WORLD:	"_brushfacer" "1" (missing or <= 0 will not run)
// ACTION:	gives all brush faces facing up a
//          the texturename "textures/up"
// PURPOSE: Caulk the whole world and run this
//          Then all brushes not seen are caulk
//			caulk faces get stripped from rendering
//          by q3map2, massively reducing triangles
//          and they don't get lightmapped either

	while (String_Contains (sinza, "_brushfacer")) {
		Con_PrintLinef ("Detected " "_brushfacer" " in .map, checking ...");
		entitylist_t list_map1 = {0};	// avol_oct3_facer.map

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		ccs *val = entitykeys_find_value (&list_map1.entity[ENTITY_WORLD_0], "_brushfacer");
		if (!val || atoi(val) <= 0) {
			entitylistfreecontents	(&list_map1);
			Con_PrintLinef ("No action: _brushfacer only works on world and it has value of 0.");
			break;
		}

		int num_faces = 0;
		int num_upwards_faces_changed = entitylist_perform_brushfacer_world_only (&list_map1, &num_faces);
		Con_PrintVarInt (num_faces);
		Con_PrintVarInt (num_upwards_faces_changed);
		
		if (num_upwards_faces_changed == 0) {
			entitylistfreecontents	(&list_map1);
			Con_PrintLinef ("No action: No faces changed.");
			break;
		}

		entitylist_key_set_replace_val (&list_map1, ENTITY_WORLD_0, "_brushfacer", timestring_newval);

		// RE-WRITE IF ANYTHING HAPPENED
		if (1) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


