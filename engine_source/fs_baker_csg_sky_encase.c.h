// fs_baker_csg_sky_encase.c.h

RELATED_ (CSG_Perform)

// ENTITY:	"_sky_encase" "8192 skies/desert2_sky"
// ACTION:	Finds bbox of world brushes (and entities?).  Makes a sky 8192 high starting at 0.  It is 32 thick.

// PURPOSE:  Quick skyboxing of geometry.  Assumes 0 is floor level.  Does not write down.
// PROBLEMS: None?

	while (String_Contains (sinza, "_sky_encase")) {
		entitylist_t list_map1 = {0};	// pac.map

		Con_PrintLinef ("Found _sky_encase in .map, checking ...");

		// PARSE THE MAP
		int isok = entitylist_parsemap_metric (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_made = entitylist_perform_sky_encase_brushes_added (&list_map1, timestring_newval);
		Con_PrintLinef ("num_made %d", num_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


