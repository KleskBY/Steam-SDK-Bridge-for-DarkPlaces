// cl_particles_effectinfo_load.c.h

static void CL_Particles_ParseEffectInfo(ccs *textstart, ccs *textend, ccs *filename)
{
	int arrayindex;
	int argc;
	int i;
	int linenumber;
	particleeffectinfo_t *info = NULL;
	const char *text = textstart;
	char argv[16][1024];
	for (linenumber = 1; ; linenumber ++) {
		argc = 0;
		for (arrayindex = 0;arrayindex < 16;arrayindex++)
			argv[arrayindex][0] = 0;

		for (;;) {
			if (!COM_ParseToken_Simple(&text, true, false, true))
				return;
			if (String_Match(com_token, "\n"))
				break;
			if (argc < 16) {
				c_strlcpy (argv[argc], com_token);
				argc++;
			}
		} // while
		if (argc < 1)
			continue;

// Baker: checkparms should be called parse_x_or_break -- or something
#define checkparms(n)			if (argc != (n)) {Con_PrintLinef ("%s:%d: error while parsing: %s given %d parameters, should be %d parameters", filename, linenumber, argv[0], argc, (n));break;}
#define readints(array, n)		checkparms(n+1);for (arrayindex = 0;arrayindex < argc - 1;arrayindex++) array[arrayindex] = strtol(argv[1+arrayindex], NULL, 0)
#define readfloats(array, n)	checkparms(n+1);for (arrayindex = 0;arrayindex < argc - 1;arrayindex++) array[arrayindex] = atof(argv[1+arrayindex])
#define readint(var)			checkparms(2);var = strtol(argv[1], NULL, 0)
#define readfloat(var)			checkparms(2);var = atof(argv[1])
#define readbool(var)			checkparms(2);var = strtol(argv[1], NULL, 0) != 0

		if (String_Match(argv[0], "effect")) {

#if 1 // Baker: info is going to get stomped.  Seal particle if applicable.
			// NO: Have tex2 copy the values.
			//if (info && Have_Flag (info->info->einfo_flags, PARTICLEEFFECT_TEX2_ACTIVE_8))
				
#endif


			int effectnameindex;
			checkparms(2);
			if (numparticleeffectinfo >= MAX_PARTICLEEFFECTINFO_8192) {
				Con_PrintLinef ("%s:%d: too many effects!", filename, linenumber);
				break;
			}
			for (effectnameindex = 1; effectnameindex < MAX_PARTICLEEFFECTNAME_4096; effectnameindex ++) {
				if (particleeffectname[effectnameindex][0]) {
					if (String_Match(particleeffectname[effectnameindex], argv[1]))
						break;
				} else {
					c_strlcpy (particleeffectname[effectnameindex], argv[1]);
					break;
				}
			} // for

			// if we run out of names, abort
			if (effectnameindex == MAX_PARTICLEEFFECTNAME_4096) {
				Con_PrintLinef ("%s:%d: too many effects!", filename, linenumber);
				break;
			}

			for (i = 0; i < numparticleeffectinfo; i ++) {
				info = particleeffectinfo + i;
				if (!Have_Flag (info->einfo_flags, PARTICLEEFFECT_DEFINED))
					if (info->effectnameindex == effectnameindex) // Baker: Presumably we found an undefined slot but what is this?
						break;
			} // for

			if (i < numparticleeffectinfo)
				continue;

/// Baker: This is assignment
			info = particleeffectinfo + numparticleeffectinfo ++;
			// copy entire info from baseline, then fix up the nameindex
// Baker: This is a struct copy
			*info = baselineparticleeffectinfo; 
			info->effectnameindex = effectnameindex;
			continue;
		}
		else if (info == NULL) {
			Con_PrintLinef ("%s:%d: command %s encountered before effect", filename, linenumber, argv[0]);
			break;
		}

		Flag_Add_To (info->einfo_flags, PARTICLEEFFECT_DEFINED);
		if (String_Match(argv[0], "countabsolute")) {readfloat(info->countabsolute);}
		else if (String_Match(argv[0], "count")) {readfloat(info->countmultiplier);}
		else if (String_Match(argv[0], "type"))
		{
			checkparms(2);
			if		(String_Match(argv[1], "alphastatic"))		info->particletype = pt_alphastatic;
			else if (String_Match(argv[1], "static"))			info->particletype = pt_static;
			else if (String_Match(argv[1], "spark"))			info->particletype = pt_spark;
			else if (String_Match(argv[1], "beam"))				info->particletype = pt_beam;
			else if (String_Match(argv[1], "rain"))				info->particletype = pt_rain;
			else if (String_Match(argv[1], "raindecal"))		info->particletype = pt_raindecal;
			else if (String_Match(argv[1], "snow"))				info->particletype = pt_snow;
			else if (String_Match(argv[1], "bubble"))			info->particletype = pt_bubble;
			else if (String_Match(argv[1], "blood")) {
																info->particletype = pt_blood;
																info->gravity = 1;
			}
			else if (String_Match(argv[1], "smoke"))			info->particletype = pt_smoke;
			else if (String_Match(argv[1], "decal"))			info->particletype = pt_decal;
			else if (String_Match(argv[1], "entityparticle"))	info->particletype = pt_entityparticle;
			else Con_PrintLinef ("%s:%d: unrecognized particle type %s", filename, linenumber, argv[1]);
			info->blendmode = particletype[info->particletype].blendmode;
			info->orientation = particletype[info->particletype].orientation;
		}
#if 1
		else if (String_Match(argv[0], "orientation")) {
			checkparms(2);
			if (String_Match(argv[1], "billboard"))		info->orientation = PORIENT_BILLBOARD_0;
			else if (String_Match(argv[1], "upright"))	info->orientation = PORIENT_UPRIGHT; // SPRITELY1
			else if (String_Match(argv[1], "spark"))	info->orientation = PORIENT_SPARK;
			else if (String_Match(argv[1], "oriented"))	info->orientation = PORIENT_ORIENTED_DOUBLESIDED;
			else if (String_Match(argv[1], "beam"))		info->orientation = PORIENT_HBEAM;
			else Con_PrintLinef ("%s:%d: unrecognized orientation %s", filename, linenumber, argv[1]);
		}
		else if (String_Match(argv[0], "blend")) { // TEX2
			checkparms(2);
			if (String_Match(argv[1], "alpha"))		info->blendmode = PBLEND_ALPHA_0;
			else if (String_Match(argv[1], "add"))		info->blendmode = PBLEND_ADD;
			else if (String_Match(argv[1], "invmod"))	info->blendmode = PBLEND_INVMOD;
			else Con_PrintLinef ("%s:%d: unrecognized blendmode %s", filename, linenumber, argv[1]);
		}
		else if (String_Match(argv[0], "color"))			{ readints(info->color, 2);		} // TEX2
		else if (String_Match(argv[0], "tex"))				{ readints(info->tex, 2);		} // TEX2
		else if (String_Match(argv[0], "size"))				{ readfloats(info->size, 2);	} // TEX2
		else if (String_Match(argv[0], "sizeincrease"))		{ readfloat(info->size[2]);		} // TEX2
		else if (String_Match(argv[0], "alpha"))			{ readfloats(info->alpha, 3);	} // TEX2
		else if (String_Match(argv[0], "time"))				{ readfloats(info->time, 2);	} // TEX2
#endif
#if 2
		else if (String_Match(argv[0], "tex2"))				{ 
			readints(info->b.tex, 2); 
#if 0 // Baker: This should work, yet it doesn't.
			Flag_Add_To (info->einfo_flags, PARTICLEEFFECT_TEX2_ACTIVE_8);
#endif

			// COPY VALUES AT TEX2 INITIALIZATION
			// TEX2 IS EXPECTED TO BE AT VERY END FOLLOWED BY IT'S PROPERTIES
			info->b.orientation = info->orientation;
			info->b.blendmode	= info->blendmode;
			info->b.color[0]	= info->color[0];
			info->b.color[1]	= info->color[1];
			info->b.size[0]		= info->size[0];
			info->b.size[1]		= info->size[1];
			info->b.size[2]		= info->size[2];
			info->b.alpha[0]	= info->alpha[0];
			info->b.alpha[1]	= info->alpha[1];
			info->b.alpha[2]	= info->alpha[2];
			info->b.time[0]		= info->time[0];
			info->b.time[1]		= info->time[1];
		}
		else if (String_Match(argv[0], "orientation2")) {
			checkparms(2);
			if (String_Match(argv[1], "billboard"))		info->b.orientation = PORIENT_BILLBOARD_0;
			else if (String_Match(argv[1], "upright"))	info->b.orientation = PORIENT_UPRIGHT; // SPRITELY1
			else if (String_Match(argv[1], "spark"))	info->b.orientation = PORIENT_SPARK;
			else if (String_Match(argv[1], "oriented"))	info->b.orientation = PORIENT_ORIENTED_DOUBLESIDED;
			else if (String_Match(argv[1], "beam"))		info->b.orientation = PORIENT_HBEAM;
			else Con_PrintLinef ("%s:%d: unrecognized orientation %s", filename, linenumber, argv[1]);
		}
		else if (String_Match(argv[0], "blend2")) {
			checkparms(2);
			if (String_Match(argv[1], "alpha"))			info->b.blendmode = PBLEND_ALPHA_0;
			else if (String_Match(argv[1], "add"))		info->b.blendmode = PBLEND_ADD;
			else if (String_Match(argv[1], "invmod"))	
				info->b.blendmode = PBLEND_INVMOD;
			else 
				Con_PrintLinef ("%s:%d: unrecognized blendmode %s", filename, linenumber, argv[1]);
		}
		else if (String_Match(argv[0], "color2"))			{ readints(info->b.color, 2); }
		else if (String_Match(argv[0], "size2"))			{ readfloats(info->b.size, 2); }
		else if (String_Match(argv[0], "sizeincrease2"))	{ readfloat(info->b.size[2]); }
		else if (String_Match(argv[0], "alpha2"))			{ readfloats(info->b.alpha, 3); }
		else if (String_Match(argv[0], "time2"))			{ readfloats(info->b.time, 2); }
#endif
		else if (String_Match(argv[0], "gravity"))					{readfloat(info->gravity);}
		else if (String_Match(argv[0], "bounce"))					{readfloat(info->bounce);}
		else if (String_Match(argv[0], "airfriction"))				{readfloat(info->airfriction);}
		else if (String_Match(argv[0], "liquidfriction"))			{readfloat(info->liquidfriction);}
		else if (String_Match(argv[0], "originoffset"))				{readfloats(info->originoffset, 3);}
		else if (String_Match(argv[0], "relativeoriginoffset"))		{readfloats(info->relativeoriginoffset, 3);}
		else if (String_Match(argv[0], "velocityoffset"))			{readfloats(info->velocityoffset, 3);}
		else if (String_Match(argv[0], "relativevelocityoffset"))	{readfloats(info->relativevelocityoffset, 3);}
		else if (String_Match(argv[0], "originjitter"))				{readfloats(info->originjitter, 3);}
		else if (String_Match(argv[0], "velocityjitter"))			{readfloats(info->velocityjitter, 3);}
		else if (String_Match(argv[0], "velocitymultiplier"))		{readfloat(info->velocitymultiplier);}
		else if (String_Match(argv[0], "lightradius"))				{readfloat(info->lightradiusstart);}
		else if (String_Match(argv[0], "lightradiusfade"))			{readfloat(info->lightradiusfade);}
		else if (String_Match(argv[0], "lighttime"))				{readfloat(info->lighttime);}
		else if (String_Match(argv[0], "lightcolor"))				{readfloats(info->lightcolor, 3);}
		else if (String_Match(argv[0], "lightshadow"))				{readbool(info->lightshadow);}
		else if (String_Match(argv[0], "lightcubemapnum"))			{readint(info->lightcubemapnum);}
		else if (String_Match(argv[0], "lightcorona"))				{readints(info->lightcorona, 2);}
		else if (String_Match(argv[0], "underwater"))				{checkparms(1);Flag_Add_To (info->einfo_flags, PARTICLEEFFECT_UNDERWATER_1);}
		else if (String_Match(argv[0], "notunderwater"))			{checkparms(1);Flag_Add_To (info->einfo_flags, PARTICLEEFFECT_NOTUNDERWATER_2);}
		else if (String_Match(argv[0], "trailspacing"))				{readfloat(info->trailspacing);if (info->trailspacing > 0) info->countmultiplier = 1.0f / info->trailspacing;}
		else if (String_Match(argv[0], "stretchfactor"))			{readfloat(info->stretchfactor);}
		else if (String_Match(argv[0], "staincolor"))				{readints(info->staincolor, 2);}
		else if (String_Match(argv[0], "stainalpha"))				{readfloats(info->stainalpha, 2);}
		else if (String_Match(argv[0], "stainsize"))				{readfloats(info->stainsize, 2);}
		else if (String_Match(argv[0], "staintex"))					{readints(info->staintex, 2);}
		else if (String_Match(argv[0], "stainless"))				{info->staintex[0] = -2; info->staincolor[0] = (unsigned int)-1; info->staincolor[1] = (unsigned int)-1; info->stainalpha[0] = 1; info->stainalpha[1] = 1; info->stainsize[0] = 2; info->stainsize[1] = 2; }
		else if (String_Match(argv[0], "rotate"))					{readfloats(info->rotate, 4);}

		// Baker: This should be called cubemap forcenearest
		else if (String_Match(argv[0], "forcenearest"))				{checkparms(1);Flag_Add_To (info->einfo_flags, PARTICLEEFFECT_FORCENEAREST_4);}
		else
			Con_PrintLinef ("%s:%d: skipping unknown command %s" NEWLINE, filename, linenumber, argv[0]);
#undef checkparms
#undef readints
#undef readfloats
#undef readint
#undef readfloat
	}




}




int CL_ParticleEffectIndexForName(const char *name)
{
	int i;
	for (i = 1;i < MAX_PARTICLEEFFECTNAME_4096 && particleeffectname[i][0];i++)
		if (String_Match(particleeffectname[i], name))
			return i;
	return 0;
}

const char *CL_ParticleEffectNameForIndex(int i)
{
	if (i < 1 || i >= MAX_PARTICLEEFFECTNAME_4096)
		return NULL;
	return particleeffectname[i];
}

// MUST match effectnameindex_t in client.h
static const char *standardeffectnames[EFFECT_TOTAL_36] = {
	"",
	"TE_GUNSHOT",
	"TE_GUNSHOTQUAD",
	"TE_SPIKE",
	"TE_SPIKEQUAD",
	"TE_SUPERSPIKE",
	"TE_SUPERSPIKEQUAD",
	"TE_WIZSPIKE",
	"TE_KNIGHTSPIKE",
	"TE_EXPLOSION",
	"TE_EXPLOSIONQUAD",
	"TE_TAREXPLOSION",
	"TE_TELEPORT",
	"TE_LAVASPLASH",
	"TE_SMALLFLASH",
	"TE_FLAMEJET",
	"EF_FLAME",
	"TE_BLOOD",
	"TE_SPARK",
	"TE_PLASMABURN",
	"TE_TEI_G3",
	"TE_TEI_SMOKE",
	"TE_TEI_BIGEXPLOSION",
	"TE_TEI_PLASMAHIT",
	"EF_STARDUST",
	"TR_ROCKET",
	"TR_GRENADE",
	"TR_BLOOD",
	"TR_WIZSPIKE",
	"TR_SLIGHTBLOOD",
	"TR_KNIGHTSPIKE",
	"TR_VORESPIKE",
	"TR_NEHAHRASMOKE",
	"TR_NEXUIZPLASMA",
	"TR_GLOWTRAIL",
	"SVC_PARTICLE"
};

CALLERS_ (r_part_newmap)
static void CL_Particles_LoadEffectInfo (const char *customfile)
{
	int i;
	int filepass;
	unsigned char *filedata;
	fs_offset_t filesize;
	char filename[MAX_QPATH_128];
	numparticleeffectinfo = 0;
	memset (particleeffectinfo, 0, sizeof(particleeffectinfo));
	memset (particleeffectname, 0, sizeof(particleeffectname));

	for (i = 0; i < EFFECT_TOTAL_36; i++)
		c_strlcpy (particleeffectname[i], standardeffectnames[i]);

	for (filepass = 0; ; filepass++) {
		if (filepass == 0) {
			if (customfile) // "effectinfo.txt" <------------- CLIENT
				c_strlcpy (filename, customfile);
			else
				c_strlcpy (filename, "effectinfo.txt");
		}
		else if (filepass == 1)
		{
			// Baker: start_effectinfo.txt
			if (!cl.worldbasename[0] || customfile)
				continue;
			c_dpsnprintf1 (filename, "%s_effectinfo.txt", cl.worldnamenoextension);
		}
		else
			break;
		filedata = FS_LoadFile(filename, tempmempool, fs_quiet_true, &filesize);
		if (!filedata)
			continue;
		CL_Particles_ParseEffectInfo((const char *)filedata, (const char *)filedata + filesize, filename);
		Mem_Free(filedata);
	} // for
}

int GetEffectList_Count (const char *s_prefix)
{
	//if (!r_refdef.scene.worldmodel || !cl.islocalgame || !cl.worldmodel) {
	//	return 0;
	//}

	// Baker: effectinfo is loaded even in disconnect state, Render_InitOnce starts it up

	stringlist_t	matchedSet;
	stringlistinit  (&matchedSet); // this does not allocate

	RELATED_ (GetShaderList_Count)

	// We cannot do comparisons here as this list is NOT SORTED nor UNIQUE
	for (int idx = 0; idx < numparticleeffectinfo; idx ++) {
		particleeffectinfo_t *info = particleeffectinfo + idx;
		if (Have_Flag (info->einfo_flags, PARTICLEEFFECT_DEFINED) == false)
			continue;

		char *s_name = particleeffectname[info->effectnameindex];

		if (s_prefix && s_prefix[0] && false == String_Starts_With_Caseless (s_name, s_prefix))
			continue;

		stringlistappend (&matchedSet, s_name);
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

// effectinfo_particleeffectnum_sv
static void CL_EffectInfo_particleeffectnum (cmd_state_t *cmd)
{
	ccs *s = cmd_argv_parm1;
	int num = SV_ParticleEffectIndex(s);
	if (num == 0)
		num = -1;
	Con_PrintLinef ("SV particleeffectnum for " QUOTED_S " is %d", s, num);
}

static void CL_EffectInfo_List_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) > 3) {
		Con_PrintLinef ("Usage: %s [optional partial]", Cmd_Argv(cmd, 0));
		return;
	}
	const char *s_effectpartial = NULL;
	if (Cmd_Argc(cmd) > 1)
		s_effectpartial = Cmd_Argv(cmd, 1);

	for (int idx = 0; idx < numparticleeffectinfo /*Baker: this is a global*/; idx ++) {
		particleeffectinfo_t *info = particleeffectinfo + idx;
		if (Have_Flag (info->einfo_flags, PARTICLEEFFECT_DEFINED) == false)
			continue;
		char *s_name = particleeffectname[info->effectnameindex];
		if (s_effectpartial && false == String_Contains_Caseless (s_name, s_effectpartial))
			continue;
		Con_PrintLinef ("%04d: %s", idx, s_name);
	} // for

}

int GetEffectInfoList_Count (ccs *s_prefix)
{
	if (!r_refdef.scene.worldmodel || !cl.islocalgame || !cl.worldmodel) {
		return 0;
	}

	stringlist_t	list = {0};

	// We cannot do comparisons here as this list is NOT SORTED
	for (int idx = 0; idx < numparticleeffectinfo /*Baker: this is a global*/; idx ++) {
		particleeffectinfo_t *info = particleeffectinfo + idx;
		if (Have_Flag (info->einfo_flags, PARTICLEEFFECT_DEFINED) == false)
			continue;
		ccs *sxy = particleeffectname[info->effectnameindex];
		//if (String_Starts_With_Caseless (sxy, s_prefix) == false)
		//	continue;
		//Con_PrintLinef ("%04d: %s", idx, s_name);
		stringlistappend (&list, sxy);
	} // for
		
	// SORT
	stringlistsort_unique (&list);

	int			num_matches = 0;

	for (int idx = 0; idx < list.numstrings; idx ++) {
		char *sxy = list.strings[idx];

		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		SPARTIAL_EVAL_

		num_matches ++;
	} // for

	stringlistfreecontents (&list);

	return num_matches;
}

//#pragma message ("effectinfo_dump EF_ENFORCERLASERIMPACT loses line number sync -- is it comments or what?  Carriage returns?")
//#pragma message ("We can effectinfo_dump with empty string and check for first line # mismatch")

static void CL_EffectInfo_Dump_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) == 1) {
		Con_PrintLinef ("Usage: %s [effect name]", Cmd_Argv(cmd, 0));
		return;
	}

	ccs *s_partial = Cmd_Argc(cmd) > 1 ? Cmd_Argv(cmd, 1) : "";

	// Find it -- there can be more than one
	RELATED_ (CL_Particles_LoadEffectInfo)
	fs_offset_t filesize;
	char *filename = "effectinfo.txt";
	char s_effectname_current[1024];
	// TODO: c_dpsnprintf1 (filename, "%s_effectinfo.txt", sv.worldnamenoextension);
	unsigned char *filedata = FS_LoadFile (filename, tempmempool, fs_quiet_true, &filesize);
	if (filedata == NULL) {
		Con_PrintLinef ("Couldn't open " QUOTED_S, filename);
		return;
	}

	const char *text = (const char *)filedata;
	const char *end_of_file_data = (const char *)filedata + filesize;
	char argv[16][1024];
	const char *text_at_start_of_effect = NULL; // Only set if we want this one
	int linenumber_at_start;
	int num_found = 0;
	for (int linenumber = 1; ; linenumber ++) {
		int argc = 0;
		for (int arrayindex = 0;arrayindex < 16;arrayindex++)
			argv[arrayindex][0] = 0;

		const char *text_before_this_line = text;
		while (1) {
			if (false == COM_ParseToken_Simple(&text, /*newline?*/ true, /*parse backslash?*/ false, /*parse comments?*/ true))
				goto donex;
			if (String_Match(com_token, NEWLINE))
				break; // exit while
			if (argc < 16) {
				c_strlcpy (argv[argc], com_token);
				argc++;
			}
		} // while
		if (argc < 1)
			continue; // Next line.

		//const char *text1 = text;
		RELATED_ (CL_Particles_LoadEffectInfo)
		if (false == String_Match(argv[0], "effect")) {
			continue;
		}

		// END OF CURRENT (PREVIOUS) EFFECT WE WANT TO TEXT CAPTURE
		if (text_at_start_of_effect) {
			// Completed an effect
			size_t text_block_length = text_before_this_line - text_at_start_of_effect;
			char *s_za = Z_StrDup_Len_Z (text_at_start_of_effect, text_block_length);

			// Baker: Kill toxic whitespace (TABS, CARRIAGES) but not NEWLINES
			for (char *whitespace_clean = s_za; *whitespace_clean; whitespace_clean ++) {
				if (*whitespace_clean < SPACE_CHAR_32 && *whitespace_clean != NEWLINE_CHAR_10)
					*whitespace_clean = SPACE_CHAR_32;
			} // for

			RELATED_ (ShaderText_Alloc , TAB_CHARACTER)

			Con_PrintLinef ("Line # %04d: %s" NEWLINE "%s",
				linenumber_at_start,
				s_effectname_current,
				s_za);
			Mem_FreeNull_ (s_za);

			text_at_start_of_effect = NULL;
		}

		// START OF NEW EFFECT
		const char *s_effectname = argv[1];
		int is_one_we_want = true;
		if (s_partial && s_partial[0])
			is_one_we_want = String_Starts_With (s_effectname, s_partial);

		if (is_one_we_want) {
			c_strlcpy (s_effectname_current, s_effectname);
			num_found ++;
			linenumber_at_start = linenumber;
			text_at_start_of_effect = text_before_this_line;
		}
	} // for

donex:
    ; // oblig for gcc

	const char *text_before_this_line = end_of_file_data;
	// Baker: Finish an effect ... we will need to test last line

	// END OF CURRENT (PREVIOUS) EFFECT WE WANT TO TEXT CAPTURE
	if (text_at_start_of_effect) {
		// Completed an effect
		size_t text_block_length = text_before_this_line - text_at_start_of_effect;
		char *s_za = Z_StrDup_Len_Z (text_at_start_of_effect, text_block_length);

		// Baker: Kill toxic whitespace (TABS, CARRIAGES) but not NEWLINES
		for (char *whitespace_clean = s_za; *whitespace_clean; whitespace_clean ++) {
			if (*whitespace_clean < SPACE_CHAR_32 && *whitespace_clean != NEWLINE_CHAR_10)
				*whitespace_clean = SPACE_CHAR_32;
		} // for

		RELATED_ (ShaderText_Alloc , TAB_CHARACTER)

		Con_PrintLinef ("Line # %04d: %s" NEWLINE "%s",
			linenumber_at_start,
			s_effectname_current,
			s_za);
		Mem_FreeNull_ (s_za);

		text_at_start_of_effect = NULL;
	}

	Con_PrintLinef ("Found %d matches", num_found);


	Mem_Free(filedata);
}



static void CL_Particles_LoadEffectInfo_f(cmd_state_t *cmd)
{
	CL_Particles_LoadEffectInfo (Cmd_Argc(cmd) > 1 ? Cmd_Argv(cmd, 1) : NULL);
}

RELATED_ (FS_JpegSplit_f, FS_GifToShader_f)

void CL_Particles_Count_f(cmd_state_t *cmd)
{
	Con_PrintLinef ("Num particles is %d", cl.num_particles);
}

void CL_Particles_Split_f (cmd_state_t *cmd) // Baker: "particles_split" command
{
	// Assume size 64.
	//unsigned char *loadimagepixelsbgra (const char *filename, qbool complain, qbool allowFixtrans, qbool convertsRGB, int *miplevel); // LoadTGA_BGRA
	byte *data_bgra = loadimagepixelsbgra (
		"particles/particlefont.tga",
		q_tx_complain_false,
		q_tx_allowfixtrans_false,
		q_tx_convertsrgb_false,
		q_tx_miplevel_null
	);

	int rows = atoi(cmd_argv_parm1); //
	int cols = atoi(cmd_argv_parm2);
	if (!rows) {
		rows = 8;
		Con_PrintLinef ("Defaulting row size %d", rows);
		
	}
	if (!cols) {
		cols = rows;
		Con_PrintLinef ("Defaulting col size %d", rows);
	}

	Con_PrintVarInt (rows);
	Con_PrintVarInt (cols);
	Con_PrintVarInt (image_width);
	Con_PrintVarInt (image_height);
	int xwidth = image_width / cols;
	int yheight = image_height / rows;
	Con_PrintVarInt (xwidth);
	Con_PrintVarInt (yheight);
	int numimages = rows * cols;
	Con_PrintLinef ("Size is %d x %d (w/h %d/%d) writing %d images to particles/images folder", image_width, image_height, xwidth, yheight, numimages);

	//int idx = 0;
	for (int r = 0; r < rows; r ++) {
		for (int c = 0; c < cols; c ++) {
			int idx = r * cols + c;
			int px = c * xwidth;
			int py = r * yheight;

			size_t image_buffer_size = RGBA_4 * xwidth * yheight;
			byte *image_buffer = (byte *)Mem_Alloc(tempmempool, image_buffer_size /*RGBA_4 * xwidth * yheight*/);
			//Image_Paste_Sub_Image (/*dest*/ image_buffer,
			//	new_width * pixel_bytes, 0, 0, pels, width * pixel_bytes, 0, 0, width, height, pixel_bytes);

			Image_Paste_Sub_Image (/*dest*/ image_buffer,
				xwidth * RGBA_4, /*xy*/ 0, 0, data_bgra, image_width * RGBA_4,
				//src:
				px, py, xwidth, yheight, RGBA_4);

			Image_Flip_RedGreen (image_buffer, image_buffer_size); //image_width * image_height *RGBA_4);

			//va_super (particles_images_xxx_dot_tga, MAX_OSPATH_EX_1024, "particles/images/%03d.tga", idx);
			va_super (particles_images_xxx_dot_png, MAX_QPATHX2_256, "particles/images/%03d.png", idx);
			//Image_WriteTGABGRA (particles_images_xxx_dot_tga, xwidth, yheight, image_buffer);

			qbool PNG_SaveImage_preflipped (const char *filename, int width, int height,
											qbool has_alpha, byte *data_rgba);

			PNG_SaveImage_preflipped (particles_images_xxx_dot_png, xwidth, yheight, /*alpha?*/ true, image_buffer);

			Mem_Free (image_buffer); // Baker: it's temppool so ok
		}
	}

	//Image_WriteTGABGRA ("particles/images/01.tga", image_width, image_height, data_bgra);

	Mem_Free (data_bgra); // Baker: it's temppool so ok

}


// Canvas size 2048 2048 .. 128 x 16
// Particle count = 256;

// image_split image default 32 ?
// image_split image 16
// image_split image 32 16
// imagepath/imagename/000.png
void CL_Image_Cols_Rows_Split_f (cmd_state_t *cmd)
{
//unsigned char *loadimagepixelsbgra (const char *filename, qbool complain, qbool allowFixtrans, qbool convertsRGB, int *miplevel); // LoadTGA_BGRA

	if (Cmd_Argc(cmd) < 2) {
		Con_PrintLinef ("image_split <imagename> [cols] [rows]");
		return;
	}
	ccs *filename = Cmd_Argv (cmd, 1);
	//ccs *s2 = Cmd_Argv (cmd, 2);
	int cols = Cmd_Argv (cmd, 2)[0] ? atoi (Cmd_Argv (cmd, 2)) : 16;
	int rows = Cmd_Argv (cmd, 3)[0] ? atoi (Cmd_Argv (cmd, 3)) : 16;

	if (!cols || !rows) {
		Con_PrintLinef ("Columns or rows is size 0");
		return;
	}

	byte *data_bgra_za = loadimagepixelsbgra (
		filename,
		q_tx_complain_false,
		q_tx_allowfixtrans_false,
		q_tx_convertsrgb_false,
		q_tx_miplevel_null
	);

	if (!data_bgra_za) {
		Con_PrintLinef ("couldn't load pixels (%s)", filename);
		return;
	}

	int src_width = image_width, src_height = image_height;
	Con_PrintLinef ("Image size is %d by %d", src_width, src_height);
	Con_PrintLinef ("Cols/Rows setting requested is %d columns and %d rows", cols, rows);
	double cell_width = (double)src_width / cols;
	double cell_height = (double)src_height / rows;
	int isok = Math_IsWholeNumber (cell_width) && Math_IsWholeNumber(cell_height);
	Con_PrintLinef ("Cols Rows %d x %d would be width height %g / %g (isok = %d)", cols, rows, cell_width, cell_height, isok);
	char out_path[MAX_OSPATH];
	c_strlcpy (out_path, filename);
	File_URL_Edit_Remove_Extension (out_path);

	if (!isok) {
		Con_PrintLinef ("The width and height is not whole number, adjust the pic or something");
		goto fail_cleanup_exit;
	}

	for (int c = 0; c < cols; c ++) {
		for (int r = 0; r < rows; r ++) {
			int idx = c + r * cols;
			int x = c * cell_width;
			int y = r * cell_height;

			byte *image_buffer = (byte *)Mem_Alloc(tempmempool, RGBA_4 * cell_width * cell_height);

			Image_Paste_Sub_Image (/*dest*/ image_buffer,
				/*dest rowbytes*/ cell_width * RGBA_4, /*xy*/ 0, 0,
				/*src: */ data_bgra_za, /*rowbytes*/ src_width * RGBA_4, x, y, cell_width, cell_height, RGBA_4);

			va_super (particles_images_xxx_dot_png, MAX_QPATHX2_256, "%s/%03d.png", out_path, idx);

			PNG_SaveImage_preflipped (particles_images_xxx_dot_png, cell_width, cell_height, q_has_alpha_true, image_buffer);

			Mem_Free (image_buffer); // Baker: it's temppool so ok
		}
	}


	//Con_PrintLinef ("Splitting %s to %s using w = %d h  = %d",
	//	imagename,
	//	imagepath,
	//	w,
	//	h
	//);


fail_cleanup_exit:
	Mem_Free (data_bgra_za); // Baker: it's temppool so ok
}


void CL_Particles_Join_f (cmd_state_t *cmd)
{
	#define loadimagepixels_typical(filename) loadimagepixelsbgra(filename, q_tx_complain_false, q_tx_allowfixtrans_false, q_tx_convertsrgb_false, q_tx_miplevel_null)
	byte *loadimagepixelsbgra (ccs *filename, qbool complain, qbool allowFixtrans, qbool convertsRGB, int *miplevel); // LOAD
	qbool PNG_SaveImage_preflipped (ccs *filename, int width, int height, qbool has_alpha, byte *data_rgba); // SAVE
	ccs *particles_particlefont_txt = "particles/images/particlefont.txt";
	//ccs *particles_particlefont_png = "particles/images/particlefont.png";
	ccs *particles_particlefont_tga = "particles/images/particlefont.tga";
	
	int ike_size = -1; // to be determined (was 128)

	for (int idx = 0; idx < MAX_PARTICLETEXTURES_256; idx ++) {
		va_super (filename, MAX_QPATHX2_256, "particles/images/%03d.png", idx);
		if (!FS_File_Exists (filename)) continue;

		byte *data_bgra = loadimagepixels_typical (filename); 	// Try to load it
		if (!data_bgra) continue; // It didn't load
		ike_size = image_width;
		break; // AND THANKS FOR ALL THE FISH!  Now get out ...
	} // next

	CMD_RETURN_IF_ (ike_size == -1, "Can't join any images in particles/images .. there are no PNG");

	int imgsize_full = ike_size * 16; // Should be 8192 for 512 * 16
	byte *image_buffer = (byte *)Mem_Alloc(tempmempool, imgsize_full /*8192*/ * imgsize_full /*8192*/ * RGBA_4);

	baker_string_t *bs_particlefont_txt = BakerString_Create_Malloc (""); // For particlefont.txt
	#define COLS_16		16
	#define ROWS_16		16

	for (int idx = 0; idx < MAX_PARTICLETEXTURES_256; idx ++) {
		int r = idx / COLS_16;
		int c = idx % ROWS_16;
		int paste_x = c * ike_size /*512*/;
		int paste_y = r * ike_size /*512*/;
		int paste_xb = paste_x + ike_size /*512*/;
		int paste_yb = paste_y + ike_size /*512*/;
		float fpaste_x = (paste_x + 0.5) / imgsize_full /*8192*/; // texco
		float fpaste_y = (paste_y + 0.5) / imgsize_full /*8192*/;
		float fpaste_xb = (paste_xb - 0.5) / imgsize_full /*8192*/;
		float fpaste_yb = (paste_yb - 0.5) / imgsize_full /*8192*/;

		// Append the texture coordinates to the txt file buffer
		BakerString_CatCFmt (bs_particlefont_txt, 
			"%d " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT NEWLINE,
			  idx, fpaste_x, fpaste_y, fpaste_xb, fpaste_yb);

		va_super (filename, MAX_QPATHX2_256, "particles/images/%03d.png", idx);
		if (!FS_File_Exists (filename)) continue; // SKIP ME

		byte *data_bgra = loadimagepixels_typical (filename); 	// Try to load it
		if (!data_bgra) continue; // It didn't load

		Image_Flip_RedGreen (data_bgra, ike_size /*512*/ * ike_size /*512*/ * RGBA_4);

		// Paste it to main, assume size 128 x 128
		int dest_rowbytes = imgsize_full /*8192*/ * RGBA_4;
		int src_rowbytes = ike_size /*512*/ * RGBA_4;
		Image_Paste_Sub_Image (/*dest*/ image_buffer,
			dest_rowbytes, /*xy*/ paste_x, paste_y, data_bgra, src_rowbytes,
			//src coordinates:
			0, 0, ike_size /*512*/ /*xwidth*/, ike_size /*512*/ /*yheight*/, RGBA_4);

		Mem_Free (data_bgra); // Baker: it's temppool so ok
		
		#undef COLS_16
		#undef ROWS_16
	} // next

#if 0
	if  (PNG_SaveImage_preflipped (particles_particlefont_png, imgsize_full /*8192*/, imgsize_full /*8192*/, /*alpha?*/ true, image_buffer))
		Con_PrintLinef ("Wrote %s (and .txt) files must be copied to particles folder and type cl_particles_reloadeffects for it to take effect", particles_particlefont_png);
	else
		Con_PrintLinef ("Couldn't write %s", particles_particlefont_png);
#endif

	//Image_CopyMux (buffer2, buffer1, width, height, flipx, flipy, flipdiagonal, 4, 4, indices);
	Image_Flip_RedGreen (image_buffer, imgsize_full /*8192*/ * imgsize_full /*8192*/ * RGBA_4);
	if (Image_WriteTGABGRA(particles_particlefont_tga, imgsize_full /*8192*/, imgsize_full /*8192*/, image_buffer))
		Con_PrintLinef ("Wrote %s (and .txt) files must be copied to particles folder and type vid_restart for it to take effect", particles_particlefont_tga);
	else
		Con_PrintLinef ("Couldn't write %s", particles_particlefont_tga);

	// Write the particles/images/particlefont.txt
	qfile_t *file = FS_OpenRealFile(particles_particlefont_txt, "w", fs_quiet_FALSE);
	if (!file) {
		Con_PrintLinef (CON_ERROR "unable to write file " QUOTED_S, particles_particlefont_txt);
	} else {
		FS_Write (file, bs_particlefont_txt->string, bs_particlefont_txt->length);
	}

	FS_Close(file);
	BakerString_Destroy_And_Null_It (&bs_particlefont_txt);

	Mem_Free (image_buffer); // Baker: it's temppool so ok
}

CALLERS_ (CL_EffectsLevel_f M_Menu_OptionsNova_AdjustSliders)
void Set_Effects (int level)
{
	switch (level) {
	case 0:
		
		Cbuf_InsertTextLine (cmd_local, "cl_decals 0; cl_decals_fadetime 1; cl_decals_models 0; cl_particles_quality 0.8; gl_flashblend 0; mod_q3bsp_nolightmaps 0; r_bloom 0; r_coronas_occlusionquery 0; r_depthfirst 0; r_drawdecals_drawdistance 500; r_drawparticles_drawdistance 2000; r_glsl_deluxemapping 0; r_glsl_offsetmapping 0; r_glsl_offsetmapping_reliefmapping 0; r_motionblur 0; r_shadow_realtime_dlight 0; r_shadow_realtime_dlight_shadows 0; r_shadow_realtime_world 0; r_shadow_realtime_world_shadows 0; r_shadow_realtime_world_lightmaps 0; r_shadow_shadowmapping 0; r_shadow_usenormalmap 0; r_showsurfaces 0; r_sky 1; r_subdivisions_tolerance 4");
		
		if (gamemode_is_galaxy || gamemode == GAME_QUAKE3_QUAKE1) { // GALAXY #3
			Cbuf_InsertTextLine (cmd_local, "r_water 1; r_water_resolutionmultiplier 1.0; r_shadow_gloss 0");
		} else {
			Cbuf_InsertTextLine (cmd_local, "r_water 0; r_water_resolutionmultiplier 0.5; r_shadow_gloss 0");
		}
		break;

	case 1:  // Vanilla level
		Cbuf_AddTextLine (cmd_local, "cl_decals 0; cl_decals_fadetime 1; cl_decals_models 0; cl_particles_quality 0.990; gl_flashblend 0; mod_q3bsp_nolightmaps 0; r_bloom 0; r_coronas_occlusionquery 0; r_depthfirst 0; r_drawdecals_drawdistance 500; r_drawparticles_drawdistance 2000; r_glsl_deluxemapping 0; r_glsl_offsetmapping 0; r_glsl_offsetmapping_reliefmapping 0; r_motionblur 0; r_shadow_realtime_dlight 1; r_shadow_realtime_dlight_shadows 0; r_shadow_realtime_world 0; r_shadow_realtime_world_shadows 0; r_shadow_realtime_world_lightmaps 0; r_shadow_shadowmapping 0; r_shadow_usenormalmap 1; r_showsurfaces 0; r_sky 1; r_subdivisions_tolerance 4");
		if (gamemode_is_galaxy || gamemode == GAME_QUAKE3_QUAKE1) { // GALAXY #3
			Cbuf_InsertTextLine (cmd_local, "r_water 1; r_water_resolutionmultiplier 1.0; r_shadow_gloss 0");
		} else {
			Cbuf_InsertTextLine (cmd_local, "r_water 1; r_water_resolutionmultiplier 0.5; r_shadow_gloss 0");
		}
		break;


	case 2:  // default level
		Cbuf_AddTextLine (cmd_local, "cl_decals 1; cl_decals_fadetime 1; cl_decals_models 0; cl_particles_quality 1; gl_flashblend 0; mod_q3bsp_nolightmaps 0; r_bloom 0; r_coronas_occlusionquery 0; r_depthfirst 0; r_drawdecals_drawdistance 500; r_drawparticles_drawdistance 2000; r_glsl_deluxemapping 0; r_glsl_offsetmapping 0; r_glsl_offsetmapping_reliefmapping 0; r_motionblur 0; r_shadow_realtime_dlight 1; r_shadow_realtime_dlight_shadows 0; r_shadow_realtime_world 0; r_shadow_realtime_world_shadows 0; r_shadow_realtime_world_lightmaps 0; r_shadow_shadowmapping 0; r_shadow_usenormalmap 1; r_showsurfaces 0; r_sky 1; r_subdivisions_tolerance 4");
		if (gamemode_is_galaxy || gamemode == GAME_QUAKE3_QUAKE1) { // GALAXY #3
			Cbuf_InsertTextLine (cmd_local, "r_water 1; r_water_resolutionmultiplier 1.0; r_shadow_gloss 1");
		} else {
			Cbuf_InsertTextLine (cmd_local, "r_water 1; r_water_resolutionmultiplier 0.5; r_shadow_gloss 1");
		}
		break;

	case 3:  // Good r_depthfirst r_coronas_occlusionquery r_bloom cl_decals_fadetime r_glsl_deluxemapping
		Cbuf_InsertTextLine (cmd_local, "cl_decals 1; cl_decals_fadetime 4; cl_decals_models 0; cl_particles_quality 1.01; gl_flashblend 0; mod_q3bsp_nolightmaps 0; r_bloom 1; r_coronas_occlusionquery 1; r_depthfirst 2; r_drawdecals_drawdistance 500; r_drawparticles_drawdistance 2000; r_glsl_deluxemapping 1; r_glsl_offsetmapping 0; r_glsl_offsetmapping_reliefmapping 0; r_motionblur 0.4; r_shadow_gloss 1; r_shadow_realtime_dlight 1; r_shadow_realtime_dlight_shadows 0; r_shadow_realtime_world 0; r_shadow_realtime_world_shadows 0; r_shadow_realtime_world_lightmaps 0; r_shadow_shadowmapping 0; r_shadow_usenormalmap 1; r_showsurfaces 0; r_sky 1; r_subdivisions_tolerance 4; r_water 1; r_water_resolutionmultiplier 1");
		Cbuf_InsertTextLine (cmd_local, "r_shadow_gloss 1");
		break;

	case 4:  // Gooderx r_glsl_offsetmapping r_subdivisions_tolerance
		Cbuf_InsertTextLine (cmd_local, "cl_decals 1; cl_decals_fadetime 10; cl_decals_models 0; cl_particles_quality 1.02; gl_flashblend 0; mod_q3bsp_nolightmaps 0; r_bloom 1; r_coronas_occlusionquery 1; r_depthfirst 2; r_drawdecals_drawdistance 500; r_drawparticles_drawdistance 2000; r_glsl_deluxemapping 1; r_glsl_offsetmapping 1; r_glsl_offsetmapping_reliefmapping 0; r_motionblur 0.4; r_shadow_gloss 1; r_shadow_realtime_dlight 1; r_shadow_realtime_dlight_shadows 1; r_shadow_realtime_world 1; r_shadow_realtime_world_shadows 0; r_shadow_realtime_world_lightmaps 1; r_shadow_shadowmapping 1; r_shadow_usenormalmap 1; r_showsurfaces 0; r_sky 1; r_subdivisions_tolerance 2; r_water 1; r_water_resolutionmultiplier 1");
		Cbuf_InsertTextLine (cmd_local, "r_shadow_gloss 1");
		break;

	case 5: // Highest r_shadow_realtime_world
	case 6: // Fall thru
	case 7: // Fall thru
		Cbuf_InsertTextLine (cmd_local, "cl_decals 1; cl_decals_fadetime 10; cl_decals_models 1; cl_particles_quality 1.03; gl_flashblend 0; mod_q3bsp_nolightmaps 0; r_bloom 1; r_coronas_occlusionquery 1; r_depthfirst 2; r_drawdecals_drawdistance 500; r_drawparticles_drawdistance 3000; r_glsl_deluxemapping 1; r_glsl_offsetmapping 1; r_glsl_offsetmapping_reliefmapping 1; r_motionblur 0.4; r_shadow_gloss 1; r_shadow_realtime_dlight 1; r_shadow_realtime_dlight_shadows 1; r_shadow_realtime_world 1; r_shadow_realtime_world_shadows 1; r_shadow_shadowmapping 1; r_shadow_usenormalmap 1; r_showsurfaces 0; r_sky 1; r_subdivisions_tolerance 1; r_water 1; r_water_resolutionmultiplier 1");
		Cbuf_InsertTextLine (cmd_local, "r_shadow_gloss 1");

		switch (level) {
		default:
				Cbuf_InsertTextLine (cmd_local, "r_shadow_realtime_world_lightmaps 1");
				break;
		case 6: Cbuf_InsertTextLine (cmd_local, "r_shadow_realtime_world_lightmaps 0.5");
				break;
		case 7: Cbuf_InsertTextLine (cmd_local, "r_shadow_realtime_world_lightmaps 0.25");
				break;
		case 8: Cbuf_InsertTextLine (cmd_local, "r_shadow_realtime_world_lightmaps 0.125");
				break;
		}

		break;
	} // sw level
	Cbuf_InsertTextLine (cmd_local, NEWLINE);
}

void CL_EffectsLevel_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) < 2) {
printme:
		Con_PrintLinef ("cl_effectlevels <num 1-6>: Set effects level");
		Con_PrintLinef ("3) Normal");
		Con_PrintLinef ("4) +Bloom, +Deluxe");
		Con_PrintLinef ("5) +Offset Mapping");
		Con_PrintLinef ("6) +Realtime Light");
		Con_PrintLinef ("================"); // March 27 2025 added these
		Con_PrintLinef ("7) +Realtime Light 50%%");
		Con_PrintLinef ("8) +Realtime Light 25%%");
		Con_PrintLinef ("9) +Realtime Light 12.5%%");
		return;
	}
	ccs *sval = Cmd_Argv(cmd, 1);
	int val = atoi(sval) -1 ;
	int origval = val;
	val = bound (0, val, (5 + 3) ); // Lock to range
	if (origval != val) {
		Con_PrintLinef ("Out of range");
		goto printme;
	}
	Set_Effects (val);
}

RELATED_ (VM_CL_particleeffectnum CL_ParticleEffectIndexForName)
CALLERS_ (/*exclusive*/ CL_ParsePointParticles)

ccs *CL_Particles_EffectIndex_To_Name (int index)
{
	// not MAX_PARTICLEEFFECTINFO_8192, use MAX_PARTICLEEFFECTNAME_4096
	if (in_range_beyond (1, index, MAX_PARTICLEEFFECTNAME_4096) == false)
		return "UNKNOWN";
	//particleeffectinfo_t *info = &particleeffectinfo[index];
	//if (in_range_beyond (0, info->effectnameindex, numparticleeffectinfo) == false)
	//	return "UNKNOWN";

	ccs *s_name = particleeffectname[index];
	return s_name;
}


