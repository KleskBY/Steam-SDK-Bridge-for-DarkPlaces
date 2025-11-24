// fs_baker_csg_originmake.c.h

// ENTITY:	"_originmake" "1"
// ACTION:	gives this entity an origin brush center.
// PURPOSE: to explode a brush model
	while (String_Contains (sinza, "_originmake")) {
		Con_PrintLinef ("Detected " "_originmake" " in .map (gives an entity an origin brush), checking ...");
		entitylist_t list_map1 = {0};	// around17.map (explosion attempt)

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_brushes_made = entitylist_perform_originmake_num_made (&list_map1, timestring_newval);
		Con_PrintVarInt (num_brushes_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_brushes_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while



