// mazegen_procs.c.h


RELATED_ (nsewcf_north_1)

#define MAZE_ENTRANCE_I			'I'
#define MAZE_EXIT_O				'O'
#define MAZE_EMPTY_SPACE_32		' '
#define MAZE_PATH_SOLVE_STAR	'*'
#define MAZE_SOLID_POUNDER		'#'


typedef struct {
    int row, col;
} point2d_s;


typedef struct {
	vec3_t	mbbmins;
	vec3_t	mbbmaxs;
} mazebbox_s;

#if 0

//int					batchsize;		/* If not specified, defaults 128 */
//int					numitems;
//int					maxitems;
//MYPTRTYPE			**pointers_array;			/* */
//size_t				datasize;

#endif

DEF_COMPAT_ARRAY_STRUCT (mazebbox_array_s, mazebbox_s);

typedef struct {
	int						rows;
	int						cols;
	size_t					sizeof_char_ray;
	size_t					sizeof_int_ray;

	int						iters;							// Detects unsolvable A-B.
	qbool					iters_exit;

	char					*maze_za;						// The maze
	char					*path_za;						// The path finder work buffer
	int						*iternums_za;					// Index work queue
	int						*iternums_opt_za;				// Optimized work queue.

	stringlist_t			listsolvesteps;					// List to solve the map
	stringlist_t			listkills;						// List of ones destroyed by path improvement.
	int						num_ignores;
	int						nummarks;

	point2d_s				startpos;
	point2d_s				endpos;

	mazebbox_array_s		blot_boxes_ba;						// mazebbox_s type manual alloc (BakerArray_Owned_Clear_)
} mazegen_s;

int NCELL(mazegen_s *m, int r, int c)
{
	return m->rows * r + c;
}


// Function to initialize the maze grid
void initialize_maze(mazegen_s *m, char *s)
{
	for (int row = 0; row < m->rows; row ++) {
		for (int col = 0; col < m->cols; col ++) {
			int cell_idx = row * m->rows + col;
			s[cell_idx] = MAZE_SOLID_POUNDER;
		} // col
	} // row
}






// Must be valid.  x1, y1 is not beyond but the last
void blot (mazegen_s *m, char *s, int r0, int rb, int c0, int cb, int chadd)
{
	char ch = 65  + chadd;
	int r1 = rb - 1;
	int c1 = cb - 1;
	// Center is filled with T (one square of empty border)
	// This helps the path finder.
	for (int col = c0; col <= c1; col ++) {
		for (int row = r0; row <= r1; row ++) {
			int cell_idx = row * m->rows + col;
			s[cell_idx] = ch; //'T'; // Represent walls with '#'
		} // for
	} // for
	for (int col = c0; col <= c1; col ++) {
		for (int row = r0; row <= r1; row ++) {
			if (!(col == c0 || row == r0 || col == c1 || row == r1))
				continue;
			int cell_idx = row * m->rows + col;
			s[cell_idx] = MAZE_EMPTY_SPACE_32; // Represent walls with '#'
		} // for
	} // for
}


// Function to generate the maze using recursive backtracking
void generate_maze(mazegen_s *m, char *s, int row, int col) {
	// Mark the current cell as a path

	int cell_idx = row * m->rows + col;
	s[cell_idx] = MAZE_EMPTY_SPACE_32;

	// Randomly choose an order of directions to explore
	int directions[4] = {0, 1, 2, 3}; // 0: up, 1: right, 2: down, 3: left
	for (int i = 0; i < 4; i++) {
		int j = rand() % (4 - i);
		int temp = directions[i];
		directions[i] = directions[j];
		directions[j] = temp;
	}

	// Try each direction
	for (int i = 0; i < 4; i++) {
		int new_row, new_col;
		// Determine the new cell based on the direction
		if (directions[i] == 0) { // Up
		  new_row = row - 2;
		  new_col = col;
		} else if (directions[i] == 1) { // Right
		  new_row = row;
		  new_col = col + 2;
		} else if (directions[i] == 2) { // Down
		  new_row = row + 2;
		  new_col = col;
		} else { // Left
		  new_row = row;
		  new_col = col - 2;
		}

		// If the new cell is within bounds and is a wall, carve a path
		//if (new_row >= 0 && new_row < SIZE && new_col >= 0 && new_col < SIZE &&
		int cell_idx_new = new_row * m->rows + new_col;
		if (in_range_beyond (1, new_row, m->rows - 1) && in_range_beyond (1, new_col, m->cols - 1) &&
			s[cell_idx_new] == MAZE_SOLID_POUNDER) {
				int cell_idx_carve = (row - (row - new_row) / 2)* m->rows + (col - (col - new_col) / 2);
				s[cell_idx_carve] = MAZE_EMPTY_SPACE_32; // Carve path between cells
			generate_maze(m, s, new_row, new_col); // Recursively generate the maze from the new cell
		} // if
	} // for
}

// Function to print the maze
void print_maze(mazegen_s *m, ccs *s, ccs *coloring) {
	for (int row = 0; row < m->rows; row ++) {

		//	Con_Printf (CON_BRONZE);
		for (int col = 0; col < m->cols; col ++) {
			int cell_idx = row * m->rows + col;
			if (coloring)
				Con_Printf ("%s%c", coloring, s[cell_idx]);
			else
				Con_Printf ("%c", s[cell_idx]);
		}
		Con_Printf(NEWLINE);
	}
}

void print_maze2(mazegen_s *m, ccs *s) {
	for (int row = 0; row < m->rows; row ++) {
		//	Con_Printf (CON_BRONZE);
		for (int col = 0; col < m->cols; col ++) {
			int cell_idx = row * m->rows + col;
			if (s[cell_idx] == '*') {
				char *setty="0123456789ABCDEF";
				int hue = m->iternums_za[cell_idx] % 16;
				va_super (scolor, 15, "^xF%x%x", hue/4 , hue/4);
				char scharo[2] = {0};
				scharo[0] = setty[hue];
				//Con_Printf ("%s%c", scolor, 65 + hue);
				Con_Printf ("%s%s", scolor, scharo);
			} else
				Con_Printf (CON_WHITE "%c", s[cell_idx]);
		}
		Con_Printf(NEWLINE);
	}
}

void print_maze3(mazegen_s *m, ccs *s) {
	for (int row = 0; row < m->rows; row ++) {

		//	Con_Printf (CON_BRONZE);
		for (int col = 0; col < m->cols; col ++) {
			int cell_idx = row * m->rows + col;
			int chx = m->iternums_opt_za[cell_idx];
			if (chx == -1) {
				Con_Printf (CON_WHITE "X");
				continue;
			} else if (s[cell_idx] == '*') {
				if (chx == 0) { // Ignored
					Con_Printf (CON_WHITE "_");
				} else {

					char *setty="0123456789ABCDEF";
					int hue = m->iternums_opt_za[cell_idx] % 16;
					va_super (scolor, 15, "^xF%x%x", hue/4 , hue/4);
					char scharo[2] = {0};
					scharo[0] = setty[hue];
					//Con_Printf ("%s%c", scolor, 65 + hue);
					Con_Printf ("%s%s", scolor, scharo);
				}
			} else
				Con_Printf (CON_BRONZE "%c", s[cell_idx]);
		}
		Con_Printf(NEWLINE);
	}
}



// Function to check if a cell is valid
qbool IsValid_Empty(mazegen_s *m, ccs *s, int r, int c)
{
	if (!in_range_beyond (0, r, m->rows - 1))
		return false;

	if (!in_range_beyond (0, c, m->cols - 1))
		return false;

	int cell_idx = r * m->rows + c;
	int ch = s[cell_idx];
    return ch == MAZE_EMPTY_SPACE_32;
}


char *edusporto_loadfile_za (mazegen_s *m, ccs *filename)
{
	//Mem_FreeNull_ (m->maze_za);
	//Mem_FreeNull_ (m->maze_sa);
	//char *sfile_za =  FS_LoadFileString_Unsafe_ZAlloc (filename);
	char *sfile_za =  FS_LoadFileQuick(filename);

	CMD_RETURN_NULL_IF_FMT_ (!sfile_za, "Couldn't open %s", filename)

	char *sout = NULL;
	stringlist_t list = {0};
	stringlistappend_split_lines_cr_scrub(&list, sfile_za);
	Mem_FreeNull_ (sfile_za);

	CMD_FAILEXIT_IF_(list.numstrings < 3, "Too few lines to be valid")

	m->rows = atoi(list.strings[0]);
	m->cols = atoi(list.strings[1]);

	m->sizeof_char_ray = (m->rows * m->cols + ONE_CHAR_1) * sizeof(char);
	sout = ZAlloc_Char (m->sizeof_char_ray);

	for (int row = 0; row < m->rows; row ++) {
		ccs *sxy = list.strings[row + 2];
		for (int col = 0; col < m->cols; col ++) {
			int cell_idx = NCELL (m, row, col);
			int ch = sxy[col];
			sout[cell_idx] = ch;
        } // col
	} // row

fail_exit:
	stringlistfreecontents (&list);
	return sout;
}

point2d_s Point2D_From_XY(int x, int y) {
    point2d_s ret = {x, y};
    return ret;
}

// LOAD maze
// SAVE maze
qbool is_solid_column (mazegen_s *m, int col)
{
	int num_solid = 0;
	for (int row = 0; row < m->rows; row ++) {
		int idx = NCELL (m, row, col);
		int ch = m->maze_za[idx];
		if (!isin6 (ch, MAZE_EMPTY_SPACE_32, MAZE_ENTRANCE_I, MAZE_ENTRANCE_I, 'A', 'B', 'C'))
			num_solid ++;
	} // row
	return num_solid == m->rows;
}

qbool is_solid_row (mazegen_s *m, int row)
{
	int num_solid = 0;
	for (int col = 0; col < m->cols; col ++) {
		int idx = NCELL (m, row, col);
		int ch = m->maze_za[idx];
		if (!isin6 (ch, MAZE_EMPTY_SPACE_32, MAZE_ENTRANCE_I, MAZE_ENTRANCE_I, 'A', 'B', 'C'))
			num_solid ++;
	} // col
	return num_solid == m->cols;
}



nsew_bits_e walls_for_cell (mazegen_s *m, ccs *dataz, int row, int col, int rowmin, int rowmax, int colmin, int colmax)
{
	int idx = NCELL (m, row, col);
	int ch = dataz[idx];

	// If NOT ... so what hits here?  POUNDER ...
	if (!isin6 (ch, MAZE_EMPTY_SPACE_32, MAZE_ENTRANCE_I, MAZE_ENTRANCE_I, 'A', 'B', 'C'))
		return nsew_bits_solid_neg1; // SOLID!
	int bits = nsew_bits_ceil_256 + nsew_bits_floor_512; // Floor and ceil are free.
	//if (row > 1) {
	if (row >= rowmin) {
		int idx = NCELL (m, row - 1, col);
		int ch = dataz[idx];
		if (ch != MAZE_EMPTY_SPACE_32 && !in_range('A', ch, 'H'))
			Flag_Add_To (bits, nsew_bits_north_1);
	}
	if (col >= colmin) {
		int idx = NCELL (m, row, col - 1);
		int ch = dataz[idx];
		if (ch != MAZE_EMPTY_SPACE_32 && !in_range('A', ch, 'H'))
			Flag_Add_To (bits, nsew_bits_west_8);
	}

	if (row <= rowmax ) {
		int idx = NCELL (m, row + 1, col);
		int ch = dataz[idx];
		if (ch != MAZE_EMPTY_SPACE_32 && !in_range('A', ch, 'H'))
			Flag_Add_To (bits, nsew_bits_south_2);
	}

	if (col <= colmax ) {
		int idx = NCELL (m, row, col + 1);
		int ch = dataz[idx];
		if (ch != MAZE_EMPTY_SPACE_32 && !in_range('A', ch, 'H'))
			Flag_Add_To (bits, nsew_bits_east_4);
	}

	if (row >= rowmin && col >= colmin) {
		int idx = NCELL (m, row - 1, col);
		int ch = dataz[idx];
		if (ch != MAZE_EMPTY_SPACE_32 && !in_range('A', ch, 'H'))
			Flag_Add_To (bits, nsew_bits_north_1);
	}
	if (col >= colmin) {
		int idx = NCELL (m, row, col - 1);
		int ch = dataz[idx];
		if (ch != MAZE_EMPTY_SPACE_32 && !in_range('A', ch, 'H'))
			Flag_Add_To (bits, nsew_bits_west_8);
	}

	return (nsew_bits_e)bits;
}

point2d_s Point2D_Find_First_Empty(mazegen_s *m, ccs *s)
{
	point2d_s result2d = {-1, -1};
	for (int row = 0; row < m->rows; row ++) {
		for (int col = 0; col < m->cols; col ++) {
			int cell_idx = NCELL(m, row, col);
			if (s[cell_idx] == MAZE_EMPTY_SPACE_32) {
				result2d = Point2D_From_XY(row, col);
				return result2d;
			}
		} // col
	} // row

	return result2d; /* did not find*/
}

point2d_s Point2D_Find_Last_Empty(mazegen_s *m, ccs *s)
{
	point2d_s result2d = {-1, -1};
	for (int row = m->rows - 1; row >= 0; row --) {
		for (int col = m->cols - 1; col >= 0; col --) {
			int cell_idx = NCELL(m, row, col);
			if (s[cell_idx] == MAZE_EMPTY_SPACE_32) {
				result2d = Point2D_From_XY(row, col);
				return result2d;
			}
		} // col
	} // row

	return result2d; /* did not find*/
}


/* returns a coordinate with -1 as x and -1 as y if the entrance is not found */
// Baker: Finds "I" (O is exit)
point2d_s Point2D_Eduo_Find_Char(mazegen_s *m, ccs *s, int find_ch)
{
    point2d_s result2d = {-1, -1};

    for (int row = 0; row < m->rows; row ++) {
		int cell_idx;
		// Check column 0 of each row
		cell_idx = NCELL(m, row, 0);
        if (s[cell_idx] == find_ch) {
            result2d = Point2D_From_XY(row, 0);
            return result2d;
        }
		// Check column last of each row.
		cell_idx = NCELL(m, row, m->cols - 1);
        if (s[cell_idx] == find_ch) {
            result2d = Point2D_From_XY(row, m->cols - 1);
            return result2d;
        }
    }
    for (int col = 0; col < m->cols; col ++) {
		int cell_idx;
		// Check row 0
		cell_idx = NCELL(m, 0, col);
        if (s[cell_idx] == find_ch) {
            result2d = Point2D_From_XY(0, col);
            return result2d;
        }
		// Check row last
		cell_idx = NCELL(m, m->rows - 1, col);
        if (s[cell_idx] == find_ch) {
            result2d = Point2D_From_XY(m->rows - 1, col);
			return result2d;
        }
    }

    return result2d; /* did not find an entrance */
}

point2d_s Point2D_Eduo_Find_Entrance(mazegen_s *m, ccs *s, int find_ch)
{
    point2d_s entrance2d = {-1, -1};

    for (int row = 0; row < m->rows; row ++) {
		int cell_idx;
		// Check column 0 of each row
		cell_idx = NCELL(m, row, 0);
        if (s[cell_idx] == MAZE_ENTRANCE_I) {
            entrance2d = Point2D_From_XY(row, 0);
            return entrance2d;
        }
		// Check column last of each row.
		cell_idx = NCELL(m, row, m->cols - 1);
        if (s[cell_idx] == MAZE_ENTRANCE_I) {
            entrance2d = Point2D_From_XY(row, m->cols - 1);
            return entrance2d;
        }
    }
    for (int col = 0; col < m->cols; col ++) {
		int cell_idx;
		// Check row 0
		cell_idx = NCELL(m, 0, col);
        if (s[cell_idx] == MAZE_ENTRANCE_I) {
            entrance2d = Point2D_From_XY(0, col);
            return entrance2d;
        }
		// Check row last
		cell_idx = NCELL(m, m->rows - 1, col);
        if (s[cell_idx] == MAZE_ENTRANCE_I) {
            entrance2d = Point2D_From_XY(m->rows - 1, col);
        }
    }

    return entrance2d; /* did not find an entrance */
}

qbool Point2D_Are_Equal(point2d_s coord1, point2d_s coord2) {
    if ((coord1.row == coord2.row) && (coord1.col == coord2.col))
        return true;
    return false;
}


static void _maze_free (mazegen_s *m)
{
	Mem_FreeNull_ (m->iternums_opt_za);
	Mem_FreeNull_ (m->iternums_za);
	Mem_FreeNull_ (m->path_za);
	Mem_FreeNull_ (m->maze_za);

	stringlistfreecontents (&m->listkills);
	stringlistfreecontents (&m->listsolvesteps);

	BakerArray_Owned_Clear (m->blot_boxes_ba);

	memset (m, 0, sizeof(*m));
}
