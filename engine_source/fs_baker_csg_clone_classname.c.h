// fs_baker_csg_clone_classname.c.h

// WORLD:	"_clone_classname" "light_csqc_emitter;origin_z += 81;spawnflags = 1"
// ACTION:	creates a copy of entity with
//			different classname that is a copy
//			with specified changes

// PURPOSE: Allow misc_model_torch 
//			* generate a light_csqc_emitter
//			* that is higher up
//			* with different spawnflags "1" instead of 6

	while (String_Contains (sinza, "_clone_classname")) {
		Con_PrintLinef ("Detected " "_clone_classname" " in .map (allows light and particles different origin), checking ...");
		entitylist_t list_map1 = {0};	// tiny_deep_lair.map
		
		// PARSE THE MAP
		int isok = entitylist_parsemap_metric (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		Con_PrintLinef ("Performing ...");
		int num_clones_classnames_made = entitylist_perform_clone_classname_num_made (&list_map1, timestring_newval);
		Con_PrintVarInt (num_clones_classnames_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_clones_classnames_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


