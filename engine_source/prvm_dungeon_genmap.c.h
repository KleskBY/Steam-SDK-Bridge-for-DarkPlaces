// prvm_dungeon_genmap.c.h

RELATED_ (M_Menu_Grid_f)
// shell_name = "proto/#dshell.map"
void Fill_MCStruct (map_compile_s *p, ccs *maps_proto_dshell_dot_map, ccs *mymap_e1m1, qbool is_verbose)
{
	// KillProcess ();
	int32s_freecontents		(&p->ints_grid);
	stringlistfreecontents	(&p->listnamefields);
	entitylistfreecontents	(&p->eout_map);

	size_t sizeof_p = sizeof(*p);
	memset (p, 0, sizeof_p);

	// Baker: The last game dir is one of record?
	ccs *dg_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1;
	c_strlcpy (p->gamedir, dg_gamedir);
	
	// c:\quak\dungeon\maps\mymap.bsp  (FS_BASEDIR)\(GAMEDIR1)\maps\ //
	c_dpsnprintf1 (p->quake_folder_trailslash, "%s", fs_basedir);
		File_URL_Edit_SlashesForward_Like_Unix (p->quake_folder_trailslash);

	c_dpsnprintf2 (p->fp_gamedir, "%s%s", p->quake_folder_trailslash, p->gamedir);
		File_URL_Edit_SlashesForward_Like_Unix (p->fp_gamedir);

	// c:\quak\dungeon\maps\mymap
	c_strlcpy (p->mapbasename, mymap_e1m1);
	c_dpsnprintf3 (p->fp_map_no_ext, "%s%s/maps/%s", 
		p->quake_folder_trailslash, 
		p->gamedir, 
		p->mapbasename);
		File_URL_Edit_SlashesForward_Like_Unix (p->fp_map_no_ext);

	c_dpsnprintf1 (p->rp_map_dot_map, "maps/%s.map", 
		p->mapbasename);

	c_dpsnprintf1 (p->fp_map_no_ext_win32, "%s",  p->fp_map_no_ext);
#ifdef _WIN32
		File_URL_Edit_SlashesBack_Like_Windows (p->fp_map_no_ext_win32);
#endif
		
	c_dpsnprintf2 (p->fp_q3map2_exe, "%s%s/q3map2/q3map2_64_light_auto.exe", 
		p->quake_folder_trailslash, 
		p->gamedir
		);
		File_URL_Edit_SlashesForward_Like_Unix (p->fp_q3map2_exe);

// dg_map_no_ext c:/quak/id1/maps/mymap
// dg_lightexe_unix c:/quak/id1/q3map2/q3map2_64_light_auto.exe
// dg_zircon_exe_unix c:/quak/zirc_sdl_32.exe

	RELATED_ (fromwtf8 FS_Shell_NonBlocking_f)
	ccs *w_zircon_exe_za = FS_File_Binary_URL_Unsafe_ZAlloc (); // WTF-8 vulnerability.  What about q3map2 though ...
	// dg_zircon_exe_unix c:/quak/zirc_sdl_32.exe
	c_dpsnprintf1 (p->fp_zircon_exe, "%s", w_zircon_exe_za);

	Mem_FreeNull_ (w_zircon_exe_za);


	// -csg c:/quak/id1/maps/mymap
	// 1. dg_map_no_ext_os
	c_dpsnprintf1 (p->csg_args, "-csg %s", p->fp_map_no_ext_win32);

//	//  -fs_basepath "C:/quak/" -fs_game dungeon -meta  -v -mv 1000000 -mi 6000000 -samplesize 8
	// -meta  -v -mv 1000000 -mi 6000000 -samplesize 8
	// 1. dg_quake_folder_trailslash c:/quake/
	// 2. gamedirname1 dungeon
	//ccs *bsp_switches = "-meta -v -mv 1000000 -mi 6000000 -samplesize 8";
	c_dpsnprintf4 (p->bsp_args, "-fs_basepath \"%s\" -fs_game %s %s \"%s\"", 
		p->quake_folder_trailslash, // c:/quake/
		p->gamedir, // dungeon
		q3map2_bsp_switches.string, // -meta -v -mv 1000000 -mi 6000000 -samplesize 8
		p->fp_map_no_ext_win32
		);

	c_dpsnprintf4 (p->vis_args, "-fs_basepath \"%s\" -fs_game %s %s \"%s\"", 
		p->quake_folder_trailslash, // c:/quake/
		p->gamedir, // dungeon
		q3map2_vis_switches.string, // -meta -v -mv 1000000 -mi 6000000 -samplesize 8
		p->fp_map_no_ext_win32
		);

	c_dpsnprintf4 (p->lit_args, "-fs_basepath \"%s\" -fs_game %s %s  \"%s\"", 
		p->quake_folder_trailslash, // c:/quake/
		p->gamedir, // dungeon
		q3map2_lit_switches.string, // -meta -v -mv 1000000 -mi 6000000 -samplesize 8
		p->fp_map_no_ext_win32
		);

	//ccs *s_shell_dot_map = "#dshell.map";
	//c_dpsnprintf1 (p->maps_shell_dot_map, "maps/%s", dshell_dot_map);
	c_strlcpy (p->maps_shell_dot_map, maps_proto_dshell_dot_map); // maps/proto/#dshell.map

	if (is_verbose) {
		Con_PrintVarString (p->fp_gamedir);
		Con_PrintVarString (p->fp_q3map2_exe);
		Con_PrintVarString (p->fp_zircon_exe);
		Con_PrintVarString (p->fp_map_no_ext_win32);
		Con_PrintVarString (p->csg_args);
		Con_PrintVarString (p->bsp_args);
		Con_PrintVarString (p->vis_args);
		Con_PrintVarString (p->lit_args);
		Con_PrintVarString (p->maps_shell_dot_map);
	}
}
