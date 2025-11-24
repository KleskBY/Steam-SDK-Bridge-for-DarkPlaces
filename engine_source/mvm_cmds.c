#include "quakedef.h"

#include "prvm_cmds.h"
#include "clvm_cmds.h"
#include "menu.h"
#include "csprogs.h"


// TODO check which strings really should be engine strings

//============================================================================
// Menu

const char *vm_m_extensions[] = {
"BX_WAL_SUPPORT",
"DP_CINEMATIC_DPV",
"DP_COVERAGE",
"DP_CRYPTO",
"DP_CSQC_BINDMAPS",
"DP_GFX_FONTS",
"DP_GFX_FONTS_FREETYPE",
"DP_UTF8",
"DP_FONT_VARIABLEWIDTH",
"DP_MENU_EXTRESPONSEPACKET",
"DP_QC_ASINACOSATANATAN2TAN",
"DP_QC_AUTOCVARS",
"DP_QC_CMD",
"DP_QC_CRC16",
"DP_QC_CVAR_TYPE",
"DP_QC_CVAR_DESCRIPTION",
"DP_QC_DIGEST",
"DP_QC_DIGEST_SHA256",
"DP_QC_FINDCHAIN_TOFIELD",
"DP_QC_I18N",
"DP_QC_LOG",
"DP_QC_RENDER_SCENE",
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
"DP_QC_UNLIMITEDTEMPSTRINGS",
"DP_QC_URI_ESCAPE",
"DP_QC_URI_GET",
"DP_QC_URI_POST",
"DP_QC_WHICHPACK",
"FTE_STRINGS",
"DP_QC_FS_SEARCH_PACKFILE",
NULL
};

qbool MP_ConsoleCommand(const char *text)
{
	prvm_prog_t *prog = MVM_prog;
	return PRVM_ConsoleCommand(prog, text, &prog->funcoffsets.GameCommand, false, -1, 0, prog->loaded, "QC function GameCommand is missing");
}

/*
=========
VM_M_setmousetarget

setmousetarget(float target)
=========
*/
static void VM_M_setmousetarget(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_setmousetarget);

	switch((int)PRVM_G_FLOAT(OFS_PARM0))
	{
	case 1:
		in_client_mouse = false;
		break;
	case 2:
		in_client_mouse = true;
		break;
	default:
		prog->error_cmd("VM_M_setmousetarget: wrong destination %f !",PRVM_G_FLOAT(OFS_PARM0));
	}
}

/*
=========
VM_M_getmousetarget

float	getmousetarget
=========
*/
static void VM_M_getmousetarget(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(0,VM_M_getmousetarget);

	if (in_client_mouse)
		PRVM_G_FLOAT(OFS_RETURN) = 2;
	else
		PRVM_G_FLOAT(OFS_RETURN) = 1;
}



/*
=========
VM_M_setkeydest

setkeydest(float dest)
=========
*/
static void VM_M_setkeydest(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1,VM_M_setkeydest);

	switch((int)PRVM_G_FLOAT(OFS_PARM0))
	{
	case 0:
		// key_game
		KeyDest_Set (key_game); // key_dest = key_game;
		break;
	case 2:
		// key_menu
		KeyDest_Set (key_menu); // key_dest = key_menu;
		break;
	case 3:
		// key_menu_grabbed
		KeyDest_Set (key_menu_grabbed); // key_dest = key_menu_grabbed;
		break;
	case 1:
		// key_message
		// key_dest = key_message
		// break;
	default:
		prog->error_cmd("VM_M_setkeydest: wrong destination %f !", PRVM_G_FLOAT(OFS_PARM0));
	}
}

/*
=========
VM_M_getkeydest

float	getkeydest
=========
*/
static void VM_M_getkeydest(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(0,VM_M_getkeydest);

	// key_game = 0, key_message = 1, key_menu = 2, key_menu_grabbed = 3, unknown = -1
	switch(key_dest)
	{
	case key_game:
		PRVM_G_FLOAT(OFS_RETURN) = 0;
		break;
	case key_menu:
		PRVM_G_FLOAT(OFS_RETURN) = 2;
		break;
	case key_menu_grabbed:
		PRVM_G_FLOAT(OFS_RETURN) = 3;
		break;
	case key_message:
		// not supported
		// PRVM_G_FLOAT(OFS_RETURN) = 1;
		// break;
	default:
		PRVM_G_FLOAT(OFS_RETURN) = -1;
	}
}


/*
=========
VM_M_getresolution

vector	getresolution(float number)
=========
*/
static void VM_M_getresolution(prvm_prog_t *prog)
{
	int nr, fs;
	VM_SAFEPARMCOUNTRANGE(1, 2, VM_M_getresolution);

	nr = (int)PRVM_G_FLOAT(OFS_PARM0);

	fs = ((prog->argc <= 1) || ((int)PRVM_G_FLOAT(OFS_PARM1)));

	if (nr < -1 || nr >= (fs ? video_resolutions_count : video_resolutions_hardcoded_count))
	{
		PRVM_G_VECTOR(OFS_RETURN)[0] = 0;
		PRVM_G_VECTOR(OFS_RETURN)[1] = 0;
		PRVM_G_VECTOR(OFS_RETURN)[2] = 0;
	}
	else if (nr == -1)
	{
		vid_mode_t *m = VID_GetDesktopMode();
		if (m)
		{
			PRVM_G_VECTOR(OFS_RETURN)[0] = m->width;
			PRVM_G_VECTOR(OFS_RETURN)[1] = m->height;
			PRVM_G_VECTOR(OFS_RETURN)[2] = m->pixelheight_num / (prvm_vec_t) m->pixelheight_denom;
		}
		else
		{
			PRVM_G_VECTOR(OFS_RETURN)[0] = 0;
			PRVM_G_VECTOR(OFS_RETURN)[1] = 0;
			PRVM_G_VECTOR(OFS_RETURN)[2] = 0;
		}
	}
	else
	{
		video_resolution_t *r = &((fs ? video_resolutions : video_resolutions_hardcoded)[nr]);
		PRVM_G_VECTOR(OFS_RETURN)[0] = r->width;
		PRVM_G_VECTOR(OFS_RETURN)[1] = r->height;
		PRVM_G_VECTOR(OFS_RETURN)[2] = r->pixelheight;
	}
}

static void VM_M_getgamedirinfo(prvm_prog_t *prog)
{
	int nr, item;
	VM_SAFEPARMCOUNT(2, VM_getgamedirinfo);

	nr = (int)PRVM_G_FLOAT(OFS_PARM0);
	item = (int)PRVM_G_FLOAT(OFS_PARM1);

	PRVM_G_INT( OFS_RETURN ) = OFS_NULL;

	if (nr >= 0 && nr < fs_all_gamedirs_count) {
		if (item == 0)
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, fs_all_gamedirs[nr].name );
		else if (item == 1)
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, fs_all_gamedirs[nr].description );
	}
}

/*
=========
VM_M_getserverliststat

float	getserverliststat(float type)
=========
*/
/*
	type:
0	serverlist_viewlist_count
1   serverlist_totalcount
2	masterquerycount
3	masterreplycount
4	serverquerycount
5	serverreplycount
6	sortfield
7	sortflags
*/
static void VM_M_getserverliststat(prvm_prog_t *prog)
{
	int type;
	VM_SAFEPARMCOUNT ( 1, VM_M_getserverliststat );

	PRVM_G_FLOAT( OFS_RETURN ) = 0;

	type = (int)PRVM_G_FLOAT( OFS_PARM0 );
	switch(type)
	{
	case 0:
		PRVM_G_FLOAT ( OFS_RETURN ) = serverlist_viewlist_count;
		return;
	case 1:
		PRVM_G_FLOAT ( OFS_RETURN ) = serverlist_cachecount;
		return;
	case 2:
		PRVM_G_FLOAT ( OFS_RETURN ) = masterquerycount;
		return;
	case 3:
		PRVM_G_FLOAT ( OFS_RETURN ) = masterreplycount;
		return;
	case 4:
		PRVM_G_FLOAT ( OFS_RETURN ) = serverquerycount;
		return;
	case 5:
		PRVM_G_FLOAT ( OFS_RETURN ) = serverreplycount;
		return;
	case 6:
		PRVM_G_FLOAT ( OFS_RETURN ) = serverlist_sortbyfield;
		return;
	case 7:
		PRVM_G_FLOAT ( OFS_RETURN ) = serverlist_sortflags;
		return;
	default:
		VM_WarningLinef (prog, "VM_M_getserverliststat: bad type %d!", type );
	}
}

/*
========================
VM_M_resetserverlistmasks

resetserverlistmasks()
========================
*/
static void VM_M_resetserverlistmasks(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(0, VM_M_resetserverlistmasks);
	ServerList_ResetMasks();
}


/*
========================
VM_M_setserverlistmaskstring

setserverlistmaskstring(float mask, float fld, string str, float op)
0-511		and
512 - 1024	or
========================
*/
static void VM_M_setserverlistmaskstring(prvm_prog_t *prog)
{
	const char *str;
	int masknr;
	serverlist_mask_t *mask;
	int field;

	VM_SAFEPARMCOUNT( 4, VM_M_setserverlistmaskstring );
	str = PRVM_G_STRING( OFS_PARM2 );

	masknr = (int)PRVM_G_FLOAT( OFS_PARM0 );
	if ( masknr >= 0 && masknr < SERVERLIST_ANDMASKCOUNT )
		mask = &serverlist_andmasks[masknr];
	else if ( masknr >= 512 && masknr - 512 < SERVERLIST_ORMASKCOUNT )
		mask = &serverlist_ormasks[masknr - 512 ];
	else
	{
		VM_WarningLinef (prog, "VM_M_setserverlistmaskstring: invalid mask number %d", masknr );
		return;
	}

	field = (int) PRVM_G_FLOAT( OFS_PARM1 );

	switch( field ) {
		case SLIF_CNAME:
			strlcpy( mask->info.cname, str, sizeof(mask->info.cname) );
			break;
		case SLIF_NAME:
			strlcpy( mask->info.name, str, sizeof(mask->info.name)  );
			break;
		case SLIF_QCSTATUS:
			strlcpy( mask->info.qcstatus, str, sizeof(mask->info.qcstatus)  );
			break;
		case SLIF_PLAYERS:
			strlcpy( mask->info.players_data, str, sizeof(mask->info.players_data)  );
			break;
		case SLIF_MAP:
			strlcpy( mask->info.map, str, sizeof(mask->info.map)  );
			break;
		case SLIF_MOD:
			strlcpy( mask->info.mod, str, sizeof(mask->info.mod)  );
			break;
		case SLIF_GAME:
			strlcpy( mask->info.game, str, sizeof(mask->info.game)  );
			break;
		default:
			VM_WarningLinef (prog, "VM_M_setserverlistmaskstring: Bad field number %d passed!", field );
			return;
	}

	mask->active = true;
	mask->tests[field] = (serverlist_maskop_t)((int)PRVM_G_FLOAT( OFS_PARM3 ));
}

/*
========================
VM_M_setserverlistmasknumber

setserverlistmasknumber(float mask, float fld, float num, float op)

0-511		and
512 - 1024	or
========================
*/
static void VM_M_setserverlistmasknumber(prvm_prog_t *prog)
{
	int number;
	serverlist_mask_t *mask;
	int	masknr;
	int field;
	VM_SAFEPARMCOUNT( 4, VM_M_setserverlistmasknumber );

	masknr = (int)PRVM_G_FLOAT( OFS_PARM0 );
	if ( masknr >= 0 && masknr < SERVERLIST_ANDMASKCOUNT )
		mask = &serverlist_andmasks[masknr];
	else if ( masknr >= 512 && masknr - 512 < SERVERLIST_ORMASKCOUNT )
		mask = &serverlist_ormasks[masknr - 512 ];
	else
	{
		VM_WarningLinef (prog, "VM_M_setserverlistmasknumber: invalid mask number %d", masknr );
		return;
	}

	number = (int)PRVM_G_FLOAT( OFS_PARM2 );
	field = (int) PRVM_G_FLOAT( OFS_PARM1 );

	switch( field ) {
		case SLIF_MAXPLAYERS:
			mask->info.maxplayers = number;
			break;
		case SLIF_NUMPLAYERS:
			mask->info.numplayers = number;
			break;
		case SLIF_NUMBOTS:
			mask->info.numbots = number;
			break;
		case SLIF_NUMHUMANS:
			mask->info.numhumans = number;
			break;
		case SLIF_PING:
			mask->info.ping = number;
			break;
		case SLIF_PROTOCOL:
			mask->info.protocol = number;
			break;
		case SLIF_FREESLOTS:
			mask->info.freeslots = number;
			break;
		case SLIF_CATEGORY:
			mask->info.category = number;
			break;
		case SLIF_ISFAVORITE:
			mask->info.isfavorite = number != 0;
			break;
		default:
			VM_WarningLinef (prog, "VM_M_setserverlistmasknumber: Bad field number %d passed!", field );
			return;
	}

	mask->active = true;
	mask->tests[field] = (serverlist_maskop_t)((int)PRVM_G_FLOAT( OFS_PARM3 ));
}


/*
========================
VM_M_resortserverlist

resortserverlist
========================
*/
static void VM_M_resortserverlist(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(0, VM_M_resortserverlist);
	ServerList_RebuildViewList();
}

/*
=========
VM_M_getserverliststring

string	getserverliststring(float field, float hostnr)
=========
*/
static void VM_M_getserverliststring(prvm_prog_t *prog)
{
	const serverlist_entry_t *cache;
	int hostnr;

	VM_SAFEPARMCOUNT(2, VM_M_getserverliststring);

	PRVM_G_INT(OFS_RETURN) = OFS_NULL;

	hostnr = (int)PRVM_G_FLOAT(OFS_PARM1);

	if (hostnr == -1 && serverlist_callbackentry)
	{
		cache = serverlist_callbackentry;
	}
	else
	{
		if (hostnr < 0 || hostnr >= serverlist_viewlist_count)
		{
			Con_Print("VM_M_getserverliststring: bad hostnr passed!\n");
			return;
		}
		cache = ServerList_GetViewEntry(hostnr);
	}
	switch( (int) PRVM_G_FLOAT(OFS_PARM0) ) {
		case SLIF_CNAME:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.cname );
			break;
		case SLIF_NAME:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.name );
			break;
		case SLIF_QCSTATUS:
			PRVM_G_INT (OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.qcstatus );
			break;
		case SLIF_PLAYERS:
			PRVM_G_INT (OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.players_data );
			break;
		case SLIF_GAME:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.game );
			break;
		case SLIF_MOD:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.mod );
			break;
		case SLIF_MAP:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->info.map );
			break;
		// TODO remove this again
		case 1024:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->line1 );
			break;
		case 1025:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, cache->line2 );
			break;
		default:
			Con_Print("VM_M_getserverliststring: bad field number passed!\n");
	}
}

/*
=========
VM_M_getserverlistnumber

float	getserverlistnumber(float field, float hostnr)
=========
*/
static void VM_M_getserverlistnumber(prvm_prog_t *prog)
{
	const serverlist_entry_t *cache;
	int hostnr;

	VM_SAFEPARMCOUNT(2, VM_M_getserverlistnumber);

	PRVM_G_INT(OFS_RETURN) = OFS_NULL;

	hostnr = (int)PRVM_G_FLOAT(OFS_PARM1);

	if (hostnr == -1 && serverlist_callbackentry)
	{
		cache = serverlist_callbackentry;
	}
	else
	{
		if (hostnr < 0 || hostnr >= serverlist_viewlist_count)
		{
			Con_Print("VM_M_getserverliststring: bad hostnr passed!\n");
			return;
		}
		cache = ServerList_GetViewEntry(hostnr);
	}
	switch( (int) PRVM_G_FLOAT(OFS_PARM0) ) {
		case SLIF_MAXPLAYERS:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.maxplayers;
			break;
		case SLIF_NUMPLAYERS:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.numplayers;
			break;
		case SLIF_NUMBOTS:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.numbots;
			break;
		case SLIF_NUMHUMANS:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.numhumans;
			break;
		case SLIF_FREESLOTS:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.freeslots;
			break;
		case SLIF_PING:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.ping;
			break;
		case SLIF_PROTOCOL:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.protocol;
			break;
		case SLIF_CATEGORY:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.category;
			break;
		case SLIF_ISFAVORITE:
			PRVM_G_FLOAT( OFS_RETURN ) = cache->info.isfavorite;
			break;
		default:
			Con_Print("VM_M_getserverlistnumber: bad field number passed!\n");
	}
}

/*
========================
VM_M_setserverlistsort

setserverlistsort(float field, float flags)
========================
*/
static void VM_M_setserverlistsort(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT( 2, VM_M_setserverlistsort );

	serverlist_sortbyfield = (serverlist_infofield_t)((int)PRVM_G_FLOAT( OFS_PARM0 ));
	serverlist_sortflags = (int) PRVM_G_FLOAT( OFS_PARM1 );
}

/*
========================
VM_M_refreshserverlist

refreshserverlist()
========================
*/
static void VM_M_refreshserverlist(prvm_prog_t *prog)
{
	qbool do_reset = false;
	VM_SAFEPARMCOUNTRANGE( 0, 1, VM_M_refreshserverlist );
	if (prog->argc >= 1 && PRVM_G_FLOAT(OFS_PARM0))
		do_reset = true;
	ServerList_QueryList(do_reset, true, false, false);
}

/*
========================
VM_M_getserverlistindexforkey

float getserverlistindexforkey(string key)
========================
*/
static void VM_M_getserverlistindexforkey(prvm_prog_t *prog)
{
	const char *key;
	VM_SAFEPARMCOUNT( 1, VM_M_getserverlistindexforkey );

	key = PRVM_G_STRING( OFS_PARM0 );
	VM_CheckEmptyString( prog, key );

	if ( String_Match( key, "cname" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_CNAME;
	else if ( String_Match( key, "ping" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_PING;
	else if ( String_Match( key, "game" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_GAME;
	else if ( String_Match( key, "mod" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_MOD;
	else if ( String_Match( key, "map" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_MAP;
	else if ( String_Match( key, "name" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_NAME;
	else if ( String_Match( key, "qcstatus" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_QCSTATUS;
	else if ( String_Match( key, "players" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_PLAYERS;
	else if ( String_Match( key, "maxplayers" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_MAXPLAYERS;
	else if ( String_Match( key, "numplayers" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_NUMPLAYERS;
	else if ( String_Match( key, "numbots" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_NUMBOTS;
	else if ( String_Match( key, "numhumans" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_NUMHUMANS;
	else if ( String_Match( key, "freeslots" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_FREESLOTS;
	else if ( String_Match( key, "protocol" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_PROTOCOL;
	else if ( String_Match( key, "category" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_CATEGORY;
	else if ( String_Match( key, "isfavorite" ) )
		PRVM_G_FLOAT( OFS_RETURN ) = SLIF_ISFAVORITE;
	else
		PRVM_G_FLOAT( OFS_RETURN ) = -1;
}

/*
========================
VM_M_addwantedserverlistkey

addwantedserverlistkey(string key)
========================
*/
static void VM_M_addwantedserverlistkey(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT( 1, VM_M_addwantedserverlistkey );
}

/*
===============================================================================
MESSAGE WRITING

used only for client and menu
server uses VM_SV_...

Write*(* data, float type, float to)

===============================================================================
*/

#define	MSG_BROADCAST	0		// unreliable to all
#define	MSG_ONE			1		// reliable to one (msg_entity)
#define	MSG_ALL			2		// reliable to all
#define	MSG_INIT		3		// write to the init string

static sizebuf_t *VM_M_WriteDest (prvm_prog_t *prog)
{
	int		dest;
	int		destclient;

	if (!sv.active)
		prog->error_cmd("VM_M_WriteDest: game is not server (%s)", prog->name);

	dest = (int)PRVM_G_FLOAT(OFS_PARM1);
	switch (dest)
	{
	case MSG_BROADCAST:
		return &sv.datagram;

	case MSG_ONE:
		destclient = (int) PRVM_G_FLOAT(OFS_PARM2);
		if (destclient < 0 || destclient >= svs.maxclients || !svs.clients[destclient].active || !svs.clients[destclient].netconnection)
			prog->error_cmd("VM_clientcommand: %s: invalid client !", prog->name);

		return &svs.clients[destclient].netconnection->message;

	case MSG_ALL:
		return &sv.reliable_datagram;

	case MSG_INIT:
		return &sv.signon;

	default:
		prog->error_cmd("WriteDest: bad destination");
		break;
	}

	return NULL;
}

static void VM_M_WriteByte (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteByte);
	MSG_WriteByte (VM_M_WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM0));
}

static void VM_M_WriteChar (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteChar);
	MSG_WriteChar (VM_M_WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM0));
}

static void VM_M_WriteShort (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteShort);
	MSG_WriteShort (VM_M_WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM0));
}

static void VM_M_WriteLong (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteLong);
	MSG_WriteLong (VM_M_WriteDest(prog), (int)PRVM_G_FLOAT(OFS_PARM0));
}

static void VM_M_WriteAngle (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteAngle);
	MSG_WriteAngle (VM_M_WriteDest(prog), PRVM_G_FLOAT(OFS_PARM0), sv.protocol);
}

static void VM_M_WriteCoord (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteCoord);
	MSG_WriteCoord (VM_M_WriteDest(prog), PRVM_G_FLOAT(OFS_PARM0), sv.protocol);
}

static void VM_M_WriteString (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteString);
	MSG_WriteString (VM_M_WriteDest(prog), PRVM_G_STRING(OFS_PARM0));
}

static void VM_M_WriteEntity (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_WriteEntity);
	MSG_WriteShort (VM_M_WriteDest(prog), PRVM_G_EDICTNUM(OFS_PARM0));
}

/*
=================
VM_M_copyentity

copies data from one entity to another

copyentity(entity src, entity dst)
=================
*/
static void VM_M_copyentity (prvm_prog_t *prog)
{
	prvm_edict_t *in, *out;
	VM_SAFEPARMCOUNT(2,VM_M_copyentity);
	in = PRVM_G_EDICT(OFS_PARM0);
	out = PRVM_G_EDICT(OFS_PARM1);
	memcpy(out->fields.fp, in->fields.fp, prog->entityfields * sizeof(prvm_vec_t));
}

//#66 vector() getmousepos (EXT_CSQC)
static void VM_M_getmousepos(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(0,VM_M_getmousepos);

	if (key_consoleactive || (key_dest != key_menu && key_dest != key_menu_grabbed))
		VectorSet(PRVM_G_VECTOR(OFS_RETURN), 0, 0, 0);
	else if (in_client_mouse)
		VectorSet(PRVM_G_VECTOR(OFS_RETURN), in_windowmouse_x * vid_conwidth.integer / vid.width, in_windowmouse_y * vid_conheight.integer / vid.height, 0);
	else
		VectorSet(PRVM_G_VECTOR(OFS_RETURN), in_mouse_x * vid_conwidth.integer / vid.width, in_mouse_y * vid_conheight.integer / vid.height, 0);
}

static void VM_M_crypto_getkeyfp(prvm_prog_t *prog)
{
	lhnetaddress_t addr;
	const char *s;
	char keyfp[FP64_SIZE + 1];

	VM_SAFEPARMCOUNT(1,VM_M_crypto_getkeyfp);

	s = PRVM_G_STRING( OFS_PARM0 );
	VM_CheckEmptyString( prog, s );

	if (LHNETADDRESS_FromString(&addr, s, 26000) && Crypto_RetrieveHostKey(&addr, NULL, keyfp, sizeof(keyfp), NULL, 0, NULL, NULL))
		PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, keyfp );
	else
		PRVM_G_INT( OFS_RETURN ) = OFS_NULL;
}
static void VM_M_crypto_getidfp(prvm_prog_t *prog)
{
	lhnetaddress_t addr;
	const char *s;
	char idfp[FP64_SIZE + 1];

	VM_SAFEPARMCOUNT(1,VM_M_crypto_getidfp);

	s = PRVM_G_STRING( OFS_PARM0 );
	VM_CheckEmptyString( prog, s );

	if (LHNETADDRESS_FromString(&addr, s, 26000) && Crypto_RetrieveHostKey(&addr, NULL, NULL, 0, idfp, sizeof(idfp), NULL, NULL))
		PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString( prog, idfp );
	else
		PRVM_G_INT( OFS_RETURN ) = OFS_NULL;
}
static void VM_M_crypto_getidstatus(prvm_prog_t *prog)
{
	lhnetaddress_t addr;
	const char *s;
	qbool issigned;

	VM_SAFEPARMCOUNT(1,VM_M_crypto_getidstatus);

	s = PRVM_G_STRING( OFS_PARM0 );
	VM_CheckEmptyString( prog, s );

	if (LHNETADDRESS_FromString(&addr, s, 26000) && Crypto_RetrieveHostKey(&addr, NULL, NULL, 0, NULL, 0, NULL, &issigned))
		PRVM_G_FLOAT( OFS_RETURN ) = issigned ? 2 : 1;
	else
		PRVM_G_FLOAT( OFS_RETURN ) = 0;
}
static void VM_M_crypto_getencryptlevel(prvm_prog_t *prog)
{
	lhnetaddress_t addr;
	const char *s;
	int aeslevel;
	char vabuf[1024];

	VM_SAFEPARMCOUNT(1,VM_M_crypto_getencryptlevel);

	s = PRVM_G_STRING( OFS_PARM0 );
	VM_CheckEmptyString( prog, s );

	if (LHNETADDRESS_FromString(&addr, s, 26000) && Crypto_RetrieveHostKey(&addr, NULL, NULL, 0, NULL, 0, &aeslevel, NULL))
		PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString(prog, aeslevel ? va(vabuf, sizeof(vabuf), "%d AES128", aeslevel) : "0");
	else
		PRVM_G_INT( OFS_RETURN ) = OFS_NULL;
}
static void VM_M_crypto_getmykeyfp(prvm_prog_t *prog)
{
	int i;
	char keyfp[FP64_SIZE + 1];

	VM_SAFEPARMCOUNT(1, VM_M_crypto_getmykeyfp);

	i = PRVM_G_FLOAT( OFS_PARM0 );
	switch(Crypto_RetrieveLocalKey(i, keyfp, sizeof(keyfp), NULL, 0, NULL))
	{
		case -1:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString(prog, "");
			break;
		case 0:
			PRVM_G_INT( OFS_RETURN ) = OFS_NULL;
			break;
		default:
		case 1:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString(prog, keyfp);
			break;
	}
}
static void VM_M_crypto_getmyidfp(prvm_prog_t *prog)
{
	int i;
	char idfp[FP64_SIZE + 1];

	VM_SAFEPARMCOUNT(1, VM_M_crypto_getmyidfp);

	i = PRVM_G_FLOAT( OFS_PARM0 );
	switch(Crypto_RetrieveLocalKey(i, NULL, 0, idfp, sizeof(idfp), NULL))
	{
		case -1:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString(prog, "");
			break;
		case 0:
			PRVM_G_INT( OFS_RETURN ) = OFS_NULL;
			break;
		default:
		case 1:
			PRVM_G_INT( OFS_RETURN ) = PRVM_SetTempString(prog, idfp);
			break;
	}
}
static void VM_M_crypto_getmyidstatus(prvm_prog_t *prog)
{
	int i;
	qbool issigned;

	VM_SAFEPARMCOUNT(1, VM_M_crypto_getmyidstatus);

	i = PRVM_G_FLOAT( OFS_PARM0 );
	switch(Crypto_RetrieveLocalKey(i, NULL, 0, NULL, 0, &issigned))
	{
		case -1:
			PRVM_G_FLOAT( OFS_RETURN ) = 0; // have no ID there
			break;
		case 0:
			PRVM_G_FLOAT( OFS_RETURN ) = -1; // out of range
			break;
		default:
		case 1:
			PRVM_G_FLOAT( OFS_RETURN ) = issigned ? 2 : 1;
			break;
	}
}

// CL_Video interface functions

/*
========================
VM_cin_open

float cin_open(string file, string name)
========================
*/
void VM_cin_open(prvm_prog_t *prog)
{
	const char *file;
	const char *name;

	VM_SAFEPARMCOUNT(2, VM_cin_open);

	file = PRVM_G_STRING(OFS_PARM0);
	name = PRVM_G_STRING(OFS_PARM1);

	VM_CheckEmptyString(prog, file);
    VM_CheckEmptyString(prog, name);

	if (CL_Cin_OpenVideo(file, name, VID_OWNER_MENU_1, "") ) {
		PRVM_G_FLOAT(OFS_RETURN) = 1;
		cl_videoplaying = VIDEO_PLAYING_MENU_2;
	}
	else
		PRVM_G_FLOAT(OFS_RETURN) = 0;
}

/*
========================
VM_cin_close

void cin_close(string name)
========================
*/
void VM_cin_close(prvm_prog_t *prog)
{
	const char *name;

	VM_SAFEPARMCOUNT( 1, VM_cin_close );

	name = PRVM_G_STRING(OFS_PARM0);
	VM_CheckEmptyString(prog, name);

	CL_CloseVideo (CL_GetVideoByName(name));
#if 1
	cl_videoplaying = VIDEO_NOT_PLAYING_0;
#endif
}

/*
========================
VM_cin_setstate
void cin_setstate(string name, float type)
========================
*/
void VM_cin_setstate (prvm_prog_t *prog)
{
	const char *name;
	clvideostate_t 	state;
	clvideo_t		*video;

	VM_SAFEPARMCOUNT (2, VM_cin_setstate);

	name = PRVM_G_STRING( OFS_PARM0 );
	VM_CheckEmptyString(prog,  name );

	state = (clvideostate_t)((int)PRVM_G_FLOAT(OFS_PARM1));

	video = CL_GetVideoByName(name);
	if ( video && state > CLVIDEO_UNUSED_0 && state < CLVIDEO_STATECOUNT_6)
		CL_SetVideoState (video, state );

#if 1
	cl_videoplaying = VIDEO_PLAYING_MENU_2;
#endif
}

/*
========================
VM_cin_getstate

float cin_getstate(string name)
========================
*/
void VM_cin_getstate(prvm_prog_t *prog)
{
	const char *name;
	clvideo_t		*video;

	VM_SAFEPARMCOUNT( 1, VM_cin_getstate );

	name = PRVM_G_STRING(OFS_PARM0);
	VM_CheckEmptyString(prog, name);

	video = CL_GetVideoByName(name);
	if ( video )
		PRVM_G_FLOAT(OFS_RETURN) = (int)video->state;
	else
		PRVM_G_FLOAT(OFS_RETURN) = 0;
}

/*
========================
VM_cin_restart

void cin_restart(string name)
========================
*/
void VM_cin_restart(prvm_prog_t *prog)
{
	const char *name;
	clvideo_t		*video;

	VM_SAFEPARMCOUNT(1, VM_cin_restart);

	name = PRVM_G_STRING(OFS_PARM0);
	VM_CheckEmptyString(prog, name);

	video = CL_GetVideoByName(name);
	if (video)
		CL_RestartVideo(video);
#if 1
	cl_videoplaying = VIDEO_PLAYING_MENU_2;
#endif
}

static void VM_M_registercommand(prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNT(1, VM_M_registercommand);
	Cmd_AddCommand(CF_CLIENT, PRVM_G_STRING(OFS_PARM0), NULL, "console command created by QuakeC");
}

prvm_builtin_t vm_m_builtins[] = {
	#define DEF(func_name, nothing, func_num) func_name, // Baker: Inclusion macro
	#include "mvm_cmds_table.h" // Will #undef DEF
};

void Menu_Builtins_Dump (cmd_state_t *cmd)
{
	typedef struct {
		ccs *fn_name;
		int builtin_num;
	} keypair_t;

	keypair_t pairs[] = {
		#define DEF(function, nothing, func_num) { STRINGIFY(function), func_num}, // Baker: Inclusion macro
		#include "mvm_cmds_table.h" // Will #undef DEF
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


const int vm_m_numbuiltins = sizeof(vm_m_builtins) / sizeof(prvm_builtin_t);

void MVM_init_cmd(prvm_prog_t *prog)
{
	r_refdef_scene_t *scene;

	VM_Cmd_Init(prog);
	prog->polygonbegin_model = NULL;
	prog->polygonbegin_guess2d = 0;

	scene = R_GetScenePointer( RST_MENU );

	memset (scene, 0, sizeof (*scene));

	scene->maxtempentities = 128;
	scene->tempentities = (entity_render_t*) Mem_Alloc(prog->progs_mempool, sizeof(entity_render_t) * scene->maxtempentities);

	scene->maxentities = MAX_EDICTS_32768 + 256 + 512;
	scene->entities = (entity_render_t **)Mem_Alloc(prog->progs_mempool, sizeof(entity_render_t *) * scene->maxentities);

	// LadyHavoc: what is this for?
	scene->ambientintensity = 32.0f;
}

void MVM_reset_cmd(prvm_prog_t *prog)
{
	// note: the menu's render entities are automatically freed when the prog's pool is freed

	//VM_Cmd_Init();
	VM_Cmd_Reset(prog);
	prog->polygonbegin_model = NULL;
	prog->polygonbegin_guess2d = 0;
}
