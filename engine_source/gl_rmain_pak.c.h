// gl_rmain_pak.c.h - "pak_this_map"

//	// CopyFile doesn't preserve date modified  Neither does Ex
//	// CopyFileEx (src_path_to_file, dst_path_to_file, NULL, NULL, NULL, 0);
//
//	//return CopyFile (src_path_to_file, dst_path_to_file, false /*don't fail if file exists*/) != 0;
//	ret = CopyFile (src_path_to_file, dst_path_to_file, false /*don't fail if file exists*/) != 0;


RELATED_ (SV_Map_f CL_Record_f r_listmaptextures)

//]pak_this_map park
//folder is
//   0: models/outdoors/tree_leaves3_shader
//   1: noshader
//   2: models/outdoors/tree_bark
//   3: models/furniture/cabinet_wood_side
//   4: models/furniture/cabinet_wood_top
//   5: models/furniture/cabinet_wood_front
//   6: textures/zz_rgb/color_gray_32
//   7: textures/ambientcg/paper_005
//   8: textures/ambientcg/wood_051
//   9: textures/azirc0/floor_lite_slate
//  10: textures/texturecan/wood_0066
//  11: textures/texturecan/tiles_0108
//  12: textures/ambientcg/Planks029L
//  13: textures/ambientcg/grass_001
//  14: textures/texturecan/concrete_0026
//  15: textures/texturecan/bricks_0020
//  16: textures/texturecan/metal_0077
//  17: textures/trak5x_sh/base_base1a
//  18: textures/trak5x_sh/light_light3a
//  19: textures/ambientcg/wood_floor_056
//  20: textures/common/caulk
//  21: textures/liquids/mirror_solid
//  22: textures/ambientcg/sandstone_wall_base_1k
//  23: textures/skies/jf_nebula_sky_nolight
//  24: textures/texturecan/bricks_0019
//  25: textures/texturecan/rooftop_0007
//  26: textures/exx/wall_crete03
//  27: textures/texturecan/tiles_0097
//  28: textures/texturecan/ground_0041
//  29: textures/texturecan/bricks_0023
//  30: textures/trak4x_sh/floor_tile3b
//  31: textures/texturecan/bricks_0018
//  32: textures/trak5x_sh/floor_floor2f
//  33: textures/common/nodraw
//  34: textures/decals/splatter01
//  35: textures/liquids/water4_tzork_rk
//  36: textures/exx/base_crete03
//  37: textures/fence/base_chainlink_dark2_nomipmap
//  38: textures/fx/corona_white
//  39: textures/effects_sh/brown_glass
//]copy


// jf_nebula_sky_nolight

// Shader_get_text - first instance
// example pak_this_shader "textures/trak5x_sh/floor_floor2f"

// Returns null if can't

// shader_t *myshader = Mod_LookupQ3Shader(s_shader);

	//textures/decals/scorch01
	//{
	//	qer_editorimage textures/decals/scorch01
	//	dpoffsetmapping none

	//	surfaceparm trans
	//	surfaceparm nonsolid
	//	surfaceparm nodlight
	//	surfaceparm nolightmap

	//	polygonOffset
	//	sort 6
	//	cull none

	//	{
	//		map textures/decals/scorch01
	//		blendFunc filter
	//		rgbgen identity
	//	}
	//}

char *ShaderText_ZAlloc (shader_t *myshader, ccs *s_shadername, char *s_return_shader, size_t s_return_shader_size)
{
	const int is_print = false;
	char *sout_zalloc = NULL;
	fssearch_t *search = FS_Search("scripts/*.shader", fs_caseless_true, fs_quiet_FALSE, fs_pakfile_null, fs_gamedironly_false);

	if (!search)
		return NULL;

	char namebuf64[Q3PATHLENGTH_64];
	int is_done = false;

	char *f;
	for (int fileindex = 0; fileindex < search->numfilenames; fileindex ++) {
		const char *text = f = (char *)FS_LoadFile(search->filenames[fileindex], tempmempool, fs_quiet_FALSE, fs_size_ptr_null);
		if (!f)
			continue;

		int bracket_depth = 0;

		const char *s0 = text;
		while (COM_ParseToken_QuakeC(&text, false)) {
			c_strlcpy (namebuf64, com_token);
			int is_one_we_want = String_Match (s_shadername, namebuf64);
			if (is_one_we_want) {
				ccs *s_file = search->filenames[fileindex];
				if (s_return_shader) {
					strlcpy (s_return_shader, s_file, s_return_shader_size);
				}
				if (is_print) Con_PrintLinef (CON_BRONZE "Found shader" CON_WHITE " in %s", s_file);
			}
			if (!COM_ParseToken_QuakeC(&text, false) || String_NOT_Match(com_token, "{")) {
				if (is_print) Con_PrintLinef ("%s parsing error - expected \"{\", found " QUOTED_S, search->filenames[fileindex], com_token);
				break;
			}
			// Parsed a "{"
			bracket_depth ++;
			while (COM_ParseToken_QuakeC(&text, false)) {
				if (String_Match_Caseless(com_token, "}")) {
					bracket_depth --;
					if (bracket_depth <= 0)
						break;
				} else if (String_Match_Caseless(com_token, "{")) {
					bracket_depth ++;
				}
			} // While
			if (is_one_we_want) {
				is_done = true;

				// Start with the name, not any comments or whitespace before it.
				ccs *s_start = strstr (s0, namebuf64);
				ccs *s_beyond = text;
				fs_offset_t text_block_length = s_beyond - s_start;
				sout_zalloc = Z_StrDup_Len_Z (s_start, text_block_length);

				// Baker: Kill toxic whitespace (TABS, CARRIAGES) but not NEWLINES
				for (char *whitespace_clean = sout_zalloc; *whitespace_clean; whitespace_clean ++) {
					if (*whitespace_clean < SPACE_CHAR_32 && *whitespace_clean != NEWLINE_CHAR_10)
						*whitespace_clean = SPACE_CHAR_32;
				} // for

				is_done = true;
				break; // We got what we need, quit looking...
			}
			s0 = text;
		} // while in the shader file
		Mem_Free (f); // free the file
		if (is_done) break; // No more checking.
	} // for each file
	FS_FreeSearch(search);

	return sout_zalloc;
}

int Shader_append (stringlist_t *plist, ccs *partial)
{
	int num_found = 0;
	fssearch_t *search = FS_Search("scripts/*.shader", fs_caseless_true, fs_quiet_FALSE, fs_pakfile_null, fs_gamedironly_false);

	if (!search)
		return num_found;

	//int is_done = false;

	for (int fileindex = 0; fileindex < search->numfilenames; fileindex ++) {
		ccs *filename = search->filenames[fileindex];
		char *f_za;
		ccs *text = f_za = FS_LoadFileQuick (filename);

		if (!text)
			continue;

		int bracket_depth = 0;

		while (COM_ParseToken_QuakeC(&text, /*newline?*/ false)) {
			char namebuf64[Q3PATHLENGTH_64];
			c_strlcpy (namebuf64, com_token);

			if (String_Starts_With_Caseless (namebuf64, partial))
				stringlistappend (plist, namebuf64);

			if (!COM_ParseToken_QuakeC(&text, /*newline?*/ false) || String_NOT_Match(com_token, "{")) {
				//if (is_print) Con_PrintLinef ("%s parsing error - expected \"{\", found " QUOTED_S, search->filenames[fileindex], com_token);
				break;
			}

			// Parsed a "{"
			bracket_depth ++;
			while (COM_ParseToken_QuakeC(&text, false)) {
				if (String_Match_Caseless(com_token, "}")) {
					bracket_depth --;
					if (bracket_depth <= 0)
						break;
				} else if (String_Match_Caseless(com_token, "{")) {
					bracket_depth ++;
				}
			} // While
		} // while in the shader file

		Mem_Free (f_za); // free the file
	} // for each file
	FS_FreeSearch(search);

	return num_found;
}


void Pak_Accum_Texture_Dependencies (stringlist_t *ptexture_dependency_list, const char *s_texture_maybe_with_extension)
{
	const int is_print = false;
	char s_texture_in_no_extension[MAX_QPATH_128];
	char s_texture_in_wild[MAX_QPATH_128];

	FS_StripExtension (s_texture_maybe_with_extension, s_texture_in_no_extension, sizeof(s_texture_in_no_extension)); // This copies the filename, does more than strip

	c_strlcpy (s_texture_in_wild, s_texture_in_no_extension);
	c_strlcat (s_texture_in_wild, "*");

	fssearch_t *search = FS_Search(s_texture_in_wild, fs_caseless_true, fs_quiet_FALSE, fs_pakfile_null, fs_gamedironly_false);

	if (!search) {
		Con_PrintLinef ("No search results for " QUOTED_S, s_texture_in_wild);
		return;
	}

	for (int fileindex = 0; fileindex < search->numfilenames; fileindex ++) {
		char *s_this_file = search->filenames[fileindex];
		int is_ok = String_Ends_With_Caseless (s_this_file, ".tga") ||
					String_Ends_With_Caseless (s_this_file, ".png") ||
					String_Ends_With_Caseless (s_this_file, ".jpg") ||
					String_Ends_With_Caseless (s_this_file, ".jpeg");

		if (is_ok == false)
			continue; // unrecognized file format, we only do images we can load.

		stringlistappend (ptexture_dependency_list, s_this_file);
		if (is_print)
			Con_PrintLinef ("Added " QUOTED_S, s_this_file);
	} // for
}

		//const char *text = f = (char *)FS_LoadFile(search->filenames[fileindex], tempmempool, fs_quiet_FALSE, fs_size_ptr_null);
		//if (!f)
		//	continue;

// "pak_this_map"

//   .bsp.zip // must not be in a .pk3
//   //misc_mdl_entities ..
//   //misc_mdl_illusionary ...
//   textures/
//   scripts/
//   models/
//   maps/ ... any relevant shit, we aren't doing the lightmap

// Not captured:
// misc_mdl_entities, misc_mdl_illusionary, sounds, cdtrack
void Texture_Dependencies_For_This_Model (model_t *m, stringlist_t *ptexture_dependency_list,
								  stringlist_t *ptexture_shader_dependency_list,
								  stringlist_t *pshader_name_list,
								  stringlist_t *pshader_textblock_dependency_list, int is_print_stuff)
{
char *ShaderText_ZAlloc (shader_t *myshader, const char *s_shadername, char *s_return_shader, size_t s_return_shader_size);

	const int is_exclude_textures_common = false; // DON'T
	texture_t	*t;
	int j;
	for (j = 0, t = m->data_textures; j < m->num_textures; j++, t++) {
		char *s_this_texture = t->name;
		if (s_this_texture[0] == 0) continue;
		if (String_Contains_Caseless(s_this_texture, "NO TEXTURE FOUND"))	continue;

		shader_t *myshader = Mod_LookupQ3Shader(s_this_texture);
		//Con_PrintLinef ("%4d: %s", j, t->name);

		if (myshader == NULL) {
			// Normal texture
			Pak_Accum_Texture_Dependencies (ptexture_dependency_list, s_this_texture);
			continue;
		}

		stringlistappend (pshader_name_list, s_this_texture); //

		if (String_Starts_With (s_this_texture, "textures/common/")) {
			if (is_print_stuff)
				Con_PrintLinef (CON_BRONZE "Common texture found and  %s " QUOTED_S,
				s_this_texture, is_exclude_textures_common ? "EXCLUDING" : "ADDING!");

			if (is_exclude_textures_common)
				continue; // Do not add, we are excluding those
		}

		if (myshader) {
			char *s_shader_textblock_zalloc = ShaderText_ZAlloc (myshader, s_this_texture, /*get shader file name?*/ NULL, 0);

			if (s_shader_textblock_zalloc == NULL) {
				Con_PrintLinef (CON_ERROR "ERROR shader text is null %s", s_this_texture);
			} else {
				stringlistappend (pshader_textblock_dependency_list, s_shader_textblock_zalloc);
			}

			Mem_FreeNull_ (s_shader_textblock_zalloc)

			if (myshader->dpreflectcube[0]) {
				//Con_PrintLinef (" reflectcube = %s", myshader->dpreflectcube);
				Pak_Accum_Texture_Dependencies (ptexture_shader_dependency_list, myshader->dpreflectcube);
			}

			if (myshader->skyboxname[0]) {
				//Con_PrintLinef (" skybox = %s", myshader->skyboxname);
				Pak_Accum_Texture_Dependencies (ptexture_shader_dependency_list, myshader->skyboxname);
			}

			for (int k = 0; k < myshader->numlayers; k ++) {
				q3shaderinfo_layer_t *layer = myshader->layers + k;
				for (int framenum = 0; framenum < layer->sh_numframes; framenum ++) {
					const char *s_layer_texture = layer->sh_ptexturename[framenum];
					if (s_layer_texture == NULL) continue;
					if (String_Starts_With (s_layer_texture, "$")) {
						if (is_print_stuff)
							Con_PrintLinef ("Ignoring texture named " QUOTED_S, s_layer_texture);
						continue;
					}

					Pak_Accum_Texture_Dependencies (ptexture_shader_dependency_list, s_layer_texture);
				} // framenum

			} // for
		} // if
	} // for

}


//	ccs *s_entities = worldmodel->brush.entities;
void Sound_Dependencies_For_This_Model (model_t *m,
	stringlist_t *psound_dependency_list, ccs *s_entities, ccs *s_ext_, int is_print_stuff)
{
	char *src_za = Z_StrDup (s_entities);

	ccs *s_findthis_za = Z_StrDupf("%s\"", s_ext_); // Add trailing "
	int slen_findthis = strlen(s_findthis_za);
	char *s_current_start = Z_StrDup (s_entities);
	// Scan for .ogg
	char *s_found;
	while (ASSIGN (s_found = strstr (s_current_start, s_findthis_za)) ) {
		// Now search backwards
		// We need to stupidly null terminate at beyond point.
		char *s_beyond = s_found + slen_findthis - ONE_CHAR_1; // s_beyond is on final quote
		s_beyond[1] = NULL_CHAR_0;

		ccs *s_begin = String_Find_Reverse (s_current_start, "\"sound/");
		if (!s_begin) {
			// Found a string with no start.
			//int j = 5; // Baker: If this can happen ... this is bad.
			break;
		}
		s_begin ++; // Skip the starting "


		int slen = s_beyond - s_begin;
		ccs *s_instance_za = Z_StrDup_Len_Z (s_begin, slen);
		stringlistappend (psound_dependency_list, s_instance_za);
		Mem_FreeNull_ (s_instance_za);

		// Update current start for next iteration
		s_current_start = &s_beyond[2];
	}
	Mem_FreeNull_ (src_za);
	Mem_FreeNull_ (s_findthis_za);

}

// Baker: A filecopy that does not use the operating system and the date and time are not
// preserved.

// Baker: FS_FileCopy_Shitty s_src is from path system, the s_dst is REAL FILE output
// Returns number of bytes written or 0 on failure.
// It is common for this return result of this to be accumulated.
CALLERS_ (FS_FileCopy_Shitty exclusive at the moment)
fs_offset_t FS_FileCopy_Shitty (const char *s_src, const char *s_dst_in)
{
	fs_offset_t filedata_datalen;
	byte *filedata = FS_LoadFile(s_src, tempmempool, fs_quiet_FALSE, &filedata_datalen);

	if (filedata == NULL) {
		Con_PrintLinef (CON_ERROR "Could not open file source " QUOTED_S, s_src);
		return 0;
	}

	// Baker: It is a real file ...
	// Baker: Now ... make sure dest does not exist

	va_super (realpath, MAX_OSPATH_EX_1024, "%s%s", fs_gamedir, s_dst_in);

	qbool is_dest_already_existing = FS_File_Or_Directory_Exists_Unsafe (realpath);
	if (is_dest_already_existing) {
		Con_PrintLinef (CON_ERROR "Dest already exists ... ignoring ... " QUOTED_S, realpath);
		return 0;
	}

	FS_CreatePath (realpath); // Baker: Only file writing attempts to create the path
	qbool is_ok = FS_Bytes_To_File_With_Size_Is_Ok_Unsafe (realpath, filedata, filedata_datalen);
	Mem_Free (filedata);
	if (!is_ok) {
		Con_PrintLinef (CON_ERROR "Could not open " QUOTED_S, realpath);
		return 0;
	}

	return filedata_datalen; // SUCCESS!
}


