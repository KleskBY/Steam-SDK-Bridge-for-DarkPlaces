// prvm_dungeon.c.h

#include "prvm_dungeon_header.c.h"

// We need to specify a shell.  (command)
//   The shell needs to get the map placement grid size  20 x 13
//   The shell needs to know the size 256 x 256


// We need to specify map pieces. (command that reads a text file)
// Each piece, we need to know the size.
// 

// Could/should dungeon pieces be part of the shell map?

RELATED_ (CSG_Perform Q13_Perform)
RELATED_ (FS_GameDir_f FS_Path_f M_Menu_Grid_f)


// Problem:
// We need .map chunks.  Reasons ... doors
// all kinds of shit.  A single entity simply will not suffice.
// We will need to brush copy (world) we don't do that with map_include

// s_targetprefix is what prefix the map chunks get.

// 1. Provide maps/#block1.map
// 2. Provide the targetname prefix like "e502_" (any targetnames/targets get replaced with that)
//     2b. What about door or other entity fields?
// 3. plist_targetfields "target,targetname" and any other fields that need prefixed.

//&listnamefields

#ifdef CONFIG_MENU // Dungeon
void Grid_From_Menu (map_compile_s *p)
{

	if (p->ints_grid.count)
		int32s_freecontents (&p->ints_grid);

	p->rows = mg->rows;
	p->cols = mg->cols;
	
	int count = p->rows * p->cols;
	for (int j = 0; j < count; j ++) {
		int myval = mg->val[j];
		int32s_add1 (&p->ints_grid, myval);
	} // for

}





void Fill_Grid_Values (map_compile_s *p)
{
	if (p->ints_grid.count)
		int32s_freecontents (&p->ints_grid);

	int count = p->rows * p->cols;
	for (int j = 0; j < count; j ++) {
		int32s_add1 (&p->ints_grid, 0);
	} // for
	
	// Place a block in 0 x 0
	int r =0, c =0;
	int idx = r * p->rows + c;
	p->ints_grid.ints[idx] = 1;
}

#include "prvm_dungeon_insertions.c.h"
#include "prvm_dungeon_genmap.c.h"

RELATED_ (FS_Dungeon_f)
qbool next_stage_process ()
{
	if (!pmc->process_handle)
		goto next_stage;

	Con_PrintLinef ("We have a process from last time %p (stage %d)...", pmc->process_handle, pmc->stage);

#ifdef _WIN32 // Dungeon Win32
	int exit_code = 0;
	int result = System_Process_Is_Still_Running_Neg1_Error(pmc->process_handle, &exit_code);

	switch (result) {
	case not_found_neg1:	// Baker: I don't experience this so I think it is a failure.
							Con_PrintLinef ("Process not found.  Clearing."); 
							goto error_out;

	case true:				Con_PrintLinef ("Process still running ..."); 
							break;

	case false:				
							Con_PrintLinef ("Process completed (exit code = %d).  Clearing.", exit_code); 
							if (exit_code) {
								goto error_out;
							}

							// SUCCESS
							goto next_stage;							
	}
	return false; // Still running
#endif

next_stage:

#ifdef _WIN32 // DUNGEON
	switch (pmc->stage) {

	case stage_none_0:
		
		pmc->stage = stage_csg_1;
		pmc->process_handle = System_Process_Create (
			pmc->fp_zircon_exe, 
			pmc->csg_args, 
			pmc->fp_gamedir
		);

		if (pmc->process_handle == 0) {
			goto error_out;
		}
		break;

	case stage_csg_1:

		pmc->stage = stage_bsp_2;
		pmc->process_handle = System_Process_Create (
			pmc->fp_q3map2_exe, 
			pmc->bsp_args, 
			pmc->fp_gamedir
		);

		if (pmc->process_handle == 0)
			goto error_out;

		break;
	case stage_bsp_2:
		pmc->stage = stage_vis_3;
		pmc->process_handle = System_Process_Create (
			pmc->fp_q3map2_exe, 
			pmc->vis_args, 
			pmc->fp_gamedir
		);

		if (pmc->process_handle == 0)
			goto error_out;

		break;

	case stage_vis_3:
		pmc->stage = stage_lit_4;
		pmc->process_handle = System_Process_Create (
			pmc->fp_q3map2_exe, 
			pmc->lit_args, 
			pmc->fp_gamedir
		);

		if (pmc->process_handle == 0)
			goto error_out;


	case stage_lit_4:
		Con_PrintLinef ("Success!");
		pmc->stage = stage_none_0;
		pmc->process_handle = NULL; 

		return true;

	}

	return true;
#endif
error_out:
	pmc->stage = stage_none_0;
	pmc->process_handle = NULL; 
	return false;

}


// blocklist,mapcompile,gridsize,gridimport,gridexport
void FS_Dungeon_f (cmd_state_t *cmd)
{
	if (pmc->process_handle) {
		// Update ...
		next_stage_process ();
		return;
	}

	ccs *s_directive	= Cmd_Argv(cmd, 1); // blocklist,mapcompile,gridsize,gridimport,gridexport
	char s_mymap_noext[MAX_QPATH_128];
	
	c_strlcpy (s_mymap_noext, File_URL_SkipPath(Cmd_Argv(cmd, 2)));	
	if (String_Ends_With (s_mymap_noext, ".map"))
		String_Remove_Extension (s_mymap_noext);

	if (String_Match(s_directive, "mapargs")) {
		Fill_MCStruct (pmc, "maps/proto/#dshell.map", s_mymap_noext, q_is_verbose_true);

		va_super (s_shell, MAX_OSPATH_EX_1024, "shell_blocking %s %s > somefile.txt", pmc->fp_q3map2_exe, pmc->bsp_args);
		Clipboard_Set_Text (s_shell); // cmd "dungeon"
		Con_PrintLinef ("Command line set to clipboard");
		return;
	}

	qbool fromgrid = false;
	if (String_Match(s_directive, "mapcompilegrid")) {
		fromgrid = true;

	}

	if (String_Match(s_directive, "mapcompile") || fromgrid) {
		Fill_MCStruct (pmc, "maps/proto/#dshell.map", s_mymap_noext, q_is_verbose_true);
		Con_PrintVarString (pmc->fp_q3map2_exe);
		Con_PrintVarString (pmc->fp_zircon_exe);
		Con_PrintVarString (pmc->fp_map_no_ext_win32);
		Con_PrintVarString (pmc->csg_args);
		Con_PrintVarString (pmc->bsp_args);
		Con_PrintVarString (pmc->vis_args);
		Con_PrintVarString (pmc->lit_args);

		pmc->block_size[0] = pmc->block_size[1] = 256;
		if (fromgrid) {

			Grid_From_Menu (pmc);
		}
		else {
			pmc->cols = 20; // yes
			pmc->rows = 13; // yes
			
			Fill_Grid_Values (pmc); // Fills pmc->ints_grid and [0] is set to 1.
		}

		Generate_Dungeon_Map_File (pmc); // Idea is that grid is supplied by UI or other and this compiles up the final .map
		
		qbool isok = next_stage_process ();

		if (!isok) {
			Con_PrintLinef ("Could not start process");
			return;
		}
		Con_PrintLinef ("Process started ok (%p) stage = %d", pmc->process_handle, pmc->stage);
	}

	//if (Cmd_Argc (cmd) < 2) {
	//	//               0       1
	//	Con_PrintLinef ("dungeon <map>");
	//	return;
	//}

	//ccs *s_mymap_noext = Cmd_Argc(cmd) > 1 ? Cmd_Argv(cmd, 1) : "mymap";
	//
	//Con_PrintVarString (pmc->gamedir);
	
}

#endif	