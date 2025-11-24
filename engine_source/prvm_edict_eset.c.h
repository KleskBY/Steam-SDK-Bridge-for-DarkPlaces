// prvm_edict_eset.c.h


RELATED_ (PRVM_ED_EdictSet_f PRVM_GlobalSet_f)
RELATED_ (SV_ShowModel_f)

void _InfoBar_Notice_NoConPrint (cvar_t *var, ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);

	Cbuf_AddTextLinef (cmd_local, "infobar 3 " QUOTED_S " silent", text);

	VA_EXPAND_ALLOC_FREE (text);
}

void _Cvar_Set_IsNotify_IsClipboard_String_Print_Fn_t (printline_fn_t printline_fn, cvar_t *var, qbool bshallnotify, qbool bshallclipboard, ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);

	Cvar_SetQuick (var, text);
	printline_fn ("%s = " QUOTED_S NEWLINE /*WALDO*/, var->name, var->string);

	if (bshallnotify)
		_InfoBar_Notice_NoConPrint (var, "%s = %s", var->name, var->string);

	if (bshallclipboard) {
		Clipboard_Set_Text (text);
		printline_fn ("%s: value set to clipboard", var->name);
	}

	VA_EXPAND_ALLOC_FREE (text);
}


void _Cvar_Set_IsNotify_IsClipboard_String (cvar_t *var, qbool bshallnotify, qbool bshallclipboard, qbool bshallprint, ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);

	Cvar_SetQuick (var, text);
	if (bshallprint)
		Con_PrintLinef ("%s = " QUOTED_S, var->name, var->string);

	if (bshallnotify)
		_InfoBar_Notice_NoConPrint (var, "%s = %s", var->name, var->string);

	if (bshallclipboard) {
		Clipboard_Set_Text (text);
		if (bshallprint)
			Con_PrintLinef ("%s: value set to clipboard", var->name);
	}

	VA_EXPAND_ALLOC_FREE (text);
}


// END VALUE / ENTNUM


static void _Any_PRVM_ED_Edict_Remove_f (cmd_state_t *cmd, ccs *s_progname)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return; // Bad program
	}

	if (cmd_argc < 2 ) {
		// No params - print information
		// ex: prvm_globalset
		Con_PrintLinef ( "%s: remove an edict", cmd_argv_command_0 );
		Con_PrintLinef ( "See also: ent_remove (server only)");
		return;
	}

	int edict_num = atoi(cmd_argv_parm1);

	if (in_range_beyond (/*world = 0*/ 1, edict_num, prog->num_edicts) == false) {
		Con_PrintLinef ("Bad edict number (1 to %d valid)", prog->num_edicts - 1);
		return; // Bad edict number
	}

	prvm_edict_t *ed = PRVM_EDICT_NUM(edict_num);

	if (PRVM_NUM_FOR_EDICT(ed) <= prog->reserved_edicts) {
		Con_PrintLinef ("%s: " "tried to remove the null entity or a reserved entity!", cmd_argv_command_0);
	}
	else if (ed->free) {
		Con_PrintLinef ("%s: " "tried to remove an already freed entity!", cmd_argv_command_0);
	}
	else {
		PRVM_ED_Free (prog, ed);
		Con_PrintLinef ("%s: Successfully removed # %d", cmd_argv_command_0, edict_num);
	}

}



static void _Any_PRVM_ED_Edict_Create_f (cmd_state_t *cmd, ccs *s_progname)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return; // Bad program
	}

	if (cmd_argc < 2 ) {
		// No params - print information
		// ex: prvm_globalset
		Con_PrintLinef ( "%s: create an edict", cmd_argv_command_0 );
		Con_PrintLinef ( "Type " CON_BRONZE "%s go" CON_WHITE " to create an entity", cmd_argv_command_0 );
		Con_PrintLinef ( "See also: ent_create (server only)");
		return;
	}

	//ccs *s_go		= cmd_argv_parm1;

	RELATED_ (VM_CL_spawn VM_remove)
	prvm_edict_t *ed = PRVM_ED_Alloc (prog); // Failure means host error

	int edictnum = PRVM_NUM_FOR_EDICT(ed);

	Con_PrintLinef ("Created edict #%d", edictnum);
	_Cvar_Set_IsNotify_IsClipboard_String (&_ent, q_notifyinfobar_true, q_clipboard_false, q_shall_print_true,
		"%d",
		edictnum
	);

}


void PRVM_ED_Edict_Remove_CSQC_f (cmd_state_t *cmd)
{
	_Any_PRVM_ED_Edict_Remove_f (cmd, "client");
}

void PRVM_ED_Edict_Create_CSQC_f (cmd_state_t *cmd)
{
	_Any_PRVM_ED_Edict_Create_f (cmd, "client");
}

//cl.entities[cl.playerentity].state_current.angles

//static void normalize_to_result(vec3_t value1, vec3_t result)
//{
//	// value1 = PRVM_G_VECTOR(OFS_PARM0);
//
//	double f = VectorLength2(value1);
//	vec3_t newvalue;
//	if (f) {
//		f = 1.0 / sqrt(f);
//		VectorScale(value1, f, newvalue);
//	}
//	else
//		VectorClear(newvalue);
//
//	//VectorCopy (newvalue, PRVM_G_VECTOR(OFS_RETURN));
//}

RELATED_ (Con_Pos_f VM_makevectors VM_normalize)
static qbool infront (vec3_t org, vec3_t angles, vec3_t p)
{
	vec3_t forward, right, up;

	// makevectors (self.angles)
	AngleVectors(angles, forward, right, up);

	// normalize (targ.origin - self.origin);
	vec3_t vec;
	VectorSubtract (p, org, vec);
	VectorNormalize (vec);

	//vector	vec;
	//float		dot;

	//makevectors (self.angles);
	//vec = normalize (targ.origin - self.origin);
	//dot = vec * v_forward;
				// HANDLE_OPCODE(OP_MUL_V):
				// OPC->_float = OPA->vector[0]*OPB->vector[0] + OPA->vector[1]*OPB->vector[1] + OPA->vector[2]*OPB->vector[2];
	float dot = DotProduct (vec, forward);


	if (dot > 0.3) {
		return true;
	}
	return false;


}

static void PRVM_ED_anyset_f(cmd_state_t *cmd, ccs *s_progname)
{
	char valuebuf[1024];
	int argc = cmd_argc;
	ccs *s_edict_num	= cmd_argv_parm1;
	ccs *s_fieldname	= cmd_argv_parm2;
	ccs *s_value		= cmd_argv_parm3;

	if (argc < 2) {
		// No params - print information
		// ex: eset
		Con_PrintLinef ("usage:" NEWLINE
			"%s <edict_num> // print an edict" NEWLINE
			"%s <edict_num> <fieldname> // print a field value" NEWLINE
			"%s <edict_num> <fieldname> <value> // set a field value" NEWLINE
			"%s <nearby> // list edicts nearby with radius 300" NEWLINE
			"%s <nearby> <radius> // list edicts nearby with custom radius " NEWLINE, // End of string
			cmd_argv_command_0, cmd_argv_command_0, cmd_argv_command_0, cmd_argv_command_0, cmd_argv_command_0);
		return;
	}

	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return; // Bad program
	}

	int edict_num = atoi (s_edict_num);
	if (in_range_beyond (0, edict_num, prog->num_edicts) == false) {
		Con_PrintLinef ("Bad edict number (0 to %d valid)", prog->num_edicts - 1);
		return; // Bad edict number
	}

	if (String_Match (cmd_argv_parm1, "nearby")) {
		int count = 0;
		float radius = (argc == 3) ? atof (cmd_argv_parm2) : 300;

		// Baker: An alternative is vieworg.
		vec3_t cl_player_origin, cl_player_angles;
		VectorCopyDestSrc (cl_player_origin, cl.entities[cl.playerentity].state_current.origin);
		VectorCopyDestSrc (cl_player_angles, cl.entities[cl.playerentity].state_current.angles);

		// List ones nearby
		for (int edict_num = AFTER_WORLD_1; edict_num < prog->num_edicts; edict_num ++) {
			prvm_edict_t	*ed = PRVM_EDICT_NUM(edict_num);

			if (ed->free)
				continue; // It's free

			//qbool has_origin = PRVM_ED_EdictGet_Fly_Lite (prog, edict_num, "origin", valuebuf, sizeof(valuebuf));

			vec3_t center;
			char s_classname[1024] = {0};
			char s_targetname[1024] = {0};
			char s_model[1024] = {0};
			char s_origin[1024] = {0};

			PRVM_ED_EdictGet_Fly_LiteX (prog, edict_num, "classname", s_classname);
			PRVM_ED_EdictGet_Fly_LiteX (prog, edict_num, "targetname", s_targetname);
			PRVM_ED_EdictGet_Fly_LiteX (prog, edict_num, "model", s_model);

			ccs *model_skippath = File_URL_SkipPath (s_model);

			{
				char s_absmin[1024], s_absmax[1024], s_origin[1024];
				PRVM_ED_EdictGet_Fly_LiteX (SVVM_prog, edict_num, "absmin", s_absmin);
				PRVM_ED_EdictGet_Fly_LiteX (SVVM_prog, edict_num, "absmax", s_absmax);
				vec3_t vabsmin; Math_atov3 (s_absmin, vabsmin);
				vec3_t vabsmax; Math_atov3 (s_absmax, vabsmax);
				if (!Vector3_IsZeros (vabsmin)) {
					// Get it from center
					VectorAdd (vabsmin, vabsmax, center);
					VectorScale (center, 0.5, center);
				}
				else {
					// Get from origin
					PRVM_ED_EdictGet_Fly_LiteX (SVVM_prog, edict_num, "origin", s_origin);
					Math_atov3 (s_origin, center);
				}
			}

			// Now check distance
			float distance = VectorDistance (cl_player_origin, center);

			if (distance > radius)
				continue; // Disqualified due to radius

			// Baker: Now we want to check if in front of.
			qbool is_infront = infront(cl_player_origin, cl_player_angles, center);

			if (!is_infront) {
				//Con_PrintLinef ("%d (%s) is near, but not in front of us", edict_num, s_classname);
				continue;
			}

			// Fill in s_origin
			c_dpsnprintf1 (s_origin, VECTOR3_G, VECTOR3_SEND(center));
			count ++;

			Con_PrintLinef ("%3d: %s (targetname: %s model: %s)",
				edict_num,
				s_classname,
				s_targetname,
				model_skippath // s_model
			);
		} // each entity

		Con_PrintLinef ("Results = %d in radius of %g and in front of view", count, radius);
		return;
	} // END RADIUS PRINT

	if (argc == 2) {
		// Become "edict"
		// ex: eset 25
		/*int isok =*/ Print_A_Edict_Print_Errors (s_progname, s_edict_num, s_fieldname);
		return;
	}

	if (argc == 3) {
		// Become "get"
		// ex: eset 25 targetname
		PRVM_ED_EdictGet_Fly (s_progname, s_edict_num, s_fieldname, valuebuf, sizeof(valuebuf));

		Con_PrintLinef ("%s", valuebuf);
		if (prvm_edictget_autostore.integer) {
			_Cvar_Set_IsNotify_IsClipboard_String (&_val, q_notifyinfobar_true, prvm_edictget_autostore.integer >= 2 /*d:2*/, q_shall_print_true,
				"%s",
				valuebuf
			);
		} // autostore
		return;
	}

	// 0    1  2     3
	// eset 40 scale 0.4    (argc 4)
	// 0    1  2     3 4 5
	// eset 40 color 2 2 2  (argc 6)

	// Value is argc == 4, if we are at 5 or greater cumulate
	if (argc == 4) {
		// 0    1  2     3
		// ex: eset 40 scale 0.4    (argc 4)
		c_strlcpy (valuebuf, s_value);
	} else /*else if (argc >= 5)*/ {
		// Accumulate
		// 0    1  2     3 4 5
		// eset 40 color 2 2 2  (argc 6)
		Argv_Cumulate (cmd, /*low arg num*/ 3, valuebuf, sizeof(valuebuf) );
		Con_PrintLinef ("Accumulated value string = " QUOTED_S, valuebuf);
	}

	/*int isok =*/ Set_A_Value_Print_Errors (s_progname, s_edict_num, s_fieldname, valuebuf);
}

static void PRVM_ED_Eset_f(cmd_state_t *cmd)
{
	PRVM_ED_anyset_f (cmd, "server");
}


static void PRVM_ED_Cset_f(cmd_state_t *cmd)
{
	PRVM_ED_anyset_f (cmd, "client");
}

#ifdef CONFIG_MENU
static void PRVM_ED_Mset_f(cmd_state_t *cmd)
{
	PRVM_ED_anyset_f (cmd, "menu");
}
#endif // CONFIG_MENU

