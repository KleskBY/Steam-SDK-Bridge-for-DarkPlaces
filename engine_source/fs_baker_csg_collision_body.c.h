// fs_baker_csg_collision_body.c.h

// ENTITY:	"_collision_body" "models/vehicles/psx/4door_car01.md3 184.5 76.703125 65.21875"
// ACTION:	Create a func_collision brush with mdl "yourmodel.md3" and "size" "yoursize"
//
// Allows QuakeC to 

//{
//"classname" "func_collision"
//"mdl" "models/vehicles/psx/van_car04.md3"
//"size" "188.938 101.813 82.25"
//}

//void misc_mdl_entity_invaders ()
//{
//	if ((self.collision_prototype = Find_Func_Collision_For_ModelName(self.model)) ) {
//		self.visual_ghost = AttachGhostAtTagNameOrigin (self, self.model, TAG_NONE, VEC_ORIGIN);
//		setmodel (self, self.collision_prototype.model); // no precache, is bsp model		
//	}
//}

	while (String_Contains (sinza, "_collision_body")) {
		Con_PrintLinef ("Detected " "_collision_body" " in .map (gives an entity an origin brush), checking ...");
		entitylist_t list_map1 = {0};	// around17.map (explosion attempt)

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_brushes_made = entitylist_perform_collision_body (&list_map1, timestring_newval);
		Con_PrintVarInt (num_brushes_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_brushes_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite

		entitylistfreecontents	(&list_map1);
		break;
	} // while



