// mazegen_pathfind_2.c.h


/* ================================================================= */
/*           METHODS AND STRUCTS USED IN THE PROGRAM                 */
/* ================================================================= */



/* Changes the array given as parameter to the possible ways the maze can continue */
/* The array must have 4 spaces */
void Maze_CheckDirections(mazegen_s *m, ccs *s, point2d_s curpos, point2d_s *poss_ways) {
    /* Can curpos go down? */
    if (curpos.row >= m->rows - 1)
        poss_ways[0] = Point2D_From_XY(-1, -1);
    else {
		int cell_idx = NCELL (m, curpos.row + 1, curpos.col);
        //if (maze[curpos.x+1][curpos.y] == MAZE_EMPTY_SPACE_32 || maze[curpos.x+1][curpos.y] == MAZE_EXIT_O)
		if (s[cell_idx] == MAZE_EMPTY_SPACE_32 || s[cell_idx] == MAZE_EXIT_O)
            poss_ways[0] = Point2D_From_XY(curpos.row + 1, curpos.col);
        else { poss_ways[0] = Point2D_From_XY(-1, -1); }
    }

    /* Can curpos go up? */
    if (curpos.row <= 0)
        poss_ways[1] = Point2D_From_XY(-1, -1);
    else {
		int cell_idx = NCELL (m, curpos.row - 1, curpos.col);
        //if (maze[curpos.x-1][curpos.y] == ' ' || maze[curpos.x-1][curpos.y] == MAZE_EXIT_O)
		if (s[cell_idx] == MAZE_EMPTY_SPACE_32 || s[cell_idx] == MAZE_EXIT_O)
            poss_ways[1] = Point2D_From_XY(curpos.row-1, curpos.col);
        else { poss_ways[1] = Point2D_From_XY(-1, -1); }
    }

    /* Can curpos go left? */
    if (curpos.col <= 0)
        poss_ways[2] = Point2D_From_XY(-1, -1);
    else {
		int cell_idx = NCELL (m, curpos.row, curpos.col - 1);
        //if (maze[curpos.x][curpos.y-1] == ' ' || maze[curpos.x][curpos.y-1] == MAZE_EXIT_O)
		if (s[cell_idx] == MAZE_EMPTY_SPACE_32 || s[cell_idx] == MAZE_EXIT_O)
            poss_ways[2] = Point2D_From_XY(curpos.row, curpos.col-1);
        else { poss_ways[2] = Point2D_From_XY(-1, -1); }
    }

    /* Can curpos go right? */
    if (curpos.col >= m->cols - 1)
        poss_ways[3] = Point2D_From_XY(-1, -1);
    else {
		int cell_idx = NCELL (m, curpos.row, curpos.col + 1);
        //if (maze[curpos.x][curpos.y+1] == ' ' || maze[curpos.x][curpos.y+1] == MAZE_EXIT_O)
		if (s[cell_idx] == MAZE_EMPTY_SPACE_32 || s[cell_idx] == MAZE_EXIT_O)
            poss_ways[3] = Point2D_From_XY(curpos.row, curpos.col+1);
        else { poss_ways[3] = Point2D_From_XY(-1, -1); }
    }
}


/* ================================================================= */
/*                   MAIN AND SOLVER FUNCTIONS                       */
/* ================================================================= */

/* if there the maze is possible, will print the path in maze and will return
    a coordinate of value (-2, -2)
    the coordinate of value (-2, -2) is only used to check if the exit has been found.
    the program will then use the possibilities array to determine where the exit is. */
/* else, will return the starting coordinate */
void maze_iter_refine (mazegen_s *m)
{
    int row_moves[] = {-1, 1,  0, 0};
    int col_moves[] = { 0, 0, -1, 1};
	#define NUM_MOVES_4 4
	m->nummarks = 0;
	//m->iternums_opt_za = (int *)Z_MemDup_Z (m->iternums_za, m->sizeof_char_ray * 4);
	Mem_FreeNull_ (m->iternums_opt_za);
	m->iternums_opt_za = ZAlloc_Int32 (m->sizeof_char_ray * 4);

	stringlistfreecontents (&m->listsolvesteps); // Sequential list of 2d positions.
	stringlistfreecontents (&m->listkills); // Sequential list of 2d positions.
	m->num_ignores = 0;
	point2d_s curpos = m->startpos;
	while (1) {
		{
			int mark_idx = NCELL (m, curpos.row, curpos.col);
			m->iternums_opt_za[mark_idx] = ++m->nummarks;
			stringlistappendf (&m->listsolvesteps, "%d %d", curpos.row, curpos.col);
		}
		int cell_idx = NCELL(m, curpos.row, curpos.col);
		int ch = m->iternums_za[cell_idx];
		// Look at N S E W
		// If lookable dirs > 1 then
		// find the highest one.  Zero the other ones.

		int best_neighbor_value = 0;
		int best_neighbor_idx = not_found_neg1;
		point2d_s nextpos = {0};
		int num_ascend_neighbors = 0;
		for (int diro = 0; diro < NUM_MOVES_4; diro ++) {
			point2d_s pt = { curpos.row + row_moves[diro], curpos.col + col_moves[diro] };
			if (in_range_beyond (1, pt.row, m->rows - 1) && in_range_beyond (1, pt.col, m->cols - 1)) {
				int dir_cell_idx = NCELL(m, pt.row, pt.col);
				int dir_ch = m->iternums_za[dir_cell_idx];
				if (dir_ch > ch) {
					num_ascend_neighbors ++;
					if (dir_ch > best_neighbor_value) {
						best_neighbor_value = dir_ch;
						best_neighbor_idx = diro;
						nextpos = pt;
					} // best
				} // higher
			} // eligible (1 to rows-2, 1 to cols - 2)
		} // diro

		if (best_neighbor_idx == not_found_neg1)
			break; // DONE // End of the line

		if (num_ascend_neighbors > 1) {
			// LOCATE IGNORED AND MARK WITH -1
			for (int diro = 0; diro < NUM_MOVES_4; diro ++) {
				point2d_s pt = { curpos.row + row_moves[diro], curpos.col + col_moves[diro] };
				if (in_range_beyond (1, pt.row, m->rows - 1) && in_range_beyond (1, pt.col, m->cols - 1)) {
					int dir_cell_idx = NCELL(m, pt.row, pt.col);
					int dir_ch = m->iternums_za[dir_cell_idx]; // READ SOURCE
					if (dir_ch > ch) {
						if (dir_ch != best_neighbor_value) {
							m->iternums_opt_za[dir_cell_idx] = -1; // DESTROYED
							stringlistappendf (&m->listkills, "%d %d", pt.row, pt.col);
							Con_PrintLinef ("Killed [R %d C %d]", pt.row, pt.col);//, ++m->num_ignores);
						} // best
					} // higher
				} // eligible (1 to rows-2, 1 to cols - 2)
			} // diro
		} // multi-higher neighbors

		curpos = nextpos;

	} // while
}



point2d_s solve_recursive(mazegen_s *m, ccs *s, point2d_s curpos, point2d_s *psolvepos)
{
	if (m->iters_exit) {
		return curpos;
	}
	char *sw = (char *)s;
    if (curpos.row == -1)
        return curpos;

	int cell_idx = NCELL (m, curpos.row, curpos.col);
    //if (maze[curpos.x][curpos.y] == MAZE_EXIT_O)
	if (s[cell_idx] == MAZE_EXIT_O) {
		psolvepos->row = curpos.row;
		psolvepos->col = curpos.col;
        return Point2D_From_XY(-2, -2);
	}

    //maze[curpos.x][curpos.y] = MAZE_PATH_SOLVE_STAR; // '*'
	m->iters ++;
	if (m->iters > maze_max_iters.integer) {
		if (!m->iters_exit) {
			m->iters_exit = true;
			Con_PrintLinef (CON_CYAN "Exiting due to iters > %d", maze_max_iters.integer);
		}
		return curpos;
	}
	sw[cell_idx] = MAZE_PATH_SOLVE_STAR;
	m->iternums_za[cell_idx] = m->iters;

    /*print_maze(maze, lines, columns); printf("\n");*/


	point2d_s poss_pos[4];
    //possibilities(curpos, s, poss, lines, columns);
	// Find possible move directions
	Maze_CheckDirections (m, s, curpos, poss_pos);

    for (int directionx = 0; directionx < 4; directionx ++) {
        //point2d_s answer = solve_recursive(poss[directionx], maze, lines, columns);
		point2d_s answer = solve_recursive(m, s, poss_pos[directionx], psolvepos);
        if (answer.row == -1) /* can't go this way */
            continue;
        if (answer.row == -2) /* found answer */
            return answer;
    }

    /* if the program reaches this area, the solving algorithm is going backwards in the maze */
    //maze[curpos.x][curpos.y] = MAZE_EMPTY_SPACE_32;// ' ';
	m->iternums_za[cell_idx] = 0;
	sw[cell_idx] = MAZE_EMPTY_SPACE_32;
    return curpos;
}


