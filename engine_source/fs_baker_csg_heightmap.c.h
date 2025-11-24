// fs_baker_csg_heightmap.c.h

RELATED_ (CSG_Perform)

// DETECT NEGATIVE ZERO: if (signbit(num)) { printf("The number is negative (including negative zero).\n"); 

// ENTITY:	"_heightmap" "0 to 384"
// ACTION:	takes any entity with this
//			1. Create an string array of all non-common/ vertex points in every face.
//          2. Uniques the list.
//			3. Creates an alpha brush (0 to 100) at each unique vertex based on distance from heightmap
// 
//			4. Because we use the faces, we do not care about metric.

// PURPOSE:  Snow
// PROBLEMS: Don't know of any

	while (String_Contains (sinza, "_heightmap")) {
		Con_PrintLinef ("Detected " "_heightmap" " in .map (allows snowing top of brushes), checking ...");
		entitylist_t list_map1 = {0};	// unishell2.map

		// PARSE THE MAP
		int isok = entitylist_parsemap_metric (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_alpha_brushes_made = entitylist_perform_heightmap_num_alpha_brushes_added (&list_map1, timestring_newval);
		Con_PrintVarInt (num_alpha_brushes_made);
		// RE-WRITE IF ANYTHING HAPPENED
		if (num_alpha_brushes_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while


