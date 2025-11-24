// fs_baker_csg_rename.c.h

// WORLD:	"_rename" "misc_model_torch=misc_model,misc_mdl_entity_kickable=misc_mdl_entity"
// ACTION:	Renames the command delimited entity types.
// PURPOSE: Using a non-Baker q3map2.exe that can't handle misc_model_anything or func_group_anything
//          but might have other uses.

	//.. takes the func_group and creates a copy with specified name.
	// all brushes will have common/trigger
	while (String_Contains (sinza, "_rename")) {
		Con_PrintLinef ("Detected " "_rename" " in .map (renames classnames to something else), checking ...");
		entitylist_t list_map1 = {0};	// aafter.map

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_renames = entitylist_perform_classname_renames (&list_map1);
		Con_PrintVarInt (num_renames);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_renames) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while



