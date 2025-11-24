// fs_baker_csg_atomize.c.h

RELATED_ (CSG_Perform)

// ENTITY:	"_atomize" "1"
// ACTION:	takes any entity with this
//			each brush becomes own entity with all same values
//			parent has classname renamed
// 			"func_breakable" "_func_breakable_processed"
// 
// PURPOSE: To create a river where everywhere you touch is a single trigger, right?
// PROBLEM: Triggers really must be boxes, so this doesn't quite work.

	while (String_Contains (sinza, "_atomize")) {
		Con_PrintLinef ("Detected " "_atomize" " in .map, checking ...");
		entitylist_t list_map1 = {0};	// unishell2.map

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_entities_made = entitylist_perform_atomize_entities_num_made (&list_map1, timestring_newval);
		Con_PrintVarInt (num_entities_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_entities_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


