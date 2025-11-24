// console_autocomplete_function_determine.c.h

#define SIN1 String_Isin1
#define SIN2 String_Isin2
#define SIN3 String_Isin3
#define SIN4 String_Isin4
#define SIN5 String_Isin5
#define SIN6 String_Isin6
#define SIN7 String_Isin7
#define SIN8 String_Isin8

// Determine search type
switch (in_arg_number) {
case 0: // Command
	break;

case 1: // Arg 1 - First arg. "map your_are_here"
	// Baker: Cvar autofill check ...
	// Could be asking for cvar autocomplete if command == a cvar.
	while (is_currently_nothing) {
		qbool is_cvar_from_nothing = false; // Set to fail

		for (cvar_t *cvar = cmd->cvars->vars; cvar; cvar = cvar->next) {
			// Check cvar name
			if (String_Match (COMMAND, cvar->name)) {
				is_cvar_from_nothing = true;
				if (String_Has_Whitespace(cvar->string))	c_dpsnprintf1 (value_autofill, QUOTED_S, cvar->string);
				else										c_strlcpy (value_autofill, cvar->string);
				break; // for
			} // if

			// Check cvar aliases for this cvar
			for (char **cvar_alias = cvar->aliases; cvar_alias && *cvar_alias; cvar_alias++) {
				if (String_Match (COMMAND, *cvar_alias)) {
					is_cvar_from_nothing = true;
					if (String_Has_Whitespace(cvar->string))	c_dpsnprintf1 (value_autofill, QUOTED_S, cvar->string);
					else										c_strlcpy (value_autofill, cvar->string);
					break; // for
				}
			} // for cvar alias
		} // for cvar

		if (is_cvar_from_nothing) ac->searchtype = search_autofill_neg1; // cvar from nothing
		break;
	} // while


	if (ac->searchtype == search_autofill_neg1)
		break; // cvar autofill

	if (SIN2 (COMMAND, "sky", "loadsky")) {
		ac->searchtype = search1_sky_6;
		do_fly_submit = true;
	}

	// GENERICS ...

	// AWARE
	else if (SIN2 (COMMAND, "which", "printfile"))			ac->searchtype = search0_aware_any_folder_or_file;
	else if (SIN6 (COMMAND, "folder","dir","ls", "jpegsplit","jpegcheck","jpegresample"))
															ac->searchtype = search0_aware_any_folder;
	else if (SIN2 (COMMAND, "csqc_progname", "sv_progs"))	ac->searchtype = search0_aware_dot_dat;
	else if (SIN2 (COMMAND, "csgtool", "maze_to_map"))		ac->searchtype = search0_aware_dot_map;
	else if (SIN1 (COMMAND, "loadfont"))					ac->searchtype = search0_fonts_dp_names;
	else if (SIN2 (COMMAND, "giftoshader","gifclip"))		ac->searchtype = search0_aware_gif;

	else if (SIN4 (COMMAND, "showmodel", "objmodeladjust", "objmodelsplit", "playermodel"))
															ac->searchtype = search0_aware_models;

	else if (SIN1 (COMMAND, "barmake"))						ac->searchtype = search0_aware_obj;
	else if (SIN2 (COMMAND, "maze_load", "maze_save"))		ac->searchtype = search0_aware_txt;
	else if (SIN3 (COMMAND, "play","playvol","playloop"))	ac->searchtype = search0_aware_wav_oggs;

	// FILE (BUT NOT PATH AWARE)

	else if (SIN2 (COMMAND, "exec", "saveconfig")	)				ac->searchtype = search0_cfg_files;
	else if (SIN3 (COMMAND, "playdemo", "record", "timedemo") )		ac->searchtype = search0_dem_files;
	else if (SIN2 (COMMAND, "game", "gamdir")		)				ac->searchtype = search0_game_dirs;
	else if (SIN3 (COMMAND, "map", "changelevel", "changelevel2") )	ac->searchtype = search0_map_list;
	else if (SIN3 (COMMAND, "save", "load", "jpegextract" ) )		ac->searchtype = search0_sav_files;
	else if (SIN1 (COMMAND, "zipinfo"))								ac->searchtype = search0_zip_files;

	// ENGINE OR GAME STATE
	else if (SIN2 (COMMAND, "modelprecache","modeldecompile") )		ac->searchtype = search0_model_precaches;
	else if (SIN1 (COMMAND, "texturefindpos"))						ac->searchtype = search0_textures_world_nofly;
	else if (SIN2 (COMMAND, "bind","unbind"))						ac->searchtype = search0_key_names;
	else if (SIN2 (COMMAND, "effectinfo_dump", "effectinfo_list"))	ac->searchtype = search0_effectinfo_effects;
	else if (SIN2 (COMMAND, "ent_create", "ent_create_toss"))		ac->searchtype = search0_server_vm_functions;


	//else if (SIN2 (COMMAND, "sky", "loadsky"))					ac->searchtype = search1_sky_6;
	else if (SIN1 (COMMAND, "reflectcubemap"))						ac->searchtype = search1_cubemap_5;

	else if (SIN1 (COMMAND, "gl_texturemode"))						ac->searchtype = search1_gl_texturemode_7;
	else if (SIN1 (COMMAND, "constants"))							ac->searchtype = search1_constants_53;
	else if (SIN1 (COMMAND, "copy"))								ac->searchtype = search1_copy_8;

	else if (SIN3 (COMMAND, "csedicts","edicts","medicts"))			ac->searchtype = search1_edicts_9;
	else if (SIN1 (COMMAND, "r_editlights_edit"))					ac->searchtype = search1_r_editlights_edit_10;
	else if (SIN1 (COMMAND, "sv_cmd"))								ac->searchtype = search1_sv_cmd_11;

	else if (SIN1 (COMMAND, "cl_cmd"))								ac->searchtype = search1_cl_cmd_12;
	else if (SIN1 (COMMAND, "cvarlist"))							ac->searchtype = search1_cvarlist_20;
	else if (SIN1 (COMMAND, "sv_protocolname"))						ac->searchtype = search1_protocolname_21;


	else if (SIN1 (COMMAND, "tosses"))								ac->searchtype = search1_tosses_23;
	else if (SIN1 (COMMAND, "envmap"))								ac->searchtype = search1_envmap_24;
	else if (SIN1 (COMMAND, "parse"))								ac->searchtype = search1_parse_26;
	else if (SIN1 (COMMAND, "playvideo"))							ac->searchtype = search1_playvideo_29;
	else if (SIN1 (COMMAND, "ui_form_load"))						ac->searchtype = search1_ui_form_load_31;


	else if (SIN8 (COMMAND, "prvm_globals", "prvm_functions", "prvm_builtins", "prvm_printfunction", "prvm_edictset", "prvm_edictget","prvm_globalset","prvm_globalget"))			ac->searchtype = search1_client_server_menu_38;


	else if (SIN1 (COMMAND, "give"))								ac->searchtype = search1_give_46;





	else if (SIN1 (COMMAND, "image_cols_rows_split"))				ac->searchtype = search1_image_cols_rows_split_55;

	else if (SIN1 (COMMAND, "screenshot"))							ac->searchtype = search1_screenshot_88;

	else if (SIN1 (COMMAND, "pos"))									ac->searchtype = search1_pos_108;



	else if (SIN1 (COMMAND, "oset"))								ac->searchtype = search1_oset_control_150; // csqc_form

	else if (SIN1 (COMMAND, "shaderprint"))							ac->searchtype = search1_shaderprint_lookat_153;
	else if (SIN1 (COMMAND, "r_replacemaptexture"))					ac->searchtype = search1_r_replacemaptexture_lookat_154;
	else if (SIN1 (COMMAND, "eset"))								ac->searchtype = search1_eset_lookat_155; // csqc_form

	else if (SIN1 (COMMAND, "effectinfo_particleeffectnum"))		ac->searchtype = search0_effectinfo_effects;

	else if (SIN1 (COMMAND, "cycle"))								ac->searchtype = search0_cvars;
	break;

case 2: // in_arg_number 2 SECOND ARGUMENT "r_replacemaptexture something you_are_here"

	if (SIN1 (COMMAND, "r_replacemaptexture")) {
		ac->searchtype = search2_r_replacemaptexture_254;
		do_fly_submit = true;
	}
	else if (ac->is_from_nothing && SIN1 (COMMAND, "bind") ) {
		ccs *s_keycode = words[1]; // key

		KeyBind_Fly (s_keycode, value_autofill, sizeof(value_autofill) );
		ac->searchtype = search_autofill_neg1; // fields?
	}

	else if (SIN1 (COMMAND, "loadfont"))						ac->searchtype = search0_aware_fonts_ttf;
	else if (SIN1 (COMMAND, "bitatomize"))						ac->searchtype = search2_bitatomize_253;
	else if (SIN1 (COMMAND, "eset"))
		ac->searchtype = search0_server_vm_fields;

	else if (SIN2 (COMMAND, "prvm_fields", "prvm_globals"))		ac->searchtype = search2_prvm_fields_globals_255;
	else if (SIN2 (COMMAND, "prvm_functions", "prvm_printfunction"))	ac->searchtype = search2_vm_functions_256;
	else if (SIN1 (COMMAND, "barmake"))							ac->searchtype = search0_aware_png_jpg_tga;

	else if (SIN2 (COMMAND, "cset", "mset"))					ac->searchtype = search2_eset_cset_fields_context_213;
	else if (SIN2 (COMMAND, "prvm_globalset","prvm_globalget")) ac->searchtype = search2_globalget_fields_context_214;

	else if (SIN1 (COMMAND, "memlist"))							ac->searchtype = search2_memlist_zone_201;
	else if (SIN1 (COMMAND, "maze_to_map"))						ac->searchtype = search0_aware_dot_map; // search2

	else if (SIN1 (COMMAND, "csgtool"))							ac->searchtype = search2_csgtool_algo_names_249;

	// ui_set control [PROPERTY]
	else if (SIN1 (COMMAND, "oset"))							ac->searchtype = search2_oset_property_250; // csqc_form

	else if (SIN1 (COMMAND, "copy")) {
		if (SIN1 (words[1], "effect"))			ac->searchtype = search0_effectinfo_effects;
		else if (SIN1 (words[1], "images"))		ac->searchtype = search0_aware_png_jpg_tga;
		//else if (SIN1 (words[1], "maptexture"))	ac->searchtype = search0_; // search2_copy_texture_271;
		else if (SIN1 (words[1], "shader"))		ac->searchtype = search0_shaderprint_nofly;
		else if (SIN1 (words[1], "model"))		ac->searchtype = search0_aware_models;
		else if (SIN1 (words[1], "sound"))		ac->searchtype = search0_aware_wav_oggs;
	}
	break;

// prvm_globalget client pmove_org
case 3: // in_arg_number 3
	// 0    1   2
#pragma message ("search3_prvm_edictset_fields_319 NOT handled")
	// eset 30 origin
	// 0              1      2 3      4
	// prvm_edictset  server # origin 5
	// prvm_globalget client
	if (SIN3 (COMMAND, "eset", "cset", "mset")) { // eset value
		// Autofill?
		// prvm_edictset server 13 targetname ?
		//ccs *s_prog			= words[1]; // server
		ccs *s_edict_num	= words[1]; // 13
		ccs *s_fieldname	= words[2]; // field
		ccs *s_progname		= SIN1 (COMMAND, "cset") ? "client" :
							  SIN1 (COMMAND, "mset") ? "menu" :
													"server";
		PRVM_ED_EdictGet_Fly (s_progname, s_edict_num, s_fieldname, value_autofill, sizeof(value_autofill));
		ac->searchtype = search_autofill_neg1; // fields?
	}

	else if (SIN2 (COMMAND, "prvm_edictset", "prvm_edictget"))	ac->searchtype = search3_prvm_edictset_fields_319; // fields?
	else if (SIN1 (COMMAND, "maze_to_map"))						ac->searchtype = search3_maze_to_map_overwrite_340; // fields?

	else if (SIN1 (COMMAND, "prvm_globalset")) {
		ccs *s_progname		= words[1]; // server
		ccs *s_fieldname	= words[2]; // pmove_org

		PRVM_ED_Global_Fly (s_progname, s_fieldname, value_autofill, sizeof(value_autofill) );
		ac->searchtype = search_autofill_neg1; // fields?
	}
	// ui_set control [PROPERTY]
	else if (SIN1 (COMMAND, "oset")) {
		// ui_set LabelMap Caption [RECALL]
#ifdef CONFIG_MENU
		ccs *s_controlname	= words[1]; // LabelMap
		ccs *s_propyname	= words[2]; // Name
		if (!UI_Fly (s_controlname, s_propyname, value_autofill, sizeof(value_autofill) ))
			dp_strlcpy ( value_autofill, "[Property not found]", sizeof(value_autofill) );
#endif // CONFIG_MENU
		ac->searchtype = search_autofill_neg1; // fields?
	}

	break;

case 4: // in_arg_number 4
	if (SIN1 (COMMAND, "jpegresample")) {
		ac->searchtype = search4_jpegresample_401; //
	}
	if (SIN1 (COMMAND, "prvm_edictset")) {
		// prvm_edictset server 13 targetname ?
		ccs *s_prog			= words[1]; // server
		ccs *s_edict_num	= words[2]; // 13
		ccs *s_fieldname	= words[3]; // field

		PRVM_ED_EdictGet_Fly (s_prog, s_edict_num, s_fieldname, value_autofill, sizeof(value_autofill) );
		ac->searchtype = search_autofill_neg1; // fields?
	}
	break;

default:
	break;
} // sw


#undef SIN1
#undef SIN2
#undef SIN3
#undef SIN4
#undef SIN5


