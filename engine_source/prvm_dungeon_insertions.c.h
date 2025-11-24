// prvm_dungeon_genmap.c.h

// Grid is already filled and rows and cols are set.
// 256 256 ->block_size is set.
qbool Generate_Dungeon_Map_File (map_compile_s *p)
{

	int isok = entitylist_parse_mapfile_is_ok (&p->eout_map, p->maps_shell_dot_map);
	if (!isok) {
		Con_PrintLinef ("Unable to load clone %s", p->maps_shell_dot_map);
		goto exitor;
	}

	// Find the mat.  Get the size.
	// Load mat1
	// Load block1
	int eidx_mat1   = entitylist_find_key_value (&p->eout_map, "targetname", "mat1");
	if (eidx_mat1 == not_found_neg1) {
		isok = false;
		Con_PrintLinef ("Error couldn't find the mat1");
		goto exitor;
	}
	entityx_t *emat1  = EntForNum(p->eout_map, eidx_mat1);
	isok = entity_key_set_value_is_ok(emat1, "classname", "info_null"); // Turn the mat into something that gets removed by QuakeC

	brmins_maxs_init (p->mat_mins, p->mat_maxs);
	/*unused int did_any =*/ entity_expand_bounds_did_brushes(emat1, p->mat_mins, p->mat_maxs);

	// END OF SHELL OPEN

	// BEGIN PASTE BLOCKS
	// Where are we pasting?

	for (int row = 0; row < p->rows; row ++) {
		for (int col = 0; col < p->cols; col ++) {
			int idx = col + row * p->cols;
			int gridval = p->ints_grid.ints[idx];
			entitylist_t eblock = {0};
			if (!gridval)
				continue; // 0 = empty

			// Open the file
			va_super (filename, MAX_QPATH_128, "%s", "maps/proto/#block1.map");
			int isok2 = entitylist_parse_mapfile_is_ok (&eblock, filename);
			if (!isok2)
				goto exitor; // Failed to open file.

			int coord_row = (p->rows - 1) - row; // Flip the row so 0,0 // 20x13 // 20-

			// Requires map block has 0,0,0 based and is 256 x 256 size.
			int add_x = col * p->block_size[0];
			int add_y = coord_row * p->block_size[1];
			vec3_t translate_amount = { p->mat_mins[0] + add_x, p->mat_mins[1] + add_y, 0 };

			va_super (prefix_here, MAX_QPATH_128, "_block%d_", idx);

			entitylist_prefix_epairs_targetnames	(&eblock, prefix_here, &p->listnamefields); // _block1_
			entitylist_append_map_with_translation	(&p->eout_map, &eblock, translate_amount);

			entitylistfreecontents (&eblock);
		} // cole
	} // row

exitor:
	//entitylistfreecontents	(&p->emap);
	entitylist_write_to_file (&p->eout_map, p->rp_map_dot_map);

	return isok;
}
