// console_autocomplete_function_enums.h

// ARG1 (usually)

#define search_autofill_neg1				-1

// FILE COMPLETION (PATH AWARE)
#define search0_aware_any_folder_or_file	900	// printfile, which
#define search0_aware_any_folder			901 // folder
#define search0_aware_dot_dat				902
#define search0_aware_dot_map				903
#define search0_aware_fonts_ttf				904
#define search0_aware_gif					905
#define search0_aware_models				906
#define search0_aware_obj					907	// "barmake" arg 1
#define	search0_aware_png_jpg_tga			908
#define search0_aware_txt					909
#define search0_aware_wav_oggs				910
//#define search0_aware_zip					911


// FILE NOT PATH AWARE
#define search0_cfg_files					920
#define search0_dem_files					922
#define search0_game_dirs					923
#define search0_map_list					924
#define search0_sav_files					925
#define search0_zip_files					926

// ENGINE OR GAMESTATE
#define search0_cvars						930
#define search0_effectinfo_effects			931
#define search0_fonts_dp_names				932		// "console", "user1", etc.
#define search0_key_names					933		// MOUSE1 etc.
#define search0_model_precaches				934
#define search0_shaderprint_nofly			936
#define search0_textures_world_nofly		935		// NO LOOK AT


// VM
#define search0_server_vm_fields			940
#define search0_server_vm_functions			942
#define search0_server_vm_globals			941



#define search1_cubemap_5					5
#define search1_sky_6						6		// FLY SUBMIT
#define search1_gl_texturemode_7			7
#define search1_copy_8						8
#define search1_edicts_9					9
#define search1_r_editlights_edit_10		10
#define search1_sv_cmd_11					11
#define search1_cl_cmd_12					12

#define search1_cvarlist_20					20			// audit,changed,owned,etc.
#define search1_protocolname_21				21
#define search1_tosses_23					23
#define search1_envmap_24					24
#define search1_parse_26					26	// parse command (autocompletes "clipboard")
#define search1_playvideo_29				29

#define search1_ui_form_load_31				31 
#define search1_printfile_32				32

#define search1_client_server_menu_38		38		// client, server, menu



#define search1_give_46						46		// 


#define search1_constants_53				53

#define search1_image_cols_rows_split_55	55		//									Splits an image into rows and columns
#define search1_screenshot_88				88		// "clipboard"
#define search1_pos_108						108		// "silent"
#define search1_oset_control_150				150 

#define search1_shaderprint_lookat_153			153		// LOOK AT
#define search1_r_replacemaptexture_lookat_154	154		// LOOK AT + FLY
#define search1_eset_lookat_155					155		// 



#define search2_memlist_zone_201				201
#define search2_eset_cset_fields_context_213	213		// context depends on client server menu
#define search2_globalget_fields_context_214	214

#define	search2_csgtool_algo_names_249			249
#define search2_oset_property_250				250
#define search2_bitatomize_253					253
#define search2_r_replacemaptexture_254			254

#define search2_prvm_fields_globals_255			255

#define search2_vm_functions_256				256

#define search3_prvm_edictset_fields_319	319
#define search3_maze_to_map_overwrite_340	340

#define search4_jpegresample_401			401		// alpha from green, black, luminance



