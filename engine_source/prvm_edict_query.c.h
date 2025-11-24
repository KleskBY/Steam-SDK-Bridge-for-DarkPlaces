// prvm_edict_query.c.h



char *PRVM_Entities_Query_EdictNum_ZAlloc (prvm_prog_t *prog, int edict_num, ccs *fieldname)
{
	RELATED_ (PRVM_ED_PrintNum)
	prvm_edict_t	*ed = PRVM_EDICT_NUM(edict_num);

	for (int fieldnum = 1; fieldnum < prog->numfielddefs; fieldnum ++) {
		mdef_t	*d = &prog->fielddefs[fieldnum];
		const char	*s_key = PRVM_GetString(prog, d->s_name);

		prvm_eval_t	*val = (prvm_eval_t *)(ed->fields.fp + d->ofs);

		// if the value is still all 0, skip the field
		int		vartype = Flag_Remove(d->type , DEF_SAVEGLOBAL);//& ~DEF_SAVEGLOBAL;
		int vector_idx;
		for (vector_idx = 0; vector_idx < prvm_type_size[vartype]; vector_idx ++)
			if (val->ivector[vector_idx])
				break;

		if (vector_idx == prvm_type_size[vartype])
			continue;

		va_super (crash_location, 1024, "PRVM_ED_Write, ent=%d, name=%s", edict_num, s_key);
		prog->statestring = crash_location;

		char valuebuf[MAX_INPUTLINE_16384];
		char *s_value = PRVM_UglyValueString(prog, (etype_t)d->type,
			val, valuebuf, sizeof(valuebuf));

		if (String_Match (s_key, fieldname)) {
			return Z_StrDup (s_value);
		}
		prog->statestring = NULL;
	} // each fieldnum

	return NULL;
}


RELATED_ (PRVM_ED_FindField ZDev_Fields_Feed_Shall_Stop_Fn)
// sendentity is 348 aiment is 101 and 214
// ammo_cells is 69 and 235
// 0 is "", model index is 1.
// There are 77 required vars in quake .. 14 are vectors that would be 3 fields so add 14*2 28 = 105
// But I still can't figure out the pattern, anyway, check unique.  There are dups
// The last one is float	modelindex (1 and 318); to string_t	noise3 (119/372);
RELATED_ (entvars_s)
RELATED_ (VM_numentityfields VM_entityfieldtype ZDev_Fields_Feed_Shall_Stop_Fn)
void PRVM_Fields_Query (prvm_prog_t *prog, feed_fn_t myfeed_shall_stop)
{
	for (int idx = 0; idx < prog->numfielddefs; idx ++) {
		mdef_t		*def	= &prog->fielddefs[idx];
		int			type	= def->type;
		int			vartype	= Flag_Remove(type, DEF_SAVEGLOBAL);
		const char	*s_key	= PRVM_GetString(prog, def->s_name);

		if (vartype == ev_float_2 && String_Ends_With_Caseless_3 (s_key, "_x", "_y", "_z"))
			continue;

		qbool shall_stop = myfeed_shall_stop (idx, s_key, NULL, NULL, NULL, NULL,
			vartype, def->type, 2);
		if (shall_stop)
			break;
	}
}



RELATED_ (PRVM_ED_FindGlobal ZDev_Globals_Feed_Shall_Stop_Fn)
void PRVM_Globals_Query (prvm_prog_t *prog, feed_fn_t myfeed_shall_stop)
{
	char vabuf[1024];
	char valuebuf[MAX_INPUTLINE_16384]; // QuakeC values written to this.

	for (int idx = 0; idx < prog->numglobaldefs; idx ++) {
		mdef_t *global = &prog->globaldefs[idx];

		int type = global->type;

		// Baker: Type can be zero -- examples .. "self"
		const char *s_key = PRVM_GetString(prog, global->s_name);

		// Baker: January 5 2025 - "devinfo" -> globals is showing self so what is this skipping?
		if (false == Have_Flag (global->type, DEF_SAVEGLOBAL))
			continue; // self, other, ...

		int vartype = Flag_Remove(type, DEF_SAVEGLOBAL);

		// Baker: What are the types that are rejected here? .. vector  cause xyz is wriiten
		// filter _x _y _z
		if (false == isin4 (vartype, ev_string_1, ev_float_2, ev_vector_3, ev_entity_4))
			continue;

		// Any of the following suffix we ignore, covered by vector.
		if (vartype == ev_float_2 && String_Ends_With_Caseless_3 (s_key, "_x", "_y", "_z"))
			continue;

		// Baker: prog->statestring provides the current action in case of a crash
		prog->statestring = va(vabuf, sizeof(vabuf), "PRVM_ED_WriteGlobals, name=%s", s_key);
		PRVM_UglyValueString (prog, (etype_t)vartype, (prvm_eval_t *)&prog->globals.fp[global->ofs], valuebuf, sizeof(valuebuf));
		prog->statestring = NULL;

		qbool shall_stop = myfeed_shall_stop (idx, s_key, valuebuf, NULL, NULL, NULL, vartype, 1, 2);
		if (shall_stop)
			break;
	}
}

RELATED_ (PRVM_ED_PrintEdicts_Either PRVM_ED_PrintNum PRVM_ED_Print)

// Do whatever save game does.
void PRVM_Entities_Query_Fieldname (prvm_prog_t *prog, feed2_fn_t myfeed_shall_stop, ccs *s_fieldname_or_null /*like classname*/)
{
	RELATED_ (PRVM_ED_PrintNum)
	qbool shall_stop = false;
	for (int edict_num = 0; edict_num < prog->num_edicts ; edict_num ++) {
		prvm_edict_t	*ed = PRVM_EDICT_NUM(edict_num);

		for (int fieldnum = 1; fieldnum < prog->numfielddefs; fieldnum ++) {
			mdef_t	*d = &prog->fielddefs[fieldnum];
			const char	*s_key = PRVM_GetString(prog, d->s_name);

			//if (s_key == NULL) {
			//	int j = 5;
			//}

			// Baker: No field name?  Return on first field
			if (s_fieldname_or_null == NULL) {
				shall_stop = myfeed_shall_stop (prog, edict_num, s_key, "", NULL, NULL, NULL, 0, fieldnum, 2);
				break; // We want out
			}

			prvm_eval_t	*val = (prvm_eval_t *)(ed->fields.fp + d->ofs);

		// if the value is still all 0, skip the field
			int		vartype = Flag_Remove(d->type , DEF_SAVEGLOBAL);//& ~DEF_SAVEGLOBAL;
			int vector_idx;
			for (vector_idx = 0; vector_idx < prvm_type_size[vartype]; vector_idx ++) {
				if (val->ivector[vector_idx])
					break;
			}

			if (vector_idx == prvm_type_size[vartype])
				continue;

			//Flex_Writef (QUOTED_S " ", name);
			//char vabuf[1024];
			va_super (crash_location, 1024, "PRVM_ED_Write, ent=%d, name=%s", edict_num, s_key);
			prog->statestring = crash_location;

			char valuebuf[MAX_INPUTLINE_16384];
			char *s_value = PRVM_UglyValueString(prog, (etype_t)d->type,
				val, valuebuf, sizeof(valuebuf));

			if (s_key && s_fieldname_or_null && String_Starts_With (s_key, s_fieldname_or_null)) {
				shall_stop = myfeed_shall_stop (prog, edict_num, s_key, s_value, NULL, NULL, NULL, vartype, fieldnum, 2);
				if (shall_stop)
					break; // Then we break again
			}
			prog->statestring = NULL;
		} // each ? um ..

		if (shall_stop)
			break;

	} // each entity


}
