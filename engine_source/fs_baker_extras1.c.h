// fs_baker_extras1.c.h


char *ShaderText_ZAlloc (shader_t *myshader, const char *s_shadername, char *s_return_shader, size_t s_return_shader_size);

// Returns null if not found

// Returns relative real path
char *FS_RealFilePath_Z_Alloc (const char *s_quake_file)
{
	int index; // Why? Don't ask ... move along ...
	searchpath_t *sp = FS_FindFile (s_quake_file, &index, fs_quiet_true);
	if (!sp) {
		//Con_PrintLinef ("%s isn't anywhere", filename);
		return NULL;
	}
	if (sp->pack) {
		//if (sp->pack->vpack)
		//	Con_PrintLinef ("%s is in virtual package %sdir", filename, sp->pack->shortname);
		//else
		//	Con_PrintLinef ("%s is in package %s", filename, sp->pack->shortname);
		return NULL;
	}

	char sbuf[MAX_OSPATH_EX_1024];
	c_dpsnprintf2 (sbuf, "%s%s", sp->filename, s_quake_file); // Hmmm.
	char *s_realpath_zalloc = Z_StrDup (sbuf);//  (char *)z_memdup_z (sp->filename, strlen(sp->filename));
	return s_realpath_zalloc;
}

void R_ShaderPrint_f (cmd_state_t *cmd)
{
	int is_ok = (cls.state == ca_connected && cls.signon == SIGNONS_4 && cl.worldmodel);

	if (!is_ok) {
		Con_PrintLinef ("Not connected");
		return;
	}

	const char *s_shadername;

	extern cvar_t showtex;
	char texstring[MAX_QPATH_128];
	if (Cmd_Argc (cmd) == 1 && showtex.value) {
		vec3_t org;
		vec3_t dest;
		vec3_t temp;
		trace_t cltrace = {0};
		int hitnetentity = -1;


		Matrix4x4_OriginFromMatrix(&r_refdef.view.matrix, org);
		VectorSet(temp, 65536, 0, 0);
		Matrix4x4_Transform(&r_refdef.view.matrix, temp, dest);
		// clear the traces as we may or may not fill them out, and mark them with an invalid fraction so we know if we did
		memset(&cltrace, 0, sizeof(cltrace));
		cltrace.fraction = 2.0;

		trace_t CL_TraceLine(const vec3_t start, const vec3_t end, int type, prvm_edict_t *passedict, int hitsupercontentsmask, int skipsupercontentsmask, int skipmaterialflagsmask, float extend, qbool hitnetworkbrushmodels, int hitnetworkplayers, int *hitnetworkentity, qbool hitcsqcentities, qbool hitsurfaces);
		cltrace = CL_TraceLine(org, dest, MOVE_HITMODEL_4, NULL,
				SUPERCONTENTS_SOLID | SUPERCONTENTS_WATER | SUPERCONTENTS_SLIME |
				SUPERCONTENTS_LAVA | SUPERCONTENTS_SKY
			,
			0,
			showtex.integer >= 2 ? MATERIALFLAGMASK_TRANSLUCENT : 0, collision_extendmovelength.value,
			true, false, &hitnetentity, true, true);
		if (cltrace.hittexture)
			c_strlcpy (texstring, cltrace.hittexture->name);
		else
			c_strlcpy (texstring, "(no texture hit)");

		s_shadername = texstring;
		goto use_look_at;
	}

	if (Cmd_Argc (cmd) < 2) {
		Con_PrintLinef ("usage: %s <shader>" NEWLINE "prints text of first instance of shader and what file it is in", Cmd_Argv(cmd, 0));
		Con_PrintLinef (" example: %s textures/trak5x_sh/floor_floor2f", Cmd_Argv(cmd, 0));
		return;
	}

	s_shadername = Cmd_Argv (cmd, 1);

    shader_t *myshader;

use_look_at:
	myshader = Mod_LookupQ3Shader(s_shadername);

	if (!myshader) {
		Con_PrintLinef ("Can't find shader " QUOTED_S, s_shadername);
		return;
	}

	//int is_sky = Have_Flag_Strict_Bool (myshader->surfaceparms, Q3SURFACEPARM_SKY);
	//Con_PrintLinef (CON_BRONZE " Is Sky? = %d", is_sky);
	Con_PrintLinef ("Found shader " QUOTED_S, s_shadername);

	char filename[MAX_QPATH_128];
	char *s_shadertext_zalloc = ShaderText_ZAlloc (myshader, s_shadername,
		filename, sizeof(filename));

	if (!s_shadertext_zalloc) {
		Con_PrintLinef ("Did not find shader text");
		return;
	}

	Con_PrintLinef ("Shader file: %s", filename);

	int index; // Why?
	searchpath_t *sp = FS_FindFile(filename, &index, fs_quiet_true);
	if (!sp) {
		Con_PrintLinef ("%s isn't anywhere", filename);
		return;
	}
	if (sp->pack)
	{
		if (sp->pack->vpack)
			Con_PrintLinef ("%s is in virtual package %sdir", filename, sp->pack->shortname);
		else
			Con_PrintLinef ("%s is in package %s", filename, sp->pack->shortname);
	}
	else
		Con_PrintLinef ("%s is file %s%s", filename, sp->filename, filename);

	Con_PrintLinef ("Shader text is: " NEWLINE "%s" NEWLINE, s_shadertext_zalloc);
	//Clipboard_Set_Text (shadertextbuf);

	Mem_FreeNull_ (s_shadertext_zalloc);
}

// Baker r3101: pwd command to say the current directory
void FS_Pwd_f(cmd_state_t *cmd)
{
	const char *s_cwd_ta = FS_Getcwd_No_Trailing_Slash_Unsafe_TempAlloc ();
	Con_PrintLinef ("Current working directory: %s", s_cwd_ta);
	Con_PrintLinef ("fs_basedir : %s", fs_basedir);
	Con_PrintLinef ("fs_gamedir : %s", fs_gamedir);
	Con_PrintLinef ("fs_userdir : %s", fs_userdir);

	Mem_FreeNull_ (s_cwd_ta);
}

void FS_DirPat_f(cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) < 2) {
		Con_PrintLinef ("Need params");
		return;
	}

	Con_PrintLinef ("If successful, the results of *.sav will match windows per gamedir");
	Con_PrintLinef ("2 is " QUOTED_S, Cmd_Argv(cmd, 2));

	const char *spat = Cmd_Argv(cmd, 1);
	int do_gamedironly = atoi(Cmd_Argv(cmd, 2)) != 0 ?  fs_gamedironly_true : fs_gamedironly_false;

	Con_PrintLinef ("Trying FS_Search with " QUOTED_S, spat);
	Con_PrintLinef ("gamedironly %d" , do_gamedironly);

	fssearch_t *t = FS_Search (spat, fs_caseless_true, fs_quiet_true, fs_pakfile_null, do_gamedironly);

	if (!t) Con_PrintLinef ("No search");

	if (t) {
		for (int idx = 0; idx < t->numfilenames; idx ++) {
			char *sxy = t->filenames[idx];
			Con_PrintLinef ("%4d " QUOTED_S, idx, sxy);
			//Add_SaveFile (sxy); // Get_SavesListGameDirOnly
		} // for
	} // t

	FS_FreeSearch_Null_ (t);
}


#ifdef CONFIG_MENU





// Baker: Make this do any color configuration.
void FS_ColorPcts_f (cmd_state_t *cmd)
{
	ccs *s_color = cmd_argv_parm1;
	char s_color_hex[12] = "0x"; // #333

	// 0x8f3933  0xRRGGBB
	// Baker: WATCH OUT THESE 2 rewrite s_color and s_colorhex
	if (String_Starts_With (s_color, "#") && strlen(s_color) == 4) {
		s_color_hex[2] = s_color[1];
		s_color_hex[3] = s_color[1];
		s_color_hex[4] = s_color[2];
		s_color_hex[5] = s_color[2];
		s_color_hex[6] = s_color[3];
		s_color_hex[7] = s_color[3];
		s_color_hex[8] = 0; // Null term
		s_color = s_color_hex;
		Con_PrintLinef ("s_color_hex = " QUOTED_S, s_color_hex);
		// And let "0x" catch it
	}

	else if (String_Starts_With (s_color, "#") && strlen(s_color) == 7) {
		c_strlcat (s_color_hex, &s_color[1]);
		s_color = s_color_hex;
		Con_PrintLinef ("s_color_hex = " QUOTED_S, s_color_hex);
		// And let "0x" catch it
	}


	if (String_Starts_With (s_color, "0x")) {
		// Convert the hexadecimal string to a long long integer
		int64_t ourhex64 = STRTOI64_FN (s_color, /*end pointer*/ NULL, HEX_BASE_16);   // aka strtoll str to long long
		//int is_okay = (errno == 0); // ERANGE is the usual bad value according to docs, means can't be represented with size available.

        int red =   ((ourhex64>>16) & 0xff);
		int green = ((ourhex64>> 8) & 0xff);
		int blue  = ((ourhex64>> 0) & 0xff);

		vec3_t v = {red, green, blue};
		char *s_color_escape_za = Color_Code_ZAlloc_Or_Null(v);

		if (s_color_escape_za) {
			Con_PrintLinef ("(%sCOLOR" CON_WHITE")", s_color_escape_za);
		}

		Con_PrintLinef ("red: %d green: %d blue: %d", red, green, blue);

		float r = red / 255.0;
		float g = green / 255.0;
		float b = blue / 255.0 ;

		va_super (s, 1024, "%4.2f %4.2f %4.2f", r, g, b);

		Clipboard_Set_Text (s); // colorpcts cmd

		Con_PrintLinef ("%s", s);
		Con_PrintLinef ("Copied to clipboard");

		Mem_FreeNull_ (s_color_escape_za);
		return;
	}

	if (Cmd_Argc(cmd) != 1 + 3) {
		Con_PrintLinef ("Need a number");
		return;
	}

	vec3_t v = { atof(Cmd_Argv(cmd, 1)) , atof(Cmd_Argv(cmd, 2)), atof(Cmd_Argv(cmd, 3)) };

	int is_percent_color = in_range(0, v[0], 1) && in_range(0, v[1], 1) && in_range(0, v[2], 1);
	int is_255_color = in_range(0, v[0], 255) && in_range(0, v[1], 255) && in_range(0, v[2], 255);

	char *s_color_escape_za = Color_Code_ZAlloc_Or_Null(v);

	if (s_color_escape_za) {
		Con_PrintLinef ("(%sCOLOR" CON_WHITE")", s_color_escape_za);
	}

	if (is_percent_color) {
		float r = v[0] * 255.0;
		float g = v[1] * 255.0;
		float b = v[2] * 255.0;
		Con_PrintLinef ("Color 0-255: %d %d %d", (int)r, (int)g, (int)b );
		Con_PrintLinef ("Hex        : %x %x %x", (int)r, (int)g, (int)b );
	} else if (is_255_color) {
		float rhex = v[0];
		float bhex = v[1];
		float ghex = v[2];
		float r = v[0] / 255.0;
		float g = v[1] / 255.0;
		float b = v[2] / 255.0 ;

		Con_PrintLinef ("Color 0-255: %d %d %d", (int)rhex, (int)bhex, (int)ghex );
		Con_PrintLinef ("Hex        : %x %x %x", (int)rhex, (int)bhex, (int)ghex );
		va_super (s, 1024, "%4.2f %4.2f %4.2f", r, g, b);
		Clipboard_Set_Text (s); // colorpcts cmd
		Con_PrintLinef ("%s", s);
		Con_PrintLinef ("Copied to clipboard");
	}
	Mem_FreeNull_ (s_color_escape_za);
}

typedef struct {
	ccs *s; uint64_t val;
} valpair_s;

static valpair_s fl_flags_pairs[] = { // bitflag
	{ "FL_FLY_1",			1 },
	{ "FL_SWIM_2",			2 },
	{ "FL_CONVEYOR",		4 },
	{ "FL_CLIENT_8",		8 },
	{ "FL_INWATER",			16 },
	{ "FL_MONSTER_32",		32 },
	{ "FL_GODMODE",			64 },
	{ "FL_NOTARGET",		128 },
	{ "FL_ITEM_256",		256 },
	{ "FL_ONGROUND_512",	512 },
	{ "FL_PARTIALGROUND",	1024 },
	{ "FL_WATERJUMP",		2048 },
	{ "FL_JUMPRELEASED",	4096 },
	{ NULL }
};

static valpair_s dpcontents_flags_pairs[] = { // bitflag
	{ "DPCONTENTS_SOLID",		1 },
	{ "DPCONTENTS_WATER",		2 },
	{ "DPCONTENTS_SLIME",		4 },
	{ "DPCONTENTS_LAVA",		8 },
	{ "DPCONTENTS_SKY",			16 },
	{ "DPCONTENTS_BODY",		32 },
	{ "DPCONTENTS_CORPSE",		64 },
	{ "DPCONTENTS_NODROP",		128 }, // an area where backpacks should not spawn
	{ "DPCONTENTS_PLAYERCLIP",	256 },
	{ "DPCONTENTS_MONSTERCLIP", 512 },
	{ "DPCONTENTS_DONOTENTER",	1024 },
	{ "DPCONTENTS_BOTCLIP",		2048 },
	{ "DPCONTENTS_OPAQUE",		4096 },
	{ NULL }
};


static valpair_s content_pairs[] = { // linear
	{ "CONTENT_EMPTY",	-1 },
	{ "CONTENT_SOLID",	-2 },
	{ "CONTENT_WATER",	-3 },
	{ "CONTENT_SLIME",	-4 },
	{ "CONTENT_LAVA",	-5 },
	{ "CONTENT_SKY",	-6 },
	{ NULL }
};

static valpair_s sold_pairs[] = { // linear
	{ "SOLID_NOT_0" ,		0 },
	{ "SOLID_TRIGGER_1",	1 },
	{ "SOLID_BBOX_2",		2 },
	{ "SOLID_SLIDEBOX_3",	3 },
	{ "SOLID_BSP_4",		4 },
	{ NULL }
};

static valpair_s modelflags_flags_pairs[] = { // bitflag
	{ "MF_ROCKET",			1 },
	{ "MF_GRENADE",			2 },
	{ "MF_GIB",				4 },
	{ "MF_ROTATE",			8 },
	{ "MF_TRACER",			16 },
	{ "MF_TRACER2",			32 },
	{ "MF_TRACER3",			64 },
	{ NULL }
};


static valpair_s movetype_pairs[] = { // linear
	{"MOVETYPE_NONE_0",				0},		///< never moves
	{"MOVETYPE_ANGLENOCLIP_1",		1},
	{"MOVETYPE_ANGLECLIP_2",		2},
	{"MOVETYPE_WALK_3",				3},		///< gravity
	{"MOVETYPE_STEP_4",				4},		///< gravity, special edge handling
	{"MOVETYPE_FLY_5",				5},
	{"MOVETYPE_TOSS_6",				6},		///< gravity
	{"MOVETYPE_PUSH_7",				7},		///< no clip to world, push and crush
	{"MOVETYPE_NOCLIP_8",			8},
	{"MOVETYPE_FLYMISSILE_9",		9},		///< extra size to monsters
	{"MOVETYPE_BOUNCE_10",			10},
	{"MOVETYPE_BOUNCEMISSILE_11",	11},	///< bounce w/o gravity
	{"MOVETYPE_FOLLOW_12",			12},	///< track movement of aiment
	{"MOVETYPE_FAKEPUSH_13",		13},	///< tenebrae's push that doesn't push
	{"MOVETYPE_PHYSICS_32",			32},	///< indicates this object is physics controlled
	{"MOVETYPE_FLY_WORLDONLY_33",	33},	///< like MOVETYPE_FLY_5, but uses MOVE_WORLDONLY_3 for all its traces; objects of this movetype better be SOLID_NOT_0 or SOLID_TRIGGER_1 please, or else...
	{ NULL }
};

static valpair_s surf_flags_pairs[] = { // bitflag
	{ "Q3SURFACEFLAG_NODAMAGE",		1 },
	{ "Q3SURFACEFLAG_SLICK",		2 }, // low friction surface
	{ "Q3SURFACEFLAG_SKY",			4 }, // sky surface (also has NOIMPACT and NOMARKS set)
	{ "Q3SURFACEFLAG_LADDER",		8 }, // climbable surface
	{ "Q3SURFACEFLAG_NOIMPACT",		16 }, // projectiles should remove themselves on impact (this is set on sky)
	{ "Q3SURFACEFLAG_NOMARKS",		32 }, // projectiles should not leave marks, such as decals (this is set on sky)
	{ "Q3SURFACEFLAG_FLESH",		64 }, // projectiles should do a fleshy effect (blood?) on impact
	{ "Q3SURFACEFLAG_NODRAW",		128 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_HINT",			256 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_SKIP",			512 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_NOLIGHTMAP",	1024 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_POINTLIGHT",	2048 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_METALSTEPS",	4096 }, // walking on this surface should make metal step sounds
	{ "Q3SURFACEFLAG_NOSTEPS",		8192 }, // walking on this surface should not make footstep sounds
	{ "Q3SURFACEFLAG_NONSOLID",		16384 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_LIGHTFILTER",	32768 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_ALPHASHADOW",	65536 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_NODLIGHT",		131072 }, // compiler hint (not important to qc)
	{ "Q3SURFACEFLAG_DUST",			262144 }, // translucent 'light beam' effect (not important to qc)
	{ NULL }
};

static valpair_s ef_flags_pairs[] = { // bitflag
	{ "EF_BRIGHTFIELD_1",		1 },
	{ "EF_MUZZLEFLASH_2", 		2 },
	{ "EF_BRIGHTLIGHT_4", 		4 },
	{ "EF_DIMLIGHT_8", 			8 },
	{ "EF_NODRAW_16",			16 },			// EF_QEX_QUADLIGHT FIGHTS	EF_NODRAW_16	// AURA 7.0
	{ "EF_ADDITIVE_32",			32 },
	{ "EF_BLUE_64",				64 },
	{ "EF_RED_128",				128 },
	{ "EF_NOGUNBOB",			256 },
	{ "EF_FULLBRIGHT",			512 },
	{ "EF_FLAME",				1024 },
	{ "EF_STARDUST",			2048 },
	{ "EF_NOSHADOW",			4096 },
	{ "EF_NODEPTHTEST",			8192 },
	{ "EF_SELECTABLE",			16384},		// 14	LadyHavoc: highlights when PRYDON_CLIENTCURSOR mouse is over it
	{ "EF_DOUBLESIDED",			32768},		// 15	[515]: disable cull face for this entity
	{ "EF_NOSELFSHADOW",		65536},		// 16	LadyHavoc: does not cast a shadow on itself (or any other EF_NOSELFSHADOW entities)
	{ "EF_DYNAMICMODELLIGHT",	131072},		// 17
	{ "EF_UNUSED18",			262144},		// 18
	{ "EF_UNUSED19",			524288},		// 19
	{ "EF_RESTARTANIM_BIT",		1048576},     // 20	div0: restart animation bit (like teleport bit, but lerps between end and start of the anim, and doesn't stop player lerping)
	{ "EF_TELEPORT_BIT",		2097152},		// 21	div0: teleport bit (toggled when teleporting, prevents lerping when the bit has changed)
	{ "EF_LOWPRECISION",		4194304},		// 22	LadyHavoc: entity is low precision (integer coordinates) to save network bandwidth  (serverside only)
	{ "EF_NOMODELFLAGS",		8388608},		// 23	indicates the model's .effects should be ignored (allows overriding them)
	{ "EF_ROCKET",				16777216},	// 24	leave a trail
	{ "EF_GRENADE",				33554432},	// 25	leave a trail
	{ "EF_GIB",					67108864},	// 26	leave a trail
	{ "EF_ROTATE",				134217728},	// 27	rotate (bonus items)
	{ "EF_TRACER",				268435456},	// 28	green split trail
	{ "EF_ZOMGIB",				536870912},	// 29	small blood trail
	{ "EF_TRACER2",				1073741824},	// 30	orange split trail + rotate
	{ "EF_TRACER3",				0x80000000},	// 31	purple trail 2 147 483 648
	{ NULL }
};

static valpair_s renderflags_flags_pairs[] = { // bitflag
	{ "RF_VIEWMODEL",				1 },
	{ "RF_EXTERNALMODEL",			2 },
	{ "RF_DEPTHHACK",				4 },
	{ "RF_ADDITIVE",				8 },
	{ "RF_USEAXIS",					16 },
	//{ "RF_DOUBLESIDED",			32 }, // Baker ?
	{ "RF_USETRANSPARENTOFFSET",	64  },   // Allows QC to customize origin used for transparent sorting via transparent_origin global, helps to fix transparent sorting bugs on a very large entities
	{ "RF_WORLDOBJECT",				128  },  // for large outdoor entities that should not be culled
	{ "RF_FULLBRIGHT",				256  },
	{ "RF_NOSHADOW",				512  },
	{ "RF_MODELLIGHT",				4096 }, // CSQC-set model light
	{ "RF_DYNAMICMODELLIGHT",		8192 }, // origin-dependent model light
	{ NULL }
};

static valpair_s spawnflags_flags_pairs[] = { // bitflag
	{ "SPAWNFLAG_NOT_EASY",			256  },
	{ "SPAWNFLAG_NOT_MEDIUM",		512  },
	{ "SPAWNFLAG_NOT_HARD",			1024 },
	{ "SPAWNFLAG_NOT_DEATHMATCH",	2048 },
	{ NULL }
};

void FS_Autocvars_From_Clip (cmd_state_t *cmd)
{
	ccs *s_za = Clipboard_Get_Text_ZAlloc ();
	CMD_RETURN_IF_ (!s_za, "No text on clipboard")



	stringlist_t list = {0};
	stringlistappend_split_lines_cr_scrub (&list, s_za);
	stringlist_t listvars = {0};
	Con_PrintLinef ("Evaluating autocvar_ where line starts with \"var\" (SPACE or TAB)" NEWLINE
		"and contains _autocvar and any comments after // becomes the description");

	for (int sidx = 0; sidx < list.numstrings; sidx ++) {
		ccs *sxy = list.strings[sidx];
		char autocvarname[256] = {0};
		char description[256] = {0};
		if (false == (String_Starts_With (sxy, "var ") || String_Starts_With (sxy, "var " TABBER)))
			continue; // We expect it to start with var
		ccs *sa = strstr (sxy, "autocvar_");
		if (!sa)
			continue;
		Com_Parse_String (sa);
		c_strlcpy (autocvarname, com_token);
		ccs *scom = strstr (sxy, "// ");
		if (scom)
			c_strlcpy (description, scom + STRINGLEN("// "));
		stringlistappendf ( &listvars, TABBER "mod_register_cvar (" QUOTED_S ", " QUOTED_S ");", autocvarname,description);
		Con_PrintLinef (listvars.strings[listvars.numstrings - 1] );
	} // For

	stringlistsort_no_unique (&listvars);
	char *sza2 = stringlist_join_lines_zalloc (&listvars);
	Clipboard_Set_Text (sza2);
	Mem_FreeNull_ (sza2);
	Con_PrintLinef ("Text set to clipboard");

	stringlistfreecontents (&listvars);
	stringlistfreecontents (&list);


	Mem_FreeNull_ (s_za);

}

void FS_Constants_f (cmd_state_t *cmd)
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<flags | solids | effects>", "Lists constants -- also try bitatomize 513 flags");
	valpair_s *p_pairs = NULL;

	ccs *stable = cmd_argv_parm1;

		 if (String_Match (stable, "contentflags_dp"))	p_pairs = &dpcontents_flags_pairs[0];
	else if (String_Match (stable, "contents"))			p_pairs = &content_pairs[0];
	else if (String_Match (stable, "effects"))			p_pairs = &ef_flags_pairs[0];
	else if (String_Match (stable, "flags"))			p_pairs = &fl_flags_pairs[0];
	
	else if (String_Match (stable, "modelflags"))		p_pairs = &modelflags_flags_pairs[0];
	else if (String_Match (stable, "movetypes"))		p_pairs = &movetype_pairs[0];
	else if (String_Match (stable, "renderflags"))		p_pairs = &renderflags_flags_pairs[0];
	else if (String_Match (stable, "solids"))			p_pairs = &sold_pairs[0];
	else if (String_Match (stable, "spawnflags"))		p_pairs = &spawnflags_flags_pairs[0];
	else if (String_Match (stable, "surfaceflags"))		p_pairs = &surf_flags_pairs[0];
	else {
		Con_PrintLinef ("Unknown constant table " QUOTED_S, stable);
		return;
	}

	if (!p_pairs)
		return;
	for (int idx = 0; ; idx ++) {
		valpair_s *mypair = &p_pairs[idx];
		if (!mypair->s)
			break;

		Con_PrintLinef ("%2d " S_FMT_LEFT_PAD_40 " %16" PRId64,
			idx, mypair->s, (uint64_t)mypair->val
		);

	} // for

}

valpair_s *Pair_Entry_For_Value (valpair_s *p_pairs, uint64_t uresult)
{
	if (!p_pairs) return NULL;
	if (!uresult) return NULL; // 0 can't hit anything
	//int index = 0;
	for (int idx = 0; ; idx ++) {
		valpair_s *mypair = &p_pairs[idx];
		if (!mypair->s)
			return NULL;
		if (mypair->val == uresult)
			return mypair;
	}
	return NULL; // unreachable if we did this right
}

RELATED_ (Edict_Value_Fixup_Did_Change)
void FS_BitAtomize_f (cmd_state_t *cmd)
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<number> [optional flag table]", "Takes integer and displays bit values")

	valpair_s *p_pairs = NULL;
	if (cmd_argc >= 3) {
		ccs *stable = cmd_argv_parm2;
		     if (String_Match (stable, "contentflags_dp"))	p_pairs = &dpcontents_flags_pairs[0];
		else if (String_Match (stable, "effects"))			p_pairs = &ef_flags_pairs[0];
		else if (String_Match (stable, "flags"))			p_pairs = &fl_flags_pairs[0];
		else if (String_Match (stable, "renderflags"))		p_pairs = &renderflags_flags_pairs[0];
		else if (String_Match (stable, "spawnflags"))		p_pairs = &spawnflags_flags_pairs[0];
		else if (String_Match (stable, "surfaceflags"))		p_pairs = &surf_flags_pairs[0];
	}

	double dnumber = atof(cmd_argv_parm1);

	uint64_t number = dnumber;

	Con_PrintLinef ("Number: %" PRId64, (int64_t) number);

	int highest = 0;
	for (uint64_t j = 0; j < 64; j ++) {
		uint64_t miney = (uint64_t)1 << j;
		//Con_PrintLinef ("%02d %16.0f %16.0f %x", (int)j, (double)(miney), (double)(number & miney), (unsigned)(number & miney));
		//ccs *sfmt = "%02d %16" PRId64 " %16" PRId64 " %" PRIu64; // Baker: <<========= Sheesh.  Fucking Microsoft.
		uint64_t uresult = (uint64_t)(number & miney);
		if (uresult !=0) highest = j;
	} // for

	// bust down to bits and then print
	int count = highest + 1;
	for (uint64_t j = 0; j < (uint64_t)count; j ++) {
		uint64_t miney = (uint64_t)1 << j;
		uint64_t uresult = (uint64_t)(number & miney);
		int64_t iresult = (int64_t)(number & miney);
		qbool is_hit = uresult != 0;
		//Con_PrintLinef ("%02d %16.0f %16.0f %x", (int)j, (double)(miney), (double)(number & miney), (unsigned)(number & miney));
		ccs *sfmt = "%2d %16" PRId64 " %s %16" PRId64 " %16" PRIu64 " %s"; // Baker: <<========= Sheesh.  Fucking Microsoft.
		ccs *scolor = is_hit ? CON_YELLOW : "";

		valpair_s *p_entry = Pair_Entry_For_Value (p_pairs, uresult);
		ccs *sresult = "";
		if (p_entry)
			sresult = p_entry->s;
		Con_PrintLinef (sfmt, (int)j, (int64_t)miney, scolor, iresult, uresult, sresult);
	} // for
}


void FS_Repeat_f (cmd_state_t *cmd)
{
	if (cmd_argc != (1 + 2)) {
		Con_PrintLinef ("usage:" NEWLINE "%s <alias> <number of times>", cmd_argv_command_0);
		return;
	}

	ccs *alias		= cmd_argv_parm1;
	ccs *snumtimes	= cmd_argv_parm2;
	int numtimes	= atoi(snumtimes);
	if (!in_range(1, numtimes, 9999)) {
		Con_PrintLinef ("numtimes not in range 1-9999 = %d", numtimes);
		return;
	}

	for (int n = 0; n < numtimes; n ++) {
		Cbuf_AddTextLinef (cmd, "%s", alias);
	}
}

#ifdef _DEBUG
//textures/common/alpha_($name_float)
//{
//	q3map_alphaMod volume
//	q3map_alphaMod set ($decimal_float)
//	surfaceparm nodraw
//	surfaceparm nonsolid
//	surfaceparm trans
//	qer_trans 0.75
//}
void FS_D_Printf_f (cmd_state_t *cmd) // d_printf %g n
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (2, "<filenameout> <start> <end> <iter>", "Custom file write");

	ccs *fmt		= cmd_argv_parm1;
	ccs *sflot		= cmd_argv_parm2;
	float flot		= atof (sflot);

	Con_PrintVarString	(fmt);
	Con_PrintVarFloat	(flot);

	Con_PrintLinef (fmt, flot); // DANGEROUS
}

void FS_D_Repeat_f (cmd_state_t *cmd)
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (4, "<filenameout> <start> <end> <iter>", "Custom file write");
	qfile_t *f = NULL;

	// textures\common .. where the icons live.
	// "C:\galaxy\zircon\    scripts/common2.shader"
	ccs *filename	= cmd_argv_parm1;
	ccs *start		= cmd_argv_parm2;
	ccs *end		= cmd_argv_parm3;
	ccs *iter		= cmd_argv_parm4;

	float fstart	= atof(start);
	float fend		= atof(end);
	float fiter		= atof(iter);

	Con_PrintVarString (filename);

	ccs *stemplatefilename = "scripts/common2.txt";
	char *template1_za = FS_LoadFileQuick (stemplatefilename);
	CMD_FAILEXIT_IF2_ (!template1_za, "Couldn't load file " QUOTED_S, stemplatefilename);

	size_t template1_slen = strlen (template1_za);

	f = FS_OpenRealFile (filename, "wb", fs_quiet_FALSE);
	CMD_FAILEXIT_IF2_ (!f, "Couldn't open file for writing " QUOTED_S, filename);

	for (float flot = fstart; flot <= fend; flot += fiter) {
		char *sxy_za = Z_StrDup (template1_za);
		//"common/alpha_%1.2f"
		if (flot == 0) flot = 0; // Get rid of negative zero issue.
		va_super (sfloat_num, 128, "%1.2f", flot); // 0.25
		va_super (sfloat_str, 128, "%1.2f", flot); // 0.25
		String_Edit_Replace (sfloat_str, sizeof(sfloat_str), ".", "_"); // 0_25

		String_Edit_Replace (sxy_za, template1_slen + ONE_CHAR_1, "($name_float)", sfloat_str); // 0_25
		String_Edit_Replace (sxy_za, template1_slen + ONE_CHAR_1, "($decimal_float)", sfloat_num); // 0_25

		FS_Print (f, sxy_za);

		Mem_FreeNull_ (sxy_za);
	} // row


fail_exit:
	FS_CloseNULL_ (f);
	Mem_FreeNull_ (template1_za);

}
#endif // _DEBUG



void FS_Divisors_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) != 2) {
		Con_PrintLinef ("Need a number");
		return;
	}

	double number = (int64_t)atof(Cmd_Argv(cmd,1));
	double root = floor(sqrt(number));

	int count = 0;
	for (double j = 2; j <= root; j ++) {
		double result = number / j;
		if (Math_IsWholeNumber(result) == false)
			continue;
		Con_PrintLinef ("%03d: %g / %g = %g", count, number, j, result);
		count ++;
	}

}

#if 1// def SHELLENABLE

RELATED_ (PRVM_ED_Eset_f)
// Need some sort of listener for completed events.
// Will need to be able to deal with disconnect, gamedir change, etc.
// Ideally multiple processes?  Later.

sys_handle_t oldprocess = NULL;
void FS_Shell_NonBlocking_f (cmd_state_t *cmd)
{
	if (oldprocess) {
		int exit_code;
		Con_PrintLinef ("We have a process from last time %p...", oldprocess);
		int result = System_Process_Is_Still_Running_Neg1_Error(oldprocess, &exit_code);

		switch (result) {
		case not_found_neg1:	Con_PrintLinef ("Process not found.  Clearing."); oldprocess = NULL; break;
		case true:				Con_PrintLinef ("Process still running ..."); break;
		case false:				Con_PrintLinef ("Process completed (exit code = %d).  Clearing.", exit_code); oldprocess = NULL; break;
		}
		return;
	}

	if (Cmd_Argc (cmd) < 2) {
		Con_PrintLinef ("shell <command line> <optional args>");
		return;
	}

	ccs *s_executable = Cmd_Argv(cmd, 1);

	// shell 0
	// exe   1
	// args  2+
	char s_args_all[1024];
	Argv_Cumulate (cmd, 2, s_args_all, sizeof(s_args_all) );

	//int is_ok = Sys_ShellExecute_NonBlocking (s_exec, s_args_all);
	oldprocess = System_Process_Create (s_executable, s_args_all, fs_workingdir_null);

	Con_PrintLinef ("Result ok = %p", oldprocess);
}

void FS_Shell_Blocking_f (cmd_state_t *cmd)
{
	if (Cmd_Argc (cmd) < 2) {
		Con_PrintLinef ("shell_wait <command line> <optional args>");
		return;
	}

	ccs *s_executable = Cmd_Argv(cmd, 1);

	char s_args_all[1024];
	Argv_Cumulate (cmd, 2, s_args_all, sizeof(s_args_all) );

	int is_ok = Sys_ShellExecute_Wait (s_executable, s_args_all, fs_workingdir_null);
	Con_PrintLinef ("Result ok = %d", is_ok);
}

#endif

void FS_Parse_f (cmd_state_t *cmd)
{
	if (Cmd_Argc (cmd) < 2) {
		Con_PrintLinef ("parse <string> or parse clipboard");
		return;
	}

	const char *s_parse = NULL;
	char *s_zalloc = NULL;
	if (Cmd_Argc (cmd) == 2 && String_Match (Cmd_Argv(cmd, 1), "clipboard")) {
		s_zalloc = Sys_Clipboard_Get_Text_ZAlloc(); // zallocs
		s_parse = s_zalloc;

	} else {
		s_parse = Cmd_Argv (cmd, 1);
	}

	const char		*data = s_parse;

	RELATED_ (String_Worldspawn_Value_For_Key_Sbuf)
		int idx = 0;
	while (1) {
		// Read some data ...
		if (COM_Parse_Basic(&data) == false)
			return; // End of data

		// Copy data over, skipping a prefix of '_' in a keyname
		const char *s_token = &com_token[0];

		Con_PrintLinef ("%4d: " QUOTED_S, idx, s_token);
		idx ++;
	} // while


	Mem_FreeNull_ (s_zalloc);
}

void FS_Base64Clipboard_f (cmd_state_t *cmd)
{
	char *s_zalloc = Sys_Clipboard_Get_Text_ZAlloc(); // zallocs
	if (s_zalloc == NULL) {
		Con_PrintLinef ("No text on clipboard");
		return;
	}

	char *s_base64_alloc = base64_encode_calloc ((byte *)s_zalloc, strlen(s_zalloc), q_reply_len_NULL); // malloc

	Clipboard_Set_Text (s_base64_alloc); // AUTH: base64clipboard cmd
	Con_PrintLinef ("Set to clipboard %u characters in --> base64 %u characters", (unsigned)strlen(s_zalloc), (unsigned)strlen(s_base64_alloc));

	free (s_base64_alloc);
	Mem_FreeNull_ (s_zalloc);
}

char *Compresso (char *s);
void FS_Base64ClipboardCompressed_f (cmd_state_t *cmd)
{
	char *s_zalloc = Clipboard_Get_Text_ZAlloc ();
	if (s_zalloc == NULL) {
		Con_PrintLinef ("No text on clipboard");
		return;
	}

	// Baker: We are compressing text here, the OUTPUT is binary
	size_t data_compressed_size = 0;
	byte *data_compressed = string_zlib_compress_malloc (s_zalloc, &data_compressed_size, SIV_DECOMPRESS_BUFSIZE_16_MB);

	char *s_base64_alloc = base64_encode_calloc (data_compressed, data_compressed_size, q_reply_len_NULL); // malloc

	Clipboard_Set_Text (s_base64_alloc); // AUTH: base64clipboardcompress

	Con_PrintLinef ("Set to clipboard %" PRId64 " characters in --> base64 %" PRId64
		" characters",
		(int64_t)strlen(s_zalloc), (int64_t)strlen(s_base64_alloc));

	free (s_base64_alloc);
	free (data_compressed);
	Mem_FreeNull_ (s_zalloc);


}

void FS_Base64ClipboardDeCompressed_f (cmd_state_t *cmd)
{
	char *s_zalloc = Clipboard_Get_Text_ZAlloc();
	if (s_zalloc == NULL) {
		Con_PrintLinef ("No text on clipboard");
		return;
	}

	//size_t slen = strlen(s_alloc);
	size_t unbase_datasize;
	byte *data_unbase64_alloc = base64_decode_calloc (s_zalloc, &unbase_datasize); // malloc

	// It is now ready for zip decompression
	char *s_data_uncompressed = string_zlib_decompress_malloc (data_unbase64_alloc,
		unbase_datasize, SIV_DECOMPRESS_BUFSIZE_16_MB);

	Clipboard_Set_Text (s_data_uncompressed); // AUTH: base64clipboarddecompress

	Con_PrintLinef ("Set to clipboard %" PRId64
		" characters in --> base64 %" PRId64 " characters",
		(int64_t)strlen(s_zalloc), (int64_t)strlen(s_data_uncompressed));

	free (s_data_uncompressed);
	free (data_unbase64_alloc);
	Mem_FreeNull_ (s_zalloc);


}
#endif


// vegetation make ... do path aware png/tga/jpg
// It is not our responsibility to make the image
// Nor the shader.
// However print out a suggested share for the applicable one.

//

RELATED_ (FS_JpegSplit_f, SCR_gifclip_f)



void FS_Mapgenmask_f (cmd_state_t *cmd)
{
	// -3520 2560 ... 4096 4098
	if (Cmd_Argc(cmd) < 1 + 6) {
		Con_PrintLinef ("usage:" NEWLINE "%s <mask> <x> <y> <w> <h> <scale32>", Cmd_Argv(cmd, 0));
		return;
	}

//	ccs *s_mask		= Cmd_Argv(cmd, 1);
//	ccs *s_x		= Cmd_Argv(cmd, 2);		float x		= atof(s_x);
//	ccs *s_y		= Cmd_Argv(cmd, 3);		float y		= atof(s_y);
//	ccs *s_w		= Cmd_Argv(cmd, 4);		float w		= atof(s_w);
//	ccs *s_h		= Cmd_Argv(cmd, 5);		float h		= atof(s_h);
//	ccs *s_scale	= Cmd_Argv(cmd, 6);		float sc	= atof(s_scale);
//
//	byte *s_mask = loadimagepixelsbgra (
//		s_name,
//		q_tx_complain_false,
//		q_tx_allowfixtrans_false,
//		q_tx_convertsrgb_false,
//		q_tx_miplevel_null
//	);
//
//	Mem_Free (data_bgra); // Baker: it's temppool so ok
//
}

#if 0 // ICED
void FS_Mapgenpoints_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) < 1 + 7) {
		Con_PrintLinef ("usage:" NEWLINE "%s <delta> <x1> <y1> <z1> <x1> <y1> <z1>", Cmd_Argv(cmd, 0));
		return;
	}

	ccs *s_span = Cmd_Argv(cmd, 1);	float span = atof(s_span);
	if (span < 32)
		span = 32;

	float spandiv2 = span / 2;
	//char *x1 = Cmd_Argv(cmd, 2), *y1 = Cmd_Argv(cmd, 3 ), z1 = Cmd_Argv(cmd, 4);
	//char *x2 = Cmd_Argv(cmd, 2), *y2 = Cmd_Argv(cmd, 3 ), z2 = Cmd_Argv(cmd, 4);
//-2426 -1168 0

//-2560, -384 to -2048

	baker_string_t *bs_mapblock = BakerString_Create_Malloc ("");
	ccs *s_mapblock =//	"{" NEWLINE
			"{" NEWLINE
			"( XL YL ZH ) ( XL YL ZL ) ( XL YH ZH ) terrain_example/alpha_000 [ 0 -1 0 5.99988 ] [ 0 0 -1 -32 ] 0 0.5 0.5 536870912 16512 0" NEWLINE
			"( XH YH ZH ) ( XH YH ZL ) ( XH YL ZH ) terrain_example/alpha_000 [ 0 -1 0 5.99988 ] [ 0 0 -1 -32 ] 0 0.5 0.5 536870912 16512 0" NEWLINE
			"( XH YH ZH ) ( XL YH ZH ) ( XH YH ZL ) terrain_example/alpha_000 [ 1 0 0 -7.38379 ] [ 0 0 -1 -32 ] 0 0.5 0.5 536870912 16512 0" NEWLINE
			"( XH YL ZL ) ( XL YL ZL ) ( XH YL ZH ) terrain_example/alpha_000 [ 1 0 0 -7.38379 ] [ 0 0 -1 -32 ] 0 0.5 0.5 536870912 16512 0" NEWLINE
			"( XH YH ZH ) ( XH YL ZH ) ( XL YH ZH ) terrain_example/alpha_000 [ 1 0 0 -7.38379 ] [ -0 1 0 -5.99988 ] 0 0.5 0.5 536870912 16512 0" NEWLINE
			"( XH YL ZL ) ( XH YH ZL ) ( XL YL ZL ) terrain_example/alpha_000 [ 1 0 0 -7.38379 ] [ -0 1 0 -5.99988 ] 0 0.5 0.5 536870912 16512 0" NEWLINE
			"}" NEWLINE;



	// x -2560 to -2048
		//vec3_t p0 = { -2560, -384, 0}; // to -2048}
		//vec3_t p1 = { -2048, -384, 0};
		vec3_t px = { -1, -384, 0};
	for (float j = -2560; j <= -2048  /*to -2048*/; j += span) {
		px[0] = j;
		vec3_t v1= {px[0]-spandiv2, px[1]-spandiv2, px[2]-spandiv2 };
		vec3_t v2= {px[0]+spandiv2, px[1]+spandiv2, px[2]+spandiv2 };


		char sx1[32], sy1[32], sz1[32];
		char sx2[32], sy2[32], sz2[32];
		c_dpsnprintf1(sx1, FLOAT_LOSSLESS_FORMAT, v1[0]);
		c_dpsnprintf1(sy1, FLOAT_LOSSLESS_FORMAT, v1[1]);
		c_dpsnprintf1(sz1, FLOAT_LOSSLESS_FORMAT, v1[2]);
		c_dpsnprintf1(sx2, FLOAT_LOSSLESS_FORMAT, v2[0]);
		c_dpsnprintf1(sy2, FLOAT_LOSSLESS_FORMAT, v2[1]);
		c_dpsnprintf1(sz2, FLOAT_LOSSLESS_FORMAT, v2[2]);

		size_t sz8192 = 8192;
		char *s_za = (char *)Mem_Alloc (tempmempool, sz8192);

		strlcpy (s_za, s_mapblock, sz8192);
		String_Edit_Replace (s_za, sz8192, "XL", sx1);
		String_Edit_Replace (s_za, sz8192, "YL", sy1);
		String_Edit_Replace (s_za, sz8192, "ZL", sz1);
		String_Edit_Replace (s_za, sz8192, "XH", sx2);
		String_Edit_Replace (s_za, sz8192, "YH", sy2);
		String_Edit_Replace (s_za, sz8192, "ZH", sz2);

		BakerString_Cat_No_Collide (bs_mapblock, strlen(s_za), s_za);
		Mem_FreeNull_ (s_za);
	}
	char *sout_za = NULL;

	Clipboard_Set_Text (bs_mapblock->string); // Cmd Mapgenpoints
	Con_PrintLinef ("Data on clipboard");

	BakerString_Destroy_And_Null_It (&bs_mapblock);
	Mem_FreeNull_ (sout_za);


}

void FS_VegetationMake_f (cmd_state_t *cmd)
{
	// Need to work on this.  Which one is cross.
	// What is other one?
	ccs *vegetion_ase_text_base64 = "KjNEU01BWF9BU0NJSUVYUE9SVAkyMDANCipDT01NRU5UCSJHZW5lcmF0ZWQgYnkgUTNNYXAyICh5ZG5hcikgLWNvbnZlcnQgLWZvcm1hdCBhc2UiDQoqU0NFTkUJew0KCSpTQ0VORV9GSUxFTkFNRQkiW01PREVMXS5ic3AiDQoJKlNDRU5FX0ZJUlNURlJBTUUJMA0KCSpTQ0VORV9MQVNURlJBTUUJMTAwDQoJKlNDRU5FX0ZSQU1FU1BFRUQJMzANCgkqU0NFTkVfVElDS1NQRVJGUkFNRQkxNjANCgkqU0NFTkVfQkFDS0dST1VORF9TVEFUSUMJMAkwCTANCgkqU0NFTkVfQU1CSUVOVF9TVEFUSUMJMAkwCTANCn0NCipNQVRFUklBTF9MSVNUCXsNCgkqTUFURVJJQUxfQ09VTlQJMw0KCSpNQVRFUklBTAkwCXsNCgkJKk1BVEVSSUFMX05BTUUJInRleHR1cmVzL2NvbW1vbi9vcmlnaW4iDQoJCSpNQVRFUklBTF9DTEFTUwkiU3RhbmRhcmQiDQoJCSpNQVRFUklBTF9ESUZGVVNFCTEJMQkxDQoJCSpNQVRFUklBTF9TSEFESU5HIFBob25nDQoJCSpNQVBfRElGRlVTRQl7DQoJCQkqTUFQX05BTUUJInRleHR1cmVzL2NvbW1vbi9vcmlnaW4iDQoJCQkqTUFQX0NMQVNTCSJCaXRtYXAiDQoJCQkqTUFQX1NVQk5PCTENCgkJCSpNQVBfQU1PVU5UCTENCgkJCSpNQVBfVFlQRQlTY3JlZW4NCgkJCSpCSVRNQVAJInRleHR1cmVzXGNvbW1vblxvcmlnaW4udGdhIg0KCQkJKkJJVE1BUF9GSUxURVIJUHlyYW1pZGFsDQoJCX0NCgl9DQoJKk1BVEVSSUFMCTEJew0KCQkqTUFURVJJQUxfTkFNRQkiW1BBVEhdL1tNT0RFTF0iDQoJCSpNQVRFUklBTF9DTEFTUwkiU3RhbmRhcmQiDQoJCSpNQVRFUklBTF9ESUZGVVNFCTEJMQkxDQoJCSpNQVRFUklBTF9TSEFESU5HIFBob25nDQoJCSpNQVBfRElGRlVTRQl7DQoJCQkqTUFQX05BTUUJIltQQVRIXS9bTU9ERUxdIg0KCQkJKk1BUF9DTEFTUwkiQml0bWFwIg0KCQkJKk1BUF9TVUJOTwkxDQoJCQkqTUFQX0FNT1VOVAkxDQoJCQkqTUFQX1RZUEUJU2NyZWVuDQoJCQkqQklUTUFQCSJbUEFUSF0vW01PREVMXSINCgkJCSpCSVRNQVBfRklMVEVSCVB5cmFtaWRhbA0KCQl9DQoJfQ0KCSpNQVRFUklBTAkyCXsNCgkJKk1BVEVSSUFMX05BTUUJIltQQVRIXS9bTU9ERUxdIg0KCQkqTUFURVJJQUxfQ0xBU1MJIlN0YW5kYXJkIg0KCQkqTUFURVJJQUxfRElGRlVTRQkxCTEJMQ0KCQkqTUFURVJJQUxfU0hBRElORyBQaG9uZw0KCQkqTUFQX0RJRkZVU0UJew0KCQkJKk1BUF9OQU1FCSJbUEFUSF0vW01PREVMXSINCgkJCSpNQVBfQ0xBU1MJIkJpdG1hcCINCgkJCSpNQVBfU1VCTk8JMQ0KCQkJKk1BUF9BTU9VTlQJMQ0KCQkJKk1BUF9UWVBFCVNjcmVlbg0KCQkJKkJJVE1BUAkiW1BBVEhdL1tNT0RFTF0iDQoJCQkqQklUTUFQX0ZJTFRFUglQeXJhbWlkYWwNCgkJfQ0KCX0NCn0NCipHRU9NT0JKRUNUCXsNCgkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjIiDQoJKk5PREVfVE0Jew0KCQkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjIiDQoJCSpJTkhFUklUX1BPUwkwCTAJMA0KCQkqSU5IRVJJVF9ST1QJMAkwCTANCgkJKklOSEVSSVRfU0NMCTAJMAkwDQoJCSpUTV9ST1cwCTEJMAkwDQoJCSpUTV9ST1cxCTAJMQkwDQoJCSpUTV9ST1cyCTAJMAkxDQoJCSpUTV9ST1czCTAJMAkwDQoJCSpUTV9QT1MJMAkwCTANCgl9DQoJKk1FU0gJew0KCQkqVElNRVZBTFVFCTANCgkJKk1FU0hfTlVNVkVSVEVYCTQNCgkJKk1FU0hfTlVNRkFDRVMJMg0KCQkqQ09NTUVOVAkiU1VSRkFDRVRZUEUJTVNUX1BMQU5BUiINCgkJKk1FU0hfVkVSVEVYX0xJU1QJew0KCQkJKk1FU0hfVkVSVEVYCTAJOC4wCTguMAkwDQoJCQkqTUVTSF9WRVJURVgJMQk4LjAJLTguMAkwDQoJCQkqTUVTSF9WRVJURVgJMgk4LjAJOC4wCTE2LjANCgkJCSpNRVNIX1ZFUlRFWAkzCTguMAktOC4wCTE2LjANCgkJfQ0KCQkqTUVTSF9OT1JNQUxTCXsNCgkJCSpNRVNIX0ZBQ0VOT1JNQUwJMAkwCS0xCTANCgkJCSpNRVNIX0ZBQ0VOT1JNQUwJMQkwCS0xCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkwCTEJMAkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMQkxCTAJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTIJMQkwCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkzCTEJMAkwDQoJCX0NCgkJKk1FU0hfRkFDRV9MSVNUCXsNCgkJCSpNRVNIX0ZBQ0UJMAlBOgkwCUI6CTIJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCQkqTUVTSF9GQUNFCTEJQToJMglCOgkzCUM6CTEJQUI6CTEJQkM6CTEJQ0E6CTEJKk1FU0hfU01PT1RISU5HCTAJKk1FU0hfTVRMSUQJMA0KCQl9DQoJCSpNRVNIX05VTVRWRVJURVgJNA0KCQkqTUVTSF9UVkVSVExJU1QJew0KCQkJKk1FU0hfVFZFUlQJMAkwLjUJMAkxDQoJCQkqTUVTSF9UVkVSVAkxCS0wLjUJMAkxDQoJCQkqTUVTSF9UVkVSVAkyCTAuNQkxCTENCgkJCSpNRVNIX1RWRVJUCTMJLTAuNQkxCTENCgkJfQ0KCQkqTUVTSF9OVU1UVkZBQ0VTCTINCgkJKk1FU0hfVEZBQ0VMSVNUCXsNCgkJCSpNRVNIX1RGQUNFCTAJMAkyCTENCgkJCSpNRVNIX1RGQUNFCTEJMgkzCTENCgkJfQ0KCX0NCgkqUFJPUF9NT1RJT05CTFVSCTANCgkqUFJPUF9DQVNUU0hBRE9XCTENCgkqUFJPUF9SRUNWU0hBRE9XCTENCgkqTUFURVJJQUxfUkVGCTANCn0NCipHRU9NT0JKRUNUCXsNCgkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjMiDQoJKk5PREVfVE0Jew0KCQkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjMiDQoJCSpJTkhFUklUX1BPUwkwCTAJMA0KCQkqSU5IRVJJVF9ST1QJMAkwCTANCgkJKklOSEVSSVRfU0NMCTAJMAkwDQoJCSpUTV9ST1cwCTEJMAkwDQoJCSpUTV9ST1cxCTAJMQkwDQoJCSpUTV9ST1cyCTAJMAkxDQoJCSpUTV9ST1czCTAJMAkwDQoJCSpUTV9QT1MJMAkwCTANCgl9DQoJKk1FU0gJew0KCQkqVElNRVZBTFVFCTANCgkJKk1FU0hfTlVNVkVSVEVYCTQNCgkJKk1FU0hfTlVNRkFDRVMJMg0KCQkqQ09NTUVOVAkiU1VSRkFDRVRZUEUJTVNUX1BMQU5BUiINCgkJKk1FU0hfVkVSVEVYX0xJU1QJew0KCQkJKk1FU0hfVkVSVEVYCTAJLTguMAk4LjAJMTYuMA0KCQkJKk1FU0hfVkVSVEVYCTEJLTguMAk4LjAJMA0KCQkJKk1FU0hfVkVSVEVYCTIJOC4wCTguMAkxNi4wDQoJCQkqTUVTSF9WRVJURVgJMwk4LjAJOC4wCTANCgkJfQ0KCQkqTUVTSF9OT1JNQUxTCXsNCgkJCSpNRVNIX0ZBQ0VOT1JNQUwJMAkwCS0xCTANCgkJCSpNRVNIX0ZBQ0VOT1JNQUwJMQkwCS0xCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkwCTAJMQkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMQkwCTEJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTIJMAkxCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkzCTAJMQkwDQoJCX0NCgkJKk1FU0hfRkFDRV9MSVNUCXsNCgkJCSpNRVNIX0ZBQ0UJMAlBOgkwCUI6CTIJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCQkqTUVTSF9GQUNFCTEJQToJMglCOgkzCUM6CTEJQUI6CTEJQkM6CTEJQ0E6CTEJKk1FU0hfU01PT1RISU5HCTAJKk1FU0hfTVRMSUQJMA0KCQl9DQoJCSpNRVNIX05VTVRWRVJURVgJNA0KCQkqTUVTSF9UVkVSVExJU1QJew0KCQkJKk1FU0hfVFZFUlQJMAktMC41CTEJMQ0KCQkJKk1FU0hfVFZFUlQJMQktMC41CTAJMQ0KCQkJKk1FU0hfVFZFUlQJMgkwLjUJMQkxDQoJCQkqTUVTSF9UVkVSVAkzCTAuNQkwCTENCgkJfQ0KCQkqTUVTSF9OVU1UVkZBQ0VTCTINCgkJKk1FU0hfVEZBQ0VMSVNUCXsNCgkJCSpNRVNIX1RGQUNFCTAJMAkyCTENCgkJCSpNRVNIX1RGQUNFCTEJMgkzCTENCgkJfQ0KCX0NCgkqUFJPUF9NT1RJT05CTFVSCTANCgkqUFJPUF9DQVNUU0hBRE9XCTENCgkqUFJPUF9SRUNWU0hBRE9XCTENCgkqTUFURVJJQUxfUkVGCTANCn0NCipHRU9NT0JKRUNUCXsNCgkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjQiDQoJKk5PREVfVE0Jew0KCQkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjQiDQoJCSpJTkhFUklUX1BPUwkwCTAJMA0KCQkqSU5IRVJJVF9ST1QJMAkwCTANCgkJKklOSEVSSVRfU0NMCTAJMAkwDQoJCSpUTV9ST1cwCTEJMAkwDQoJCSpUTV9ST1cxCTAJMQkwDQoJCSpUTV9ST1cyCTAJMAkxDQoJCSpUTV9ST1czCTAJMAkwDQoJCSpUTV9QT1MJMAkwCTANCgl9DQoJKk1FU0gJew0KCQkqVElNRVZBTFVFCTANCgkJKk1FU0hfTlVNVkVSVEVYCTQNCgkJKk1FU0hfTlVNRkFDRVMJMg0KCQkqQ09NTUVOVAkiU1VSRkFDRVRZUEUJTVNUX1BMQU5BUiINCgkJKk1FU0hfVkVSVEVYX0xJU1QJew0KCQkJKk1FU0hfVkVSVEVYCTAJOC4wCS04LjAJMTYuMA0KCQkJKk1FU0hfVkVSVEVYCTEJLTguMAktOC4wCTE2LjANCgkJCSpNRVNIX1ZFUlRFWAkyCTguMAk4LjAJMTYuMA0KCQkJKk1FU0hfVkVSVEVYCTMJLTguMAk4LjAJMTYuMA0KCQl9DQoJCSpNRVNIX05PUk1BTFMJew0KCQkJKk1FU0hfRkFDRU5PUk1BTAkwCTAJLTEJMA0KCQkJKk1FU0hfRkFDRU5PUk1BTAkxCTAJLTEJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTAJMAkwCTENCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkxCTAJMAkxDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMgkwCTAJMQ0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTMJMAkwCTENCgkJfQ0KCQkqTUVTSF9GQUNFX0xJU1QJew0KCQkJKk1FU0hfRkFDRQkwCUE6CTAJQjoJMglDOgkxCUFCOgkxCUJDOgkxCUNBOgkxCSpNRVNIX1NNT09USElORwkwCSpNRVNIX01UTElECTANCgkJCSpNRVNIX0ZBQ0UJMQlBOgkyCUI6CTMJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCX0NCgkJKk1FU0hfTlVNVFZFUlRFWAk0DQoJCSpNRVNIX1RWRVJUTElTVAl7DQoJCQkqTUVTSF9UVkVSVAkwCTAuNQkwLjUJMQ0KCQkJKk1FU0hfVFZFUlQJMQktMC41CTAuNQkxDQoJCQkqTUVTSF9UVkVSVAkyCTAuNQkxLjUJMQ0KCQkJKk1FU0hfVFZFUlQJMwktMC41CTEuNQkxDQoJCX0NCgkJKk1FU0hfTlVNVFZGQUNFUwkyDQoJCSpNRVNIX1RGQUNFTElTVAl7DQoJCQkqTUVTSF9URkFDRQkwCTAJMgkxDQoJCQkqTUVTSF9URkFDRQkxCTIJMwkxDQoJCX0NCgl9DQoJKlBST1BfTU9USU9OQkxVUgkwDQoJKlBST1BfQ0FTVFNIQURPVwkxDQoJKlBST1BfUkVDVlNIQURPVwkxDQoJKk1BVEVSSUFMX1JFRgkwDQp9DQoqR0VPTU9CSkVDVAl7DQoJKk5PREVfTkFNRQkibWF0MG1vZGVsMHN1cmY1Ig0KCSpOT0RFX1RNCXsNCgkJKk5PREVfTkFNRQkibWF0MG1vZGVsMHN1cmY1Ig0KCQkqSU5IRVJJVF9QT1MJMAkwCTANCgkJKklOSEVSSVRfUk9UCTAJMAkwDQoJCSpJTkhFUklUX1NDTAkwCTAJMA0KCQkqVE1fUk9XMAkxCTAJMA0KCQkqVE1fUk9XMQkwCTEJMA0KCQkqVE1fUk9XMgkwCTAJMQ0KCQkqVE1fUk9XMwkwCTAJMA0KCQkqVE1fUE9TCTAJMAkwDQoJfQ0KCSpNRVNICXsNCgkJKlRJTUVWQUxVRQkwDQoJCSpNRVNIX05VTVZFUlRFWAk0DQoJCSpNRVNIX05VTUZBQ0VTCTINCgkJKkNPTU1FTlQJIlNVUkZBQ0VUWVBFCU1TVF9QTEFOQVIiDQoJCSpNRVNIX1ZFUlRFWF9MSVNUCXsNCgkJCSpNRVNIX1ZFUlRFWAkwCS04LjAJLTguMAkxNi4wDQoJCQkqTUVTSF9WRVJURVgJMQktOC4wCS04LjAJMA0KCQkJKk1FU0hfVkVSVEVYCTIJLTguMAk4LjAJMTYuMA0KCQkJKk1FU0hfVkVSVEVYCTMJLTguMAk4LjAJMA0KCQl9DQoJCSpNRVNIX05PUk1BTFMJew0KCQkJKk1FU0hfRkFDRU5PUk1BTAkwCTAJLTEJMA0KCQkJKk1FU0hfRkFDRU5PUk1BTAkxCTAJLTEJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTAJLTEJMAkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMQktMQkwCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkyCS0xCTAJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTMJLTEJMAkwDQoJCX0NCgkJKk1FU0hfRkFDRV9MSVNUCXsNCgkJCSpNRVNIX0ZBQ0UJMAlBOgkwCUI6CTIJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCQkqTUVTSF9GQUNFCTEJQToJMglCOgkzCUM6CTEJQUI6CTEJQkM6CTEJQ0E6CTEJKk1FU0hfU01PT1RISU5HCTAJKk1FU0hfTVRMSUQJMA0KCQl9DQoJCSpNRVNIX05VTVRWRVJURVgJNA0KCQkqTUVTSF9UVkVSVExJU1QJew0KCQkJKk1FU0hfVFZFUlQJMAktMC41CTEJMQ0KCQkJKk1FU0hfVFZFUlQJMQktMC41CTAJMQ0KCQkJKk1FU0hfVFZFUlQJMgkwLjUJMQkxDQoJCQkqTUVTSF9UVkVSVAkzCTAuNQkwCTENCgkJfQ0KCQkqTUVTSF9OVU1UVkZBQ0VTCTINCgkJKk1FU0hfVEZBQ0VMSVNUCXsNCgkJCSpNRVNIX1RGQUNFCTAJMAkyCTENCgkJCSpNRVNIX1RGQUNFCTEJMgkzCTENCgkJfQ0KCX0NCgkqUFJPUF9NT1RJT05CTFVSCTANCgkqUFJPUF9DQVNUU0hBRE9XCTENCgkqUFJPUF9SRUNWU0hBRE9XCTENCgkqTUFURVJJQUxfUkVGCTANCn0NCipHRU9NT0JKRUNUCXsNCgkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjYiDQoJKk5PREVfVE0Jew0KCQkqTk9ERV9OQU1FCSJtYXQwbW9kZWwwc3VyZjYiDQoJCSpJTkhFUklUX1BPUwkwCTAJMA0KCQkqSU5IRVJJVF9ST1QJMAkwCTANCgkJKklOSEVSSVRfU0NMCTAJMAkwDQoJCSpUTV9ST1cwCTEJMAkwDQoJCSpUTV9ST1cxCTAJMQkwDQoJCSpUTV9ST1cyCTAJMAkxDQoJCSpUTV9ST1czCTAJMAkwDQoJCSpUTV9QT1MJMAkwCTANCgl9DQoJKk1FU0gJew0KCQkqVElNRVZBTFVFCTANCgkJKk1FU0hfTlVNVkVSVEVYCTQNCgkJKk1FU0hfTlVNRkFDRVMJMg0KCQkqQ09NTUVOVAkiU1VSRkFDRVRZUEUJTVNUX1BMQU5BUiINCgkJKk1FU0hfVkVSVEVYX0xJU1QJew0KCQkJKk1FU0hfVkVSVEVYCTAJOC4wCS04LjAJMA0KCQkJKk1FU0hfVkVSVEVYCTEJLTguMAktOC4wCTANCgkJCSpNRVNIX1ZFUlRFWAkyCTguMAktOC4wCTE2LjANCgkJCSpNRVNIX1ZFUlRFWAkzCS04LjAJLTguMAkxNi4wDQoJCX0NCgkJKk1FU0hfTk9STUFMUwl7DQoJCQkqTUVTSF9GQUNFTk9STUFMCTAJMAktMQkwDQoJCQkqTUVTSF9GQUNFTk9STUFMCTEJMAktMQkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMAkwCS0xCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkxCTAJLTEJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTIJMAktMQkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMwkwCS0xCTANCgkJfQ0KCQkqTUVTSF9GQUNFX0xJU1QJew0KCQkJKk1FU0hfRkFDRQkwCUE6CTAJQjoJMglDOgkxCUFCOgkxCUJDOgkxCUNBOgkxCSpNRVNIX1NNT09USElORwkwCSpNRVNIX01UTElECTANCgkJCSpNRVNIX0ZBQ0UJMQlBOgkyCUI6CTMJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCX0NCgkJKk1FU0hfTlVNVFZFUlRFWAk0DQoJCSpNRVNIX1RWRVJUTElTVAl7DQoJCQkqTUVTSF9UVkVSVAkwCTAuNQkwCTENCgkJCSpNRVNIX1RWRVJUCTEJLTAuNQkwCTENCgkJCSpNRVNIX1RWRVJUCTIJMC41CTEJMQ0KCQkJKk1FU0hfVFZFUlQJMwktMC41CTEJMQ0KCQl9DQoJCSpNRVNIX05VTVRWRkFDRVMJMg0KCQkqTUVTSF9URkFDRUxJU1QJew0KCQkJKk1FU0hfVEZBQ0UJMAkwCTIJMQ0KCQkJKk1FU0hfVEZBQ0UJMQkyCTMJMQ0KCQl9DQoJfQ0KCSpQUk9QX01PVElPTkJMVVIJMA0KCSpQUk9QX0NBU1RTSEFET1cJMQ0KCSpQUk9QX1JFQ1ZTSEFET1cJMQ0KCSpNQVRFUklBTF9SRUYJMA0KfQ0KKkdFT01PQkpFQ1QJew0KCSpOT0RFX05BTUUJIm1hdDBtb2RlbDBzdXJmNyINCgkqTk9ERV9UTQl7DQoJCSpOT0RFX05BTUUJIm1hdDBtb2RlbDBzdXJmNyINCgkJKklOSEVSSVRfUE9TCTAJMAkwDQoJCSpJTkhFUklUX1JPVAkwCTAJMA0KCQkqSU5IRVJJVF9TQ0wJMAkwCTANCgkJKlRNX1JPVzAJMQkwCTANCgkJKlRNX1JPVzEJMAkxCTANCgkJKlRNX1JPVzIJMAkwCTENCgkJKlRNX1JPVzMJMAkwCTANCgkJKlRNX1BPUwkwCTAJMA0KCX0NCgkqTUVTSAl7DQoJCSpUSU1FVkFMVUUJMA0KCQkqTUVTSF9OVU1WRVJURVgJNA0KCQkqTUVTSF9OVU1GQUNFUwkyDQoJCSpDT01NRU5UCSJTVVJGQUNFVFlQRQlNU1RfUExBTkFSIg0KCQkqTUVTSF9WRVJURVhfTElTVAl7DQoJCQkqTUVTSF9WRVJURVgJMAktOC4wCTguMAkwDQoJCQkqTUVTSF9WRVJURVgJMQktOC4wCS04LjAJMA0KCQkJKk1FU0hfVkVSVEVYCTIJOC4wCTguMAkwDQoJCQkqTUVTSF9WRVJURVgJMwk4LjAJLTguMAkwDQoJCX0NCgkJKk1FU0hfTk9STUFMUwl7DQoJCQkqTUVTSF9GQUNFTk9STUFMCTAJMAktMQkwDQoJCQkqTUVTSF9GQUNFTk9STUFMCTEJMAktMQkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMAkwCTAJLTENCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkxCTAJMAktMQ0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTIJMAkwCS0xDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMwkwCTAJLTENCgkJfQ0KCQkqTUVTSF9GQUNFX0xJU1QJew0KCQkJKk1FU0hfRkFDRQkwCUE6CTAJQjoJMglDOgkxCUFCOgkxCUJDOgkxCUNBOgkxCSpNRVNIX1NNT09USElORwkwCSpNRVNIX01UTElECTANCgkJCSpNRVNIX0ZBQ0UJMQlBOgkyCUI6CTMJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCX0NCgkJKk1FU0hfTlVNVFZFUlRFWAk0DQoJCSpNRVNIX1RWRVJUTElTVAl7DQoJCQkqTUVTSF9UVkVSVAkwCS0wLjUJMS41CTENCgkJCSpNRVNIX1RWRVJUCTEJLTAuNQkwLjUJMQ0KCQkJKk1FU0hfVFZFUlQJMgkwLjUJMS41CTENCgkJCSpNRVNIX1RWRVJUCTMJMC41CTAuNQkxDQoJCX0NCgkJKk1FU0hfTlVNVFZGQUNFUwkyDQoJCSpNRVNIX1RGQUNFTElTVAl7DQoJCQkqTUVTSF9URkFDRQkwCTAJMgkxDQoJCQkqTUVTSF9URkFDRQkxCTIJMwkxDQoJCX0NCgl9DQoJKlBST1BfTU9USU9OQkxVUgkwDQoJKlBST1BfQ0FTVFNIQURPVwkxDQoJKlBST1BfUkVDVlNIQURPVwkxDQoJKk1BVEVSSUFMX1JFRgkwDQp9DQoqR0VPTU9CSkVDVAl7DQoJKk5PREVfTkFNRQkibWF0Mm1vZGVsMHN1cmY4Ig0KCSpOT0RFX1RNCXsNCgkJKk5PREVfTkFNRQkibWF0Mm1vZGVsMHN1cmY4Ig0KCQkqSU5IRVJJVF9QT1MJMAkwCTANCgkJKklOSEVSSVRfUk9UCTAJMAkwDQoJCSpJTkhFUklUX1NDTAkwCTAJMA0KCQkqVE1fUk9XMAkxCTAJMA0KCQkqVE1fUk9XMQkwCTEJMA0KCQkqVE1fUk9XMgkwCTAJMQ0KCQkqVE1fUk9XMwkwCTAJMA0KCQkqVE1fUE9TCTAJMAkwDQoJfQ0KCSpNRVNICXsNCgkJKlRJTUVWQUxVRQkwDQoJCSpNRVNIX05VTVZFUlRFWAk0DQoJCSpNRVNIX05VTUZBQ0VTCTINCgkJKkNPTU1FTlQJIlNVUkZBQ0VUWVBFCU1TVF9QTEFOQVIiDQoJCSpNRVNIX1ZFUlRFWF9MSVNUCXsNCgkJCSpNRVNIX1ZFUlRFWAkwCTAJMzIuMAkwDQoJCQkqTUVTSF9WRVJURVgJMQkwCTMyLjAJNjQuMA0KCQkJKk1FU0hfVkVSVEVYCTIJMAktMzIuMAk2NC4wDQoJCQkqTUVTSF9WRVJURVgJMwkwCS0zMi4wCTANCgkJfQ0KCQkqTUVTSF9OT1JNQUxTCXsNCgkJCSpNRVNIX0ZBQ0VOT1JNQUwJMAkwCS0xCTANCgkJCSpNRVNIX0ZBQ0VOT1JNQUwJMQkwCS0xCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkwCS0xCTAJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTEJLTEJMAkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMgktMQkwCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkzCS0xCTAJMA0KCQl9DQoJCSpNRVNIX0ZBQ0VfTElTVAl7DQoJCQkqTUVTSF9GQUNFCTAJQToJMAlCOgkyCUM6CTEJQUI6CTEJQkM6CTEJQ0E6CTEJKk1FU0hfU01PT1RISU5HCTAJKk1FU0hfTVRMSUQJMA0KCQkJKk1FU0hfRkFDRQkxCUE6CTAJQjoJMwlDOgkyCUFCOgkxCUJDOgkxCUNBOgkxCSpNRVNIX1NNT09USElORwkwCSpNRVNIX01UTElECTANCgkJfQ0KCQkqTUVTSF9OVU1UVkVSVEVYCTQNCgkJKk1FU0hfVFZFUlRMSVNUCXsNCgkJCSpNRVNIX1RWRVJUCTAJMAkwCTENCgkJCSpNRVNIX1RWRVJUCTEJMAkxCTENCgkJCSpNRVNIX1RWRVJUCTIJMQkxCTENCgkJCSpNRVNIX1RWRVJUCTMJMQkwCTENCgkJfQ0KCQkqTUVTSF9OVU1UVkZBQ0VTCTINCgkJKk1FU0hfVEZBQ0VMSVNUCXsNCgkJCSpNRVNIX1RGQUNFCTAJMAkyCTENCgkJCSpNRVNIX1RGQUNFCTEJMAkzCTINCgkJfQ0KCX0NCgkqUFJPUF9NT1RJT05CTFVSCTANCgkqUFJPUF9DQVNUU0hBRE9XCTENCgkqUFJPUF9SRUNWU0hBRE9XCTENCgkqTUFURVJJQUxfUkVGCTINCn0NCipHRU9NT0JKRUNUCXsNCgkqTk9ERV9OQU1FCSJtYXQybW9kZWwwc3VyZjkiDQoJKk5PREVfVE0Jew0KCQkqTk9ERV9OQU1FCSJtYXQybW9kZWwwc3VyZjkiDQoJCSpJTkhFUklUX1BPUwkwCTAJMA0KCQkqSU5IRVJJVF9ST1QJMAkwCTANCgkJKklOSEVSSVRfU0NMCTAJMAkwDQoJCSpUTV9ST1cwCTEJMAkwDQoJCSpUTV9ST1cxCTAJMQkwDQoJCSpUTV9ST1cyCTAJMAkxDQoJCSpUTV9ST1czCTAJMAkwDQoJCSpUTV9QT1MJMAkwCTANCgl9DQoJKk1FU0gJew0KCQkqVElNRVZBTFVFCTANCgkJKk1FU0hfTlVNVkVSVEVYCTQNCgkJKk1FU0hfTlVNRkFDRVMJMg0KCQkqQ09NTUVOVAkiU1VSRkFDRVRZUEUJTVNUX1BMQU5BUiINCgkJKk1FU0hfVkVSVEVYX0xJU1QJew0KCQkJKk1FU0hfVkVSVEVYCTAJLTMyLjAJMAkwDQoJCQkqTUVTSF9WRVJURVgJMQktMzIuMAkwCTY0LjANCgkJCSpNRVNIX1ZFUlRFWAkyCTMyLjAJMAk2NC4wDQoJCQkqTUVTSF9WRVJURVgJMwkzMi4wCTAJMA0KCQl9DQoJCSpNRVNIX05PUk1BTFMJew0KCQkJKk1FU0hfRkFDRU5PUk1BTAkwCTAJLTEJMA0KCQkJKk1FU0hfRkFDRU5PUk1BTAkxCTAJLTEJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTAJMAktMQkwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMQkwCS0xCTANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkyCTAJLTEJMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTMJMAktMQkwDQoJCX0NCgkJKk1FU0hfRkFDRV9MSVNUCXsNCgkJCSpNRVNIX0ZBQ0UJMAlBOgkwCUI6CTIJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCQkqTUVTSF9GQUNFCTEJQToJMAlCOgkzCUM6CTIJQUI6CTEJQkM6CTEJQ0E6CTEJKk1FU0hfU01PT1RISU5HCTAJKk1FU0hfTVRMSUQJMA0KCQl9DQoJCSpNRVNIX05VTVRWRVJURVgJNA0KCQkqTUVTSF9UVkVSVExJU1QJew0KCQkJKk1FU0hfVFZFUlQJMAkwCTAJMQ0KCQkJKk1FU0hfVFZFUlQJMQkwCTEJMQ0KCQkJKk1FU0hfVFZFUlQJMgkxCTEJMQ0KCQkJKk1FU0hfVFZFUlQJMwkxCTAJMQ0KCQl9DQoJCSpNRVNIX05VTVRWRkFDRVMJMg0KCQkqTUVTSF9URkFDRUxJU1QJew0KCQkJKk1FU0hfVEZBQ0UJMAkwCTIJMQ0KCQkJKk1FU0hfVEZBQ0UJMQkwCTMJMg0KCQl9DQoJfQ0KCSpQUk9QX01PVElPTkJMVVIJMA0KCSpQUk9QX0NBU1RTSEFET1cJMQ0KCSpQUk9QX1JFQ1ZTSEFET1cJMQ0KCSpNQVRFUklBTF9SRUYJMg0KfQ0K"; // corn02.ase was source or perhaps brush02.ase
	ccs *vegetion_ase_text_autosprite = "KjNEU01BWF9BU0NJSUVYUE9SVAkyMDANCipDT01NRU5UCSJHZW5lcmF0ZWQgYnkgUTNNYXAyICh5ZG5hcikgLWNvbnZlcnQgLWZvcm1hdCBhc2UiDQoqU0NFTkUJew0KCSpTQ0VORV9GSUxFTkFNRQkiW01PREVMXS5ic3AiDQoJKlNDRU5FX0ZJUlNURlJBTUUJMA0KCSpTQ0VORV9MQVNURlJBTUUJMTAwDQoJKlNDRU5FX0ZSQU1FU1BFRUQJMzANCgkqU0NFTkVfVElDS1NQRVJGUkFNRQkxNjANCgkqU0NFTkVfQkFDS0dST1VORF9TVEFUSUMJMC4wMDAwCTAuMDAwMAkwLjAwMDANCgkqU0NFTkVfQU1CSUVOVF9TVEFUSUMJMC4wMDAwCTAuMDAwMAkwLjAwMDANCn0NCipNQVRFUklBTF9MSVNUCXsNCgkqTUFURVJJQUxfQ09VTlQJMw0KCSpNQVRFUklBTAkwCXsNCgkJKk1BVEVSSUFMX05BTUUJInRleHR1cmVzL2NvbW1vbi9jYXVsayINCgkJKk1BVEVSSUFMX0NMQVNTCSJTdGFuZGFyZCINCgkJKk1BVEVSSUFMX0RJRkZVU0UJMS4wMDAwMDAJMS4wMDAwMDAJMS4wMDAwMDANCgkJKk1BVEVSSUFMX1NIQURJTkcgUGhvbmcNCgkJKk1BUF9ESUZGVVNFCXsNCgkJCSpNQVBfTkFNRQkidGV4dHVyZXMvY29tbW9uL2NhdWxrIg0KCQkJKk1BUF9DTEFTUwkiQml0bWFwIg0KCQkJKk1BUF9TVUJOTwkxDQoJCQkqTUFQX0FNT1VOVAkxLjANCgkJCSpNQVBfVFlQRQlTY3JlZW4NCgkJCSpCSVRNQVAJIi4uXHRleHR1cmVzXGNvbW1vblxjYXVsay50Z2EiDQoJCQkqQklUTUFQX0ZJTFRFUglQeXJhbWlkYWwNCgkJfQ0KCX0NCgkqTUFURVJJQUwJMQl7DQoJCSpNQVRFUklBTF9OQU1FCSJ0ZXh0dXJlcy9jb21tb24vbm9kcmF3Ig0KCQkqTUFURVJJQUxfQ0xBU1MJIlN0YW5kYXJkIg0KCQkqTUFURVJJQUxfRElGRlVTRQkxLjAwMDAwMAkxLjAwMDAwMAkxLjAwMDAwMA0KCQkqTUFURVJJQUxfU0hBRElORyBQaG9uZw0KCQkqTUFQX0RJRkZVU0UJew0KCQkJKk1BUF9OQU1FCSJ0ZXh0dXJlcy9jb21tb24vbm9kcmF3Ig0KCQkJKk1BUF9DTEFTUwkiQml0bWFwIg0KCQkJKk1BUF9TVUJOTwkxDQoJCQkqTUFQX0FNT1VOVAkxLjANCgkJCSpNQVBfVFlQRQlTY3JlZW4NCgkJCSpCSVRNQVAJIi4uXHRleHR1cmVzXGNvbW1vblxub2RyYXcudGdhIg0KCQkJKkJJVE1BUF9GSUxURVIJUHlyYW1pZGFsDQoJCX0NCgl9DQoJKk1BVEVSSUFMCTIJew0KCQkqTUFURVJJQUxfTkFNRQkiW1BBVEhdL1tNT0RFTF0iDQoJCSpNQVRFUklBTF9DTEFTUwkiU3RhbmRhcmQiDQoJCSpNQVRFUklBTF9ESUZGVVNFCTEuMDAwMDAwCTEuMDAwMDAwCTEuMDAwMDAwDQoJCSpNQVRFUklBTF9TSEFESU5HIFBob25nDQoJCSpNQVBfRElGRlVTRQl7DQoJCQkqTUFQX05BTUUJIltQQVRIXS9bTU9ERUxdIg0KCQkJKk1BUF9DTEFTUwkiQml0bWFwIg0KCQkJKk1BUF9TVUJOTwkxDQoJCQkqTUFQX0FNT1VOVAkxLjANCgkJCSpNQVBfVFlQRQlTY3JlZW4NCgkJCSpCSVRNQVAJIltQQVRIXS9bTU9ERUxdIg0KCQkJKkJJVE1BUF9GSUxURVIJUHlyYW1pZGFsDQoJCX0NCgl9DQp9DQoqR0VPTU9CSkVDVAl7DQoJKk5PREVfTkFNRQkibWF0Mm1vZGVsMHN1cmYwIg0KCSpOT0RFX1RNCXsNCgkJKk5PREVfTkFNRQkibWF0Mm1vZGVsMHN1cmYwIg0KCQkqSU5IRVJJVF9QT1MJMAkwCTANCgkJKklOSEVSSVRfUk9UCTAJMAkwDQoJCSpJTkhFUklUX1NDTAkwCTAJMA0KCQkqVE1fUk9XMAkxLjAJMAkwDQoJCSpUTV9ST1cxCTAJMS4wCTANCgkJKlRNX1JPVzIJMAkwCTEuMA0KCQkqVE1fUk9XMwkwCTAJMA0KCQkqVE1fUE9TCTAuMDAwMDAwCTAuMDAwMDAwCTAuMDAwMDAwDQoJfQ0KCSpNRVNICXsNCgkJKlRJTUVWQUxVRQkwDQoJCSpNRVNIX05VTVZFUlRFWAk0DQoJCSpNRVNIX05VTUZBQ0VTCTINCgkJKkNPTU1FTlQJIlNVUkZBQ0VUWVBFCU1TVF9QTEFOQVIiDQoJCSpNRVNIX1ZFUlRFWF9MSVNUCXsNCgkJCSpNRVNIX1ZFUlRFWAkwCTguMDAwMDAwCS0xLjAwMDAwMAkxLjAwMDAwMA0KCQkJKk1FU0hfVkVSVEVYCTEJLTguMDAwMDAwCS0xLjAwMDAwMAktMTYuMDAwMDAwDQoJCQkqTUVTSF9WRVJURVgJMgktOC4wMDAwMDAJLTEuMDAwMDAwCTEuMDAwMDAwDQoJCQkqTUVTSF9WRVJURVgJMwk4LjAwMDAwMAktMS4wMDAwMDAJLTE2LjAwMDAwMA0KCQl9DQoJCSpNRVNIX05PUk1BTFMJew0KCQkJKk1FU0hfRkFDRU5PUk1BTAkwCTAuMDAwMDAwCS0xLjAwMDAwMAkwLjAwMDAwMA0KCQkJKk1FU0hfRkFDRU5PUk1BTAkxCTAuMDAwMDAwCS0xLjAwMDAwMAkwLjAwMDAwMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTAJMC4wMDAwMDAJLTEuMDAwMDAwCTAuMDAwMDAwDQoJCQkqTUVTSF9WRVJURVhOT1JNQUwJMQkwLjAwMDAwMAktMS4wMDAwMDAJMC4wMDAwMDANCgkJCSpNRVNIX1ZFUlRFWE5PUk1BTAkyCTAuMDAwMDAwCS0xLjAwMDAwMAkwLjAwMDAwMA0KCQkJKk1FU0hfVkVSVEVYTk9STUFMCTMJMC4wMDAwMDAJLTEuMDAwMDAwCTAuMDAwMDAwDQoJCX0NCgkJKk1FU0hfRkFDRV9MSVNUCXsNCgkJCSpNRVNIX0ZBQ0UJMAlBOgkwCUI6CTIJQzoJMQlBQjoJMQlCQzoJMQlDQToJMQkqTUVTSF9TTU9PVEhJTkcJMAkqTUVTSF9NVExJRAkwDQoJCQkqTUVTSF9GQUNFCTEJQToJMwlCOgkwCUM6CTEJQUI6CTEJQkM6CTEJQ0E6CTEJKk1FU0hfU01PT1RISU5HCTAJKk1FU0hfTVRMSUQJMA0KCQl9DQoJCSpNRVNIX05VTVRWRVJURVgJNA0KCQkqTUVTSF9UVkVSVExJU1QJew0KCQkJKk1FU0hfVFZFUlQJMAkxLjAwMDAwMAkxLjAwMDAwMAkxLjAwMDAwMA0KCQkJKk1FU0hfVFZFUlQJMQkwLjAwMDAwMAkwLjAwNTE1MAkxLjAwMDAwMA0KCQkJKk1FU0hfVFZFUlQJMgkwLjAwMDAwMAkxLjAwMDAwMAkxLjAwMDAwMA0KCQkJKk1FU0hfVFZFUlQJMwkxLjAwMDAwMAkwLjAwNTE1MAkxLjAwMDAwMA0KCQl9DQoJCSpNRVNIX05VTVRWRkFDRVMJMg0KCQkqTUVTSF9URkFDRUxJU1QJew0KCQkJKk1FU0hfVEZBQ0UJMAkwCTIJMQ0KCQkJKk1FU0hfVEZBQ0UJMQkzCTAJMQ0KCQl9DQoJfQ0KCSpQUk9QX01PVElPTkJMVVIJMA0KCSpQUk9QX0NBU1RTSEFET1cJMQ0KCSpQUk9QX1JFQ1ZTSEFET1cJMQ0KCSpNQVRFUklBTF9SRUYJMg0KfQ0K"; // Ender

	if (Cmd_Argc(cmd) < 2) {
		Con_PrintLinef ("usage:" NEWLINE "%s <image>", Cmd_Argv(cmd, 0));
		return;
	}

	ccs *s_texture = Cmd_Argv(cmd, 1); //
	ccs *s_switch = Cmd_Argv(cmd, 2); //

	int is_autosprite = s_switch[0] && String_Match_Caseless (s_switch, "sprite");

	Con_PrintLinef ("Wants autosprite = %d ", is_autosprite);

	size_t unbase_datasize;
	char *s_unbase64_calloc = (char *)base64_decode_calloc (vegetion_ase_text_base64, &unbase_datasize);

	size_t s_autosprite_za_size = strlen(vegetion_ase_text_autosprite) * 2;
	char *s_autosprite_za = (char *)Mem_Alloc (tempmempool, s_autosprite_za_size);

	strlcpy (s_autosprite_za, vegetion_ase_text_autosprite, s_autosprite_za_size);

	// Baker: Extra room for replacement.
	size_t smaxsize = unbase_datasize * 2;
	s_unbase64_calloc = (char *)realloc (s_unbase64_calloc, smaxsize);


	char *s_model_za = Z_StrDup (s_texture);
	File_URL_Edit_Remove_Extension (s_model_za); //
	char *s_model_ase_za;
	if (is_autosprite)
		s_model_ase_za = Z_StrDupf ("%s_autosprite.obj", s_model_za);  // append _solid and append .ase
	else
		s_model_ase_za = Z_StrDupf ("%s.ase", s_model_za);  // append _solid and append .ase
	char *s_barename_noext_za = Z_StrDup(File_URL_SkipPath(s_model_za));
	char *s_path_notrailslash_za = Z_StrDup(s_model_ase_za); // models/outdoors -- no trailing slash
	File_URL_Edit_Reduce_To_Parent_Path_No_Trailing_Slash (s_path_notrailslash_za);

	//remove filename or strip to path
	Con_PrintLinef ("Path is " QUOTED_S, s_path_notrailslash_za);
	Con_PrintLinef ("Texture bare name is " QUOTED_S, s_barename_noext_za);

	// [PATH] like models/outdoors s_path_notrailslash_za
	// [MODEL] like brush02 s_barename_noext_za
	char *targ = is_autosprite ? s_autosprite_za : s_unbase64_calloc;
	//char *targfile = is_autosprite ? s_autosprite_za : s_unbase64_calloc;
	String_Edit_Replace (targ, smaxsize, "[MODEL]", s_barename_noext_za);
	String_Edit_Replace (targ, smaxsize, "[PATH]", s_path_notrailslash_za);

	qfile_t *f = FS_OpenRealFile(s_model_ase_za, "wb", fs_quiet_FALSE);

	if (f) {
		FS_Printf (f, "%s" NEWLINE, targ);
		FS_CloseNULL_ (f);
		Con_PrintLinef ("Wrote " QUOTED_S, s_model_ase_za);
	} else {
		Con_PrintLinef ("Could not open " QUOTED_S " for writing", s_model_ase_za);
	}


	freenull_ (s_unbase64_calloc);
	Mem_FreeNull_ (s_autosprite_za);
	Mem_FreeNull_ (s_path_notrailslash_za);
	Mem_FreeNull_ (s_barename_noext_za);
	Mem_FreeNull_ (s_model_za);
	Mem_FreeNull_ (s_model_ase_za);
}
#endif

//Cmd_AddCommand(CF_SHARED, "jpegsplit", FS_JpegSplit_f, "jpegsplit <folder> (test) -- or --  <folder> go (run conversion!) --- load all TGA/PNG in supplied folder, write them as .jpg to same directory including any _alpha jpegs.  DarkPlaces pattern matching hates periods '.' in path names, beware! [Zircon]");
void FS_JpegCheck_f (cmd_state_t *cmd)
{
	unsigned char *loadimagepixelsbgra (const char *filename, qbool complain, qbool allowFixtrans, qbool convertsRGB, int *miplevel); // LoadTGA_BGRA
	qbool JPEG_SaveImage_preflipped (const char *filename, int width, int height, unsigned char *data);
	void Image_CopyMux(unsigned char *outpixels, const unsigned char *inpixels, int inputwidth, int inputheight, qbool inputflipx, qbool inputflipy, qbool inputflipdiagonal, int numoutputcomponents, int numinputcomponents, int *outputinputcomponentindices);

	extern int image_width, image_height; // Baker: image globals !!!
	int is_write = cmd_argc == 3;
	ccs *s_folder = cmd_argv_parm1; // "zircon_beta_windows_20240116.zip";

	fs_offset_t savings = 0;
	//int		num_qualified;

	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<folder> ", "Checks .jpg and .jpeg size savings");

	stringlist_t slist = {0};

	stringlistappend_from_dir_pattern (&slist, s_folder, ".jpeg", q_strip_exten_false);
	stringlistappend_from_dir_pattern (&slist, s_folder, ".jpg",  q_strip_exten_false);

	Con_PrintLinef ("%d results %s" NEWLINE, slist.numstrings, is_write ?  "(" CON_BRONZE "Writing .jpgs" CON_WHITE ")" : "("CON_CYAN "Test only" CON_WHITE ")" );


	int total_bigsavings = 0;


	for (int idx = 0; idx < slist.numstrings; idx ++) {
		char *sxy = slist.strings[idx];
		fs_offset_t oldsize = FS_FileSizeEx_Safe (sxy);

		Vid_SetWindowTitlef ("%d/%d %s", idx, slist.numstrings, sxy);

		//if (is_write == false) {
		//	Con_PrintLinef ("%4d: " S_FMT_LEFT_PAD_40 " " INT64_F, idx, sxy, oldsize);
		//	goto free_data_continue;
		//}

		// Prepare Read Name
		va_super (s_testname, MAX_QPATH_128, "%s", sxy);

		c_strlcpy (s_testname, "#delme.jpg");

		byte *data_bgra = loadimagepixelsbgra (
			sxy,
			q_tx_complain_false,
			q_tx_allowfixtrans_false,
			q_tx_convertsrgb_false,
			q_tx_miplevel_null
		);

		if (data_bgra == NULL) {
			Con_PrintLinef ("Error loading %s", sxy);
			goto free_data_continue;
		}

		unsigned char *noalphabuffer_3 = (unsigned char *)Mem_Alloc(tempmempool, image_width * image_height * RGB_3);

		int	indices[4] = {0,1,2,3}; // BGRA
		if (1 /*jpeg*/) { indices[0] = 2; indices[2] = 0; }


		Image_CopyMux (noalphabuffer_3, data_bgra, image_width, image_height, /*flipx*/ false, /*flipy*/ true, /*flipdiagonal*/ false, 3, 4, indices);

		if (1) {

			int is_ok = JPEG_SaveImage_preflipped (
				s_testname,
				image_width,
				image_height,
				noalphabuffer_3 //data_bgra
				);
			if (is_ok == false)
				Con_PrintLinef ("Error saving JPEG %s ", s_testname);
		} // diffuse


		fs_offset_t newsize = FS_FileSizeEx_Safe (s_testname);
		fs_offset_t savings_here = oldsize - newsize;

		if (savings_here > (oldsize * 0.5) || savings_here > 50000) {
			Con_PrintLinef (CON_RED "%4d: " S_FMT_LEFT_PAD_40 " " INT64_F " to " INT64_F "(savings " INT64_F ")", idx, sxy, oldsize, newsize,savings_here );
			total_bigsavings ++;
			savings += savings_here;


			if (is_write) {
				va_super (s_name_write, MAX_QPATH_128, "%s", sxy);
				c_strlcat (s_name_write, "_newb");
				int is_ok = JPEG_SaveImage_preflipped (
					s_name_write,
					image_width,
					image_height,
					noalphabuffer_3 //data_bgra
					);
				if (is_ok == false)
					Con_PrintLinef ("Error saving JPEG %s ", s_name_write);
			}

			//Con_PrintLinef ("Total savings = " INT64_F, savings);
		}

		Mem_Free (noalphabuffer_3); // Baker: it's temppool so ok
		Mem_Free (data_bgra); // Baker: it's temppool so ok


free_data_continue:

		; // Oblig
	} // for

	stringlistfreecontents (&slist);

	if (is_write == false) {
		// Con_PrintLinef (CON_BRONZE "Total savings would be %d files = " INT64_F, total_bigsavings, savings);
		Con_PrintLinef (CON_BRONZE "Total savings would be %d files = " INT64_F, total_bigsavings, savings);
		Con_PrintLinef (CON_BRONZE "TO REALLY RUN CONVERSION:" NEWLINE CON_GREEN "%s %s go " CON_WHITE "// REALLY RUN CONVERSION!", cmd_argv_command_0, cmd_argv_parm1);
		//Con_PrintLinef (CON_BRONZE "Note that DarkPlaces pattern matching hates periods '.' in path names.");
		return;
	}

	Vid_SetWindowTitlef (gamename); // RESET
}

static qbool jpg_savequick_noalpha_isok (ccs *filename, const byte *pels, int w, int h)
{
	qbool JPEG_SaveImage_preflipped (ccs *filename, int width, int height, unsigned char *data);
	void Image_CopyMux(byte *outpixels, const byte *inpixels, int inputwidth, int inputheight, qbool inputflipx, qbool inputflipy, qbool inputflipdiagonal, int numoutputcomponents, int numinputcomponents, int *outputinputcomponentindices);

	byte *noalphabuffer_3 = (byte *)Mem_Alloc(tempmempool, w * h * RGB_3);

	int	indices[4] = {0,1,2,3};		// BGRA
	indices[0] = 2; indices[2] = 0; // JPEG

	Image_CopyMux (noalphabuffer_3, pels, w, h, /*flipx*/ false, /*flipy*/ true, /*flipdiagonal*/ false, 3, 4, indices);

	int is_ok = JPEG_SaveImage_preflipped (
		filename,
		w,
		h,
		noalphabuffer_3
	);

	Mem_FreeNull_ (noalphabuffer_3);
	if (is_ok == false) {
		Con_PrintLinef ("Error saving JPEG %s ", filename);
		return false;
	}

	return true;
}

static qbool jpg_savequick_alpha_will_edit_pels_isok (ccs *filename, byte *pels /*edited*/, int w, int h)
{
	if (!jpg_savequick_noalpha_isok (filename, pels, w, h))
		return false;

	// Part 2: Alpha channel
	va_super (filename_alpha, MAX_QPATH_128, "%s", filename);
	File_URL_Edit_Remove_Extension(filename_alpha);
	File_URL_Edit_Strip_Set_Extension (filename_alpha, "_alpha.jpg", sizeof(filename_alpha));

	// Copy alpha to rgb
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		pels[y + 0] = pels[y + 1] = pels[y + 2] = pels[y + 3];
	} // for

	if (!jpg_savequick_noalpha_isok (filename_alpha, pels, w, h))
		return false;

	return true;
}

void FS_JpegResample_f (cmd_state_t *cmd)
{
	extern cvar_t cl_video_frame_modulo;
	extern int image_width, image_height; // Baker: image globals !!!
	byte *loadimagepixelsbgra (ccs *filename, qbool complain, qbool allowFixtrans, qbool convertsRGB, int *miplevel); // LoadTGA_BGRA

	// +/- that amount for each rgb.  Need settitle.

	RELATED_ (FS_GifToShader_f)

	CMD_USAGE_NUM_ARGS_DESCRIBE_ (4, "<folder> <width> <height> [alpha channel method]", "converts png to alpha jpg (1 = alpha, 2 = green, 3 = green w/alpha)" NEWLINE "set cl_video_frame_modulo 2 for 50" PERCENT_SIGN " or 4 for 25" PERCENT_SIGN)
	ccs *folder			= cmd_argv_parm1;
	int width_resample	= atoi(cmd_argv_parm2);
	int height_resample	= atoi(cmd_argv_parm3);
	ccs *alpha_method	= cmd_argv_parm4; // Optional here

	int chroma_red		= atoi(cmd_argv_parm5); //
	int chroma_green	= atoi(cmd_argv_parm6); //
	int chroma_blue		= atoi(cmd_argv_parm7); //
	int chroma_tolerance = atoi(cmd_argv_parm8);

	Con_PrintVarInt		(width_resample);
	Con_PrintVarInt		(height_resample);
	Con_PrintVarString	(alpha_method);
	Con_PrintVarInt		(chroma_red);
	Con_PrintVarInt		(chroma_green);
	Con_PrintVarInt		(chroma_blue);

	stringlist_t list = {0}; 		baker_string_t	*bstextures = BakerString_Create_Malloc ("");

	stringlistappend_from_dir_pattern (&list, folder, ".png", q_strip_exten_false);

	Con_PrintLinef ("Matching = %d results" NEWLINE, list.numstrings);

	int num_adds =0;
	int did_print = false;
	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *sxy = list.strings[idx];
		if (cl_video_frame_modulo.integer) {
			if (idx % cl_video_frame_modulo.integer != 0)
				continue;
		}

		Vid_SetWindowTitlef ("%d/%d %s", idx, list.numstrings, sxy);

		va_super (filename_out, MAX_QPATH_128, "%s", sxy);
		File_URL_Edit_Strip_Set_Extension (filename_out, ".jpg", sizeof(filename_out));

		// READ
		byte *data_bgra_za = loadimagepixelsbgra (sxy, q_tx_complain_false, q_tx_allowfixtrans_false, q_tx_convertsrgb_false, q_tx_miplevel_null);
		CMD_CONTINUE_LOOP_MSG_IF_F1_ (data_bgra_za == NULL, "Error loading image %s", sxy);

		//int is_ok_clip = Sys_Clipboard_Set_Image_BGRA_Is_Ok ((bgra4 *)data_bgra_za, image_width, image_height);
		//if (String_Match (alpha_method, "noalpha") { }
		if (String_Match (alpha_method, "alpha_green0_other_1")) {
			Image_Alpha_Green_0_Other_1_BGRA (data_bgra_za, image_width, image_height);
		} else if (String_Match (alpha_method, "alpha_green0_other_luminance")) {
			Image_Alpha_Green_0_Other_Luminance_BGRA (data_bgra_za, image_width, image_height);
		} else if (String_Match (alpha_method, "alpha_black0_other1")) {
			Image_Alpha_Black_0_Other_1_BGRA (data_bgra_za, image_width, image_height);
		} else if (String_Match (alpha_method, "alpha_luminance")) {
			Image_Alpha_From_Luminance_BGRA (data_bgra_za, image_width, image_height);
		} else if (String_Starts_With_Caseless (alpha_method, "alpha_luminance_LT_")) {
			ccs *lookat = alpha_method + strlen("alpha_luminance_LT_");
			int luminance_max_value = atoi(lookat);
			if (did_print == false) {
				Con_PrintVarInt		(luminance_max_value);
				did_print = true;
			}
			Image_Alpha_From_Luminance_LT_X_BGRA (data_bgra_za, image_width, image_height, luminance_max_value);
		} else if (String_Starts_With_Caseless (alpha_method, "alpha_green_0_luminance_LT_")) {
			ccs *lookat = alpha_method + strlen("alpha_green_0_luminance_LT_");
			int luminance_max_value = atoi(lookat);
			if (did_print == false) {
				Con_PrintVarInt		(luminance_max_value);
				did_print = true;
			}
			Image_Alpha_From_Luminance_LT_X_Green_0_BGRA (data_bgra_za, image_width, image_height, luminance_max_value);
		}
		else if (String_Starts_With_Caseless (alpha_method, "chromakey_LT_")) {
			ccs *lookat = alpha_method + strlen("chromakey_LT_");
			int luminance_max_value = atoi(lookat);
			if (did_print == false) {
				Con_PrintVarInt		(luminance_max_value);
				Con_PrintVarInt		(chroma_red);
				Con_PrintVarInt		(chroma_green);
				Con_PrintVarInt		(chroma_blue);
				Con_PrintVarInt		(chroma_tolerance);
				did_print = true;
			}
			Image_Alpha_From_Luminance_LT_X_Ignore_RGB_Provided_Tolerance_0_BGRA (data_bgra_za,
				image_width, image_height, luminance_max_value, chroma_red, chroma_green, chroma_blue, chroma_tolerance);


		}
#if 0
		// DEBUG
		qbool is_ok = Sys_Clipboard_Set_Image_Alpha_BGRA_Is_Ok (data_bgra_za, image_width, image_height);
#endif

		if (width_resample && height_resample) {
			byte *pels_resample_za = (byte *)Image_Bilinear_Resize_ZAlloc ((bgra4 *)data_bgra_za, image_width, image_height, width_resample, height_resample);
			jpg_savequick_alpha_will_edit_pels_isok (filename_out, pels_resample_za, width_resample, height_resample);
			Mem_FreeNull_ (pels_resample_za);
		} else {
			// No resample
			jpg_savequick_alpha_will_edit_pels_isok (filename_out, data_bgra_za, image_width, image_height);
		}

		Mem_FreeNull_ (data_bgra_za);

		if (num_adds < TEXTURE_MAXFRAMES_64) {
			if (num_adds > 0)
				BakerString_CatC (bstextures, SPACER);
			BakerString_CatC (bstextures, filename_out);
			num_adds ++;
		}


	} // for


	if (list.numstrings > 0) {
		//baker_string_t	*bstextures = BakerString_Create_Malloc ("");

		float total_seconds = 2.0; // Why not ...
		ccs *s = "[TEXTURE_NO_EXT]" NEWLINE
				"{" NEWLINE
				"	qer_editorimage [TEXTURE_0_TGA]" NEWLINE
				"	{" NEWLINE
	//			"		map [TEXTURE_0_TGA]" NEWLINE
				"		// Frequency: The number of times that the animation cycle will repeat within " NEWLINE
				"		// a one second time period." NEWLINE
				"		// The larger the value, the more repeats within a second. " NEWLINE
				"		animmap [FREQUENCY] [TEXTURES]" NEWLINE
				"	}" NEWLINE
				"	{" NEWLINE
				"		map $lightmap" NEWLINE
				"		tcGen lightmap" NEWLINE
				"		blendfunc filter" NEWLINE
				"	}" NEWLINE
				"" NEWLINE
				"}" NEWLINE;

		// The number of times that the animation cycle will repeat within a one second time period.
		// The larger the value, the more repeats within a second.
		//float frequency = 1.0 / (float)total_seconds;
		float frequency = 1.0 / (float)total_seconds;

		char *s_shader_text_a = (char *)Mem_Alloc (tempmempool, 16384);
		strlcpy (s_shader_text_a, s, 16384);

		//va_super (s_tga0, MAX_QPATH_128, "%s", list.numstrings[0] );
		va_super (s_frequency, 1024, FLOAT_LOSSLESS_FORMAT, frequency);

		String_Edit_Replace (s_shader_text_a, 16384, "[TEXTURE_NO_EXT]", "textures/myvideo");
		String_Edit_Replace (s_shader_text_a, 16384, "[TEXTURE_0_TGA]", list.strings[0] );
		String_Edit_Replace (s_shader_text_a, 16384, "[FREQUENCY]", "1"); // s_frequency
		String_Edit_Replace (s_shader_text_a, 16384, "[TEXTURES]", bstextures->string);

		Clipboard_Set_Text (s_shader_text_a); // giftoshader
		Con_PrintLinef ("Shader text copied to clipboard");
		Mem_FreeNull_ (s_shader_text_a);

		BakerString_Destroy_And_Null_It (&bstextures);
	}

cleanup:
	stringlistfreecontents (&list);

	Vid_SetWindowTitlef (gamename); // RESET
}

void FS_JpegSplit_f (cmd_state_t *cmd)
{
	extern int image_width, image_height; // Baker: image globals !!!
	unsigned char *loadimagepixelsbgra (const char *filename, qbool complain, qbool allowFixtrans, qbool convertsRGB, int *miplevel); // LoadTGA_BGRA

	CMD_USAGE_NUM_ARGS_DESCRIBE_ (2, "<folder> go", "converts png/tga to jpg with _alpha")


	ccs *folder		= cmd_argv_parm1;
	int is_go		= cmd_argc == 3;

	stringlist_t list = {0};

	RELATED_ (LoadTGA_BGRA, JPEG_SaveImage_preflipped, Image_CopyAlphaFromBlueBGRA)

	stringlistappend_from_dir_pattern (&list, folder, ".png", q_strip_exten_false);
	stringlistappend_from_dir_pattern (&list, folder, ".tga", q_strip_exten_false);

	Con_PrintLinef ("%d results %s" NEWLINE, list.numstrings, is_go ?  "(" CON_BRONZE "Writing .jpgs" CON_WHITE ")" : "("CON_CYAN "Test only" CON_WHITE ")" );

	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *sxy = list.strings[idx];

		Vid_SetWindowTitlef ("%d/%d %s", idx, list.numstrings, sxy);

		if (is_go == false) {
			Con_PrintLinef ("%4d: %s", idx, sxy);
			continue;
		}

		va_super (filename_out, MAX_QPATH_128, "%s", sxy);
		File_URL_Edit_Strip_Set_Extension (filename_out, ".jpg", sizeof(filename_out));

		RELATED_ (LoadTGA_BGRA imageformats_textures)

		byte *data_bgra_za = loadimagepixelsbgra (sxy, q_tx_complain_false, q_tx_allowfixtrans_false, q_tx_convertsrgb_false, q_tx_miplevel_null);

		CMD_CONTINUE_LOOP_MSG_IF_F1_ (data_bgra_za == NULL, "Error loading image %s", sxy);

		qbool is_alpha_channel = Image_Has_Alpha (data_bgra_za, image_width, image_height);

		Con_PrintLinef ("%4d: alpha? %s - %s", idx, is_alpha_channel? CON_BRONZE "Y" CON_WHITE : "N", sxy);

		//int is_ok_clip = Sys_Clipboard_Set_Image_BGRA_Is_Ok ((unsigned int *)data_bgra, image_width, image_height);
		jpg_savequick_noalpha_isok (filename_out, data_bgra_za, image_width, image_height);
		if (is_alpha_channel)
			jpg_savequick_alpha_will_edit_pels_isok (filename_out, data_bgra_za, image_width, image_height);

		Mem_Free (data_bgra_za);
	} // for

	stringlistfreecontents (&list);

	if (is_go == false) {
		Con_PrintLinef (CON_BRONZE "testing done %s", cmd_argv_command_0);
		Con_PrintLinef (CON_BRONZE "TO REALLY RUN CONVERSION:" NEWLINE CON_GREEN "%s %s go " CON_WHITE "// REALLY RUN CONVERSION!", Cmd_Argv(cmd, 0), Cmd_Argv(cmd, 1));
		Con_PrintLinef (CON_BRONZE "Note that DarkPlaces pattern matching hates periods '.' in path names.");
		return;
	}

	Vid_SetWindowTitlef (gamename); // RESET

}

void FS_Zipinfo_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) != 2) {
		Con_PrintLinef ("usage:" NEWLINE "%s <file>", Cmd_Argv(cmd, 0));
		return;
	}

	const char *s_filename = Cmd_Argv(cmd, 1); // "zircon_beta_windows_20240116.zip";

	char fullpath[MAX_OSPATH];

	searchpath_t *search = FS_FindFile (s_filename, fs_package_index_reply_null, fs_quiet_true);
	if (!search) {
		Con_PrintLinef ("Did not find file");
		return;// FS_FILETYPE_NONE_0;
	}

	if (search->pack && !search->pack->vpack) {
		Con_PrintLinef ("Can't do zip files in pak or pak3");
		return; // FS_FILETYPE_FILE_1; // TODO can't check directories in paks yet, maybe later
	}

	c_dpsnprintf2 (fullpath, "%s%s", search->filename, s_filename);

	Con_PrintLinef ("Real file is: %s", fullpath);

	int is_ok = Zip_List_Print_Is_Ok (fullpath);
	if (is_ok == false)
		Con_PrintLinef ("zip info failed");
}


#include <time.h>
// "20241005 10:12 AM"
void TimeString_Fill_YYYYMMDD_HHMMSSAM (char *timestring, size_t timestring_size) // "20241005 10:12 AM"
{
	time_t crt_time;
	time (&crt_time);

#if _MSC_VER >= 1400
	struct tm crt_tm;
#else
	struct tm *crt_tm;
#endif

	#if _MSC_VER >= 1400
		localtime_s (&crt_tm, &crt_time);
		strftime (timestring, timestring_size, "%a %b %d %H:%M:%S %Y", &crt_tm);
	#else
		crt_tm = localtime (&crt_time);
		strftime (timestring, timestring_size, "%a %b %d %H:%M:%S %Y", crt_tm);
	#endif


}

// "02/02/2024 10:12 AM"
// Baker: This gets the timestring for a file (used for maps menu)
CALLERS_ (VM_FS_FileInfo plus MAPX_TextureChange which is maps menu)
// returns zero on failure?
double FS_FileTime_Since_1970_Safe (ccs *filename)
{
	char realfilename[MAX_OSPATH_EX_1024];
	ccs *x_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1; // Con_PrintVarString (x_gamedir);
	c_strlcpy (realfilename, fs_basedir); // Has trail, right?
	c_strlcat (realfilename, x_gamedir); // No trail
	c_strlcat (realfilename, "/");
    c_strlcat (realfilename, filename);
	File_URL_Edit_SlashesForward_Like_Unix (realfilename);

	double filetime_since1970 = FS_File_DateTime_Unsafe(realfilename);
	return filetime_since1970;
}


qbool File_TimeString_Fill_MM_DD_YY_HH_MM_AM (ccs *filename, char *timestring, size_t timestring_size)
{
	//char *realpathname_zalloc = NULL;

	// Fill menu
	char realfilename[MAX_OSPATH_EX_1024];
	ccs *x_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1; // Con_PrintVarString (x_gamedir);
	c_strlcpy (realfilename, fs_basedir); // Has trail, right?
	c_strlcat (realfilename, x_gamedir); // No trail
	c_strlcat (realfilename, "/");
    c_strlcat (realfilename, filename);
	File_URL_Edit_SlashesForward_Like_Unix (realfilename);

	double sav_filetime_since1970 = FS_File_DateTime_Unsafe(realfilename);

	if (sav_filetime_since1970 == 0) {
		timestring[0] = NULL_CHAR_0;
		return false;
	}

	time_t		rawtime			= (time_t)(double)sav_filetime_since1970;
	struct tm	*tmx			= localtime(&rawtime);


	#if _MSC_VER >= 1400
		//localtime_s (&crt_tm, &crt_time);
		strftime (timestring, timestring_size, "%Y/%m/%d %I:%M %p", tmx);
	#else
		//crt_tm = localtime (&crt_time);
		strftime (timestring, timestring_size, "%Y/%m/%d %I:%M %p", tmx);
	#endif

	return true;
}

char fs_csg_basedir[MAX_OSPATH];

CALLERS_ (FS_Init_Dir) // -csg and -q13 only
void PathWork_CSG(int csg_argnum, char *mappath_fill, size_t mappath_fill_size)
{
		// Baker: With C:/galaxy/zircon_beta_gcc.exe -csg C:\galaxy\zircon\maps\aafter
		ccs *mapfullpath = sys.argv[csg_argnum /*j*/ + 1];

		c_strlcpy (fs_csg_basedir, mapfullpath); // fs_csg_basedir	now ..."C:\galaxy\zircon\maps\aafter"
		File_URL_Edit_SlashesForward_Like_Unix (fs_csg_basedir); // fs_csg_basedir	"C:/galaxy/zircon/maps/aafter"

		strlcpy (mappath_fill, fs_csg_basedir, mappath_fill_size);
		if (String_Ends_With_Caseless (mappath_fill, ".map") == false)
			strlcat (mappath_fill, ".map", mappath_fill_size);

		// Baker: We get Windows style map path without .map
		// "C:/galaxy/zircon_beta_gcc.exe -csg C:\galaxy\zircon\maps\treefall_auto1
		// The parent folder should be our basedir?
		// Or is it 2 up?

		char *lastslash = strrchr(fs_csg_basedir, '/');
		if (lastslash) {
			*lastslash = 0;
			if (lastslash)
				lastslash = strrchr(fs_csg_basedir, '/');
			*lastslash = 0;
		}

		c_strlcpy (fs_basedir, fs_csg_basedir);
		// fs_basedir	0x01f54810 "C:/galaxy/zircon"	char [260]
		// fs_csg_basedir	0x01f55238 "C:/galaxy/zircon"	char [260]

		int i = (int)strlen (fs_basedir);
		if (i > 0 && (fs_basedir[i-1] == '\\' || fs_basedir[i-1] == '/'))
			fs_basedir[i-1] = 0;
		//is_forced = true; // // Baker r1001: -nohome is the behavior on Windows and Mac
#if 1
		// _wchdir
		qbool isok = FS_chdir (fs_basedir);
		if (isok) {
			DebugPrintLinef ("Changed dir to %s", fs_basedir);
		}
		//chdir (fs_basedir); // Baker: We are in the "c:/galaxy/zircon" here!  (not c:\galaxy!)
#endif
}

unsigned Color_From_Bytes (byte red, byte green, byte blue, byte alpha)
{
	return ((unsigned)red + ((unsigned)green << 8) + ((unsigned)blue << 16) + ((unsigned)alpha << 24));
}

