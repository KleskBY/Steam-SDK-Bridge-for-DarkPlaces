// fs_baker_csg_layout_to_boxes.c.h

// WORLD:	"_layout_to_boxes" "1"
// ACTION:	* Each brush in world becomes a bbox room
// 			* Each func_wall in world becomes a bbox room
// 			*Creates each floor with a width of wall width

// EXTRA:	"_layout_to_boxes_wall_width"	"8"  (default 64)
// 			"_layout_to_boxes_wall_height"	"256" (default 160)
//			"_layout_to_boxes_modelname"	"layout_model_graystone"	(The model room")
//			May or may not contain a single "func_lightrod" in the model room

RELATED_ (entitylist_perform_layout_from_boxes entity_make_brush_bbox_with_texture2)
RELATED_ (entity_make_brush_install_wall_entities)
	
	while (String_Contains (sinza, "_layout_to_boxes")) {
		Con_PrintLinef ("Detected " "_layout_to_boxes" " in .map (room from 2D brush layout), checking ...");
		entitylist_t list_map1 = {0};	// elder_quad_shrine.map

		// PARSE THE MAP
		int isok = entitylist_parsemaptxt (&list_map1, sinza);
		if (isok == false) Perform_Exit_1_Error("entitylist_parsemaptxt failed"); // failed to parse

		int num_boxes_made = entitylist_perform_layout_from_boxes (&list_map1);
		Con_PrintVarInt (num_boxes_made);

		// RE-WRITE IF ANYTHING HAPPENED
		if (num_boxes_made) {
			CSG_REWRITE_RELOAD_
		} // actions performed so rewrite
		
		entitylistfreecontents	(&list_map1);
		break;
	} // while



