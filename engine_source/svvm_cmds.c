#include "quakedef.h"

#include "prvm_cmds.h"
#include "jpeg.h"

//============================================================================
// Server



const char *vm_sv_extensions[] = {
	"BX_WAL_SUPPORT",
	"DP_BUTTONCHAT",
	"DP_BUTTONUSE",
	"DP_CL_LOADSKY",
	"DP_CON_ALIASPARAMETERS",
	"DP_CON_BESTWEAPON",
	"DP_CON_EXPANDCVAR",
	"DP_CON_SET",
	"DP_CON_SETA",
	"DP_CON_STARTMAP",
	"DP_COVERAGE",
	"DP_CRYPTO",
	"DP_CSQC_BINDMAPS",
	"DP_CSQC_ENTITYWORLDOBJECT",
	"DP_CSQC_ENTITYMODELLIGHT",
	"DP_CSQC_ENTITYTRANSPARENTSORTING_OFFSET",
	"DP_CSQC_MAINVIEW",
	"DP_CSQC_MINFPS_QUALITY",
	"DP_CSQC_MULTIFRAME_INTERPOLATION",
	"DP_CSQC_BOXPARTICLES",
	"DP_CSQC_SPAWNPARTICLE",
	"DP_CSQC_QUERYRENDERENTITY",
	"DP_CSQC_ROTATEMOVES",
	"DP_CSQC_SETPAUSE",
	"DP_CSQC_V_CALCREFDEF_WIP1",
	"DP_CSQC_V_CALCREFDEF_WIP2",
	"DP_EF_ADDITIVE",
	"DP_EF_BLUE",
	"DP_EF_DOUBLESIDED",
	"DP_EF_DYNAMICMODELLIGHT",
	"DP_EF_FLAME",
	"DP_EF_FULLBRIGHT",
	"DP_EF_NODEPTHTEST",
	"DP_EF_NODRAW",
	"DP_EF_NOGUNBOB",
	"DP_EF_NOSELFSHADOW",
	"DP_EF_NOSHADOW",
	"DP_EF_RED",
	"DP_EF_RESTARTANIM_BIT",
	"DP_EF_STARDUST",
	"DP_EF_TELEPORT_BIT",
	"DP_ENT_ALPHA",
	"DP_ENT_COLORMOD",
	"DP_ENT_CUSTOMCOLORMAP",
	"DP_ENT_EXTERIORMODELTOCLIENT",
	"DP_ENT_GLOW",
	"DP_ENT_GLOWMOD",
	"DP_ENT_LOWPRECISION",
	"DP_ENT_SCALE",
	"DP_ENT_TRAILEFFECTNUM",
	"DP_ENT_VIEWMODEL",
	"DP_GFX_EXTERNALTEXTURES",
	"DP_GFX_EXTERNALTEXTURES_PERMAP",
	"DP_GFX_FOG",
	"DP_GFX_MODEL_INTERPOLATION",
	"DP_GFX_QUAKE3MODELTAGS",
	"DP_GFX_SKINFILES",
	"DP_GFX_SKYBOX",
	"DP_GFX_FONTS",
	"DP_GFX_FONTS_FREETYPE",
	"DP_UTF8",
	"DP_FONT_VARIABLEWIDTH",
	"DP_HALFLIFE_MAP",
	"DP_HALFLIFE_MAP_CVAR",
	"DP_HALFLIFE_SPRITE",
	"DP_INPUTBUTTONS",
	"DP_LIGHTSTYLE_STATICVALUE",
	"DP_LITSPRITES",
	"DP_LITSUPPORT",
	"DP_MONSTERWALK",
	"DP_MOVETYPEBOUNCEMISSILE",
	"DP_MOVETYPEFLYWORLDONLY",
	"DP_MOVETYPEFOLLOW",
	"DP_NULL_MODEL",
	"DP_QC_ASINACOSATANATAN2TAN",
	"DP_QC_AUTOCVARS",
	"DP_QC_CHANGEPITCH",
	"DP_QC_CMD",
	"DP_QC_COPYENTITY",
	"DP_QC_CRC16",
	"DP_QC_CVAR_DEFSTRING",
	"DP_QC_CVAR_DESCRIPTION",
	"DP_QC_CVAR_STRING",
	"DP_QC_CVAR_TYPE",
	"DP_QC_DIGEST",
	"DP_QC_DIGEST_SHA256",
	"DP_QC_EDICT_NUM",
	"DP_QC_ENTITYDATA",
	"DP_QC_ENTITYSTRING",
	"DP_QC_ETOS",
	"DP_QC_EXTRESPONSEPACKET",
	"DP_QC_FINDBOX",
	"DP_QC_FINDCHAIN",
	"DP_QC_FINDCHAINFLAGS",
	"DP_QC_FINDCHAINFLOAT",
	"DP_QC_FINDCHAIN_TOFIELD",
	"DP_QC_FINDFLAGS",
	"DP_QC_FINDFLOAT",
	"DP_QC_FS_SEARCH",
	"DP_QC_FS_SEARCH_PACKFILE",
	"DP_QC_GETLIGHT",
	"DP_QC_GETSURFACE",
	"DP_QC_GETSURFACETRIANGLE",
	"DP_QC_GETSURFACEPOINTATTRIBUTE",
	"DP_QC_GETTAGINFO",
	"DP_QC_GETTAGINFO_BONEPROPERTIES",
	"DP_QC_GETTIME",
	"DP_QC_GETTIME_CDTRACK",
	"DP_QC_I18N",
	"DP_QC_LOG",
	"DP_QC_MINMAXBOUND",
	"DP_QC_MULTIPLETEMPSTRINGS",
#if 111 // Baker r0061: Classic DarkPlaces physics

#else
"DP_QC_NUDGEOUTOFSOLID",
#endif
	"DP_QC_NUM_FOR_EDICT",
	"DP_QC_RANDOMVEC",
	"DP_QC_SINCOSSQRTPOW",
	"DP_QC_SPRINTF",
	"DP_QC_STRFTIME",
	"DP_QC_STRINGBUFFERS",
	"DP_QC_STRINGBUFFERS_CVARLIST",
	"DP_QC_STRINGBUFFERS_EXT_WIP",
	"DP_QC_STRINGCOLORFUNCTIONS",
	"DP_QC_STRING_CASE_FUNCTIONS",
	"DP_QC_STRREPLACE",
	"DP_QC_TOKENIZEBYSEPARATOR",
	"DP_QC_TOKENIZE_CONSOLE",
	"DP_QC_TRACEBOX",
	"DP_QC_TRACETOSS",
	"DP_QC_TRACE_MOVETYPE_HITMODEL",
	"DP_QC_TRACE_MOVETYPE_WORLDONLY",
	"DP_QC_UNLIMITEDTEMPSTRINGS",
	"DP_QC_URI_ESCAPE",
	"DP_QC_URI_GET",
	"DP_QC_URI_POST",
	"DP_QC_VECTOANGLES_WITH_ROLL",
	"DP_QC_VECTORVECTORS",
	"DP_QC_WHICHPACK",
	"DP_QUAKE2_MODEL",
	"DP_QUAKE2_SPRITE",
	"DP_QUAKE3_MAP",
	"DP_QUAKE3_MODEL",
	"DP_REGISTERCVAR",
	"DP_SKELETONOBJECTS",
	"DP_SND_DIRECTIONLESSATTNNONE",
	"DP_SND_FAKETRACKS",
	"DP_SND_SOUND7_WIP1",
	"DP_SND_SOUND7_WIP2",
	"DP_SND_OGGVORBIS",
	"DP_SND_SETPARAMS",
	"DP_SND_STEREOWAV",
	"DP_SND_GETSOUNDTIME",
	"DP_VIDEO_DPV",
	"DP_VIDEO_SUBTITLES",
	"DP_SOLIDCORPSE",
	"DP_SPRITE32",
	"DP_SV_BOTCLIENT",
	"DP_SV_BOUNCEFACTOR",
	"DP_SV_CLIENTCAMERA",
	"DP_SV_CLIENTCOLORS",
	"DP_SV_CLIENTNAME",
	"DP_SV_CMD",
	"DP_SV_CUSTOMIZEENTITYFORCLIENT",
	"DP_SV_DISABLECLIENTPREDICTION",
	"DP_SV_DISCARDABLEDEMO",
	"DP_SV_DRAWONLYTOCLIENT",
	"DP_SV_DROPCLIENT",
	"DP_SV_EFFECT",
	"DP_SV_ENTITYCONTENTSTRANSITION",
	"DP_SV_MODELFLAGS_AS_EFFECTS",
	"DP_SV_MOVETYPESTEP_LANDEVENT",
	"DP_SV_NETADDRESS",
	"DP_SV_NODRAWTOCLIENT",
	"DP_SV_ONENTITYNOSPAWNFUNCTION",
	"DP_SV_ONENTITYPREPOSTSPAWNFUNCTION",
	"DP_SV_PING",
	"DP_SV_PING_PACKETLOSS",
	"DP_SV_PLAYERPHYSICS",
	"DP_PHYSICS_ODE",
	"DP_SV_POINTPARTICLES",
	"DP_SV_POINTSOUND",
	"DP_SV_PRECACHEANYTIME",
	"DP_SV_PRINT",
	"DP_SV_PUNCHVECTOR",
	"DP_SV_QCSTATUS",
	"DP_SV_ROTATINGBMODEL",
	"DP_SV_SETCOLOR",
	"DP_SV_SHUTDOWN",
	"DP_SV_SLOWMO",
	"DP_SV_SPAWNFUNC_PREFIX",
	"DP_SV_WRITEPICTURE",
	"DP_SV_WRITEUNTERMINATEDSTRING",
	"DP_TE_BLOOD",
	"DP_TE_BLOODSHOWER",
	"DP_TE_CUSTOMFLASH",
	"DP_TE_EXPLOSIONRGB",
	"DP_TE_FLAMEJET",
	"DP_TE_PARTICLECUBE",
	"DP_TE_PARTICLERAIN",
	"DP_TE_PARTICLESNOW",
	"DP_TE_PLASMABURN",
	"DP_TE_QUADEFFECTS1",
	"DP_TE_SMALLFLASH",
	"DP_TE_SPARK",
	"DP_TE_STANDARDEFFECTBUILTINS",
	"DP_TRACE_HITCONTENTSMASK_SURFACEINFO"
	"DP_USERMOVETYPES",
	"DP_VIEWZOOM",
	"EXT_BITSHIFT",
	"FRIK_FILE",
	"FTE_CSQC_SKELETONOBJECTS",
	"FTE_QC_CHECKPVS",
	"FTE_STRINGS",
	"KRIMZON_SV_PARSECLIENTCOMMAND",
	"NEH_CMD_PLAY2",
	"NEH_RESTOREGAME",
	"NEXUIZ_PLAYERMODEL",
	"NXQ_GFX_LETTERBOX",
	"PRYDON_CLIENTCURSOR",
	"TENEBRAE_GFX_DLIGHTS",
	"TW_SV_STEPCONTROL",
	"ZQ_PAUSE",
	"DP_RM_CLIPGROUP",

// Baker r7084:
	"ZIRCON_RTLIGHT_BAKE",
	"ZIRCON_GAMECOMMANDS",

// Baker: Intermap
	"ZIRCON_INTERMAP_VERSION_1",			// Intermap mechanism
	"ZIRCON_PFLAGS_CORONA_ONLY_RENDERS",	// Trivial .. corona only renders.

	"ZIRCON_GETENTITY_1",					// Extra Zircon getentity fields
	"ZIRCON_DUNGEON_V1",					// .map compiling --- map_load, map_close, compile, etc.
	"ZIRCON_FORMS_V1",						// User-interface
	"ZIRCON_GETSURFACENUMPOINTS_FLAGS",		// 
	"KLESKBY_STEAM",
	NULL,
	
	//"EXT_CSQC" // not ready yet
};

/*
=================
VM_SV_setorigin

This is the only valid way to move an object without using the physics of the world (setting velocity and waiting).  Directly changing origin will not set internal links correctly, so clipping would be messed up.  This should be called when an object is spawned, and then only if it is teleported.

setorigin (entity, origin)
=================
*/
static void VM_SV_setorigin(prvm_prog_t *prog)
{
	prvm_edict_t	*e;

	VM_SAFEPARMCOUNT(2, VM_SV_setorigin);

	e = PRVM_G_EDICT(OFS_PARM0);
	if (e == prog->edicts)
	{
		VM_WarningLinef (prog, "setorigin: can not modify world entity");
		return;
	}
	if (e->free)
	{
		VM_WarningLinef (prog, "setorigin: can not modify free entity");
		return;
	}
	VectorCopy(PRVM_G_VECTOR(OFS_PARM1), PRVM_serveredictvector(e, origin));
	if (e->priv.required->mark == PRVM_EDICT_MARK_WAIT_FOR_SETORIGIN)
		e->priv.required->mark = PRVM_EDICT_MARK_SETORIGIN_CAUGHT;
	SV_LinkEdict(e);
}

// TODO: rotate param isnt used.. could be a bug. please check this and remove it if possible [1/10/2008 Black]
static void SetMinMaxSize (prvm_prog_t *prog, prvm_edict_t *e, float *min, float *max, qbool rotate)
{
	int		i;

	for (i=0 ; i<3 ; i++)
		if (min[i] > max[i])
			prog->error_cmd("SetMinMaxSize: backwards mins/maxs");

// set derived values
	VectorCopy (min, PRVM_serveredictvector(e, mins));
	VectorCopy (max, PRVM_serveredictvector(e, maxs));
	VectorSubtract (max, min, PRVM_serveredictvector(e, size));

	SV_LinkEdict(e);
}

/*
=================
VM_SV_setsize

the size box is rotated by the current angle
LadyHavoc: no it isn't...

setsize (entity, minvector, maxvector)
=================
*/
static void VM_SV_setsize(prvm_prog_t *prog)
{
	prvm_edict_t	*e;
	vec3_t mins, maxs;

	VM_SAFEPARMCOUNT(3, VM_SV_setsize);

	e = PRVM_G_EDICT(OFS_PARM0);
	if (e == prog->edicts)
	{
		VM_WarningLinef (prog, "setsize: can not modify world entity");
		return;
	}
	if (e->free)
	{
		VM_WarningLinef (prog, "setsize: can not modify free entity");
		return;
	}
	VectorCopy(PRVM_G_VECTOR(OFS_PARM1), mins);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM2), maxs);
	SetMinMaxSize(prog, e, mins, maxs, false);
}


/*
=================
VM_SV_setmodel

setmodel(entity, model)
=================
*/
static vec3_t quakemins = {-16, -16, -16}, quakemaxs = {16, 16, 16};

RELATED_ () // cl precache happens where?

void VMX_SV_setmodel (prvm_prog_t *prog, prvm_edict_t *e, const char *s_model)
{
	model_t	*mod;
	int		i;

	if (e == prog->edicts) {
		VM_WarningLinef (prog, "setmodel: can not modify world entity");
		return;
	}
	if (e->free) {
		VM_WarningLinef (prog, "setmodel: can not modify free entity");
		return;
	}
	i = SV_ModelIndex (s_model, PRECACHE_MODE_1);
	PRVM_serveredictstring(e, model) = PRVM_SetEngineString(prog, sv.model_precache[i]);
	PRVM_serveredictfloat(e, modelindex) = i;

	mod = SV_GetModelByIndex(i);

	if (mod)
	{
		if (mod->type != mod_alias || sv_gameplayfix_setmodelrealbox.integer)
			SetMinMaxSize(prog, e, mod->normalmins, mod->normalmaxs, /*rotate?*/ true);
		else
			SetMinMaxSize(prog, e, quakemins, quakemaxs, /*rotate?*/ true);
	}
	else
		SetMinMaxSize(prog, e, vec3_origin, vec3_origin, true);
}

static void VM_SV_setmodel(prvm_prog_t *prog)
{
	prvm_edict_t	*e;
	model_t	*mod;
	int		i;

	VM_SAFEPARMCOUNT(2, VM_SV_setmodel);

	e = PRVM_G_EDICT(OFS_PARM0);
	if (e == prog->edicts)
	{
		VM_WarningLinef (prog, "setmodel: can not modify world entity");
		return;
	}
	if (e->free)
	{
		VM_WarningLinef (prog, "setmodel: can not modify free entity");
		return;
	}
#ifdef _DEBUG
	ccs *s = PRVM_G_STRING(OFS_PARM1);
	if (s[0] == '*') {
		int j = 5;
	}
#endif
	i = SV_ModelIndex(PRVM_G_STRING(OFS_PARM1), PRECACHE_MODE_1);
	PRVM_serveredictstring(e, model) = PRVM_SetEngineString(prog, sv.model_precache[i]);
	PRVM_serveredictfloat(e, modelindex) = i;

	mod = SV_GetModelByIndex(i);

	if (mod)
	{
		if (mod->type != mod_alias || sv_gameplayfix_setmodelrealbox.integer)
			SetMinMaxSize(prog, e, mod->normalmins, mod->normalmaxs, true);
		else
			SetMinMaxSize(prog, e, quakemins, quakemaxs, true);
	}
	else
		SetMinMaxSize(prog, e, vec3_origin, vec3_origin, true);
}

/*
=================
VM_SV_sprint

single print to a specific client

sprint(clientent, value)
=================
*/
static void VM_SV_sprint(prvm_prog_t *prog)
{
	client_t	*client;
	int			entnum;
	char string[VM_STRINGTEMP_LENGTH_16384];

	VM_SAFEPARMCOUNTRANGE(2, 8, VM_SV_sprint);

	VM_VarString(prog, 1, string, sizeof(string));

	entnum = PRVM_G_EDICTNUM(OFS_PARM0);
	// LadyHavoc: div0 requested that sprintto world  operate like print
	if (entnum == 0)
	{
		Con_Print(string);
		return;
	}

	if (entnum < 1 || entnum > svs.maxclients || !svs.clients[entnum-1].active)
	{
		VM_WarningLinef (prog, "tried to centerprint to a non-client");
		return;
	}

	client = svs.clients + entnum-1;
	if (!client->netconnection)
		return;

	MSG_WriteChar(&client->netconnection->message,svc_print);
	MSG_WriteString(&client->netconnection->message, string);
}


/*
=================
VM_SV_centerprint

single print to a specific client

centerprint(clientent, value)
=================
*/
static void VM_SV_centerprint(prvm_prog_t *prog)
{
	client_t	*client;
	int			entnum;
	char string[VM_STRINGTEMP_LENGTH_16384];

	VM_SAFEPARMCOUNTRANGE(2, 8, VM_SV_centerprint);

	entnum = PRVM_G_EDICTNUM(OFS_PARM0);

	if (entnum < 1 || entnum > svs.maxclients || !svs.clients[entnum-1].active)
	{
		VM_WarningLinef (prog, "tried to centerprint to a non-client");
		return;
	}

	client = svs.clients + entnum-1;
	if (!client->netconnection)
		return;

	VM_VarString(prog, 1, string, sizeof(string));
	MSG_WriteChar(&client->netconnection->message,svc_centerprint);
	MSG_WriteString(&client->netconnection->message, string);
}

/*
=================
VM_SV_particle

particle(origin, color, count)
=================
*/
static void VM_SV_particle(prvm_prog_t *prog)
{
	vec3_t		org, dir;
	int		color;
	int		count;

	VM_SAFEPARMCOUNT(4, VM_SV_particle);

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), org);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM1), dir);
	color = (int)PRVM_G_FLOAT(OFS_PARM2);
	count = (int)PRVM_G_FLOAT(OFS_PARM3);
	SV_StartParticle (org, dir, color, count);
}


/*
=================
VM_SV_ambientsound

=================
*/
static void VM_SV_ambientsound(prvm_prog_t *prog)
{
	const char	*samp;
	vec3_t		pos;
	prvm_vec_t	vol, attenuation;
	int			soundnum, is_large_soundindex;
	int			is_fitz = isin2 (sv.protocol, PROTOCOL_FITZQUAKE666, PROTOCOL_FITZQUAKE999);

	VM_SAFEPARMCOUNT(4, VM_SV_ambientsound);

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), pos);
	samp = PRVM_G_STRING(OFS_PARM1);
	vol = PRVM_G_FLOAT(OFS_PARM2);
	attenuation = PRVM_G_FLOAT(OFS_PARM3);

// check to see if samp was properly precached
	soundnum = SV_SoundIndex(samp, 1);
	if (!soundnum)
		return;

	is_large_soundindex = false;
	if (soundnum >= 256)
		is_large_soundindex = true;

	if (sv.protocol == PROTOCOL_NEHAHRABJP)
		is_large_soundindex = false;

	// add an svc_spawnambient command to the level signon packet

	if (is_large_soundindex)
		if (is_fitz)
			MSG_WriteByte (&sv.signon, svcfitz_spawnstaticsound2);
		else
			MSG_WriteByte (&sv.signon, svc_spawnstaticsound2);
	else
		MSG_WriteByte (&sv.signon, svc_spawnstaticsound);

	MSG_WriteVector(&sv.signon, pos, sv.protocol);

	if (is_large_soundindex || isin2(sv.protocol, PROTOCOL_NEHAHRABJP2, PROTOCOL_NEHAHRABJP3))
		MSG_WriteShort (&sv.signon, soundnum);
	else
		MSG_WriteByte (&sv.signon, soundnum);

	MSG_WriteByte (&sv.signon, (int)(vol*255));
	MSG_WriteByte (&sv.signon, (int)(attenuation*64));

}

/*
=================
VM_SV_sound

Each entity can have eight independant sound sources, like voice,
weapon, feet, etc.

Channel 0 is an auto-allocate channel, the others override anything
already running on that entity/channel pair.

An attenuation of 0 will play full volume everywhere in the level.
Larger attenuations will drop off.

void(entity e, float chan, string samp, float volume[, float atten[, float pitchchange[, float flags]]]) sound (QUAKE)
=================
*/
static void VM_SV_sound(prvm_prog_t *prog)
{
	const char	*sample;
	int			channel;
	prvm_edict_t		*entity;
	int 		nvolume;
	int flags;
	float attenuation;
	float pitchchange;

	VM_SAFEPARMCOUNTRANGE(4, 7, VM_SV_sound);

	entity = PRVM_G_EDICT(OFS_PARM0);
	channel = (int)PRVM_G_FLOAT(OFS_PARM1);
	sample = PRVM_G_STRING(OFS_PARM2);
	nvolume = (int)(PRVM_G_FLOAT(OFS_PARM3) * 255);
	if (prog->argc < 5)
	{
		Con_DPrintLinef ("VM_SV_sound: given only 4 parameters, expected 5, assuming attenuation = ATTN_NORMAL");
		attenuation = 1;
	}
	else
		attenuation = PRVM_G_FLOAT(OFS_PARM4);
	if (prog->argc < 6)
		pitchchange = 0;
	else
		pitchchange = PRVM_G_FLOAT(OFS_PARM5) * 0.01f;

	if (prog->argc < 7)
	{
		flags = 0;
		if (channel >= 8 && channel <= 15) // weird QW feature
		{
			flags |= CHANNELFLAG_RELIABLE;
			channel -= 8;
		}
	}
	else
	{
		// LadyHavoc: we only let the qc set certain flags, others are off-limits
		flags = (int)PRVM_G_FLOAT(OFS_PARM6) & (CHANNELFLAG_RELIABLE | CHANNELFLAG_FORCELOOP | CHANNELFLAG_PAUSED | CHANNELFLAG_FULLVOLUME);
	}

	if (sv_clmovement_soundreliable.integer)
		Flag_Add_To (flags, CHANNELFLAG_RELIABLE);

	// "ambience/windfly.wav" other
	// int old_other = PRVM_serverglobaledict(other);
	// How to check if is player?
	// if (entnum > 0 && entnum <= svs.maxclients) {
	// How get host_client from entnum
	// host_client = svs.clients .. svs.clients[i]
	if (sv_allow_zircon_move.integer && String_Match (sample, "ambience/windfly.wav")) {
		int other_entnum = PRVM_serverglobaledict(other);
		int client_idnum = other_entnum - 1; // World is 0
		if (in_range_beyond (0, client_idnum, svs.maxclients)) {
			client_t *hcl = &svs.clients[client_idnum];
			SV_PhysicsX_Zircon_Warp_Start (hcl, "windtunnel");
		} // is player
	} // is windfly

	if (nvolume < 0 || nvolume > 255)
	{
		VM_WarningLinef (prog, "SV_StartSound: volume must be in range 0-1");
		return;
	}

	if (attenuation < 0 || attenuation > 4)
	{
		VM_WarningLinef (prog, "SV_StartSound: attenuation must be in range 0-4");
		return;
	}

	channel = CHAN_USER2ENGINE(channel);

	if (!IS_CHAN(channel))
	{
		VM_WarningLinef (prog, "SV_StartSound: channel must be in range 0-127");
		return;
	}

	SV_StartSound (entity, channel, sample, nvolume, attenuation, flags & CHANNELFLAG_RELIABLE, pitchchange);
}

/*
=================
VM_SV_pointsound

Follows the same logic as VM_SV_sound, except instead of
an entity, an origin for the sound is provided, and channel
is omitted (since no entity is being tracked).

=================
*/
static void VM_SV_pointsound(prvm_prog_t *prog)
{
	const char	*sample;
	int 		nvolume;
	float		attenuation;
	float		pitchchange;
	vec3_t		org;

	VM_SAFEPARMCOUNTRANGE(4, 5, VM_SV_pointsound);

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), org);
	sample = PRVM_G_STRING(OFS_PARM1);
	nvolume = (int)(PRVM_G_FLOAT(OFS_PARM2) * 255);
	attenuation = PRVM_G_FLOAT(OFS_PARM3);
	pitchchange = prog->argc < 5 ? 0 : PRVM_G_FLOAT(OFS_PARM4) * 0.01f;

	if (nvolume < 0 || nvolume > 255)
	{
		VM_WarningLinef (prog, "SV_StartPointSound: volume must be in range 0-1");
		return;
	}

	if (attenuation < 0 || attenuation > 4)
	{
		VM_WarningLinef (prog, "SV_StartPointSound: attenuation must be in range 0-4");
		return;
	}

	SV_StartPointSound (org, sample, nvolume, attenuation, pitchchange);
}

/*
=================
VM_SV_traceline

Used for use tracing and shot targeting
Traces are blocked by bbox and exact bsp entityes, and also slide box entities
if the tryents flag is set.

traceline (vector1, vector2, movetype, ignore)
=================
*/
static void VM_SV_traceline(prvm_prog_t *prog)
{
	vec3_t	v1, v2;
	trace_t	trace;
	int		move;
	prvm_edict_t	*ent;

	VM_SAFEPARMCOUNTRANGE(4, 8, VM_SV_traceline); // allow more parameters for future expansion

	prog->xfunction->builtinsprofile += 30;

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), v1);	// START
	VectorCopy(PRVM_G_VECTOR(OFS_PARM1), v2);	// END
	move = (int)PRVM_G_FLOAT(OFS_PARM2);		// MOVETYPE
	ent = PRVM_G_EDICT(OFS_PARM3);				// IGNORE

	if (VEC_IS_NAN(v1[0]) || VEC_IS_NAN(v1[1]) || VEC_IS_NAN(v1[2]) || VEC_IS_NAN(v2[0]) || VEC_IS_NAN(v2[1]) || VEC_IS_NAN(v2[2]))
		prog->error_cmd("%s: NAN errors detected in traceline('%f %f %f', '%f %f %f', %d, entity %d)\n", prog->name, v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], move, PRVM_EDICT_TO_PROG(ent));

	trace = SV_TraceLine(v1, v2, move, ent, SV_GenericHitSuperContentsMask(ent), 0, 0, collision_extendtracelinelength.value);

	VM_SetTraceGlobals(prog, &trace);
}


/*
=================
VM_SV_tracebox

Used for use tracing and shot targeting
Traces are blocked by bbox and exact bsp entityes, and also slide box entities
if the tryents flag is set.

tracebox (vector1, vector mins, vector maxs, vector2, tryents)
=================
*/
// LadyHavoc: added this for my own use, VERY useful, similar to traceline
static void VM_SV_tracebox(prvm_prog_t *prog)
{
	if (sv.is_qex) {
		// PF_CheckPlayerEXFlags qex fights VM_SV_tracebox
		// G_FLOAT(OFS_RETURN) = 0;
		// #90 RERELEASE PF_CheckPlayerEXFlags
		PRVM_G_FLOAT(OFS_RETURN) = 0;
		return;
	}

	vec3_t v1, v2, m1, m2;
	trace_t	trace;
	int		move;
	prvm_edict_t	*ent;

	VM_SAFEPARMCOUNTRANGE(6, 8, VM_SV_tracebox); // allow more parameters for future expansion

	prog->xfunction->builtinsprofile += 30;

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), v1);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM1), m1);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM2), m2);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM3), v2);
	move = (int)PRVM_G_FLOAT(OFS_PARM4);
	ent = PRVM_G_EDICT(OFS_PARM5);

	// Baker: Visual Studio does not necessarily have isnan
	// Baker: This macro casts and checks bytes
	// since the storage of floats/double is a standard
	// this macro should be fine everywhere.
	if (VEC_IS_NAN(v1[0]) || VEC_IS_NAN(v1[1]) || VEC_IS_NAN(v1[2]) || VEC_IS_NAN(v2[0]) || VEC_IS_NAN(v2[1]) || VEC_IS_NAN(v2[2]))
		prog->error_cmd ("%s: NAN errors detected in tracebox('%f %f %f', '%f %f %f', '%f %f %f', '%f %f %f', %d, entity %d)", prog->name, v1[0], v1[1], v1[2], m1[0], m1[1], m1[2], m2[0], m2[1], m2[2], v2[0], v2[1], v2[2], move, PRVM_EDICT_TO_PROG(ent));

	trace = SV_TraceBox(v1, m1, m2, v2, move, ent, SV_GenericHitSuperContentsMask(ent), 0, 0, collision_extendtraceboxlength.value);

	VM_SetTraceGlobals(prog, &trace);
}

trace_t SV_Trace_Toss(prvm_prog_t *prog, prvm_edict_t *tossent, prvm_edict_t *ignore)
{
	int i;
	float gravity;
	vec3_t move, end, tossentorigin, tossentmins, tossentmaxs;
	vec3_t original_origin;
	vec3_t original_velocity;
	vec3_t original_angles;
	vec3_t original_avelocity;
	trace_t trace;

	VectorCopy(PRVM_serveredictvector(tossent, origin)   , original_origin   );
	VectorCopy(PRVM_serveredictvector(tossent, velocity) , original_velocity );
	VectorCopy(PRVM_serveredictvector(tossent, angles)   , original_angles   );
	VectorCopy(PRVM_serveredictvector(tossent, avelocity), original_avelocity);

	gravity = PRVM_serveredictfloat(tossent, gravity);
	if (!gravity)
		gravity = 1.0f;
	gravity *= sv_gravity.value * 0.025;

	for (i = 0;i < 200;i++) // LadyHavoc: sanity check; never trace more than 10 seconds
	{
		SV_CheckVelocity (tossent);
		PRVM_serveredictvector(tossent, velocity)[2] -= gravity;
		VectorMA (PRVM_serveredictvector(tossent, angles), 0.05, PRVM_serveredictvector(tossent, avelocity), PRVM_serveredictvector(tossent, angles));
		VectorScale (PRVM_serveredictvector(tossent, velocity), 0.05, move);
		VectorAdd (PRVM_serveredictvector(tossent, origin), move, end);
		VectorCopy(PRVM_serveredictvector(tossent, origin), tossentorigin);
		VectorCopy(PRVM_serveredictvector(tossent, mins), tossentmins);
		VectorCopy(PRVM_serveredictvector(tossent, maxs), tossentmaxs);
		trace = SV_TraceBox(tossentorigin, tossentmins, tossentmaxs, end, MOVE_NORMAL_0, tossent, SV_GenericHitSuperContentsMask(tossent), 0, 0, collision_extendmovelength.value);
		VectorCopy (trace.endpos, PRVM_serveredictvector(tossent, origin));
		PRVM_serveredictvector(tossent, velocity)[2] -= gravity;

		if (trace.fraction < 1)
			break;
	}

	VectorCopy(original_origin   , PRVM_serveredictvector(tossent, origin)   );
	VectorCopy(original_velocity , PRVM_serveredictvector(tossent, velocity) );
	VectorCopy(original_angles   , PRVM_serveredictvector(tossent, angles)   );
	VectorCopy(original_avelocity, PRVM_serveredictvector(tossent, avelocity));

	return trace;
}

// void(entity ent, entity ignore) tracetoss = #64;
// Baker: DP_QC_TRACETOSS - this function looks a bit expensive (iterates 200 times in SV_Trace_Toss)
static void VM_SV_tracetoss(prvm_prog_t *prog)
{
	trace_t	trace;
	prvm_edict_t	*ent;
	prvm_edict_t	*ignore;

	VM_SAFEPARMCOUNT(2, VM_SV_tracetoss);

	prog->xfunction->builtinsprofile += 600;

	ent = PRVM_G_EDICT(OFS_PARM0);
	if (ent == prog->edicts) {
		VM_WarningLinef (prog, "tracetoss: can not use world entity");
		return;
	}
	ignore = PRVM_G_EDICT(OFS_PARM1);

	trace = SV_Trace_Toss(prog, ent, ignore);

	VM_SetTraceGlobals(prog, &trace);
}

//============================================================================

static unsigned char *checkpvs;
#if 1 // June 2
#else
static int checkpvsbytes;
static unsigned char checkpvs[MAX_MAP_LEAFS_65536/8];
#endif

// Returns clients edict number for checkclient (which is monster find target)
static int VM_SV_newcheckclient(prvm_prog_t *prog, int check)
{
	int		i;
	prvm_edict_t	*ent;
	vec3_t	org;

// cycle to the next one

	check = bound(1, check, svs.maxclients);
	if (check == svs.maxclients)
		i = 1;
	else
		i = check + 1;

	for ( ;  ; i++)
	{
		// count the cost
		prog->xfunction->builtinsprofile++;
		// wrap around
		if (i == svs.maxclients+1)
			i = 1;
		// look up the client's edict
		ent = PRVM_EDICT_NUM(i);
		// check if it is to be ignored, but never ignore the one we started on (prevent infinite loop)
		if (i != check && (ent->free || PRVM_serveredictfloat(ent, health) <= 0 || ((int)PRVM_serveredictfloat(ent, flags) & FL_NOTARGET)))
			continue;
		// found a valid client (possibly the same one again)
		break;
	}

// get the PVS for the entity
	VectorAdd(PRVM_serveredictvector(ent, origin), PRVM_serveredictvector(ent, view_ofs), org);
#if 1 // June 2
#else
	checkpvsbytes = 0;
#endif
	if (sv.worldmodel && sv.worldmodel->brush.FatPVS)
#if 1 // June 2
		sv.worldmodel->brush.FatPVS(sv.worldmodel, org, 0, &checkpvs, sv_mempool, false);
	else
		checkpvs = NULL;
#else
		checkpvsbytes = sv.worldmodel->brush.FatPVS(sv.worldmodel, org, 0, checkpvs, sizeof(checkpvs), false);
#endif

	return i;
}

/*
=================
VM_SV_checkclient

Returns a client (or object that has a client enemy) that would be a
valid target.

If there is more than one valid option, they are cycled each frame

If (self.origin + self.viewofs) is not in the PVS of the current target,
it is not returned at all.

name checkclient ()
=================
*/
int c_invis, c_notvis; // Baker: August 6 2024 -- Yay!  Some shitty globals.  And used for nothing so engine developer vars.
static void VM_SV_checkclient(prvm_prog_t *prog)
{
	prvm_edict_t	*ent, *self;
	vec3_t	view;

	VM_SAFEPARMCOUNT(0, VM_SV_checkclient);

	// find a new check if on a new frame
	if (sv.time - sv.lastchecktime >= 0.1) {
		sv.lastcheck = VM_SV_newcheckclient(prog, sv.lastcheck);
		sv.lastchecktime = sv.time;
	}

	// return check if it might be visible
	ent = PRVM_EDICT_NUM(sv.lastcheck);
	if (ent->free || PRVM_serveredictfloat(ent, health) <= 0) {
		VM_RETURN_EDICT(prog->edicts);
		return;
	}

	// if current entity can't possibly see the check entity, return 0
	self = PRVM_PROG_TO_EDICT(PRVM_serverglobaledict(self));
	VectorAdd(PRVM_serveredictvector(self, origin), PRVM_serveredictvector(self, view_ofs), view);
#if 1 // June 2
	if (sv.worldmodel && checkpvs && !sv.worldmodel->brush.BoxTouchingPVS(sv.worldmodel, checkpvs, view, view))
#else
	if (sv.worldmodel && checkpvsbytes && !sv.worldmodel->brush.BoxTouchingPVS(sv.worldmodel, checkpvs, view, view))
#endif
	{
		c_notvis ++;
		VM_RETURN_EDICT(prog->edicts); // Baker: I assume this returns 0 or world.
		return;
	}

	// might be able to see it
	c_invis++;
	VM_RETURN_EDICT(ent);
}

//============================================================================

/*
=================
VM_SV_checkpvs

Checks if an entity is in a point's PVS.
Should be fast but can be inexact.

float checkpvs(vector viewpos, entity viewee) = #240;
=================
*/
static void VM_SV_checkpvs(prvm_prog_t *prog)
{
	vec3_t viewpos, absmin, absmax;
	prvm_edict_t *viewee;
#if 1
	unsigned char *pvs;
#else
	#if 1 // June 2
		unsigned char *fatpvs = NULL;
	#else
		int fatpvsbytes;
		unsigned char fatpvs[MAX_MAP_LEAFS_65536/8];
	#endif
#endif

	VM_SAFEPARMCOUNT(2, VM_SV_checkpvs);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), viewpos);
	viewee = PRVM_G_EDICT(OFS_PARM1);

	if (viewee->free)
	{
		VM_WarningLinef (prog, "checkpvs: can not check free entity");
		PRVM_G_FLOAT(OFS_RETURN) = 4;
		return;
	}

#if 1
	if (!sv.worldmodel || !sv.worldmodel->brush.GetPVS || !sv.worldmodel->brush.BoxTouchingPVS)
	{
		// no PVS support on this worldmodel... darn
		PRVM_G_FLOAT(OFS_RETURN) = 3;
		return;
	}
	pvs = sv.worldmodel->brush.GetPVS(sv.worldmodel, viewpos);
	if (!pvs)
	{
		// viewpos isn't in any PVS... darn
		PRVM_G_FLOAT(OFS_RETURN) = 2;
		return;
	}
	VectorCopy(PRVM_serveredictvector(viewee, absmin), absmin);
	VectorCopy(PRVM_serveredictvector(viewee, absmax), absmax);
	PRVM_G_FLOAT(OFS_RETURN) = sv.worldmodel->brush.BoxTouchingPVS(sv.worldmodel, pvs, absmin, absmax);
#else
	// using fat PVS like FTEQW does (slow)
	if (!sv.worldmodel || !sv.worldmodel->brush.FatPVS || !sv.worldmodel->brush.BoxTouchingPVS)
	{
		// no PVS support on this worldmodel... darn
		PRVM_G_FLOAT(OFS_RETURN) = 3;
		return;
	}
#if 1 // June 2
	sv.worldmodel->brush.FatPVS(sv.worldmodel, viewpos, 8, &fatpvs, sv_mempool, false);
	if(!fatpvs)
#else
	fatpvsbytes = sv.worldmodel->brush.FatPVS(sv.worldmodel, viewpos, 8, fatpvs, sizeof(fatpvs), false);
	if (!fatpvsbytes)
#endif
	{
		// viewpos isn't in any PVS... darn
		PRVM_G_FLOAT(OFS_RETURN) = 2;
		return;
	}
	VectorCopy(PRVM_serveredictvector(viewee, absmin), absmin);
	VectorCopy(PRVM_serveredictvector(viewee, absmax), absmax);
	PRVM_G_FLOAT(OFS_RETURN) = sv.worldmodel->brush.BoxTouchingPVS(sv.worldmodel, fatpvs, absmin, absmax);
#endif
}


/*
=================
VM_SV_stuffcmd

Sends text over to the client's execution buffer

stuffcmd (clientent, value, ...)
=================
*/
static void VM_SV_stuffcmd(prvm_prog_t *prog)
{
	int		entnum;
	client_t	*old;
	char	string[VM_STRINGTEMP_LENGTH_16384];

	VM_SAFEPARMCOUNTRANGE(2, 8, VM_SV_stuffcmd);

	entnum = PRVM_G_EDICTNUM(OFS_PARM0);
	if (entnum < 1 || entnum > svs.maxclients || !svs.clients[entnum-1].active)
	{
		VM_WarningLinef (prog, "Can't stuffcmd to a non-client");
		return;
	}

	VM_VarString(prog, 1, string, sizeof(string));

	old = host_client;
	host_client = svs.clients + entnum-1;
	SV_ClientCommandsf ("%s", string);
	host_client = old;
}

/*
=================
VM_SV_findradius

Returns a chain of entities that have origins within a spherical area

findradius (origin, radius)
=================
*/
static void VM_SV_findradius(prvm_prog_t *prog)
{
	prvm_edict_t *ent, *chain;
	vec_t radius, radius2;
	vec3_t org, eorg, mins, maxs;
	int i;
	int numtouchedicts;
	static prvm_edict_t *touchedicts[MAX_EDICTS_32768];
	int chainfield;

	VM_SAFEPARMCOUNTRANGE(2, 3, VM_SV_findradius);

	if (prog->argc == 3)
		chainfield = PRVM_G_INT(OFS_PARM2);
	else
		chainfield = prog->fieldoffsets.chain;
	if (chainfield < 0)
		prog->error_cmd("VM_SV_findradius: %s doesnt have the specified chain field !", prog->name);

	chain = (prvm_edict_t *)prog->edicts;

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), org);
	radius = PRVM_G_FLOAT(OFS_PARM1);
	radius2 = radius * radius;

	mins[0] = org[0] - (radius + 1);
	mins[1] = org[1] - (radius + 1);
	mins[2] = org[2] - (radius + 1);
	maxs[0] = org[0] + (radius + 1);
	maxs[1] = org[1] + (radius + 1);
	maxs[2] = org[2] + (radius + 1);
	numtouchedicts = SV_EntitiesInBox(mins, maxs, MAX_EDICTS_32768, touchedicts);
	if (numtouchedicts > MAX_EDICTS_32768)
	{
		// this never happens
		Con_PrintLinef ("SV_EntitiesInBox returned %d edicts, max was %d", numtouchedicts, MAX_EDICTS_32768);
		numtouchedicts = MAX_EDICTS_32768;
	}
	for (i = 0;i < numtouchedicts;i++)
	{
		ent = touchedicts[i];
		prog->xfunction->builtinsprofile++;
		// Quake did not return non-solid entities but darkplaces does
		// (note: this is the reason you can't blow up fallen zombies)
		if (PRVM_serveredictfloat(ent, solid) == SOLID_NOT_0 && !sv_gameplayfix_blowupfallenzombies.integer)
			continue;
		// LadyHavoc: compare against bounding box rather than center so it
		// doesn't miss large objects, and use DotProduct instead of Length
		// for a major speedup
		VectorSubtract(org, PRVM_serveredictvector(ent, origin), eorg);
		if (sv_gameplayfix_findradiusdistancetobox.integer)
		{
			eorg[0] -= bound(PRVM_serveredictvector(ent, mins)[0], eorg[0], PRVM_serveredictvector(ent, maxs)[0]);
			eorg[1] -= bound(PRVM_serveredictvector(ent, mins)[1], eorg[1], PRVM_serveredictvector(ent, maxs)[1]);
			eorg[2] -= bound(PRVM_serveredictvector(ent, mins)[2], eorg[2], PRVM_serveredictvector(ent, maxs)[2]);
		}
		else
			VectorMAMAM(1, eorg, -0.5f, PRVM_serveredictvector(ent, mins), -0.5f, PRVM_serveredictvector(ent, maxs), eorg);
		if (DotProduct(eorg, eorg) < radius2)
		{
			PRVM_EDICTFIELDEDICT(ent,chainfield) = PRVM_EDICT_TO_PROG(chain);
			chain = ent;
		}
	}

	VM_RETURN_EDICT(chain);
}

/*
=================
VM_SV_findbox

Returns a chain of entities that are touching a box (a simpler findradius); supports DP_QC_FINDCHAIN_TOFIELD

findbox (mins, maxs)
=================
*/
static void VM_SV_findbox(prvm_prog_t *prog)
{
	prvm_edict_t *chain;
	int i, numtouchedicts;
	static prvm_edict_t *touchedicts[MAX_EDICTS_32768];
	int chainfield;

	VM_SAFEPARMCOUNTRANGE(2, 3, VM_SV_findbox);

	if (prog->argc == 3)
		chainfield = PRVM_G_INT(OFS_PARM2);
	else
		chainfield = prog->fieldoffsets.chain;
	if (chainfield < 0)
		prog->error_cmd ("VM_SV_findbox: %s doesnt have the specified chain field !", prog->name);

	chain = (prvm_edict_t *)prog->edicts;

	numtouchedicts = SV_EntitiesInBox(PRVM_G_VECTOR(OFS_PARM0), PRVM_G_VECTOR(OFS_PARM1), MAX_EDICTS_32768, touchedicts);
	if (numtouchedicts > MAX_EDICTS_32768)
	{
		// this never happens
		Con_PrintLinef ("SV_EntitiesInBox returned %d edicts, max was %d", numtouchedicts, MAX_EDICTS_32768);
		numtouchedicts = MAX_EDICTS_32768;
	}
	for (i = 0; i < numtouchedicts; i ++)
	{
		prog->xfunction->builtinsprofile++;
		PRVM_EDICTFIELDEDICT(touchedicts[i], chainfield) = PRVM_EDICT_TO_PROG(chain);
		chain = touchedicts[i];
	}

	VM_RETURN_EDICT(chain);
}

static void VM_SV_precache_sound(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_precache_sound);
	PRVM_G_FLOAT(OFS_RETURN) = SV_SoundIndex(PRVM_G_STRING(OFS_PARM0), 2);
}

// #20 void(string s) precache_model (QUAKE)
// #75 string(string s) precache_model2 (QUAKE)
static void VM_SV_precache_model(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_precache_model);
	SV_ModelIndex(PRVM_G_STRING(OFS_PARM0), PRECACHE_MODEL_CMD_MODE_2);
	PRVM_G_INT(OFS_RETURN) = PRVM_G_INT(OFS_PARM0);
}

RELATED_ (SV_ShowModel_f)
void VMX_SV_precache_model (prvm_prog_t *prog, const char *s_model)
{
	SV_ModelIndex (s_model, PRECACHE_MODEL_CMD_MODE_2);
}

/*
===============
VM_SV_walkmove

float(float yaw, float dist[, settrace]) walkmove
===============
*/
static void VM_SV_walkmove(prvm_prog_t *prog)
{
	prvm_edict_t	*ent;
	float	yaw, dist;
	vec3_t	move;
	mfunction_t	*oldf;
	int 	oldself;
	qbool	settrace;

	VM_SAFEPARMCOUNTRANGE(2, 3, VM_SV_walkmove);

	// assume failure if it returns early
	PRVM_G_FLOAT(OFS_RETURN) = 0;

	ent = PRVM_PROG_TO_EDICT(PRVM_serverglobaledict(self));
	if (ent == prog->edicts) {
		VM_WarningLinef (prog, "walkmove: can not modify world entity");
		return;
	}

	if (ent->free) {
		VM_WarningLinef (prog, "walkmove: can not modify free entity");
		return;
	}
	yaw = PRVM_G_FLOAT(OFS_PARM0);
	dist = PRVM_G_FLOAT(OFS_PARM1);
	settrace = prog->argc >= 3 && PRVM_G_FLOAT(OFS_PARM2);

	if ( !( (int)PRVM_serveredictfloat(ent, flags) & (FL_ONGROUND_512|FL_FLY_1|FL_SWIM_2) ) )
		return;

	yaw = yaw*M_PI*2 / 360;

	move[0] = cos(yaw)*dist;
	move[1] = sin(yaw)*dist;
	move[2] = 0;

// save program state, because SV_movestep may call other progs
	oldf = prog->xfunction;
	oldself = PRVM_serverglobaledict(self);

	PRVM_G_FLOAT(OFS_RETURN) = SV_movestep(ent, move, true, false, settrace);

// restore program state
	prog->xfunction = oldf;
	PRVM_serverglobaledict(self) = oldself;
}

/*
===============
VM_SV_droptofloor

void() droptofloor
===============
*/
#if 111 // Baker r0061: Classic DarkPlaces physics

static void VM_SV_droptofloor(prvm_prog_t *prog)
{
	prvm_edict_t		*ent;
	vec3_t		end, entorigin, entmins, entmaxs;
	trace_t		trace;

	VM_SAFEPARMCOUNTRANGE(0, 2, VM_SV_droptofloor); // allow 2 parameters because the id1 defs.qc had an incorrect prototype

	// assume failure if it returns early
	PRVM_G_FLOAT(OFS_RETURN) = 0;

	ent = PRVM_PROG_TO_EDICT(PRVM_serverglobaledict(self));
	if (ent == prog->edicts) {
		VM_WarningLinef (prog, "droptofloor: can not modify world entity");
		return;
	}

	if (ent->free) {
		VM_WarningLinef (prog, "droptofloor: can not modify free entity");
		return;
	}

	VectorCopy (PRVM_serveredictvector(ent, origin), end);
	if (sv.worldmodel->brush.isq3bsp)
		end[2] -= 4096;
	else if (sv.worldmodel->brush.isq2bsp)
		end[2] -= 128;
	else
		end[2] -= 256; // Quake, QuakeWorld

	if (sv_gameplayfix_droptofloorstartsolid_nudgetocorrect.integer /*d:0 for Quake*/)
		SV_NudgeOutOfSolid(ent);

	VectorCopy(PRVM_serveredictvector(ent, origin), entorigin);
	VectorCopy(PRVM_serveredictvector(ent, mins), entmins);
	VectorCopy(PRVM_serveredictvector(ent, maxs), entmaxs);
	trace = SV_TraceBox(entorigin, entmins, entmaxs, end, MOVE_NORMAL_0, ent, SV_GenericHitSuperContentsMask(ent), 0, 0, collision_extendmovelength.value /*d: 16*/);
	if (trace.startsolid && sv_gameplayfix_droptofloorstartsolid.integer /*d:1 for Quake in Zircon but not DarkPlaces Classic*/) {
		vec3_t offset, org;
		VectorSet(offset, 0.5f * (PRVM_serveredictvector(ent, mins)[0] + PRVM_serveredictvector(ent, maxs)[0]), 0.5f * (PRVM_serveredictvector(ent, mins)[1] + PRVM_serveredictvector(ent, maxs)[1]), PRVM_serveredictvector(ent, mins)[2]);
		VectorAdd(PRVM_serveredictvector(ent, origin), offset, org);
		trace = SV_TraceLine(org, end, MOVE_NORMAL_0, ent, SV_GenericHitSuperContentsMask(ent), 0, 0, collision_extendmovelength.value /*d: 16*/);
		VectorSubtract(trace.endpos, offset, trace.endpos);
		if (trace.startsolid) {
			Con_DPrintLinef (CON_WARN "droptofloor at " VECTOR3_5d1F " (#%d %s) - COULD NOT FIX BADLY PLACED ENTITY",
				VECTOR3_SEND (PRVM_serveredictvector(ent, origin)),
				PRVM_NUM_FOR_EDICT(ent),
				PRVM_GetString(prog, PRVM_serveredictstring(ent, classname))
			);
			SV_LinkEdict(ent);
			PRVM_serveredictfloat(ent, flags) = (int)PRVM_serveredictfloat(ent, flags) | FL_ONGROUND_512;
			PRVM_serveredictedict(ent, groundentity) = 0;
			PRVM_G_FLOAT(OFS_RETURN) = 1;
		}
		else if (trace.fraction < 1)
		{
			// Baker: This seems randomish.  Had a mine and nothing was wrong with it.
			// Why can't trace fraction be less than 1?
			Con_DPrintLinef (CON_WARN "droptofloor at " VECTOR3_5d1F " (#%d %s) - FIXED BADLY PLACED ENTITY",
				VECTOR3_SEND (PRVM_serveredictvector(ent, origin)),
				PRVM_NUM_FOR_EDICT(ent),
				PRVM_GetString(prog, PRVM_serveredictstring(ent, classname))
			);
			VectorCopy (trace.endpos, PRVM_serveredictvector(ent, origin));
			if (sv_gameplayfix_droptofloorstartsolid_nudgetocorrect.integer /*d:0 for Quake*/)
				SV_NudgeOutOfSolid(ent);
			SV_LinkEdict(ent);
			PRVM_serveredictfloat(ent, flags) = (int)PRVM_serveredictfloat(ent, flags) | FL_ONGROUND_512;
			PRVM_serveredictedict(ent, groundentity) = PRVM_EDICT_TO_PROG(trace.ent);
			PRVM_G_FLOAT(OFS_RETURN) = 1;
			// if support is destroyed, keep suspended (gross hack for floating items in various maps)
			ent->priv.server->suspendedinairflag = true;
		}
	}
	else
	{
		if (!trace.allsolid && trace.fraction < 1)
		{
			VectorCopy (trace.endpos, PRVM_serveredictvector(ent, origin));

			SV_LinkEdict(ent);
			PRVM_serveredictfloat(ent, flags) = (int)PRVM_serveredictfloat(ent, flags) | FL_ONGROUND_512;
			PRVM_serveredictedict(ent, groundentity) = PRVM_EDICT_TO_PROG(trace.ent);
			PRVM_G_FLOAT(OFS_RETURN) = 1;
			// if support is destroyed, keep suspended (gross hack for floating items in various maps)
			ent->priv.server->suspendedinairflag = true;
		}
	}
}

#else
inline static qbool droptofloor_bsp_failcond(trace_t *trace)
{
	if (sv.worldmodel->brush.isq3bsp || sv.worldmodel->brush.isq2bsp)
		return trace->startsolid;
	else
		return trace->allsolid || trace->fraction == 1;
}
static void VM_SV_droptofloor(prvm_prog_t *prog)
{
	prvm_edict_t *ent;
	vec3_t        end;
	trace_t       trace;

	VM_SAFEPARMCOUNTRANGE(0, 2, VM_SV_droptofloor); // allow 2 parameters because the id1 defs.qc had an incorrect prototype

	// assume failure if it returns early
	PRVM_G_FLOAT(OFS_RETURN) = 0;

	ent = PRVM_PROG_TO_EDICT(PRVM_serverglobaledict(self));
	if (ent == prog->edicts)
	{
		VM_WarningLinef (prog, "droptofloor: can not modify world entity");
		return;
	}
	if (ent->free)
	{
		VM_WarningLinef (prog, "droptofloor: can not modify free entity");
		return;
	}

	if (sv_gameplayfix_droptofloorstartsolid_nudgetocorrect.integer)
	{
		int n = PHYS_NudgeOutOfSolid(prog, ent);
		if (!n)
			VM_WarningLinef (prog, "droptofloor at \"%f %f %f\": sv_gameplayfix_droptofloorstartsolid_nudgetocorrect COULD NOT FIX badly placed entity " QUOTED_S " before drop", PRVM_gameedictvector(ent, origin)[0], PRVM_gameedictvector(ent, origin)[1], PRVM_gameedictvector(ent, origin)[2], PRVM_GetString(prog, PRVM_gameedictstring(ent, classname)));
		else if (n > 0)
			VM_WarningLinef (prog, "droptofloor at \"%f %f %f\": sv_gameplayfix_droptofloorstartsolid_nudgetocorrect FIXED badly placed entity " QUOTED_S " before drop", PRVM_gameedictvector(ent, origin)[0], PRVM_gameedictvector(ent, origin)[1], PRVM_gameedictvector(ent, origin)[2], PRVM_GetString(prog, PRVM_gameedictstring(ent, classname)));
	}

	VectorCopy (PRVM_serveredictvector(ent, origin), end);
	if (sv.worldmodel->brush.isq3bsp)
		end[2] -= 4096;
	else if (sv.worldmodel->brush.isq2bsp)
		end[2] -= 128;
	else
		end[2] -= 256; // Quake, QuakeWorld

	/* bones_was_here: not using SV_GenericHitSuperContentsMask(ent) anymore because it was setting:
	 * items:    SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY
	 * monsters: SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP
	 * explobox: SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_CORPSE
	 * which caused (startsolid == true) when, for example, a health was touching a monster.
	 * Changing MOVE_NORMAL_0 also fixes that, but other engines are using MOVE_NORMAL_0 here.
	 */
	trace = SV_TraceBox(PRVM_serveredictvector(ent, origin), PRVM_serveredictvector(ent, mins), PRVM_serveredictvector(ent, maxs), end, MOVE_NORMAL_0, ent, SUPERCONTENTS_SOLID, 0, 0, collision_extendmovelength.value);
	if (droptofloor_bsp_failcond(&trace))
	{
		if (sv_gameplayfix_droptofloorstartsolid.integer)
		{
			vec3_t offset, org;

			offset[0] = 0.5f * (PRVM_serveredictvector(ent, mins)[0] + PRVM_serveredictvector(ent, maxs)[0]);
			offset[1] = 0.5f * (PRVM_serveredictvector(ent, mins)[1] + PRVM_serveredictvector(ent, maxs)[1]);
			offset[2] = PRVM_serveredictvector(ent, mins)[2];
			VectorAdd(PRVM_serveredictvector(ent, origin), offset, org);
			VectorAdd(end, offset, end);

			trace = SV_TraceLine(org, end, MOVE_NORMAL_0, ent, SUPERCONTENTS_SOLID, 0, 0, collision_extendmovelength.value);
			if (droptofloor_bsp_failcond(&trace))
			{
				VM_WarningLinef (prog, "droptofloor at \"%f %f %f\": sv_gameplayfix_droptofloorstartsolid COULD NOT FIX badly placed entity " QUOTED_S, PRVM_serveredictvector(ent, origin)[0], PRVM_serveredictvector(ent, origin)[1], PRVM_serveredictvector(ent, origin)[2], PRVM_GetString(prog, PRVM_gameedictstring(ent, classname)));
				return;
			}
			VM_WarningLinef (prog, "droptofloor at \"%f %f %f\": sv_gameplayfix_droptofloorstartsolid FIXED badly placed entity " QUOTED_S, PRVM_serveredictvector(ent, origin)[0], PRVM_serveredictvector(ent, origin)[1], PRVM_serveredictvector(ent, origin)[2], PRVM_GetString(prog, PRVM_gameedictstring(ent, classname)));
			VectorSubtract(trace.endpos, offset, PRVM_serveredictvector(ent, origin));

			// only because we dropped it without considering its bbox
			if (sv_gameplayfix_droptofloorstartsolid_nudgetocorrect.integer)
				PHYS_NudgeOutOfSolid(prog, ent);
		}
		else
		{
			VM_WarningLinef (prog, "droptofloor at \"%f %f %f\": badly placed entity " QUOTED_S ", startsolid: %d allsolid: %d", PRVM_serveredictvector(ent, origin)[0], PRVM_serveredictvector(ent, origin)[1], PRVM_serveredictvector(ent, origin)[2], PRVM_GetString(prog, PRVM_gameedictstring(ent, classname)), trace.startsolid, trace.allsolid);
			return;
		}
	}
	else
		VectorCopy(trace.endpos, PRVM_serveredictvector(ent, origin));

	SV_LinkEdict(ent);
	PRVM_serveredictfloat(ent, flags) = (int)PRVM_serveredictfloat(ent, flags) | FL_ONGROUND_512;
	PRVM_serveredictedict(ent, groundentity) = PRVM_EDICT_TO_PROG(trace.ent);
	PRVM_G_FLOAT(OFS_RETURN) = 1;
	// if support is destroyed, keep suspended (gross hack for floating items in various maps)
	ent->priv.server->suspendedinairflag = true;
}
#endif

/*
===============
VM_SV_lightstyle

void(float style, string value) lightstyle
===============
*/
static void VM_SV_lightstyle(prvm_prog_t *prog)
{
	int		style;
	const char	*val;
	client_t	*client;
	int			j;

	VM_SAFEPARMCOUNT(2, VM_SV_lightstyle);

	style = (int)PRVM_G_FLOAT(OFS_PARM0);
	val = PRVM_G_STRING(OFS_PARM1);

	if ((unsigned) style >= MAX_LIGHTSTYLES_256 ) {
		prog->error_cmd ("PF_lightstyle: style: %d >= 64", style);
	}

// change the string in sv
	c_strlcpy(sv.lightstyles[style], val); // , sizeof(sv.lightstyles[style]));

// send message to all clients on this server
	if (sv.state != ss_active)
		return;

	for (j = 0, client = svs.clients;j < svs.maxclients; j++, client++) {
		if (client->active && client->netconnection) {
			MSG_WriteChar (&client->netconnection->message, svc_lightstyle);
			MSG_WriteChar (&client->netconnection->message,style);
			MSG_WriteString (&client->netconnection->message, val);
		}
	}
}

/*
=============
VM_SV_checkbottom
=============
*/
static void VM_SV_checkbottom(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_checkbottom);
	PRVM_G_FLOAT(OFS_RETURN) = SV_CheckBottom (PRVM_G_EDICT(OFS_PARM0));
}

/*
=============
VM_SV_pointcontents
=============
*/
static void VM_SV_pointcontents(prvm_prog_t *prog)
{
	vec3_t point;
	VM_SAFEPARMCOUNT(1, VM_SV_pointcontents);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), point);
	PRVM_G_FLOAT(OFS_RETURN) = Mod_Q1BSP_NativeContentsFromSuperContents(SV_PointSuperContents(point));
}

/*
=============
VM_SV_aim

Pick a vector for the player to shoot along
vector aim(entity, missilespeed)
=============
*/
static void VM_SV_aim(prvm_prog_t *prog)
{
	prvm_edict_t	*ent, *check, *bestent;
	vec3_t	start, dir, end, bestdir;
	int		i, j;
	trace_t	tr;
	float	dist, bestdist;
	//float	speed;

	VM_SAFEPARMCOUNT(2, VM_SV_aim);

	// assume failure if it returns early
	VectorCopy(PRVM_serverglobalvector(v_forward), PRVM_G_VECTOR(OFS_RETURN));
	// if sv_aim is so high it can't possibly accept anything, skip out early
	if (sv_aim.value >= 1)
		return;

	ent = PRVM_G_EDICT(OFS_PARM0);
	if (ent == prog->edicts)
	{
		VM_WarningLinef (prog, "aim: can not use world entity");
		return;
	}
	if (ent->free)
	{
		VM_WarningLinef (prog, "aim: can not use free entity");
		return;
	}
	//speed = PRVM_G_FLOAT(OFS_PARM1);

	VectorCopy (PRVM_serveredictvector(ent, origin), start);
	start[2] += 20;

// try sending a trace straight
	VectorCopy (PRVM_serverglobalvector(v_forward), dir);
	VectorMA (start, 2048, dir, end);
	tr = SV_TraceLine(start, end, MOVE_NORMAL_0, ent, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY, 0, 0, collision_extendmovelength.value);
	if (tr.ent && PRVM_serveredictfloat(((prvm_edict_t *)tr.ent), takedamage) == DAMAGE_AIM
	&& (!teamplay.integer || PRVM_serveredictfloat(ent, team) <=0 || PRVM_serveredictfloat(ent, team) != PRVM_serveredictfloat(((prvm_edict_t *)tr.ent), team)) )
	{
		VectorCopy (PRVM_serverglobalvector(v_forward), PRVM_G_VECTOR(OFS_RETURN));
		return;
	}


// try all possible entities
	VectorCopy (dir, bestdir);
	bestdist = sv_aim.value;
	bestent = NULL;

	check = PRVM_NEXT_EDICT(prog->edicts);
	for (i = 1; i < prog->num_edicts; i ++, check = PRVM_NEXT_EDICT(check) ) {
		prog->xfunction->builtinsprofile ++;
		if (PRVM_serveredictfloat(check, takedamage) != DAMAGE_AIM)
			continue;
		if (check == ent)
			continue;
		if (teamplay.integer && PRVM_serveredictfloat(ent, team) > 0 && PRVM_serveredictfloat(ent, team) == PRVM_serveredictfloat(check, team))
			continue;	// don't aim at teammate
		for (j=0 ; j<3 ; j++)
			end[j] = PRVM_serveredictvector(check, origin)[j]
			+ 0.5*(PRVM_serveredictvector(check, mins)[j] + PRVM_serveredictvector(check, maxs)[j]);
		VectorSubtract (end, start, dir);
		VectorNormalize (dir);
		dist = DotProduct (dir, PRVM_serverglobalvector(v_forward));
		if (dist < bestdist)
			continue;	// to far to turn
		tr = SV_TraceLine(start, end, MOVE_NORMAL_0, ent, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY, 0, 0, collision_extendmovelength.value);
		if (tr.ent == check)
		{	// can shoot at this one
			bestdist = dist;
			bestent = check;
		}
	}

	if (bestent)
	{
		VectorSubtract (PRVM_serveredictvector(bestent, origin), PRVM_serveredictvector(ent, origin), dir);
		dist = DotProduct (dir, PRVM_serverglobalvector(v_forward));
		VectorScale (PRVM_serverglobalvector(v_forward), dist, end);
		end[2] = dir[2];
		VectorNormalize (end);
		VectorCopy (end, PRVM_G_VECTOR(OFS_RETURN));
	}
	else
	{
		VectorCopy (bestdir, PRVM_G_VECTOR(OFS_RETURN));
	}
}

/*
===============================================================================

MESSAGE WRITING

===============================================================================
*/

#define	MSG_BROADCAST	0		// unreliable to all
#define	MSG_ONE			1		// reliable to one (msg_entity)
#define	MSG_ALL			2		// reliable to all
#define	MSG_INIT		3		// write to the init string
#define	MSG_ENTITY		5

static sizebuf_t *WriteDest(prvm_prog_t *prog)
{
	int		entnum;
	int		dest;
	prvm_edict_t	*ent;

	dest = (int)PRVM_G_FLOAT(OFS_PARM0);
	switch (dest)
	{
	case MSG_BROADCAST:
		return &sv.datagram;

	case MSG_ONE:
		ent = PRVM_PROG_TO_EDICT(PRVM_serverglobaledict(msg_entity));
		entnum = PRVM_NUM_FOR_EDICT(ent);
		if (entnum < 1 || entnum > svs.maxclients)
		{
			VM_WarningLinef (prog, "WriteDest: tried to write to non-client");
			return &sv.reliable_datagram;
		}
		else if (!svs.clients[entnum-1].active)
		{
			VM_WarningLinef (prog, "WriteDest: tried to write to a disconnected client");
			return &sv.reliable_datagram;
		}
		else if (!svs.clients[entnum-1].netconnection)
		{
			VM_WarningLinef (prog, "WriteDest: tried to write to a bot client");
			return &sv.reliable_datagram;
		}
		else
			return &svs.clients[entnum-1].netconnection->message;

	default:
		VM_WarningLinef (prog, "WriteDest: bad destination");
	case MSG_ALL:
		return &sv.reliable_datagram;

	case MSG_INIT:
		return &sv.signon;

	case MSG_ENTITY:
		return sv.writeentitiestoclient_msg;
	}

	//return NULL;
}

static void VM_SV_WriteByte(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteByte);
	MSG_WriteByte (WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM1));
}

static void VM_SV_WriteChar(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteChar);
	MSG_WriteChar (WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM1));
}

static void VM_SV_WriteShort(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteShort);
	MSG_WriteShort (WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM1));
}

static void VM_SV_WriteLong(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteLong);
	MSG_WriteLong (WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM1));
}

static void VM_SV_WriteAngle(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteAngle);
	MSG_WriteAngle (WriteDest(prog), PRVM_G_FLOAT(OFS_PARM1), sv.protocol);
}

static void VM_SV_WriteCoord(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteCoord);
	MSG_WriteCoord (WriteDest(prog), PRVM_G_FLOAT(OFS_PARM1), sv.protocol);
}

static void VM_SV_WriteString(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteString);
	MSG_WriteString (WriteDest(prog), PRVM_G_STRING(OFS_PARM1));
}

static void VM_SV_WriteUnterminatedString(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteUnterminatedString);
	MSG_WriteUnterminatedString (WriteDest(prog), PRVM_G_STRING(OFS_PARM1));
}


static void VM_SV_WriteEntity(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_WriteEntity);
	MSG_WriteShort (WriteDest(prog), PRVM_G_EDICTNUM(OFS_PARM1));
}

// writes a picture as at most size bytes of data
// message:
//   IMGNAME \0 SIZE(short) IMGDATA
// if failed to read/compress:
//   IMGNAME \0 \0 \0
//#501 void(float dest, string name, float maxsize) WritePicture (DP_SV_WRITEPICTURE))

RELATED_ (VM_CL_ReadPicture)
static void VM_SV_WritePicture(prvm_prog_t *prog)
{
	const char *imgname;
	void *buf;
	size_t size;

	VM_SAFEPARMCOUNT(3, VM_SV_WritePicture);

	imgname = PRVM_G_STRING(OFS_PARM1);
	size = (size_t) PRVM_G_FLOAT(OFS_PARM2);
	if (size > 65535)
		size = 65535;

	MSG_WriteString(WriteDest(prog), imgname);
	if (Image_Compress(imgname, size, &buf, &size))
	{
		// actual picture
		MSG_WriteShort(WriteDest(prog), (int)size);
		SZ_Write(WriteDest(prog), (unsigned char *) buf, (int)size);
	}
	else
	{
		// placeholder
		MSG_WriteShort(WriteDest(prog), 0);
	}
}

//////////////////////////////////////////////////////////
RELATED_ (CL_ParseStatic)
// Baker: Zircon extended static entities ...
void Maybe_Prestore_Extra_Static_Attributes (prvm_prog_t *prog, prvm_edict_t *ent)
{
	int extra_effects = 0;

	// Baker: Why is this not checking CL? Answer: We are writing to signon buffer!
	// host_client cannot apply here.
	if (isin1 (sv.protocol, PROTOCOL_DARKPLACES7) &&
		Have_Zircon_Ext_Flag_SV_Hard (ZIRCON_EXT_STATIC_ENT_ALPHA_COLORMOD_SCALE_32)) {
		// Stay
	} else {
		return;
	}

	unsigned char cs_alpha = 255;
	unsigned char cs_colormod[3] = {0};
	unsigned char cs_effects_additive1_fullbright2 = 0;
	unsigned char cs_scale = 16;

	float f;

	int is_shorthand = true;
	//int alphanoshadow = 0;

	int i;
	//unsigned char cs_alpha = 255;
	f = (PRVM_serveredictfloat(ent, alpha) * 255.0f); // ZERO is normal
	if (f)
	{
		extra_effects ++;
		i = (int)f;
		cs_alpha = (unsigned char)bound(0, i, 255);
	}

	prvm_vec_t *v;
	//unsigned char cs_colormod[3] = {0};
	v = PRVM_serveredictvector(ent, colormod);
	if (VectorLength2(v)) {
		extra_effects ++;
		i = (int)(v[0] * 32.0f);cs_colormod[0] = bound(0, i, 255);
		i = (int)(v[1] * 32.0f);cs_colormod[1] = bound(0, i, 255);
		i = (int)(v[2] * 32.0f);cs_colormod[2] = bound(0, i, 255);
		is_shorthand = false;
	}

	//unsigned char cs_scale = 16;
	f = (PRVM_serveredictfloat(ent, scale) * 16.0f);
	if (f) {
		i = (int)f;
		cs_scale = (unsigned char)bound(0, i, 255);
		if (f == 16) {
			is_shorthand = is_shorthand;
		} else if (f == 0) {
			is_shorthand = is_shorthand;
		}
		else {
			is_shorthand = false;
		}

	}

	//unsigned char cs_effects_additive1_fullbright2 = 0;
	i = (unsigned)PRVM_serveredictfloat(ent, effects);

	if (Have_Flag (i, EF_ADDITIVE_32))	{
		extra_effects ++;
		Flag_Add_To (cs_effects_additive1_fullbright2, EF_SHORTY_ADDITIVE_1);
		is_shorthand = false;
	}

	if (Have_Flag (i, EF_FULLBRIGHT)) {
		Flag_Add_To (cs_effects_additive1_fullbright2, EF_SHORTY_FULLBRIGHT_2);
		extra_effects ++;
		is_shorthand = false;
	}

	if (Have_Flag (i, EF_NOSHADOW)) {
		Flag_Add_To (cs_effects_additive1_fullbright2, EF_SHORTY_NOSHADOW_4);
		extra_effects ++;
	}

	if (extra_effects == 0)
		return; // NOTHING

	char vabuf[1024];
	char *sv_hint_string;
	// alpha colormod0 colormod1   colormod2 effects scale = 6 numbers
#if 1
	sv_hint_string = "//!XX" NEWLINE;
	if (is_shorthand) {
		// Baker: Warning %c is not promoted to integer
		// So it should be passed as unsigned char
		// But some compilers might int32_t promote?
		// "Narrow types such as char always get converted to int when passed in a function with ..."
		// So let us be safe an assume we need to do (unsigned char)
		sv_hint_string = va(vabuf, sizeof(vabuf), "//!%c%c" NEWLINE,
			(unsigned char)cs_alpha, (unsigned char)cs_effects_additive1_fullbright2);
	}
#else
	sv_hint_string = va(vabuf, sizeof(vabuf), HINT_MESSAGE_PREFIX "stor 6 %d %d %d %d %d %d" NEWLINE,
		cs_alpha,
		cs_colormod[0],
		cs_colormod[1],
		cs_colormod[2],
		cs_effects_additive1_fullbright2,
		cs_scale
	);
#endif

    //                    alpha 204
	//                          0 1 2 NS SCALE 1/0
	//Game hint: "stor" to "6 204 0 0 0 4 16"
	// alpha noshadow "@ 204 4"


	Con_DPrintLinef	("Sending before static: " QUOTED_S, sv_hint_string); // No newline, hint_string already has one
	MSG_WriteByte	(&sv.signon, svc_stufftext);
	MSG_WriteString (&sv.signon, sv_hint_string);
}


static void VM_SV_makestatic(prvm_prog_t *prog)
{
	prvm_edict_t *ent;
	int			i, large;
	int			is_fitz = isin2 (sv.protocol, PROTOCOL_FITZQUAKE666, PROTOCOL_FITZQUAKE999);
	int			is_rmq	= isin1 (sv.protocol, PROTOCOL_FITZQUAKE999);

	// allow 0 parameters due to an id1 qc bug in which this function is used
	// with no parameters (but directly after setmodel with self in OFS_PARM0)
	VM_SAFEPARMCOUNTRANGE(0, 1, VM_SV_makestatic);

	if (prog->argc >= 1)
		ent = PRVM_G_EDICT(OFS_PARM0);
	else
		ent = PRVM_PROG_TO_EDICT(PRVM_serverglobaledict(self));
	if (ent == prog->edicts)
	{
		VM_WarningLinef (prog, "makestatic: can not modify world entity");
		return;
	}
	if (ent->free)
	{
		VM_WarningLinef (prog, "makestatic: can not modify free entity");
		return;
	}

	Maybe_Prestore_Extra_Static_Attributes (prog, ent);

// Dec 22 2024 unused	int modelindex = (int)PRVM_serveredictfloat(ent, modelindex);

	if (is_fitz) {
		// Baker: This is radically different
		RELATED_ (CL_ParseStatic)
		int fitz_bits = 0;

		if ((int)PRVM_serveredictfloat(ent, modelindex) & 0xFF00)
			fitz_bits |= B_FITZ_LARGEMODEL_1;

		if ((int)PRVM_serveredictfloat(ent, frame) & 0xFF00)
			fitz_bits |= B_FITZ_LARGEFRAME_2;
#if 111
		if ((int)PRVM_serveredictfloat(ent, alpha) != FITZ_ENTALPHA_DEFAULT_0)
			fitz_bits |= B_FITZ_ALPHA_4;
#endif

		if (is_rmq) // DPD99
		{
			//eval_t *val;
			//val = GetEdictFieldValue(ent, "scale");
			//if (val)
			//	ent->scale = ENTSCALE_ENCODE(val->_float);
			//else
#if 000
				ent->scale = FITZ_ENTSCALE_DEFAULT_16;

			if (ent->scale != FITZ_ENTSCALE_DEFAULT_16)
				fitz_bits |= B_FITZ_SCALE_8;
#endif
		}

		if (fitz_bits) {
			MSG_WriteByte (&sv.signon, svcfitz_spawnstatic2);
			MSG_WriteByte (&sv.signon, fitz_bits);
		}
		else
			MSG_WriteByte (&sv.signon, svc_spawnstatic);

// Dec 22 2024 unused		int modelindex = (int)PRVM_serveredictfloat(ent, modelindex);
		if (fitz_bits & B_FITZ_LARGEMODEL_1)
			MSG_WriteShort (&sv.signon, (int)PRVM_serveredictfloat(ent, modelindex));
		else
			MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, modelindex));

		if (fitz_bits & B_FITZ_LARGEFRAME_2)
			MSG_WriteShort (&sv.signon, (int)PRVM_serveredictfloat(ent, frame));
		else
			MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, frame));
		//johnfitz

		MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, colormap));
		MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, skin));
		for (i = 0; i < 3; i++) {
			MSG_WriteCoord(&sv.signon, PRVM_serveredictvector(ent, origin)[i], sv.protocol);
			MSG_WriteAngle(&sv.signon, PRVM_serveredictvector(ent, angles)[i], sv.protocol);
		}

#if 000
		//johnfitz -- PROTOCOL_FITZQUAKE
		if (fitz_bits & B_FITZ_ALPHA_4)
			MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, alpha));
		//johnfitz

		if (fitz_bits & B_FITZ_SCALE_8)
			MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, scale));
#endif

		goto fitzquake_bypass;
	} // is_fitz

	large = false;
	if (PRVM_serveredictfloat(ent, modelindex) >= 256 || PRVM_serveredictfloat(ent, frame) >= 256)
		large = true;
// 999
	if (isin3 (sv.protocol, PROTOCOL_NEHAHRABJP, PROTOCOL_NEHAHRABJP2, PROTOCOL_NEHAHRABJP3)) {
		MSG_WriteByte (&sv.signon,svc_spawnstatic);
		MSG_WriteShort (&sv.signon, (int)PRVM_serveredictfloat(ent, modelindex));
		MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, frame));
	}
	else if (large) {
		MSG_WriteByte (&sv.signon, svc_spawnstatic2);
		MSG_WriteShort (&sv.signon, (int)PRVM_serveredictfloat(ent, modelindex));
		MSG_WriteShort (&sv.signon, (int)PRVM_serveredictfloat(ent, frame));
	}
	else
	{
		MSG_WriteByte (&sv.signon,svc_spawnstatic);
		MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, modelindex));
		MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, frame));
	}

	MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, colormap));
	MSG_WriteByte (&sv.signon, (int)PRVM_serveredictfloat(ent, skin));

	for (i=0 ; i<3 ; i++)
	{
		MSG_WriteCoord(&sv.signon, PRVM_serveredictvector(ent, origin)[i], sv.protocol);
		MSG_WriteAngle(&sv.signon, PRVM_serveredictvector(ent, angles)[i], sv.protocol);
	}


fitzquake_bypass:
// throw the entity away now
	PRVM_ED_Free(prog, ent);
}

//=============================================================================

/*
==============
VM_SV_setspawnparms
==============
*/
static void VM_SV_setspawnparms(prvm_prog_t *prog)
{
	prvm_edict_t	*ent;
	int		i;
	client_t	*client;

	VM_SAFEPARMCOUNT(1, VM_SV_setspawnparms);

	ent = PRVM_G_EDICT(OFS_PARM0);
	i = PRVM_NUM_FOR_EDICT(ent);
	if (i < 1 || i > svs.maxclients || !svs.clients[i-1].active)
	{
		Con_PrintLinef ("tried to setspawnparms on a non-client");
		return;
	}

	// copy spawn parms out of the client_t
	client = svs.clients + i - 1;
	for (i = 0 ; i < NUM_SPAWN_PARMS_16; i ++)
		(&PRVM_serverglobalfloat(parm1))[i] = client->spawn_parms[i];
}

// #79 RERELEASE PF_finalefinished
static void VM_SV_qex_finalefinished (prvm_prog_t *prog)
{
	if (sv.is_qex) {
		// PF_finalefinished -- #79 (RERELEASE)
		PRVM_G_FLOAT(OFS_RETURN) = 0; // Quakespasm does a nop for PF_finalefinished
		return;
	}
	prog->error_cmd ("Unimplemented builtin #79");
	PRVM_G_FLOAT(OFS_RETURN) = 0;
}

// QUAKESPASM:
//static void PF_localsound (void)
//{
//	const char	*sample;
//	int		entnum;
//
//	entnum = G_EDICTNUM(OFS_PARM0);
//	sample = G_STRING(OFS_PARM1);
//	if (entnum < 1 || entnum > svs.maxclients) {
//		Con_Printf ("tried to localsound to a non-client\n");
//		return;
//	}
//	SV_LocalSound (&svs.clients[entnum-1], sample);
//}

//void SV_LocalSound (client_t *client, const char *sample)
//{
//	int	sound_num, field_mask;
//
//	for (sound_num = 1; sound_num < MAX_SOUNDS && sv.sound_precache[sound_num]; sound_num++)
//	{
//		if (!strcmp(sample, sv.sound_precache[sound_num]))
//			break;
//	}
//	if (sound_num == MAX_SOUNDS || !sv.sound_precache[sound_num])
//	{
//		Con_Printf ("SV_LocalSound: %s not precached\n", sample);
//		return;
//	}
//
//	field_mask = 0;
//	if (sound_num >= 256)
//	{
//		if (sv.protocol == PROTOCOL_NETQUAKE)
//			return;
//		field_mask = SND_LARGESOUND;
//	}
//
//	if (client->message.cursize > client->message.maxsize-4)
//		return;
//
//	MSG_WriteByte (&client->message, svc_localsound);
//	MSG_WriteByte (&client->message, field_mask);
//	if (field_mask & SND_LARGESOUND)
//		MSG_WriteShort (&client->message, sound_num);
//	else
//		MSG_WriteByte (&client->message, sound_num);
//}


static void VM_SV_qex_localsound (prvm_prog_t *prog)
{
	if (sv.is_qex) {
		// PF_localsound -- // #80 (RERELEASE) Quakespasm plays a sound void localsound (entity client, string sample) = #80
		PRVM_G_FLOAT(OFS_RETURN) = 0; // Quakespasm sends an svc_localsound
		return;
	}
	prog->error_cmd ("Unimplemented builtin #80");
	PRVM_G_FLOAT(OFS_RETURN) = 0;
}



/*
=================
VM_SV_getlight

Returns a color vector indicating the lighting at the requested point.

(Internal Operation note: actually measures the light beneath the point, just like
                          the model lighting on the client)

getlight(vector)
=================
*/
static void VM_SV_getlight(prvm_prog_t *prog)
{
	vec3_t ambientcolor, diffusecolor, diffusenormal;
	vec3_t p;
	VM_SAFEPARMCOUNT(1, VM_SV_getlight);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), p);
	VectorClear(ambientcolor);
	VectorClear(diffusecolor);
	VectorClear(diffusenormal);
	if (sv.worldmodel && sv.worldmodel->brush.LightPoint)
		sv.worldmodel->brush.LightPoint(sv.worldmodel, p, ambientcolor, diffusecolor, diffusenormal);
	VectorMA(ambientcolor, 0.5, diffusecolor, PRVM_G_VECTOR(OFS_RETURN));
}

typedef struct
{
	unsigned char	type;	// 1/2/8 or 0 to indicate unused
	int		fieldoffset;
}customstat_t;

static customstat_t vm_customstats[MAX_CL_STATS_256]; // matches the regular stat numbers, but only MIN_VM_STAT_32 to MAX_VM_STAT_220 range is used if things are working properly (can register stats from MAX_VM_STAT_220 to MAX_CL_STATS_256 but will warn)
static int vm_customstats_last;

void VM_CustomStats_Clear (void)
{
	memset(vm_customstats, 0, sizeof(vm_customstats));
	vm_customstats_last = -1;
}

void VM_SV_UpdateCustomStats (client_t *client, prvm_edict_t *ent, sizebuf_t *msg, int *stats)
{
	prvm_prog_t *prog = SVVM_prog;
	int			i;
	char		s[17];
	union {
		int i;
		float f;
	} u;

	// Baker: If no custom stats, vm_customstats_last is -1 and loop does not run
	for (i = MIN_VM_STAT_32; i <= vm_customstats_last; i ++) {
		if (!vm_customstats[i].type)
			continue;
		switch(vm_customstats[i].type)
		{
		//string as 16 bytes
		case 1:
			memset(s, 0, 17);
			strlcpy(s, PRVM_E_STRING(ent, vm_customstats[i].fieldoffset), 16);
			stats[i] = s[ 0] + s[ 1] * 256 + s[ 2] * 65536 + s[ 3] * 16777216;
			stats[i+1] = s[ 4] + s[ 5] * 256 + s[ 6] * 65536 + s[ 7] * 16777216;
			stats[i+2] = s[ 8] + s[ 9] * 256 + s[10] * 65536 + s[11] * 16777216;
			stats[i+3] = s[12] + s[13] * 256 + s[14] * 65536 + s[15] * 16777216;
			break;
		//float field sent as-is
		case 8:
			// can't directly use PRVM_E_INT on the field because it may be PRVM_64 and a double is not the representation we want to send
			u.f = PRVM_E_FLOAT(ent, vm_customstats[i].fieldoffset);
			stats[i] = u.i;
			break;
		//integer value of float field
		case 2:
			stats[i] = (int)PRVM_E_FLOAT(ent, vm_customstats[i].fieldoffset);
			break;
		default:
			break;
		}
	}
}

extern cvar_t sv_gameplayfix_customstats;

// void(float index, float type, .void field) SV_AddStat = #232; // addstat
// Set up an auto-sent player stat.
// Client's get their own fields sent to them. Index may not be less than 32.
// Type is a value equating to the ev_ values found in qcc to dictate types. Valid ones are:
//          1: string (4 stats carrying a total of 16 charactures)
//          2: float (one stat, float converted to an integer for transportation)
//          8: integer (one stat, not converted to an int, so this can be used to transport floats as floats - what a unique idea!)
static void VM_SV_AddStat(prvm_prog_t *prog)
{
	int		off, i, type;

	VM_SAFEPARMCOUNT(3, VM_SV_AddStat);

	i		= (int)PRVM_G_FLOAT(OFS_PARM0);
	type	= (int)PRVM_G_FLOAT(OFS_PARM1);
	off		= PRVM_G_INT  (OFS_PARM2);

	switch (type)
	{
	case 1:	// EV_STRING
	case 2: // EV_FLOAT
	case 3: // EV_VECTOR
	case 4: //
	case 8: // EV_INTEGER
		break;
	default:
		VM_WarningLinef (prog, "PF_SV_AddStat: unrecognized type %d - supported types are:" NEWLINE
			"1 - EV_STRING  (string up to 16 bytes, takes 4 stat slots)" NEWLINE
			"2 - EV_FLOAT   (truncate to int32)" NEWLINE
			"3 - EV_VECTOR  (takes 4 stat slots)" NEWLINE
			"8 - EV_INTEGER (send as float)", type);
		return;
	}

	if (i < 0) {
		VM_WarningLinef (prog, "PF_SV_AddStat: index (%d) may not be less than %d", i, MIN_VM_STAT_32);
		return;
	}

	if (i >= MAX_CL_STATS_256) {
		VM_WarningLinef (prog, "PF_SV_AddStat: index (%d) >= MAX_CL_STATS_256 (%d), not supported by protocol, and AddStat beyond MAX_VM_STAT_220 (%d) conflicts with engine MOVEVARS", i, MAX_CL_STATS_256, MAX_VM_STAT_220);
		return;
	}

	if (i > (MAX_CL_STATS_256 - 4) && type == ev_string_1)
	{
		VM_WarningLinef (prog, "PF_SV_AddStat: index (%d) > (MAX_CL_STATS_256 (%d) - 4) with string type won't fit in the protocol, and AddStat beyond MAX_VM_STAT_220 conflicts with engine MOVEVARS", i, MAX_CL_STATS_256);
		return;
	}

	// these are hazardous to override but sort of allowed if one wants to be adventurous...  and enjoys warnings.
	if (i < MIN_VM_STAT_32) {
		// Baker: Quake 1.5 uses stat 0
		// "clientstat(0, EV_FLOAT, health);"
		// This is the same as #define STAT_HEALTH 0
		Con_DPrintLinef ("PF_SV_AddStat: index (%d) < MIN_VM_STAT_32 (%d) may conflict with engine stats - allowed, but this may break things", i, MIN_VM_STAT_32);
	}
	else if (i >= MAX_VM_STAT_220 && !sv_gameplayfix_customstats.integer)
		VM_WarningLinef (prog, "PF_SV_AddStat: index (%d) >= MAX_VM_STAT_220 (%d) conflicts with engine stats - allowed, but this may break slowmo and stuff", i, MAX_VM_STAT_220);
	else if (i > (MAX_VM_STAT_220 - 4) && type == 1 && !sv_gameplayfix_customstats.integer)
		VM_WarningLinef (prog, "PF_SV_AddStat: index (%d) >= MAX_VM_STAT_220 (%d) - 4 with string type won't fit within MAX_VM_STAT_220, thus conflicting with engine stats - allowed, but this may break slowmo and stuff", i, MAX_VM_STAT_220);

	vm_customstats[i].type			= type;
	vm_customstats[i].fieldoffset	= off;
	if (vm_customstats_last < i)
		vm_customstats_last = i;
}

/*
=================
VM_SV_copyentity

copies data from one entity to another

copyentity(src, dst)
=================
*/
static void VM_SV_copyentity(prvm_prog_t *prog)
{
	prvm_edict_t *in, *out;
	VM_SAFEPARMCOUNT(2, VM_SV_copyentity);
	in = PRVM_G_EDICT(OFS_PARM0);
	if (in == prog->edicts)
	{
		VM_WarningLinef (prog, "copyentity: can not read world entity");
		return;
	}
	if (in->free)
	{
		VM_WarningLinef (prog, "copyentity: can not read free entity");
		return;
	}
	out = PRVM_G_EDICT(OFS_PARM1);
	if (out == prog->edicts)
	{
		VM_WarningLinef (prog, "copyentity: can not modify world entity");
		return;
	}
	if (out->free)
	{
		VM_WarningLinef (prog, "copyentity: can not modify free entity");
		return;
	}
	memcpy(out->fields.fp, in->fields.fp, prog->entityfields * sizeof(prvm_vec_t));

	SV_LinkEdict(out);
}


/*
=================
VM_SV_setcolor

sets the color of a client and broadcasts the update to all connected clients

setcolor(clientent, value)
=================
*/
static void VM_SV_setcolor(prvm_prog_t *prog)
{
	client_t *client;
	int entnum, i;

	VM_SAFEPARMCOUNT(2, VM_SV_setcolor);
	entnum = PRVM_G_EDICTNUM(OFS_PARM0);
	i = (int)PRVM_G_FLOAT(OFS_PARM1);

	if (entnum < 1 || entnum > svs.maxclients || !svs.clients[entnum-1].active)
	{
		Con_PrintLinef ("tried to setcolor a non-client");
		return;
	}

	client = svs.clients + entnum-1;
	if (client->edict)
	{
		PRVM_serveredictfloat(client->edict, clientcolors) = i;
		PRVM_serveredictfloat(client->edict, team) = (i & 15) + 1;
	}
	client->colors = i;
	if (client->old_colors != client->colors)
	{
		client->old_colors = client->colors;
		// send notification to all clients
		MSG_WriteByte (&sv.reliable_datagram, svc_updatecolors);
		MSG_WriteByte (&sv.reliable_datagram, client - svs.clients);
		MSG_WriteByte (&sv.reliable_datagram, client->colors);
	}
}

/*
=================
VM_SV_effect

effect(origin, modelname, startframe, framecount, framerate)
=================
*/
static void VM_SV_effect(prvm_prog_t *prog)
{
	int i;
	const char *s;
	vec3_t org;
	VM_SAFEPARMCOUNT(5, VM_SV_effect);
	s = PRVM_G_STRING(OFS_PARM1);
	if (!s[0])
	{
		VM_WarningLinef (prog, "effect: no model specified");
		return;
	}

	i = SV_ModelIndex(s, PRECACHE_MODE_1);
	if (!i)
	{
		VM_WarningLinef (prog, "effect: model not precached");
		return;
	}

	if (PRVM_G_FLOAT(OFS_PARM3) < 1)
	{
		VM_WarningLinef (prog, "effect: framecount < 1");
		return;
	}

	if (PRVM_G_FLOAT(OFS_PARM4) < 1)
	{
		VM_WarningLinef (prog, "effect: framerate < 1");
		return;
	}

	VectorCopy(PRVM_G_VECTOR(OFS_PARM0), org);
	SV_StartEffect(org, i, (int)PRVM_G_FLOAT(OFS_PARM2), (int)PRVM_G_FLOAT(OFS_PARM3), (int)PRVM_G_FLOAT(OFS_PARM4));
}

static void VM_SV_te_blood(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_blood);
	if (PRVM_G_FLOAT(OFS_PARM2) < 1)
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_BLOOD);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// velocity
	MSG_WriteChar(&sv.datagram, bound(-128, (int) PRVM_G_VECTOR(OFS_PARM1)[0], 127));
	MSG_WriteChar(&sv.datagram, bound(-128, (int) PRVM_G_VECTOR(OFS_PARM1)[1], 127));
	MSG_WriteChar(&sv.datagram, bound(-128, (int) PRVM_G_VECTOR(OFS_PARM1)[2], 127));
	// count
	MSG_WriteByte(&sv.datagram, bound(0, (int) PRVM_G_FLOAT(OFS_PARM2), 255));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_bloodshower(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(4, VM_SV_te_bloodshower);
	if (PRVM_G_FLOAT(OFS_PARM3) < 1)
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_BLOODSHOWER);
	// min
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// max
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// speed
	MSG_WriteCoord(&sv.datagram, PRVM_G_FLOAT(OFS_PARM2), sv.protocol);
	// count
	MSG_WriteShort(&sv.datagram, (int)bound(0, PRVM_G_FLOAT(OFS_PARM3), 65535));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_explosionrgb(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(2, VM_SV_te_explosionrgb);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_EXPLOSIONRGB);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// color
	MSG_WriteByte(&sv.datagram, bound(0, (int) (PRVM_G_VECTOR(OFS_PARM1)[0] * 255), 255));
	MSG_WriteByte(&sv.datagram, bound(0, (int) (PRVM_G_VECTOR(OFS_PARM1)[1] * 255), 255));
	MSG_WriteByte(&sv.datagram, bound(0, (int) (PRVM_G_VECTOR(OFS_PARM1)[2] * 255), 255));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_particlecube(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(7, VM_SV_te_particlecube);
	if (PRVM_G_FLOAT(OFS_PARM3) < 1)
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_PARTICLECUBE);
	// min
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// max
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// velocity
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	// count
	MSG_WriteShort(&sv.datagram, (int)bound(0, PRVM_G_FLOAT(OFS_PARM3), 65535));
	// color
	MSG_WriteByte(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM4));
	// gravity true/false
	MSG_WriteByte(&sv.datagram, ((int) PRVM_G_FLOAT(OFS_PARM5)) != 0);
	// randomvel
	MSG_WriteCoord(&sv.datagram, PRVM_G_FLOAT(OFS_PARM6), sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_particlerain(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(5, VM_SV_te_particlerain);
	if (PRVM_G_FLOAT(OFS_PARM3) < 1)
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_PARTICLERAIN);
	// min
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// max
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// velocity
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	// count
	MSG_WriteShort(&sv.datagram, (int)bound(0, PRVM_G_FLOAT(OFS_PARM3), 65535));
	// color
	MSG_WriteByte(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM4));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_particlesnow(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(5, VM_SV_te_particlesnow);
	if (PRVM_G_FLOAT(OFS_PARM3) < 1)
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_PARTICLESNOW);
	// min
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// max
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// velocity
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	// count
	MSG_WriteShort(&sv.datagram, (int)bound(0, PRVM_G_FLOAT(OFS_PARM3), 65535));
	// color
	MSG_WriteByte(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM4));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_spark(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_spark);
	if (PRVM_G_FLOAT(OFS_PARM2) < 1)
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_SPARK);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// velocity
	MSG_WriteChar(&sv.datagram, bound(-128, (int) PRVM_G_VECTOR(OFS_PARM1)[0], 127));
	MSG_WriteChar(&sv.datagram, bound(-128, (int) PRVM_G_VECTOR(OFS_PARM1)[1], 127));
	MSG_WriteChar(&sv.datagram, bound(-128, (int) PRVM_G_VECTOR(OFS_PARM1)[2], 127));
	// count
	MSG_WriteByte(&sv.datagram, bound(0, (int) PRVM_G_FLOAT(OFS_PARM2), 255));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_gunshotquad(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_gunshotquad);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_GUNSHOTQUAD);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_spikequad(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_spikequad);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_SPIKEQUAD);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_superspikequad(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_superspikequad);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_SUPERSPIKEQUAD);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_explosionquad(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_explosionquad);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_EXPLOSIONQUAD);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_smallflash(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_smallflash);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_SMALLFLASH);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_customflash(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(4, VM_SV_te_customflash);
	if (PRVM_G_FLOAT(OFS_PARM1) < 8 || PRVM_G_FLOAT(OFS_PARM2) < (1.0 / 256.0))
		return;
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_CUSTOMFLASH);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// radius
	MSG_WriteByte(&sv.datagram, (int)bound(0, PRVM_G_FLOAT(OFS_PARM1) / 8 - 1, 255));
	// lifetime
	MSG_WriteByte(&sv.datagram, (int)bound(0, PRVM_G_FLOAT(OFS_PARM2) * 256 - 1, 255));
	// color
	MSG_WriteByte(&sv.datagram, (int)bound(0, PRVM_G_VECTOR(OFS_PARM3)[0] * 255, 255));
	MSG_WriteByte(&sv.datagram, (int)bound(0, PRVM_G_VECTOR(OFS_PARM3)[1] * 255, 255));
	MSG_WriteByte(&sv.datagram, (int)bound(0, PRVM_G_VECTOR(OFS_PARM3)[2] * 255, 255));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_gunshot(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_gunshot);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_GUNSHOT);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_spike(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_spike);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_SPIKE);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_superspike(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_superspike);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_SUPERSPIKE);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_explosion(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_explosion);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_EXPLOSION);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_tarexplosion(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_tarexplosion);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_TAREXPLOSION);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_wizspike(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_wizspike);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_WIZSPIKE);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_knightspike(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_knightspike);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_KNIGHTSPIKE);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_lavasplash(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_lavasplash);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_LAVASPLASH);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_teleport(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_teleport);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_TELEPORT);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_explosion2(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_explosion2);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_EXPLOSION2);
	// origin
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// color
	MSG_WriteByte(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM1));
	MSG_WriteByte(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM2));
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_lightning1(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_lightning1);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_LIGHTNING1);
	// owner entity
	MSG_WriteShort(&sv.datagram, PRVM_G_EDICTNUM(OFS_PARM0));
	// start
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// end
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_lightning2(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_lightning2);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_LIGHTNING2);
	// owner entity
	MSG_WriteShort(&sv.datagram, PRVM_G_EDICTNUM(OFS_PARM0));
	// start
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// end
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_lightning3(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_lightning3);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_LIGHTNING3);
	// owner entity
	MSG_WriteShort(&sv.datagram, PRVM_G_EDICTNUM(OFS_PARM0));
	// start
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// end
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_beam(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_beam);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_BEAM);
	// owner entity
	MSG_WriteShort(&sv.datagram, PRVM_G_EDICTNUM(OFS_PARM0));
	// start
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// end
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM2)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_plasmaburn(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_te_plasmaburn);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_PLASMABURN);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	SV_FlushBroadcastMessages();
}

static void VM_SV_te_flamejet(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(3, VM_SV_te_flamejet);
	MSG_WriteByte(&sv.datagram, svc_temp_entity);
	MSG_WriteByte(&sv.datagram, TE_FLAMEJET);
	// org
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM0)[2], sv.protocol);
	// vel
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[0], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[1], sv.protocol);
	MSG_WriteCoord(&sv.datagram, PRVM_G_VECTOR(OFS_PARM1)[2], sv.protocol);
	// count
	MSG_WriteByte(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM2));
	SV_FlushBroadcastMessages();
}

//void(entity e, string s) clientcommand = #440; // executes a command string as if it came from the specified client
//this function originally written by KrimZon, made shorter by LadyHavoc
static void VM_SV_clientcommand(prvm_prog_t *prog)
{
	client_t *temp_client;
	int i;
	VM_SAFEPARMCOUNT(2, VM_SV_clientcommand);

	//find client for this entity
	i = (PRVM_NUM_FOR_EDICT(PRVM_G_EDICT(OFS_PARM0)) - 1);
	if (i < 0 || i >= svs.maxclients || !svs.clients[i].active)
	{
		Con_Print("PF_clientcommand: entity is not a client\n");
		return;
	}

	temp_client = host_client;
	host_client = svs.clients + i;
	Cmd_ExecuteString(cmd_serverfromclient, PRVM_G_STRING(OFS_PARM1), src_client, true);
	host_client = temp_client;
}

//void(entity e, entity tagentity, string tagname) setattachment = #443; // attachs e to a tag on tagentity (note: use "" to attach to entity origin/angles instead of a tag)
static void VM_SV_setattachment(prvm_prog_t *prog)
{
	prvm_edict_t *e = PRVM_G_EDICT(OFS_PARM0);
	prvm_edict_t *tagentity = PRVM_G_EDICT(OFS_PARM1);
	const char *tagname = PRVM_G_STRING(OFS_PARM2);
	model_t *model;
	int tagindex;
	VM_SAFEPARMCOUNT(3, VM_SV_setattachment);

	if (e == prog->edicts)
	{
		VM_WarningLinef (prog, "setattachment: can not modify world entity");
		return;
	}
	if (e->free)
	{
		VM_WarningLinef (prog, "setattachment: can not modify free entity");
		return;
	}

	if (tagentity == NULL)
		tagentity = prog->edicts;

	tagindex = 0;

	if (tagentity != NULL && tagentity != prog->edicts && tagname && tagname[0])
	{
		model = SV_GetModelFromEdict(tagentity);
		if (model)
		{
			tagindex = Mod_Alias_GetTagIndexForName(model, (int)PRVM_serveredictfloat(tagentity, skin), tagname);
			if (tagindex == 0)
				Con_DPrintLinef ("setattachment(edict %d, edict %d, string " QUOTED_S "): tried to find tag named " QUOTED_S " on entity %d (model " QUOTED_S ") but could not find it", PRVM_NUM_FOR_EDICT(e), PRVM_NUM_FOR_EDICT(tagentity), tagname, tagname, PRVM_NUM_FOR_EDICT(tagentity), model->model_name);
		}
		else
			Con_DPrintLinef ("setattachment(edict %d, edict %d, string " QUOTED_S "): tried to find tag named " QUOTED_S " on entity %d but it has no model", PRVM_NUM_FOR_EDICT(e), PRVM_NUM_FOR_EDICT(tagentity), tagname, tagname, PRVM_NUM_FOR_EDICT(tagentity));
	}

	PRVM_serveredictedict(e, tag_entity) = PRVM_EDICT_TO_PROG(tagentity); // TAGX
	PRVM_serveredictfloat(e, tag_index) = tagindex; // TAGX
}

/////////////////////////////////////////
// DP_MD3_TAGINFO extension coded by VorteX

static int SV_GetTagIndex (prvm_prog_t *prog, prvm_edict_t *e, const char *tagname)
{
	int i;

	i = (int)PRVM_serveredictfloat(e, modelindex);
	if (i < 1 || i >= MAX_MODELS_8192)
		return -1;

	return Mod_Alias_GetTagIndexForName(SV_GetModelByIndex(i), (int)PRVM_serveredictfloat(e, skin), tagname);
}

static int SV_GetExtendedTagInfo (prvm_prog_t *prog, prvm_edict_t *e, int tagindex, int *parentindex, const char **tagname, matrix4x4_t *tag_localmatrix)
{
	int r;
	model_t *model;

	*tagname = NULL;
	*parentindex = 0;
	Matrix4x4_CreateIdentity(tag_localmatrix);

	if (tagindex >= 0 && (model = SV_GetModelFromEdict(e)) && model->num_bones)
	{
		r = Mod_Alias_GetExtendedTagInfoForIndex(model, (int)PRVM_serveredictfloat(e, skin), e->priv.server->frameblend, &e->priv.server->skeleton, tagindex - 1, parentindex, tagname, tag_localmatrix);

		if (!r) // success?
			*parentindex += 1;

		return r;
	}

	return 1;
}

void SV_GetEntityMatrix (prvm_prog_t *prog, prvm_edict_t *ent, matrix4x4_t *out, qbool viewmatrix)
{
	float scale;
	float pitchsign = 1;

	scale = PRVM_serveredictfloat(ent, scale);
	if (!scale)
		scale = 1.0f;

	if (viewmatrix)
		Matrix4x4_CreateFromQuakeEntity(out, PRVM_serveredictvector(ent, origin)[0], PRVM_serveredictvector(ent, origin)[1], PRVM_serveredictvector(ent, origin)[2] + PRVM_serveredictvector(ent, view_ofs)[2], PRVM_serveredictvector(ent, v_angle)[0], PRVM_serveredictvector(ent, v_angle)[1], PRVM_serveredictvector(ent, v_angle)[2], scale * cl_viewmodel_scale.value);
	else
	{
		pitchsign = SV_GetPitchSign(prog, ent);
		Matrix4x4_CreateFromQuakeEntity(out, PRVM_serveredictvector(ent, origin)[0], PRVM_serveredictvector(ent, origin)[1], PRVM_serveredictvector(ent, origin)[2], pitchsign * PRVM_serveredictvector(ent, angles)[0], PRVM_serveredictvector(ent, angles)[1], PRVM_serveredictvector(ent, angles)[2], scale);
	}
}

static int SV_GetEntityLocalTagMatrix(prvm_prog_t *prog, prvm_edict_t *ent, int tagindex, matrix4x4_t *out)
{
	model_t *model;
	if (tagindex >= 0 && (model = SV_GetModelFromEdict(ent)) && model->animscenes)
	{
		VM_GenerateFrameGroupBlend(prog, ent->priv.server->framegroupblend, ent);
		VM_FrameBlendFromFrameGroupBlend(ent->priv.server->frameblend, ent->priv.server->framegroupblend, model, sv.time);
		VM_UpdateEdictSkeleton(prog, ent, model, ent->priv.server->frameblend);
		return Mod_Alias_GetTagMatrix(model, ent->priv.server->frameblend, &ent->priv.server->skeleton, tagindex, out);
	}
	*out = identitymatrix;
	return 0;
}

// Warnings/errors code:
// 0 - normal (everything all-right)
// 1 - world entity
// 2 - free entity
// 3 - null or non-precached model
// 4 - no tags with requested index
// 5 - runaway loop at attachment chain
static int SV_GetTagMatrix (prvm_prog_t *prog, matrix4x4_t *out, prvm_edict_t *ent, int tagindex)
{
	int ret;
	int modelindex, attachloop;
	matrix4x4_t entitymatrix, tagmatrix, attachmatrix;
	model_t *model;

	*out = identitymatrix; // warnings and errors return identical matrix

	if (ent == prog->edicts)
		return 1;
	if (ent->free)
		return 2;

	modelindex = (int)PRVM_serveredictfloat(ent, modelindex);
	if (modelindex <= 0 || modelindex >= MAX_MODELS_8192)
		return 3;

	model = SV_GetModelByIndex(modelindex);

	VM_GenerateFrameGroupBlend(prog, ent->priv.server->framegroupblend, ent);
	VM_FrameBlendFromFrameGroupBlend(ent->priv.server->frameblend, ent->priv.server->framegroupblend, model, sv.time);
	VM_UpdateEdictSkeleton(prog, ent, model, ent->priv.server->frameblend);

	tagmatrix = identitymatrix;
	// DP_GFX_QUAKE3MODELTAGS, scan all chain and stop on unattached entity
	attachloop = 0;
	for (;;)
	{
		if (attachloop >= 256) // prevent runaway looping
			return 5;
		// apply transformation by child's tagindex on parent entity and then
		// by parent entity itself
		ret = SV_GetEntityLocalTagMatrix(prog, ent, tagindex - 1, &attachmatrix);
		if (ret && attachloop == 0)
			return ret;
		SV_GetEntityMatrix(prog, ent, &entitymatrix, false);
		Matrix4x4_Concat(&tagmatrix, &attachmatrix, out);
		Matrix4x4_Concat(out, &entitymatrix, &tagmatrix);
		// next iteration we process the parent entity
		if (PRVM_serveredictedict(ent, tag_entity))
		{
			tagindex = (int)PRVM_serveredictfloat(ent, tag_index);
			ent = PRVM_EDICT_NUM(PRVM_serveredictedict(ent, tag_entity));
		}
		else
			break;
		attachloop++;
	}

	// RENDER_VIEWMODEL magic
	if (PRVM_serveredictedict(ent, viewmodelforclient))
	{
		Matrix4x4_Copy(&tagmatrix, out);
		ent = PRVM_EDICT_NUM(PRVM_serveredictedict(ent, viewmodelforclient));

		SV_GetEntityMatrix(prog, ent, &entitymatrix, true);
		Matrix4x4_Concat(out, &entitymatrix, &tagmatrix);
	}
	return 0;
}

//float(entity ent, string tagname) gettagindex;

static void VM_SV_gettagindex(prvm_prog_t *prog)
{
	prvm_edict_t *ent;
	const char *tag_name;
	int tag_index;

	VM_SAFEPARMCOUNT(2, VM_SV_gettagindex);

	ent = PRVM_G_EDICT(OFS_PARM0);
	tag_name = PRVM_G_STRING(OFS_PARM1);

	if (ent == prog->edicts)
	{
		VM_WarningLinef (prog, "VM_SV_gettagindex(entity #%d): can't affect world entity", PRVM_NUM_FOR_EDICT(ent));
		return;
	}
	if (ent->free)
	{
		VM_WarningLinef (prog, "VM_SV_gettagindex(entity #%d): can't affect free entity", PRVM_NUM_FOR_EDICT(ent));
		return;
	}

	tag_index = 0;
	if (!SV_GetModelFromEdict(ent))
		Con_DPrintLinef ("VM_SV_gettagindex(entity #%d): null or non-precached model", PRVM_NUM_FOR_EDICT(ent));
	else
	{
		tag_index = SV_GetTagIndex(prog, ent, tag_name); // TAGX
		if (tag_index == 0)
			if (developer_extra.integer)
				Con_DPrintLinef ("VM_SV_gettagindex(entity #%d): tag " QUOTED_S " not found", PRVM_NUM_FOR_EDICT(ent), tag_name);
	}
	PRVM_G_FLOAT(OFS_RETURN) = tag_index;
}

//vector(entity ent, float tagindex) gettaginfo;
static void VM_SV_gettaginfo(prvm_prog_t *prog)
{
	prvm_edict_t *e;
	int tagindex;
	matrix4x4_t tag_matrix;
	matrix4x4_t tag_localmatrix;
	int parentindex;
	const char *tagname;
	int returncode;
	vec3_t forward, left, up, origin;
	const model_t *model;

	VM_SAFEPARMCOUNT(2, VM_SV_gettaginfo);

	e = PRVM_G_EDICT(OFS_PARM0);
	tagindex = (int)PRVM_G_FLOAT(OFS_PARM1);

	returncode = SV_GetTagMatrix(prog, &tag_matrix, e, tagindex);
	Matrix4x4_ToVectors(&tag_matrix, forward, left, up, origin);
	VectorCopy(forward, PRVM_serverglobalvector(v_forward));
	VectorNegate(left, PRVM_serverglobalvector(v_right));
	VectorCopy(up, PRVM_serverglobalvector(v_up));
	VectorCopy(origin, PRVM_G_VECTOR(OFS_RETURN));
	model = SV_GetModelFromEdict(e);
	VM_GenerateFrameGroupBlend(prog, e->priv.server->framegroupblend, e);
	VM_FrameBlendFromFrameGroupBlend(e->priv.server->frameblend, e->priv.server->framegroupblend, model, sv.time);
	VM_UpdateEdictSkeleton(prog, e, model, e->priv.server->frameblend);
	SV_GetExtendedTagInfo(prog, e, tagindex, &parentindex, &tagname, &tag_localmatrix);
	Matrix4x4_ToVectors(&tag_localmatrix, forward, left, up, origin);

	PRVM_serverglobalfloat(gettaginfo_parent) = parentindex;
	PRVM_serverglobalstring(gettaginfo_name) = tagname ? PRVM_SetTempString(prog, tagname) : 0;
	VectorCopy(forward, PRVM_serverglobalvector(gettaginfo_forward));
	VectorNegate(left, PRVM_serverglobalvector(gettaginfo_right));
	VectorCopy(up, PRVM_serverglobalvector(gettaginfo_up));
	VectorCopy(origin, PRVM_serverglobalvector(gettaginfo_offset));

	switch(returncode)
	{
		case 1:
			VM_WarningLinef (prog, "gettagindex: can't affect world entity");
			break;
		case 2:
			VM_WarningLinef (prog, "gettagindex: can't affect free entity");
			break;
		case 3:
			Con_DPrintLinef ("SV_GetTagMatrix(entity #%d): null or non-precached model", PRVM_NUM_FOR_EDICT(e));
			break;
		case 4:
			Con_DPrintLinef ("SV_GetTagMatrix(entity #%d): model has no tag with requested index %d", PRVM_NUM_FOR_EDICT(e), tagindex);
			break;
		case 5:
			Con_DPrintLinef ("SV_GetTagMatrix(entity #%d): runaway loop at attachment chain", PRVM_NUM_FOR_EDICT(e));
			break;
	}
}

//void(entity clent) dropclient (DP_SV_DROPCLIENT)
static void VM_SV_dropclient(prvm_prog_t *prog)
{
	int clientnum;
	client_t *oldhostclient;
	VM_SAFEPARMCOUNT(1, VM_SV_dropclient);
	clientnum = PRVM_G_EDICTNUM(OFS_PARM0) - 1;
	if (clientnum < 0 || clientnum >= svs.maxclients)
	{
		VM_WarningLinef (prog, "dropclient: not a client");
		return;
	}
	if (!svs.clients[clientnum].active)
	{
		VM_WarningLinef (prog, "dropclient: that client slot is not connected");
		return;
	}
	oldhostclient = host_client;
	host_client = svs.clients + clientnum;
	SV_DropClient(false, "Client dropped");
	host_client = oldhostclient;
}

//entity() spawnclient (DP_SV_BOTCLIENT)
static void VM_SV_spawnclient(prvm_prog_t *prog)
{
	int i;
	prvm_edict_t	*ed;
	VM_SAFEPARMCOUNT(0, VM_SV_spawnclient);
	prog->xfunction->builtinsprofile += 2;
	ed = prog->edicts;
	for (i = 0;i < svs.maxclients;i++)
	{
		if (!svs.clients[i].active)
		{
			prog->xfunction->builtinsprofile += 100;
			SV_ConnectClient (i, NULL);
			// this has to be set or else ClientDisconnect won't be called
			// we assume the qc will call ClientConnect...
			svs.clients[i].clientconnectcalled = true;
			ed = PRVM_EDICT_NUM(i + 1);
			break;
		}
	}
	VM_RETURN_EDICT(ed);
}

//float(entity clent) clienttype (DP_SV_BOTCLIENT)
static void VM_SV_clienttype(prvm_prog_t *prog)
{
	int clientnum;
	VM_SAFEPARMCOUNT(1, VM_SV_clienttype);
	clientnum = PRVM_G_EDICTNUM(OFS_PARM0) - 1;
	if (clientnum < 0 || clientnum >= svs.maxclients)
		PRVM_G_FLOAT(OFS_RETURN) = 3; // CLIENTTYPE_NOTACLIENT
	else if (!svs.clients[clientnum].active)
		PRVM_G_FLOAT(OFS_RETURN) = 0; // CLIENTTYPE_DISCONNECTED
	else if (svs.clients[clientnum].netconnection)
		PRVM_G_FLOAT(OFS_RETURN) = 1; // CLIENTTYPE_REAL
	else
		PRVM_G_FLOAT(OFS_RETURN) = 2; // CLIENTTYPE_BOT
}

/*
===============
VM_SV_serverkey

string(string key) serverkey
===============
*/
static void VM_SV_serverkey(prvm_prog_t *prog)
{
	char string[VM_STRINGTEMP_LENGTH_16384];
	VM_SAFEPARMCOUNT(1, VM_SV_serverkey);
	InfoString_GetValue(svs.serverinfo, PRVM_G_STRING(OFS_PARM0), string, sizeof(string));
	PRVM_G_INT(OFS_RETURN) = PRVM_SetTempString(prog, string);
}

//#333 void(entity e, float mdlindex) setmodelindex (EXT_CSQC)
static void VM_SV_setmodelindex(prvm_prog_t *prog)
{
	prvm_edict_t	*e;
	model_t	*mod;
	int		i;
	VM_SAFEPARMCOUNT(2, VM_SV_setmodelindex);

	e = PRVM_G_EDICT(OFS_PARM0);
	if (e == prog->edicts)
	{
		VM_WarningLinef (prog, "setmodelindex: can not modify world entity");
		return;
	}
	if (e->free)
	{
		VM_WarningLinef (prog, "setmodelindex: can not modify free entity");
		return;
	}
	i = (int)PRVM_G_FLOAT(OFS_PARM1);
	if (i <= 0 || i >= MAX_MODELS_8192)
	{
		VM_WarningLinef (prog, "setmodelindex: invalid modelindex");
		return;
	}
	if (!sv.model_precache[i][0])
	{
		VM_WarningLinef (prog, "setmodelindex: model not precached");
		return;
	}

	PRVM_serveredictstring(e, model) = PRVM_SetEngineString(prog, sv.model_precache[i]);
	PRVM_serveredictfloat(e, modelindex) = i;

	mod = SV_GetModelByIndex(i);

	if (mod)
	{
		if (mod->type != mod_alias || sv_gameplayfix_setmodelrealbox.integer)
			SetMinMaxSize(prog, e, mod->normalmins, mod->normalmaxs, true);
		else
			SetMinMaxSize(prog, e, quakemins, quakemaxs, true);
	}
	else
		SetMinMaxSize(prog, e, vec3_origin, vec3_origin, true);
}

//#334 string(float mdlindex) modelnameforindex (EXT_CSQC)
static void VM_SV_modelnameforindex(prvm_prog_t *prog)
{
	int i;
	VM_SAFEPARMCOUNT(1, VM_SV_modelnameforindex);

	PRVM_G_INT(OFS_RETURN) = OFS_NULL;

	i = (int)PRVM_G_FLOAT(OFS_PARM0);
	if (i <= 0 || i >= MAX_MODELS_8192)
	{
		VM_WarningLinef (prog, "modelnameforindex: invalid modelindex");
		return;
	}
	if (!sv.model_precache[i][0])
	{
		VM_WarningLinef (prog, "modelnameforindex: model not precached");
		return;
	}

	PRVM_G_INT(OFS_RETURN) = PRVM_SetEngineString(prog, sv.model_precache[i]);
}

//#335 float(string effectname) particleeffectnum (EXT_CSQC)
static void VM_SV_particleeffectnum(prvm_prog_t *prog)
{
	int			i;
	VM_SAFEPARMCOUNT(1, VM_SV_particleeffectnum);
	i = SV_ParticleEffectIndex(PRVM_G_STRING(OFS_PARM0));
	if (i == 0)
		i = -1;
	PRVM_G_FLOAT(OFS_RETURN) = i;
}

// #336 void(entity ent, float effectnum, vector start, vector end) trailparticles (EXT_CSQC)
static void VM_SV_trailparticles(prvm_prog_t *prog)
{
	vec3_t start, end;
	VM_SAFEPARMCOUNT(4, VM_SV_trailparticles);

	if ((int)PRVM_G_FLOAT(OFS_PARM0) < 0)
		return;

	MSG_WriteByte(&sv.datagram, svc_trailparticles);
	MSG_WriteShort(&sv.datagram, PRVM_G_EDICTNUM(OFS_PARM0));
	MSG_WriteShort(&sv.datagram, (int)PRVM_G_FLOAT(OFS_PARM1));
	VectorCopy(PRVM_G_VECTOR(OFS_PARM2), start);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM3), end);
	MSG_WriteVector(&sv.datagram, start, sv.protocol);
	MSG_WriteVector(&sv.datagram, end, sv.protocol);
	SV_FlushBroadcastMessages();
}

//#337 void(float effectnum, vector origin, vector dir, float count) pointparticles (EXT_CSQC)
RELATED_ (VM_CL_pointparticles -> CL_ParticleEffect -> CL_ParticleBox -> CL_NewParticlesFromEffectinfo)
RELATED_ (svc_pointparticles1 -> CL_ParsePointParticles1 -> CL_ParticleEffect -> CL_ParticleBox -> CL_NewParticlesFromEffectinfo)

RELATED_ (CL_NewParticle superboss)
// Baker: Write effectnum, org, velocity, count to CL
// Baker: Spike says better would be precache the particles effectnums with name
// Baker: Particle effect nums are very fragile.
static void VM_SV_pointparticles(prvm_prog_t *prog)
{
	int effectnum, count;
	vec3_t org, vel;
	VM_SAFEPARMCOUNTRANGE(4, 8, VM_SV_pointparticles);

	if ((int)PRVM_G_FLOAT(OFS_PARM0) < 0)
		return;

	effectnum = (int)PRVM_G_FLOAT(OFS_PARM0);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM1), org);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM2), vel);
	count = bound(0, (int)PRVM_G_FLOAT(OFS_PARM3), 65535);
	if (count == 1 && !VectorLength2(vel)) {
		// 1+2+12=15 bytes
		MSG_WriteByte(&sv.datagram, svc_pointparticles1);
		MSG_WriteShort(&sv.datagram, effectnum);
		MSG_WriteVector(&sv.datagram, org, sv.protocol);
	}
	else
	{
		// 1+2+12+12+2=29 bytes
		MSG_WriteByte(&sv.datagram, svc_pointparticles);
		MSG_WriteShort(&sv.datagram, effectnum);
		MSG_WriteVector(&sv.datagram, org, sv.protocol);
		MSG_WriteVector(&sv.datagram, vel, sv.protocol);
		MSG_WriteShort(&sv.datagram, count);
	}

	SV_FlushBroadcastMessages();
}

qbool SV_VM_ConsoleCommand (const char *text)
{
	prvm_prog_t *prog = SVVM_prog;
	return PRVM_ConsoleCommand(prog, text, &prog->funcoffsets.ConsoleCmd, true, PRVM_EDICT_TO_PROG(sv.world.prog->edicts), sv.time,  !(!sv.active || !prog || !prog->loaded), "QC function ConsoleCmd is missing");
}

// #352 void(string cmdname) registercommand (EXT_CSQC)
static void VM_SV_registercommand (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_SV_registercmd);
	Cmd_AddCommand(CF_SERVER, PRVM_G_STRING(OFS_PARM0), NULL, "console command created by QuakeC");
}

//PF_setpause,    // void(float pause) setpause	= #531;
static void VM_SV_setpause(prvm_prog_t *prog) {
	int pauseValue;
	pauseValue = (int)PRVM_G_FLOAT(OFS_PARM0);
	if (pauseValue != 0) { //pause the game
		sv.paused = 1;
		sv.pausedstart = host.realtime;
	} else { //disable pause, in case it was enabled
		if (sv.paused != 0) {
			sv.paused = 0;
			sv.pausedstart = 0;
		}
	}
	// send notification to all clients
	MSG_WriteByte(&sv.reliable_datagram, svc_setpause);
	MSG_WriteByte(&sv.reliable_datagram, sv.paused);
}

// #263 float(float modlindex) skel_create = #263; // (FTE_CSQC_SKELETONOBJECTS) create a skeleton (be sure to assign this value into .skeletonindex for use), returns skeleton index (1 or higher) on success, returns 0 on failure  (for example if the modelindex is not skeletal), it is recommended that you create a new skeleton if you change modelindex.
static void VM_SV_skel_create(prvm_prog_t *prog)
{
	int modelindex = (int)PRVM_G_FLOAT(OFS_PARM0);
	model_t *model = SV_GetModelByIndex(modelindex);
	skeleton_t *skeleton;
	int i;
	PRVM_G_FLOAT(OFS_RETURN) = 0;
	if (!model || !model->num_bones)
		return;
	for (i = 0;i < MAX_EDICTS_32768;i++)
		if (!prog->skeletons[i])
			break;
	if (i == MAX_EDICTS_32768)
		return;
	prog->skeletons[i] = skeleton = (skeleton_t *)Mem_Alloc(prog->progs_mempool, sizeof(skeleton_t) + model->num_bones * sizeof(matrix4x4_t));
	PRVM_G_FLOAT(OFS_RETURN) = i + 1;
	skeleton->model = model;
	skeleton->relativetransforms = (matrix4x4_t *)(skeleton+1);
	// initialize to identity matrices
	for (i = 0;i < skeleton->model->num_bones;i++)
		skeleton->relativetransforms[i] = identitymatrix;
}

// #264 float(float skel, entity ent, float modlindex, float retainfrac, float firstbone, float lastbone) skel_build = #264; // (FTE_CSQC_SKELETONOBJECTS) blend in a percentage of standard animation, 0 replaces entirely, 1 does nothing, 0.5 blends half, etc, and this only alters the bones in the specified range for which out of bounds values like 0,100000 are safe (uses .frame, .frame2, .frame3, .frame4, .lerpfrac, .lerpfrac3, .lerpfrac4, .frame1time, .frame2time, .frame3time, .frame4time), returns skel on success, 0 on failure
static void VM_SV_skel_build(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	skeleton_t *skeleton;
	prvm_edict_t *ed = PRVM_G_EDICT(OFS_PARM1);
	int modelindex = (int)PRVM_G_FLOAT(OFS_PARM2);
	float retainfrac = PRVM_G_FLOAT(OFS_PARM3);
	int firstbone = PRVM_G_FLOAT(OFS_PARM4) - 1;
	int lastbone = PRVM_G_FLOAT(OFS_PARM5) - 1;
	model_t *model = SV_GetModelByIndex(modelindex);
	int numblends;
	int bonenum;
	int blendindex;
	framegroupblend_t framegroupblend[MAX_FRAMEGROUPBLENDS_4];
	frameblend_t frameblend[MAX_FRAMEBLENDS_8];
	matrix4x4_t bonematrix;
	matrix4x4_t matrix;
	PRVM_G_FLOAT(OFS_RETURN) = 0;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	firstbone = max(0, firstbone);
	lastbone = min(lastbone, model->num_bones - 1);
	lastbone = min(lastbone, skeleton->model->num_bones - 1);
	VM_GenerateFrameGroupBlend(prog, framegroupblend, ed);
	VM_FrameBlendFromFrameGroupBlend(frameblend, framegroupblend, model, sv.time);
	for (numblends = 0;numblends < MAX_FRAMEBLENDS_8 && frameblend[numblends].rlerp;numblends++)
		;
	for (bonenum = firstbone;bonenum <= lastbone;bonenum++)
	{
		memset(&bonematrix, 0, sizeof(bonematrix));
		for (blendindex = 0;blendindex < numblends;blendindex++)
		{
			Matrix4x4_FromBonePose7s(&matrix, model->num_posescale, model->data_poses7s + 7 * (frameblend[blendindex].subframe * model->num_bones + bonenum));
			Matrix4x4_Accumulate(&bonematrix, &matrix, frameblend[blendindex].rlerp);
		}
		Matrix4x4_Normalize3(&bonematrix, &bonematrix);
		Matrix4x4_Interpolate(&skeleton->relativetransforms[bonenum], &bonematrix, &skeleton->relativetransforms[bonenum], retainfrac);
	}
	PRVM_G_FLOAT(OFS_RETURN) = skeletonindex + 1;
}

// #265 float(float skel) skel_get_numbones = #265; // (FTE_CSQC_SKELETONOBJECTS) returns how many bones exist in the created skeleton
static void VM_SV_skel_get_numbones(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	skeleton_t *skeleton;
	PRVM_G_FLOAT(OFS_RETURN) = 0;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	PRVM_G_FLOAT(OFS_RETURN) = skeleton->model->num_bones;
}

// #266 string(float skel, float bonenum) skel_get_bonename = #266; // (FTE_CSQC_SKELETONOBJECTS) returns name of bone (as a tempstring)
static void VM_SV_skel_get_bonename(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int bonenum = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	skeleton_t *skeleton;
	PRVM_G_INT(OFS_RETURN) = 0;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	if (bonenum < 0 || bonenum >= skeleton->model->num_bones)
		return;
	PRVM_G_INT(OFS_RETURN) = PRVM_SetTempString(prog, skeleton->model->data_bones[bonenum].name);
}

// #267 float(float skel, float bonenum) skel_get_boneparent = #267; // (FTE_CSQC_SKELETONOBJECTS) returns parent num for supplied bonenum, 0 if bonenum has no parent or bone does not exist (returned value is always less than bonenum, you can loop on this)
static void VM_SV_skel_get_boneparent(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int bonenum = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	skeleton_t *skeleton;
	PRVM_G_FLOAT(OFS_RETURN) = 0;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	if (bonenum < 0 || bonenum >= skeleton->model->num_bones)
		return;
	PRVM_G_FLOAT(OFS_RETURN) = skeleton->model->data_bones[bonenum].parent + 1;
}

// #268 float(float skel, string tagname) skel_find_bone = #268; // (FTE_CSQC_SKELETONOBJECTS) get number of bone with specified name, 0 on failure, tagindex (bonenum+1) on success, same as using gettagindex on the modelindex
static void VM_SV_skel_find_bone(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	const char *tagname = PRVM_G_STRING(OFS_PARM1);
	skeleton_t *skeleton;
	PRVM_G_FLOAT(OFS_RETURN) = 0;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	PRVM_G_FLOAT(OFS_RETURN) = Mod_Alias_GetTagIndexForName(skeleton->model, 0, tagname) + 1;
}

// #269 vector(float skel, float bonenum) skel_get_bonerel = #269; // (FTE_CSQC_SKELETONOBJECTS) get matrix of bone in skeleton relative to its parent - sets v_forward, v_right, v_up, returns origin (relative to parent bone)
static void VM_SV_skel_get_bonerel(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int bonenum = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	skeleton_t *skeleton;
	matrix4x4_t matrix;
	vec3_t forward, left, up, origin;
	VectorClear(PRVM_G_VECTOR(OFS_RETURN));
	VectorClear(PRVM_clientglobalvector(v_forward));
	VectorClear(PRVM_clientglobalvector(v_right));
	VectorClear(PRVM_clientglobalvector(v_up));
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	if (bonenum < 0 || bonenum >= skeleton->model->num_bones)
		return;
	matrix = skeleton->relativetransforms[bonenum];
	Matrix4x4_ToVectors(&matrix, forward, left, up, origin);
	VectorCopy(forward, PRVM_clientglobalvector(v_forward));
	VectorNegate(left, PRVM_clientglobalvector(v_right));
	VectorCopy(up, PRVM_clientglobalvector(v_up));
	VectorCopy(origin, PRVM_G_VECTOR(OFS_RETURN));
}

// #270 vector(float skel, float bonenum) skel_get_boneabs = #270; // (FTE_CSQC_SKELETONOBJECTS) get matrix of bone in skeleton in model space - sets v_forward, v_right, v_up, returns origin (relative to entity)
static void VM_SV_skel_get_boneabs(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int bonenum = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	skeleton_t *skeleton;
	matrix4x4_t matrix;
	matrix4x4_t temp;
	vec3_t forward, left, up, origin;
	VectorClear(PRVM_G_VECTOR(OFS_RETURN));
	VectorClear(PRVM_clientglobalvector(v_forward));
	VectorClear(PRVM_clientglobalvector(v_right));
	VectorClear(PRVM_clientglobalvector(v_up));
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	if (bonenum < 0 || bonenum >= skeleton->model->num_bones)
		return;
	matrix = skeleton->relativetransforms[bonenum];
	// convert to absolute
	while ((bonenum = skeleton->model->data_bones[bonenum].parent) >= 0)
	{
		temp = matrix;
		Matrix4x4_Concat(&matrix, &skeleton->relativetransforms[bonenum], &temp);
	}
	Matrix4x4_ToVectors(&matrix, forward, left, up, origin);
	VectorCopy(forward, PRVM_clientglobalvector(v_forward));
	VectorNegate(left, PRVM_clientglobalvector(v_right));
	VectorCopy(up, PRVM_clientglobalvector(v_up));
	VectorCopy(origin, PRVM_G_VECTOR(OFS_RETURN));
}

// #271 void(float skel, float bonenum, vector org) skel_set_bone = #271; // (FTE_CSQC_SKELETONOBJECTS) set matrix of bone relative to its parent, reads v_forward, v_right, v_up, takes origin as parameter (relative to parent bone)
static void VM_SV_skel_set_bone(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int bonenum = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	vec3_t forward, left, up, origin;
	skeleton_t *skeleton;
	matrix4x4_t matrix;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	if (bonenum < 0 || bonenum >= skeleton->model->num_bones)
		return;
	VectorCopy(PRVM_clientglobalvector(v_forward), forward);
	VectorNegate(PRVM_clientglobalvector(v_right), left);
	VectorCopy(PRVM_clientglobalvector(v_up), up);
	VectorCopy(PRVM_G_VECTOR(OFS_PARM2), origin);
	Matrix4x4_FromVectors(&matrix, forward, left, up, origin);
	skeleton->relativetransforms[bonenum] = matrix;
}

// #272 void(float skel, float bonenum, vector org) skel_mul_bone = #272; // (FTE_CSQC_SKELETONOBJECTS) transform bone matrix (relative to its parent) by the supplied matrix in v_forward, v_right, v_up, takes origin as parameter (relative to parent bone)
static void VM_SV_skel_mul_bone(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int bonenum = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	vec3_t forward, left, up, origin;
	skeleton_t *skeleton;
	matrix4x4_t matrix;
	matrix4x4_t temp;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	if (bonenum < 0 || bonenum >= skeleton->model->num_bones)
		return;
	VectorCopy(PRVM_G_VECTOR(OFS_PARM2), origin);
	VectorCopy(PRVM_clientglobalvector(v_forward), forward);
	VectorNegate(PRVM_clientglobalvector(v_right), left);
	VectorCopy(PRVM_clientglobalvector(v_up), up);
	Matrix4x4_FromVectors(&matrix, forward, left, up, origin);
	temp = skeleton->relativetransforms[bonenum];
	Matrix4x4_Concat(&skeleton->relativetransforms[bonenum], &matrix, &temp);
}

// #273 void(float skel, float startbone, float endbone, vector org) skel_mul_bones = #273; // (FTE_CSQC_SKELETONOBJECTS) transform bone matrices (relative to their parents) by the supplied matrix in v_forward, v_right, v_up, takes origin as parameter (relative to parent bones)
static void VM_SV_skel_mul_bones(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int firstbone = PRVM_G_FLOAT(OFS_PARM1) - 1;
	int lastbone = PRVM_G_FLOAT(OFS_PARM2) - 1;
	int bonenum;
	vec3_t forward, left, up, origin;
	skeleton_t *skeleton;
	matrix4x4_t matrix;
	matrix4x4_t temp;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	VectorCopy(PRVM_G_VECTOR(OFS_PARM3), origin);
	VectorCopy(PRVM_clientglobalvector(v_forward), forward);
	VectorNegate(PRVM_clientglobalvector(v_right), left);
	VectorCopy(PRVM_clientglobalvector(v_up), up);
	Matrix4x4_FromVectors(&matrix, forward, left, up, origin);
	firstbone = max(0, firstbone);
	lastbone = min(lastbone, skeleton->model->num_bones - 1);
	for (bonenum = firstbone;bonenum <= lastbone;bonenum++)
	{
		temp = skeleton->relativetransforms[bonenum];
		Matrix4x4_Concat(&skeleton->relativetransforms[bonenum], &matrix, &temp);
	}
}

// #274 void(float skeldst, float skelsrc, float startbone, float endbone) skel_copybones = #274; // (FTE_CSQC_SKELETONOBJECTS) copy bone matrices (relative to their parents) from one skeleton to another, useful for copying a skeleton to a corpse
static void VM_SV_skel_copybones(prvm_prog_t *prog)
{
	int skeletonindexdst = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	int skeletonindexsrc = (int)PRVM_G_FLOAT(OFS_PARM1) - 1;
	int firstbone = PRVM_G_FLOAT(OFS_PARM2) - 1;
	int lastbone = PRVM_G_FLOAT(OFS_PARM3) - 1;
	int bonenum;
	skeleton_t *skeletondst;
	skeleton_t *skeletonsrc;
	if (skeletonindexdst < 0 || skeletonindexdst >= MAX_EDICTS_32768 || !(skeletondst = prog->skeletons[skeletonindexdst]))
		return;
	if (skeletonindexsrc < 0 || skeletonindexsrc >= MAX_EDICTS_32768 || !(skeletonsrc = prog->skeletons[skeletonindexsrc]))
		return;
	firstbone = max(0, firstbone);
	lastbone = min(lastbone, skeletondst->model->num_bones - 1);
	lastbone = min(lastbone, skeletonsrc->model->num_bones - 1);
	for (bonenum = firstbone;bonenum <= lastbone;bonenum++)
		skeletondst->relativetransforms[bonenum] = skeletonsrc->relativetransforms[bonenum];
}

// #275 void(float skel) skel_delete = #275; // (FTE_CSQC_SKELETONOBJECTS) deletes skeleton at the beginning of the next frame (you can add the entity, delete the skeleton, renderscene, and it will still work)
static void VM_SV_skel_delete(prvm_prog_t *prog)
{
	int skeletonindex = (int)PRVM_G_FLOAT(OFS_PARM0) - 1;
	skeleton_t *skeleton;
	if (skeletonindex < 0 || skeletonindex >= MAX_EDICTS_32768 || !(skeleton = prog->skeletons[skeletonindex]))
		return;
	Mem_Free(skeleton);
	prog->skeletons[skeletonindex] = NULL;
}

// #276 float(float modlindex, string framename) frameforname = #276; // (FTE_CSQC_SKELETONOBJECTS) finds number of a specified frame in the animation, returns -1 if no match found
static void VM_SV_frameforname(prvm_prog_t *prog)
{
	int modelindex = (int)PRVM_G_FLOAT(OFS_PARM0);
	model_t *model = SV_GetModelByIndex(modelindex);
	const char *name = PRVM_G_STRING(OFS_PARM1);
	int i;
	PRVM_G_FLOAT(OFS_RETURN) = -1;
	if (!model || !model->animscenes)
		return;
	for (i = 0;i < model->numframes;i++)
	{
		if (String_Match_Caseless(model->animscenes[i].name, name))
		{
			PRVM_G_FLOAT(OFS_RETURN) = i;
			break;
		}
	}
}

// #277 float(float modlindex, float framenum) frameduration = #277; // (FTE_CSQC_SKELETONOBJECTS) returns the intended play time (in seconds) of the specified framegroup, if it does not exist the result is 0, if it is a single frame it may be a small value around 0.1 or 0.
static void VM_SV_frameduration(prvm_prog_t *prog)
{
	int modelindex = (int)PRVM_G_FLOAT(OFS_PARM0);
	model_t *model = SV_GetModelByIndex(modelindex);
	int framenum = (int)PRVM_G_FLOAT(OFS_PARM1);
	PRVM_G_FLOAT(OFS_RETURN) = 0;
	if (!model || !model->animscenes || framenum < 0 || framenum >= model->numframes)
		return;
	if (model->animscenes[framenum].framerate)
		PRVM_G_FLOAT(OFS_RETURN) = model->animscenes[framenum].framecount / model->animscenes[framenum].framerate;
}


prvm_builtin_t vm_sv_builtins[] = {
	#define DEF(func_name, nothing, func_num) func_name, // Baker: Inclusion macro
	#include "svvm_cmds_table.h" // Will #undef DEF
};


void Server_Builtins_Dump (cmd_state_t *cmd)
{
	typedef struct {
		ccs *fn_name;
		int builtin_num;
	} keypair_t;

	keypair_t pairs[] = {
		#define DEF(function, nothing, func_num) { STRINGIFY(function), func_num}, // Baker: Inclusion macro
		#include "svvm_cmds_table.h" // Will #undef DEF
	};
	int count = ARRAY_COUNT (pairs);
	ccs *contains_string = cmd_argv_parm2;

	if (contains_string[0]) {
		Con_PrintLinef ("Filtering to " QUOTED_S, cmd_argv_parm2);
	}

	int num_matches = 0;
	for (int n = 0; n < count; n ++) {
		keypair_t *sxy = &pairs[n];
		if (n != sxy->builtin_num) {
			Con_PrintLinef (CON_RED "Error builtin %d should be %d for %s", n, sxy->builtin_num, sxy->fn_name);
		}
		ccs *name = sxy->fn_name;
		if (name[0] == '0')
			continue; // It is NULL

		if (contains_string[0] && String_Contains (sxy->fn_name, contains_string) == false)
			continue; // Wants ones containing this and we don't have it.

		num_matches ++;
		Con_PrintLinef ("%4d: " S_FMT_LEFT_PAD_40 " # %d", n, sxy->fn_name, sxy->builtin_num);
	} // for

	Con_PrintVarInt (num_matches);
}


const int vm_sv_numbuiltins = sizeof(vm_sv_builtins) / sizeof(prvm_builtin_t);

void SVVM_init_cmd(prvm_prog_t *prog)
{
	VM_Cmd_Init(prog);
}

void SVVM_reset_cmd(prvm_prog_t *prog)
{
	World_End(&sv.world);

	if (prog->loaded && PRVM_serverfunction(SV_Shutdown))
	{
		func_t s = PRVM_serverfunction(SV_Shutdown);
		PRVM_serverglobalfloat(time) = sv.time;
		PRVM_serverfunction(SV_Shutdown) = 0; // prevent it from getting called again
		prog->ExecuteProgram(prog, s,"SV_Shutdown() required");
	}

	VM_Cmd_Reset(prog);
}
