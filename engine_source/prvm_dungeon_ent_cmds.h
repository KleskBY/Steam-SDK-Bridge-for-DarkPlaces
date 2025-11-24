// prvm_dungeon_ent_cmds.h

//float map_append_map (float hmapdest, float hmapsrc) = #152;
//VM entity_attributes
// entity_has_originbrush
//
//VM entity_has_originbrush
//VM entity translate
//VM replace texture
//VM entity find texture
//num textures

//void VM_entity_move (prvm_prog_t *prog) // #151 float map_move (float hmap, vector move) = #151;
//{
//	entitylist_translate_epairs_origin	(&list_paste, translate_amount);
//	entitylist_translate_brushes		(&list_paste, translate_amount);
//}


// #160 float map_entity_key_count (float hmap, float entnum) = #159;
// Returns entity number, 0 is world, failure returns -1 on invalid entnum
// Returns -4 on handle failure.
// float numkeys = map_entity_key_count (h, entum)
void VM_map_entity_key_count (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(2, 2, VM_map_entity_key_count); // hmap, entnum // opt:

	HMAP_HANDLE_BOILER;

	int entnum = (int)PRVM_G_FLOAT(OFS_PARM1);

	entitylist_t *pe = prog->open_dot_maps[handle];

	if (in_range_beyond (0, entnum, pe->count) == false) {
		VM_WarningLinef (prog, "%s: entnum %d invalid, numents is %d in %s", __func__, entnum, pe->count, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -1;
		return;
	}

	entityx_t *r_ent = &pe->entity[entnum];
	int num_keys_for_ent = r_ent->pairslist.numstrings / 2; // Number of pairs is numstrings divided by 2.

	PRVM_G_FLOAT(OFS_RETURN) = num_keys_for_ent;
}


// #161 string map_entity_key_for_index (float hmap, float entnum, float keyindex) = #160;
// Returns NULL (world) on failure
// string skeyval = map_entity_key_for_index(h, entnum, /*keyindex*/ 0);

RELATED_ (VM_fgets)
void VM_map_entity_key_for_index (prvm_prog_t *prog)
{
	char string16384[VM_STRINGTEMP_LENGTH_16384];

	VM_SAFEPARMCOUNTRANGE(3, 3, VM_map_entity_key_for_index); // hmap, entnum, keyindex // opt:

	// set the return value regardless of any possible errors
	PRVM_G_INT(OFS_RETURN) = OFS_NULL;

	HMAP_HANDLE_STRING_RETURN_BOILER;

	int entnum = (int)PRVM_G_FLOAT(OFS_PARM1);
	int keyindex = (int)PRVM_G_FLOAT(OFS_PARM2);

	entitylist_t *pe = prog->open_dot_maps[handle];

	if (in_range_beyond (0, entnum, pe->count) == false) {
		VM_WarningLinef (prog, "%s: entnum %d invalid, numents is %d in %s", __func__, entnum, pe->count, prog->name);
		return;
	}

	entityx_t *r_ent = &pe->entity[entnum];
	int num_keys_for_ent = r_ent->pairslist.numstrings / 2; // Number of pairs is numstrings divided by 2.

	if (in_range_beyond (0, keyindex, num_keys_for_ent) == false) {
		VM_WarningLinef (prog, "%s: keyindex %d invalid for entnum %d, num_keys_for_ent is %d in %s", __func__, keyindex, entnum, num_keys_for_ent, prog->name);
		return;
	}

	int index_mult_2 = keyindex * 2;
	ccs *keyname = r_ent->pairslist.strings[index_mult_2 + 0];
	//ccs *keyval = r_ent->pairslist.strings[index_mult_2 + 1];

	dp_strlcpy (string16384, keyname, sizeof(string16384));

	PRVM_G_INT(OFS_RETURN) = PRVM_SetTempString(prog, string16384);
}

void VM_map_entity_keyvalue_for_index (prvm_prog_t *prog)
{
	char string16384[VM_STRINGTEMP_LENGTH_16384];

	VM_SAFEPARMCOUNTRANGE(3, 3, VM_map_entity_keyvalue_for_index); // hmap, entnum, keyindex // opt:

	// set the return value regardless of any possible errors
	PRVM_G_INT(OFS_RETURN) = OFS_NULL;

	HMAP_HANDLE_STRING_RETURN_BOILER;

	int entnum = (int)PRVM_G_FLOAT(OFS_PARM1);
	int keyindex = (int)PRVM_G_FLOAT(OFS_PARM2);

	entitylist_t *pe = prog->open_dot_maps[handle];

	if (in_range_beyond (0, entnum, pe->count) == false) {
		VM_WarningLinef (prog, "%s: entnum %d invalid, numents is %d in %s", __func__, entnum, pe->count, prog->name);
		return;
	}

	entityx_t *r_ent = &pe->entity[entnum];
	int num_keys_for_ent = r_ent->pairslist.numstrings / 2; // Number of pairs is numstrings divided by 2.

	if (in_range_beyond (0, keyindex, num_keys_for_ent) == false) {
		VM_WarningLinef (prog, "%s: keyindex %d invalid for entnum %d, num_keys_for_ent is %d in %s", __func__, keyindex, entnum, num_keys_for_ent, prog->name);
		return;
	}

	int index_mult_2 = keyindex * 2;
	//ccs *keyname = r_ent->pairslist.strings[index_mult_2 + 0];
	ccs *keyvalue = r_ent->pairslist.strings[index_mult_2 + 1];

	dp_strlcpy (string16384, keyvalue, sizeof(string16384));

	PRVM_G_INT(OFS_RETURN) = PRVM_SetTempString(prog, string16384);
}

// #162 string map_entity_get_key_value (float hmap, float entnum, string key) = #162;
// string s = map_entity_get_key_value (h, 1, "classname");
void VM_map_entity_key_get_value (prvm_prog_t *prog)
{
	char string16384[VM_STRINGTEMP_LENGTH_16384];

	// set the return value regardless of any possible errors
	PRVM_G_INT(OFS_RETURN) = OFS_NULL;

	VM_SAFEPARMCOUNTRANGE(3, 3, VM_map_entity_key_get_value); // hmap 0, entnum 1, keyname 2 // opt:

	HMAP_HANDLE_STRING_RETURN_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];
	int entnum = (int)PRVM_G_FLOAT(OFS_PARM1);

	if (in_range_beyond (0, entnum, pe->count) == false) {
		VM_WarningLinef (prog, "%s: entnum %d invalid, numents is %d in %s", __func__, entnum, pe->count, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -1;
		return;
	}

	entityx_t *r_ent = &pe->entity[entnum];

	ccs *keyname = PRVM_G_STRING(OFS_PARM2);	VM_CheckEmptyString(prog, keyname);
	ccs *keyvalue = entity_key_get_value (r_ent, keyname);

	if (!keyvalue) {
		// Not found, return NULL
		return;
	}

	dp_strlcpy (string16384, keyvalue, sizeof(string16384));
	PRVM_G_INT(OFS_RETURN) = PRVM_SetTempString(prog, string16384);
}

// #163 float map_entity_set_key_value (float hmap, float entnum, string key, string value) = #163;
// float isok = map_entity_set_key_value (h, 1, "classname", "ted");
// string keyvalue = map_entity_get_key_value (h, 1, "classname");

// If the key does not exist, fail.  Do NOT add the key.
void VM_map_entity_key_set_value (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(4, 4, VM_map_entity_key_set_value); // hmap 0, entnum 1, keyname 2, 3 value // opt:

	HMAP_HANDLE_BOILER;

	int entnum = (int)PRVM_G_FLOAT(OFS_PARM1);
	entitylist_t *pe = prog->open_dot_maps[handle];

	if (in_range_beyond (0, entnum, pe->count) == false) {
		VM_WarningLinef (prog, "%s: entnum %d invalid, numents is %d in %s", __func__, entnum, pe->count, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -1;
		return;
	}

	entityx_t *r_ent = &pe->entity[entnum];

	ccs *keyname	= PRVM_G_STRING(OFS_PARM2);	VM_CheckEmptyString(prog, keyname);
	ccs *keyvalue	= PRVM_G_STRING(OFS_PARM3);	VM_CheckEmptyString(prog, keyvalue);

	int isok = entity_key_set_value_is_ok (r_ent, keyname, keyvalue);
	if (!isok) {
		// No existing key, add the key.
		isok = entity_key_set_value_is_ok_force (r_ent, keyname, keyvalue);
		// isok should return 2 "added"
		PRVM_G_FLOAT(OFS_RETURN) = 2;
		return;
	}

	PRVM_G_FLOAT(OFS_RETURN) = 1;
}

// #164 float map_entity_get_mins_maxs (float hmap, float entnum, __inout vector mins_out, __inout vector maxs_out) = #164;
// float wasprocessed = map_entity_get_mins_maxs (h, /*entnum* 1, minsout, maxsout);
void VM_map_entity_get_mins_maxs (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(4, 4, VM_map_entity_key_count); // hmap 0, entnum 1, minsout 2, maxsout 3 // opt:

	HMAP_HANDLE_BOILER;

	int entnum = (int)PRVM_G_FLOAT(OFS_PARM1);
	entitylist_t *pe = prog->open_dot_maps[handle];

	if (in_range_beyond (0, entnum, pe->count) == false) {
		VM_WarningLinef (prog, "%s: entnum %d invalid, numents is %d in %s", __func__, entnum, pe->count, prog->name);
		PRVM_G_FLOAT(OFS_RETURN) = -1;
		return;
	}

	entityx_t *r_ent = &pe->entity[entnum];

	vec3_t pmins = {0}, pmaxs = {0};
	brmins_maxs_init (pmins, pmaxs);

	int did_any = entity_expand_bounds_did_brushes(r_ent, pmins, pmaxs);

	if (did_any == 0) {
        // Did not find any brushes
        PRVM_G_FLOAT(OFS_RETURN) = 0;
        return;
	}
	VectorCopyDestSrc(PRVM_G_VECTOR(OFS_PARM2), pmins);
	VectorCopyDestSrc(PRVM_G_VECTOR(OFS_PARM3), pmaxs);

	PRVM_G_FLOAT(OFS_RETURN) = 1;
}



// #169 float map_entity_add (float hmap, string keyname, string keyvalue) = #169;
//int entnum = map_entity_add (h, "classname", "shambler");
void VM_map_entity_add (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(3, 3, VM_map_entity_add); // hmap "classname", "shambler"  // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];

	ccs *keyname	= PRVM_G_STRING(OFS_PARM1);	VM_CheckEmptyString(prog, keyname);
	ccs *keyvalue	= PRVM_G_STRING(OFS_PARM2);	VM_CheckEmptyString(prog, keyvalue);

	int entnum = entitylist_add_entity (pe); // Can't fail.

	entityx_t *r_ent = &pe->entity[entnum];

	/*int isok = */ entity_key_set_value_is_ok_force (r_ent, keyname, keyvalue); // Can't fail.

	PRVM_G_FLOAT(OFS_RETURN) = entnum;
}

// #171 float map_texture_replace (float hmap, string find, string replace) = #171;
// float numdone = map_texture_replace(h, "texturecan/tiles_0014", "texturecan/tiles_0019");
// Textures do not have extensions in .map files.  Returns number of replacements performed.
void VM_map_texture_replace (prvm_prog_t *prog)
{
	VM_SAFEPARMCOUNTRANGE(3, 3, VM_map_texture_replace); // hmap sfind sreplacewith // opt:

	HMAP_HANDLE_BOILER;

	entitylist_t *pe = prog->open_dot_maps[handle];

	ccs *texturefind	= PRVM_G_STRING(OFS_PARM1);	VM_CheckEmptyString(prog, texturefind);
	ccs *texturereplace	= PRVM_G_STRING(OFS_PARM2);	VM_CheckEmptyString(prog, texturereplace);

	int numreplaces = entitylist_replace_texture (pe, texturefind, texturereplace);

	PRVM_G_FLOAT(OFS_RETURN) = numreplaces;
}


