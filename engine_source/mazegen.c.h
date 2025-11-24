// mazegen.c.h

#include "mazegen_procs.c.h"
#include "mazegen_pathfind_2.c.h"
#include "mazegen_mazegen1.c.h"

static mazegen_s _maze = {0}, *maze = &_maze;

void SCR_maze_path_find_f (cmd_state_t *cmd)
{
}

void SCR_mazegen_f (cmd_state_t *cmd) // Seems to work
{
	mazegen_name_rows_cols_blots_blotmin_blotmax  (maze, maze_rows.integer, maze_columns.integer,
		maze_blots.integer, 5, 7, 5, 7);

	Con_PrintLinef (CON_GREEN "There is an answer to this maze.");

	print_maze (maze, maze->maze_za, CON_BRONZE);
	Con_PrintLinef (CON_CYAN "^^^^ Above: maze_za");
	print_maze (maze, maze->path_za, CON_BRONZE);
	Con_PrintLinef (CON_CYAN "^^^^ Above: path_za RAW");
	print_maze2 (maze, maze->path_za);
	Con_PrintLinef (CON_CYAN "^^^^ Above: path_za amalgam");

	Con_PrintLinef ("Improving solution  ...");
	maze_iter_refine (maze); // Creates lists.
	print_maze3 (maze, maze->path_za);

	Con_PrintLinef (CON_GREEN "There is an answer to this maze.");
	Con_PrintLinef (CON_GREEN "Exits at [R %d, C %d] - iters %d", maze->endpos.row, maze->endpos.col, maze->iters);
}

qbool Maze_To_Map_Is_Ok (mazegen_s *m, ccs *maze_model_filename, ccs *filename, qbool is_overwriteok);


// maze_save filename
void SCR_maze_save_f (cmd_state_t *cmd)
{
	ccs *savefilename1 = cmd_argv_parm1;
	qfile_t *f = NULL;

	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<savefilename.txt>", "saves current random maze to .txt")

	CMD_FAILEXIT_IF_ (!maze->maze_za, "No maze active");

	f = FS_OpenRealFile (savefilename1, "wb", fs_quiet_FALSE);
	CMD_FAILEXIT_IF2_ (!f, "Couldn't open file for writing " QUOTED_S, savefilename1);

	FS_PrintLinef (f, "%d", maze->rows);
	FS_PrintLinef (f, "%d", maze->cols);

	for (int row = 0; row < maze->rows; row ++) {
		for (int col = 0; col < maze->cols; col ++) {
			int cell_idx = NCELL (maze, row, col);
			FS_Printf (f, "%c", maze->maze_za[cell_idx]);
        } // col
		FS_Print (f, NEWLINE);
	} // row

	FS_CloseNULL_ (f);

	Con_PrintLinef ("Wrote " QUOTED_S, savefilename1);

fail_exit:
	FS_CloseNULL_ (f);
}

// maze_clear .. clears all maze data
void SCR_maze_clear_f (cmd_state_t *cmd)
{
	_maze_free (&_maze);

	memset (&_maze, 0, sizeof(_maze));

	Con_PrintLinef ("Maze cleared");
}



// June 10 2025: BUSTED PATH SOLVE.
void SCR_maze_load_f (cmd_state_t *cmd) // "test5.txt"
{
//	int argc = cmd_argc;
	ccs *loadfilename1 = cmd_argv_parm1;

	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<loadfilename.txt>", "Loads current random maze from .txt")

	_maze_free (&_maze);

	maze->maze_za = edusporto_loadfile_za(maze, loadfilename1);

	CMD_FAILEXIT_IF2_ (!maze->maze_za, "Couldn't load maze txt file %s", loadfilename1)

	maze->startpos	= Point2D_Eduo_Find_Char(maze, maze->maze_za, MAZE_ENTRANCE_I);
	maze->endpos	= Point2D_Eduo_Find_Char(maze, maze->maze_za, MAZE_EXIT_O);

	print_maze (maze, maze->maze_za, CON_BRONZE);

	Con_PrintLinef ("Above is unaltered maze");

	// Copy the maze
	maze->path_za = (char *)Z_MemDup_Z (maze->maze_za, maze->sizeof_char_ray);

	// Because edu uses those to check if found exit.
	Con_PrintLinef (CON_CYAN "Entrance to maze: [R %d, C %d]", maze->startpos.row, maze->startpos.col);

	print_maze (maze, maze->path_za, CON_GREEN);

	Con_PrintLinef (CON_BRONZE "Above is maze before solve.");

	// PATH OPTIMIZATION
	while (/*ALLOW BREAK FAILURE*/ 1) {
		point2d_s solvepos2 = {-1,-1};
		point2d_s exit_to_maze = {-1,-1};

		size_t sizeofiternums = (maze->rows * maze->cols + ONE_CHAR_1) * sizeof(int);

		Mem_FreeNull_ (maze->iternums_za);
		maze->iternums_za = ZAlloc_Int32 (sizeofiternums);

		solve_recursive(maze, maze->path_za, maze->startpos, &solvepos2);

		if (maze->iters_exit) {
			Con_PrintLinef ("Hit iters limit %d... failed maze_max_iters = %d", maze->iters, maze_max_iters.integer);
			break;
		}

		if (Point2D_Are_Equal(maze->startpos, exit_to_maze)) {
			Con_PrintLinef (CON_RED "There is no answer to this maze. Iters = %d", maze->iters);
			break;
		}

		Con_PrintLinef (CON_GREEN "There is an answer to this maze.");
		print_maze (maze, maze->path_za, CON_BLUE);

		print_maze2 (maze, maze->path_za);

		Con_PrintLinef ("Optimizing path  ...");
		maze_iter_refine (maze);
		print_maze3 (maze, maze->path_za);

		Con_PrintLinef (CON_GREEN "There is an answer to this maze.");
		Con_PrintLinef (CON_GREEN "Exits at [R %d, C %d] - iters %d", solvepos2.row, solvepos2.col, maze->iters);

		break;
	} // block

fail_exit:
	;
}


// maze_to_map modelname filename
void SCR_maze_to_map_f (cmd_state_t *cmd)
{
	//maze_to_map maps/maze_model_hell.map maps/mymap.map overwrite
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<maps/model.map> <maps/filename.map> [overwrite]", "Writes current maze to .map using model.map as the sample room");

	ccs		*modelname1			= cmd_argv_parm1;
	ccs		*filename1			= cmd_argv_parm2;
	qbool	is_overwrite_mode	= String_Match(cmd_argv_parm3, "overwrite");

	va_super (filename,  MAX_OSPATH_EX_1024, "%s", filename1);
	va_super (modelname, MAX_OSPATH_EX_1024, "%s", modelname1);

	if (String_Ends_With (filename,  ".map") == false)	c_strlcat (filename,  ".map");
	if (String_Ends_With (modelname, ".map") == false)	c_strlcat (modelname, ".map");

	Con_PrintVarString	(modelname);
	Con_PrintVarString	(filename);
	Con_PrintVarInt		(is_overwrite_mode);

	// maps/maze_model_hell.map
	qbool isok = Maze_To_Map_Is_Ok (maze, modelname, filename, is_overwrite_mode);

	if (!isok) return; // An error message printed, just leave.

	Con_PrintLinef ("Wrote %s", filename);
}

// "maps/maze_model_hell.map";
qbool Maze_To_Map_Is_Ok (mazegen_s *m, ccs *maze_model_filename, ccs *filename, qbool is_overwriteok)
{
	qbool bret = false;
	double clock_start = Sys_DirtyTime ();

	entitylist_t list_model = {0};
	entitylist_t list_map1 = {0};

	CMD_FAILEXIT_IF_ (!m->maze_za, "No maze loaded")

	qbool bfilenameexists = FS_File_Exists(filename);

	CMD_FAILEXIT_IF2_ (bfilenameexists && is_overwriteok == false, "Filename %s already exists and allow overwrite is false", filename)

	{
		char *sin_model_za = FS_LoadFileQuick (maze_model_filename);
		CMD_FAILEXIT_IF2_ (!sin_model_za, "Couldn't load maze model %s", maze_model_filename)

		int isok = entitylist_parsemap_metric (&list_model, sin_model_za);
		Mem_FreeNull_ (sin_model_za); // Done with this.

		CMD_FAILEXIT_IF2_ (!isok, "Couldn't parse maze model map %s", maze_model_filename)
	}

examine_maze_model:
    ;
	int north_wall_entnum = entitylist_find_key_value_ex(&list_model, "classname", "func_group_north_wall", AFTER_WORLD_1, list_model.count);
	int floor_wall_entnum = entitylist_find_key_value_ex(&list_model, "classname", "func_group_floor_wall", AFTER_WORLD_1, list_model.count);
	entityx_t *north = north_wall_entnum == not_found_neg1 ? NULL : &list_model.entity[north_wall_entnum];
	entityx_t *floor = floor_wall_entnum == not_found_neg1 ? NULL : &list_model.entity[floor_wall_entnum];

	float wall_height = north ? north->bedims[2] : 0;
	float tile_size = floor ? floor->bedims[0] : 0;

	CMD_FAILEXIT_IF_ (tile_size == 0,	"Tile size is 0 in the layout model room.")
	CMD_FAILEXIT_IF_ (wall_height == 0,	"Wall height is 0  in the layout model room.")

	Con_PrintVarInt (tile_size);
	Con_PrintVarInt (wall_height);

construct_world:
;
	// CONSTRUCT WORLD
	entityx_t *d_ent_world = entitylist_add (&list_map1);
	//stringlistappendf2	(&d_ent_world->pairslist, "classname", "%s", "worldspawn"); // key / val
	entity_add_key_value_unsafe_f (d_ent_world,	"classname",	"worldspawn");
	entity_add_key_value_unsafe_f (d_ent_world,	"_keeplights",	"1");
	entity_add_key_value_unsafe_f (d_ent_world, "worldtype",	"0");
	entity_add_key_value_unsafe_f (d_ent_world, "_blocksize",	"2048 2048 2048");

	// ROWS - Are ascending, we need them descending
	// COLS - Are ascending
	vec3_t worldsize = {0}, worldmins = {0}, worldmaxs = {0};
	worldsize[0]	= m->rows * tile_size; // 32 * 256 = 8192
	worldsize[1]	= m->cols * tile_size; // 32 * 256 = 8192
	worldsize[2]	= wall_height; // 32 * 256 = 8192

	Con_PrintVarInt			(m->rows);
	Con_PrintVarInt			(m->cols);
	Con_PrintVarVector3		(worldsize);

	worldmins[0]	= - (worldsize[0] / 2.0);
	worldmins[1]	= - (worldsize[1] / 2.0);
	worldmins[2]	= 0;

	worldmaxs[0]	=   (worldsize[0] / 2.0);
	worldmaxs[1]	=   (worldsize[1] / 2.0);
	worldmaxs[2]	= 0;

	Con_PrintVarVector3		(worldmins);

	qbool is_solid_colmax2 = is_solid_column (m, m->cols - 2);
	qbool is_solid_rowmax2 = is_solid_row (m, m->rows - 2);
	qbool is_solid_colmin2 = is_solid_column (m, 2);
	qbool is_solid_rowmin2 = is_solid_row (m, 2);

	// Maze generator is stupid and can make extra solid walls
	int rowmin = 1;  if (is_solid_rowmin2) rowmin ++;
	int colmin = 1;  if (is_solid_colmin2) colmin ++;
	int rowmax = m->rows - 1;  if (is_solid_rowmax2) rowmax --;
	int colmax = m->cols - 1;  if (is_solid_colmax2) colmax --;

	Con_PrintVarInt	(rowmin);
	Con_PrintVarInt	(colmin);
	Con_PrintVarInt	(rowmax);
	Con_PrintVarInt	(colmax);

	vec3_t vcursor;
	vcursor[0] =  worldmins[0];
	vcursor[1] = -worldmins[1] - tile_size; // -4096 ==> 4096
	vcursor[2] = 0;

	qbool did_info_player_start = false;

	static int iters;

	for (int row = rowmin; row <= rowmax; row ++, vcursor[1] += (-tile_size)) {
		vcursor[0] =  worldmins[0];	// Reset
		for (int col = colmin; col < colmax; col ++, vcursor[0] += tile_size) {
			int cell_idx = NCELL (m, row, col);
			int ch = m->maze_za[cell_idx];

			int nsbits;
			// Stops before "I" and "O" just in case?
			if (in_range_beyond('A', ch, 'H')) {
				nsbits = nsew_bits_floor_512 | nsew_bits_ceil_256;
			} else {
				// Solid returns pure 0.
				nsbits = (int)walls_for_cell (m, m->maze_za, row, col, rowmin, rowmax, colmin, colmax);
			}
			//Con_PrintLinef ("RC %3d, %3d bits = %d", row, col, nsbits & 15);

			if (nsbits == nsew_bits_solid_neg1)
				continue; // SOLID

			if (did_info_player_start == false) {
				entityx_t *xs = entitylist_add (&list_map1);
				vec3_t vorg;
				VectorCopyDestSrc (vorg, vcursor);
				vorg[0] += tile_size / 2.0;
				vorg[1] += tile_size / 2.0;
				vorg[2] = 64;
				entity_add_key_value_unsafe_f (xs, "classname", "info_player_start");
				entity_add_key_value_unsafe_f (xs, "origin", VECTOR3_G, VECTOR3_SEND(vorg));

				did_info_player_start = true;
			}

			ccs *direction_moves[] = {"n", "s", "e", "w", "c", "f"};
			int bit_moves[] = {
				nsew_bits_north_1,
				nsew_bits_south_2,
				nsew_bits_east_4,
				nsew_bits_west_8,
				nsew_bits_ceil_256,
				nsew_bits_floor_512,
				//nsew_bits_north_1 + nsew_bits_south_2 + nsew_bits_east_4 + nsew_bits_west_8, // alpha move
			};
			int num_moves = ARRAY_COUNT (bit_moves);
			for (int move_num = 0; move_num < num_moves; move_num ++) {
				int this_bit_move = bit_moves[move_num];
				ccs *direction_move = direction_moves[move_num];
loop_nonworld_entities:
				// Ok ... the nasty.  Have to add alpha brushes as func_group
				// Add to what though ... specific classnames.
				while (Have_Flag (nsbits, this_bit_move)) {
					int start_idx = AFTER_WORLD_1, beyond_idx = list_model.count;

					while (1) { // // Find all entities in model with 'n' - NORTH
						ccs *nsew_keyval = NULL; iters ++;
						int entnum = entitylist_find_key_any_value (&list_model, "_nsew", start_idx, beyond_idx, &nsew_keyval);
						if (entnum == not_found_neg1) break; // NO MORE MATCHES
						start_idx = entnum + 1; // Move beyond

						entityx_t *me = &list_model.entity[entnum];
						//ccs *_nsew = entity_key_get_value (me, "_nsew");

						if (!String_Contains (nsew_keyval, direction_move))
							continue;

						ccs *s_chance = entity_key_get_value (me, "_chance");
						if (s_chance) {
							float chance = atof(s_chance);
							float mynum = lhrandom (0, 1);
							if (mynum > chance) continue; // Failed dice roll.
						}

						//ccs *classname = entity_key_get_value (me, "classname");

						// We have an entity.
						entityx_t *xn = entitylist_add_pasting_this_ent (&list_map1, me);
						entity_translate_with_metric (xn, vcursor);

						entity_add_key_value_unsafe_f (xn, "_rowcol", "RC_%d_%d"	, row, col);

						ccs *_alphawanted= entity_key_get_value (me, "_alphawanted");

						if (_alphawanted) {
							// Add alpha brushes to func group at all 8 points.  Too many is fine for now.
							// Calc the alpha how?
							// do all 8 corners adding a brush with appropriate alpha texture
							// redo the common/alpha shaders with a better naming convention.
							// bbmin range to bbmax pct for each corner.
							entity_add_alpha_brushes (xn, worldmins, worldmaxs, worldsize);

						} // Specific classnames

						ccs *_lightpattern = entity_key_get_value (me, "_lightpattern");
						while (_lightpattern) {
							entityx_t *w_ent = &list_model.entity[ENTITY_WORLD_0];
							// Override color based on origin.
							int ival = atoi (_lightpattern);
							// Find the lightpattern
							va_super (lightpatternfind, 128, "_lightpattern%d", ival);
							ccs *wlightpatternval = entity_key_get_value (w_ent, lightpatternfind);
							//entity_key_set_vector_force (,"_color", lightvector);
							//"_lightpattern" 1 4000 distfrom 0 0 0 0.97 0.21 0.07 0.1 0.8 0.5
							if (!wlightpatternval) {
								Con_PrintLinef ("Cannot locate %s in world", lightpatternfind);
								break;
							}

							// world:
							// "_lightpattern1"
							// Radius where we hit 0.
							//
							//  0     1       2 3 4 5    6    7     8   9  10
							//  4000 distfrom 0 0 0 0.97 0.21 0.07 0.1 0.8 0.5
							stringlist_t list = {0};
							stringlistappend_split (&list, wlightpatternval, SPACER);
							if (list.numstrings != 11) {
								Con_PrintLinef ("World %s does not have 11 args in value", lightpatternfind);
								stringlistfreecontents (&list);
								break;
							}
							float radius = atof(list.strings[0]);
							if (radius <= 0) {
								Con_PrintLinef ("World %s has invalid radius of %g", lightpatternfind, radius);
								stringlistfreecontents (&list);
								break;
							}
							vec3_t vcenter = { atof(list.strings[2]), atof(list.strings[3]), atof(list.strings[4]) };
							vec3_t color0 = { atof(list.strings[5]), atof(list.strings[6]), atof(list.strings[7]) };
							vec3_t color1 = { atof(list.strings[8]), atof(list.strings[9]), atof(list.strings[10]) };
							stringlistfreecontents (&list);

							vec3_t eorg = {0};
							qbool isfound = entity_key_get_vector_isok (xn, "origin", eorg);
							if (!isfound) {
								Con_PrintLinef ("World %s has invalid radius of %g", lightpatternfind, radius);
								break;
							}

							float dist = VectorDistance (eorg, vcenter);
							float pct = dist / radius;
							pct = bound (0, pct, 1);
							vec3_t colorblended;
							colorblended[0] = color0[0] * pct + color1[0] * (1.0-pct);
							colorblended[1] = color0[1] * pct + color1[1] * (1.0-pct);
							colorblended[2] = color0[2] * pct + color1[2] * (1.0-pct);
							entity_key_set_vector_force (xn, "_color", colorblended);

							break;
						} // lightpattern while if


					} // while finding north entities.
					break;
				} // north while if
			} // each direction


		} // col

	} // r

	double clock_map_done = Sys_DirtyTime ();

	Con_PrintLinef ("Elapsed: %g", clock_map_done - clock_start);

	{
		baker_string_t *bsout = entitylist_maptext_bsalloc (&list_map1);
		ccs *s_size = String_Num_To_Thousands_Sbuf (bsout->length);
		Con_PrintLinef (".Map file bytes = %s", s_size);
		ccs *s_size_mb = String_Num_To_Thousands_Sbuf (bsout->length / (1024 * 1024));
		Con_PrintLinef (".Map file = %s MB", s_size_mb);

		CMD_FAILEXIT_IF_ (!bsout, "Couldn't export entitylist to text") // Can't happen, right?

		// RE-WRITE
		int isok = FS_WriteFileStringQuick (filename, bsout->string);
		BakerString_Destroy_And_Null_It (&bsout);
		CMD_FAILEXIT_IF2_ (!isok, "Failed to write %s", filename) // Can't happen, right?
	}

	double clock_write_done = Sys_DirtyTime ();

	Con_PrintLinef ("Elapsed: %g", clock_write_done - clock_start);

	bret = true; // SUCCESS

fail_exit:
	entitylistfreecontents	(&list_map1);
	entitylistfreecontents	(&list_model);
	return bret;
}




void SCR_csgtool_f (cmd_state_t *cmd)
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (2, "<maps/model.map> <brushfacer or ...> ", "Performs CSG tool re-write function on specified map");

	entitylist_t list_map1 = {0};
	ccs		*filename1		= cmd_argv_parm1;	// maps/mymap.map or maps/mymap
	ccs		*toolcommand	= cmd_argv_parm2;	// brushfacer

	va_super (filename,  MAX_OSPATH_EX_1024, "%s", filename1);
	if (String_Ends_With (filename,  ".map") == false)	c_strlcat (filename,  ".map");

	qbool bfilenameexists = FS_File_Exists(filename);
	double time0 = Sys_DirtyTime ();

	CMD_FAILEXIT_IF2_ (bfilenameexists == false, "Filename %s does not exist to read", filename)

	char *sin_za = FS_LoadFileQuick (filename);
	CMD_FAILEXIT_IF2_ (!sin_za, "Couldn't %s", filename)

	int isok = entitylist_parsemap_metric (&list_map1, sin_za);
	Mem_FreeNull_ (sin_za); // Done with this.

	CMD_FAILEXIT_IF2_ (!isok, "Couldn't parse maze model map %s", filename)

	qbool bop_ok = false;
	void TimeString_Fill_YYYYMMDD_HHMMSSAM (char *timestring, size_t timestring_size);
	char timestring[64]; TimeString_Fill_YYYYMMDD_HHMMSSAM (timestring, sizeof(timestring));
	va_super (timestring_newval, 128, "0 - %s", timestring);  // looks like "0 - 20241005 10:12 AM" //char *snewval_za = Z_StrDupf ("0 - %s", timestring); 


operations:

	// Currently pins common/terrain with alpha brushes.
	if (String_Match (toolcommand, "_csg_test_rewrite")) {
		//int num_done = 1;
		//CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Rewrite makes no changes");

	} else if (String_Match (toolcommand, "pincommonterrain")) {
		int num_done = entitylist_perform_pincommonterrain_num_done (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "decal_automatic")) {
		int num_done = entitylist_perform__decal__automatic (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "brushfacer")) {
		int num_done = entitylist_perform_brushfacer_world_only (&list_map1, /*&num_faces*/ NULL);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "collision_body")) {
		int num_done = entitylist_perform_collision_body (&list_map1, timestring_newval);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "concat_ents_clipboard")) {
		int num_done = entitylist_perform_concat_ents_clipboard (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "caulkworld")) {
		int num_done = entitylist_caulk_world_regular_textures (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "brushunfacer_scratch1")) {
		int num_done = entitylist_perform_unbrushfacer_scratch1_world_only (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "cubeit")) {
		int num_done = entitylist_perform_cubeit (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "adjacents")) {
		int num_done = entitylist_perform_adjacents_num_done (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "noflat_to_flat")) {
		int num_done = entitylist_perform_noflat_to_flat_num_done (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "flatzerosettexture")) {
		float z = 0.0;
		int num_done = entitylist_perform_flatzerosettexture_num_done (&list_map1, z);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "infodump")) {
		//int num_done =
		entitylist_perform_infodump (&list_map1);
		bop_ok = true;
		Con_PrintLinef ("Infodump shall not write ... skipping");
		goto fail_exit;

	} else if (String_Match (toolcommand, "removenondominanttexturebrushes")) {
		int num_done = entitylist_perform_removenondominanttexturebrushes (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else if (String_Match (toolcommand, "weld")) {
		int num_done = entitylist_perform_weld_num_done (&list_map1);
		CMD_FAILEXIT_IF2_ (num_done == 0, "Operation made num changes == %d", num_done);
		bop_ok = true;	Con_PrintLinef ("Num changes = %d", num_done);

	} else {
		CMD_FAILEXIT_IF2_ (1, "Tool %s is unknown", toolcommand);
	}

	CMD_FAILEXIT_IF_ (!bop_ok, "Operation failed");

	double time1 = Sys_DirtyTime ();
	double deltatime = time1 - time0;
	Con_PrintLinef ("Op time is %g secs", deltatime);

	// Write the file
	{
		baker_string_t *bsout = entitylist_maptext_bsalloc (&list_map1);
		ccs *s_size = String_Num_To_Thousands_Sbuf (bsout->length);
		Con_PrintLinef (".Map file bytes = %s", s_size);
		ccs *s_size_mb = String_Num_To_Thousands_Sbuf (bsout->length / (1024 * 1024));
		Con_PrintLinef (".Map file = %s MB", s_size_mb);

		CMD_FAILEXIT_IF_ (!bsout, "Couldn't export entitylist to text") // Can't happen, right?

		// RE-WRITE
		int isok = FS_WriteFileStringQuick (filename, bsout->string);
		BakerString_Destroy_And_Null_It (&bsout);
		CMD_FAILEXIT_IF2_ (!isok, "Failed to write %s", filename) // Can't happen, right?
	}

	double time2 = Sys_DirtyTime ();
	double deltatime2 = time2 - time1;


	Con_PrintLinef ("Wrote %s in %g seconds", filename, deltatime2);

fail_exit:
	Mem_FreeNull_ (sin_za); // Done with this.
	entitylistfreecontents	(&list_map1);
}





