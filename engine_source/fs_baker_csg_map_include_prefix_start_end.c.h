// fs_baker_csg_map_include_prefix_start_end.c.h

// WORLD:	"_originmake" "1"
// ACTION:	insert several maps into a map
// PURPOSE: insert several maps into a map

// map_include_prefix_start_end ... "around 1 10" // around1.map to around10.map // 3 tokens prefix start end
// map_include_origin			... -20000 -20000 -2000  where first paste occurs
// map_include_step				... 8000

// AAFTER uses these ..
// map_include_set "invaders_bossname invaders1" (OPTIONAL) // Not sure what this does.

// AFTER HAS:
//"map_include_prefix_start_end" "around 2 16"
//"map_include_move" "8192 0 0"
//"map_include_step" "2048"
//"map_include_set" "invaders_bossname invaders1"
//"map_include_change" "invaders_round"
//"map_include_name_fields" "door_name attach_to controller"

// AAFTER ... Also has

//{
//"classname" "misc_mdl_entity_invaders"
//"model" "models/vehicles/psx/taxi_car05c.md3"
//"spawnflags" "65536"
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
//"origin" "-1399 -620 88"
//}
// OUTPUT IS
//{
//"model" "*1"
//"origin" "94.469002 50.906502 41.125000"
//"size" "188.938 101.813 82.25"
//"mdl" "models/vehicles/psx/van_car04.md3"
//"classname" "func_collision"
//}
//{
//"model" "*2"
//"origin" "109.515503 41.703152 33.671902"
//"size" "219.031 83.4063 67.3438"
//"mdl" "models/vehicles/psx/taxi_car05c.md3"
//"classname" "func_collision"
//}



// The logs stuff on around4 does not seem to use collision volumes.

// map is "around4b" is donkey kong


	while (String_Contains (sinza, "map_include_prefix_start_end")) {
		entitylist_t list_map1 = {0};	// aafter.map

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		RELATED_ (VM_map_move)

		// Baker: entitylist_gen_models INSERTS as entity #1
		int modgen = entitylist_epairs_find_model_gen_entitynum (&list_map1, "model_gen");
		if (modgen != not_found_neg1) {
			// Generate func_collision entities of various sizes
			entitylist_gen_models (&list_map1, modgen);
		}

		ccs *val, *datasrc;

		// Parse prefix, jstart, jlast
		val = entitykeys_find_value (&list_map1.entity[0], "map_include_prefix_start_end");
		if (!val) Perform_Exit_1_Error("entitykeys_find_value failed"); // couldn't find

		datasrc = val;
		char prefix[MAX_QPATHX2_256]; int jstart = -1, jlast = -1;

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		c_strlcpy (prefix, com_token); // around
		// prefix	0x0019b3bc "around"	char [256]

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		jstart = atoi (com_token);	// 1
		// jstart	2	int

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		jlast = atoi (com_token);	// 10
		// jlast	3	int

		//qbool shall_caulk = false;
		val = entitykeys_find_value (&list_map1.entity[0], "map_include_caulkworld");
		if (val && val[0] != '0') {
			Con_PrintLinef ("Request for map_include_caulkworld on inclusions detected to reduce lightmap size.");

		}

		// Parse move
		val = entitykeys_find_value (&list_map1.entity[0], "map_include_move");
		// val	0x04b92810 "-32768 -32768 -4196"	const char *

		if (!val) Perform_Exit_1_Error("entitykeys_find_value failed"); // couldn't find

		datasrc = val;
		vec3_t move0;

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		move0[0] = atof (com_token);	// 1

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		move0[1] = atof (com_token);	// 1

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		move0[2] = atof (com_token);	// 1

		// Parse step
		val = entitykeys_find_value (&list_map1.entity[0], "map_include_step");
		if (!val) Perform_Exit_1_Error("entitykeys_find_value failed"); // couldn't find
		// val	0x04b92920 "8192"	const char *

		datasrc = val;
		float step0;

		if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
		step0 = atof (com_token);	// 1

		// map_include_set "invaders_bossname invaders1" (OPTIONAL)
		val = entitykeys_find_value (&list_map1.entity[0], "map_include_set");
		stringlist_t listset = {0};		// "invaders_bossname" "invaders1"

		datasrc = val;
		if (val) {
			if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
			stringlistappend (&listset, com_token);	// invaders_bossname

			if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
			stringlistappend (&listset, com_token);	// "invaders1"
		}

		// map_include_change "invaders_round" (OPTIONAL)
		val = entitykeys_find_value (&list_map1.entity[0], "map_include_change");
		stringlist_t listchange = {0};		// "invaders_round"

		datasrc = val;
		if (val) {
			if (COM_Parse_Basic(&datasrc) == false) Perform_Exit_1_Error("COM_Parse_Basic to get val failed"); // parse failure
			stringlistappend (&listchange, com_token);	// invaders_round
		}

		// CREATE A LIST OF FIELDS THAT ARE ENTITY REFERENCES TO PREFIX. targetname, target, etc.
		// map_include_name_fields "door_name attach_to" (OPTIONAL)
		RELATED_ (entitylist_prefix_epairs_targetnames)
		stringlist_t listnamefields = {0};
		stringlistappend (&listnamefields, "targetname");
		stringlistappend (&listnamefields, "target");

		val = entitykeys_find_value (&list_map1.entity[0], "map_include_name_fields");
		datasrc = val;
		if (val) {
			// This breaks when all fields read ...
			while (1) {
				if (COM_Parse_Basic(&datasrc) == false) break; // parse failure
				stringlistappend (&listnamefields, com_token);	// invaders_round
			}
		}

		// PLAN
		// FOR EACH MAP
		// MOVE EVERYTHING AND COPY INTO SRC
		vec3_t translate_amount;
		VectorCopy (move0, translate_amount); // src -> dest

		// Baker: We are ascending the paste point by +8192 after each iteration.
		for (int j = jstart; j <= jlast; j ++, translate_amount[2] += step0) {
			entitylist_t list_paste = {0};	// aafter.map
			char smap[MAX_OSPATH];
			c_dpsnprintf3 (smap, "%s/maps/%s%d.map", fs_csg_basedir, prefix, j); // prefix like "around 2 10"
			//smap	0x0019b244 "C:/galaxy/zircon/maps/around2.map"	char [260]

			char *sinza2 = (char *)FS_LoadFileString_Unsafe_ZAlloc (smap);
			if (!sinza2) Perform_Exit_1_Error ("Loadfile of %s failed", smap); // Couldn't open map

			// PARSE THE MAP
			int isok2 = entitylist_parsemaptxt (&list_paste, sinza2);
			if (isok2 == false) Perform_Exit_1_Error("entitylist_parsemaptxt list_paste failed"); // failed to parse

			// IN LIST_PASTE:
			// Translate entity 0 brushes accordingly
			// Translate ALL origin keys in all entities epairs.

			va_super (prefix_here, MAX_QPATH_128, "%s%d_", prefix, j);

			entitylist_caulk_world_regular_textures (&list_paste);
			//entitylist_prefix_epairs		(&list_paste, prefix_here);
			entitylist_prefix_epairs_targetnames	(&list_paste, prefix_here, &listnamefields);

			entitylist_translate_epairs_origin		(&list_paste, translate_amount);
			entitylist_translate_brushes	(&list_paste, translate_amount);

			entitylist_map_include_nonworld_set			(&list_paste, &listset);
			if (listchange.numstrings) {
				ccs *key_force = listchange.strings[0];
				va_super (val_force, 1024, "%d", j);
				entitylist_nonworld_setthis (&list_paste, key_force, val_force);
			}

//			entitylist_prefix_epairs_targetnames	(&list_paste, prefix_here, &listnamefields);
//
//			entitylist_translate_epairs_origin		(&list_paste, translate_amount);
//			entitylist_translate_brushes	(&list_paste, translate_amount);

			//entitylist_to_clipboard (&list_paste); // check our work

			// Add entity 0 brushes TO MAP1
			// Add entity ALL brushes and epairs to MAP1
			entitylist_brush0_append		(&list_map1, &list_paste);	// no keys, just brushes.
			entitylist_nonworld_append		(&list_map1, &list_paste);	// brushes and keys.

			entitylistfreecontents	(&list_paste);
			Mem_FreeNull_ (sinza2);
		} // map_include_prefix_start_end

		stringlistfreecontents (&listset);
		stringlistfreecontents (&listchange);
		stringlistfreecontents (&listnamefields);

		// RE-WRITE IF ANYTHING HAPPENED
		if (1) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while
