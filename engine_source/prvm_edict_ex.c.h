// prvm_edict_ex.c.h

RELATED_ (PRVM_ED_EdictSet_f PRVM_ED_Eset_f PRVM_ED_GlobalGet_f PRVM_ED_Global_Fly)
RELATED_ (GetGlobalListServer_Count)
static qbool Set_A_Global_Print_Errors (ccs *s_progname, ccs *s_fieldname, ccs *s_value)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return false; // Bad program
	}

	mdef_t *global = PRVM_ED_FindGlobal(prog, s_fieldname);
	if (!global) {
		Con_PrintLinef ( "No global \"%s\" in %s!", s_fieldname, s_progname);
		return false;
	}

//	PRVM_ValueString(prog, (etype_t)global->type, PRVM_GLOBALFIELDVALUE(global->ofs), valuebuf, sizeof(valuebuf) );

	PRVM_ED_ParseEpair (prog, /*ent*/ NULL, global, s_value, qp_parse_backslash_true);
	return true;
}

#if 0
static qbool Get_A_Global_Print_Errors (ccs *s_progname, ccs *s_fieldname, char *valuebuf, size_t valuebuf_size)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return false; // Bad program
	}

	mdef_t *global = PRVM_ED_FindGlobal(prog, s_fieldname);
	if (!global) {
		Con_PrintLinef ( "No global \"%s\" in %s!", s_fieldname, s_progname);
		return false;
	}

//	PRVM_ValueString(prog, (etype_t)global->type, PRVM_GLOBALFIELDVALUE(global->ofs), valuebuf, sizeof(valuebuf) );

	PRVM_ED_ParseEpair (prog, /*ent*/ NULL, global, valuebuf, qp_parse_backslash_true);
	return true;
}
#endif

static qbool Set_A_Value_Print_Errors (ccs *s_progname, ccs *s_edict_num, ccs *s_fieldname, ccs *valuebuf)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return false; // Bad program
	}

	int edict_num = atoi (s_edict_num);
	if (in_range_beyond (0, edict_num, prog->num_edicts) == false) {
		Con_PrintLinef ("Bad edict number (0 to %d valid)", prog->num_edicts - 1);
		return false; // Bad edict number
	}
	prvm_edict_t *ed = PRVM_EDICT_NUM(edict_num);

	mdef_t *key = PRVM_ED_FindField(prog, s_fieldname);
	if (!key) {
		Con_PrintLinef ("Unknown fieldname %s", s_fieldname);
		return false; // Bad field
	}

	PRVM_ED_ParseEpair(prog, ed, key, valuebuf, qp_parse_backslash_true);
	return true;
}

qbool Print_A_Edict_Print_Errors (ccs *s_progname, ccs *s_edict_num, ccs *s_fieldname)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return false; // Bad program
	}

	int edict_num = atoi (s_edict_num);
	if (in_range_beyond (0, edict_num, prog->num_edicts) == false) {
		Con_PrintLinef ("Bad edict number (0 to %d valid)", prog->num_edicts - 1);
		return false; // Bad edict number
	}
	//prvm_edict_t *ed = PRVM_EDICT_NUM(edict_num);

	mdef_t *key = PRVM_ED_FindField(prog, s_fieldname);
	if (!key) {
		Con_PrintLinef ("Unknown fieldname %s", s_fieldname);
		return false; // Bad field
	}

	stringlist_t list = {0};

	PRVM_ED_PrintNum (prog, edict_num, q_vm_printfree_true,
		q_wildcard_fieldname_null, q_vm_classname_NULL, q_vm_targetname_NULL, &list);

	// Baker: If was "free" it printed and we do nothing here
	if (list.numstrings == 0)
		goto edict_was_free_already_printed;

	// PRESORT: "server EDICT 84" needs to be on top of the sort.
	// So .. make it have a leading space and restore it after sort
	{
		char chold = list.strings[0][0];
		list.strings[0][0] = SPACE_CHAR_32;

		// SORT
		stringlistsort (&list, fs_make_unique_false);

		list.strings[0][0] = chold; // restore " erver EDICT 84" => "server EDICT 84"
	}

	// PRINT EACH
	for (int idx = 0; idx < list.numstrings; idx ++) {
		ccs *sxy = list.strings[idx];
		Con_PrintLinef ("%s", sxy);
	}

edict_was_free_already_printed:
	stringlistfreecontents (&list);
	return true;
}

#if 0 // Because PRVM_Ent_Text_Add_Edict_Is_Ok is superior
qbool stringlist_fill_edict_is_ok (stringlist_t *plist, prvm_prog_t *prog, int edict_num)
{
	//prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Server not active");
		return false; // Bad program
	}

	if (in_range_beyond (0, edict_num, prog->num_edicts) == false) {
		Con_PrintLinef ("Bad edict number (0 to %d valid)", prog->num_edicts - 1);
		return false; // Bad edict number
	}
	//prvm_edict_t *ed = PRVM_EDICT_NUM(edict_num);



	PRVM_ED_PrintNum (prog, edict_num, q_vm_printfree_false_pairs_neg_1,
		q_wildcard_fieldname_null, q_vm_classname_NULL, q_vm_targetname_NULL, plist);

	// Baker: If was "free" it printed and we do nothing here
	if (plist->numstrings == 0)
		goto edict_was_free_already_printed;

	// PRESORT: "server EDICT 84" needs to be on top of the sort.
	// So .. make it have a leading space and restore it after sort
	{
//		char chold = plist->strings[0][0];
//		plist->strings[0][0] = SPACE_CHAR_32;

		// SORT
		stringlistsort (plist, fs_make_unique_false);

//		plist->strings[0][0] = chold; // restore " erver EDICT 84" => "server EDICT 84"
	}

#if 0 // No
	// PRINT EACH
	for (int idx = 0; idx < plist->numstrings; idx ++) {
		ccs *sxy = plist->strings[idx];
		Con_PrintLinef ("%s", sxy);
	}
#endif

edict_was_free_already_printed:
	// stringlistfreecontents (&list);
	return true;
}
#endif

// Returns false if no field or bad program or bad edict number
qbool PRVM_ED_EdictGet_Fly_Lite (prvm_prog_t *prog, int edict_num, ccs *s_fieldname, char *valuebuf, size_t valuebuf_size)
{
	if (!prog) {
		dpsnprintf (valuebuf, valuebuf_size, "%s", "**program not active/found");
		return false; // Bad program
	}

	if (in_range_beyond (0, edict_num, prog->num_edicts) == false) {
		dpsnprintf (valuebuf, valuebuf_size, "**" "Bad edict number (0 to %d valid)" "**", prog->num_edicts - 1);
		return false; // Bad edict num
	}

	prvm_edict_t *ed = PRVM_EDICT_NUM(edict_num);

	mdef_t *key = PRVM_ED_FindField(prog, s_fieldname);
	if (!key) {
		dpsnprintf (valuebuf, valuebuf_size, "** unknown fieldname %s **", s_fieldname);
		return false; // Bad field
	}

	prvm_eval_t *v = (prvm_eval_t *)(ed->fields.fp + key->ofs);
	/*ccs *s =*/ PRVM_UglyValueString(prog, (etype_t)key->type, v, valuebuf, valuebuf_size);

	return true;
}


RELATED_ (PRVM_ED_EdictGet_Fly_Lite see also)
qbool PRVM_ED_EdictGet_Fly (ccs *s_progname, ccs *s_edict_num, ccs *fieldname, char *valuebuf, size_t valuebuf_size)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);

	int edict_num = atoi(s_edict_num);

	return PRVM_ED_EdictGet_Fly_Lite(prog, edict_num, fieldname, valuebuf, valuebuf_size);
}

RELATED_ (GetGlobalListServer_Count PRVM_ED_EdictGet_Fly)
RELATED_ (DevTabSelectOnChange PRVM_Globals_Query)

CALLERS_ (PRVM_GlobalSet_f)
qbool PRVM_ED_Global_Fly (ccs *s_progname, ccs *s_fieldname, char *valuebuf, size_t valuebuf_size)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		dpsnprintf (valuebuf, valuebuf_size, "%s", "**program not active/found");
		return false; // Bad program
	}

	mdef_t *global = PRVM_ED_FindGlobal(prog, s_fieldname);
	if (!global) {
		dpsnprintf (valuebuf, valuebuf_size, "** unknown global %s **", s_fieldname);
		return false; // Bad field
	}

	prvm_eval_t *v = (prvm_eval_t *) &prog->globals.fp[global->ofs];
	/*ccs *s =*/ PRVM_UglyValueString(prog, (etype_t)global->type, v, valuebuf, valuebuf_size);

	return true;
}

#include "prvm_edict_query.c.h"
#include "prvm_edict_eset.c.h"


CALLERS_ (PRVM_ED_GlobalGet_f)
RELATED_ (PRVM_Globals_Query PRVM_ED_PrintEdicts_Either PRVM_ED_Print)

qbool Print_Globals (ccs *s_progname)
{
	prvm_prog_t *prog = PRVM_FriendlyProgFromString(s_progname);
	if (!prog) {
		Con_PrintLinef ("Program %s not active", s_progname);
		return false; // Bad program
	}

	// This includes intermediate variables and such ... list can be huge.
	for (int idx = 0; idx < prog->numglobaldefs; idx ++) {
		char valuebuf[1024];

		mdef_t		*global			= &prog->globaldefs[idx];
		ccs			*s_globalname	= PRVM_GetString(prog, global->s_name);
		int			vartype			= Flag_Remove(global->type , DEF_SAVEGLOBAL);
		ccs			*s_vartype		= VarType_For_EV (vartype);
		prvm_eval_t *v				= (prvm_eval_t *) &prog->globals.fp[global->ofs];
		ccs			*s_value		= PRVM_UglyValueString(prog, (etype_t)global->type, v, valuebuf, sizeof(valuebuf));

		// Baker: January 5 2025 - What is this skipping?
		if (false == Have_Flag (global->type, DEF_SAVEGLOBAL))
			continue; // Baker: I think this excludes intermediates, fields and functions

		// Baker: What are the types that are rejected here? .. vector  cause xyz is wriiten
		// filter _x _y _z
		if (false == isin4 (vartype, ev_string_1, ev_float_2, ev_vector_3, ev_entity_4))
			continue;

		// Any of the following suffix we ignore, covered by vector.
		if (vartype == ev_float_2 && String_Ends_With_Caseless_3 (s_globalname, "_x", "_y", "_z"))
			continue;


		Con_PrintLinef ("%4d: " CON_GRAY_50 "%-6.6s" CON_WHITE " %-35.35s %s" /*S_FMT_LEFT_PAD_14 " %s " CON_GRAY_50 "(%s)" CON_WHITE*/,
			idx,
			s_vartype,
			s_globalname,
			s_value
		);
	}

	return true;
}
