// cl_screen_utils.c.h

byte *Jpeg_From_BGRA_Temppool_Alloc (const bgra4 *bgrapels_in, int width, int height, /*reply*/ size_t *size_out);
unsigned char *Screenshot_Jpeg_Temppool_Alloc (int x, int y, int width, int height, /*reply*/ size_t *size_out);


int Screenshot_Jpeg_Prep_RenderBuffer_Is_Ok (int width, int height)
{
	RELATED_ (R_Envmap_f)
	int size_x = width;
	int size_y = height;

	if (size_x > vid.width || size_y > vid.height) {
		Con_PrintLinef ("Your resolution is not big enough to render %d x %d", size_x, size_y);
		return false;
	}

	r_refdef.envmap = true;

	R_UpdateVariables();

	r_refdef.view.x = 0;					r_refdef.view.y = 0;				r_refdef.view.z = 0;
	r_refdef.view.width = size_x;			r_refdef.view.height = size_y;		r_refdef.view.depth = 1;
	r_refdef.view.useperspective = true;	r_refdef.view.isoverlay = false;	r_refdef.view.ismain = true;

	r_refdef.view.frustum_x = 1; // tan(45 * M_PI / 180.0);
	r_refdef.view.frustum_y = 1; // tan(45 * M_PI / 180.0);

#if 1
	r_refdef.view.frustum_y = tan(90 /*scr_fov.value*/ * M_PI / 360.0) * (3.0 / 4.0) * 1 /*cl.viewzoom*/;
	r_refdef.view.frustum_x = r_refdef.view.frustum_y * (float)r_refdef.view.width / (float)r_refdef.view.height / vid_pixelheight.value;

	r_refdef.view.frustum_x *= r_refdef.frustumscale_x; // Baker: frustumscale_x / y is always 1 as far as I can tell
	r_refdef.view.frustum_y *= r_refdef.frustumscale_y; //        except underwater where DarkPlaces warps it.
	r_refdef.view.ortho_x = atan(r_refdef.view.frustum_x) * (360.0 / M_PI); // abused as angle by VM_CL_R_SetView
	r_refdef.view.ortho_y = atan(r_refdef.view.frustum_y) * (360.0 / M_PI); // abused as angle by VM_CL_R_SetView
#endif

	r_refdef.view.ortho_x = 90; // abused as angle by VM_CL_R_SetView
	r_refdef.view.ortho_y = 90; // abused as angle by VM_CL_R_SetView

	CL_UpdateEntityShading();

	r_refdef_view_t oldview = r_refdef.view;
	r_refdef.view.width = size_x, r_refdef.view.height = size_y;

	Matrix4x4_CreateFromQuakeEntity (
		&r_refdef.view.matrix,
		r_refdef.view.origin[0],
		r_refdef.view.origin[1],
		r_refdef.view.origin[2],

		cl.viewangles[0], // envmapinfo[j].angles[0],
		cl.viewangles[1], // envmapinfo[j].angles[1],
		cl.viewangles[2], // envmapinfo[j].angles[2],
		/*scale*/ 1
	);
	r_refdef.view.quality = 1;
	r_refdef.view.clear = true;

	R_Mesh_Start();
	R_RenderView(0, NULL, NULL, r_refdef.view.x, r_refdef.view.y, r_refdef.view.width, r_refdef.view.height);
	R_Mesh_Finish();

	r_refdef.view = oldview;

	r_refdef.envmap = false;

	return true; // Ok!
}

byte *Screenshot_To_Jpeg_Blob_ZAlloc_512_320 (size_t *pblobsizeout) //
{
	int width = SAVEGAME_PIC_WIDTH_512;
	int height = SAVEGAME_PIC_HEIGHT_320;
	RELATED_ (SCR_ScreenShot_f  R_Envmap_f SCR_ScreenShot)

	int is_ok = Screenshot_Jpeg_Prep_RenderBuffer_Is_Ok (width, height);
	if (is_ok == false) {
		return NULL;
	}

	int x = 0;
	int y = vid.height - (r_refdef.view.y + r_refdef.view.height);

	byte *data_allocz = Screenshot_Jpeg_Temppool_Alloc(x, y, width, height, pblobsizeout);
	return data_allocz;
}

RELATED_ (Screenshot_To_Jpeg_Blob_ZAlloc_512_320)
char *Screenshot_To_Jpeg_String_Malloc_512_320 (void) //
{
	int width = SAVEGAME_PIC_WIDTH_512;
	int height = SAVEGAME_PIC_HEIGHT_320;
	RELATED_ (SCR_ScreenShot_f  R_Envmap_f SCR_ScreenShot)

	int is_ok = Screenshot_Jpeg_Prep_RenderBuffer_Is_Ok (width, height);
	if (is_ok == false) {
		return NULL;
	}

	int x = 0;
	int y = vid.height - (r_refdef.view.y + r_refdef.view.height);

	size_t data_size;
	unsigned char *data_allocz = Screenshot_Jpeg_Temppool_Alloc(x, y, width, height, &data_size);

	char *s_base64_alloc = base64_encode_calloc (data_allocz, data_size, q_reply_len_NULL); // malloc
	Mem_Free (data_allocz);
	return s_base64_alloc; // free (s_base64_alloc);
}


RELATED_ (Screenshot_To_Jpeg_String_Malloc_512_320)

// Extracts a screenshot from a Zircon save.
char *SCR_Screenshot_Get_JPEG_String_From_Save_File_Alloc (const char *s_savegame_with_sav_ext)
{
	char *s_jpeg_string_alloc = NULL;

	fs_offset_t filesize;
	unsigned char *filedata = FS_LoadFile (s_savegame_with_sav_ext, tempmempool, fs_quiet_true, &filesize);

	//Sys_PrintfToTerminal (va32("SCR_Screenshot_Get_JPEG_String_From_Save_File_Alloc " QUOTED_S, s_savegame_with_sav_ext));
	if (filedata == NULL) {
		return NULL;
	}

	//int is_ok = true;
	const char *text = (const char *)filedata;

	int jpeg_slen_int = -1;
	const char *s = strstr (text, "sv.screenshot "); // sv.screenshot [bytes] [string]

	if (!s) {
		Con_PrintLinef ("This save contains no sv.screenshot");
		//is_ok = false;
		goto cleanup;
	}


	text = s;
	COM_Parse_Basic (&text); // sv.screenshot
	COM_Parse_Basic (&text); jpeg_slen_int = atoi(com_token);

	if (false == in_range_beyond (2, jpeg_slen_int, SAVEGAME_JPEG_MAXSIZE_STRING_SIZE)) {
		// Rejected
		goto cleanup;
	}

	// Sys_PrintfToTerminal (va32("jpeg len = %d" NEWLINE, jpeg_slen_int));

	 // Cursor is on space for base64 blob
	s = text;
	while (*s && ISWHITESPACE(*s))
		s ++;

	s_jpeg_string_alloc = (char *)Z_MemDup_Z (s, (size_t)jpeg_slen_int);

cleanup:
	Mem_Free(filedata);
	return s_jpeg_string_alloc;
}



// Baker: image_width, image_height globals for size
bgra4 *SCR_Screenshot_Get_JPEG_BGRA4_From_Save_File_ZAlloc (ccs *s_savegame_name)
{
	char *s_jpeg_string_alloc = SCR_Screenshot_Get_JPEG_String_From_Save_File_Alloc(s_savegame_name);

	if (!s_jpeg_string_alloc)
		return NULL;

	bgra4 *jpeg_bgra_data_zalloc = Jpeg_Base64_BGRA_Decode_ZAlloc (s_jpeg_string_alloc);

	return jpeg_bgra_data_zalloc;
}



void SCR_jpegextract_from_savegame_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) == 1) {
		Con_PrintLinef ("Usage: %s [save file] or %s [save file] string  -- extracts screenshot and puts image on clipboard",
                  Cmd_Argv(cmd, 0) , Cmd_Argv(cmd, 0) );
		return;
	}

	qbool is_string_output = Cmd_Argc(cmd) == 3 && String_Match (Cmd_Argv(cmd, 2), "string");

	ccs *s_filename = Cmd_Argv(cmd, 1);
	char savename[MAX_QPATH_128];
	c_strlcpy (savename, s_filename);

	FS_DefaultExtension (savename, ".sav", sizeof(savename));

	switch (is_string_output) {
	case true:
		{
			char *s_zalloc = SCR_Screenshot_Get_JPEG_String_From_Save_File_Alloc (savename);
			if (!s_zalloc) {
				Con_PrintLinef ("Couldn't obtain screenshot from save " QUOTED_S, savename);
				return;
			}
			int jpeg_slen = (int)strlen(s_zalloc);
			Clipboard_Set_Text (s_zalloc); // jpegextractfromsavegame
			Mem_FreeNull_ (s_zalloc);
			Con_PrintLinef ("Jpeg string copied to clipboard, strlen = %d", jpeg_slen);
		}

		break;

	default:
		{
			bgra4 *jpeg_bgra_zalloc = SCR_Screenshot_Get_JPEG_BGRA4_From_Save_File_ZAlloc(savename);

			if (!jpeg_bgra_zalloc) {
				Con_PrintLinef ("Couldn't obtain screenshot from save " QUOTED_S, savename);
				return;
			}

			Sys_Clipboard_Set_Image_BGRA_Is_Ok (jpeg_bgra_zalloc, image_width, image_height);
			Con_PrintLinef ("Image size is %d x %d", image_width, image_height);
			Mem_FreeNull_ (jpeg_bgra_zalloc);

			Con_PrintLinef ("Jpeg image copied to clipboard");

			break;
		} // if
	} // sw


}

#ifdef CONFIG_MENU


void SCR_documentation_dump_f (cmd_state_t *cmd)
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "go", "writes documentation_*.txt for cvars/QC tab delimited; type " QUOTED_STR("folder") " after running" );

	// Do cvars
	stringlist_t list = {0};
	baker_string_t *bsa = NULL;
	ccs *filename;

	filename = "documentation_cvars.txt";
		Cvar_List_Append_Tabbed_Exclude_UnderscoreNames (&list, /*exclude leading underscore names*/ true);
		bsa = stringlist_join_lines_bkalloc(&list);
		qbool isok = FS_WriteFileStringQuick (filename, bsa->string);
		Con_PrintLinef ("%s %s", isok ? "Wrote" : "Couldn't write", filename);

		BakerString_Destroy_And_Null_It (&bsa);
		stringlistfreecontents (&list);


}

void SCR_jpegdecodeclipstring_f (cmd_state_t *cmd)
{
	bgra4 *jpegpels = NULL;
	char *s_base64allocz = Clipboard_Get_Text_ZAlloc();
	if (!s_base64allocz || !*s_base64allocz) {
		Con_PrintLinef ("No string on clipboard");
		goto cleanup;
	}

	jpegpels = Jpeg_Base64_BGRA_Decode_ZAlloc (s_base64allocz);

	if (jpegpels) {
		Con_PrintLinef ("Decoded JPEG string %d x %d", image_width, image_height);

		Con_PrintLinef ("Copying to clipboard");
		qbool is_ok = Sys_Clipboard_Set_Image_BGRA_Is_Ok (jpegpels, image_width, image_height);
		Con_PrintLinef ("JPEG to clipboard = %s", is_ok ? "Ok!" : "Failed!");
		if (!is_ok) {
			Con_DPrintLinef ("Couldn't set clipboard image");
		}
	}

cleanup:
	Mem_FreeNull_ (jpegpels);
	Mem_FreeNull_ (s_base64allocz);
}
#endif

bgra4 *JPEG_Decode_String_Zalloc (const char *s_base64jpeg, int *pwidth, int *pheight)
{
	bgra4 *jpegpels_zalloc = Jpeg_Base64_BGRA_Decode_ZAlloc(s_base64jpeg);

	if (!jpegpels_zalloc) {
		Con_PrintLinef ("Could not decode JPEG base64 string");
		return NULL;
	}

	*pwidth = image_width;
	*pheight = image_height;

	return jpegpels_zalloc;
}

#ifdef CONFIG_MENU
void SCR_clipimagetobase64string_f (cmd_state_t *cmd)
{
	int width, height;
	bgra4 *bgrapels_zalloc = Sys_Clipboard_Get_Image_BGRA_ZAlloc (&width, &height);
	if (!bgrapels_zalloc) {
		Con_PrintLinef ("Cannot get clipboard image");
	}

	// Turn it to jpeg.
	size_t data_size;
	unsigned char *data_zalloc = Jpeg_From_BGRA_Temppool_Alloc(bgrapels_zalloc, width, height, &data_size);

	char *s_base64_calloc = base64_encode_calloc (data_zalloc, data_size, q_reply_len_NULL); // malloc
	Mem_FreeNull_ (data_zalloc);

	Clipboard_Set_Text (s_base64_calloc); // clipimagetobase64
	Con_PrintLinef ("jpeg string from clipboard to clipboard strlen = %d", (int)strlen(s_base64_calloc));

	freenull_ (s_base64_calloc);

	Mem_FreeNull_ (bgrapels_zalloc);
}

// cliptraceoutline

void SCR_opendialog_f (cmd_state_t *cmd)
{
	char *wtf8name_za = FS_OpenDialog_Unsafe_ZAlloc ();

	if (wtf8name_za == NULL) {
		Con_PrintLinef ("Dialog aborted");
		return;
	}

	Con_PrintLinef ("Result = %s", wtf8name_za);

	// Now print the contents
	char *text_za = (char *)FS_LoadFileString_Unsafe_ZAlloc(wtf8name_za);

	if (!text_za) {
		Con_PrintLinef ("Couldn't open %s", wtf8name_za);
	} else {
		stringlist_t list = {0};
		stringlistappend_split_lines_cr_scrub (&list, text_za);
		stringlist_condump_with_linenums (&list);
		stringlistfreecontents (&list);
	}

	Mem_FreeNull_ (text_za);
	Mem_FreeNull_ (wtf8name_za);

}

void SCR_cliptraceoutline_f(cmd_state_t *cmd)
{
	int width, height;

	unsigned int *img_za = Sys_Clipboard_Get_Image_BGRA_ZAlloc(&width, &height);

	if (!img_za) {
		Con_PrintLinef ("No image on clipboard");
		return;
	}

	unsigned int *from_top_za = (bgra4 *)Z_Malloc (width * height * BGRA_4);
	unsigned int *from_left_za = (bgra4 *)Z_Malloc (width * height * BGRA_4);
	unsigned int *from_right_za = (bgra4 *)Z_Malloc (width * height * BGRA_4);
	unsigned int *from_bottom_za = (bgra4 *)Z_Malloc (width * height * BGRA_4);
	unsigned int *combined_za = (bgra4 *)Z_Malloc (width * height * BGRA_4);

	Con_PrintLinef ("Size is %d x %d", width, height);

top:
	for (int row = 0; row < height; row ++) {
		for (int col = 0; col < width; col ++) {
			unsigned int val = img_za[row * width + col];
			unsigned int dst = 0;
			if (!val)
				continue; // We have no color so not an edge.

			// We have color
			if (row == 0) {
				// We have color + nothing above us
				dst = COLOR_RGBA (255, 255, 255, 255);
			}
			else {
				unsigned int val_up = img_za[(row - 1) * width + col];
				// We have no color above us therefore we are a top edge.
				if (val_up == 0) {
					// We have color + no color in row above us.
					dst = COLOR_RGBA (255, 255, 255, 255);
				}
			}
			if (dst)
				from_top_za[row * width + col] = dst;
			//else
			//	from_top_za[row * width + col] = COLOR_RGBA (255, 0, 255, 255);
		} // col
	} // row

	Sys_Clipboard_Set_Image_BGRA_Is_Ok (from_top_za, width, height);

//	int j = 5;

left:
	for (int row = 0; row < height; row ++) {
		for (int col = 0; col < width; col ++) {
			unsigned int val = img_za[row * width + col];
			unsigned int dst = 0;
			if (!val)
				continue; // We have no color so not an edge.

			// We have color
			if (col == 0) {
				// We have color + nothing left of us
				dst = COLOR_RGBA (255, 255, 255, 255);
			}
			else {
				unsigned int val_left = img_za[row * width + (col - 1)];
				// We have no color above us therefore we are a top edge.
				if (val_left == 0) {
					// We have color + no color in row above us.
					dst = COLOR_RGBA (255, 255, 255, 255);
				}
			}
			if (dst)
				from_left_za[row * width + col] = dst;
		} // col
	} // row

bottom:
	for (int row = 0; row < height; row ++) {
		for (int col = 0; col < width; col ++) {
			unsigned int val = img_za[row * width + col];
			unsigned int dst = 0;
			if (!val)
				continue; // We have no color so not an edge.

			// We have color
			if (row == height - 1) {
				// We have color + nothing below us
				dst = COLOR_RGBA (255, 255, 255, 255);
			}
			else {
				unsigned int val_down = img_za[(row + 1) * width + col];
				// We have no color below us therefore we are a bottom edge.
				if (val_down == 0) {
					// We have color + no color in row above us.
					dst = COLOR_RGBA (255, 255, 255, 255);
				}
			}
			if (dst)
				from_bottom_za[row * width + col] = dst;
		} // col
	} // row

	Sys_Clipboard_Set_Image_BGRA_Is_Ok (from_bottom_za, width, height);

right:
	for (int row = 0; row < height; row ++) {
		for (int col = 0; col < width; col ++) {
			unsigned int val = img_za[row * width + col];
			unsigned int dst = 0;
			if (!val)
				continue; // We have no color so not an edge.

			// We have color
			if (col == width - 1) {
				// We have color + nothing right of us
				dst = COLOR_RGBA (255, 255, 255, 255);
			}
			else {
				unsigned int val_right = img_za[row * width + (col + 1)];
				// We have no color above us therefore we are a top edge.
				if (val_right == 0) {
					// We have color + no color in row above us.
					dst = COLOR_RGBA (255, 255, 255, 255);
				}
			}
			if (dst)
				from_right_za[row * width + col] = dst;
		} // col
	} // row

	for (int row = 0; row < height; row ++) {
		for (int col = 0; col < width; col ++) {

			unsigned int vup = from_top_za[row * width + col];
			unsigned int vdn = from_bottom_za[row * width + col];
			unsigned int vleft = from_left_za[row * width + col];
			unsigned int vright = from_right_za[row * width + col];


			if (vup || vdn || vleft || vright) {
				combined_za[row * width + col] = COLOR_RGBA (255, 255, 255, 255);
			}
		} // col
	} // row

	Sys_Clipboard_Set_Image_BGRA_Is_Ok (combined_za, width, height);


	Mem_FreeNull_ (from_bottom_za);
	Mem_FreeNull_ (from_top_za);
	Mem_FreeNull_ (from_left_za);
	Mem_FreeNull_ (from_right_za);
	Mem_FreeNull_ (combined_za);

	Mem_FreeNull_ (img_za);
}

void SCR_jpegshotclip_f (cmd_state_t *cmd)
{
	int is_command_ok = (cls.state == ca_connected && cls.signon == SIGNONS_4 && cl.worldmodel);

	if (!is_command_ok) {
		Con_PrintLinef ("Not running a map");
		return;
	}

	char *s_base64_alloc = Screenshot_To_Jpeg_String_Malloc_512_320 ();
	//char *s_base64_alloc = Screenshot_Jpeg_String_Malloc ();

	if (s_base64_alloc == NULL) {
		Con_PrintLinef ("Failed!");
		return;
	}

	Clipboard_Set_Text (s_base64_alloc); // cmd jpegshotclip

	Con_PrintLinef ("jpeg screenshot text set to clipboard");

	free (s_base64_alloc);
}
#endif

void SCR_ToolMarker_f (cmd_state_t *cmd)
{
	qbool should_print_more = true; // Silence too much printing
	//             1      2
	// tool_marker 69
	// tool_marker client 69
	// tool_marker 1      2   3
	ccs *arg1 = Cmd_Argv (cmd, 1);
	ccs *arg2 = Cmd_Argv (cmd, 2);
	//ccs *arg3 = Cmd_Argv (cmd, 3);
	char s_args_all[1024];

	prvm_prog_t *prog = NULL;
	int edictnum = 0;

	qbool is_silent_server = cmd_argc == (1 + 2) && String_Match (arg2, "silent");

	if (Cmd_Argc(cmd) == 1) {

		Con_PrintLinef ("tool_marker is at: " VECTOR3_G, VECTOR3_SEND(cl.tool_marker_origin) );
		Con_PrintLineEmpty ();
		Con_PrintLinef ("usage examples:");
		Con_PrintLinef ("tool_marker 55 // set origin to server #edict 55 origin");
		Con_PrintLinef ("tool_marker 55 silent // set origin to server #edict 55 origin no print");
		Con_PrintLinef ("tool_marker client 55 // set origin to client #edict 55 origin");
		Con_PrintLinef ("tool_marker 0 // Turn tool marker off");
		Con_PrintLinef ("tool_marker 506 220.2 12.4 // Set to this origin");
		Con_PrintLineEmpty ();

		return;
	}

	if (Cmd_Argc(cmd) == 2 && atof(arg1) == 0) {

		if (cl.tool_marker_on) {
			cl.tool_marker_on = false;
			Con_PrintLinef ("Marker turned off");
		}

		return;
	}

	// tool_marker client 3 // csedict 3
	if (Cmd_Argc(cmd) == 3 && String_Match (arg1, "client") && !is_silent_server) {
		prog = PRVM_FriendlyProgFromString("client");
		edictnum = atoi(arg2);
		Con_PrintLinef ("Marker from client entity #%d", edictnum);
		should_print_more = false; // Said enough
		goto from_prog;
	}

	if (Cmd_Argc(cmd) == 2 || is_silent_server) {
		prog = PRVM_FriendlyProgFromString("server");
		edictnum = atoi(arg1);
		if (!is_silent_server)
			Con_PrintLinef ("Marker from server entity #%d (tool_marker 0 = off)", edictnum);
		should_print_more = false; // Said enough
		goto from_prog;
	}

	// Accumulate the args
	Argv_Cumulate (cmd, 1, s_args_all, sizeof(s_args_all) );
	goto from_string;

from_prog:
	RELATED_ (PRVM_ED_EdictGet_f PRVM_ED_Eset_f)
	// detect if client not running

	if (!prog) {
		// It already will have printed a message.
		return;
	}

	prvm_edict_t	*ed_1	= PRVM_EDICT_NUM(edictnum);
	mdef_t			*key_2	= PRVM_ED_FindField(prog, "origin");

	char s_absmin[1024];
	char s_absmax[1024];
	PRVM_ED_EdictGet_Fly_Lite (SVVM_prog, edictnum, "absmin", s_absmin, sizeof(s_absmin));
	PRVM_ED_EdictGet_Fly_Lite (SVVM_prog, edictnum, "absmax", s_absmax, sizeof(s_absmax));
	vec3_t vabsmin; Math_atov3 (s_absmin, vabsmin);
	vec3_t vabsmax; Math_atov3 (s_absmax, vabsmax);
	if (Vector3_IsZeros (vabsmin))
		goto hard_old_way;
	vec3_t ctr; VectorAdd (vabsmin, vabsmax, ctr);
	VectorScale (ctr, 0.5, ctr);
	VectorCopyDestSrc (cl.tool_marker_origin, ctr);
	cl.tool_marker_on = true;
	if (should_print_more)
		Con_PrintLinef ("Tool marker set to " VECTOR3_5d1F, VECTOR3_SEND(cl.tool_marker_origin) );
	return;

hard_old_way:
    ; // oblig

	char valuebuf[MAX_INPUTLINE_16384];
	prvm_eval_t *v	= (prvm_eval_t *)(ed_1->fields.fp + key_2->ofs);
	const char	*s	= PRVM_UglyValueString(prog, (etype_t)key_2->type, v, valuebuf, sizeof(valuebuf));

	if (should_print_more) // Said enough
		Con_PrintLinef ("%s", s);
	c_strlcpy (s_args_all, s);

	// Do we need to dequote?

from_string:
    ; // Needed or gcc compile fails with a dumb reason about labels and declarations. Dec 15 2024
	vec3_t org = {0,0,0};
	const char *t = s_args_all;
	COM_Parse_Basic (&t); org[0] = atof(com_token);
	COM_Parse_Basic (&t); org[1] = atof(com_token);
	COM_Parse_Basic (&t); org[2] = atof(com_token);
	VectorCopyDestSrc (cl.tool_marker_origin, org);
	cl.tool_marker_on = true;
	if (should_print_more)
		Con_PrintLinef ("Tool marker set to " VECTOR3_5d1F, VECTOR3_SEND(cl.tool_marker_origin) );
}

/*
==================
SCR_ScreenShot_f
==================
*/

void SCR_ScreenShot_f (cmd_state_t *cmd)
{
	static int shotnumber;
	static char old_prefix_name[MAX_QPATH_128];
	char prefix_name[MAX_QPATH_128];
	char filename[MAX_QPATH_128];
	unsigned char *buffer1;
	unsigned char *buffer2;
	qbool jpeg = (scr_screenshot_jpeg.integer != 0);
	qbool png = (scr_screenshot_png.integer != 0) && !jpeg;
	char vabuf[1024];

	qbool toclipboard = String_Match (cmd_argv_parm1, "clipboard");

	if (toclipboard) {
		// Just skip all this.
		c_strlcpy (filename, "clipboard");
	}
	else if (Cmd_Argc(cmd) == 2) {
		const char *ext;
		c_strlcpy(filename, Cmd_Argv(cmd, 1));
		ext = FS_FileExtension(filename);
		if (String_Match_Caseless(ext, "jpg"))
		{
			jpeg = true;
			png = false;
		}
		else if (String_Match_Caseless(ext, "tga"))
		{
			jpeg = false;
			png = false;
		}
		else if (String_Match_Caseless(ext, "png"))
		{
			jpeg = false;
			png = true;
		}
		else
		{
			Con_PrintLinef ("screenshot: supplied filename must end in .jpg or .tga or .png");
			return;
		}
	}
	else if (scr_screenshot_timestamp.integer)
	{
		int shotnumber100;

		// TODO maybe make capturevideo and screenshot use similar name patterns?
		if (scr_screenshot_name_in_mapdir.integer && cl.worldbasename[0])
			dpsnprintf(prefix_name, sizeof(prefix_name), "%s/%s%s", cl.worldbasename, scr_screenshot_name.string, Sys_TimeString("%Y%m%d%H%M%S"));
		else
			dpsnprintf(prefix_name, sizeof(prefix_name), "%s%s", scr_screenshot_name.string, Sys_TimeString("%Y%m%d%H%M%S"));

		// find a file name to save it to
		for (shotnumber100 = 0;shotnumber100 < 100;shotnumber100++)
			if (!FS_SysFileExists(va(vabuf, sizeof(vabuf), "%s/screenshots/%s-%02d.tga", fs_gamedir, prefix_name, shotnumber100))
			 && !FS_SysFileExists(va(vabuf, sizeof(vabuf), "%s/screenshots/%s-%02d.jpg", fs_gamedir, prefix_name, shotnumber100))
			 && !FS_SysFileExists(va(vabuf, sizeof(vabuf), "%s/screenshots/%s-%02d.png", fs_gamedir, prefix_name, shotnumber100)))
				break;
		if (shotnumber100 >= 100)
		{
			Con_Print("Couldn't create the image file - already 100 shots taken this second!\n");
			return;
		}

		dpsnprintf(filename, sizeof(filename), "screenshots/%s-%02d.%s", prefix_name, shotnumber100, jpeg ? "jpg" : png ? "png" : "tga");
	}
	else
	{
		// TODO maybe make capturevideo and screenshot use similar name patterns?
		if (scr_screenshot_name_in_mapdir.integer && cl.worldbasename[0])
			dpsnprintf(prefix_name, sizeof(prefix_name), "%s/%s", cl.worldbasename, Sys_TimeString(scr_screenshot_name.string));
		else
			dpsnprintf(prefix_name, sizeof(prefix_name), "%s", Sys_TimeString(scr_screenshot_name.string));

		// if prefix changed, gamedir or map changed, reset the shotnumber so
		// we scan again
		// FIXME: should probably do this whenever FS_Rescan or something like that occurs?
		if (strcmp(old_prefix_name, prefix_name))
		{
			dpsnprintf(old_prefix_name, sizeof(old_prefix_name), "%s", prefix_name );
			shotnumber = 0;
		}

		// find a file name to save it to
		for (;shotnumber < 1000000;shotnumber++)
			if (!FS_SysFileExists(va(vabuf, sizeof(vabuf), "%s/screenshots/%s%06d.tga", fs_gamedir, prefix_name, shotnumber))
			 && !FS_SysFileExists(va(vabuf, sizeof(vabuf), "%s/screenshots/%s%06d.jpg", fs_gamedir, prefix_name, shotnumber))
			 && !FS_SysFileExists(va(vabuf, sizeof(vabuf), "%s/screenshots/%s%06d.png", fs_gamedir, prefix_name, shotnumber)))
				break;
		if (shotnumber >= 1000000)
		{
			Con_Print("Couldn't create the image file - you already have 1000000 screenshots!\n");
			return;
		}

		dpsnprintf(filename, sizeof(filename), "screenshots/%s%06d.%s", prefix_name, shotnumber, jpeg ? "jpg" : png ? "png" : "tga");

		shotnumber++;
	}

	buffer1 = (unsigned char *)Mem_Alloc(tempmempool, vid.width * vid.height * 4);
	buffer2 = (unsigned char *)Mem_Alloc(tempmempool, vid.width * vid.height * (scr_screenshot_alpha.integer ? 4 : 3));

	if (SCR_ScreenShot (filename, buffer1, buffer2, 0, 0, vid.width, vid.height, false, false, false,
		jpeg, png, /*gamma correct?*/ true, scr_screenshot_alpha.integer != 0 /*d: 0*/, toclipboard))
		Con_PrintLinef ("Wrote %s", filename);
	else
	{
		Con_PrintLinef (CON_ERROR "Unable to write %s", filename);
		if (jpeg || png)
		{
			if (SCR_ScreenShot (filename, buffer1, buffer2, 0, 0, vid.width, vid.height,
				false, false, false, false, false, /*gamma correct?*/ true,
				scr_screenshot_alpha.integer != 0 /*d: 0*/, q_clipboard_false))
			{
				strlcpy(filename + strlen(filename) - 3, "tga", 4);
				Con_PrintLinef ("Wrote %s", filename);
			}
		}
	}

	Mem_Free (buffer1);
	Mem_Free (buffer2);
}

#ifdef CONFIG_VIDEO_CAPTURE
static void SCR_CaptureVideo_BeginVideo(void)
{
	double r, g, b;
	unsigned int i;
	int width = cl_capturevideo_width.integer, height = cl_capturevideo_height.integer;
	if (cls.capturevideo.active)
		return;
	memset(&cls.capturevideo, 0, sizeof(cls.capturevideo));
	// soundrate is figured out on the first SoundFrame

	if (width == 0 && height != 0)
		width = (int) (height * (double)vid.width / ((double)vid.height * vid_pixelheight.value)); // keep aspect
	if (width != 0 && height == 0)
		height = (int) (width * ((double)vid.height * vid_pixelheight.value) / (double)vid.width); // keep aspect

	if (width < 2 || width > vid.width) // can't scale up
		width = vid.width;
	if (height < 2 || height > vid.height) // can't scale up
		height = vid.height;

	// ensure it's all even; if not, scale down a little
	if (width % 1)
		--width;
	if (height % 1)
		--height;

	cls.capturevideo.width = width;
	cls.capturevideo.height = height;
	cls.capturevideo.active = true;
	cls.capturevideo.framerate = bound(1, cl_capturevideo_fps.value, 1001) * bound(1, cl_capturevideo_framestep.integer, 64);
	cls.capturevideo.framestep = cl_capturevideo_framestep.integer;
	cls.capturevideo.soundrate = S_GetSoundRate();
	cls.capturevideo.soundchannels = S_GetSoundChannels();
	cls.capturevideo.startrealtime = host.realtime;
	cls.capturevideo.frame = cls.capturevideo.lastfpsframe = 0;
	cls.capturevideo.starttime = cls.capturevideo.lastfpstime = host.realtime;
	cls.capturevideo.soundsampleframe = 0;
	cls.capturevideo.is_realtime = cl_capturevideo_realtime.integer != 0;
	cls.capturevideo.screenbuffer = (unsigned char *)Mem_Alloc(tempmempool, vid.width * vid.height * 4);
	cls.capturevideo.outbuffer = (unsigned char *)Mem_Alloc(tempmempool, width * height * (4+4) + 18);
	dpsnprintf(cls.capturevideo.basename, sizeof(cls.capturevideo.basename), "video/%s%03i", Sys_TimeString(cl_capturevideo_nameformat.string), cl_capturevideo_number.integer);
	Cvar_SetValueQuick(&cl_capturevideo_number, cl_capturevideo_number.integer + 1);

	/*
	for (i = 0;i < 256;i++)
	{
		unsigned char j = (unsigned char)bound(0, 255*pow(i/255.0, gamma), 255);
		cls.capturevideo.rgbgammatable[0][i] = j;
		cls.capturevideo.rgbgammatable[1][i] = j;
		cls.capturevideo.rgbgammatable[2][i] = j;
	}
	*/
/*
R = Y + 1.4075 * (Cr - 128);
G = Y + -0.3455 * (Cb - 128) + -0.7169 * (Cr - 128);
B = Y + 1.7790 * (Cb - 128);
Y = R *  .299 + G *  .587 + B *  .114;
Cb = R * -.169 + G * -.332 + B *  .500 + 128.;
Cr = R *  .500 + G * -.419 + B * -.0813 + 128.;
*/

	// identity gamma table
	BuildGammaTable16(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, cls.capturevideo.vidramp, 256);
	BuildGammaTable16(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, cls.capturevideo.vidramp + 256, 256);
	BuildGammaTable16(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, cls.capturevideo.vidramp + 256*2, 256);
	if (scr_screenshot_gammaboost.value != 1)
	{
		double igamma = 1 / scr_screenshot_gammaboost.value;
		for (i = 0;i < 256 * 3;i++)
			cls.capturevideo.vidramp[i] = (unsigned short) (0.5 + pow(cls.capturevideo.vidramp[i] * (1.0 / 65535.0), igamma) * 65535.0);
	}

	for (i = 0;i < 256;i++)
	{
		r = 255*cls.capturevideo.vidramp[i]/65535.0;
		g = 255*cls.capturevideo.vidramp[i+256]/65535.0;
		b = 255*cls.capturevideo.vidramp[i+512]/65535.0;
		// NOTE: we have to round DOWN here, or integer overflows happen. Sorry for slightly wrong looking colors sometimes...
		// Y weights from RGB
		cls.capturevideo.rgbtoyuvscaletable[0][0][i] = (short)(r *  0.299);
		cls.capturevideo.rgbtoyuvscaletable[0][1][i] = (short)(g *  0.587);
		cls.capturevideo.rgbtoyuvscaletable[0][2][i] = (short)(b *  0.114);
		// Cb weights from RGB
		cls.capturevideo.rgbtoyuvscaletable[1][0][i] = (short)(r * -0.169);
		cls.capturevideo.rgbtoyuvscaletable[1][1][i] = (short)(g * -0.332);
		cls.capturevideo.rgbtoyuvscaletable[1][2][i] = (short)(b *  0.500);
		// Cr weights from RGB
		cls.capturevideo.rgbtoyuvscaletable[2][0][i] = (short)(r *  0.500);
		cls.capturevideo.rgbtoyuvscaletable[2][1][i] = (short)(g * -0.419);
		cls.capturevideo.rgbtoyuvscaletable[2][2][i] = (short)(b * -0.0813);
		// range reduction of YCbCr to valid signal range
		cls.capturevideo.yuvnormalizetable[0][i] = 16 + i * (236-16) / 256;
		cls.capturevideo.yuvnormalizetable[1][i] = 16 + i * (240-16) / 256;
		cls.capturevideo.yuvnormalizetable[2][i] = 16 + i * (240-16) / 256;
	}

	if (cl_capturevideo_ogg.integer)
	{
		if (SCR_CaptureVideo_Ogg_Available())
		{
			SCR_CaptureVideo_Ogg_BeginVideo();
			return;
		}
		else
			Con_Print("cl_capturevideo_ogg: libraries not available. Capturing in AVI instead.\n");
	}

	SCR_CaptureVideo_Avi_BeginVideo();
}

void SCR_CaptureVideo_EndVideo(void)
{
	if (!cls.capturevideo.active)
		return;
	cls.capturevideo.active = false;

	Con_Printf ("Finishing capture of %s.%s (%d frames, %d audio frames)\n", cls.capturevideo.basename, cls.capturevideo.formatextension, cls.capturevideo.frame, cls.capturevideo.soundsampleframe);

	if (cls.capturevideo.videofile)
	{
		cls.capturevideo.endvideo();
	}

	if (cls.capturevideo.screenbuffer)
	{
		Mem_Free (cls.capturevideo.screenbuffer);
		cls.capturevideo.screenbuffer = NULL;
	}

	if (cls.capturevideo.outbuffer)
	{
		Mem_Free (cls.capturevideo.outbuffer);
		cls.capturevideo.outbuffer = NULL;
	}

	memset(&cls.capturevideo, 0, sizeof(cls.capturevideo));
}

static void SCR_ScaleDownBGRA(unsigned char *in, int inw, int inh, unsigned char *out, int outw, int outh)
{
	// TODO optimize this function

	int x, y;
	float area;

	// memcpy is faster than me
	if (inw == outw && inh == outh)
	{
		memcpy(out, in, 4 * inw * inh);
		return;
	}

	// otherwise: a box filter
	area = (float)outw * (float)outh / (float)inw / (float)inh;
	for(y = 0; y < outh; ++y)
	{
		float iny0 =  y    / (float)outh * inh; int iny0_i = (int) floor(iny0);
		float iny1 = (y+1) / (float)outh * inh; int iny1_i = (int) ceil(iny1);
		for(x = 0; x < outw; ++x)
		{
			float inx0 =  x    / (float)outw * inw; int inx0_i = (int) floor(inx0);
			float inx1 = (x+1) / (float)outw * inw; int inx1_i = (int) ceil(inx1);
			float r = 0, g = 0, b = 0, alpha = 0;
			int xx, yy;

			for(yy = iny0_i; yy < iny1_i; ++yy)
			{
				float ya = min(yy+1, iny1) - max(iny0, yy);
				for(xx = inx0_i; xx < inx1_i; ++xx)
				{
					float a = ya * (min(xx+1, inx1) - max(inx0, xx));
					r += a * in[4*(xx + inw * yy)+0];
					g += a * in[4*(xx + inw * yy)+1];
					b += a * in[4*(xx + inw * yy)+2];
					alpha += a * in[4*(xx + inw * yy)+3];
				}
			}

			out[4*(x + outw * y)+0] = (unsigned char) (r * area);
			out[4*(x + outw * y)+1] = (unsigned char) (g * area);
			out[4*(x + outw * y)+2] = (unsigned char) (b * area);
			out[4*(x + outw * y)+3] = (unsigned char) (alpha * area);
		}
	}
}

static void SCR_CaptureVideo_VideoFrame(int newframestepframenum)
{
	int x = 0, y = 0;
	int width = cls.capturevideo.width, height = cls.capturevideo.height;

	if (newframestepframenum == cls.capturevideo.framestepframe)
		return;

	CHECKGLERROR
	// speed is critical here, so do saving as directly as possible

	GL_ReadPixelsBGRA(x, y, vid.width, vid.height, cls.capturevideo.screenbuffer);

	SCR_ScaleDownBGRA (cls.capturevideo.screenbuffer, vid.width, vid.height, cls.capturevideo.outbuffer, width, height);

	cls.capturevideo.videoframes(newframestepframenum - cls.capturevideo.framestepframe);
	cls.capturevideo.framestepframe = newframestepframenum;

	if (cl_capturevideo_printfps.integer)
	{
		char buf[80];
		double t = host.realtime;
		if (t > cls.capturevideo.lastfpstime + 1)
		{
			double fps1 = (cls.capturevideo.frame - cls.capturevideo.lastfpsframe) / (t - cls.capturevideo.lastfpstime + 0.0000001);
			double fps  = (cls.capturevideo.frame                                ) / (t - cls.capturevideo.starttime   + 0.0000001);
			dpsnprintf(buf, sizeof(buf), "capturevideo: (%.1fs) last second %.3ffps, total %.3ffps\n", cls.capturevideo.frame / cls.capturevideo.framerate, fps1, fps);
			Sys_PrintToTerminal(buf);
			cls.capturevideo.lastfpstime = t;
			cls.capturevideo.lastfpsframe = cls.capturevideo.frame;
		}
	}
}

void SCR_CaptureVideo_SoundFrame(const portable_sampleframe_t *paintbuffer, size_t length)
{
	cls.capturevideo.soundsampleframe += (int)length;
	cls.capturevideo.soundframe(paintbuffer, length);
}

static void SCR_CaptureVideo(void)
{
	int newframenum;
	if (cl_capturevideo.integer)
	{
		if (!cls.capturevideo.active)
			SCR_CaptureVideo_BeginVideo();
		if (cls.capturevideo.framerate != cl_capturevideo_fps.value * cl_capturevideo_framestep.integer)
		{
			Con_Printf ("You can not change the video framerate while recording a video.\n");
			Cvar_SetValueQuick(&cl_capturevideo_fps, cls.capturevideo.framerate / (double) cl_capturevideo_framestep.integer);
		}
		// for AVI saving we have to make sure that sound is saved before video
		if (cls.capturevideo.soundrate && !cls.capturevideo.soundsampleframe)
			return;
		if (cls.capturevideo.is_realtime)
		{
			// preserve sound sync by duplicating frames when running slow
			newframenum = (int)((host.realtime - cls.capturevideo.startrealtime) * cls.capturevideo.framerate);
		}
		else
			newframenum = cls.capturevideo.frame + 1;
		// if falling behind more than one second, stop
		if (newframenum - cls.capturevideo.frame > 60 * (int)ceil(cls.capturevideo.framerate))
		{
			Cvar_SetValueQuick(&cl_capturevideo, 0);
			Con_Printf ("video saving failed on frame %d, your machine is too slow for this capture speed.\n", cls.capturevideo.frame);
			SCR_CaptureVideo_EndVideo();
			return;
		}
		// write frames
		SCR_CaptureVideo_VideoFrame(newframenum / cls.capturevideo.framestep);
		cls.capturevideo.frame = newframenum;
		if (cls.capturevideo.error)
		{
			Cvar_SetValueQuick(&cl_capturevideo, 0);
			Con_Printf ("video saving failed on frame %d, out of disk space? stopping video capture.\n", cls.capturevideo.frame);
			SCR_CaptureVideo_EndVideo();
		}
	}
	else if (cls.capturevideo.active)
		SCR_CaptureVideo_EndVideo();
}
#endif

/*
===============
R_Envmap_f

Grab six views for environment mapping tests
===============
*/
struct envmapinfo_s
{
	float angles[3];
	const char *name;
	qbool flipx, flipy, flipdiagonaly;
}
envmapinfo[12] =
{
	{{  0,   0, 0}, "rt", false, false, false},
	{{  0, 270, 0}, "ft", false, false, false},
	{{  0, 180, 0}, "lf", false, false, false},
	{{  0,  90, 0}, "bk", false, false, false},
	{{-90, 180, 0}, "up",  true,  true, false},
	{{ 90, 180, 0}, "dn",  true,  true, false},

	{{  0,   0, 0}, "px",  true,  true,  true},
	{{  0,  90, 0}, "py", false,  true, false},
	{{  0, 180, 0}, "nx", false, false,  true},
	{{  0, 270, 0}, "ny",  true, false, false},
	{{-90, 180, 0}, "pz", false, false,  true},
	{{ 90, 180, 0}, "nz", false, false,  true}
};

RELATED_ (R_LevelShot_Maps_All_f)
//int levelshots_next_map(void)
//{
//}

CALLERS_ (R_LevelShot_Here_f)
// Returns 1 for TGA, 2 for JPEG
int has_levelshot (ccs *mapbasename_no_path_no_ext, char *sname_reply, size_t sizeof_sname)
{
	ccs *map = mapbasename_no_path_no_ext;
	va_super (levelshotname_tga, MAX_QPATHX2_256, "levelshots/%s.tga", map);
	int result = FS_FileExists (levelshotname_tga);
	if (result) {
		if (sname_reply)
			dp_strlcpy (sname_reply, levelshotname_tga, sizeof_sname);
		return 1;
	}

	va_super (levelshotname_jpg, MAX_QPATHX2_256, "levelshots/%s.jpg", map);
	result = FS_FileExists (levelshotname_jpg);

	if (result) {
		if (sname_reply)
			dp_strlcpy (sname_reply, levelshotname_jpg, sizeof_sname);
		return 2;
	}
	return 0;
}

int getnextmap_levelshots_after (int trueidxafterthis)
{ // We get a -1 or something.
	int result = not_found_neg1;
	for (int idx = trueidxafterthis + 1; idx < m_maplist_count; idx ++) {
		maplist_s *mm = &m_maplist[idx];
		int has_shot = has_levelshot ((ccs*)mm->s_name_after_maps_folder_a, /*name reply*/ NULL, 0);
		if (has_shot)
			continue; // Don't need
		result = idx;
		break;
	}
	return result; // Returns idx0 or not_found_neg1
}


void levels_shot_decide_map_and_act (int after_this_idx0)
{
	int idx = getnextmap_levelshots_after(after_this_idx0);
	cls.cl_first_world_frame_rendered = 0;
	if (idx == not_found_neg1) {
		cls.levelshots_index_plus1 = 0; // DONE!
		Vid_SetWindowTitlef (gamename);
		Con_PrintLinef ("Levelshots process completed!");
	} else {

		maplist_s *mm = &m_maplist[idx];

		// WRITE A SCREENSHOT FAILED IMAGE FIRST
		va_super (levelshots_mapname_jpg, MAX_QPATH_128, "levelshots/%s.jpg", mm->s_name_after_maps_folder_a);
		fs_offset_t filesize;
		ccs *s_file = "engine/screenshot_failed.jpg";
		byte *jpeg_failed_za = FS_LoadFile(s_file, tempmempool, fs_quiet_FALSE, &filesize);
		if (!jpeg_failed_za) {
			Con_PrintLinef ("Couldn't open engine jpeg %s", s_file);
		}
		else {
			size_t blobsize = filesize;
			/*int isok =*/ FS_WriteFile(levelshots_mapname_jpg, jpeg_failed_za, blobsize);
			Vid_SetWindowTitlef ("Levelshotting: %s", levelshots_mapname_jpg);
		}

		Mem_FreeNull_ (jpeg_failed_za);

		Cbuf_AddTextLinef (cmd_local, "map %s", mm->s_name_after_maps_folder_a);
		cls.levelshots_index_plus1 = PLUS1(idx);
	}

}

static void R_LevelShot_Here_f (cmd_state_t *cmd)
{
	// Baker: April 17 2025 - Increased fix where
	// Quake has worldmodel but no worldentity during
	// part of download/handshake
	if (!r_refdef.scene.worldmodel) {
		Con_PrintLinef (CON_WARN "%s: no r_refdef.scene.worldmodel", __func__);
		return;
	}
	if (!r_refdef.scene.worldentity) {
		Con_PrintLinef (CON_WARN "%s: no r_refdef.scene.worldentity", __func__);
		return;
	}

	//if (!r_refdef.scene.worldmodel)
	//	return; // December 11 2024 - Missing map or map not downloaded yet.

	ccs *sarg1 = Cmd_Argv (cmd, 1);
	int is_silent = sarg1 && sarg1[0] != NULL_CHAR_0;
	size_t blobsize = 0;
	byte *jpeg_data_zalloc = Screenshot_To_Jpeg_Blob_ZAlloc_512_320 (&blobsize);

	if (!jpeg_data_zalloc) {
		Con_PrintLinef (CON_WARN "Screenshot_To_Jpeg_String_Malloc_512_320 failed" NEWLINE);
		return;
	}

	va_super (levelshots_mapname_jpg, MAX_QPATH_128, "levelshots/%s.jpg", cl.worldbasename);

	int isok = FS_WriteFile(levelshots_mapname_jpg, jpeg_data_zalloc, blobsize);
	if (isok == false)
		Con_PrintLinef (CON_WARN "Error writing " QUOTED_S, levelshots_mapname_jpg);
	else {
		if (is_silent)
			Con_DPrintLinef ("Wrote %s size %d bytes", levelshots_mapname_jpg, (int)blobsize);
		else
			Con_PrintLinef ("Wrote %s size %d bytes", levelshots_mapname_jpg, (int)blobsize);
	}

	Mem_FreeNull_ (jpeg_data_zalloc);

	if (cls.levelshots_index_plus1)
		levels_shot_decide_map_and_act (UNPLUS1(cls.levelshots_index_plus1));
}

static void R_LevelShot_Maps_All_f (cmd_state_t *cmd)
{
//extern maplist_s m_maplist[MAXMAPLIST_4096];
//extern int m_maplist_count;
	ccs *prefix = "";
	if (Cmd_Argc(cmd) == 2) {
		prefix = Cmd_Argv(cmd, 1);
	}
	RELATED_ (M_Menu_Maps_f)

		// Fill it ...
	GetMapList(prefix, NULL, 0, /*is_menu_fill*/ true, /*autocompl*/ false, /*suppress*/ false);

	if (m_maplist_count) {
		levels_shot_decide_map_and_act (not_found_neg1);
		Con_PrintLinef ("Started levelshots generating process ... " NEWLINE
						"please be patient ..." NEWLINE
						"Do not use menu or do console commands ...");
	}
	else Con_PrintLinef ("No gamedir maps to make level shots");
}



static void R_Envmap_f (cmd_state_t *cmd)
{
	int j, size;
	char filename[MAX_QPATH_128], basename[MAX_QPATH_128];
	unsigned char *buffer1;
	unsigned char *buffer2;
#if 0
	r_rendertarget_t *rt;
#endif
	int is_auto = false;


	if (Cmd_Argc(cmd) == 2 && String_Match_Caseless(Cmd_Argv(cmd, 1), "auto")) {
		is_auto = true;
	}

	else if (Cmd_Argc(cmd) != 3) {
		Con_PrintLinef ("envmap <basename> <size>: save out 6 cubic environment map images, usable with loadsky, note that size must one of 128, 256, 512, or 1024 and can't be bigger than your current resolution");
		Con_PrintLinef (NEWLINE "envmap auto: automatically create 512 sized images (viewsize 120)");
		return;
	}

	if (cls.state != ca_connected) {
		Con_PrintLinef ("envmap: No map loaded");
		return;
	}

	c_strlcpy (basename, is_auto ? "auto_" : Cmd_Argv(cmd, 1));
	size = is_auto ? 512 : atoi(Cmd_Argv(cmd, 2));

	if (size != 128 && size != 256 && size != 512 && size != 1024) {
		Con_PrintLinef ("envmap: size must be one of 128, 256, 512, or 1024");
		return;
	}

	if (size > vid.width || size > vid.height) {
		Con_PrintLinef ("envmap: your resolution is not big enough to render that size");
		return;
	}

	r_refdef.envmap = true;

	R_UpdateVariables();

	r_refdef.view.x = 0;
	r_refdef.view.y = 0;
	r_refdef.view.z = 0;
	r_refdef.view.width = size;
	r_refdef.view.height = size;
	r_refdef.view.depth = 1;
	r_refdef.view.useperspective = true;
	r_refdef.view.isoverlay = false;
	r_refdef.view.ismain = true;

	r_refdef.view.frustum_x = 1; // tan(45 * M_PI / 180.0);
	r_refdef.view.frustum_y = 1; // tan(45 * M_PI / 180.0);
	r_refdef.view.ortho_x = 90; // abused as angle by VM_CL_R_SetView
	r_refdef.view.ortho_y = 90; // abused as angle by VM_CL_R_SetView

	buffer1 = (unsigned char *)Mem_Alloc(tempmempool, size * size * 4);
	buffer2 = (unsigned char *)Mem_Alloc(tempmempool, size * size * 3);

#if 0000 // ZEROIC
	// TODO: use TEXTYPE_COLORBUFFER16F and output to .exr files as well?
	rt = R_RenderTarget_Get(size, size, TEXTYPE_DEPTHBUFFER24STENCIL8, true, TEXTYPE_COLORBUFFER, TEXTYPE_UNUSED, TEXTYPE_UNUSED, TEXTYPE_UNUSED);
#endif
	CL_UpdateEntityShading();

	r_refdef_view_t oldview = r_refdef.view;
	r_refdef.view.width = size, r_refdef.view.height = size;

	//for (j = 0;j < 12;j++)
	int j_max = is_auto ? 6 : 12;
	for (j = 0;j < j_max; j++) { // Baker: Use the 6 human friendly ones for auto
		c_dpsnprintf2 (filename, "env/%s%s", basename, envmapinfo[j].name);
		Matrix4x4_CreateFromQuakeEntity(&r_refdef.view.matrix, r_refdef.view.origin[0], r_refdef.view.origin[1], r_refdef.view.origin[2], envmapinfo[j].angles[0], envmapinfo[j].angles[1], envmapinfo[j].angles[2], 1);
		r_refdef.view.quality = 1;
		r_refdef.view.clear = true;
		R_Mesh_Start();
#if 1
	//R_RenderView(0, NULL, NULL, r_refdef.view.x, r_refdef.view.y, r_refdef.view.width, r_refdef.view.height);
	R_RenderView(0, NULL, NULL, r_refdef.view.x, r_refdef.view.y, r_refdef.view.width, r_refdef.view.height);
#else
		R_RenderView(rt->fbo, rt->depthtexture, rt->colortexture[0], 0, 0, size, size);
#endif
		R_Mesh_Finish();

#if 0
		c_strlcat (filename, ".tga");
		SCR_ScreenShot(filename, buffer1, buffer2, 0, vid.height - (r_refdef.view.y + r_refdef.view.height), size, size, envmapinfo[j].flipx, envmapinfo[j].flipy, envmapinfo[j].flipdiagonaly, q_is_jpeg_false, q_is_png_false, false, false);
		Con_PrintLinef ("Wrote envmap " QUOTED_S, filename);

		File_URL_Edit_Remove_Extension (filename);
#endif

		c_strlcat (filename, ".jpg");
		SCR_ScreenShot(filename, buffer1, buffer2, 0, vid.height - (r_refdef.view.y + r_refdef.view.height), size, size, envmapinfo[j].flipx, envmapinfo[j].flipy, envmapinfo[j].flipdiagonaly, q_is_jpeg_true, q_is_png_false, false, false, q_clipboard_false);
		Con_PrintLinef ("Wrote envmap " QUOTED_S, filename);
	}

	Mem_Free (buffer1);
	Mem_Free (buffer2);
	r_refdef.view = oldview;

	r_refdef.envmap = false;
}

//=============================================================================

void SHOWLMP_decodehide(void)
{
	int i;
	char *lmplabel;
	lmplabel = MSG_ReadString(&cl_message, cl_readstring, sizeof(cl_readstring));
	for (i = 0;i < cl.num_showlmps;i++)
		if (cl.showlmps[i].isactive && strcmp(cl.showlmps[i].label, lmplabel) == 0)
		{
			cl.showlmps[i].isactive = false;
			return;
		}
}

void SHOWLMP_decodeshow(void)
{
	int k;
	char lmplabel[256], picname[256];
	float x, y;
	strlcpy (lmplabel,MSG_ReadString(&cl_message, cl_readstring, sizeof(cl_readstring)), sizeof (lmplabel));
	strlcpy (picname, MSG_ReadString(&cl_message, cl_readstring, sizeof(cl_readstring)), sizeof (picname));
	if (gamemode == GAME_NEHAHRA) // LadyHavoc: nasty old legacy junk
	{
		x = MSG_ReadByte(&cl_message);
		y = MSG_ReadByte(&cl_message);
	}
	else
	{
		x = MSG_ReadShort(&cl_message);
		y = MSG_ReadShort(&cl_message);
	}
	if (!cl.showlmps || cl.num_showlmps >= cl.max_showlmps)
	{
		showlmp_t *oldshowlmps = cl.showlmps;
		cl.max_showlmps += 16;
		cl.showlmps = (showlmp_t *) Mem_Alloc(cls.levelmempool, cl.max_showlmps * sizeof(showlmp_t));
		if (oldshowlmps)
		{
			if (cl.num_showlmps)
				memcpy(cl.showlmps, oldshowlmps, cl.num_showlmps * sizeof(showlmp_t));
			Mem_Free(oldshowlmps);
		}
	}
	for (k = 0;k < cl.max_showlmps;k++)
		if (cl.showlmps[k].isactive && String_Match(cl.showlmps[k].label, lmplabel))
			break;
	if (k == cl.max_showlmps)
		for (k = 0;k < cl.max_showlmps;k++)
			if (!cl.showlmps[k].isactive)
				break;
	cl.showlmps[k].isactive = true;
	strlcpy (cl.showlmps[k].label, lmplabel, sizeof (cl.showlmps[k].label));
	strlcpy (cl.showlmps[k].pic, picname, sizeof (cl.showlmps[k].pic));
	cl.showlmps[k].x = x;
	cl.showlmps[k].y = y;
	cl.num_showlmps = max(cl.num_showlmps, k + 1);
}

void SHOWLMP_drawall(void)
{
	int i;
	for (i = 0;i < cl.num_showlmps;i++)
		if (cl.showlmps[i].isactive)
			DrawQ_Pic(cl.showlmps[i].x, cl.showlmps[i].y, Draw_CachePic_Flags (cl.showlmps[i].pic, CACHEPICFLAG_NOTPERSISTENT), 0, 0, 1, 1, 1, 1, 0);
}

/*
==============================================================================

						SCREEN SHOTS

==============================================================================
*/

// buffer1: 4*w*h
// buffer2: 3*w*h (or 4*w*h if screenshotting alpha too)

CALLERS_ (SCR_ScreenShot_f)
qbool SCR_ScreenShot(char *filename, unsigned char *buffer1, unsigned char *buffer2,
					 int x, int y, int width, int height, qbool flipx, qbool flipy,
					 qbool flipdiagonal, qbool jpeg, qbool png, qbool gammacorrect, qbool keep_alpha, qbool toclipboard)
{
	int	indices[4] = {0,1,2,3}; // BGRA
	qbool ret;


	GL_ReadPixelsBGRA(x, y, width, height, buffer1);

	if (gammacorrect && (scr_screenshot_gammaboost.value /*d:1*/ != 1))
	{
		int i;
		double igamma = 1.0 / scr_screenshot_gammaboost.value;
		unsigned short vidramp[256 * 3];
		// identity gamma table
		BuildGammaTable16(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, vidramp, 256);
		BuildGammaTable16(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, vidramp + 256, 256);
		BuildGammaTable16(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, vidramp + 256*2, 256);
		if (scr_screenshot_gammaboost.value != 1)
		{
			for (i = 0;i < 256 * 3;i++)
				vidramp[i] = (unsigned short) (0.5 + pow(vidramp[i] * (1.0 / 65535.0), igamma) * 65535.0);
		}
		for (i = 0;i < width*height*4;i += 4)
		{
			buffer1[i] = (unsigned char) (vidramp[buffer1[i] + 512] * 255.0 / 65535.0 + 0.5); // B
			buffer1[i+1] = (unsigned char) (vidramp[buffer1[i+1] + 256] * 255.0 / 65535.0 + 0.5); // G
			buffer1[i+2] = (unsigned char) (vidramp[buffer1[i+2]] * 255.0 / 65535.0 + 0.5); // R
			// A
		}
	}

	if (toclipboard) {
		Image_Flip_Buffer (buffer1, width, height, RGBA_4);
		qbool is_ok = Sys_Clipboard_Set_Image_BGRA_Is_Ok ((rgba4 *) buffer1, width, height);

		return is_ok;
	}

	if (png) {
		Image_Flip_RedGreen (buffer1, width * height * RGBA_4);
		Image_Flip_Buffer (buffer1, width, height, RGBA_4);
		ret = PNG_SaveImage_preflipped (filename, width, height, true, buffer1);
		return ret;
	}

	if (keep_alpha && !jpeg)
	{
		if (!png)
			flipy = !flipy; // TGA: not preflipped
		Image_CopyMux (buffer2, buffer1, width, height, flipx, flipy, flipdiagonal, 4, 4, indices);
		if (png)
			ret = PNG_SaveImage_preflipped (filename, width, height, true, buffer2);
		else
			ret = Image_WriteTGABGRA(filename, width, height, buffer2);
	}
	else
	{
		if (jpeg)
		{
			indices[0] = 2;
			indices[2] = 0; // RGB
		}
		Image_CopyMux (buffer2, buffer1, width, height, flipx, flipy, flipdiagonal, 3, 4, indices);
		if (jpeg)
			ret = JPEG_SaveImage_preflipped (filename, width, height, buffer2);
		else if (png)
			ret = PNG_SaveImage_preflipped (filename, width, height, false, buffer2);
		else
			ret = Image_WriteTGABGR_preflipped (filename, width, height, buffer2);
	}

	return ret;
}

//=============================================================================

int scr_numtouchscreenareas;
scr_touchscreenarea_t scr_touchscreenareas[128];

static void SCR_DrawTouchscreenOverlay(void)
{
	int i;
	scr_touchscreenarea_t *a;
	cachepic_t *pic;
	for (i = 0, a = scr_touchscreenareas;i < scr_numtouchscreenareas;i++, a++)
	{
		if (vid_touchscreen_outlinealpha.value > 0 && a->rect[0] >= 0 && a->rect[1] >= 0 && a->rect[2] >= 4 && a->rect[3] >= 4)
		{
			DrawQ_Fill(a->rect[0] +              2, a->rect[1]                 , a->rect[2] - 4,          1    , 1, 1, 1, vid_touchscreen_outlinealpha.value * (0.5f + 0.5f * a->active), 0);
			DrawQ_Fill(a->rect[0] +              1, a->rect[1] +              1, a->rect[2] - 2,          1    , 1, 1, 1, vid_touchscreen_outlinealpha.value * (0.5f + 0.5f * a->active), 0);
			DrawQ_Fill(a->rect[0]                 , a->rect[1] +              2,          2    , a->rect[3] - 2, 1, 1, 1, vid_touchscreen_outlinealpha.value * (0.5f + 0.5f * a->active), 0);
			DrawQ_Fill(a->rect[0] + a->rect[2] - 2, a->rect[1] +              2,          2    , a->rect[3] - 2, 1, 1, 1, vid_touchscreen_outlinealpha.value * (0.5f + 0.5f * a->active), 0);
			DrawQ_Fill(a->rect[0] +              1, a->rect[1] + a->rect[3] - 2, a->rect[2] - 2,          1    , 1, 1, 1, vid_touchscreen_outlinealpha.value * (0.5f + 0.5f * a->active), 0);
			DrawQ_Fill(a->rect[0] +              2, a->rect[1] + a->rect[3] - 1, a->rect[2] - 4,          1    , 1, 1, 1, vid_touchscreen_outlinealpha.value * (0.5f + 0.5f * a->active), 0);
		}
		pic = a->pic ? Draw_CachePic_Flags(a->pic, CACHEPICFLAG_FAILONMISSING_256) : NULL;
		if (Draw_IsPicLoaded(pic))
			DrawQ_Pic(a->rect[0], a->rect[1], pic, a->rect[2], a->rect[3], 1, 1, 1, vid_touchscreen_overlayalpha.value * (0.5f + 0.5f * a->active), 0);
		if (a->text && a->text[0])
		{
			int textwidth = DrawQ_TextWidth(a->text, 0, a->textheight, a->textheight, false, FONT_CHAT);
			DrawQ_String(a->rect[0] + (a->rect[2] - textwidth) * 0.5f, a->rect[1] + (a->rect[3] - a->textheight) * 0.5f, a->text, 0, a->textheight, a->textheight, 1.0f, 1.0f, 1.0f, vid_touchscreen_overlayalpha.value, 0, NULL, false, FONT_CHAT);
		}
	}
}


byte *Jpeg_From_BGRA_Temppool_Alloc (const bgra4 *bgrapels_in, int width, int height, /*reply*/ size_t *size_out)
{
	RELATED_ (R_Envmap_f)
	qbool	jpeg_true = true;

	unsigned char *jpegbuf2_3_alloc  = (unsigned char *)Mem_Alloc(tempmempool, width * height *
		(scr_screenshot_alpha.integer /*d: 0*/ ? RGBA_4 : RGB_3));

	size_t jpeg_bytes_maxsize = width * height * RGB_3;
	unsigned char *jpeg_bytes_out_alloc = (unsigned char *)Mem_Alloc(tempmempool, jpeg_bytes_maxsize);


//////////////////////////////////////////////////////////////////////////////////////
	int	indices[4] = {0,1,2,3}; // BGRA
	if (jpeg_true) {
		indices[0] = 2;
		indices[2] = 0; // RGB
	}

	qbool flipx_false = false;

	// Baker: Clipboard data does not need y flip
	qbool flipy_false = false;

	qbool flipdiagonal_false = false;
	Image_CopyMux (jpegbuf2_3_alloc, (const unsigned char *)bgrapels_in, width, height, flipx_false, flipy_false,
		flipdiagonal_false, /*numoutput component*/ RGB_3, /*num input coponents*/ RGBA_4, indices);

	// Baker: JPEG_SaveImage_to_Buffer seems to take rgb3 pels
	size_t jpeg_bytes_out_actual_size = JPEG_SaveImage_to_Buffer (
		(char *)jpeg_bytes_out_alloc,
		jpeg_bytes_maxsize, width, height, jpegbuf2_3_alloc);

	//Mem_Free (from_opengl_pels4_alloc);
	Mem_Free (jpegbuf2_3_alloc);

	*size_out = jpeg_bytes_out_actual_size;
	return jpeg_bytes_out_alloc; // temppool
}

unsigned char *Screenshot_Jpeg_Temppool_Alloc (int x, int y, int width, int height, /*reply*/ size_t *size_out)
{
	RELATED_ (R_Envmap_f)
	qbool	jpeg_true = true;

	unsigned char *from_opengl_pels4_alloc = (unsigned char *)Mem_Alloc(tempmempool, width * height * RGBA_4);
	unsigned char *jpegbuf2_3_alloc  = (unsigned char *)Mem_Alloc(tempmempool, width * height *
		(scr_screenshot_alpha.integer /*d: 0*/ ? RGBA_4 : RGB_3));

	size_t jpeg_bytes_maxsize = width * height * RGB_3;
	unsigned char *jpeg_bytes_out_alloc = (unsigned char *)Mem_Alloc(tempmempool, jpeg_bytes_maxsize);

	GL_ReadPixelsBGRA (x, y, width, height, /*outbuf ->*/ from_opengl_pels4_alloc);

//////////////////////////////////////////////////////////////////////////////////////
	int	indices[4] = {0,1,2,3}; // BGRA
	if (jpeg_true) {
		indices[0] = 2;
		indices[2] = 0; // RGB
	}

	qbool flipx_false = false;
	qbool flipy_true = true;
	qbool flipdiagonal_false = false;
	Image_CopyMux (jpegbuf2_3_alloc, from_opengl_pels4_alloc, width, height, flipx_false, flipy_true,
		flipdiagonal_false, /*numoutput component*/ RGB_3, /*num input coponents*/ RGBA_4, indices);

	// Baker: JPEG_SaveImage_to_Buffer seems to take rgb3 pels
	size_t jpeg_bytes_out_actual_size = JPEG_SaveImage_to_Buffer (
		(char *)jpeg_bytes_out_alloc,
		jpeg_bytes_maxsize, width, height, jpegbuf2_3_alloc);

	Mem_Free (from_opengl_pels4_alloc);
	Mem_Free (jpegbuf2_3_alloc);

	*size_out = jpeg_bytes_out_actual_size;
	return jpeg_bytes_out_alloc; // temppool
}


#if 0

// Baker: This is a fullscreen jpeg malloc, we don't use it anywhere
char *Screenshot_Jpeg_String_Malloc_As_Is (void)
{
	RELATED_ (SCR_ScreenShot_f  R_Envmap_f SCR_ScreenShot)
	size_t data_size;
	unsigned char *data_alloc = Screenshot_Jpeg_Temppool_Alloc(/*xy*/ 0, 0, vid.width, vid.height, &data_size);
	char *s_base64_alloc = base64_encode_calloc (data_alloc, data_size, q_reply_len_NULL); // malloc
	Mem_Free (data_alloc);
	return s_base64_alloc; // free (s_base64_alloc);
}
#endif

