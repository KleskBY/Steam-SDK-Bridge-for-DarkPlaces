// prvm_dungeon_header.c.h

typedef enum {
	stage_none_0 = 0,
	stage_csg_1 = 1,
	stage_bsp_2 = 2,
	stage_vis_3 = 3,
	stage_lit_4 = 4,
} stage_e;

typedef struct {
	char gamedir[MAX_QPATH_128];					// dungeon
	char fp_gamedir[MAX_QPATH_128];					// c:\quak\dungeon (no trail) working dir for map compiler or it won't find textures

	char maps_shell_dot_map[MAX_QPATH_128];			// maps/shell.map

	char quake_folder_trailslash[MAX_OSPATH_EX_1024];// c:\quak\ //
	char mapbasename[MAX_OSPATH_EX_1024];			// mymap
	char fp_map_no_ext[MAX_OSPATH_EX_1024];			// C:/quak/dungeon/maps/mymap
	char rp_map_dot_map[MAX_QPATH_128];				// maps/mymap.map
	char fp_map_no_ext_win32[MAX_OSPATH_EX_1024];	// C:\quak\dungeon\maps\mymap // win32 style
	char fp_q3map2_exe[MAX_OSPATH_EX_1024];			// C:/quak/dungeon/q3map2/q3map2_64_light_auto.exe
	char fp_zircon_exe[MAX_OSPATH_EX_1024];			// C:/quak/zircon_beta_gcc.exe

	char csg_args[MAX_OSPATH_EX_1024];				// -csg C:\quak\dungeon\maps\mymap
	char bsp_args[MAX_OSPATH_EX_1024];				// -fs_basepath "C:/quak/" -fs_game dungeon -meta -v -mv 1000000 -mi 6000000 -samplesize 8 "C:\quak\dungeon\maps\mymap"
	char vis_args[MAX_OSPATH_EX_1024];				// -fs_basepath "C:/quak/" -fs_game dungeon -vis -saveprt -fast "C:\quak\dungeon\maps\mymap"
	char lit_args[MAX_OSPATH_EX_1024];				// -fs_basepath "C:/quak/" -fs_game dungeon -light -v -deluxe -faster -cheapgrid -filter -gamma 0.5  "C:\quak\dungeon\maps\mymap"

	
	stage_e			stage;	// stage_csg_1, etc.

				int				rows;
				int				cols;
				int				num_pieces;
				vec3_t			block_size;
				vec3_t			mat_mins;
				vec3_t			mat_maxs;
				vec3_t			mat_size;

				sys_handle_t	process_handle;
	
	ALLOC___	 entitylist_t	eout_map;						// Output map
	ALLOC___	 int32list_s	ints_grid;
	ALLOC___	 stringlist_t	listnamefields;
} map_compile_s;

map_compile_s mc, *pmc = &mc;

