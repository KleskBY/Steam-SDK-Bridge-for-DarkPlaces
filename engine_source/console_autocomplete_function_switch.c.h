// console_autocomplete_function_switch.c.h

switch (ac->searchtype) {
case search_autofill_neg1:
	Get_Solo_Count			("%s", value_autofill); // CVAR FILL
	break;

case search0_aware_any_folder:		GetFolder_Aware_Count (s);	break; // "dir", "ls", "folder"
case search0_aware_dot_dat:			GetFileList_Awareness (s, ".dat", EXCL_NULL); break;
case search0_aware_dot_map:			GetFileList_Awareness (s, ".map", EXCL_NULL); break;
case search0_aware_gif:				GetFileList_Awareness (s, ".gif", EXCL_NULL); break;
case search0_aware_models:			GetFileList_Awareness (s, ".md3,.mdl,.spr,.obj", EXCL_NULL); break;
case search0_aware_obj:				GetFileList_Awareness (s, ".obj", EXCL_NULL); break;
case search0_aware_png_jpg_tga:		GetFileList_Awareness (s, ".jpg,.png,.tga", EXCL_NULL); break;
case search0_aware_fonts_ttf:		GetFileList_Awareness (s, ".ttf", EXCL_NULL); break;
case search0_aware_wav_oggs:		GetFileList_Awareness (s, ".wav,.ogg", EXCL_NULL); break; // May 1 2025: Upgraded
case search0_aware_txt:				GetFileList_Awareness (s, ".txt", EXCL_NULL); break;


// FILE NOT PATH AWARE - SOME OF THESE EXTENSION STRIP playdemo demo1 and load here
case search0_cfg_files:				GetFileList_Not_Path_Aware_Count (q_folder_NULL, s, ".cfg", q_strip_exten_false); break;
//case search0_dat_files:			GetFileList_Not_Path_Aware_Count (q_folder_NULL, s, ".dat", q_strip_exten_false); break;
case search0_dem_files:				GetFileList_Not_Path_Aware_Count (q_folder_NULL, s, ".dem", q_strip_exten_true); break;
case search0_game_dirs:				GetModList_Count (s);	break; // game
case search0_map_list:				GetMapList (s, q_reply_buf_NULL, q_reply_size_0, q_is_menu_fill_false, q_is_zautocomplete_true, is_quiet);	break;
case search0_sav_files:				GetFileList_Not_Path_Aware_Count (q_folder_NULL, s, ".sav", q_strip_exten_true); break;
case search0_zip_files:				GetFileList_Not_Path_Aware_Count (q_folder_NULL, s, ".zip", q_strip_exten_false); break;

// ENGINE / GAME STATE
case search0_effectinfo_effects:	GetEffectList_Count			(s);	break;
case search0_fonts_dp_names:		GetCommad_Count				(s, "centerprint,chat,console,default,infobar,menu,notify,sbar"); break;
case search0_key_names:				GetKeyboardList_Count		(s);	break; // "bind", "unbind"
case search0_model_precaches:		GetModelPrecacheList_Count	(s);	break;
case search0_shaderprint_nofly:
	{
		int Shader_append (stringlist_t *plist, ccs *partial);
		stringlist_t list = {0};
		Shader_append (&list, s);
		stringlistsort_unique (&list);
		GetStringList_Sorted_Count (&list, s);
		stringlistfreecontents (&list);
	}
	break;

case search0_textures_world_nofly:	GetTexWorld_Count			(s);	break;

// VM - // Baker: is_from_nothing prints a ton of extra stuff
case search0_server_vm_fields:
	GetFieldListServer_Count		("server", s, ac->is_from_nothing);
	break;

case search0_server_vm_functions:
	{
		stringlist_t list = {0};
		stringlist_append_functions_unique (&list, SVVM_prog, s);
		GetStringList_Sorted_Count (&list, s);
		stringlistfreecontents (&list);
	}
	break;

case search0_server_vm_globals:
	GetGlobalListServer_Count		("server", s, ac->is_from_nothing);
	break;

case search1_cubemap_5:
	GetFileList_Awareness (s, "_rt.png,_rt.tga,_rt.jpg;rt.png,rt.tga,rt.jpg,"
		                      "_nx.png,_nx.tga,_nx.jpg;nx.png,nx.tga,nx.jpg"
		, EXCL_NULL); break;
	break;

case search1_sky_6:
	// Autocomplete from nothing eliminates the option of autocomplete from nothing
	//if (ac->is_from_nothing) {
	//	ccs *ssky = g_skyname[0] ? g_skyname : "* no skybox*";
	//	GetCommad_Count (s, ssky);
	//} else {
	GetSkyList_Count			(s);
	//}
	break;

case search1_gl_texturemode_7:		GetTexMode_Count			(s);	break;
case search1_copy_8:				GetCommad_Count				(s, "ents,pos,angles,tex,image,effect,texture,shader,model,sound,shader"); break;
case search1_edicts_9:				GetCommad_Count				(s, "classname,message,model,noise,nearby,targetname"); break; // cvarlist "changed"


case search1_r_editlights_edit_10:	GetREditLightsEdit_Count	(s);	break;
case search1_sv_cmd_11:				GetGameCommands_Count		(s, prvm_sv_gamecommands.string);	break;
case search1_cl_cmd_12:				GetGameCommands_Count		(s, prvm_cl_gamecommands.string);	break; // Baker r7103 gamecommand autocomplete



case search1_cvarlist_20:			GetCommad_Count				(s, "audit,changed,owned,newdefault"); break; // cvarlist "changed"
case search1_protocolname_21:		GetCommad_Count				(s, "666,999,dp7,quake"); break; // sv_protocolname

case search1_tosses_23:				GetCommad_Count				(s, "nofilter,summary"); break;
case search1_envmap_24:				GetCommad_Count				(s, "auto"); break;
case search1_parse_26:				GetCommad_Count				(s, "clipboard"); break;


case search1_playvideo_29:			GetVideoList_Count			(s /*".gif;*.jpg;*.dpv"*/); break;

case search1_ui_form_load_31:		GetFileList_Awareness (s, "*.txt", EXCL_NULL); break;

// Wildcard as they come ...
case search1_printfile_32:			GetFileList_Not_Path_Aware_Count (q_folder_NULL, s, "" /*.dem"*/, q_strip_exten_false); break;


//case search1_giftoshader_34:		GetAnimatedGIF_Count		(s);	break;

case search1_client_server_menu_38:
#ifdef CONFIG_MENU
									GetCommad_Count				(s, PRVM_Gen_Progs_Commad ());
#endif
	break;

#if 0 // Dungeon command is dead.
case 39: GetCommad_Count		(s, "blocklist,mapcompile,mapargs,gridimport,gridexport"); break; // dungeon_f
#endif

case search2_vm_functions_256:
	// Baker: What is term 0?
	
	{
		ccs *s_progname		= words[1]; // server
		prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
		if (prog) {
			stringlist_t list = {0};
			stringlist_append_functions_unique (&list, prog, s);
			GetStringList_Sorted_Count (&list, s);
			stringlistfreecontents (&list);
		}
	}

	break;

case search1_give_46: // "give"
	GetCommad_Count	(s,
		"a // armor,"
		"s // shells,"
		"n // nails,"
		"c // cells,"
		"r // rockets,"
		"h // health,"
		"key_silver // silver key,"
		"key_gold // gold key");
	break;



case search1_image_cols_rows_split_55:		GetFileList_Awareness (s, ".png,.tga,.jpg", "_gloss,_norm,_alpha,_reflect,_glow"); break; //
case search1_screenshot_88:					GetCommad_Count				(s, "clipboard"); break;

case search1_constants_53:
	RELATED_ (FS_Constants_f)
	GetCommad_Count	(s,
		"contents // CONTENT_WATER (.watertype) .."								","
		"contentflags_dp // SUPERCONTENTS_WATER (trace_dphitcontents) .."		","
		"effects // EF_ADDITIVE (.effects) .."									","
		"flags // FL_MONSTER (.flags) ."										","
		"modelflags // MF_ROTATE (.modelflags) .."								","
		"movetypes // MOVETYPE_WALK (.movetype) .."								","
		"renderflags // RF_USEAXIS (CSQC .renderflags) .."						","
		"solids // SOLID_BSP (.solid) .."										","
		"spawnflags // SPAWNFLAG_NOT_EASY (.spawnflags) .."						","
		"surfaceflags // Q3SURFACEFLAG_NOMARKS (trace_dphitq3surfaceflags) .."	// Ender ","
		);
	break;

case search2_bitatomize_253:
	RELATED_ (FS_Constants_f FS_BitAtomize_f)
	GetCommad_Count	(s,
		"contentflags_dp // SUPERCONTENTS_WATER (trace_dphitcontents) .."			","
		"effects // EF_ADDITIVE (.effects) .."										","
		"flags // FL_MONSTER (.flags) ."											","
		"renderflags // RF_USEAXIS (CSQC .renderflags) .."							","
		"spawnflags // SPAWNFLAG_NOT_EASY (.spawnflags) .."							","
		"surfaceflags // Q3SURFACEFLAG_NOMARKS (trace_dphitq3surfaceflags) .."		// Ender ","
		);
	break;



//
// SEARCH1 NOVAS
//


case search1_pos_108:				GetCommad_Count				(s, "silent"); break;



case search1_shaderprint_lookat_153: {
	if (s[0] == 0) {
		// No partial ..
		Con_DPrintLinef ("No partial for arg 1");
		char texturename[MAX_QPATH_128] = {0};
		qbool gottexture = Lookat_Texture_GetName_Is_Ok (texturename, sizeof(texturename));
		if (gottexture == false) {
			Con_PrintLinef ("Not detecting a 'looking at' map texture (can only collide with bsp)  ...");
			GetTexWorld_Count	(s);
		} else {
			Con_DPrintLinef ("Looking at texture %s ...", texturename);
			GetCommad_Count		(s, texturename);
		}
	} else {
		Con_DPrintLinef ("Have partial for arg 1");
		GetShaderList_Count			(s); break;
	}
	break; // r_replacemaptexture arg1 world textures

							  } // shaderprint block
case search1_eset_lookat_155:
	{
		if (s[0] == 0 && sv.active && cls.state == ca_connected && cls.signon == SIGNONS_4) {
			// No partial
			//int hitnetentity = -1;

			vec3_t org; Matrix4x4_OriginFromMatrix(&r_refdef.view.matrix, org);
			vec3_t dest, temp = {65536, 0, 0}; Matrix4x4_Transform(&r_refdef.view.matrix, temp, dest);
			prvm_prog_t *prog = SVVM_prog;
			prvm_edict_t *plyr = PRVM_PROG_TO_EDICT(1); // Baker: Weakness is player #1 isn't you.  TODO: Determine player #
			trace_t svtrace = SV_TraceLine(org, dest, MOVE_HITMODEL_4, /*passedict*/ plyr,
				SUPERCONTENTS_SOLID,  SUPERCONTENTS_SKIP_NONE_0, /*skip*/ MATERIALFLAGMASK_TRANSLUCENT, collision_extendmovelength.value);

			if (svtrace.ent != NULL) {


				int entnum = (int)PRVM_EDICT_TO_PROG(svtrace.ent);
				Con_DPrintLinef ("Hit server entity %d", (int)PRVM_EDICT_TO_PROG(svtrace.ent));
				va_super (sentnum, 128, "%d", entnum);
				GetCommad_Count		(s, sentnum);
			} else {
				Con_DPrintLinef ("No entity hit");
			}
		}
		else {
			// There is a partial
			GetCommad_Count	(s, "nearby");
		}
		break;
	}

case search1_r_replacemaptexture_lookat_154: {
	if (s[0] == 0) {
		// No partial ..
		Con_DPrintLinef ("No partial for arg 1");
		char texturename[MAX_QPATH_128] = {0};
		qbool gottexture = Lookat_Texture_GetName_Is_Ok (texturename, sizeof(texturename));
		if (gottexture == false) {
			Con_PrintLinef ("Not detecting a 'looking at' map texture (can only collide with bsp)  ...");
			GetTexWorld_Count	(s);
		} else {
			Con_DPrintLinef ("Looking at texture %s ...", texturename);
			GetCommad_Count		(s, texturename);
		}
	} else {
		Con_DPrintLinef ("Have partial for arg 1");
		GetTexWorld_Count		(s);
	}
	break; // r_replacemaptexture arg1 world textures
} // r_replace block





case search1_oset_control_150: // oset [CONTROL]
#ifdef CONFIG_MENU
		Get_UI_ObjectName_AutoComp_Count (s);
#endif
	break;

case search2_memlist_zone_201:		GetCommad_Count				(s, "fonts,q3shaders,zone"); break;

case search2_prvm_fields_globals_255:	GetCommad_Count				(s, "sorted"); break;

case search2_eset_cset_fields_context_213:
	{  // set cset eset fieldname
		ccs *s_progname		= String_Isin1 (COMMAND, "cset") ? "client" : 
							  String_Isin1 (COMMAND, "mset") ? "menu" : 
								"server";
		GetFieldListServer_Count		(s_progname, s, ac->is_from_nothing);
	}
	break;
case search2_globalget_fields_context_214: // prvm_globalset
	{
		ccs *s_prog		= words[1]; // server
		GetGlobalListServer_Count		(s_prog, s, ac->is_from_nothing);
	}
	break;

//case search2_maze_to_map_240:	GetFiles_Aware_Commas_Count	(q_folder_NULL, s, ".map", q_strip_exten_false); break;

case search2_csgtool_algo_names_249:
		GetCommad_Count
		(s,
		"_csg_test_rewrite,adjacents,brushfacer,brushunfacer_scratch1,caulkworld,collision_body,concat_ents_clipboard,cubeit,decal_automatic,flatzerosettexture,noflat_to_flat,infodump,pincommonterrain,removenondominanttexturebrushes,weld");
	break;


case search2_oset_property_250: // oset control [PROPERTY]
#ifdef CONFIG_MENU
		Get_UI_Property_AutoComp_Count (s);
#endif
	break;


// r_replacemap textures (which does fly submit) so must have exclusive control of #40
case search2_r_replacemaptexture_254:	GetFileList_Awareness (s, ".png,.tga,.jpg", "_gloss,_norm,_alpha,_reflect,_glow"); break; // r_replacemaptexture




case search3_maze_to_map_overwrite_340:		GetCommad_Count	(s, "overwrite"); break;
	break;

case search4_jpegresample_401:
	GetCommad_Count	(s, "noalpha,alpha_green0_other_1,alpha_green0_other_luminance,alpha_luminance,alpha_black0_other1,alpha_luminance_LT_128,alpha_green_0_luminance_LT_48,chromakey_LT_48"); break;
	break;
} // switch

if (ac->s_match_alphatop_a == NULL) {
exit_possible2:
	must_end_autocomplete = "No possible matches";
	goto exit_out; // No possible matches
}
goto search_completed;
