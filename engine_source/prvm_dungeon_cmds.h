// prvm_dungeon_cmds.h

#include "prvm_dungeon_ent_cmds.h"

// Returns PLUS1(index)
RELATED_ (VM_search_begin VM_callfunction VM_isfunction VM_getentityfieldstring)
RELATED_ (PRVM_Crash)

// float map_load (string filename) = #150;
// Returns 1 or greater on success
// allowing 0 value uninitialized variables to never contain a valid handle.
// Example:
// float h = map_load ("maps/proto/#dshell.map");
void VM_map_load (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(1, 1, VM_map_load); // 0: filename  Optionals: None

	ccs *filename = PRVM_G_STRING(OFS_PARM0);  VM_CheckEmptyString(prog, filename); // Baker: Host error if first character is whitespace or NULL char 0

	int handle;
	for (handle = 0; handle < PRVM_MAX_OPEN_DOT_MAPS_8; handle ++)
		if (prog->open_dot_maps[handle] == 0)
			break;

	if (handle >= PRVM_MAX_OPEN_DOT_MAPS_8) {
		PRVM_G_FLOAT(OFS_RETURN) = -2; VM_WarningLinef (prog, "VM_map_load: %s ran out of map handles (%d)", prog->name, PRVM_MAX_OPEN_DOT_MAPS_8);
		return;
	}

	// Baker: Construct the entity_t
	entitylist_t *plist = (entitylist_t *)Mem_Alloc(prog->progs_mempool, sizeof(entitylist_t));

	qbool errorcode = entitylist_parse_mapfile_errorcode (plist, filename);
	// returns SUCCESS_OK_0, -1 (file not found) or -3 (couldn't parse)

	if (errorcode) {
		PRVM_G_FLOAT(OFS_RETURN) = errorcode; // File not found or couldn't parse. -1 or -3

		entitylistfreecontents (plist); // free data
		Mem_FreeNull_ (plist); // free the struct
		return;
	}

	prog->open_dot_maps[handle] = plist;
	PRVM_G_FLOAT(OFS_RETURN) = PLUS1(handle);
}

// #157 float map_close (float hmap) = #157; // Return value is true or -4 for invalid handle
void VM_map_close (prvm_prog_t *prog)
{
	RELATED_ (VM_search_end)

	VM_SAFEPARMCOUNT(1, VM_map_close);

	// Baker: Deliberately no boiler macro
	int _hmap = (int)PRVM_G_FLOAT(OFS_PARM0);
	int handle = UNPLUS1(_hmap);

	if (in_range_beyond (0, handle, PRVM_MAX_OPEN_DOT_MAPS_8) == false) {
		VM_WarningLinef (prog, "VM_map_close: invalid handle %d used in %s", handle, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = HMAP_HANDLE_INVALID_NEG_4;
		return;
	}

	if (prog->open_dot_maps[handle] == NULL) {
		VM_WarningLinef (prog, "VM_map_close: no such handle %d in %s", handle, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = HMAP_HANDLE_INVALID_NEG_4;
		return;
	}

	entitylistfreecontents (prog->open_dot_maps[handle]);
	Mem_FreeNull_ (prog->open_dot_maps[handle]);

	PRVM_G_FLOAT(OFS_PARM0) = 0; // Does this work?

	PRVM_G_FLOAT(OFS_RETURN) = 1;
}

// #153 float map_save_as (float hmap, string filename) = #153;
// true = success, false = failure, -4 invalid handle, -1 refused if does not end with .map
// float isok = map_save_as (h, "maps/my_map.map") = #153;
void VM_map_save_as (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(2, 2, VM_map_save_as); // hmap filename // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];

	ccs *filename = PRVM_G_STRING(OFS_PARM1);	VM_CheckEmptyString(prog, filename);

	if (String_Starts_With (filename, "maps/") == false) {
		VM_WarningLinef (prog, "VM_map_save_as: refused filename " QUOTED_S " must start with maps/ %s", filename, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -1;
		return;
	}

	if (String_Ends_With (filename, ".map") == false) {
		VM_WarningLinef (prog, "VM_map_save_as: refused filename " QUOTED_S " must end with .map %s", filename, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -1;
		return;
	}

	qbool isok = entitylist_write_to_file (pe, filename);

	PRVM_G_FLOAT(OFS_RETURN) = isok ? 1 : 0 ; // Return true on success
}

// #154 float map_clipboard_set (float hmap) = #154; // Returns true on success, -4 on invalid handle
// map_clipboard_set (h);
void VM_map_clipboard_set (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(1, 1, VM_map_clipboard_set); // hmap // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];

	baker_string_t *bsa = entitylist_maptext_bsalloc (pe);
	if (!bsa) { // Virtually impossible?
		VM_WarningLinef (prog, "%s: could not allocate string handle %d in %s", __func__, handle, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -6;
		return;
	}

	Clipboard_Set_Text (bsa->string); // CSQC: map_clipboard_set

	BakerString_Destroy_And_Null_It (&bsa);

	PRVM_G_FLOAT(OFS_RETURN) = 1; // Return true on success
}


// #156 float map_prefix_fields_values (float hmap, string fieldscomma, string prefix) = #156; // returns number changed.
// float numchanges = map_prefix_fields_values(hmap, "target,targetname", "block1_");
void VM_map_prefix_fields_values (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(3, 3, VM_map_prefix_fields_values); // hmap commanames prefix // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];

	ccs *comma_names	= PRVM_G_STRING(OFS_PARM1);	VM_CheckEmptyString(prog, comma_names);
	ccs *prefix			= PRVM_G_STRING(OFS_PARM2);	VM_CheckEmptyString(prog, prefix);

	stringlist_t list_comma_names = {0};
	stringlistappend_split (&list_comma_names, comma_names, ",");

	int num_changed = entitylist_prefix_epairs_targetnames (pe, prefix, &list_comma_names);

	stringlistfreecontents (&list_comma_names);

	PRVM_G_FLOAT(OFS_RETURN) = num_changed;
}


// #158 float map_entity_count (float hmap) = #158;
// Return number of entities in the map.  Returns < 0 on failure.
// float numentities = map_entity_count (h);
void VM_map_entity_count (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_map_entity_count);

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];

	int num_entities_in_map = pe->count;

	PRVM_G_FLOAT(OFS_RETURN) = num_entities_in_map;
}


// #159 /*entnum*/ float map_find_entity (float hmap, string key, string value, [float afterindex]) = #159; // Returns entity number, 0 is world, failure returns -1
// Returns -4 on handle failure, returns -1 if not found.
// float entnum = map_find_entity (h, "targetname", "mat1");
void VM_map_find_entity (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(3, 4, VM_map_find_entity); // hmap, key, value // opt: afterindex

	HMAP_HANDLE_BOILER;

	ccs *keyname = PRVM_G_STRING(OFS_PARM1);	VM_CheckEmptyString(prog, keyname);
	ccs *keyvalue = PRVM_G_STRING(OFS_PARM2);	VM_CheckEmptyString(prog, keyvalue);
	int startindex = START_IDX_0;

	if (prog->argc >= 4) { // h targetname mat1 0
		startindex = (int)PRVM_G_FLOAT(OFS_PARM3) + 1;
	}

	entitylist_t *pe = prog->open_dot_maps[handle];

	int entnum = entitylist_find_key_value_ex (pe, keyname, keyvalue, startindex, pe->count);

	PRVM_G_FLOAT(OFS_RETURN) = entnum;
}


// #152 float map_append_map (float hmapdest, float hmapsrc) = #152;
// Returns <=0 on failure, true on success
void VM_map_append_map (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(2, 2, VM_map_append_map); // dst src // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe_dest = prog->open_dot_maps[handle];

	// Baker: Deliberately no boiler macro
	int _hmap2 = (int)PRVM_G_FLOAT(OFS_PARM1);
	int handle2 = UNPLUS1(_hmap2);

	if (in_range_beyond (0, handle2, PRVM_MAX_OPEN_DOT_MAPS_8) == false) {
		VM_WarningLinef (prog, "%s: invalid handle2 %d used in %s", __func__, handle2, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = HMAP_HANDLE_INVALID_NEG_4;
		return;
	}

	if (prog->open_dot_maps[handle2] == NULL) {
		VM_WarningLinef (prog, "%s: no such handle2 %d in %s", __func__, handle2, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = HMAP_HANDLE_INVALID_NEG_4;
		return;
	}

	if (handle == handle2) {
		VM_WarningLinef (prog, "%s: Dest handle == source handle %d in %s", __func__, handle, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -5; // Dest and source are the same.
		return;
	}

	entitylist_t *pe_src = prog->open_dot_maps[handle2];

	// Add entity 0 brushes TO MAP1
	// Add entity ALL brushes and epairs to MAP1
	entitylist_brush0_append		(pe_dest, pe_src);	// no keys, just brushes.
	entitylist_nonworld_append		(pe_dest, pe_src);	// brushes and keys.

	PRVM_G_FLOAT(OFS_RETURN) = 1; // true = success
}

RELATED_ (VM_uri_get VM_bufstr_get VM_callfunction VM_isfunction)
RELATED_ (FS_Shell_Blocking_f)
RELATED_ (FS_Dungeon_f)


// #151 float map_move (float hmap, vector move) = #151;
// float isok = map_move (h, '200 200 200')
// The move is relative, not absolute.  It will add +200 +200 +200 to all entities.
// First the brushes (and any patches are moved) and then any "origin" are updated.
// At this time, this operation does not "fail" aside from invalid handle -4
void VM_map_move (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(2, 2, VM_map_move); // hmap, vec // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];
	vec3_t move_amount; VectorCopyDestSrc (move_amount, PRVM_G_VECTOR(OFS_PARM1));

	if (!move_amount[0] && !move_amount[1] && !move_amount[2])
		goto quick_exit; // Zero move

	entitylist_translate_epairs_origin	(pe, move_amount);
	entitylist_translate_brushes		(pe, move_amount); // Has texture scaling.

quick_exit:
	PRVM_G_FLOAT(OFS_RETURN) = 1; // true = success
}


// #155 float map_compile_with_callback (string filename,
//			void(float hmap, float stage, float status) callback) = #155;
// Ex: map_compile_with_callback ("maps/mymap.map", Map_Compile_Complete_Callback
// Example is ...
//cvar_t q3map2_csg_compiler = {CF_SERVER, "q3map2_csg_compiler", "", "If emptystring, engine .exe is the compiler [Zircon]"};
//cvar_t q3map2_compiler = {CF_SERVER, "q3map2_compiler", "q3map2/q3map2.exe", "If emptystring, engine .exe is the compiler [Zircon]"};
//cvar_t q3map2_csg_switches = {CF_SERVER, "q3map2_csg_switches", "-csg", "q3map2 switches for bsp phase [Zircon]"};
//cvar_t q3map2_bsp_switches = {CF_SERVER, "q3map2_bsp_switches", "-game nexuiz -meta -v -mv 1000000 -mi 6000000 -samplesize 8", "q3map2 switches for bsp phase [Zircon]"};
//cvar_t q3map2_vis_switches = {CF_SERVER, "q3map2_vis_switches", "-game nexuiz -vis -saveprt -fast", "q3map2 switches for vis phase [Zircon]"};
//cvar_t q3map2_lit_switches = {CF_SERVER, "q3map2_lit_switches", "-game nexuiz -light -v -deluxe -faster -cheapgrid -filter -gamma 0.5", "q3map2 switches for light phase [Zircon]"};

//./dungeon/q3map2/q3map2.x86_64 -fs_game "dungeon"-meta -v
// dungeon\q3map2\q3map2_64_light_auto.exe -fs_game dungeon -game nexuiz -meta -v -mv 1000000 -mi 6000000 -samplesize 8 "dungeon/maps/my_map"

RELATED_ (FS_Dungeon_f)

	//// 6. cmdline - fake + unused
	//va_super (x_cmdline, MAX_OSPATH_EX_1024, "%s %s \"%s\"", x_compiler, x_switches, x_map_noext);
	//Con_PrintVarString (x_cmdline);

#define MAPSTAGE_CSG_1 1
#define MAPSTAGE_BSP_2 2
#define MAPSTAGE_VIS_3 3
#define MAPSTAGE_LIT_4 4

void VM_map_compile_with_callback (prvm_prog_t *prog)
{
	int status = -2;

	VM_SAFEPARMCOUNTRANGE(4, 4, VM_map_compile_with_callback); // h, file, stage, callback

	HMAP_HANDLE_BOILER;

	//int strno = (int)PRVM_G_INT(OFS_PARM1); //
	ccs *filename = PRVM_G_STRING(OFS_PARM1);	VM_CheckEmptyString(prog, filename);
	int stage = (int)PRVM_G_FLOAT(OFS_PARM2);

	if (in_range (MAPSTAGE_CSG_1, stage, MAPSTAGE_LIT_4) == false) {
		VM_WarningLinef (prog, "%s: invalid stage %d (expected 1 to 4) in %s", __func__, stage, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -7;
		return;
	}

	func_t mycallback = PRVM_G_INT(OFS_PARM3);

	entitylist_t *pe = prog->open_dot_maps[handle];

	// 1. gamedir ("dungeon")
	ccs *x_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1; // Con_PrintVarString (x_gamedir);

	// 2. full path to gamedir ("c:\quak\dungeon_lite\") with trailing slash
	va_super (fp_basedir, MAX_OSPATH_EX_1024, "%s", fs_basedir /*has trailing slash*/);
	File_URL_Edit_SlashesForward_Like_Unix (fp_basedir);

	// 2. full path to gamedir ("c:\quak\dungeon_lite\") with trailing slash
	va_super (fp_gamedir, MAX_OSPATH_EX_1024, "%s%s/", fs_basedir /*has trailing slash*/, x_gamedir);
	File_URL_Edit_SlashesForward_Like_Unix (fp_gamedir); // Con_PrintVarString (fp_gamedir);

	// 2. compile binary ("dungeon/q3map2/q3map2.x86_64" or "dungeon/q3map2/q3map2.exe")
	// a. q3map2_compiler.string = "q3map2/q3map2.exe"
	char fp_compiler[MAX_OSPATH_EX_1024];
	switch (stage) {
	case MAPSTAGE_CSG_1:	c_dpsnprintf2 (fp_compiler, "%s%s", fp_gamedir, q3map2_csg_compiler.string); break;
	default:				c_dpsnprintf2 (fp_compiler, "%s%s", fp_gamedir, q3map2_compiler.string); break;
	}
	// Con_PrintVarString (fp_compiler);

	// 3. map
	// filename = "maps/mymap.map"
	// dungeon/maps/my_map

	va_super (fp_map_noext, MAX_OSPATH_EX_1024, "%s%s", fp_gamedir, filename); // maps/my_map
	File_URL_Edit_Remove_Extension (fp_map_noext); // Con_PrintVarString (fp_map_noext);

#if 0 //def _WIN32
	// Don't do it.  Garux compiler hates it.
	File_URL_Edit_SlashesBack_Like_Windows (fp_basedir);
#endif

	char x_switches[MAX_OSPATH_EX_1024];
	// 4. switches
	switch (stage) {
	case MAPSTAGE_CSG_1: c_dpsnprintf3 (x_switches, "-fs_basepath \"%s\" -fs_game %s %s", fp_basedir, x_gamedir, q3map2_csg_compiler.string); break;
	case MAPSTAGE_BSP_2: c_dpsnprintf3 (x_switches, "-fs_basepath \"%s\" -fs_game %s %s", fp_basedir, x_gamedir, q3map2_bsp_switches.string); break;
	case MAPSTAGE_VIS_3: c_dpsnprintf3 (x_switches, "-fs_basepath \"%s\" -fs_game %s %s", fp_basedir, x_gamedir, q3map2_vis_switches.string); break;
	case MAPSTAGE_LIT_4: c_dpsnprintf3 (x_switches, "-fs_basepath \"%s\" -fs_game %s %s", fp_basedir, x_gamedir, q3map2_lit_switches.string); break;
	} // -fs_basepath "C:/galaxy/"
	// Con_PrintVarString (x_switches);

	// 5. x_switches_full includes "maps/my_map"
	va_super (x_switches_full, MAX_OSPATH_EX_1024, "%s \"%s\"", x_switches, fp_map_noext); //Con_PrintVarString (x_switches_full);

	// Verify that process is not running, etc.
	RELATED_ (VM_callfunction CLVM_ExecuteProgram PRVM_ExecuteProgram PRVM_EnterFunction OP_CALL0 SV_CheckContentsTransition next_stage_process)

	//prog->ExecuteProgram(prog, mycallback, "Callback undefined");
	pe->func_t_stored_int = mycallback;

	if (prog->process_handle) {
		VM_WarningLinef (prog, "%s: BUSY process already running for %d in %s", __func__, prog->process_index, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -5; // Dest and source are the same.
		return;
	}

	RELATED_ (FS_Shell_Blocking_f)

	Con_DPrintLinef ("%s %s", fp_compiler, x_switches_full);

#ifdef _WIN32
	prog->process_handle = System_Process_Create (fp_compiler, x_switches_full, fp_basedir);

	if (!prog->process_handle) {
		VM_WarningLinef (prog, "%s: Unable to start process %s in %s", __func__, fp_compiler, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -6; // Dest and source are the same.
		return;
	}

	Con_PrintLinef ("Process started with handle %p", prog->process_handle);

	host_dungeon_state = 1;
	//int iters = 0;
	while (prog->process_handle) {
		// Do a busy wait ...

		int exit_code;
		int running = System_Process_Is_Still_Running_Neg1_Error (prog->process_handle, &exit_code);
		if (running == (int)true) {
			// Still running
		} else if (running == not_found_neg1) {
			VM_WarningLinef (prog, "%s: Process not found -1 in %s", __func__, prog->name);
			PRVM_G_FLOAT(OFS_RETURN) = -6; // Dest and source are the same.
			prog->process_handle = NULL;
			status = -1;
			return;
		} else {
			// OK ! running = false
			Con_PrintLinef ("%s: Process completed with exit code %d in %s", __func__, exit_code, prog->name);
			prog->process_handle = NULL;
			status = 0;
			PRVM_G_FLOAT(OFS_RETURN) = 1; // Success!
			break;
		}
	}
#else
	qbool isok = Sys_ShellExecute_Wait (fp_compiler, x_switches_full, fs_workingdir_null);
	prog->process_handle = NULL;
	status = isok ? 0 : -2;
#endif

ok_do_callback:
    ; // Obligatory :(
	RELATED_ (SV_CheckContentsTransition CLVM_ExecuteProgram)
	// QuakeC: The callback looks like: void Map_Compile_Complete_Callback (float hmap, float stage, float status);

	PRVM_G_FLOAT(OFS_PARM0) = handle;
	PRVM_G_FLOAT(OFS_PARM1) = stage;
	PRVM_G_FLOAT(OFS_PARM2) = status;

	prog->ExecuteProgram(prog, mycallback, "Callback undefined");

	host_dungeon_state = 0;
}


