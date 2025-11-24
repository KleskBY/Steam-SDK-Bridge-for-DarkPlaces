// mazegen_mazegen1.c.h


void _mazegen_name_rows_cols_blots_blotmin_blotmax (mazegen_s *m, int nrows, int ncols)
{
	// We exit with maze_za and nothing else.  The maze_za does not have "O" or "I".
	while (/* bad maze loop*/ 1) { // Maze generator fucks up somtimes ...
		
		_maze_free (m);
		
		m->rows = nrows;
		m->cols = ncols;
		m->sizeof_char_ray = (m->rows * m->cols + ONE_CHAR_1) * sizeof(char);
		m->sizeof_int_ray  = (m->rows * m->cols + ONE_CHAR_1) * sizeof(int);
		m->maze_za = ZAlloc_Char (m->sizeof_char_ray);

		initialize_maze(m, m->maze_za); // SOLID WORLD

		point2d_s starting_point;
		starting_point.row = 1 + (lhrandom(0, 1) * (m->cols - 2));
		starting_point.col = 1 + (lhrandom(0, 1) * (m->rows - 2));

		generate_maze(m, m->maze_za, starting_point.row, starting_point.col); // CARVE PATHS

		m->startpos	= Point2D_Find_First_Empty	(m, m->maze_za);
		m->endpos	= Point2D_Find_Last_Empty	(m, m->maze_za);

		// Detect fucked maze by making sure we have empties nears the corners of the bbox.
		if (m->startpos.row > 0 + 5 || m->startpos.col > 0 + 5 || m->endpos.row < ((m->rows - 1) -  5) || m->endpos.col < ((m->cols - 1) -  5) ) {
			Con_PrintLinef (CON_BRONZE "Generated a bad one .. trying again");
			continue; // This one sucks!
		}
		break;
	} // while to avoid badly carved mazes.

	// Good maze!
}


// MAZE GENERATOR CREATES SOLVABLE MAZE WITH PATH_ZA SOLVED
// No path optimization occurs here.  No lists of points written here.
void mazegen_name_rows_cols_blots_blotmin_blotmax (mazegen_s *m, 
	int nrows, int ncols,
	int num_blots, int blotminsize_rows, int blotmaxsize_rows, int blotminsize_cols, int blotmaxsize_cols)
{
	int cell_idx;

	while (/*DISCARD UNSOLVABLE MAZES*/ 1) {
		_mazegen_name_rows_cols_blots_blotmin_blotmax (m, nrows, ncols);

		point2d_s solvepos2		= {-1,-1};
		point2d_s exit_to_maze	= {-1,-1};

		// 1. Create path_za = clone maze_za
		// 2. Put I and O in path_za

		m->path_za = (char *)Z_MemDup_Z (m->maze_za, m->sizeof_char_ray);
		cell_idx = NCELL (m, m->startpos.row, m->startpos.col);	m->path_za[cell_idx] = MAZE_ENTRANCE_I;
		cell_idx = NCELL (m, m->endpos.row, m->endpos.col);		m->path_za[cell_idx] = MAZE_EXIT_O;

		// Create iternums.  Path creation will mark them with iters .. 351, 352, 353 .. of moved steps.
		m->iternums_za = ZAlloc_Int32 (m->sizeof_int_ray);

		solve_recursive (m, m->path_za, m->startpos, &solvepos2);

		if (Point2D_Are_Equal(m->startpos, exit_to_maze)) {
			// NO SOLUTION
			Con_PrintLinef ("Hit iters limit %d... failed maze_max_iters = %d", m->iters, maze_max_iters.integer);
			DebugPrintLinef ("Hit iters limit %d... failed maze_max_iters = %d", m->iters, maze_max_iters.integer);
			continue; // DISCARD THIS MAZE, MAKE ANOTHER AND TRY AGAIN
		}

		if (m->iters_exit) {
			// ITERS LIMIT HIT (ALSO NO SOLUTION, BUT PROBABLY GOT STUCK IN EXTENDED WHITESPACE DUE TO ALGO)
			// I think the algo will keep turning so any big whitespace gap is a pit trap.
			Con_PrintLinef	("Hit iters limit %d... failed maze_max_iters = %d", m->iters, maze_max_iters.integer);
			DebugPrintLinef ("Hit iters limit %d... failed maze_max_iters = %d", m->iters, maze_max_iters.integer);
			continue; // DISCARD THIS MAZE, MAKE ANOTHER AND TRY AGAIN
		}

		// BLOT AFTER PATH FIND DUE TO EXTENDED WHITESPACE WEAKNESS IN ALGO

		for (int blotnum = 0; blotnum < num_blots; blotnum ++) {
			int diameter_r = (int)(lhrandom(0, 1) * blotmaxsize_rows + blotminsize_rows);
			int diameter_c = (int)(lhrandom(0, 1) * blotmaxsize_cols + blotminsize_cols);

			int num_fails = 0;
			while (/*BLOT COLLISION LOOP*/ 1) { // We don't want overlapping blots. Bails on 10 failures.
				// 60 width. 20. diameter.
				// 25 width --> 1 to 59 35 + 25 = 60 .. (ncols 60 - 2) -
				int r0 = 1 + (lhrandom(0, 1) * ((m->rows - 3) - diameter_r)); // center?
				int c0 = 1 + (lhrandom(0, 1) * ((m->cols - 3) - diameter_c));
				int rb = r0 + diameter_r;
				int cb = c0 + diameter_c;

				mazebbox_s sbb = {0};	VectorSet (sbb.mbbmins, r0, c0, 0);	VectorSet (sbb.mbbmaxs, rb, cb, 0);

				// Check collisions
				int hitidx = not_found_neg1;
				for (int idx = 0; idx < m->blot_boxes_ba.numitems; idx ++) {
					mazebbox_s *boxo = BakerArray_Item_At(m->blot_boxes_ba, idx);
					if (BoxesOverlap2D(sbb.mbbmins,sbb.mbbmaxs, boxo->mbbmins, boxo->mbbmaxs)) {
						hitidx = idx;
						break;
					}
				} // for

				// If collision failure, restart the loop ..
				if (hitidx != not_found_neg1) {
					num_fails ++;
					Con_PrintLinef ("Overlap");
					if (num_fails > 10) {
						Con_PrintLinef ("Num fails > 10 no more blotting");
						break; // Fuck it
					}
					continue; // Try again
				}

				BakerArray_New_Empty_Item (mazebbox_s *, newbox, m->blot_boxes_ba);

				VectorCopyDestSrc (newbox->mbbmins, sbb.mbbmins);
				VectorCopyDestSrc (newbox->mbbmaxs, sbb.mbbmaxs);

				blot (m, m->maze_za, newbox->mbbmins[0], newbox->mbbmaxs[0], newbox->mbbmins[1], newbox->mbbmaxs[1], blotnum);
				Con_PrintLinef ("Made BBOX RC " VECTOR3_G " to " VECTOR3_G, VECTOR3_SEND(newbox->mbbmins), VECTOR3_SEND(newbox->mbbmaxs) );
				break; // While
			} // while blot collision loop
		} // for each blotnum

		BakerArray_Owned_Clear (m->blot_boxes_ba);
		break;
	} // while - unsolvable maze loop
}
