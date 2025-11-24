// filematch_csg_p_keys.c.h

RELATED_ (VM_map_find_entity)
// Returns NULL or value for key for the entity
ccs *entitykeys_find_value (entityx_t *e, ccs *keyname)
{
	// list_map1.entity[0]
	for (int j = 0; j < e->pairslist.numstrings; j += 2) {
		ccs *key = e->pairslist.strings[j + 0];
		ccs *val = e->pairslist.strings[j + 1];
		if (String_Match (key, keyname))
			return val;
	}
	return NULL;
}

int entity_key_idx_for_name (entityx_t *r_ent, ccs *keyname_wanted)
{
	for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
		ccs *key = r_ent->pairslist.strings[kx + 0];
		//Dec 22 2024 unused ... ccs *val = r_ent->pairslist.strings[kx + 1];
		if (String_Match(key, keyname_wanted))
			return kx;
	} // each epair

	return not_found_neg1;
}

qbool entity_get_center_isok (entityx_t *r_ent, vec3_t dest)
{
	//vec3_t vcenter;// = (e.absmin + e.absmax) * 0.5;
	
	VectorAdd	(r_ent->bemins, r_ent->bemaxs, dest);
	VectorScale	(dest, 0.5, dest);
	return true;
}

qbool entity_key_get_vector_isok (entityx_t *r_ent, ccs *keyname_wanted, vec3_t dest)
{
	ccs *val = entity_key_get_value (r_ent, keyname_wanted);
	if (!val) {
		VectorSet (dest, -999999, -999999, -999999); // Whatever (ALSO better not happen)
		return false;
	}

	Math_atov3 (val, dest); // string, vector
	return true;
}


qbool entity_key_set_vector_force (entityx_t *d_ent, ccs *keyname_wanted, const vec3_t v)
{
	va_super (s_vector, 128, VECTOR3_G, VECTOR3_SEND(v) );
	return (qbool)entity_key_set_value_is_ok_force (d_ent, keyname_wanted, s_vector);
}

qbool entity_key_set_vector_xyz (entityx_t *d_ent, ccs *keyname_wanted, float x, float y, float z)
{
	va_super (s_vector, 128, VECTOR3_G, x, y, z);
	return (qbool)entity_key_set_value_is_ok_force (d_ent, keyname_wanted, s_vector);
}


ccs *entity_key_get_value (entityx_t *r_ent, ccs *keyname_wanted)
{
	for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
		ccs *key = r_ent->pairslist.strings[kx + 0];
		ccs *val = r_ent->pairslist.strings[kx + 1];
		if (String_Match (key, keyname_wanted))
			return val;
	}
	return NULL;
}

qbool entity_key_idx_set_value_is_ok (entityx_t *d_ent, int keyidx, ccs *snewval)
{
	//Dec 22 2024 unused ... ccs *key = r_ent->pairslist.strings[keyidx + 0];
	//Dec 22 2024 unused ... ccs *val = r_ent->pairslist.strings[keyidx + 1];
	Mem_FreeNull_ (d_ent->pairslist.strings[keyidx + 1]);
	d_ent->pairslist.strings[keyidx + 1] = Z_StrDup (snewval);
	return true;
}

int entity_key_idx_set_key_is_ok (entityx_t *d_ent, int keyidx, ccs *snewval)
{
	//Dec 22 2024 unused ... ccs *key = r_ent->pairslist.strings[keyidx + 0];
	//Dec 22 2024 unused ... ccs *val = r_ent->pairslist.strings[keyidx + 1];
	Mem_FreeNull_ (d_ent->pairslist.strings[keyidx + 0]);
	d_ent->pairslist.strings[keyidx + 0] = Z_StrDup (snewval);
	return true;
}

RELATED_ (entity_key_set_value_is_ok_force)
int entity_key_set_value_is_ok (entityx_t *d_ent, ccs *keyname_wanted, ccs *value_to_set)
{
	int keyidx = entity_key_idx_for_name (d_ent, keyname_wanted);
	if (keyidx == not_found_neg1)
		return false;
	return entity_key_idx_set_value_is_ok (d_ent, keyidx, value_to_set);
}

qbool entity_keypair_add_unsafe (entityx_t *d_ent, ccs *key, ccs *val)
{
	stringlistappend (&d_ent->pairslist, key);
	stringlistappend (&d_ent->pairslist, val);
	return true;
}

int entity_key_rename_is_ok (entityx_t *d_ent, ccs *keyname_wanted, ccs *value_to_set)
{
	int keyidx = entity_key_idx_for_name (d_ent, keyname_wanted);
	if (keyidx == not_found_neg1)
		return false;
	// Found it.  Change it.
	return entity_key_idx_set_key_is_ok (d_ent, keyidx, value_to_set);
}

int entity_key_set_value_is_ok_force (entityx_t *d_ent, ccs *keyname_wanted, ccs *value_to_set)
{
	int keyidx = entity_key_idx_for_name (d_ent, keyname_wanted);
	if (keyidx == not_found_neg1) {
		stringlistappend (&d_ent->pairslist, keyname_wanted); // Add epair.
		stringlistappend (&d_ent->pairslist, value_to_set);
		return 2;
	}
	return entity_key_idx_set_value_is_ok (d_ent, keyidx, value_to_set);
}
