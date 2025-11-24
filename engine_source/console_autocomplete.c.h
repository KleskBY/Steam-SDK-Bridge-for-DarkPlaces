// console_autocomplete.c.h



// Performed by:
RELATED_ (Key_ClearEditLine if console which is called by Con_ToggleConsole)
// Con_ToggleConsole resets history to -1
// Partial reset needs to be performed in the console
// by damn near any key that is not TAB or shift TAB

// What keys should not reset autocomplete
// 1. TAB and shift TAB
// 2. ALT-TAB
// 3. Probably ALT-ENTER
// 4. Mousewheel scrolling through history
// 5. Setting zoom like CTRL-ZERO or CTRL-PLUS or CTRL-MINUS


autocomplete_t _g_autocomplete;

int GetSkyList_Count (const char *s_prefix)
{
	fssearch_t	*t;
	char		spattern[1024] = "gfx/env/*"; // Default if no prefix provided
	int			num_matches = 0;
	int			j;

	// Baker: if a prefix was provided use it literally
	// We hope it looks like "gfx/env/s"
	if (s_prefix && s_prefix[0]) {
		c_dpsnprintf1 (spattern, "%s*", s_prefix);
	}

	t = FS_Search(spattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);
	if (t && t->numfilenames > 0) {
		num_matches = t->numfilenames;
		for (j = 0; j < t->numfilenames; j++) {
			char *sxy = t->filenames[j];
			File_URL_Edit_Remove_Extension (sxy);
			// We are using _rt .. what about the other crazy supported suffixes like "pz"
			// Nah .. we are doing Quake skyboxes only
			int slen = (int)strlen (sxy); // We know strlen >= 3
			if (String_Ends_With (sxy, "_rt")) {
				sxy[slen - 3] = 0;
			} else if (String_Ends_With (sxy, "rt")) {
				sxy[slen - 2] = 0;
			} else {
				// No trail of _rt or rt
				continue;
			}

			SPARTIAL_EVAL_

			num_matches ++;
		} // for
	} // if


	if (t) FS_FreeSearch(t);

	return num_matches;
}



RELATED_ (PRVM_ED_Eset_f PRVM_Fields_Query PRVM_ED_EdictGet_Fly )
// Baker: is_from_nothing prints a ton of extra stuff
int GetFieldListServer_Count (ccs *s_progname, ccs *s_prefix, qbool is_from_nothing)
{
#ifdef CONFIG_MENU
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);

	stringlist_t list = {0};
	int num_matches = 0;

	if (!prog) {
		//Con_PrintLinef ("program %s not active/found", s_progname);
		return 0; // No matches because bad program name
	}

	for (int idx = 0; idx < prog->numfielddefs; idx ++) {
		mdef_t		*def	= &prog->fielddefs[idx];
		int			type	= def->type;
		int			vartype	= Flag_Remove(type, DEF_SAVEGLOBAL);
		const char	*s_key0	= PRVM_GetString(prog, def->s_name);

		ccs *s_type = VarType_For_EV(vartype);
		if (strlen(s_key0) == 0)
			continue; // Ignore blank
		if (is_from_nothing) {
			va_super (sxy, 1024, "%s // %s %s fieldnum #%d", s_key0, s_type, s_progname, idx);
			stringlistappend (&list, sxy);
		} else {
			va_super (sxy, 1024, "%s", s_key0);
			stringlistappend (&list, sxy);
		}
	} // for

	stringlistsort (&list, fs_make_unique_true);

	for (int idx = 0; idx < list.numstrings; idx++) {
		const char *sxy =  list.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		num_matches ++;
		SPARTIAL_EVAL_
	} // idx

	stringlistfreecontents (&list);
	return num_matches;
#else
	return 0;
#endif
}

int GetGlobalListServer_Count (ccs *s_progname, ccs *s_prefix, qbool is_from_nothing)
{
#ifdef CONFIG_MENU
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);

	stringlist_t list = {0};
	int num_matches = 0;

	if (!prog) {
		//Con_PrintLinef ("program %s not active/found", s_progname);
		return 0; // No matches because bad program name
	}

	//RELATED_ (PRVM_ED_GlobalGet_f  PRVM_Global_f PRVM_Globals_f)
	for (int idx = 0; idx < prog->numglobaldefs; idx ++) {
		mdef_t		*def	= &prog->globaldefs[idx];
		int			type	= def->type;
		int			vartype	= Flag_Remove(type, DEF_SAVEGLOBAL);
		const char	*s_key0	= PRVM_GetString(prog, def->s_name);

		//if  (vartype == ev_field_5)
		//	continue; // Don't do these?

		if (strlen(s_key0) == 0)
			continue; // Ignore blank

		if (false == Have_Flag (def->type, DEF_SAVEGLOBAL))
			continue; // self, other, ...

		// Baker: What are the types that are rejected here? .. vector  cause xyz is wriiten
		// filter _x _y _z
		if (false == isin4 (vartype, ev_string_1, ev_float_2, ev_vector_3, ev_entity_4))
			continue;

		// Any of the following suffix we ignore, covered by vector.
		if (vartype == ev_float_2 && String_Ends_With_Caseless_3 (s_key0, "_x", "_y", "_z"))
			continue;

		RELATED_ (PRVM_Globals_Query)
		ccs *s_type = VarType_For_EV(vartype);
		if (is_from_nothing) {
			va_super (sxy, 1024, "%s // %s %s globalnum #%d", s_key0, s_type, s_progname, idx);
			stringlistappend (&list, sxy);
		} else {
			va_super (sxy, 1024, "%s", s_key0);
			stringlistappend (&list, sxy);
		}

	} // for

	stringlistsort (&list, fs_make_unique_true);

	for (int idx = 0; idx < list.numstrings; idx++) {
		const char *sxy =  list.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		num_matches ++;
		SPARTIAL_EVAL_
	} // idx

	stringlistfreecontents (&list);
	return num_matches;
#else
	return 0;
#endif
}

int GetTexMode_Count (const char *s_prefix)
{
	// This list has to be alpha sorted for
	// autocomplete to work right
	const char *slist[] =  {
		"GL_LINEAR",
		"GL_LINEAR_MIPMAP_LINEAR",
		"GL_LINEAR_MIPMAP_NEAREST",
		"GL_NEAREST",
		"GL_NEAREST_MIPMAP_LINEAR",
		"GL_NEAREST_MIPMAP_NEAREST",
	};

	int	array_count = (int)ARRAY_COUNT(slist);
	int num_matches = 0;

	for (int idx = 0; idx < array_count; idx++) {
		const char *sxy =  slist[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

	} // idx
	return num_matches;
}


/*
GetMapList

Made by [515]
Prints not only map filename, but also
its format (q1/q2/q3/hl) and even its message
*/
//[515]: here is an ugly hack.. two gotos... oh my... *but it works*
//LadyHavoc: rewrote bsp type detection, rewrote message extraction to do proper worldspawn parsing
//LadyHavoc: added .ent file loading, and redesigned error handling to still try the .ent file even if the map format is not recognized, this also eliminated one goto
//LadyHavoc: FIXME: man this GetMapList is STILL ugly code even after my cleanups...

int m_maplist_count;
qbool m_maplist_type_is_menu; // 0
maplist_s m_maplist[MAXMAPLIST_4096];

// Baker r0086: Rewritten so .obj maps located in maps folder are listed like other maps.
qbool GetMapList (const char *s_partial, char *completedname,
	int completednamebufferlength, int is_menu_fill,
	int is_zautocomplete, int is_suppress_print)
{
	fssearch_t	*t_bsp;
	fssearch_t	*t_obj;
	char		s_pattern[1024];
	char		s_map_title[1024];
	int			j, k, our_max_length, p, partial_length, our_min_length;
	unsigned char *lengths_array;
	qfile_t		*f;
	unsigned char buf[1024];

	if (is_menu_fill) {
		m_maplist_count = 0;
	}

	c_dpsnprintf1 (s_pattern, "maps/%s*.bsp", s_partial);

	t_bsp = FS_Search(s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, is_menu_fill ? fs_gamedironly_true : fs_gamedironly_false);

	c_dpsnprintf1 (s_pattern, "maps/%s*.obj", s_partial);
	t_obj = FS_Search(s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, is_menu_fill ? fs_gamedironly_true : fs_gamedironly_false);

	if (!t_bsp && !t_obj)
		return 0;

	// Baker: Make list
	stringlist_t	maplist;

	stringlistinit	(&maplist);

	if (t_bsp) {
		fssearch_t	*pt = t_bsp;
		for (int idx = 0; idx < pt->numfilenames; idx++ ) {
			stringlistappend (&maplist, pt->filenames[idx]);
		} // for

		FS_FreeSearch(pt);
	} // if

	if (t_obj) {
		fssearch_t	*pt = t_obj;
		for (int idx = 0; idx < pt->numfilenames; idx++ ) {
			stringlistappend (&maplist, pt->filenames[idx]);
		} // for
		FS_FreeSearch (pt);
	} // if

	stringlistsort	(&maplist, fs_make_unique_true);

	if (maplist.numstrings > 1) {
		if (is_menu_fill == false && is_suppress_print == false)
			Con_PrintLinef (CON_BRONZE " %d maps found:", maplist.numstrings);
	}

	lengths_array = (unsigned char *)Z_Malloc(maplist.numstrings);
	our_min_length = 666;

	for (our_max_length = j = 0; j < maplist.numstrings; j++) {
		k = (int)strlen(maplist.strings[j]);
		k -= 9; // Why 9?  maps/ is 5, .bsp is 4
		if (our_max_length < k)
			our_max_length = k;
		else
		if (our_min_length > k)
			our_min_length = k;
		lengths_array[j] = k;
	}
	partial_length = (int)strlen(s_partial);

	for (j = 0; j < maplist.numstrings; j ++) {
		char *s_this_filename = maplist.strings[j];
		int lumpofs = 0, lumplen = 0;
		char *entities = NULL;
		const char *data = NULL;
		char keyname[64];
		char entfilename[MAX_QPATH_128];
		char desc[64];
		int map_format_code = 0;
		int is_playable = false;
		desc[0] = 0;
		int is_obj_map = String_Ends_With (s_this_filename, ".obj");

		c_strlcpy (s_map_title, CON_RED "ERROR: open failed" CON_WHITE);

		p = 0;
		f = FS_OpenVirtualFile(s_this_filename, fs_quiet_true);

		if (f) {
			c_strlcpy (s_map_title, CON_RED "ERROR: not a known map format" CON_WHITE);

			memset(buf, 0, 1024);
			FS_Read(f, buf, 1024);
			//#pragma message ("Baker: It is said that .bsp that are .md3
			//or such bypass requirement of info_player_start")
			if (is_obj_map) {
					c_strlcpy (desc, "OBJ");
					map_format_code = 7;
			} else if (!memcmp(buf, "IBSP", 4)) {
				p = LittleLong(((int *)buf)[1]);
				if (isin3 (p, Q3BSPVERSION_46, Q3BSPVERSION_LIVE_47, Q3BSPVERSION_IG_48)) {
					q3dheader_t *header = (q3dheader_t *)buf;
					lumpofs = LittleLong(header->lumps[Q3LUMP_ENTITIES].fileofs);
					lumplen = LittleLong(header->lumps[Q3LUMP_ENTITIES].filelen);
					c_dpsnprintf1 (desc, "Q3BSP%d", p);
					if (p == 47)		map_format_code = 8;
					else if (p == 48)	map_format_code = 9;
					else map_format_code = 3;
				}
				else if (p == Q2BSPVERSION) {
					q2dheader_t *header = (q2dheader_t *)buf;
					lumpofs = LittleLong(header->lumps[Q2LUMP_ENTITIES].fileofs);
					lumplen = LittleLong(header->lumps[Q2LUMP_ENTITIES].filelen);
					c_dpsnprintf1 (desc, "Q2BSP%d", p);
					map_format_code = 2;
				}
				else {
					c_dpsnprintf1 (desc, "IBSP%d", p);
					map_format_code = 4;
				}
			} else if (BuffLittleLong(buf) == BSPVERSION /*29*/) {
				lumpofs = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES);
				lumplen = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES + 4);
				c_strlcpy (desc, "BSP29");
				map_format_code = 1;
			} else if (BuffLittleLong(buf) == 30 /*Half-Life*/) {
				lumpofs = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES);
				lumplen = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES + 4);
				c_strlcpy (desc, "BSPHL");
				map_format_code = -1;
			} else if (!memcmp(buf, "BSP2", 4)) {
				lumpofs = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES);
				lumplen = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES + 4);
				c_strlcpy (desc, "BSP2");
				map_format_code = 1;
			} else if (!memcmp(buf, "2PSB", 4)) {
				lumpofs = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES);
				lumplen = BuffLittleLong(buf + 4 + 8 * LUMP_ENTITIES + 4);
				c_strlcpy (desc, "BSP2RMQe");
				map_format_code = 1;
			} else if (!memcmp(buf, "VBSP", 4)) {
				hl2dheader_t *header = (hl2dheader_t *)buf;
				lumpofs = LittleLong(header->lumps[HL2LUMP_ENTITIES].fileofs);
				lumplen = LittleLong(header->lumps[HL2LUMP_ENTITIES].filelen);
				int versi = LittleLong(((int *)buf)[1]);
				c_dpsnprintf1 (desc, "VBSP%d", versi);
				map_format_code = 5;
			} else {
				c_dpsnprintf1(desc, "unknown%d", BuffLittleLong(buf));
				map_format_code = -2;
			}
			c_strlcpy (entfilename, s_this_filename);
			memcpy (entfilename + strlen(entfilename) - 4, ".ent", 5);  // DOT_ENT (4)
			entities = (char *)FS_LoadFile(entfilename, tempmempool, fs_quiet_true, fs_size_ptr_null);

			if (entities == NULL && lumplen >= 10) {
				FS_Seek(f, lumpofs, SEEK_SET);
				entities = (char *)Z_Malloc(lumplen + 1);
				FS_Read(f, entities, lumplen);
			}
			if (entities) {
				// if there are entities to parse, a missing message key just
				// means there is no title, so clear the message string now
				s_map_title[0] = 0;
				is_playable = String_Contains (entities, "info_player_start") ||
					String_Contains (entities, "info_player_deathmatch");

				data = entities;
				for (;;) {
					int this_len;
					if (!COM_ParseToken_Simple(&data, false, false, true))
						break;
					if (com_token[0] == '{')
						continue;
					if (com_token[0] == '}')
						break;

					// skip leading whitespace
					for (k = 0; com_token[k] && ISWHITESPACE(com_token[k]);k++);
					for (this_len = 0; this_len < (int)sizeof(keyname) - 1 && com_token[k+this_len] && !ISWHITESPACE(com_token[k+this_len]);this_len++) {
						keyname[this_len] = com_token[k + this_len];
					} // for
					keyname[this_len] = 0;
					if (!COM_ParseToken_Simple(&data, false, false, true))
						break;
					if (developer_extra.integer)
						Con_DPrintLinef ("key: %s %s", keyname, com_token);
					if (String_Match(keyname, "message")) {
						// get the map title
						c_strlcpy (s_map_title, com_token);
						break;
					} // if "message"
				} // for
			} // if entities
		}
		if (entities)
			Z_Free(entities);
		if (f)
			FS_Close(f);

		if (is_obj_map == false) {
			*(s_this_filename + lengths_array[j] + 5) = NULL_CHAR_0; // Strips extension
		}

		// Ignore unplayable map
		if (!is_playable)
			continue;

		if (is_zautocomplete) {
			const char *sxy = s_this_filename + 5;

			SPARTIAL_EVAL_
		}

		if (is_menu_fill == false && is_suppress_print == false) {
			// If we print, print only maps with spawnpoints
			// This should avoid healthboxes and such
			//if (is_zautocomplete == false) {
				// Zircon autocomplete does not print here on 2nd autocomplete
				// For example, press TAB it prints
				// Press TAB again, it does not
				Con_PrintLinef ("%16s (%-8s) %s", s_this_filename + 5, desc, s_map_title);
			//}
		} else {
			// Map fill
			if (m_maplist_count < (int)ARRAY_COUNT(m_maplist) /* map with no title should still list */ /*&& s_map_title[0]*/) {
				// Baker: Explanation ...
				// The maps menu column 1 is "E1M1" we limit this to 15 characters, hence buffer size of 16
				// The maps title column is limited to 27 characters
				// However, we need the full filename after "maps/" in case we truncated
				//
				char s_file_trunc_at_16[16];
				char s_title_trunc_at_28[28];

				c_strlcpy (s_file_trunc_at_16, s_this_filename + 5);
				c_strlcpy (s_title_trunc_at_28, s_map_title);

				maplist_s *mx = &m_maplist[m_maplist_count];

				freenull_ (mx->s_name_after_maps_folder_a);
				freenull_ (mx->s_name_trunc_16_a);
				freenull_ (mx->s_map_title_trunc_28_a);

				mx->s_name_after_maps_folder_a	= (unsigned char *)strdup	(s_this_filename + 5);
				mx->s_name_trunc_16_a			= (unsigned char *)strdup	(s_file_trunc_at_16);
				mx->s_map_title_trunc_28_a		= (unsigned char *)strdup	(s_title_trunc_at_28);
				mx->s_bsp_code					= (unsigned char *) (
					isin1 (map_format_code, 8) ? "QL" :
					isin1 (map_format_code, 9) ? "IG" :
					isin2 (map_format_code, 3,4) ? "Q3" :
					isin1 (map_format_code, 7)	  ? "OBJ" :
							"");
				m_maplist_count ++;
			} // if m_maplist_count < max
		} // if menu fill
	} // for numfilenames
	if (is_menu_fill == false && is_suppress_print == false)
		Con_Print("\n");

	for (p = partial_length; p < our_min_length; p ++)
	{
		k = *(maplist.strings[0] + 5 + p);
		if (k == 0)
			goto endcomplete;
		for (j = 1; j < maplist.numstrings; j++)
			if (*(maplist.strings[j] + 5 + p) != k)
				goto endcomplete;
	}
endcomplete:
	if (p > partial_length && completedname && completednamebufferlength > 0) {
		memset(completedname, 0, completednamebufferlength);
		memcpy(completedname, (maplist.strings[0] + 5), Smallest(p, completednamebufferlength - 1));
	}
	Z_Free(lengths_array);

	stringlistfreecontents (&maplist);

	return p > partial_length;
}

int GetVideoList_Count (const char *s_prefix)
{
	// Get any .dpv
	// Get any _fps folders
	// Get any .gif
	char		s_pattern[1024];

	stringlist_t list = {0};
	fssearch_t	*t = NULL;

	// .gif
	c_dpsnprintf1 (s_pattern, "video/%s*.gif", s_prefix);
	t = FS_Search (s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);
	stringlistappendfssearch (&list, t);
	FS_FreeSearch_Null_ (t);

	// .dpv
	c_dpsnprintf1 (s_pattern, "video/%s*.dpv", s_prefix);
	t = FS_Search (s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);
	stringlistappendfssearch (&list, t);
	FS_FreeSearch_Null_ (t);

	// Baker: .jpeg ... I want the autocomplete to work with the files in a .pak
	// However, .pk3 and .pak remove the directories information on load.
	// So this will not autocomplete from pk3 or pak
	// "playvideo" will work fine
	{
		stringlist_t all_contents_of_video_folder_list = {0};

#if 1
		char gamedir_slash_video_slash[MAX_OSPATH_EX_1024]; // id1/video/
		c_dpsnprintf2 (gamedir_slash_video_slash, "%s%s", fs_gamedir /* "id1/" */ , "video/" );
		// game pathos tends to be "id1/" here
		// game pathos tends to be "id1/progs/"

		// Baker: I totally don't trust this at this time.
		// This returns file names without the path
		stringlist_append_listdirectory	(&all_contents_of_video_folder_list, gamedir_slash_video_slash /* id1/video/ */, "");

		//stringlist_condump_with_linenums (&all_contents_of_video_folder_list);

		stringlist_t fps_list = {0};
		stringlist_t *plist = &all_contents_of_video_folder_list;
		for (int idx = 0; idx < plist->numstrings; idx++) {
			char *sxy = plist->strings[idx];

			if (false == String_Ends_With (sxy, "_fps"))
				continue;

			if (false == String_Starts_With (sxy, s_prefix))
				continue;

			// Baker: The autocomplete doesn't want the full name just the partial

			char nameout[MAX_OSPATH_EX_1024];
			c_strlcpy (nameout, "video/");
			c_strlcat (nameout, sxy);
			stringlistappend (&fps_list, nameout);
		} // for
		//stringlist_condump_with_linenums (&fps_list);

		stringlistappendlist (&list, &fps_list);

		//
		stringlistfreecontents (&all_contents_of_video_folder_list);
		stringlistfreecontents (&fps_list);

#endif

	}
	stringlistsort (&list, fs_make_unique_true);

	int num_matches = 0;

	// Baker: We want to keep the extension
	// Baker: We want to remove "video\" prefix
	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *sxy = list.strings[idx];

		// Remove video/ prefix
		sxy += STRINGLEN("video/");

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents (&list);

	return num_matches;
}

// Ideal is that is path aware and returns path matches
// And filter extensions.
// s_dot_extension_commad = "

// search1_ui_form_load_31 is sole user.  What does this do differently?
// This is a multi-extension comma delimited not-aware search.
// This ADDs directory matches.

// Baker: This seems PATH AWARE! August 1 2025
// Q: Why is the code so short?  Is this an upgrade?
RELATED_ (GetFiles_Aware_Commas_Count)
#define EXCL_NULL NULL
int GetFileList_Awareness (ccs *s_prefix, ccs *s_comma_dot_extensions, /*OPTIONAL*/ ccs *s_command_prohibited_suffixes)
{
	stringlist_t list_prohibited_suffixes = {0}; // Like textures that end with _gloss
	
	if (s_command_prohibited_suffixes)
		stringlistappend_split (&list_prohibited_suffixes, s_command_prohibited_suffixes, ",");

	int			num_matches = 0;
	stringlist_t listextensions = {0};
	stringlistappend_split (&listextensions, s_comma_dot_extensions, ",");

	stringlist_t list = {0};

	// Add file matches for every extension
	for (int idx = 0; idx < listextensions.numstrings; idx++) {
		ccs *s_dot_ext = listextensions.strings[idx];
		va_super (s_pattern, MAX_QPATH_128, "%s*%s", s_prefix, s_dot_ext); // "en*.png"
		stringlistappend_from_pattern (&list, s_pattern);
	}

	// Add directory completions
	stringlistappend_dir_matches (&list, s_prefix);

	stringlistsort_unique (&list);

	for (int idx = 0; idx < list.numstrings; idx++) {
		ccs *sxy = list.strings[idx];
#if 1
		if (list_prohibited_suffixes.numstrings) {
			va_super (noext, MAX_OSPATH_EX_1024, "%s", sxy);
			File_URL_Edit_Remove_Extension (noext);

			if (stringlist_string_endswithany(noext, &list_prohibited_suffixes))
				continue;  // REJECTED FOR SUFFIX (like _GLOSS)
		}
#endif
		SPARTIAL_EVAL_
		num_matches ++;
	}

	stringlistfreecontents (&list_prohibited_suffixes);
	stringlistfreecontents (&listextensions);
	stringlistfreecontents (&list);
	return num_matches;
}

// Baker: Not path aware
int GetFileList_Not_Path_Aware_Count (ccs *s_folder_with_slash_or_null, ccs *s_prefix, ccs *s_dot_extension, int is_strip_extension)
{
	fssearch_t	*t;
	//char		s_pattern[1024];
	int			num_matches = 0;
	int			j;

#if 0
	if (s_folder_with_slash_or_null) // Baker: This is always false because no caller uses it.
		c_dpsnprintf3 (s_pattern, "%s%s*%s", s_folder_with_slash_or_null, s_prefix, s_dot_extension);
	else
		c_dpsnprintf2 (s_pattern, "%s*%s", s_prefix, s_dot_extension);
#endif
	
	va_super (s_pattern, MAX_OSPATH_EX_1024, "%s*%s", s_prefix, s_dot_extension);

	if (s_folder_with_slash_or_null)
		String_Edit_Insert_At_0_Self_Safe (s_pattern, sizeof(s_pattern), s_folder_with_slash_or_null);

	t = FS_Search (s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	if (t && t->numfilenames > 0) {
		int array_count = t->numfilenames;

		for (j = 0; j < array_count; j ++) {
			char *sxy = t->filenames[j];

			if (is_strip_extension)
				File_URL_Edit_Remove_Extension (sxy);

			if (s_folder_with_slash_or_null)
				sxy += strlen (s_folder_with_slash_or_null);

			SPARTIAL_EVAL_

			num_matches ++;
		} // for
	} // if

	if (t) FS_FreeSearch(t);

	return num_matches;
}

#if 0
int GetFiles_Aware_Commas_Count (ccs *s_folder_with_slash_or_null, ccs *s_prefix, ccs *s_comma_dot_extensions, int is_strip_extension) // "play"
{
	int			num_matches = 0;

	stringlist_t list_extensions = {0};
	stringlistappend_split	(&list_extensions, s_comma_dot_extensions, ",");

	// WIDE - WILDCARD SEARCH NO EXTENSION FILTER.
	char		s_pattern[1024];

	if (s_folder_with_slash_or_null)
		c_dpsnprintf2 (s_pattern, "%s%s*", s_folder_with_slash_or_null, s_prefix);
	else
		c_dpsnprintf1 (s_pattern, "%s*", s_prefix);

	stringlist_t list_search_results = {0};
	stringlistappend_search_pattern (&list_search_results, s_pattern);
	stringlistsort (&list_search_results, fs_make_unique_true);

	for (int j = 0; j < list_search_results.numstrings; j ++) {
		char *sxy = list_search_results.strings[j];
		va_super (noext, MAX_OSPATH_EX_1024, "%s", sxy);

		// EXTENSION REJECTION TEST
		qbool is_rejected = false;

		ccs *sxy_dot_ext = File_URL_GetExtension_With_Dot (sxy);
		if (sxy_dot_ext) {
			// It has an extension.  Does it match a desired extension?
			qbool is_extension_wanted = false;
			for (int ext_idx = 0; ext_idx < list_extensions.numstrings; ext_idx ++) {
				char *s_dot_extension = list_extensions.strings[ext_idx];
				if (String_Match_Caseless (sxy_dot_ext, s_dot_extension)) {
					is_extension_wanted = true;
					break;
				}
			} // for

			is_rejected = (is_extension_wanted == false);
		} // if

		if (is_rejected)
			continue;
		// END REJECTION TEST

		File_URL_Edit_Remove_Extension (noext);

		if (is_strip_extension)
			File_URL_Edit_Remove_Extension (sxy);

		if (s_folder_with_slash_or_null)
			sxy += strlen (s_folder_with_slash_or_null);

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

exitor:
	stringlistfreecontents (&list_search_results);
	stringlistfreecontents (&list_extensions);

	return num_matches;
}

RELATED_ (GetCommad_Count) // r_replacemaptexture uses this.

int GetFiles_Aware_Except_Count (ccs *s_folder_with_slash_or_null, ccs *s_prefix, ccs *s_dot_extension, ccs *s_comma_trail_list, int is_strip_extension)
{
	int			num_matches = 0;

	stringlist_t list_trails = {0};
	stringlistappend_split(&list_trails, s_comma_trail_list, ",");

	char		s_pattern[1024];
	if (s_folder_with_slash_or_null)
		c_dpsnprintf3 (s_pattern, "%s%s*%s", s_folder_with_slash_or_null, s_prefix, s_dot_extension);
	else
		c_dpsnprintf2 (s_pattern, "%s*%s", s_prefix, s_dot_extension);

	stringlist_t list_search_results = {0};
	stringlistappend_search_pattern (&list_search_results, s_pattern);

	for (int j = 0; j < list_search_results.numstrings; j ++) {
		char *sxy = list_search_results.strings[j];
		va_super (noext, MAX_OSPATH_EX_1024, "%s", sxy);

		File_URL_Edit_Remove_Extension (noext);

		qbool is_rejected = false;
		for (int ext_idx = 0; ext_idx < list_trails.numstrings; ext_idx ++) {
			char *strail = list_trails.strings[ext_idx];
			if (String_Ends_With (noext, strail)) {
				is_rejected = true;
				break;
			}
		} // for

		if (is_rejected)
			continue;

		if (is_strip_extension)
			File_URL_Edit_Remove_Extension (sxy);

		if (s_folder_with_slash_or_null)
			sxy += strlen (s_folder_with_slash_or_null);

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

exitor:
	stringlistfreecontents (&list_search_results);
	stringlistfreecontents (&list_trails);

	return num_matches;
}
#endif

int GetModList_Count (ccs *s_prefix)
{
	char *s_current_working_dir_ta = FS_Getcwd_No_Trailing_Slash_Unsafe_TempAlloc (); // Sys_Getcwd_SBuf(); // No trailing slash

	// Just in case
	String_Edit_RemoveTrailingUnixSlash (s_current_working_dir_ta);

	// Determine Quake folder
	char s_quake_folder_trail_slash[1024];
	if (fs_basedir[0])	c_strlcpy		(s_quake_folder_trail_slash, fs_basedir);
	else				c_dpsnprintf1	(s_quake_folder_trail_slash, "%s/", s_current_working_dir_ta);

	// Get list of files in Quake folder
	stringlist_t list = {0};

	stringlist_append_listdirectory	(&list, s_quake_folder_trail_slash, fs_pattern_all_empty_string);
	stringlistsort_unique			(&list);

	int num_matches = 0;
	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *s_this = list.strings[idx];

		if (String_Is_Dot(s_this) || String_Is_DotDot(s_this))
			continue; // ignore "." and ".." as filenames

		if (String_Starts_With_Caseless (s_this, s_prefix) == false)
			continue; // Not prefix match

		if (String_Isin2 (s_this, "bin32", "bin64"))
			continue; // These folders are for DLL

		va_super (s_this_fullpath, MAX_OSPATH_EX_1024, "%s%s", s_quake_folder_trail_slash, s_this);

		int file_directory_type = FS_SysFileOrDirectoryType (s_this_fullpath);

		if (file_directory_type != FS_FILETYPE_DIRECTORY_2)
			continue; // It is not a directory

		// Baker: FS_CheckGameDir does so little AFAIK
		const char *s_description = FS_CheckGameDir(s_this);
		if (s_description == NULL || s_description == fs_checkgamedir_missing)
			continue;

		char *sxy = s_this;

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents(&list);
	Mem_FreeNull_ (s_current_working_dir_ta);

	return num_matches;
}

// Expands format and returns 1.
int Get_Solo_Count (ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	int			num_matches = 0;

	char *sxy =  text;

	SPARTIAL_EVAL_
	num_matches ++;

	VA_EXPAND_ALLOC_FREE (text);
	return num_matches;
}

int GetCommad_Count (const char *s_prefix, const char *s_comma_separated_items)
{
	int			num_matches = 0;

	stringlist_t list = {0};
	stringlistappend_split	(&list, s_comma_separated_items, ",");
	stringlistsort_unique (&list);

	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *sxy =  list.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // idx

	stringlistfreecontents (&list);
	return num_matches;
}

int GetStringList_Sorted_Count (stringlist_t *plist, ccs *s_prefix)
{
	int			num_matches = 0;
	for (int idx = 0; idx < plist->numstrings; idx ++) {
		char *sxy = plist->strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // idx

	return num_matches;
}



int GetAny1_Count (const char *s_prefix, const char *s_singleton)
{
	int			num_matches = 0;
	// Ok .. this has to be sorted due to first/last.
	const char *slist[] =  {
		s_singleton,
	};

	int			array_count = (int)ARRAY_COUNT(slist);

	for (int idx = 0; idx < array_count; idx ++) {
		const char *sxy =  slist[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // idx
	return num_matches;
}

//int GetCopyCmd_Count (const char *s_prefix)
//{
//	int			num_matches = 0;
//	// Ok .. this has to be sorted due to first/last.
//	const char *slist[] =  {
//		"ents",
//	};
//
//	int			array_count = (int)ARRAY_COUNT(slist);
//
//	for (int idx = 0; idx < array_count; idx ++) {
//		const char *sxy =  slist[idx];
//		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
//			continue;
//
//		SPARTIAL_EVAL_
//
//		num_matches ++;
//	} // idx
//	return num_matches;
//}

//int GetEdictsCmd_Count (const char *s_prefix)
//{
//	int num_matches = 0;
//	// Ok .. this has to be sorted due to first/last.
//	const char *slist[] =  {
//		"classname",
//		"message",
//		"model",
//		"noise",
//		"targetname",
//	};
//
//	int			array_count = (int)ARRAY_COUNT(slist);
//
//	for (int idx = 0; idx < array_count; idx ++) {
//		const char *sxy =  slist[idx];
//		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
//			continue;
//
//		SPARTIAL_EVAL_
//
//		num_matches ++;
//	} // idx
//	return num_matches;
//}

//
int GetREditLightsEdit_Count (const char *s_prefix)
{
	// Ok .. this has to be sorted due to first/last.
	const char *slist[] =  {
		"ambient",
		"angles",
		"anglesx",
		"anglesy",
		"anglesz",
		"color",
		"colorscale",
		"corona",
		"coronasize",
		"cubemap",
		"diffuse",
		"move",
		"movex",
		"movey",
		"movez",
		"normalmode",
		"origin",
		"originscale",
		"originx",
		"originy",
		"originz",
		"radius",
		"radiusscale",
		"realtimemode",
		"shadows",
		"sizescale",
		"specular",
		"style",
	};

	int array_count = (int)ARRAY_COUNT(slist);
	int	num_matches = 0;

	for (int idx = 0; idx < array_count; idx++) {
		const char *sxy =  slist[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_
	} // idx
	return num_matches;
}

int GetModelList_Count (const char *s_prefix)
{
	char		spattern[1024];
	char		s_prefix2[1024] = {0};

	if (s_prefix && s_prefix[0]) {
		// Remove ext ... Why?
		c_strlcpy (s_prefix2, s_prefix);
		File_URL_Edit_Remove_Extension (s_prefix2);
	} else {
		c_strlcpy (s_prefix2, "progs/");

	}

	c_dpsnprintf1 (spattern, "%s*.mdl", s_prefix2);

	fssearch_t	*t = FS_Search (spattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	int num_matches = 0;
	if (t && t->numfilenames > 0) {
		// The file search already checked prefix validity
		for (int idx = 0; idx < t->numfilenames; idx++) {
			char *sxy = t->filenames[idx];

			SPARTIAL_EVAL_

			num_matches ++;
		} // for
	} // if

	if (t) FS_FreeSearch(t);

	return num_matches;
}

int GetTexGeneric_Count (const char *s_prefix)
{
	char		spattern[1024];
	char		s_prefix2[1024] = {0};

	if (s_prefix && s_prefix[0]) {
		// Remove ext ... Why?
		c_strlcpy (s_prefix2, s_prefix);
		File_URL_Edit_Remove_Extension (s_prefix2);
	}

	c_dpsnprintf1 (spattern, "%s*.tga", s_prefix2);

	fssearch_t	*t = FS_Search (spattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	int num_matches = 0;

	if (t && t->numfilenames > 0) {
		for (int idx = 0; idx < t->numfilenames; idx++) {
			char *sxy = t->filenames[idx];

			SPARTIAL_EVAL_

			num_matches ++;
		} // for
	} // if

	if (t) FS_FreeSearch(t);

	return num_matches;
}


#if 0 // NO CALLERS ANYMORE
int GetSoundList_Count (const char *s_prefix)
{
	char		s_prefix_no_ext[MAX_QPATHX2_256] = {0};

	// Remove extension before we add an extension in the pattern further down
	c_strlcpy (s_prefix_no_ext, s_prefix);
	File_URL_Edit_Remove_Extension (s_prefix_no_ext);

	stringlist_t list = {0};

	va_super(spattern1, MAX_QPATHX2_256, "%s*.wav", s_prefix_no_ext); stringlistappend_search_pattern (&list, spattern1);
	va_super(spattern2, MAX_QPATHX2_256, "%s*.ogg", s_prefix_no_ext); stringlistappend_search_pattern (&list, spattern2);

	stringlistsort (&list, fs_make_unique_true);

	int num_matches = 0;

	for (int idx = 0; idx < list.numstrings; idx++) {
		char *sxy = list.strings[idx];

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents (&list);
	return num_matches;
}
#endif

RELATED_ (R_ReplaceWorldTexture)

// Baker: July 31 2025 - This is like a list of textures that happen to be shaders.
int GetShaderList_Count (ccs *s_prefix)
{
	if (!r_refdef.scene.worldmodel || !cl.islocalgame || !cl.worldmodel) {
		return 0;
	}

	model_t		*m = r_refdef.scene.worldmodel;

	stringlist_t	matchedSet;
	stringlistinit  (&matchedSet); // this does not allocate

	texture_t	*tx = m->data_textures;

	// We cannot do comparisons here as this list is NOT SORTED
	for (int j = 0; j < m->num_textures; j ++, tx ++) {
		if (String_Starts_With_Caseless (tx->name, s_prefix) == false)
			continue;

		stringlistappend (&matchedSet, tx->name);
	} // for

	// SORT
	stringlistsort (&matchedSet, fs_make_unique_true);

	int			num_matches = 0;

	for (int idx = 0; idx < matchedSet.numstrings; idx ++) {
		char *sxy = matchedSet.strings[idx];

		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents (&matchedSet);

	return num_matches;
}


int GetTexWorld_Count (const char *s_prefix)
{
	if (!r_refdef.scene.worldmodel || !cl.islocalgame || !cl.worldmodel) {
		return 0;
	}

	model_t		*m = r_refdef.scene.worldmodel;

	stringlist_t	matchedSet;
	stringlistinit  (&matchedSet); // this does not allocate

	texture_t	*tx = m->data_textures;

	// We cannot do comparisons here as this list is NOT SORTED
	for (int j = 0; j < m->num_textures; j ++, tx ++) {
		if (String_Starts_With_Caseless (tx->name, s_prefix) == false)
			continue;

		stringlistappend (&matchedSet, tx->name);
	} // for

	// SORT
	stringlistsort (&matchedSet, fs_make_unique_true);

	int			num_matches = 0;

	for (int idx = 0; idx < matchedSet.numstrings; idx ++) {
		char *sxy = matchedSet.strings[idx];

		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents (&matchedSet);

	return num_matches;
}


// "folder" "dir" "ls" -- this completes in a weird way
//#pragma message ("Baker: KEEP ME - GetFolder_Aware_Count - This folder path completion autocomplete needs cleanup")

int GetFolder_Aware_Count (const char *s_prefix)
{
	ccs *prefix_lastcomponent_only = File_URL_SkipPath (s_prefix); // id1

	va_super (gamedir_no_slash, MAX_OSPATH_EX_1024, "%s", fs_gamedir);
	File_URL_Remove_Trailing_Unix_Slash (gamedir_no_slash);

	// Remove the trailing slash off gamedir and get the last path component like "c:\quak\id1" ==> "id1"
	const char *gamedir_last_component_no_slash = File_URL_SkipPath(gamedir_no_slash); // "id1"

	RELATED_ (Con_Folder_f)

	va_super (searchpath_we_want, MAX_OSPATH_EX_1024, "%s", s_prefix);
	//c_strlcpy  (searchpath_we_want, s_prefix);
	if (String_Ends_With (searchpath_we_want, "/") == false)
		File_URL_Edit_Reduce_To_Parent_Path_No_Trailing_Slash (searchpath_we_want);

	va_super (searchpattern, MAX_OSPATH_EX_1024, "%s/%s", gamedir_last_component_no_slash, searchpath_we_want);

	// gamedirname1

	if (String_Ends_With (searchpattern,  "/")==false) {
		c_strlcat  (searchpattern, "/");		// Directory to list
		c_strlcat  (searchpath_we_want, "/");
	}

	stringlist_t list = {0};
	stringlist_append_listdirectory	(&list, searchpattern /*fs_gamedir*/, fs_pattern_all_empty_string);

	// SORT
	stringlistsort_unique (&list);

	int num_matches = 0;

	char single_match_result[1024]; // For entering a directory

	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *s_this = list.strings[idx]; // "#a"

		if (String_Is_Dot(s_this) || String_Is_DotDot(s_this))
			continue; // ignore "." and ".." as filenames

		if (String_Starts_With_Caseless (s_this, prefix_lastcomponent_only) == false)
			continue;

		// "schloss"
		va_super (realpath, MAX_OSPATH_EX_1024, "%s%s%s", fs_gamedir, searchpath_we_want, s_this);

		//FS_FILETYPE_e ftype = FS_FileOrDirectoryType(s_this); // This seems to fail directories?
		qbool is_directory = FS_Directory_Exists_Unsafe(realpath); // This seems to fail directories?

		if (is_directory == false)
			continue; // Not a directory

		// Preserve the result to a variable
		// outside the for loop in case there is only one match
		c_dpsnprintf2 (single_match_result, "%s%s", searchpath_we_want, s_this); // COPY
		//c_strlcpy (single_match_result, searchdir_plus_sthis);
		//c_strlcat (single_match_result, s_this);

		char *sxy = single_match_result;
		File_URL_Edit_Remove_Extension (sxy);

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	autocomplete_t *ac = &_g_autocomplete;
	if (num_matches == 1) {
		// If only one match, we indicate the intention to help
		// the autocompletion "enter the folder"
		if (String_Match (s_prefix, ac->s_match_alphalast_a)) {
			// Completely replace search results
			freenull_ (ac->s_match_after_a)
			freenull_ (ac->s_match_alphalast_a);
			freenull_ (ac->s_match_alphatop_a);
			freenull_ (ac->s_match_before_a);

			c_strlcpy (single_match_result, s_prefix);
			c_strlcat (single_match_result, "/");

			char *sxy = single_match_result;
			SPARTIAL_EVAL_
		}
	}

	stringlistfreecontents(&list);

	return num_matches;
}

// s_path "sound/doors/" s_prefix "air" s_suffix ".wav"
RELATED_ (GetShowModelList_Count GetSoundList_Count) // May 28 2025 - Many callers
int CatList (stringlist_t *pmain_list, const char *s_path, const char *s_prefix, const char *s_suffix)
{
	// For our purposes, we need to skip the first path
	const char *s_subpath = String_Find_Skip_Past (s_path, "/");

	char		spattern[1024];
	char		s_prefix2[1024] = {0};

	if (s_prefix && s_prefix[0]) {
		// Remove ext ... Why?
		c_strlcpy (s_prefix2, s_prefix);
		File_URL_Edit_Remove_Extension (s_prefix2);
	}

	c_dpsnprintf3 (spattern, "%s%s%s"  /*"*.wav"*/, s_subpath, s_prefix2, s_suffix);

	fssearch_t	*t = FS_Search (spattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	int num_matches = 0;

	if (t && t->numfilenames > 0) {
		for (int idx = 0; idx < t->numfilenames; idx++) {
			// Baker: We are getting the fullpath like "progs/player.mdl"
			// However, for our purposes we need to reduce

			char *sxy = t->filenames[idx];
			const char *s_skippath = File_URL_SkipPath(sxy);
			stringlistappend (pmain_list, s_skippath);
			num_matches ++;
		} // for
	} // if

	if (t) FS_FreeSearch(t);

	return num_matches;
}


#if 0 // Baker: August 1 ... I want this dead
// "showmodel"
// Autocomplete of paths and or .md3 .obj .mdl .spr .bsp in path
// Outdated ... see "play"
// Baker: this should use stringlistappend_search_pattern to get rid of the ugly.  May 1 2025
int GetShowModelList_Count (const char *s_prefix)
{
	autocomplete_t *ac = &_g_autocomplete;

	stringlist_t list;

	char s_prefix_copy[1024] ;
	char gamepathos[1024] ;
	char s_prefix_filename_only[1024] ;
	char sthisy[1024] ;

	const char *safterpath = File_URL_SkipPath (s_prefix);

	// Baker: May 1 2025 - All this ugly digging around is because we have to search in the real
	// file system.
#if 1

		va_super (sgdwork, MAX_OSPATH_EX_1024, "%s", fs_gamedir); // fs_gamedir = "c:\galaxy/zircon/"
		File_URL_Remove_Trailing_Unix_Slash (sgdwork); // sgdwork becomes "c:\galaxy/zircon"
		const char *slastcom = File_URL_SkipPath(sgdwork); // slastcom becomes "zircon"
		va_super (sgamedirlast, MAX_OSPATH_EX_1024, "%s", slastcom); // sgamedirlast is "zircon"
		File_URL_Remove_Trailing_Unix_Slash (sgamedirlast); // does nothing
		//sgamedirlast = "zircon"
//		int j = 5;
#else

		char sgdwork[1024];
		c_strlcpy (sgdwork, fs_gamedir); // "id1/"
		File_URL_Remove_Trailing_Unix_Slash (sgdwork);

		const char *slastcom = File_URL_SkipPath(sgdwork);
		char sgamedirlast[1024];
		c_strlcpy (sgamedirlast, slastcom);
		File_URL_Remove_Trailing_Unix_Slash (sgamedirlast);
		// sgamedirlast is like "id1" or "travail" or whatever
#endif

	RELATED_ (Con_Folder_f)

	// What is dir?
	c_strlcpy  (s_prefix_copy, s_prefix);
	if (String_Ends_With (s_prefix_copy, "/") == false)
		File_URL_Edit_Reduce_To_Parent_Path_Trailing_Slash (s_prefix_copy);
	c_strlcpy  (s_prefix_filename_only, safterpath);

	c_strlcpy  (gamepathos, sgamedirlast); // "id1"
	c_strlcat  (gamepathos, "/");
	// fs_gamedir "C:\Users\Main\Documents/My Games/zircon/id1/"
	// fs_gamedir "id1/"
	// gamedirname1
	if (s_prefix_copy[0]) // s_prefix_copy is "models"
		c_strlcat  (gamepathos, s_prefix_copy); // = "zircon/models" for zircon/models/med

	if (s_prefix_copy[0] && String_Ends_With (gamepathos,  "/")==false) {
		c_strlcat  (gamepathos, "/");		// Directory to list
		c_strlcat  (s_prefix_copy, "/"); // s_prefix_copy becomes "models/"
	}

	stringlistinit	(&list);	

#ifdef _DEBUG
	ccs *x_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1;
#endif
	//va_super (pathic, MAX_OSPATH_EX_1024, "%s/", x_gamedir);

	// Baker: May 1 2025 - gamepathos is relative to fs_basedir (!!!) so actually starts with game directory

	stringlist_append_listdirectory	(&list, gamepathos, fs_pattern_all_empty_string);

	CatList (&list, gamepathos, s_prefix_filename_only, "*.mdl");
	CatList (&list, gamepathos, s_prefix_filename_only, "*.md3");
	CatList (&list, gamepathos, s_prefix_filename_only, "*.spr");
	CatList (&list, gamepathos, s_prefix_filename_only, "*.obj");

	// SORT
	stringlistsort (&list, /*uniq*/ fs_make_unique_true);

	int num_matches = 0;

	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *s_this = list.strings[idx];

		if (String_Is_Dot(s_this) || String_Is_DotDot(s_this))
			continue; // ignore "." and ".." as filenames

		if (String_Starts_With_Caseless (s_this, s_prefix_filename_only) == false)
			continue;

		if (String_Ends_With_Caseless (s_this, ".mdl") ||
			String_Ends_With_Caseless (s_this, ".md3") ||
			String_Ends_With_Caseless (s_this, ".spr") ||
			String_Ends_With_Caseless (s_this, ".obj") ||
			String_Contains (s_this, ".") == false)
		{
			// Stay
		} else {
			continue; // Disqualified
		}

		// Preserve the result to a variable
		// outside the for loop in case there is only one match
		c_strlcpy (sthisy, s_prefix_copy);
		c_strlcat (sthisy, s_this);

		char *sxy = sthisy;
		//File_URL_Edit_Remove_Extension (sxy);

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	// Baker: Don't offer a trailing slash if there is a dot in it, probably a .mdl or other file
	if (num_matches == 1 && String_Contains(ac->s_match_alphalast_a, ".") == false) {
		// If only one match, we indicate the intention to help
		// the autocompletion "enter the folder"
		if (String_Match (s_prefix, ac->s_match_alphalast_a)) {
			// Completely replace search results
			freenull_ (ac->s_match_after_a)
			freenull_ (ac->s_match_alphalast_a);
			freenull_ (ac->s_match_alphatop_a);
			freenull_ (ac->s_match_before_a);

			c_strlcpy (sthisy, s_prefix);
			c_strlcat (sthisy, "/");

			char *sxy = sthisy;
			SPARTIAL_EVAL_
		}
	}

	stringlistfreecontents(&list);

	return num_matches;
}
#endif

//int GetAnimatedGIF_Count (const char *s_prefix)
//{
//	autocomplete_t *ac = &_g_autocomplete;
//
//	stringlist_t list;
//
//	char s_prefix_copy[1024] ;
//	char gamepathos[1024] ;
//	char s_prefix_filename_only[1024] ;
//	char sthisy[1024] ;
//
//	const char *safterpath = File_URL_SkipPath (s_prefix);
//		char sgdwork[1024];
//		c_strlcpy (sgdwork, fs_gamedir);
//		File_URL_Remove_Trailing_Unix_Slash (sgdwork);
//
//		const char *slastcom = File_URL_SkipPath(sgdwork);
//		char sgamedirlast[1024];
//		c_strlcpy (sgamedirlast, slastcom);
//		File_URL_Remove_Trailing_Unix_Slash (sgamedirlast);
//		// sgamedirlast is like "id1" or "travail" or whatever
//
//	RELATED_ (Con_Folder_f)
//
//	// What is dir?
//	c_strlcpy  (s_prefix_copy, s_prefix);
//	if (String_Ends_With (s_prefix_copy, "/") == false)
//		File_URL_Edit_Reduce_To_Parent_Path_Trailing_Slash (s_prefix_copy);
//	c_strlcpy  (s_prefix_filename_only, safterpath);
//
//	c_strlcpy  (gamepathos, sgamedirlast); // "id1"
//	c_strlcat  (gamepathos, "/");
//	// fs_gamedir "C:\Users\Main\Documents/My Games/zircon/id1/"
//	// fs_gamedir "id1/"
//	// gamedirname1
//	if (s_prefix_copy[0])
//		c_strlcat  (gamepathos, s_prefix_copy);
//
//	if (s_prefix_copy[0] && String_Ends_With (gamepathos,  "/")==false) {
//		c_strlcat  (gamepathos, "/");		// Directory to list
//		c_strlcat  (s_prefix_copy, "/");
//	}
//
//	stringlistinit	(&list);
//	// game pathos tends to be "id1/" here
//	// game pathos tends to be "id1/progs/"
//	stringlist_append_listdirectory	(&list, gamepathos /*fs_gamedir*/, fs_pattern_all_empty_string);
//
//	CatList (&list, gamepathos, s_prefix_filename_only, "*.tga");
//	CatList (&list, gamepathos, s_prefix_filename_only, "*.jpg");
//	CatList (&list, gamepathos, s_prefix_filename_only, "*.png");
//
//	// SORT
//	stringlistsort (&list, /*uniq*/ fs_make_unique_true);
//
//	int num_matches = 0;
//
//	for (int idx = 0; idx < list.numstrings; idx ++) {
//		char *s_this = list.strings[idx];
//
//		if (String_Is_Dot(s_this) || String_Is_DotDot(s_this))
//			continue; // ignore "." and ".." as filenames
//
//		if (String_Starts_With_Caseless (s_this, s_prefix_filename_only) == false)
//			continue;
//
//		if (String_Ends_With_Caseless (s_this, ".gif") ||
//			String_Contains (s_this, ".") == false)
//		{
//			// Stay
//		} else {
//			continue; // Disqualified
//		}
//
//		// Preserve the result to a variable
//		// outside the for loop in case there is only one match
//		c_strlcpy (sthisy, s_prefix_copy);
//		c_strlcat (sthisy, s_this);
//
//		char *sxy = sthisy;
//		//File_URL_Edit_Remove_Extension (sxy);
//
//		SPARTIAL_EVAL_
//
//		num_matches ++;
//	} // for
//
//	// Baker: Don't offer a trailing slash if there is a dot in it, probably a .mdl or other file
//	if (num_matches == 1 && String_Contains(ac->s_match_alphalast_a, ".") == false) {
//		// If only one match, we indicate the intention to help
//		// the autocompletion "enter the folder"
//		if (String_Match (s_prefix, ac->s_match_alphalast_a)) {
//			// Completely replace search results
//			freenull_ (ac->s_match_after_a)
//			freenull_ (ac->s_match_alphalast_a);
//			freenull_ (ac->s_match_alphatop_a);
//			freenull_ (ac->s_match_before_a);
//
//			c_strlcpy (sthisy, s_prefix);
//			c_strlcat (sthisy, "/");
//
//			char *sxy = sthisy;
//			SPARTIAL_EVAL_
//		}
//	}
//
//	stringlistfreecontents(&list);
//
//	return num_matches;
//}


int GetGameCommands_Count (const char *s_prefix, ccs *s_gamecommands_string)
{
	// This process depends on this s_gamecommands_string having items.
	if (s_gamecommands_string[0] == NULL_CHAR_0)
		return 0;


#if 1
	stringlist_t matchedSet = {0};
	char *s_za = Z_StrDup (s_gamecommands_string);
	String_Edit_Trim (s_za);
	//ccs *s_dbl = NULL;

	while (strstr(s_za, " " " ")) {
		size_t slenp1 = strlen(s_za) + ONE_CHAR_1;
		String_Edit_Replace (s_za, slenp1, "  ", " "); // Replace 2x spaces with 1x space.
	}

	stringlistappend_split (&matchedSet, s_za, " ");
	Mem_FreeNull_ (s_za);

#endif


#if 0
	stringlist_t matchedSet;

	stringlistinit	(&matchedSet); // this does not allocate, memset 0
	const char	*s_space_delim		= " ";
	int			s_len			= (int)strlen(s_gamecommands_string);
	int			s_delim_len		= (int)strlen(s_space_delim);

	// Baker: This works the searchpos against s_gamecommands_string
	// finding the delimiter (space) and adding a list item until there are no more spaces
	// (an iteration with no space adds the rest of the string.

	// Baker: have we tested this against a single item without a space to see what happens?
	// It looks like it can handle that.

	// BUILD LIST

	int			searchpos		= 0;
	while (1) {
		char s_this_copy[MAX_INPUTLINE_16384];
		const char	*space_pos	= strstr (&s_gamecommands_string[searchpos], s_space_delim); // string_find_pos_start_at(s, s_delim, searchpos);
		int			endpos		= (space_pos == NULL) ? (s_len - 1) : ( (space_pos - s_gamecommands_string) - 1); // (commapos == not_found_neg1) ? (s_len -1) : (commapos -1);
		int			this_w		= (endpos - searchpos + 1); // string_range_width (searchpos, endpos); (endpos - startpos + 1)

		memcpy (s_this_copy, &s_gamecommands_string[searchpos], this_w);
		s_this_copy[this_w] = NULL_CHAR_0; // term

		stringlistappend (&matchedSet, s_this_copy);

		// If no space found, we added the rest of the string as an item, so get out!
		if (space_pos == NULL)
			break;

		searchpos = (space_pos - s_gamecommands_string) + s_delim_len;
	} // while
#endif


	// SORT plus unique-ify
	stringlistsort (&matchedSet, fs_make_unique_true);

	int num_matches = 0;

	for (int idx = 0; idx < matchedSet.numstrings; idx ++) {
		char *sxy = matchedSet.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents( &matchedSet );

	return num_matches;
}

int GetModelPrecacheList_Count (const char *s_prefix)
{
	if (!r_refdef.scene.worldmodel)
		return 0;

	stringlist_t list = {0};

	// Baker: Why 1?
	for (int j = 1; j < MAX_MODELS_8192 && cl.model_name[j][0]; j ++) {
		stringlistappend (&list, cl.model_name[j]);
	} // for

	// Sort

	stringlistsort (&list, fs_make_unique_true);

	int			num_matches = 0;

	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *sxy =  list.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false) {
			continue;
		}

		SPARTIAL_EVAL_

		num_matches ++;

	} // idx

	stringlistfreecontents (&list);

	return num_matches;
}

RELATED_ (PRVM_ED_EdictGet_Fly)

void KeyBind_Fly (ccs *keystring, char *valuebuf, size_t valuebuf_size)
{
	int b = Key_StringToKeynum (keystring);

	if (b == -1 || b >= MAX_KEYS_44032) {
		dp_strlcpy (valuebuf, "*isn't a valid key*", valuebuf_size);
		return;
	}

	if (!keybindings[0][b]) {
		dp_strlcpy (valuebuf, "*is not bound*", valuebuf_size);
		return;
	}

	// Baker: Not sure if dpsnprintf is secure against self-reference.
	char buf1[2048];
	char buf2[2048];

	c_strlcpy (buf1, keybindings[0][b]);

	if (String_Has_Whitespace(buf1)) {
		c_dpsnprintf1 (buf2, QUOTED_S, buf1);
	} else {
		c_strlcpy (buf2, buf1);
	}

	dp_strlcpy (valuebuf, buf2, valuebuf_size);

	return;
}

