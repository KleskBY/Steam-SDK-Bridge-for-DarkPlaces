// fs_baker_csg_clone_trigger.c.h

// WORLD:	"_clone_trigger" "trigger_push" //
// ACTION:	takes the func_group and creates a copy "trigger_push" copy with specified name.
//			all brushes will have common/trigger

// PURPOSE: To create a river where everywhere you touch is a single trigger, right?
// PROBLEM: Triggers really must be boxes, so this doesn't quite work.

	while (String_Contains (sinza, "_clone_trigger")) {
		Con_PrintLinef ("Detected " "_clone_trigger" " in .map (allows rivers with triggers), checking ...");
		entitylist_t list_map1 = {0};	// current3.map
		
		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_triggers_made = entitylist_perform_clone_as_trigger_entities_num_made (&list_map1, timestring_newval);
		Con_PrintVarInt (num_triggers_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_triggers_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


