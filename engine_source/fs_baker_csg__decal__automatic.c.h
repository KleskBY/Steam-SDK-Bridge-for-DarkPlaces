// fs_baker_csg__decal__automatic.c.h

#define COMMAND_NAME "_automatic_droptofloor"
#define COMMAND_DESC "Find _decal with target = _automatic_droptofloor and make a info_null target for it"

	while (String_Contains (sinza, COMMAND_NAME)) {
		Con_PrintLinef ("Detected " "_automatic_droptofloor" " in .map (_decal target _automatic_droptofloor support), checking ...");
		entitylist_t list_map1 = {0};	// aafter.map

		// PARSE THE MAP
		int isok = entitylist_parsemap_metric (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_done = entitylist_perform__decal__automatic (&list_map1);
		Con_PrintVarInt (num_done);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_done) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while

#undef COMMAND_NAME 
#undef COMMAND_DESC


