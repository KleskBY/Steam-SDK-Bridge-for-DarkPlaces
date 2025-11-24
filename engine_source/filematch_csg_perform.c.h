// filematch_csg_perform.c.h

RELATED_ (CSG_Perform entitylist_)

// West wall: We expect 0,0 - 64, 200
// Returns brush: We add 6 brush rows, west east south north ceil floor
brush_s *brush_list_model_add_wall_isok (entitylist_t *plist_map, entitylist_t *plist_model, entityx_t *p_ent,
   vec3_t brmins, vec3_t brmaxs, quadrant6_e my_quadrant)
{
#define XH brmaxs[0]
#define YH brmaxs[1]
#define ZH brmaxs[2]
#define XL brmins[0]
#define YL brmins[1]
#define ZL brmins[2]

	vec3_t brdims;
	VectorSubtract (brmaxs, brmins, brdims);

	if (brdims[0] <= 0 || brdims[1] <= 0 || brdims[2] <= 0) {
		return NULL;
	}

	//int brush_number = p_ent->brushlist.count;
	brush_s *p_brush = brushlist_add (&p_ent->brushlist); // CREATE BRUSH FOR DEST

	VectorCopyDestSrc (p_brush->bxmins, brmins);
	VectorCopyDestSrc (p_brush->bxmaxs, brmaxs);
	VectorCopyDestSrc (p_brush->bxdims, brdims);

	entityx_t	*p_ent_mold_world = &plist_model->entity[ENTITY_WORLD_0];
	int mold_west_brush_idx = UNPLUS1(plist_model->quadrants_world_brushnums_plus1[my_quadrant]);
	brush_s *p_brush_mold = &p_ent_mold_world->brushlist.brush[mold_west_brush_idx];

	int mold_tri_idx;
	brushrow_s *mold_row;
	brushrow_s *br;

west: // XH is missing

	br = brushrowlist_add (&p_brush->brushrowlist); // NEW BRUSH ROW.

	mold_tri_idx = UNPLUS1(p_brush_mold->quadrants_brush_faces_plus1[quadrant_west_0]);
	mold_row = &p_brush_mold->brushrowlist.brushrow[mold_tri_idx];

//	//( [XL] [YL] [ZH] ) ( [XL] [YL] [ZL] ) ( [XL] [YH] [ZH] ) common/caulk [ 0 1 0 -20.4 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YL, ZH); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XL, YH, ZH);
	Vector4CopyDestSrc	(br->xtra1, mold_row->xtra1); Vector4CopyDestSrc	(br->xtra2, mold_row->xtra2);
	Vector6CopyDestSrc	(br->ftrail, mold_row->ftrail); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (mold_row->pbrtexture);

east: // XH is missing

	br = brushrowlist_add (&p_brush->brushrowlist); // NEW BRUSH ROW.

	mold_tri_idx = UNPLUS1(p_brush_mold->quadrants_brush_faces_plus1[quadrant_east_1]);
	mold_row = &p_brush_mold->brushrowlist.brushrow[mold_tri_idx];

//	// ( [XH] [YH] [ZH] )                ( [XH] [YH] [ZL] )          ( [XH] [YL] [ZH] )
	VectorSet (br->a, XH, YH, ZH); VectorSet (br->b, XH, YH, ZL); VectorSet (br->c, XH, YL, ZH);
	Vector4CopyDestSrc	(br->xtra1, mold_row->xtra1); Vector4CopyDestSrc	(br->xtra2, mold_row->xtra2);
	Vector6CopyDestSrc	(br->ftrail, mold_row->ftrail); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (mold_row->pbrtexture);

south: // YH is missing

	br = brushrowlist_add (&p_brush->brushrowlist); // NEW BRUSH ROW.

	mold_tri_idx = UNPLUS1(p_brush_mold->quadrants_brush_faces_plus1[quadrant_south_2]);
	mold_row = &p_brush_mold->brushrowlist.brushrow[mold_tri_idx];

//	//( [XH] [YL] [ZH] ) ( [XH] [YL] [ZL] ) ( [XL] [YL] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XH, YL, ZL); VectorSet (br->c, XL, YL, ZH);
	Vector4CopyDestSrc	(br->xtra1, mold_row->xtra1); Vector4CopyDestSrc	(br->xtra2, mold_row->xtra2);
	Vector6CopyDestSrc	(br->ftrail, mold_row->ftrail); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (mold_row->pbrtexture);

north: // XL is missing

	br = brushrowlist_add (&p_brush->brushrowlist); // NEW BRUSH ROW.

	mold_tri_idx = UNPLUS1(p_brush_mold->quadrants_brush_faces_plus1[quadrant_north_3]);
	mold_row = &p_brush_mold->brushrowlist.brushrow[mold_tri_idx];

//	// ( [XL] [YH] [ZH] ) ( [XL] [YH] [ZL] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZH); VectorSet (br->b, XL, YH, ZL); VectorSet (br->c, XH, YH, ZH);
	Vector4CopyDestSrc	(br->xtra1, mold_row->xtra1); Vector4CopyDestSrc	(br->xtra2, mold_row->xtra2);
	Vector6CopyDestSrc	(br->ftrail, mold_row->ftrail); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (mold_row->pbrtexture);

ceil: // ZL is missing

	br = brushrowlist_add (&p_brush->brushrowlist); // NEW BRUSH ROW.

	mold_tri_idx = UNPLUS1(p_brush_mold->quadrants_brush_faces_plus1[quadrant_ceiling_5]);
	mold_row = &p_brush_mold->brushrowlist.brushrow[mold_tri_idx];

//	// ( [XH] [YL] [ZH] ) ( [XL] [YL] [ZH] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XL, YL, ZH); VectorSet (br->c, XH, YH, ZH);
	Vector4CopyDestSrc	(br->xtra1, mold_row->xtra1); Vector4CopyDestSrc	(br->xtra2, mold_row->xtra2);
	Vector6CopyDestSrc	(br->ftrail, mold_row->ftrail); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (mold_row->pbrtexture);

floor: // ZH is missing

	br = brushrowlist_add (&p_brush->brushrowlist); // NEW BRUSH ROW.

	mold_tri_idx = UNPLUS1(p_brush_mold->quadrants_brush_faces_plus1[quadrant_floor_4]);
	mold_row = &p_brush_mold->brushrowlist.brushrow[mold_tri_idx];

	// ( [XL] [YH] [ZL] ) ( [XL] [YL] [ZL] ) ( [XH] [YH] [ZL] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet	(br->a, XL, YH, ZL); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XH, YH, ZL);
	Vector4CopyDestSrc	(br->xtra1, mold_row->xtra1); Vector4CopyDestSrc	(br->xtra2, mold_row->xtra2);
	Vector6CopyDestSrc	(br->ftrail, mold_row->ftrail); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (mold_row->pbrtexture);
#undef XH
#undef YH
#undef ZH
#undef XL
#undef YL
#undef ZL
	return p_brush; // isok
}

// svaluetowrite looks like "0 - 20241005 10:12 AM"
// We set _originmake value to "0 - 20241005 10:12 AM" so it is known it happened.
// if you read the .map source.
int entitylist_perform_originmake_num_made (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	int num_made = 0;
	for (int ex = AFTER_WORLD_1; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		char **pval = NULL;
		//int val_idx = not_found_neg1;
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			ccs *val = p_ent->pairslist.strings[kx + 1];

			// It "_originmake" value has anything that is NOT a leading 0
			// "" or "value" or "1"
			// ... we run it

			// "0" or "0 - something here" ... we do not run it.
			if (String_Match(key, "_originmake")) {
				if (val[0] != '0') {
					if (entity_key_idx_for_name (p_ent, "_atomize") != not_found_neg1) {
						pval = NULL; // NEVER DO THIS ENTITY
						break;
					}
					pval = &p_ent->pairslist.strings[kx + 1]; //wants__originmake = true;
					break;
				}
			}
		} // epairs

		if (pval == NULL /*wants__originmake == false*/)
			continue;

		// SO WE
		vec3_t brmins = { 999999,  999999,  999999};
		vec3_t brmaxs = {-999999, -999999, -999999};

		int are_any_brushes = false;

		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];

			if (p_brush->is_a_patch)
				continue; // PATCH, SKIP

			if (p_brush->brushrowlist.count && are_any_brushes == false)
				are_any_brushes = true;

			// REAL BRUSH
			for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];
				bounds_expand (pbr->a, brmins, brmaxs);
				bounds_expand (pbr->b, brmins, brmaxs);
				bounds_expand (pbr->c, brmins, brmaxs);
			} // brush row
		} // brush

		// ADD A ORIGIN BRUSH from MINS/MAXS BBOX
		if (are_any_brushes) {
			num_made ++;
			entity_add_brush_originbrush_from_bbox (p_ent, brmins, brmaxs);


#if 1
			entity_key_set_value_is_ok (p_ent, "_originmake", s0_plus_timestamp);
#else
			char *newvaluestring = Z_StrDup (svaluetowrite);
			Mem_FreeNull_ (*pval);
			*pval = newvaluestring;
#endif
		}

	} // entities in .map


	return num_made;
}





void entitylist_key_set_replace_val (entitylist_t *plist, int entnum, ccs *key_force, ccs *val_force)
{
	entityx_t *p_ent = &plist->entity[entnum];
	int did_set = false;

	for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
		ccs *key = p_ent->pairslist.strings[kx + 0];

		if (String_Match(key, key_force) == false)
			continue;
		//ccs *val = p_ent->pairslist.strings[kx + 1];

		char *newval = Z_StrDup (val_force); // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
		Mem_FreeNull_ (p_ent->pairslist.strings[kx + 1]);
		p_ent->pairslist.strings[kx + 1] = newval;
		did_set = true;

		// ASSUME MAXIMUM OF A SINGLE "origin" per entity,
		// SO ..
		break;	// GET OUT!
	} // epairs

	if (!did_set) {
		// COULDN'T FIND FOR THIS ENTITY, ADD IT
		stringlistappend (&p_ent->pairslist, key_force); // KEY
		stringlistappend (&p_ent->pairslist, val_force); // VALUE
	}
}


// target and targetname
// e11 ---> around_e11
#if 0 // NO CALLERS ANYMORE
void entitylist_map_include_prefix_epairs (entitylist_t *plist, ccs *prefix)
{
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			ccs *val = p_ent->pairslist.strings[kx + 1];

			if (String_Isin2(key,"targetname","target")== false )
				continue;

			char *newval = Z_StrDupf ("%s%s", prefix, val);  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
			Mem_FreeNull_ (p_ent->pairslist.strings[kx + 1]);
			p_ent->pairslist.strings[kx + 1] = newval;

		} // epairs
	} // entities in .map
}
#endif

// returns num changed.


// Starts with
int entitylist_epairs_find_model_gen_entitynum (entitylist_t *plist, ccs *prefix)
{
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			if (String_Starts_With(key, prefix /*"model_gen"*/))
				return ex;
		} // epairs
	} // entities in .map
	return not_found_neg1;
}

// make car brushes
static void entitylist_make_insert_func_collision_ent_brush_at_1 (entitylist_t *plist, ccs *s)
{
// "models/vehicles/psx/van_car04.md3 188.9375 101.8125 82.25"
// For each
// Make a func_wall_modelgen
// "size" "INSERT"
// "mdl" "INSERT"
// in QuakeC, solid 0
	//entityx_t	*p_ent_world = &plist->entity[0];

	// Insert as first entity
	entityx_t	*d_ent = entitylist_add_at_1_shiftup(plist);

	// We want to be entity #1 so that we exist before all other entities except world.

	ccs *text = s;
	char model[MAX_QPATHX2_256];
	vec3_t vsize;
	COM_Parse_Basic(&text);	c_strlcpy (model, com_token);
	COM_Parse_Basic(&text);	vsize[0] = atof(com_token);
	COM_Parse_Basic(&text);	vsize[1] = atof(com_token);
	COM_Parse_Basic(&text);	vsize[2] = atof(com_token);

	Con_PrintLinef ("Making: func_collision body: %s (size " VECTOR3_5d1F")", model, VECTOR3_SEND(vsize) );

	stringlistappend	(&d_ent->pairslist, "classname"); // KEY
	stringlistappend	(&d_ent->pairslist, "func_collision"); // VAL

	stringlistappend	(&d_ent->pairslist, "mdl"); // KEY
	stringlistappend	(&d_ent->pairslist, model); // VAL

	stringlistappend	(&d_ent->pairslist, "size"); // KEY
	stringlistappendf	(&d_ent->pairslist, VECTOR3_G, VECTOR3_SEND(vsize) ); // VAL


#define XH vsize[0]
#define YH vsize[1]
#define ZH vsize[2]
#define XL 0
#define YL 0
#define ZL 0

	brush_s *d_brush;
	brushrow_s *br;

	d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST

	// 221 83.4 67.3

	// ( [XH] [YH] [ZH] ) ( [XH] [YH] [ZL] ) ( [XH] [YL] [ZH] ) common/caulk
		// [ 0 1 0 -20.4 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0

	br = brushrowlist_add (&d_brush->brushrowlist);
	//          ( [XH] [YH] [ZH] )                ( [XH] [YH] [ZL] )          ( [XH] [YL] [ZH] )
	VectorSet (br->a, XH, YH, ZH); VectorSet (br->b, XH, YH, ZL); VectorSet (br->c, XH, YL, ZH);
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 0, 160, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/caulk");

	br = brushrowlist_add (&d_brush->brushrowlist); // 2 - all numbers identical to 1 except abc
	//( [XL] [YL] [ZH] ) ( [XL] [YL] [ZL] ) ( [XL] [YH] [ZH] ) common/caulk [ 0 1 0 -20.4 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YL, ZH); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XL, YH, ZH);
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 0, 160, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/caulk");

	br = brushrowlist_add (&d_brush->brushrowlist); // 3
	//( [XH] [YL] [ZH] ) ( [XH] [YL] [ZL] ) ( [XL] [YL] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	//( 221 0 67.3 ) ( 221 0 0 ) ( 0 0 67.3 ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XH, YL, ZL); VectorSet (br->c, XL, YL, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 0, 160, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/caulk");

	br = brushrowlist_add (&d_brush->brushrowlist); // 4 - all numbers identical to 3 except abc
	// ( [XL] [YH] [ZH] ) ( [XL] [YH] [ZL] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZH); VectorSet (br->b, XL, YH, ZL); VectorSet (br->c, XH, YH, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 0, 160, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/caulk");

	br = brushrowlist_add (&d_brush->brushrowlist); // 5
	// ( [XL] [YH] [ZL] ) ( [XL] [YL] [ZL] ) ( [XH] [YH] [ZL] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZL); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XH, YH, ZL);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 0, 160, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/caulk");

	br = brushrowlist_add (&d_brush->brushrowlist); // 6 - all ident to 5 except a b c
	// ( [XH] [YL] [ZH] ) ( [XL] [YL] [ZH] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XL, YL, ZH); VectorSet (br->c, XH, YH, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 0, 160, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/caulk");

	d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST


	br = brushrowlist_add (&d_brush->brushrowlist);
	//          ( [XH] [YH] [ZH] )                ( [XH] [YH] [ZL] )          ( [XH] [YL] [ZH] )
	VectorSet (br->a, XH, YH, ZH); VectorSet (br->b, XH, YH, ZL); VectorSet (br->c, XH, YL, ZH);
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	//553648128 16512 0
	br->pbrtexture = Z_StrDup ("common/origin");

	br = brushrowlist_add (&d_brush->brushrowlist); // 2 - all numbers identical to 1 except abc
	//( [XL] [YL] [ZH] ) ( [XL] [YL] [ZL] ) ( [XL] [YH] [ZH] ) common/caulk [ 0 1 0 -20.4 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YL, ZH); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XL, YH, ZH);
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/origin");

	br = brushrowlist_add (&d_brush->brushrowlist); // 3
	//( [XH] [YL] [ZH] ) ( [XH] [YL] [ZL] ) ( [XL] [YL] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XH, YL, ZL); VectorSet (br->c, XL, YL, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/origin");

	br = brushrowlist_add (&d_brush->brushrowlist); // 4 - all numbers identical to 3 except abc
	// ( [XL] [YH] [ZH] ) ( [XL] [YH] [ZL] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZH); VectorSet (br->b, XL, YH, ZL); VectorSet (br->c, XH, YH, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/origin");

	br = brushrowlist_add (&d_brush->brushrowlist); // 5
	// ( [XL] [YH] [ZL] ) ( [XL] [YL] [ZL] ) ( [XH] [YH] [ZL] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZL); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XH, YH, ZL);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/origin");

	br = brushrowlist_add (&d_brush->brushrowlist); // 6 - all ident to 5 except a b c
	// ( [XH] [YL] [ZH] ) ( [XL] [YL] [ZH] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XL, YL, ZH); VectorSet (br->c, XH, YH, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup ("common/origin");

#undef XH
#undef YH
#undef ZH
#undef XL
#undef YL
#undef ZL

}







// Returns entity number or -1
int entitylist_find_key_value (entitylist_t *plist, ccs *key, ccs *val)
{
	return entitylist_find_key_value_ex(plist, key, val, ENTITY_WORLD_0, plist->count);
}

int entitylist_find_key_value_ex (entitylist_t *plist, ccs *key_to_find, ccs *val_to_find, int start_idx, int beyond_idx) // "entity find entity_find"
{
	for (int eidx = start_idx; eidx < plist->count; eidx ++) {
		entityx_t	*r_ent = &plist->entity[eidx];

		ccs *val = entity_key_get_value(r_ent, key_to_find);

		if (!val) continue;

		if (String_Match (val, val_to_find))
			return eidx;
	} // for

	return not_found_neg1;
}


int entitylist_find_key_any_value (entitylist_t *plist, ccs *key_to_find, int start_idx, int beyond_idx, ccs **pVal)
{
	for (int eidx = start_idx; eidx < beyond_idx; eidx ++) {
		entityx_t	*r_ent = &plist->entity[eidx];

		ccs *val = entity_key_get_value(r_ent, key_to_find);

		if (!val) continue;

		NOT_MISSING_ASSIGN (pVal, val);
		return eidx;
	} // for

	return not_found_neg1;
}



// This should copy bxmins and friends ...
RELATED_ (VM_map_append_map)

void entity_copy_brushes_from_entity (entityx_t *d_ent, const entityx_t *r_ent)
{
	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[bx];	// READ BRUSH FROM SRC
		brush_s *d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST

		d_brush->is_a_patch = r_brush->is_a_patch;
		if (r_brush->is_a_patch) { // If patch, copy patch stuff ...
			d_brush->texture = Z_StrDup (r_brush->texture);
			d_brush->rows = r_brush->rows; d_brush->cols = r_brush->cols;
			d_brush->other3[0] = r_brush->other3[0]; d_brush->other3[1] = r_brush->other3[1]; d_brush->other3[2] = r_brush->other3[2];
		}
		if (r_brush->is_a_patch == false) { // Real brush
			// REAL BRUSH
			for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
				const brushrow_s *rbr = &r_brush->brushrowlist.brushrow[brow];
				brushrow_s *dbr = brushrowlist_add(&d_brush->brushrowlist);

				memcpy (dbr, rbr, sizeof(*rbr));
				dbr->pbrtexture = Z_StrDup (rbr->pbrtexture);
			} // brushrow
		} else { // PATCH
			for (int prow = 0; prow < r_brush->patchrowlist.count; prow ++) {
				patchrow_s *rpr = &r_brush->patchrowlist.patchrow[prow];
				patchrow_s *dpr = patchrowlist_add (&d_brush->patchrowlist);
				// 5 x number of columns - mem copy all the numbers
				size_t size_to_copy = sizeof(dpr->flots); // Should be about 1200
				memcpy (dpr->flots, rpr->flots, size_to_copy);
			}
		} // brush
	} // brushes in entity
}

void entity_copy_keys_from_entity (entityx_t *d_ent, const entityx_t *r_ent)
{
	for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
		ccs *key = r_ent->pairslist.strings[kx + 0];
		ccs *val = r_ent->pairslist.strings[kx + 1];

		stringlistappend (&d_ent->pairslist, key); // KEY
		stringlistappend (&d_ent->pairslist, val); // VALUE
	} // epairs
}

// Return idx of the new brush
int entity_copy_return_idx (entitylist_t *plist, entityx_t *r_ent)
{
	int new_entity_index = plist->count;
	entityx_t	*d_ent_new = entitylist_add (plist); // D = DEST

	if (r_ent->brushlist.count) { // ADD PASTE ENTS
		entity_copy_brushes_from_entity (d_ent_new, r_ent);
	}

	entity_copy_keys_from_entity (d_ent_new, r_ent);

	return new_entity_index;
}


//qbool entitylist_map_paste_at (entitylist_t *plist, ccs *s_relpath_map_block, vec3_t vtranslate, ccs *s_targetprefix, stringlist_t *plist_targetfields)
//{
//	entitylist_t list_paste = {0};
//
//	int isok = entitylist_parsemaptxt (&list_paste, s_relpath_map_block);
//	if (!isok) { return false; }
//
//	entitylistfreecontents (&list_paste);
//	return true;
//}

RELATED_ (brush_list_model_add_wall_isok)
CALLERS_ (entity_make_brush_bbox_with_texture2 entitylist_perform_layout_from_boxes)
void Perform_Exit_1_Error (ccs *fmt, ...);
qbool entity_make_brush_install_wall_entities (
	entitylist_t *plist_map, entitylist_t *plist_model,
	brush_s *p_brush_wall,
   /*size_pack_3_s *pInterior, */quadrant6_e my_quadrant)
{
	static int nummies;


	// All world brushes have been metriced
	//entitylist_metric_world_brushes_with_walls
	if (my_quadrant != quadrant_north_3) return false; // Only north right now.

	entityx_t	*p_ent_mold_world = &plist_model->entity[ENTITY_WORLD_0];

	//int entity_count = plist_model->count;
	int start_idx = AFTER_WORLD_1;
	ccs *find_this_key = "_layout_wall_north";
	int mold_quad_brush_idx = UNPLUS1(plist_model->quadrants_world_brushnums_plus1[my_quadrant]);
	brush_s *p_brush_mold_wall = &p_ent_mold_world->brushlist.brush[mold_quad_brush_idx];

	nummies ++;
	Con_PrintLinef ("entity_make_brush_install_wall_entities: %d", nummies ++);
	int start_at_this_idx = AFTER_WORLD_1;
	while (1) {
		ccs *wall_string_value; // value of "_layout_wall_north"
		int entnum = entitylist_find_key_any_value (plist_model, find_this_key, start_at_this_idx, plist_model->original_ent_count, &wall_string_value);
		if (entnum == not_found_neg1) {
			// END OF LOOP - NO MORE MATCHES
			break;
		}
		start_at_this_idx = entnum + 1;
		Con_PrintLinef ("Found entnum %d", entnum);
		entityx_t *r_ent_attachment = &plist_model->entity[entnum];

		// We only
		qbool bis_point_entity = r_ent_attachment->brushlist.count == 0;
		if (!bis_point_entity)
			continue; // Can't handle brush entities at the moment.

		// Q: Will the pre-processor have to do more?
		// A: Yes, misc_model_torches emitter will have to run afterwards but not right now.

		start_idx = entnum + 1; // Next iteration look past us

		// Take pval and parse it out
		stringlist_t list_comma_command = {0}; // "odds .50,mod world_x 118 = 0"
		stringlistappend_split	(&list_comma_command, wall_string_value, ",");

		float odds = -1;
		float mod_of = -1;
		float mod_must_equal = -1;
		float world_axis = -1;
		for (int j = 0; j < list_comma_command.numstrings; j ++) {
			char *this_command_line = list_comma_command.strings[j];

			// SPACE SPLIT                                     (0) (1)     (2) (3) (4)
			stringlist_t list_cmdargs = {0}; // "odds .50" or "mod world_x 118 = 0"
			stringlistappend_split	(&list_cmdargs, this_command_line, " ");
			if (list_cmdargs.numstrings >= 2) {
				ccs *cmd = list_cmdargs.strings[0];
				if (String_Match (cmd, "odds")) {
					odds = atof (list_cmdargs.strings[1]);
				} else if (String_Match (cmd, "mod") && list_cmdargs.numstrings == 5) {
					ccs *s_axis = list_cmdargs.strings[1];
					ccs *s_modof = list_cmdargs.strings[2];
#ifdef _DEBUG
					ccs *s_eq = list_cmdargs.strings[3];
#endif
					ccs *s_val = list_cmdargs.strings[4];
					if (String_Match (s_axis, "world_x"))
						world_axis = 0;
					else if (String_Match (s_axis, "world_y"))
						world_axis = 1;
					mod_of = atoi (s_modof);
					mod_must_equal = atoi (s_val);
				}

			} // if

			stringlistfreecontents (&list_cmdargs);
		} // for

		stringlistfreecontents (&list_comma_command);

		// DO NOT PASS GO SCENARIOS
		if (odds == 0) continue;

		// Calculate positioning that is required
		// Z = North wall floor Z and this entity floor z.  Subtract the 2.
		ccs *s_attach_origin = entity_key_get_value (r_ent_attachment, "origin");
		if (!s_attach_origin) {
			// This would be bad.
			Perform_Exit_1_Error ("entity_key_get_value/origin failed");
		}
		vec3_t vattachorigin;
		Math_atov3 (s_attach_origin, vattachorigin);
		vec3_t vdelta;
		vdelta[0] = vattachorigin[0] - p_brush_mold_wall->bxmins[0];
		vdelta[1] = vattachorigin[1] - p_brush_mold_wall->bxmins[1]; // 400 - 500 -100
		vdelta[2] = vattachorigin[2] - p_brush_mold_wall->bxmins[2]; // 23.5 - 0

		for (int x = p_brush_wall->bxmins[0]; x < p_brush_wall->bxmaxs[0]; x ++) {
			qbool hit_x = (x % ((int)mod_of)) == mod_must_equal;
			if (!hit_x) continue; // Not a hit here
			float myroll = lhrandom(0, 1);
			qbool hit_odds = odds == not_found_neg1 ? true : (myroll <= odds);
			if (!hit_odds) continue; // Failed roll

			// We now must add the entity.
			// The entity is a copy.
			RELATED_ (VM_map_entity_count)

			// COPY THE ATTACHMENT
			int new_entnum = entity_copy_return_idx (plist_map, r_ent_attachment); // D = DEST
			entityx_t *d_new_ent = &plist_map->entity[new_entnum];
			 //entity_copy_brushes_from_entity (d_new_ent,
			// SET ORIGIN.
			vec3_t vorg;
			VectorSet (vorg, x, p_brush_wall->bxmins[1] + vdelta[1] /*y is south some units*/, 0 + vdelta[2]);
			entity_key_set_vector_force (d_new_ent, "origin", vorg);
			static int static_oddshit_count;
			Con_PrintLinef ("odds hit: %d", static_oddshit_count ++);
			//entity_key_set_vector_force (d_new_ent, "origin", vorg);
		} //

		// Determine if hits mod = statement

	} // while (1) entity seek of _layout_wall_north
	return true;
}



int entitylist_change_textures_q1 (entitylist_t *plist, ccs *s_sky_texture, ccs *s_textures_folder) // Returns a (baker_string_t *) or NULL if no data
{
	entityx_t	*p_ent_world = &plist->entity[0];
	stringlistappend (&p_ent_world->pairslist, "_keeplights"); // KEY
	stringlistappend (&p_ent_world->pairslist, "1"); // RADIUS LIGHT


	int num_faces = 0;
	for (int ex = 0; ex < plist->count; ex ++) { // WORLD + ALL
		entityx_t	*p_ent = &plist->entity[ex];

		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];

			if (p_brush->is_a_patch)
				continue; // WE DON'T DO THESE

			// REAL BRUSH
			for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];

				int slen = strlen(pbr->pbrtexture);
				for (int j = 0; j < slen; j ++) {
					int ch = pbr->pbrtexture[j];
					if (isupper(ch)) {
						int ch2 = tolower (ch);
						pbr->pbrtexture[j] = ch2;
					}
				}

				num_faces ++;

				// STAR * TO POUNDER #
				if (pbr->pbrtexture[0] == '*')
					pbr->pbrtexture[0] = '#';
				else if (String_Starts_With (pbr->pbrtexture, "sky")) {
					Mem_FreeNull_ (pbr->pbrtexture);
					pbr->pbrtexture = Z_StrDup (s_sky_texture);
					continue;
				}


				// There are no extensions on textures
				// ( -3392 -696 848 ) ( -3392 -696 0 ) ( -3392 -152 848 ) texturecan/others_0029_facade [ 0 1 0 128 ] [ 0 0 -1 480 ] 0 0.5 0.5
				//va_super (s_texture_rename, 64, "%s/%s", "textures/id1wad", pbr->pbrtexture);
				// NO TEXTURES FOLDER

				va_super (s_texture_rename, 64, "%s/%s", s_textures_folder, pbr->pbrtexture);
				Mem_FreeNull_ (pbr->pbrtexture);
				pbr->pbrtexture = Z_StrDup (s_texture_rename);
			} // row
		} // brush
	} // entities in .map

	// Add "spawnflags" "1" to all "light" entities.
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			ccs *val = p_ent->pairslist.strings[kx + 1];


			if (String_Isin1(key,"classname") && /*String_Isin1*/  String_Starts_With(val, "light")) {
				stringlistappend (&p_ent->pairslist, "spawnflags"); // KEY
				stringlistappend (&p_ent->pairslist, "1"); // RADIUS LIGHT
				break;
			}

			// _KEEPLIGHTS fool!
			//} else if (String_Isin1(key,"classname") && String_Starts_With(val, "light")) {
			//	// Baker: light_ anything gets nuked by q3map2
			//	// light_small_wall_torch -> qight_small_wall_torch
			//	p_ent->pairslist.strings[kx + 1][0] = 'q';
			//}
		} // epairs
	} // entities in .map

	return num_faces;
}

void entitylist_gen_models (entitylist_t *plist, int ex /*entnum*/)
{
	// Baker: We are inserting at entity 1

	// We must perform the insertion AFTER the entities loop
	// Otherwise we will be messing up the entities loop by inserting entries
	// while working the loop.
	stringlist_t modelstoaddlist = {0};
	entityx_t	*p_ent = &plist->entity[ex];
	for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
		ccs *key = p_ent->pairslist.strings[kx + 0];
		ccs *val = p_ent->pairslist.strings[kx + 1];
		if (String_Starts_With(key, "model_gen"))
			stringlistappend (&modelstoaddlist, val);
	} // epairs

	// Add the accumulated entities
	for (int j = 0; j < modelstoaddlist.numstrings; j ++) {
		ccs *val = modelstoaddlist.strings[j];

		//int around2_main_entnum = entitylist_find_key_value_ex (plist, "targetname", "around1_main", /*startindex*/ 1);
		//entityx_t *p_around2_main = &plist->entity[around2_main_entnum];

		entitylist_make_insert_func_collision_ent_brush_at_1 (plist, val);

		//int around3_main_entnum = entitylist_find_key_value_ex (plist, "targetname", "around1_main", /*startindex*/ 1);
		//entityx_t *p_around3_main = &plist->entity[around3_main_entnum];

	}

	// Free
	stringlistfreecontents (&modelstoaddlist);
}

entityx_t *entity_clone_trigger_brush_as (entitylist_t *plist, entityx_t *r_ent, ccs *s_newclassname)
{
	// CLONE ENTITY AS A TRIGGER BRUSH
	entityx_t *d_ent = entitylist_add(plist);

	// STAGE: COPY ENTITY KEYS/VALUES except "_atomize"
	for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
		ccs *key = r_ent->pairslist.strings[kx + 0];
		ccs *val = r_ent->pairslist.strings[kx + 1];

		if (String_Isin1(key, "_clone_trigger"))
			continue; // DO NOT DO THESE

//		if (String_Isin1(key, "classname")) {
//			int j = 5;
//		}
		stringlistappend (&d_ent->pairslist, key); // KEY
		stringlistappend (&d_ent->pairslist, val); // VALUE
	} // epairs

	/*int isok =*/ entity_key_set_value_is_ok (d_ent, "classname", s_newclassname);
	//int j = 5;

	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[bx];

		// STAGE: COPY PATCH STUFF STORED IN R_BRUSH  ...
		if (r_brush->is_a_patch) {
			continue; // Not eligible.
		}

		brush_s *d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST

		d_brush->is_a_patch = false; //r_brush->is_a_patch; // Yes, but we aren't a patch.

		// STAGE: COPY REAL BRUSH ROWS
		for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
			brushrow_s *rbr = &r_brush->brushrowlist.brushrow[brow];
			brushrow_s *dbr = brushrowlist_add(&d_brush->brushrowlist);

			memcpy (dbr, rbr, sizeof(*rbr)); // Cloning the brush row numbers
			dbr->pbrtexture = Z_StrDup ("common/trigger");
		} // brushrow

	} // brushes in entity

	return d_ent;
}

// Baker: This is shitty but leave it alone.  MOVE ALONG ...
//CALLERS_ (x2 map_include)

void entitylist_nonworld_setthis (entitylist_t *plist, ccs *key_force, ccs *val_force)
{
	for (int ex = 1; ex < plist->count; ex ++) { // NON-WORLD, WE START AT 1
		entityx_t	*p_ent = &plist->entity[ex];

		int did_set = false;

		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];

			if (String_Match(key, key_force) == false)
				continue;
			//ccs *val = p_ent->pairslist.strings[kx + 1];

			char *newval = Z_StrDup (val_force);  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
			Mem_FreeNull_ (p_ent->pairslist.strings[kx + 1]);
			p_ent->pairslist.strings[kx + 1] = newval;
			did_set = true;

			// ASSUME MAXIMUM OF A SINGLE "origin" per entity,
			// SO ..
			break;	// GET OUT!
		} // epairs

		if (!did_set) {
			// COULDN'T FIND FOR THIS ENTITY, ADD IT
			stringlistappend (&p_ent->pairslist, key_force); // KEY
			stringlistappend (&p_ent->pairslist, val_force); // VALUE
		}
	} // entities in .map
}

// Caller: fs_baker_csg_map_include_prefix_start_end.c.h
// Baker: This works a list of pairs and sets values.
void entitylist_map_include_nonworld_set (entitylist_t *plist, stringlist_t *plistpairset)
{
	for (int kx = 0; kx < plistpairset->numstrings; kx += 2) {
		ccs *key = plistpairset->strings[kx + 0];
		ccs *val = plistpairset->strings[kx + 1];
		entitylist_nonworld_setthis (plist, key, val);
	} // Each set
}

// Not sky
int entitylist_caulk_world_regular_textures (entitylist_t *plist)
{
	float num_done = 0;
	for (int ex = ENTITY_WORLD_0; ex < 1; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];

		// We don't want to do light emitting ones ... but how common is that?
		for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &d_ent->brushlist.brush[bx];

			for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];

				//stringlistappend (&list_textures, pbr->pbrtexture);
				//num_faces ++;
				if (String_Contains( pbr->pbrtexture, "common/"))
					continue; // Don't do common

				if (String_Contains( pbr->pbrtexture, "sky"))
					continue; // Don't do sky

				face_replace_texture (pbr, "common/caulk");
				num_done ++;
			} // faces
		} // brush
	} // entities in .map
	return num_done;
}

RELATED_ (Generate_Dungeon_Map_File VM_map_prefix_fields_values map_include)
int entitylist_prefix_epairs_targetnames (entitylist_t *plist, ccs *prefix, stringlist_t *plist_prefixes)
{
	int num_changed = 0;
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];

			//if (String_Isin2(key,"targetname","target")== false )
			//	continue;

			int match_idx = stringlist_find_index (plist_prefixes, key);

			if (match_idx == not_found_neg1)
				continue;

			ccs *val = p_ent->pairslist.strings[kx + 1];

			char *newval = Z_StrDupf ("%s%s", prefix, val);  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
			Mem_FreeNull_ (p_ent->pairslist.strings[kx + 1]);
			p_ent->pairslist.strings[kx + 1] = newval;
			num_changed ++;
		} // epairs
	} // entities in .map
	return num_changed;
}

ccs *brush_get_first_non_common_texture (brush_s *a)
{
	for (int brow = 0; brow < a->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &a->brushrowlist.brushrow[brow];
		if (String_Contains (pbr->pbrtexture, "common/"))
			continue;
		return pbr->pbrtexture;
	} // faces
	return NULL;
}

qbool brush_has_texture (brush_s *a, ccs *texture)
{
	for (int brow = 0; brow < a->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &a->brushrowlist.brushrow[brow];
		if (String_Match (pbr->pbrtexture, texture))
			return true;
	} // faces
	return false;
}


// World only
int entitylist_perform_removenondominanttexturebrushes (entitylist_t *plist)
{
	int num_done = 0;
	entityx_t	*p_ent_world = &plist->entity[ENTITY_WORLD_0];
	stringlist_t list_textures = {0};
	stringlist_append_entity_textures (&list_textures, p_ent_world);

	ccs *most_common = stringlist_find_most_common_index (&list_textures, "common/caulk");

	Con_PrintLinef ("Dominant texture is %s", most_common);
	Con_PrintLinef ("Removing brushes from world not using dominant texture ...");

	for (int bx = 0; bx < p_ent_world->brushlist.count; bx ++) {
		brush_s *a = &p_ent_world->brushlist.brush[bx];
		if (brush_has_texture (a, most_common)) continue;
		a->is_considered_deleted = true;
		num_done ++;
	} // brush

	stringlistfreecontents (&list_textures);
	return num_done;
}

int entitylist_perform_infodump (entitylist_t *plist)
{
	// Print all the textures
	// # entities
	// # list of entity types
	// # brushes (in entire map)
	// # faces (in entire map)
	// unique textures and their count
	stringlist_t list_textures = {0};
	stringlist_t list_classnames = {0};


	RELATED_ (entitylist_maptext_bsalloc)
	entityx_t	*p_ent_world = &plist->entity[ENTITY_WORLD_0];

	ccs *stitle = entity_key_get_value(p_ent_world, "message");

	Con_PrintLinef ("Map title: %s", stitle ? stitle : "<NO TITLE>");
	int num_entities = 0, num_patches = 0, num_brushes = 0, num_faces = 0, num_non_common_faces = 0;
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		num_entities ++;
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			ccs *val = p_ent->pairslist.strings[kx + 1];

			if (String_Match (key, "classname"))
				//stringlistappend_unique_didAdd (&list_classnames, val);
				stringlistappend (&list_classnames, val);
		} // epairs

		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];

			//if (p_brush->is_considered_deleted) continue; // June 15 2025: Baker -- Deleted brush support

			if (p_brush->is_a_patch == false) {
				// REAL BRUSH
				num_brushes ++;
				for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
					brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];
					//stringlistappend_unique_didAdd (&list_textures, pbr->pbrtexture);
					stringlistappend (&list_textures, pbr->pbrtexture);
					num_faces ++;
					if (String_Contains( pbr->pbrtexture, "common/") == false)
						num_non_common_faces ++;
				} // faces

			} else {
				num_patches ++;
				stringlistappend_unique_didAdd (&list_textures, p_brush->texture);
			}
		} // brush
	} // entities in .map

//#ifdef _DEBUG
	Con_PrintLinef ("=============");
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		//qbool is_sizeless = Vector3_IsZeros (p_ent->bedims);
		ccs *classname = entity_key_get_value (p_ent, "classname");
		Con_PrintLinef ("Entity # %4d %s min max dim " VECTOR3_G " " VECTOR3_G " " VECTOR3_G,
			ex,
			classname,
			VECTOR3_SEND(p_ent->bemins), VECTOR3_SEND(p_ent->bemaxs), VECTOR3_SEND(p_ent->bedims)
		);

		num_entities ++;
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			ccs *val = p_ent->pairslist.strings[kx + 1];

			if (String_Match (key, "classname"))
				//stringlistappend_unique_didAdd (&list_classnames, val);
				stringlistappend (&list_classnames, val);
		} // epairs

		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *a = &p_ent->brushlist.brush[bx];
			ccs *texturename = brush_get_first_non_common_texture(a);
			Con_PrintLinef ("Entity # %4d Brush %4d " S_FMT_LEFT_PAD_40 " min max dim " VECTOR3_G " " VECTOR3_G " " VECTOR3_G,
				ex,
				bx,
				texturename,
				VECTOR3_SEND(a->bxmins), VECTOR3_SEND(a->bxmaxs), VECTOR3_SEND(a->bxdims)

			);

		}
	}
	Con_PrintLinef ("=============");
//#endif

	Con_PrintLinef ("Classnames:");
	stringlist_condump_autocount_uniques (&list_classnames);
	Con_PrintLinef ("=============");
	Con_PrintLinef ("Textures:");
	stringlist_condump_autocount_uniques (&list_textures);
	Con_PrintLinef ("=============");

	ccs *most_common = stringlist_find_most_common_index (&list_textures, "common/caulk");
	Con_PrintLinef ("Most common texture is %s", most_common);

	stringlistfreecontents (&list_textures);
	stringlistfreecontents (&list_classnames);

	Con_PrintVarInt (num_patches);
	Con_PrintVarInt (num_entities);
	Con_PrintVarInt (num_brushes);
	Con_PrintVarInt (num_faces);
	Con_PrintVarInt (num_non_common_faces);
	Con_PrintLinef ("(num_non_common_faces means face that isn't common/caulk or common/trigger etc.)");

	return 1;

}


int entitylist_perform_unbrushfacer_scratch1_world_only (entitylist_t *plist)
{
	extern cvar_t scratch1;
	ccs *func_group_name = scratch1.string;
	int num_changed = 0;
	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) { // WOLRD ONLY
		entityx_t	*e = &plist->entity[ex];
		ccs *classname = entity_key_get_value (e, "classname");

		// Check classname "func_group_this_one" or whatever.
		if (!String_Match (func_group_name, classname))
			continue; // Not right one.

		for (int bx = 0; bx < e->brushlist.count; bx ++) {
			brush_s *p_brush = &e->brushlist.brush[bx];

			for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];

				vec3_t pba; VectorSubtract (pbr->b, pbr->a, pba);
				vec3_t pca; VectorSubtract (pbr->c, pbr->a, pca);
				vec3_t cross; CrossProduct (pba, pca, cross);

				if (cross[2] < 0) {
					// This one is facing up.
				} else {
					num_changed ++;
					face_retexture_z (pbr, "common/caulk");
				}
			} // row
		} // brush
	} // entities in .map

	return num_changed;
}

// All upwards facing brushes get texture "textures/up"
int entitylist_perform_brushfacer_world_only (entitylist_t *plist, int *pnum_faces)
{
	int num_upwards_faces_changed = 0;
	int num_faces = 0;
	for (int ex = ENTITY_WORLD_0; ex < ( /*world only*/ ENTITY_WORLD_0 + 1); ex ++) { // WOLRD ONLY
		entityx_t	*p_ent = &plist->entity[ex];

		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];
			if (p_brush->is_a_patch)
				continue; // WE DON'T DO THESE

			// REAL BRUSH
			for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];

				num_faces ++;

				vec3_t pba; VectorSubtract (pbr->b, pbr->a, pba);
				vec3_t pca; VectorSubtract (pbr->c, pbr->a, pca);

				vec3_t cross; CrossProduct (pba, pca, cross);

				int is_up_brush = false;
				if (cross[2] < 0) {
					// This one is facing up.
					is_up_brush = true;
					num_upwards_faces_changed ++;
					Mem_FreeNull_ (pbr->pbrtexture);
					pbr->pbrtexture = Z_StrDup ("textures/up");
				}

			} // row

		} // brush
	} // entities in .map
	NOT_MISSING_ASSIGN (pnum_faces, num_faces);
	return num_upwards_faces_changed;
}

int entitylist_perform_atomize_entities_num_made (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	int num_made = 0;
	for (int ex = 1 /*after world!*/; ex < plist->count; ex ++) {
		entityx_t	*r_ent = &plist->entity[ex];
		entityx_t	*d_ent_final = NULL;

		// Check early out scenario ...
		if (r_ent->brushlist.count == 0)
			continue; // No brushes

		int brush2ent_idx = entity_key_idx_for_name (r_ent, "_atomize");

		if (brush2ent_idx == not_found_neg1)
			continue; // "_atomize" Does not exist for entity.

//#define val NO NO
		ccs *this_val = r_ent->pairslist.strings[brush2ent_idx + 1];

		// It "_atomize" value has anything that is NOT a leading 0 -- ... we run it
		// "" or "value" or "1" ... we run those
		// "0" or "0 - something here" ... we do not run it.
		if (this_val[0] == '0')
			continue; // Leading zero .. do not run

clone_brush_or_patch:
		for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
			brush_s *r_brush = &r_ent->brushlist.brush[bx];

			// TURN THIS BRUSH INTO A NEW ENTITY.
			entityx_t *d_ent = d_ent_final = entitylist_add(plist);

			// STAGE: COPY ENTITY KEYS/VALUES except "_atomize"
			for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
				ccs *this_key2 = r_ent->pairslist.strings[kx + 0];
				ccs *this_val2 = r_ent->pairslist.strings[kx + 1];

				// Don't write "_atomize"
				if (String_Match (this_key2, "_atomize"))
					continue;

				stringlistappend (&d_ent->pairslist, this_key2); // KEY
				stringlistappend (&d_ent->pairslist, this_val2); // VALUE
			} // epairs

			brush_s *d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST

			d_brush->is_a_patch = r_brush->is_a_patch; // Yes, but we aren't a patch.

			// STAGE: COPY PATCH STUFF STORED IN R_BRUSH  ...
			if (r_brush->is_a_patch) {
				// If patch, copy patch stuff ...
				d_brush->texture = Z_StrDup (r_brush->texture);
				d_brush->rows = r_brush->rows;
				d_brush->cols = r_brush->cols;
				d_brush->other3[0] = r_brush->other3[0];
				d_brush->other3[1] = r_brush->other3[1];
				d_brush->other3[2] = r_brush->other3[2];
			}

			// STAGE: COPY BRUSH OR PATCH ROWS
			if (r_brush->is_a_patch == false) {
				// REAL BRUSH
				for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
					brushrow_s *rbr = &r_brush->brushrowlist.brushrow[brow];
					brushrow_s *dbr = brushrowlist_add(&d_brush->brushrowlist);

					memcpy (dbr, rbr, sizeof(*rbr));
					dbr->pbrtexture = Z_StrDup (rbr->pbrtexture);
				} // brushrow
			} else {
				// PATCH
				for (int prow = 0; prow < r_brush->patchrowlist.count; prow ++) {
					patchrow_s *rpr = &r_brush->patchrowlist.patchrow[prow];
					patchrow_s *dpr = patchrowlist_add (&d_brush->patchrowlist);

					// 5 x number of columns - mem copy all the numbers
					size_t size_to_copy = sizeof(dpr->flots); // Should be about 1200
//					int j = 5;
					memcpy (dpr->flots, rpr->flots, size_to_copy);
				}
			} // brush
		} // brushes in entity

		// THIS ENTITY HAD ATOMIZATION ... CHANGE THE CLASSNAME "_func_wall_processed"
		num_made ++;

		if (d_ent_final) {
			int idx_for_cnt = entity_key_idx_for_name(d_ent_final, "cnt");
			if (idx_for_cnt == not_found_neg1) {
				// Mark the final brush created with "cnt" "-1" so QuakeC
				// knows it is the last one.
				stringlistappend (&d_ent_final->pairslist, "cnt"); // KEY
				stringlistappend (&d_ent_final->pairslist, "-1"); // VALUE
			} else {
				/*int isok = unused dec 22 2024 */ entity_key_set_value_is_ok (d_ent_final, "cnt", "-1");
			}
		} // d_ent_final

		ccs *s_classname =  entity_key_get_value(r_ent, "classname");
		int isok = true;
		if (s_classname) {
			va_super (snewval, /*slen*/ 64, "_" "%s" "_processed", s_classname);

			// 1. CHANGE CLASSNAME.  (WHY?  So Quake doesn't know what it is and ignores it.)
			//       WHY?  Not ready to delete entities yet.
			// 2. CHANGE "_atomize" to 0
			isok = entity_key_set_value_is_ok(r_ent, "classname", snewval);
		}
//		if (!isok) {
//			int j = 5;
//		}
		isok = entity_key_set_value_is_ok(r_ent, "_atomize", s0_plus_timestamp);
//		if (!isok) {
	//		int j = 5;
		//}
	} // entities in .map

	return num_made;
}

int _entitylist_perform_classname_rename_single (entitylist_t *plist, ccs *oldclassname, ccs *newclassname)
{
	// Find the "_rename" in entity #0
	// Find all of them and replace them.
	int num_renames = 0;

	for (int ex = 1 /*after world!*/; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		ccs *s_classname = entity_key_get_value(p_ent, "classname");

		if (!s_classname)
			continue; // does not have a classname
		if (String_Match (s_classname, oldclassname) == false)
			continue; // Not a match for what we want

		qbool isok = entity_key_set_value_is_ok (p_ent, "classname", newclassname);
		if (isok)
			num_renames ++;
	}

	return num_renames;
}

int entitylist_perform_classname_renames (entitylist_t *plist_map)
{
	// Find the "_rename" in entity #0
	// Find all of them and replace them.
	int num_renames = 0;
	entityx_t	*p_ent_world = &plist_map->entity[0]; // world
	ccs *_rename_value = entity_key_get_value (p_ent_world, "_rename");

	if (_rename_value == NULL)
		return 0; // No such key

	stringlist_t list = {0}; //"misc_model_torch=misc_model,misc_mdl_entity_kickable=misc_mdl_entity"
	stringlistappend_split	(&list, _rename_value, ",");

	for (int j = 0; j < list.numstrings; j ++) {
		char *sxy = list.strings[j];
		stringlist_t list2 = {0}; // misc_model_torch=misc_model
		stringlistappend_split	(&list2, sxy, "=");

		if (list2.numstrings != 2) {
			// Expected 2
			break;
		}
		char *s_oldname = list2.strings[0];
		char *s_newname = list2.strings[1];
		int num_done_here = _entitylist_perform_classname_rename_single (plist_map, s_oldname, s_newname);
		num_renames += num_done_here;

		stringlistfreecontents (&list2);
	}

	stringlistfreecontents (&list);
	return num_renames;
}

// 2 possibilities ...
// func_group --> _clone_trigger -> trigger_pushable (BAD but supported, harder in map editor.)
// trigger_pushable --> func_group (GOOD, easier in map editor.)
// Find entities.  If has "_clone_trigger" "new classname"
// Copy the entity brush with common/trigger texture
// This is NOT atomize.
int entitylist_perform_clone_as_trigger_entities_num_made (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	int num_made = 0;
	for (int ex = 1 /*after world!*/; ex < plist->count; ex ++) {
		entityx_t	*r_ent = &plist->entity[ex];

		// Check early out scenario ...
		if (r_ent->brushlist.count == 0)
			continue; // No brushes

		ccs *_clone_trigger_val = entity_key_get_value (r_ent, "_clone_trigger");

		if (_clone_trigger_val == NULL)
			continue; // "_clone_trigger" Does not exist for entity.

		int is_func_group = String_Match (_clone_trigger_val, "func_group");
		ccs *val_old_classname = entity_key_get_value(r_ent, "classname");
		ccs *val_new_classname = is_func_group ? val_old_classname : _clone_trigger_val;

		// If it is composed of only patches it still makes an entity and mapper is a knucklehead.
		/*entityx_t *d_ent =*/ entity_clone_trigger_brush_as(plist, r_ent, val_new_classname);

		// If is_func_group ... change our classname to func_group.
		if (is_func_group) {
			entity_key_set_value_is_ok (r_ent, "classname", "func_group");
		}

		num_made ++;
	} // entities in .map

	return num_made;
}



// "_clone_classname" "light_csqc_emitter;origin_z += 81;spawnflags = 1"
// Create an exact copy of the entity
RELATED_ (entitylist_perform_layout_from_boxes)
int entitylist_perform_clone_classname_num_made (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	int num_made = 0;
	int entity_count_at_start = plist->count; // Prevent expansion -- no use new var

	for (int ex = AFTER_WORLD_1; ex < entity_count_at_start; ex ++) {
		entityx_t	*r_ent = &plist->entity[ex]; // R = READ

		//ccs *classname = entity_key_get_value (r_ent, "classname");
		ccs *value_clone_adjustments = entity_key_get_value (r_ent, "_clone_classname");
		if (!value_clone_adjustments) continue;

		//
		// DO IT

		// CLONE THE KEYS ========================================================================
		entityx_t	*d_ent_new = entitylist_add (plist); // D = DEST
		for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
			ccs *key = r_ent->pairslist.strings[kx + 0];
			ccs *val = r_ent->pairslist.strings[kx + 1];

			if (String_Match (key, "_clone_classname")) continue; // Don't copy this one!

			stringlistappend (&d_ent_new->pairslist, key); // KEY
			stringlistappend (&d_ent_new->pairslist, val); // VALUE
		} // epairs
		// END CLONE THE KEYS ========================================================================


		// NOW ADJUST
		// GET ORIGIN ==============
		{ // SET NEW ENT ORIGIN KEY.  WE WON'T NEED THESE VARIABLES LATER ...
			vec3_t vorigin = {0};
			ccs *current_origin = entity_key_get_value(r_ent, "origin");// list_argos.strings[2]; // guaranteed due to above
			if (!current_origin) {
				// Brush model like func_group
				// Get the center.
				VectorAdd (r_ent->bemaxs, r_ent->bemins, vorigin);
				//Con_PrintVarVector3 (r_ent->bemins);
				//Con_PrintVarVector3 (r_ent->bemaxs);
				//Con_PrintVarVector3 (vorigin);
				VectorScale (vorigin, 0.5, vorigin);
				//Con_PrintLinef ("Detected brush model needs origin ... " VECTOR3_G, VECTOR3_SEND(vorigin));
			} else {
				// Has origin key
				Math_atov3 (current_origin, vorigin);
			}
			//Con_PrintLinef ("Handling entity %d classname %s at " VECTOR3_G, ex, classname, VECTOR3_SEND(vorigin));
			entity_key_set_vector_force (d_ent_new, "origin", vorigin);
			Con_PrintLinef ("New entity origin set to ... " VECTOR3_G, VECTOR3_SEND(vorigin));
		}
		// END GET ORIGIN ==============


		stringlist_t list_adjustments = {0};
		// Got a tokenize control style split?  Deal with a space split.
		stringlistappend_split	(&list_adjustments, value_clone_adjustments, ",");

		if (String_Contains (value_clone_adjustments, ";")) {
			Con_PrintLinef ("Major error ... you cannot have semi-colons in _clone_classname");
		}
		// First one is classname
		// 1. Clone it
		// 2. Copy all epairs (except _clone_classname)
		// 3. Adjust values
		// "_clone_classname" "light_csqc_emitter;origin_z += 81;spawnflags = 1"
		RELATED_ (VM_map_load)

		ccs *newclassname = list_adjustments.strings[/*classname*/ 0];
		entity_key_set_value_is_ok (d_ent_new, "classname", newclassname);
		// Baker: We start at 1 because 0 is classname and we just read it
		for (int idx = 1; idx < list_adjustments.numstrings; idx ++) {
			char *sxy = list_adjustments.strings[idx];
			stringlist_t list_argos = {0};
			stringlistappend_split	(&list_argos, sxy, " ");
			if (list_argos.numstrings != 2) { /*nada*/
				Con_PrintLinef ("Our split didn't have 2 strings for " QUOTED_S " ignoring ..", sxy);
			}
			else if (String_Match (list_argos.strings[0], "origin_y")) {
				// origin_y += y
				vec3_t vorigin = {0};
				ccs *current_origin = entity_key_get_value(d_ent_new, "origin");// list_argos.strings[2]; // guaranteed due to above
				//Con_PrintVarString (current_origin);
				Math_atov3 (current_origin, vorigin);
				//Con_PrintVarVector3 (vorigin);

				ccs *origin_adjust_y = list_argos.strings[1]; // guaranteed due to above

				float adjust_y = atof(origin_adjust_y);
				vorigin[1] += adjust_y;

				entity_key_set_vector_force (d_ent_new, "origin", vorigin);
				Con_PrintLinef ("New adjusted y entity origin set to ... " VECTOR3_G, VECTOR3_SEND(vorigin));
			}
			else if (String_Match (list_argos.strings[0], "origin_z")) {
				// origin_z += z
				vec3_t vorigin = {0};
				ccs *current_origin = entity_key_get_value(d_ent_new, "origin");// list_argos.strings[2]; // guaranteed due to above
				//Con_PrintVarString (current_origin);
				Math_atov3 (current_origin, vorigin);
				//Con_PrintVarVector3 (vorigin);

				ccs *origin_adjust_z = list_argos.strings[1]; // guaranteed due to above
				float adjust_z = atof(origin_adjust_z);
				vorigin[2] += adjust_z;

				entity_key_set_vector_force (d_ent_new, "origin", vorigin);
				Con_PrintLinef ("New adjusted z entity origin set to ... " VECTOR3_G, VECTOR3_SEND(vorigin));
			}
			else if (String_Match (list_argos.strings[0], "spawnflags") ) {
				ccs *s_spawnflags_value = list_argos.strings[1]; // guaranteed due to above
				//float spawnflags_value = atof(s_spawnflags_value);
				entity_key_set_value_is_ok_force (d_ent_new, "spawnflags", s_spawnflags_value);
			} else {
				Con_PrintLinef ("We have a command we don't know named " QUOTED_S " ignoring ..", list_argos.strings[0]);
#ifdef _DEBUG
				int j = 5; // unknown command
#endif
			}
			stringlistfreecontents (&list_argos);
		} // for
		stringlistfreecontents (&list_adjustments);

		num_made ++;
	} // entities in .map


	return num_made;
}

// Returns number of bboxes processed (brushes in world + func_walls)
CALLERS_ (CSG_Perform)
void Perform_Exit_1_Error (ccs *fmt, ...);

CALLERS_ (entitylist_perform_layout_from_boxes entity_add_brush_originbrush_from_bbox)
RELATED_ (entity_brush_add_with_bbox_and_texture)
qbool entity_make_brush_bbox_with_texture2 (entitylist_t *plist_map, entitylist_t *plist_model, entityx_t *p_ent_world_no,
   vec3_t brmins, vec3_t brmaxs,
   float wall_width, float wall_height, float total_height)
{
	// MAKE A FUNCGROUP
	int number_of_entities = plist_map->count;
	entityx_t *p_ent_group = entitylist_add(plist_map);

	size_pack_3_s room_interior;

	VectorSet (room_interior.mins, brmins[0] + wall_width, brmins[1] + wall_width, 0); // a =
	VectorSet (room_interior.maxs, brmaxs[0] - wall_width, brmaxs[1] - wall_width, wall_height); // a =
	VectorSubtract (room_interior.maxs, room_interior.mins, room_interior.dims);

#ifdef _DEBUG
	if (number_of_entities == 65) {
		int j = 5;
	}
#endif

	stringlistappend2	(&p_ent_group->pairslist, "classname", "func_group");
	stringlistappendf2	(&p_ent_group->pairslist, "targetname", "room_%d", number_of_entities);

	float floor_height = wall_width;
	vec3_t xbrmins, xbrmaxs;
	vec3_t brdims; VectorSubtract (brmaxs, brmins, brdims);
	brush_s *p_brush_wall = NULL;

	// We need to add 6 walls and a lightrod ...

west_wall:
	// WEST EAST SUPERIOR
	// |      maxs
	// ______
	// mins              X
	VectorSet (xbrmins, brmins[0] + 0,          brmins[1], /*floor*/ 0             ); // a =
	VectorSet (xbrmaxs, brmins[0] + wall_width, brmaxs[1], /*ceil*/  wall_height   ); // a =

	p_brush_wall = brush_list_model_add_wall_isok (plist_map, plist_model, p_ent_group, xbrmins, xbrmaxs, quadrant_west_0);
	if (!p_brush_wall) return false; // Can only happen if room is too small

east_wall:
	// WEST EAST SUPERIOR
	// |      maxs
	// ______
	// mins              X
	VectorSet (xbrmins, brmaxs[0] - wall_width, brmins[1], /*floor*/ 0             ); // a =
	VectorSet (xbrmaxs, brmaxs[0] + 0,			brmaxs[1], /*ceil*/  wall_height   ); // a =

	p_brush_wall = brush_list_model_add_wall_isok (plist_map, plist_model, p_ent_group, xbrmins, xbrmaxs, quadrant_east_1);

south_wall:
	// SOUTH NORTH INFERIOR
	// |      maxs
	// ______
	// mins              X
	VectorSet (xbrmins, brmins[0] + wall_width, brmins[1] + 0,          /*floor*/ 0             ); // a =
	VectorSet (xbrmaxs, brmaxs[0] - wall_width, brmins[1] + wall_width, /*ceil*/  wall_height   ); // a =

	p_brush_wall = brush_list_model_add_wall_isok (plist_map, plist_model, p_ent_group, xbrmins, xbrmaxs, quadrant_south_2);
	if (!p_brush_wall) return false;

north_wall:
	// SOUTH NORTH INFERIOR
	// |      maxs
	// ______
	// mins              X
	VectorSet (xbrmins, brmins[0] + wall_width, brmaxs[1] - wall_width,   /*floor*/ 0             ); // a =
	VectorSet (xbrmaxs, brmaxs[0] - wall_width, brmaxs[1] + 0           , /*ceil*/  wall_height   ); // a =

	p_brush_wall = brush_list_model_add_wall_isok (plist_map, plist_model, p_ent_group, xbrmins, xbrmaxs, quadrant_north_3);


	entity_make_brush_install_wall_entities (plist_map, plist_model, p_brush_wall, quadrant_north_3);

floor_wall:
	// CEIL FLOOR SUPERIOR
	// |      maxs
	// ______
	// mins              X
	VectorSet (xbrmins, brmins[0] , brmins[1] , /*floor*/ 0 - floor_height ); // a =
	VectorSet (xbrmaxs, brmaxs[0] , brmaxs[1] , /*ceil*/  0 ); // a =

	p_brush_wall = brush_list_model_add_wall_isok (plist_map, plist_model, p_ent_group, xbrmins, xbrmaxs, quadrant_floor_4);
	if (!p_brush_wall) return false;

ceil_wall:
	// CEIL FLOOR SUPERIOR
	// |      maxs
	// ______
	// mins              X
	VectorSet (xbrmins, brmins[0], brmins[1] , wall_height ); // a =
	VectorSet (xbrmaxs, brmaxs[0], brmaxs[1], wall_height + floor_height); // a =

	p_brush_wall = brush_list_model_add_wall_isok (plist_map, plist_model, p_ent_group, xbrmins, xbrmaxs, quadrant_ceiling_5);
	if (!p_brush_wall) return false;

player_start: // spammy but whatever ...
    ; // YAY!
	vec3_t vmiddle;
	VectorSet (vmiddle, (brmins[0] + brdims[0] / 2), (brmins[1] + brdims[1] / 2), wall_height / 2 - 4 ); //

	entityx_t *p_ent_info_player_start = entitylist_add(plist_map);
	stringlistappend2	(&p_ent_info_player_start->pairslist, "classname", "info_player_start"); // key / val
	stringlistappendf2	(&p_ent_info_player_start->pairslist, "origin", VECTOR3_G, VECTOR3_SEND(vmiddle) ); // "origin" "0 192 186"

//lightrod:
 //   ; // YAY!
    int lightrodnum = plist_model->func_lightrod_entnum;
	if (lightrodnum > 0) {
		// This might work poorly on very big square rooms.
		float lightrod_bottom = wall_height / 2 - 4; //  (brmins[2] + brdims[2] / 2) - 4;
		float lightrod_top = wall_height / 2 + 4; // (brmins[2] + brdims[2] / 2) + 4;
		qbool is_north_south = brdims[1] > brdims[0];
		if (is_north_south) {
			VectorSet (xbrmins, (brmins[0] + brdims[0] / 2) - 4, brmins[1] + wall_width * 2, lightrod_bottom ); //
			VectorSet (xbrmaxs, (brmins[0] + brdims[0] / 2) + 4, brmaxs[1] - wall_width * 2, lightrod_top ); //
		} else {
			// East west
			VectorSet (xbrmins, brmins[0] + wall_width * 2, (brmins[1] + brdims[1] / 2) - 4, lightrod_bottom ); //
			VectorSet (xbrmaxs, brmaxs[0] - wall_width * 2, (brmins[1] + brdims[1] / 2) + 4, lightrod_top ); //
		}

		vec3_t xbrdims; VectorSubtract (xbrmaxs, xbrmins, xbrdims);
		if (xbrdims[0] <= 0 || xbrdims[1] <= 0 || xbrdims[2] <= 0) {
			Con_PrintLinef ("Lightrod too small .. skipping");
			return true;
		}

		// We must create an entity here.  It is named func_lightrod
		entityx_t *p_ent_lightrod_mold = &plist_model->entity[lightrodnum];
		ccs *lightrod_texturename = entity_get_first_texture (p_ent_lightrod_mold);
		// size is 8 x 8 x width of room minus something
		// Unless the room is north south.

		entityx_t *p_ent_new_lightrod = entitylist_add(plist_map);
		stringlistappend2 (&p_ent_new_lightrod->pairslist, "classname", "func_lightrod"); // key / val

		// add brush .. similar to origin
		qbool lightrod_bok = entity_brush_add_with_bbox_and_texture (p_ent_new_lightrod, xbrmins, xbrmaxs, lightrod_texturename);
		if (!lightrod_bok) return false;
	} // end lightrod

beyond_lightrod:
	// Each room as func_group_room .. give targetname room number

	// _layout_wall_north
	// odds .50;mod world_x 118 = 0

	return true;
}


//static qbool _floor_everything_entity_was_processed (entityx_t *d_ent)
//{
//	qbool was_processed = entity_update_bbox_from_brushes_was_processed (d_ent);
//	if (!was_processed)
//		return false; // No brushes (like "light" or a "_decal" which is a patch
//
//	vec3_t move_amount = {0};
//	// if the maxs is 500 we want to move -500
//	move_amount[2] = 0 - d_ent->bbemaxs[2]; // 0 - 500
//	if (move_amount[2] == 0)
//		return false; // Success but no move required
//
//	entitylist_translate_brushes		(pe, move_amount); // Has texture scaling.
//
//	RELATED_ (VM_map_move)
//
//	return true;
//}

RELATED_ (entitylist_translate_brushes VM_map_move)
int entitylist_perform_floor_everything_return_num_moved (entitylist_t *plist, ccs *s_floorvalue)
{
	float floor_level_wanted = atof(s_floorvalue);
	entitylist_metric_world_brushes_nova (plist);

	// WORLD:	move the brushes
	// ENTITY:	move the entity
	int num_moved = 0;

	// WORLD
	for (int ex = ENTITY_WORLD_0; ex == ENTITY_WORLD_0; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex]; // D = WRITE

		if (Vector3_IsZeros (d_ent->bedims))
			continue; // No measured brushes => no brushes ... keep moving

		RELATED_ (entitylist_translate_brushes) // Very similar

		// Translate EACH brush separately
		for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
			brush_s *d_brush = &d_ent->brushlist.brush[bx];

			vec3_t move_amount_add = {0}; // if the maxs is 500 we want to move -500 .. we are moving top down.
			move_amount_add[2] = floor_level_wanted - d_brush->bxmaxs[2]; // 0 - 500 = -500

			if (move_amount_add[2] == 0)
				continue; // No move needed

			qbool was_processed = brush_or_patch_tranlate_was_processed (d_brush, move_amount_add);
			if (was_processed)
				num_moved ++;
		} // brush
	} // entities in .map

	// ENTITIES: MOVE DOWN - we aren't doing pairs because ?
	for (int ex = AFTER_WORLD_1; ex < plist->count; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex]; // D = WRITE

		if (Vector3_IsZeros (d_ent->bedims))
			continue; // No measured brushes => no brushes ... keep moving

		vec3_t move_amount_add = {0};
		// if the maxs is 500 we want to move -500 .. we are moving top down.
		move_amount_add[2] = floor_level_wanted - d_ent->bemaxs[2]; // 0 - 500 = -500

		if (move_amount_add[2] == 0)
			continue; // No move needed

		RELATED_ (entitylist_translate_brushes) // Very similar

		// Translate entity brushes.
		for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
			brush_s *d_brush = &d_ent->brushlist.brush[bx];
			qbool was_processed = brush_or_patch_tranlate_was_processed (d_brush, move_amount_add);
			if (was_processed)
				num_moved ++;
		} // brush
	} // entities in .map

	return num_moved;
}

int entitylist_perform_layout_from_boxes (entitylist_t *plist_map)
{
	entityx_t	*p_ent_world = &plist_map->entity[ENTITY_WORLD_0];
	int num_processed = 0;

	entitylist_metric_world_brushes_nova (plist_map);

	ccs *modelname = entity_key_get_value (p_ent_world, "_layout_to_boxes_modelname");

	// "_layout_to_boxes_wall_width" "64"  (default)
	ccs *s_wall_width = entity_key_get_value (p_ent_world, "_layout_to_boxes_wall_width");
	float wall_width = s_wall_width ? atof(s_wall_width) : 64.0; // Default 64 (also floor_height)

	// "_layout_to_boxes_wall_width" "64"  (default)
	ccs *s_wall_height = entity_key_get_value (p_ent_world, "_layout_to_boxes_wall_height");
	float wall_height = s_wall_height ? atof(s_wall_height) : 160.0; // Default 160

	float box_total_height = wall_width /*floor*/ + wall_height + wall_width /*ceil*/;


#if 1
	int j = Sys_CheckParm("-csg");
	char mappath[MAX_OSPATH];
	PathWork_CSG (j, mappath, sizeof(mappath) ); // "C:/galaxy/zircon/maps/elder_quad_shrine.map" from sys.argv

	File_URL_Edit_Reduce_To_Parent_Path_No_Trailing_Slash (mappath); // "C:/galaxy/zircon/maps"
	c_strlcat (mappath, "/");  // "C:/galaxy/zircon/maps/"
	c_strlcat (mappath, modelname);  // "C:/galaxy/zircon/mapslayout_model_1"
	c_strlcat (mappath, ".map"); // "C:/galaxy/zircon/mapslayout_model_1.map"

	char *sin_model = FS_LoadFileString_Unsafe_ZAlloc (mappath);
	if (!sin_model) Perform_Exit_1_Error("sin_model FS_LoadFileString_Unsafe_ZAlloc failed"); // Couldn't open map

	entitylist_t list_model = {0};
	// PARSE THE MAP
	int isok = entitylist_parsemaptxt (&list_model, sin_model);
	if (isok == false) Perform_Exit_1_Error("list_model entitylist_parsemaptxt failed"); // failed to parse

	// This figures out the walls.
	entitylist_metric_world_brushes_nova			(&list_model);
	entity_layoutbox_identify_world_brush_quadrants (&list_model);

	int lightrod_num_or_neg1 = entitylist_find_key_value (&list_model, "classname", "func_lightrod");

	list_model.func_lightrod_entnum = lightrod_num_or_neg1;

	//entitylist_metric_world_brushes_with_walls (&list_model);
#endif

#if 0 // No we use it for something
	// textures/azirc0/brick_graystone ==> NO ==> azirc0/brick_graystone
	ccs *texturename = entity_key_get_value (p_ent_world, "_layout_to_boxes_texturename");
	if (!texturename)
		texturename = "common/trigger";
#endif

	int brush_list_count_at_start = p_ent_world->brushlist.count; // Prevent expansion
	int entity_count_at_start = plist_map->count; // Prevent expansion

	//_layout_to_boxes_texture
	for (int brush_idx = 0; brush_idx < brush_list_count_at_start; brush_idx ++) {
		brush_s *p_brush = &p_ent_world->brushlist.brush[brush_idx];

		if (p_brush->is_a_patch || !p_brush->brushrowlist.count)
			continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

		vec3_t brmins, brmaxs;
		qbool was_processed = brush_get_bbox_was_processed (p_brush, brmins, brmaxs);

		if (!was_processed)
			continue; // patch or something

		brmins[2] = 0; brmaxs[2] = box_total_height;

		qbool bok = entity_make_brush_bbox_with_texture2 (plist_map, &list_model, p_ent_world, brmins, brmaxs, wall_width, wall_height, box_total_height);
		if (!bok)
			Perform_Exit_1_Error ("entity_make_brush_bbox_with_texture2 failed");

		num_processed ++;
	} // each brush in world

	for (int ex = AFTER_WORLD_1; ex < entity_count_at_start; ex ++) {
		entityx_t	*p_ent = &plist_map->entity[ex];
		ccs *ent_classname = entity_key_get_value (p_ent, "classname");
		if (String_Match (ent_classname, "func_wall") == false)
			continue; // Not a func_wall


		qbool was_processed = entity_update_bbox_from_brushes_was_processed (p_ent);
		if (!was_processed)
			continue;

		vec3_t boxymins, boxymaxs;
		VectorCopyDestSrc (boxymins, p_ent->bemins);
		VectorCopyDestSrc (boxymaxs, p_ent->bemaxs);
		boxymins[2] = 0; boxymaxs[2] = box_total_height;

		qbool bok = entity_make_brush_bbox_with_texture2 (plist_map, &list_model, p_ent_world, boxymins, boxymaxs, wall_width, wall_height, box_total_height);
		if (!bok)
			Perform_Exit_1_Error ("entity_make_brush_bbox_with_texture2 failed");

		num_processed ++;
	} // entities that are "func_wall"

	entitylistfreecontents	(&list_model);
	Mem_FreeNull_ (sin_model);

	return num_processed;
}

RELATED_ (entity_make_brush_bbox_with_texture2 entity_make_brush_install_wall_entities)

#ifdef _DEBUG
static int num_dups_stopped = 0;
#endif

// ONLY SEND ENTITIES HERE WITH _heightmap value.
CALLERS_ (entitylist_perform_heightmap_num_alpha_brushes_added)
int entity_perform_heightmap_num_alpha_brushes_added (entityx_t *d_ent, float zlow, float zhigh, float zrange)
{
	qbool is_negative_range = zrange < 0;
	RELATED_ (entitylist_write_to_file)
	RELATED_ (VECTOR3_G entity_add_alpha_brushes)
	int num_added = 0;
	stringlist_t list_vertex_strings = {0}; // For entire entity.

	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		const brush_s *r_brush = &d_ent->brushlist.brush[bx];

		// IGNORE PATCHES
		if (r_brush->is_a_patch) continue;

		// Now determine quadrants for each row
		for (int facenum = 0; facenum < r_brush->brushrowlist.count; facenum ++) {
			const brushrow_s *r_face = &r_brush->brushrowlist.brushrow[facenum];

#if 0 // NO!  EVERY VERTEX.
			// IGNORE FACES WITH A COMMON TEXTURE "common/caulk" etc.
			if (String_Starts_With (r_face->pbrtexture, "common/")) continue;
#endif

#if 1
			stringlistappend_face_vertex_unique (&list_vertex_strings, r_face);
#else
			va_super (svectora, 128, VECTOR3_G, VECTOR3_SEND(r_face->a));
			qbool wasaddeda = stringlistappend_unique_didAdd (&list_vertex_strings, svectora);

			#ifdef _DEBUG
				if (wasaddeda == false)
					num_dups_stopped ++;
			#endif
			va_super (svectorb, 128, VECTOR3_G, VECTOR3_SEND(r_face->b));
			qbool wasaddedb = stringlistappend_unique_didAdd (&list_vertex_strings, svectorb);

			#ifdef _DEBUG
				if (wasaddedb == false)
					num_dups_stopped ++;
			#endif
			va_super (svectorc, 128, VECTOR3_G, VECTOR3_SEND(r_face->c));
			qbool wasaddedc = stringlistappend_unique_didAdd (&list_vertex_strings, svectorc);

			#ifdef _DEBUG
				if (wasaddedc == false)
					num_dups_stopped ++;
			#endif
#endif
		} // next face
	} // next brush

	for (int idx = 0; idx < list_vertex_strings.numstrings; idx ++) {
		char *sxy = list_vertex_strings.strings[idx];
		vec3_t point3d; Math_atov3 (sxy, point3d);
		vec3_t brmins, brmaxs;
		VectorCopyDestSrc (brmins, point3d);
		VectorCopyDestSrc (brmaxs, point3d);

		for (int d = 0; d < 3; d ++) {
			brmins[d] -= 48;
			brmaxs[d] += 48;
		} // for

		float f = (point3d[2] - zlow)/zrange; // (100 - 0) / 384
		if (is_negative_range) {
			f = bound (0, fabs(f), 1); // 384 - 384 // -384   0 - (384) -384
			if (f == 0) f = 0; // Get rid of stupid negative 0 problem.
			//f = 1 - f;
		} else {
			f = bound (0, f, 1);
			if (f == 0) f = 0; // Get rid of stupid negative 0 problem.
		}

		va_super (texturename, 128, "common/alpha_%1.2f", f);
		String_Edit_Replace (texturename, sizeof(texturename), ".", "_");
		//if (String_Contains (texturename, "-")) {
		//	int j = 5;
		//}
		entity_brush_add_with_bbox_and_texture (d_ent, brmins, brmaxs, texturename);

		num_added ++;
	} // for


	stringlistfreecontents (&list_vertex_strings);
	return num_added;
}

int entitylist_perform_heightmap_num_alpha_brushes_added (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	int num_made = 0;
	for (int ex = AFTER_WORLD_1; ex < plist->count; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];
		ccs			*_heightmap = entity_key_get_value (d_ent, "_heightmap");
		if (!_heightmap) continue; // NO _HEIGHTMAP

		if (String_Starts_With (_heightmap, "0 -")) continue; // Already pre-processed.

		va_super (_heightmap_copy, 256, "%s", _heightmap);
		String_Edit_Replace (_heightmap_copy, sizeof(_heightmap_copy), "to", "0");
		vec3_t vheightmap = {0};
		Math_atov3 (_heightmap_copy, vheightmap);
		float range = vheightmap[2] - vheightmap[0]; // Max 384 - mins 0

		int numadded =
			entity_perform_heightmap_num_alpha_brushes_added (d_ent, vheightmap[0], vheightmap[2], range);

		//qbool isok =
		entity_key_set_value_is_ok(d_ent, "_heightmap", s0_plus_timestamp);

		num_made += numadded;
	} // entities in .map

	return num_made;
}

// 1. All triangles with a normal of x get put into a group.
// 2. Search for triangles with (???) and turn them into
// 3. Triangle brushes with same Z min and same Z max and every point x,y,z, the x y or z matches a bbox corner.
// 4. Same bbox.

int brush_find_yz_match(entityx_t *r_ent, const vec3_t bbmins, const vec3_t bbmaxs, int start_idx, int beyond_idx, ccs *texturename)
{
	// p_ent_world->brushlist.count
	for (int brush_idx = start_idx; brush_idx < beyond_idx /*r_ent->brushlist.count*/; brush_idx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[brush_idx];
		if (r_brush->is_a_patch)					continue;
		if (r_brush->is_considered_deleted)			continue;

		if (! (r_brush->bxmins[1] == bbmins[1] ))	continue; // Not same Y min
		if (! (r_brush->bxmins[2] == bbmins[2] ))	continue; // Not same Z min
		if (! (r_brush->bxmaxs[1] == bbmaxs[1] ))	continue; // Not same Y max
		if (! (r_brush->bxmaxs[2] == bbmaxs[2] ))	continue; // Not same Z max

		ccs *mytext = brush_get_first_non_common_texture (r_brush);
		if (!mytext) continue; // No texture
		if (!String_Match (mytext, texturename))
			continue; // Wrong texture
		// full match
		return brush_idx;
	} // next brush.
	return not_found_neg1;
}

int brush_find_trianguler_with_xyz_match(entityx_t *r_ent, const vec3_t bbmins, const vec3_t bbmaxs, int ignore_brush_idx, int hard_limit)
{

	for (int brush_idx = 0; brush_idx < hard_limit /*r_ent->brushlist.count*/; brush_idx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[brush_idx];
		if (brush_idx == ignore_brush_idx)											continue;
		if (r_brush->brushrowlist.count != 5 /*triangle + top + bottom is 5*/)		continue;
		if (r_brush->is_a_patch)													continue;
		if (r_brush->is_considered_deleted)											continue;

		if (! (r_brush->bxmins[0] == bbmins[0] )) continue; // Not same X min
		if (! (r_brush->bxmins[1] == bbmins[1] )) continue; // Not same Y min
		if (! (r_brush->bxmins[2] == bbmins[2] )) continue; // Not same Z min
		if (! (r_brush->bxmaxs[0] == bbmaxs[0] )) continue; // Not same X max
		if (! (r_brush->bxmaxs[1] == bbmaxs[1] )) continue; // Not same Y max
		if (! (r_brush->bxmaxs[2] == bbmaxs[2] )) continue; // Not same Z max

		// full match

		return brush_idx;
	} // next brush.
	return not_found_neg1;
}

int brush_find_cuboid_with_xyz_rightof(entityx_t *r_ent, const vec3_t bbmins, const vec3_t bbmaxs, int start_idx, int beyond_idx, int ignore_idx)
{

	for (int brush_idx = start_idx; brush_idx < beyond_idx; brush_idx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[brush_idx];

		if (brush_idx == ignore_idx)												continue; // IGNORE
		if (r_brush->is_considered_deleted)											continue;
		if (r_brush->brushrowlist.count != 6 /*rect 4 sides + top + bottom is 6*/)	continue;
		if (r_brush->is_a_patch)													continue;

		// We are assuming these are flat right now ...

		if (! (r_brush->bxmins[0] == bbmaxs[0] )) continue; // RIGHTOF MATCH!

		if (! (r_brush->bxmins[1] == bbmins[1] )) continue; // Not same Y min
		if (! (r_brush->bxmins[2] == bbmins[2] )) continue; // Not same Z min

		if (! (r_brush->bxmaxs[1] == bbmaxs[1] )) continue; // Not same Y max
		if (! (r_brush->bxmaxs[2] == bbmaxs[2] )) continue; // Not same Z max

		// full match
		return brush_idx;
	} // next brush.
	return not_found_neg1;
}


brushrow_s *brush_get_face_containing_caseless (brush_s *r_brush, ccs *findme_substring)
{
	for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &r_brush->brushrowlist.brushrow[brow];

		if (String_Contains_Caseless(pbr->pbrtexture, findme_substring))
			return pbr;
	} // next face
	return NULL;
}

// Only returns triangular and rectangular brushes with a single upside
// Requires a flat bottom.
// Requires real brush.
brushrow_s *brush_get_face_single_upward_tri_or_quad (brush_s *r_brush)
{
	if (!isin2 (r_brush->brushrowlist.count, 5, 6)) // Baker: this should excluded patches too.
		return NULL; // tri (3+ 2) or quad (4 + 2) are only eligible brush shapes

	brushrow_s *candidate = NULL;

	int num_upwards_faces = 0;
	int num_flatty = 0;

	for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &r_brush->brushrowlist.brushrow[brow];

		vec3_t pba;		VectorSubtract (pbr->b, pbr->a, pba);
		vec3_t pca;		VectorSubtract (pbr->c, pbr->a, pca);
		vec3_t cross;	CrossProduct (pba, pca, cross);

		qbool is_flatty = (pbr->a[2] == pbr->b[2] && pbr->b[2] == pbr->c[2]);

		if (is_flatty) num_flatty ++;


		if (cross[2] < 0) {
			// This one is facing up.
			num_upwards_faces ++;
			candidate = pbr;
		}
	} // row

	if (num_upwards_faces == 1 && num_flatty >= 1)
		return candidate;
	return NULL; // Multiple faces that are facing up.

}

qbool face_is_up_or_down (brushrow_s *face)
{
	vec3_t pba; VectorSubtract (face->b, face->a, pba);
	vec3_t pca; VectorSubtract (face->c, face->a, pca);
	vec3_t cross; CrossProduct (pba, pca, cross);
	//Example: vectornormal3 -9984 1280 276 -9984 1280 -1772 -9728 1536 84 is a side which has 0 crossproduct Z

	if (cross[2] != 0)
		return false; // SIDE
	return true;
}

#define brush_is_flat_top_get_face(a) brush_get_face_facing_up_flat_only(a) // See: brush_get_face_single_upward_tri_or_quad

qbool face_is_flat (brushrow_s *pbr)
{
	qbool is_flatty = (pbr->a[2] == pbr->b[2] && pbr->b[2] == pbr->c[2]);
	return is_flatty;
}

brushrow_s *brush_get_face_facing_up_flat_only (brush_s *r_brush)
{
	brushrow_s *candidate = NULL;
	// REAL BRUSH
	int num_upwards_faces = 0;
	int num_flatty = 0;
//	if (r_brush->brushrowlist.count != num_faces_required)
//		return NULL;

	for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &r_brush->brushrowlist.brushrow[brow];

		vec3_t pba; VectorSubtract (pbr->b, pbr->a, pba);
		vec3_t pca; VectorSubtract (pbr->c, pbr->a, pca);

		vec3_t cross; CrossProduct (pba, pca, cross);

		qbool is_flatty = (pbr->a[2] == pbr->b[2] && pbr->b[2] == pbr->c[2]);

		if (is_flatty) {
			num_flatty ++;
			//int kj = 5;
		}

		//// Not a flatty.
		//if (!is_flatty)
		//	continue;

		int is_up_brush = false;
		//#pragma message ("What is the cross for a flat one?")
		// Looks like 0 0 and -53632 with the calc we did.
		// Can't say if we didn't reverse it or something
		// Or do the orientation wrong CW vs. CCW
		if (cross[2] < 0) {
			// This one is facing up.
			is_up_brush = true;
			num_upwards_faces ++;
			//return pbr;
			candidate = pbr;
		}
	} // row

	if (num_upwards_faces == 1 && num_flatty == 2)
		return candidate;
	return NULL; // Multiple faces that are facing up.
}

int entity_perform_weld_num_added (entityx_t *d_ent, ccs *most_common)
{
	// 1. 5 faces - triangle + top + bottom.
	// 2. Every x, y, z matches a bbox corner x,y,z coordate.
	// 3. Create a string index 7d ... space 6d zmins space 6d zmaxs. so string + 8 is z identification.
	// 4. If bboxes are SAME for any 2 triangles.  Merge them.
	int num_processed = 0;
	stringlist_t list_row_yz_min_maxes = {0};
	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &d_ent->brushlist.brush[bx];

		// ONLY TRIANGULAR BRUSHES.
		if (BRUSH_IS_5_SIDED_TRI(r_brush) == false)
			continue;

		//brushrow_s *r_face_asphalt = brush_get_face_containing_caseless (r_brush, "asphalt");

		// We want brushes with only a single flat top

		brushrow_s *flat_face = brush_get_face_facing_up_flat_only(r_brush);
		if (flat_face == NULL)
			continue; // Not a flat top
		if (!String_Match (flat_face->pbrtexture, most_common))
			continue; // Not the most common texture .. ignore


		va_super (s, 256, "%g %g %g %g",
			r_brush->bxmins[1], r_brush->bxmaxs[1], r_brush->bxmins[2], r_brush->bxmaxs[2]);

		//if (r_face_asphalt) {
		//	Con_PrintLinef ("Asphalt detected %s list count = %d", s, list_row_yz_min_maxes.numstrings);
		//}
		stringlistappend_unique_didAdd (&list_row_yz_min_maxes, s);
	} // next brush

//	stringlistsort_no_unique (&list_row_yz_min_maxes);
	Con_PrintLinef ("Dumping list_row_yz_min_maxes ...");
	stringlist_condump_with_linenums (&list_row_yz_min_maxes);

	// Work every list item.
	int hard_limit = d_ent->brushlist.count;
	for (int idx = 0; idx < list_row_yz_min_maxes.numstrings; idx++) {
		// OH ... we need to for loop this ...
		ccs *sxy =  list_row_yz_min_maxes.strings[idx]; // miny maxy minz maxz

		//if (String_Match (sxy, "1869.91 2226.09 -64 208")) {
		//	int j = 555;
		//}
		vec4_t v4split = {0};
		Math_atov4 (sxy, v4split);
		vec3_t bbmins = { 0, v4split[0], v4split[2]};
		vec3_t bbmaxs = { 0, v4split[1], v4split[3]};


		int start_idx = 0;
		int after_idx = d_ent->brushlist.count;
		while (1) {
			// Now find a brush matching this one.
			int brush_idx1 = brush_find_yz_match(d_ent, bbmins, bbmaxs, start_idx, after_idx, most_common);


			if (brush_idx1 == not_found_neg1) {
				break; // NO MORE
			}

			start_idx = brush_idx1 + 1; // For next iter
			brush_s *d_brush1 = &d_ent->brushlist.brush[brush_idx1];
//			vec3_t bbmins2 = {0};
			if (!d_brush1) {
				Con_PrintLinef ("Couldn't find brush yz matching mins: " VECTOR3_G " maxs: " VECTOR3_G,
					VECTOR3_SEND(bbmins),VECTOR3_SEND(bbmaxs) );
				continue; // Impossible, right?
			} else {
//				VectorCopyDestSrc (bbmins2, d_brush1->bxmins);
//				int jj = 444;
			}

			// It might not be a flat one, check eligibility
			brushrow_s *flat_face = brush_get_face_facing_up_flat_only(d_brush1);

			if (!flat_face)
				continue; // NOT ELIGIBILE BECAUSE NOT FLAT

			//brushrow_s *r_face_asphalt = brush_get_face_containing_caseless (d_brush1, "asphalt");

			//if (r_face_asphalt) {
			//	int j = 5;
				// This better find a friend
			//}

			// We have a brush.  Find any other brush

			int brush_idx2 = brush_find_trianguler_with_xyz_match(d_ent, d_brush1->bxmins, d_brush1->bxmaxs, brush_idx1, hard_limit);
			//is it a flatty?
			//brush_s *d_brush2 =

			//Our brush references are dangerous because page array!  On realloc, they fail.

			if (brush_idx2 == not_found_neg1) {
				// We don't always have a winner.
				continue;
			}

			// Kill both
			brush_s *d_brush2 = &d_ent->brushlist.brush[brush_idx2];
			d_brush1->is_considered_deleted = true;
			d_brush2->is_considered_deleted = true;

			vec3_t bxmins, bxmaxs;
			VectorCopyDestSrc (bxmins, d_brush1->bxmins);
			VectorCopyDestSrc (bxmaxs, d_brush1->bxmaxs);
				//if (Vector3_IsZeros(bxmins) && Vector3_IsZeros(bxmaxs)) {
				//	int j = 55;
				//}

			int newidx = entity_brush_add_with_bbox_and_texture_return_idx (d_ent, /*d_brush1->*/bxmins, /*d_brush1->*/bxmaxs, "common/up1");
			if (newidx != not_found_neg1) {
				brush_s *n_brush = &d_ent->brushlist.brush[newidx];
				brush_update_bbox_from_rows_was_processed (n_brush);
				//if (Vector3_IsZeros(n_brush->bxmins) && Vector3_IsZeros(n_brush->bxmaxs)) {
				//	int j = 55;
				//}
			} else {
				//int j = 5;
			}


			num_processed ++;
		} // brush loop
	} // next list item

	stringlistfreecontents (&list_row_yz_min_maxes);

	// Now unite any xH1 == xL2  Y match Z match new brushes, making oldies as delete.
	//int tri_count = d_ent->brushlist.count;
	for (int brush_idx1 = 0; brush_idx1 < d_ent->brushlist.count; brush_idx1 ++) {
		brush_s *d_brush1 = &d_ent->brushlist.brush[brush_idx1];

		if (d_brush1->is_considered_deleted)		continue;	// REMOVED
		if (d_brush1->is_a_patch)				continue;	// PATCH, SKIP - probably can't happen
		if (d_brush1->brushrowlist.count != 6)	continue;	// NOT CUBEOID - probably can't happen

		// Assume this brush makes sense.
		// Now find a brush next to it.  And repeat.

		vec3_t bxmins, bxmaxs;
		VectorCopyDestSrc (bxmins, d_brush1->bxmins);
		VectorCopyDestSrc (bxmaxs, d_brush1->bxmaxs);

		//if (Vector3_IsZeros(bxmins) && Vector3_IsZeros(bxmaxs)) {
		//	int j = 5;
		//}

		int start_idx = hard_limit + 1;
		int beyond_idx = d_ent->brushlist.count;
		int num_brush_combines = 0;
		while (1) {
			int brush_idx2 = brush_find_cuboid_with_xyz_rightof (d_ent, bxmins, bxmaxs, start_idx, beyond_idx, brush_idx1);
			if (brush_idx2 == not_found_neg1)
				break;

			start_idx = brush_idx2;

			// Need to expand.
			brush_s *d_brush2 = &d_ent->brushlist.brush[brush_idx2];
			d_brush1->is_considered_deleted = true;
			d_brush2->is_considered_deleted = true;
			bxmaxs[0] = d_brush2->bxmaxs[0]; // EXTEND
			num_brush_combines ++;

			Con_PrintLinef ("brushidx %d num_combines %d: Extended a rect: "
				"new bbox = " VECTOR3_G " maxs " VECTOR3_G,
				brush_idx1,
				num_brush_combines,
				VECTOR3_SEND(bxmins), VECTOR3_SEND(bxmaxs)
			);

			// RESET - do not presume an order.
			start_idx = hard_limit + 1;
		} // while.

		if (num_brush_combines) {
			int newidx2 = entity_brush_add_with_bbox_and_texture_return_idx (d_ent, /*d_brush1->*/bxmins, /*d_brush1->*/bxmaxs, "common/caulk");
			brush_s *n_brush_cube = &d_ent->brushlist.brush[newidx2];
			brush_update_bbox_from_rows_was_processed (n_brush_cube);
			brushrow_s *flat_face = brush_get_face_facing_up_flat_only(n_brush_cube);
			if (flat_face) {
				Mem_FreeNull_ (flat_face->pbrtexture);
				flat_face->pbrtexture = Z_StrDup ("common/up2");
			}

			//if (Vector3_IsZeros(n_brush_cube->bxmins) && Vector3_IsZeros(n_brush_cube->bxmaxs)) {
			//	int j = 5;
			//}
			// Find upface

		}
		// Delete brush
	} //

	return num_processed;
}

// WELD checks only dominant texture.
int entitylist_perform_weld_num_done (entitylist_t *plist)
{

	int num_processed_total = 0;
	entityx_t	*p_ent_world = &plist->entity[ENTITY_WORLD_0];
	stringlist_t list_textures = {0};
	stringlist_append_entity_textures (&list_textures, p_ent_world);

	ccs *most_common = stringlist_find_most_common_index (&list_textures, "common/caulk");



	Con_PrintLinef ("Dominant texture is %s", most_common);

	Con_PrintLinef ("Textures ...");
	stringlistsort_unique (&list_textures);
	stringlist_condump_with_linenums (&list_textures);

	//Con_PrintLinef ("Considering ONLY brushes from all entities using dominant texture ...");

	Con_PrintLinef ("Processing each texture ...");
	for (int sidx = 0; sidx < list_textures.numstrings; sidx++) {
		char *sxy = list_textures.strings[sidx];

		for (int ex = 0; ex < plist->count; ex ++) {
			entityx_t	*d_ent = &plist->entity[ex];
			Con_PrintLinef ("Texture %d of %d for Entity #%d ...", sidx, list_textures.numstrings, ex);
			int num_done = entity_perform_weld_num_added (d_ent, sxy /*most_common*/);
			if (num_done)
				num_processed_total += num_done;
		} // entities in .map
	} // each stringlist item

	stringlistfreecontents (&list_textures);
	return num_processed_total;
}


#pragma message ("Consider _sky_encase arg3 indicating chunksize like 4096 divisions")
int entitylist_perform_sky_encase_brushes_added (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	#define INTEREST_STRING "_sky_encase"
	int num_made = 0;
	float wall_thickness_16_0 = 16.0;
#define TESS_SIZE_HERE 16384

	for (int ex = ENTITY_WORLD_0; ex < ENTITY_WORLD_0 + 1; ex ++) { // WORLD ONLY
		entityx_t	*d_ent = &plist->entity[ex];
		ccs			*_sky_encase = entity_key_get_value (d_ent, INTEREST_STRING);
		if (!_sky_encase) continue; // NO _HEIGHTMAP

		if (String_Starts_With (_sky_encase, "0 -")) {
			Con_PrintLinef (INTEREST_STRING " already ran with value of 0 .. skipping phase");
			continue; // Already pre-processed.
		}

		char s0_wallheight[1024], s1_texturename[Q3PATHLENGTH_64];
		qbool isok = arch2_space_split_isok (_sky_encase, s0_wallheight, s1_texturename);

		if (!isok) {
			Con_PrintLinef ("Entity %d had " INTEREST_STRING " could not parse to 2 strings .. defaulting desert2 8192.." QUOTED_S,
				ex,
				_sky_encase
			);
			//continue; // ignore?

			c_strlcpy (s0_wallheight, "8192");
			c_strlcpy (s1_texturename, "skies/desert2_sky");
			//8192 skies/desert2_sky
		}

		float wall_height = atof(s0_wallheight);
		ccs *texturename = s1_texturename; //

		// Ok now do 6 sider.
		// AVOIDING PAGE ARRAY.
		vec3_t brmins, brmaxs;

		VectorCopy (plist->bbentsmins, brmins);
		VectorCopy (plist->bbentsmaxs, brmaxs);

		vec3_t brmins_side, brmaxs_side;
		float low_z = 0;

		if (brmins[2] < 0)
			low_z = brmins[2]; // -400 ==>  FLOOR SIDE OF ENCASE

		// N
		VectorSet (brmaxs_side, brmaxs[0], brmaxs[1] + wall_thickness_16_0, wall_height);
		VectorSet (brmins_side, brmins[0], brmaxs[1] + 0, low_z );
	//quadrant_west_0,
	//quadrant_east_1,
	//quadrant_south_2,
	//quadrant_north_3,
	//quadrant_floor_4,
	//quadrant_ceiling_5,
		entity_brushs_add_size_texture_quadrant_num_done (d_ent, quadrant_south_2, texturename, brmins_side, brmaxs_side, TESS_SIZE_HERE, "common/caulk");

		// S
		VectorSet (brmaxs_side, brmaxs[0], brmins[1] - 0, wall_height);
		VectorSet (brmins_side, brmins[0], brmins[1] - wall_thickness_16_0, low_z );
		entity_brushs_add_size_texture_quadrant_num_done (d_ent, quadrant_north_3, texturename, brmins_side, brmaxs_side, TESS_SIZE_HERE, "common/caulk");
		// W
		VectorSet (brmaxs_side, brmins[0] - 0, brmaxs[1], wall_height); // MAX
		VectorSet (brmins_side, brmins[0] - wall_thickness_16_0, brmins[1], low_z );
		entity_brushs_add_size_texture_quadrant_num_done (d_ent, quadrant_east_1, texturename, brmins_side, brmaxs_side, TESS_SIZE_HERE, "common/caulk");

		// E
		VectorSet (brmaxs_side, brmaxs[0] + wall_thickness_16_0, brmaxs[1], wall_height); // MAX
		VectorSet (brmins_side, brmaxs[0] + 0, brmins[1], low_z );
		entity_brushs_add_size_texture_quadrant_num_done (d_ent, quadrant_west_0, texturename, brmins_side, brmaxs_side, TESS_SIZE_HERE, "common/caulk");

		// UP
		VectorSet (brmaxs_side, brmaxs[0] , brmaxs[1], wall_height + wall_thickness_16_0); // MAX
		VectorSet (brmins_side, brmins[0] , brmins[1], wall_height );
		entity_brushs_add_size_texture_quadrant_num_done (d_ent, quadrant_floor_4, texturename, brmins_side, brmaxs_side, TESS_SIZE_HERE, "common/caulk");

		// DOWN
		VectorSet (brmaxs_side, brmaxs[0] , brmaxs[1], low_z ); // MAX
		VectorSet (brmins_side, brmins[0] , brmins[1], low_z - wall_thickness_16_0 );
		entity_brushs_add_size_texture_quadrant_num_done (d_ent, quadrant_ceiling_5, texturename, brmins_side, brmaxs_side, TESS_SIZE_HERE, "common/caulk");

		qbool isok2 __ATTRIBUTE_USED__ = entity_key_set_value_is_ok(d_ent, INTEREST_STRING, s0_plus_timestamp);

		num_made += 1;
	} // entities in .map

	#undef INTEREST_STRING
	return num_made;
}

//int brush_perform_adjacent_blend_num_done (brush_s *d_brush)
//{
//
//}

//brushrow_s *t_face = brush_get_face_single_upward_tri_or_quad(d_brush);
//#define BRUSH_IS_5_SIDED(pbrush) (pbrush->brushrowlist.count == (3 + 2))
int brush_is_adjacent (brush_s *b1, brushrow_s *face1, brush_s *b2)
{
	//  The top should share a mins/maxs
	//brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(pb2);
	//int is_tri = (b1->brushrowlist.count == (3 + 2));
	if (BRUSH_IS_5_SIDED_TRI(b1) && BRUSH_IS_5_SIDED_TRI(b2)) {
		if (Vector3Compare(b1->bxmins, b2->bxmins) && Vector3Compare(b1->bxmaxs, b2->bxmaxs)) {
			return quadrant_triangle_friend_7;
		}
	}

	qbool is_adjacent = false;
	for (int a = 0; a < b1->brushrowlist.count; a ++) {
		brushrow_s *pbr1 = &b1->brushrowlist.brushrow[a];

		for (int b = 0; b < b2->brushrowlist.count; b ++) {
			brushrow_s *pbr2 = &b2->brushrowlist.brushrow[b];
			if (Vector3Compare(pbr1->bfmins, pbr2->bfmins) &&  Vector3Compare(pbr1->bfmins, pbr2->bfmins)) {
				is_adjacent = true;
				return quadrant_adjacent_any_8;
			}
		} // a
	} // a


	//NOT_MISSING_ASSIGN (pnum_diff_adjacents, num_different_adjacents);

	return quadrant_none_neg1;
}

// This is shared min/max for a face
// And we are hitting diagonal adjacents + triangle friends.
qbool brush_is_adjacent_with_brush_with_diags (brush_s *pb1, brush_s *pb2)
{
	//  The top should share a mins/maxs
	//brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(pb2);
	//int is_tri = (pb1->brushrowlist.count == (3 + 2));

	qbool is_adjacent = false;
	//int num_adjac
	for (int a = 0; a < pb1->brushrowlist.count; a ++) {
		brushrow_s *pbr1 = &pb1->brushrowlist.brushrow[a];

		if (face_is_up_or_down(pbr1))
			continue; // We only want sides.

		for (int b = 0; b < pb2->brushrowlist.count; b ++) {
			brushrow_s *pbr2 = &pb2->brushrowlist.brushrow[b];

			if (face_is_up_or_down(pbr2))
				continue; // We only want sides.

			if (Vector3Compare(pbr1->bfmins, pbr2->bfmins) &&  Vector3Compare(pbr1->bfmaxs, pbr2->bfmaxs)) {
				is_adjacent = true;
				//num_different_adjacents ++;
			}
		} // a
	} // a

	//NOT_MISSING_ASSIGN (pnum_diff_adjacents, num_different_adjacents);

	return is_adjacent;
}

qbool brush_has_a_north_face (const brush_s *b)
{
	for (int brow = 0; brow < b->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &b->brushrowlist.brushrow[brow];
		if (b->bxmaxs[1] == pbr->bfmins[1]) // This face is northmost
			if (pbr->bfdims[1] == 0) // This face is X flat |------- |
				return true;
	}
	return false;
}

qbool brush_has_a_south_face (const brush_s *b)
{
	for (int brow = 0; brow < b->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &b->brushrowlist.brushrow[brow];
		if (b->bxmins[1] == pbr->bfmaxs[1]) // This face is southmost
			if (pbr->bfdims[1] == 0) // This face is X flat |------- |
				return true;
	}
	return false;
}

qbool brush_has_a_west_face (const brush_s *b)
{
	for (int brow = 0; brow < b->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &b->brushrowlist.brushrow[brow];
		if (b->bxmins[0] == pbr->bfmaxs[0]) // This face is westmost
			if (pbr->bfdims[0] == 0) // This face is Y flat |------- |
				return true;
	}
	return false;
}

qbool brush_has_a_east_face (const brush_s *b)
{
	for (int brow = 0; brow < b->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &b->brushrowlist.brushrow[brow];
		if (b->bxmaxs[0] == pbr->bfmins[0]) // This face is westmost
			if (pbr->bfdims[0] == 0) // This face is Y flat |------- |
				return true;
	}
	return false;
}

// Any same face neighbor.
quadrant6_e brush_is_adjacent_with_face_match(brush_s *a, brush_s *b)
{
	//  The top should share a mins/maxs
	//brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(pb2);
	//int is_tri = (pb1->brushrowlist.count == (3 + 2));
	if (Vector3Compare (a->bxmins, b->bxmins) && Vector3Compare (a->bxmaxs, b->bxmaxs))
		return quadrant_triangle_friend_7;

	if (a->bxmins[1] == b->bxmins[1] && a->bxmaxs[1] == b->bxmaxs[1]) {
		// SAME Y mins/maxs
		if (a->bxmins[0] == b->bxmaxs[0]) {
			if (!brush_has_a_east_face (b))
				return quadrant_none_neg1; // Diag
			return quadrant_west_0; // b is west of a
		}
		if (a->bxmaxs[0] == b->bxmins[0]) {
			if (!brush_has_a_west_face (b))
				return quadrant_none_neg1; // Diag

			return quadrant_east_1; // b is east of a
		}

		return quadrant_none_neg1;
	}

	if (a->bxmins[0] == b->bxmins[0] && a->bxmaxs[0] == b->bxmaxs[0]) {
		// SAME X mins/maxs
		if (a->bxmins[1] == b->bxmaxs[1]) {
			if (!brush_has_a_north_face (b))
				return quadrant_none_neg1; // Diag

			return quadrant_south_2; // b is south of a
		}
		if (a->bxmaxs[1] == b->bxmins[1]) {
			if (!brush_has_a_south_face (b))
				return quadrant_none_neg1; // Diag

			return quadrant_north_3; // b is north of a
		}
		return quadrant_none_neg1;
	}

	// How to eliminate the diag matches?
	// The wanted ones have a top face with 2 points of commonality if truly adjacent.

	return quadrant_none_neg1;
}

// Something is not quite right with this one
quadrant6_e brush_is_adjacent_with_face_match_diag(brush_s *a, brush_s *b)
{
	//  The top should share a mins/maxs
	if (Vector3Compare (a->bxmins, b->bxmins) && Vector3Compare (a->bxmaxs, b->bxmaxs))
		return quadrant_triangle_friend_7;

	if (a->bxmins[1] == b->bxmins[1] && a->bxmaxs[1] == b->bxmaxs[1]) {
		// SAME Y mins/maxs
		if (a->bxmins[0] == b->bxmaxs[0]) {
			if (!brush_has_a_east_face (b))
				return quadrant_diag_adjacent_any_9; // Diag
			return quadrant_west_0; // b is west of a
		}
		if (a->bxmaxs[0] == b->bxmins[0]) {
			if (!brush_has_a_west_face (b))
				return quadrant_diag_adjacent_any_9; // Diag

			return quadrant_east_1; // b is east of a
		}

		return quadrant_none_neg1;
	}

	if (a->bxmins[0] == b->bxmins[0] && a->bxmaxs[0] == b->bxmaxs[0]) {
		// SAME X mins/maxs
		if (a->bxmins[1] == b->bxmaxs[1]) {
			if (!brush_has_a_north_face (b))
				return quadrant_diag_adjacent_any_9; // Diag

			return quadrant_south_2; // b is south of a
		}
		if (a->bxmaxs[1] == b->bxmins[1]) {
			if (!brush_has_a_south_face (b))
				return quadrant_diag_adjacent_any_9; // Diag

			return quadrant_north_3; // b is north of a
		}
		return quadrant_none_neg1;
	}

	// How to eliminate the diag matches?
	// The wanted ones have a top face with 2 points of commonality if truly adjacent.

	return quadrant_none_neg1;
}


// NOTE: > instead of >= ... equals is a TOUCH HIT
qbool BOX_XY_Touch (vec3_t aL, vec3_t aH, vec3_t bL, vec3_t bH)
{
	//if (a->left >= RECT_RIGHTOF(*target))  return false; // a_x beyond width (cull)
	//if (a->top  >= RECT_BOTTOMOF(*target)) return false; // a_y beyond height (cull)
	if (aL[0] > bH[0]) return false;
	if (aL[1] > bH[1]) return false;
	//if (RECT_RIGHTOF(*a)  <= target->left) return false; // x2 doesn't reach 0 (cull)
	//if (RECT_BOTTOMOF(*a) <= target->top)  return false; // x2 doesn't reach 0 (cull)
	if (aH[0] < bL[0]) return false;
	if (aH[1] < bL[1]) return false;
	return true; // Touch!
}

qbool BOX_XY_Collide (vec3_t aL, vec3_t aH, vec3_t bL, vec3_t bH)
{
	//if (a->left >= RECT_RIGHTOF(*target))  return false; // a_x beyond width (cull)
	//if (a->top  >= RECT_BOTTOMOF(*target)) return false; // a_y beyond height (cull)
	if (aL[0] >= bH[0]) return false;
	if (aL[1] >= bH[1]) return false;
	//if (RECT_RIGHTOF(*a)  <= target->left) return false; // x2 doesn't reach 0 (cull)
	//if (RECT_BOTTOMOF(*a) <= target->top)  return false; // x2 doesn't reach 0 (cull)
	if (aH[0] <= bL[0]) return false;
	if (aH[1] <= bL[1]) return false;
	return true; // Collision!
}

qbool brush_xy_touches_brush (brush_s *a, brush_s *b)
{
	qbool does_touch = BOX_XY_Touch(a->bxmins, a->bxmaxs, b->bxmins, b->bxmaxs);
	return does_touch;
}

qbool face_bbox_touch_point(brushrow_s *f1, vec3_t p)
{
	qbool does_touch = BOX_XY_Touch(f1->bfmins, f1->bfmaxs, p, p);
	return does_touch;
}

qbool face_bbox_touch_face(brushrow_s *f1, brushrow_s *f2)
{
	qbool does_touch = BOX_XY_Touch(f1->bfmins, f1->bfmaxs, f2->bfmins, f2->bfmaxs);
	return does_touch;
}

ccs *quadrant_to_string (quadrant6_e q)
{
	switch (q) {
	case quadrant_none_neg1: return "quadrant_none_neg1";

	case quadrant_west_0: return "quadrant_west_0";
	case quadrant_east_1: return "quadrant_east_1";
	case quadrant_south_2: return "quadrant_south_2";
	case quadrant_north_3: return "quadrant_north_3";
	case quadrant_floor_4: return "quadrant_floor_4";
	case quadrant_ceiling_5: return "quadrant_ceiling_5";
	//case quadrant_MAXCOUNT_6, // Used to dim arrays only.
	case quadrant_triangle_friend_7: return "quadrant_triangle_friend_7";
	case quadrant_adjacent_any_8: return "quadrant_adjacent_any_8";
	}
	return "NOT FOUND";


}



RELATED_ (SCR_csgtool_f brush_get_face_single_upward_tri_or_quad)
int entity_perform_pincommonterrain_blend_num_done (entityx_t *d_ent, int entnum)
{
	int num_made = 0;

	if (!d_ent->brushlist.count) return 0; // No brushes.

	stringlist_t list_instructions = {0}; // what to do to each brush
	stringlist_t list_textures = {0}; // what to do to each brush

	// ACCUMULATE LIST OF ALL TEXTURES in FUNC_GROUP
	for (int brush_idx = 0; brush_idx < d_ent->brushlist.count; brush_idx ++) {
		brush_s *b1 = &d_ent->brushlist.brush[brush_idx];
		brushrow_s *t_face = brush_get_face_single_upward_tri_or_quad(b1);

		if (!t_face) {
			Con_PrintLinef ("Entity %4d brush %4d is reported as not having a single upward face", entnum, brush_idx);
			//brushrow_s *t_face2 = brush_get_face_single_upward_tri_or_quad(b1);
			continue; // I don't think this is supposed to happen
		}

		if (String_Match(t_face->pbrtexture, "common/caulk"))
			continue; // Hope this doesn't happen - means up facing brush is caulk.

		if (String_Contains(t_face->pbrtexture, "common/"))
			continue; // Like common/terrain .. means full blend mode.

		stringlistappend_unique_didAdd (&list_textures, t_face->pbrtexture);
	}

	stringlistsort_no_unique (&list_textures);
	stringlist_condump_with_linenums (&list_textures);

	// 1. Identify texture areas (NOT HERE ... use common/terrain or make a blend texture"
	// 2. Identify all relevant zmax verts.
	// 3. For each point, calculate a percent using texture a / num_touches where num_touches is exclusively a and b textures
	ccs *texture_wanted = "common/terrain";
	// For each brush print # adjacents
	stringlist_t list_vertex = {0}; // What does something like this .. heightmap
	RELATED_ (entity_perform_heightmap_num_alpha_brushes_added)
	for (int bidx = 0; bidx < d_ent->brushlist.count; bidx ++) {
		brush_s *a = &d_ent->brushlist.brush[bidx];
		brushrow_s *face = brush_get_face_single_upward_tri_or_quad(a);
		if (!face || !String_Match(face->pbrtexture, texture_wanted))
			continue; // Don't want

		stringlistappend_face_vertex_unique (&list_vertex, face);
	} //

	stringlistsort_unique (&list_vertex);
	stringlist_condump_with_linenums (&list_vertex);


	for (int idx = 0; idx < list_vertex.numstrings; idx ++) {
		char *sxy = list_vertex.strings[idx];

		float f = 0.33;
		entity_brush_add_vstr_alpha_with_texturename (d_ent, sxy, f);

		num_made ++;
	} // for


	stringlistfreecontents (&list_vertex);



#if 0
	// For each brush print # adjacents
	for (int bidx = 0; bidx < d_ent->brushlist.count; bidx ++) {
		brush_s *a = &d_ent->brushlist.brush[bidx];
		//brushrow_s *t_face = brush_get_face_single_upward_tri_or_quad(b1);
		int num_adjacent = 0;
		for (int bidx2 = 0; bidx2 < d_ent->brushlist.count; bidx2 ++) {
			brush_s *b = &d_ent->brushlist.brush[bidx2];
			if (a == b)
				continue;

			quadrant6_e q = brush_is_adjacent_with_brush2(a,b);

			if (q == quadrant_none_neg1)
				continue;

			////if (bidx == 58 && bidx2 == 51) {
			////	int j = 5;
			////}

			brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(b);
			//if (bidx == 58 && face2 && String_Contains (face2->pbrtexture, "1red")) {
			//	int j = 5;
			//}

			//if (!brush_is_adjacent_with_brush(b1,b2))
			//	continue; // not adjacent

			Con_PrintLinef ("Ent %5d Brush %5d: %d %s (brush %5d - quadrant %s)",
				entnum,
				bidx,
				num_adjacent,
				face2->pbrtexture,
				bidx2,
				quadrant_to_string (q)
			);
			num_adjacent ++;
		} // each brush against b1
	} // each brush in ent
#endif
	stringlistfreecontents (&list_textures);
	stringlistfreecontents (&list_instructions);

	return num_made;
}

RELATED_ (SCR_csgtool_f brush_get_face_single_upward_tri_or_quad)
int entitylist_perform_pincommonterrain_num_done (entitylist_t *plist)
{
	int num_done = 0;
	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) { // WORLD ONLY
		entityx_t	*d_ent = &plist->entity[ex];
		int num_done_ent = entity_perform_pincommonterrain_blend_num_done(d_ent, ex);
		num_done += num_done_ent;
	} // next entity
	return num_done;
}

void face_replace_texture (brushrow_s *pbr, ccs *sreplace)
{
	Mem_FreeNull_ (pbr->pbrtexture);
	pbr->pbrtexture = Z_StrDup (sreplace);
}

// Remove "noflat" and substitute "flat"
int entitylist_perform_noflat_to_flat_num_done (entitylist_t *plist)
{
	stringlist_t list = {0};
	int num_done = 0;
	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];
		for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
			brush_s *a = &d_ent->brushlist.brush[bx];
			brushrow_s *pbr = brush_get_face_single_upward_tri_or_quad(a);
			if (!pbr)
				continue; // Not eligible due to some reason.  Wrong shape or something.

			qbool is_flat = face_is_flat(pbr);
			qbool is_wanted =  String_Ends_With (pbr->pbrtexture, "_noflat") ||
				String_Ends_With (pbr->pbrtexture, "_noflat_trim") ||
				String_Ends_With (pbr->pbrtexture, "_flat_trim") ||
				String_Ends_With (pbr->pbrtexture, "_flat");

			if (!is_wanted)
				continue; // Not a "no_flat" class of texture

			stringlistappend_unique_didAdd (&list, pbr->pbrtexture);

			// Construct the basename
			va_super (basename, Q3PATHLENGTH_64, "%s", pbr->pbrtexture);
			if (!String_Edit_Remove_Trailing_Text (basename, "_noflat"))
				if (!String_Edit_Remove_Trailing_Text (basename, "_flat"))
					if (!String_Edit_Remove_Trailing_Text (basename, "_flat_trim"))
						String_Edit_Remove_Trailing_Text (basename, "_noflat_trim");

			int flat_neighbors = 0; int nonflat_neighbors = 0;
			for (int bidx2 = 0; bidx2 < d_ent->brushlist.count; bidx2 ++) {
				brush_s *b = &d_ent->brushlist.brush[bidx2];
				if (b == a) continue; // Don't do self.
				brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(b);
				//quadrant6_e q = brush_is_adjacent_with_face_match (a,b); // NO DIAGS
				//quadrant6_e q = brush_is_adjacent_with_vert_match (a,b);
				if (!String_Starts_With( face2->pbrtexture, basename))
					continue; // Not same texture class

				qbool they_touch = face_bbox_touch_face (pbr, face2);
				if (!they_touch)
					continue; // Not bbox touching
				//if (q == quadrant_none_neg1) continue; // Not adjacent
				//brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(b);
				//if (!face2) continue; // ???
				qbool is_flat_neighbor = face_is_flat(face2);
				if (is_flat_neighbor)
					flat_neighbors ++;
				else nonflat_neighbors ++;
			} // for brush neighbors

			if (is_flat == false && flat_neighbors == 0)
				continue; // nonflat next to nonflats

			// DECIDE ON 3 CATEGORIES.
			if (is_flat && nonflat_neighbors == 0)
				dp_strlcat (basename, "_flat", sizeof(basename));
			else if (is_flat && nonflat_neighbors)
				dp_strlcat (basename, "_flat_trim", sizeof(basename));
			else if (is_flat == false && flat_neighbors == 0)
				dp_strlcat (basename, "_noflat", sizeof(basename));
			else
				dp_strlcat (basename, "_noflat_trim", sizeof(basename));

			face_replace_texture (pbr, basename);

			num_done ++;
		} // brush
	} // for ent
	Con_PrintLinef ("List of texturenames replaced:");
	stringlist_condump_with_linenums (&list);
	stringlistfreecontents (&list);
	return num_done;
}

int entitylist_perform_flatzerosettexture_num_done (entitylist_t *plist, float z)
{
	int num_done = 0;
	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];
		for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
			brush_s *a = &d_ent->brushlist.brush[bx];
			brushrow_s *pbr = brush_get_face_facing_up_flat_only (a);
			if (!pbr)
				continue; // No flat face.

			if (pbr->bfmins[2] == z) {
				face_replace_texture (pbr, "common/up");
				num_done ++;
			}
		} // brush
	} // ent
	return num_done;
}


// Remove "noflat" and substitute "flat"
RELATED_ (entity_perform_pincommonterrain_blend_num_done)


int entity_perform_adjacents_num_done (entityx_t *d_ent, int entnum)
{
	int num_blended = 0;
	if (d_ent->brushlist.count == 0) return 0; // No brushes.

	stringlist_t list_textures = {0}; // what to do to each brush

	// ACCUMULATE LIST OF ALL TEXTURES in FUNC_GROUP
	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		brush_s *a = &d_ent->brushlist.brush[bx];

		// NO RECTS!  They hit to many things.
		if (!BRUSH_IS_5_SIDED_TRI(a)) continue; // NO RECT RIGHT NOW

		brushrow_s *pbr = brush_get_face_single_upward_tri_or_quad(a);

		if (!pbr) {
			Con_PrintLinef ("Entity %4d brush %4d is reported as not having a single upward face", entnum, bx);
			continue; // I don't think this is supposed to happen
		}

		if (String_Contains (pbr->pbrtexture, "common/")) {
			Con_PrintLinef ("Entity %4d brush %4d is reported as having a common/ texture of %s (skipping)", entnum, bx, pbr->pbrtexture);
			continue; // Ignoring a common texture in an upward facing brush
		}

		if (String_Contains (pbr->pbrtexture, "skies/")) {
			Con_PrintLinef ("Entity %4d brush %4d is reported as a sky texture %s (skipping)", entnum, bx, pbr->pbrtexture);
			continue; // Ignoring a common texture in an upward facing brush
		}

		if ( (bx % 256 == 0)) {
			Vid_SetWindowTitlef ("Phase 1: bx %d/%d %g", bx, d_ent->brushlist.count, (float)bx / d_ent->brushlist.count ); // RESET
		}

		stringlistappend_unique_didAdd (&list_textures, pbr->pbrtexture);

		// Find all adjacent brushes.
		for (int bx2 = 0; bx2 < d_ent->brushlist.count; bx2 ++) {
			brush_s *b = &d_ent->brushlist.brush[bx2];
			if (a == b) continue;

			if (!brush_xy_touches_brush (a, b)) continue;

			//qbool is_adjacent = brush_is_adjacent_with_brush_with_diags(a, b);
			//if (!is_adjacent)  continue;

			brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(b);

			if (String_Contains (face2->pbrtexture, "common/")) {
				Con_PrintLinef ("Entity %4d brush %4d (FACE2) is reported as having a common/ texture of %s (skipping)", entnum, bx2, face2->pbrtexture);
				continue; // Ignoring a common texture in an upward facing brush
			}

			if (String_Match (pbr->pbrtexture, face2->pbrtexture))
				a->temp_num_adjacent_sames ++;
			else
				a->temp_num_adjacent_diffs ++;

		} // brush2 b

		if (a->temp_num_adjacent_diffs) {
			Con_PrintLinef ("%5d: Blended ent %d bx %d diff adjacents = %d", num_blended, entnum, bx, a->temp_num_adjacent_diffs);
			num_blended ++;
		}
	} // brush

	// SORT THEN WE CAN ID
	stringlistsort_unique	(&list_textures);
	Con_PrintLinef ("Here is a list of unique textures found for entnum %d:", entnum);
	stringlist_condump_raw	(&list_textures);

	stringlist_t list_ops = {0};
	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		brush_s *a = &d_ent->brushlist.brush[bx];
		if (!a->temp_num_adjacent_diffs) continue;

		brushrow_s *pbr = brush_get_face_single_upward_tri_or_quad(a);

		if ( (bx % 256 == 0)) {
			Vid_SetWindowTitlef ("Phase 2: bx %d/%d %g", bx, d_ent->brushlist.count, (float)bx / d_ent->brushlist.count ); // RESET
		}

		// Find all adjacent brushes.
		int idx1 = stringlist_find_index (&list_textures, pbr->pbrtexture);
		int idx2 = not_found_neg1;
		int idx3 = not_found_neg1;
		for (int bx2 = 0; bx2 < d_ent->brushlist.count; bx2 ++) {
			brush_s *b = &d_ent->brushlist.brush[bx2];
			if (a == b) continue;

			brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(b);
			//if (!brush_xy_touches_brush (a, b)) continue;
			qbool they_touch = face_bbox_touch_face (pbr, face2);
			if (!they_touch) continue; // Not bbox touching

			//qbool is_adjacent = brush_is_adjacent_with_brush_with_diags(a, b);
			//if (!is_adjacent) continue; // NOT HELPFUL :(

			if (String_Match (pbr->pbrtexture, face2->pbrtexture)) continue;

			if (String_Contains (pbr->pbrtexture, "common/")) {
				Con_PrintLinef ("Entity %4d brush %4d is reported as having a common/ texture of %s (skipping)", entnum, bx, pbr->pbrtexture);
				continue; // Ignoring a common texture in an upward facing brush
			}

			int x2 = stringlist_find_index (&list_textures, face2->pbrtexture);

			if (idx2 == not_found_neg1)
				idx2 = x2;

			if (x2 != idx2) {
				Con_PrintLinef ("Found a brush with 2 different adjacent textures .. entnum %d brush %d ...", entnum, bx2);
				if (idx3 == not_found_neg1)
					idx3 = x2;
				Con_PrintLinef ("First is (%d) %s", idx1, list_textures.strings[idx1]);
				Con_PrintLinef ("Second is (%d) %s", idx2, list_textures.strings[idx2]);
				Con_PrintLinef ("Third is (%d) %s", idx3, list_textures.strings[idx3]);
			}
		} // brush2 b

		if (idx1 == not_found_neg1 || idx2 == not_found_neg1) {
			Con_PrintLinef ("Double unknown texture number ent %d brush %d .. skipping", entnum, bx);
			// face_replace_texture (pbr, "common/triple");
			continue; // DONE
		}

		qbool is_inverted = idx1 > idx2;
		if (idx1 > idx2) {
			int tmp = idx1;
			idx1 = idx2;
			idx2 = tmp;
		}
		va_super (texturename, Q3PATHLENGTH_64, "common/%d_to_%d", idx1, idx2);

		//face_replace_texture (pbr, texturename);

		stringlistappendf (&list_ops, "%d %s %d %d %d",
			bx,
			texturename,
			idx1,
			idx2,
			(int)is_inverted
		);

		// ALPHA BRUSHES
	} // brush

	Con_PrintLinef ("About to perform %d ops", list_ops.numstrings);

	stringlist_t list_vertex_no_sort = {0};

	for (int sidx = 0; sidx < list_ops.numstrings; sidx++) {
		ccs *sxy =  list_ops.strings[sidx];
		char s0_bx[1024], s1_texturename[Q3PATHLENGTH_64], s2_txidx1[8], s3_txidx2[8], s4_is_inverted[1024];
		qbool ok = arch5_space_split_isok (sxy, s0_bx, s1_texturename, s2_txidx1, s3_txidx2, s4_is_inverted);
		if (!ok) {
			Con_PrintLinef ("Arch5 split fail");
		}
		int bx = atoi(s0_bx);
		brush_s *a = &d_ent->brushlist.brush[bx];
		brushrow_s *face = brush_get_face_single_upward_tri_or_quad(a);
		face_replace_texture (face, s1_texturename);

		va_super (svectora, 128, VECTOR3_G, VECTOR3_SEND(face->b));
		qbool wasaddeda = stringlistappend_unique_evens_didAdd (&list_vertex_no_sort, svectora);
		if (wasaddeda)
			stringlistappendf (&list_vertex_no_sort, "%s %s", s2_txidx1, s3_txidx2); // A - B

		va_super (svectorb, 128, VECTOR3_G, VECTOR3_SEND(face->b));
		qbool wasaddedb = stringlistappend_unique_evens_didAdd (&list_vertex_no_sort, svectorb);
		if (wasaddedb)
			stringlistappendf (&list_vertex_no_sort, "%s %s", s2_txidx1, s3_txidx2); // A - B

		va_super (svectorc, 128, VECTOR3_G, VECTOR3_SEND(face->c));
		qbool wasaddedc = stringlistappend_unique_evens_didAdd (&list_vertex_no_sort, svectorc);
		if (wasaddedc)
			stringlistappendf (&list_vertex_no_sort, "%s %s", s2_txidx1, s3_txidx2); // A - B
	} // sidx

	stringlistfreecontents (&list_ops); // Rename ops

	//stringlistsort_unique (&list_vertex);
	stringlist_condump_with_linenums (&list_vertex_no_sort);

	// NOW WORK THE PINS FIGURING OUT ALPHA.  FOR NOW, ASSUME 0 to 1.
	int starting_brush_count =  d_ent->brushlist.count;
	for (int sidx = 0; sidx < list_vertex_no_sort.numstrings; sidx += 2) {
		char *sxy		= list_vertex_no_sort.strings[sidx + 0];
		char *sidx_cbo	= list_vertex_no_sort.strings[sidx + 1];
		char s0_idx0[Q3PATHLENGTH_64];
		char s1_idx1[Q3PATHLENGTH_64];
		arch2_space_split_isok (sidx_cbo, s0_idx0, s1_idx1);
		int idx0 = atoi(s0_idx0);
		int idx1 = atoi(s1_idx1);
		ccs *s0 = list_textures.strings[idx0];
		ccs *s1 = list_textures.strings[idx1];
		vec3_t point3d; Math_atov3 (sxy, point3d);

		float num_a = 0;
		float num_b = 0;
		float num_other = 0;
		for (int bx2 = 0; bx2 < starting_brush_count; bx2 ++) {
			brush_s *b = &d_ent->brushlist.brush[bx2];
			//if (a == b) continue;

			brushrow_s *face2 = brush_get_face_single_upward_tri_or_quad(b);

			qbool they_touch = face_bbox_touch_point (face2, point3d);
			if (!they_touch) continue; // Not bbox touching

			if (String_Match (face2->pbrtexture, s0))
				num_a ++;
			else if (String_Match (face2->pbrtexture, s1))
				num_b ++;
			else {
				num_other ++; // like 0 to 1 ... ignore for now?
				num_b ++; // alpha dominance
			}
		}
		float num_tot = num_a + num_b;
		//if (num_tot == 0) {
		//	int j = 5; // Shouldn't happen?
		//}

		float f = (float)num_a / num_tot;
		entity_brush_add_vstr_alpha_with_texturename (d_ent, sxy, f);
	} // for

	stringlistfreecontents	(&list_vertex_no_sort);

	Vid_SetWindowTitlef (gamename); // RESET
	stringlistfreecontents	(&list_textures);

	return num_blended;
}

int entitylist_perform_adjacents_num_done (entitylist_t *plist)
{
//- get all textures
//- brush texturenumplus1
//- mark adjacents in list_adjacent_idxs
//- work adjacents
//- make alpha posts
//- adjacents to posts
//- num qualified / a% = alpha

	//stringlist_t list = {0};
	int num_done = 0;
	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];
		num_done += entity_perform_adjacents_num_done (d_ent, ex);
	} // for ent

	Vid_SetWindowTitlef (gamename); // RESET
	return num_done;
}

#define bbox_6 6
#define RECTUS_ACTIVE_1		1
#define RECTUS_DELETED_0	0

qbool WriteBox_Clip_To_DidWrite (floatlist_s *flist, ccs *dir, vec3_t minz, vec3_t maxz, vec3_t clipminz, vec3_t clipmaxz)
{
	int numclips = 0;
	Con_PrintLinef ("Considering RECT dir = %s received " VECTOR3BBOX_G, dir, VECTOR3BBOX_SEND (minz, maxz) );
	Con_PrintLinef ("Clipping against parent " VECTOR3BBOX_G, VECTOR3BBOX_SEND (clipminz, clipmaxz));
	if (minz[0] < clipminz[0]) {
		Con_PrintLinef ("Mins X clipped from %g to %g",minz[0], clipminz[0]);
		minz[0] = clipminz[0];
		numclips ++;
	}
	if (maxz[0] > clipmaxz[0]) {
		Con_PrintLinef ("Maxs X clipped from %g to %g",maxz[0], clipmaxz[0]);
		maxz[0] = clipmaxz[0];
		numclips ++;
	}
	if (minz[1] < clipminz[1]) {
		Con_PrintLinef ("Mins Y clipped from %g to %g",minz[1], clipminz[1]);
		minz[1] = clipminz[1];
		numclips ++;
	}
	if (maxz[1] > clipmaxz[1]) {
		Con_PrintLinef ("Maxs X clipped from %g to %g",maxz[1], clipmaxz[1]);
		maxz[1] = clipmaxz[1];
		numclips ++;
	}
	vec3_t dims;
	VectorSubtract (maxz, minz, dims);
	Con_PrintLinef ("Final dimensions: " VECTOR3_G, VECTOR3_SEND(dims) );
	if (dims[0] <= 0) {
		Con_PrintLinef ("Rejected on X width %g", dims[0]);
		return false;
	}
	if (dims[1] <= 0) {
		Con_PrintLinef ("Rejected on Y height %g", dims[1]);
		return false;
	}

	Con_PrintLinef ("Rect survived with %d clips============================================================", numclips);
	maxz[2] = RECTUS_ACTIVE_1;
	floats_addvector (flist, minz);
	floats_addvector (flist, maxz);

	return true;
}

#define emins e->bemins
#define emaxs e->bemaxs

int collide_many (floatlist_s *flist, entityx_t *e, int entnum)
{
	int starting_count = flist->count;
	Con_PrintLinef ("Ent %d: " VECTOR3BBOX_G " testing versus floorage", entnum, VECTOR3BBOX_SEND(e->bemins,e->bemaxs) );
	for (int fidx = 0; fidx < starting_count; fidx += bbox_6) {
		vec3_t bmins, bmaxs;
		floats_vector_read_at (bmins, flist, fidx + 0);
		floats_vector_read_at (bmaxs, flist, fidx + 3);

		//Con_PrintVarTitleBBOX ("Floor initial size is ", imins, imaxs);
		if (bmaxs[2] == RECTUS_DELETED_0)
			continue; // Do not want ...


		//if (bmaxs[0] - bmins[0] == 0) continue; // Was deleted.
		Con_PrintLinef ("Floorage %d: (" VECTOR3BBOX_G ")",
			fidx / bbox_6,
			VECTOR3BBOX_SEND( bmins, bmaxs)
		);
		if (!BOX_XY_Collide (bmins, bmaxs, emins, emaxs)) {
			Con_PrintLinef ("Entnum %d: does not collide with floorage %d",
				entnum,
				fidx / bbox_6
			);
			continue;
		}
		Con_PrintLinef (CON_RED "Entnum %d: collision with floorage %d",
			entnum,
			fidx / bbox_6
		);

		flist->floats[fidx + bbox_6 - 1] = RECTUS_DELETED_0; // MARK AS DELETED
		Con_PrintLinef ("Floorage %d marked as deleted",
			fidx / bbox_6
		);

		// Everyone is a niner
		// NW N NE
		// W  X  E
		// SW S SE
		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------


		vec3_t ne0 = { bmins[0], bmins[1], -1 }; // BBOXS NW
		vec3_t ne1 = { emins[0], emins[1], -1 }; // EMINS NW

		WriteBox_Clip_To_DidWrite (flist, "ne", ne0, ne1, bmins, bmaxs);

		vec3_t nc0 = { emins[0], bmins[1], -1 }; // BBOX IS THE NORTH ONLY
		vec3_t nc1 = { emaxs[0], emins[1], -1 }; // Figure out Z at write time.

		WriteBox_Clip_To_DidWrite (flist, "n", nc0, nc1, bmins, bmaxs);

		vec3_t nw0 = { emaxs[0], bmins[1], -1 }; // BBOX TOP AND RIGHT
		vec3_t nw1 = { bmaxs[0], emins[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "nw", nw0, nw1, bmins, bmaxs);


		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------
//center:
  //  ;
		vec3_t cw0 = { bmins[0], emins[1], -1 }; // BBOX RIGHT ONLY
		vec3_t cw1 = { emins[0], emaxs[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "w", cw0, cw1, bmins, bmaxs);

#if 0 // CENTER GOES AWAY
		vec3_t cc0 = { emins[0], emins[1], -1 }; // E FULL
		vec3_t cc1 = { emaxs[0], emaxs[1], -1 }; //

		WriteBox (flist, xx0, xx1);
#endif
		vec3_t ce0 = { emaxs[0], emins[1], -1 }; // BBOX RIGHT ONLY
		vec3_t ce1 = { bmaxs[0], emaxs[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "e", ce0, ce1, bmins, bmaxs);
//south:
		vec3_t sw0 = { bmins[0], emaxs[1], -1 }; // BBOXS NW
		vec3_t sw1 = { emins[0], bmaxs[1], -1 }; // EMINS NW

		WriteBox_Clip_To_DidWrite (flist, "sw", sw0, sw1, bmins, bmaxs);

		vec3_t sc0 = { emins[0], emaxs[1], -1 }; // BBOX IS THE NORTH ONLY
		vec3_t sc1 = { emaxs[0], bmaxs[1], -1 }; // Figure out Z at write time.

		WriteBox_Clip_To_DidWrite (flist, "s", sc0, sc1, bmins, bmaxs);

		vec3_t se0 = { emaxs[0], emaxs[1], -1 }; // BBOX TOP AND RIGHT
		vec3_t se1 = { bmaxs[0], bmaxs[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "se", se0, se1, bmins, bmaxs);

		//myprintf ("%4d: " FLOAT_LOSSLESS_FORMAT, idx, f);
	} // for idx

	Con_PrintLinef (" ");
	return 1;
}
#undef emins
#undef emaxs

RELATED_ (VM_map_load)
// Baker: This never has brushes.
// Baker: Our missing here is adding the entities to the map, nothing more ...
int entitylist_perform_concat_ents_string (entitylist_t *pe_dest, ccs *s)
{
	int ret_is_ok = false;
	entitylist_t list_concat = {0};

	qbool is_parse_ok = entitylist_parsemap_metric (&list_concat, s);
	CMD_FAILEXIT_IF_ (!is_parse_ok, "Couldn't parse supplied text")

#ifdef _DEBUG
	entitylist_to_clipboard(&list_concat);
#endif

	// CONCAT
	entitylist_epairs_only_concat (pe_dest, &list_concat);	// brushes and keys.

	ret_is_ok = true;

fail_exit:
	entitylistfreecontents (&list_concat); // free data

	return ret_is_ok;
}

int entitylist_perform_concat_ents_intlist32 (entitylist_t *pe_dest, int32list_s *ilist)
{
	RELATED_ (Add_Edict_Is_Ok)
	int ret_is_ok = false;

	//// BUILD STRING FROM ENTITIES SUPPLIED
	//SV_Savegame_to
	//ret_is_ok = entitylist_perform_concat_ents_string (&list_concat, clip_za);

fail_exit:
	return ret_is_ok;
}

int entitylist_perform_concat_ents_clipboard (entitylist_t *pe_dest)
{
	int ret_is_ok = false;
	ccs *clip_za = Clipboard_Get_Text_ZAlloc();

	CMD_FAILEXIT_IF_ (!clip_za || strlen(clip_za) == 0, "No text on clipboard")

	ret_is_ok = entitylist_perform_concat_ents_string (pe_dest, clip_za);

fail_exit:
	Mem_FreeNull_ (clip_za);

	return ret_is_ok;
}


int entitylist_perform_floorage_world_only (entitylist_t *plist)
{
	int num_done = 0;
	floatlist_s list_bboxes_6 = {0};
	vec3_t imins, imaxs;
	VectorCopyDestSrc (imins, plist->bbentsmins);
	VectorCopyDestSrc (imaxs, plist->bbentsmaxs);
	imaxs[2] = RECTUS_ACTIVE_1;
	Con_PrintVarTitleBBOX ("Floor initial size is ", imins, imaxs);
	floats_addvector (&list_bboxes_6, imins);
	floats_addvector (&list_bboxes_6, imaxs);

	entityx_t	*dworld = &plist->entity[ENTITY_WORLD_0];
	ccs *val = entity_key_get_value (dworld, "_floorage");
	if (!val || atoi(val) > 0)
		val = "maps_sh/fang_grass_only";

	for (int ex = AFTER_WORLD_1; ex < plist->count; ex ++) {
		entityx_t	*re = &plist->entity[ex];
		if (Vector3_IsZeros(re->bedims))
			continue;
		ccs *s_classname =  entity_key_get_value(re, "classname");
		if (!s_classname || String_Starts_With (s_classname, "func_group_terrain") == false) {
			continue;
		}
		if (re->bemins[2] >= 0) {
			Con_PrintLinef ("Entity %d mins z is %g >= ground level z of 0, ignoring ...", ex, re->bemins[2]);
			continue; // Above ground entity.
		}

		Con_PrintLinef ("Entity %d is a %s .. checking ...", ex, s_classname);

		collide_many (&list_bboxes_6, re, ex);
#if 1 // Let's do a single one first.
		//break; //
#endif
	} // entities that are "func_wall"

	// Add brushes of the size
	floatlist_s *flist = &list_bboxes_6;
	for (int fidx = 0; fidx < flist->count; fidx += bbox_6) {
		vec3_t bmins, bmaxs;
		floats_vector_read_at (bmins, flist, fidx + 0);
		floats_vector_read_at (bmaxs, flist, fidx + 3);
		if (bmaxs[2] == RECTUS_DELETED_0)
			continue; // Deleted if Z marked 0
		if (bmaxs[0] - bmins[0] <= 0)
			continue; // Was deleted.  Better not happen.
		bmins[2] = imins[2];
		bmaxs[2] = 0;
		entity_brushs_add_size_texture_quadrant_num_done (dworld, quadrant_ceiling_5, /*texturename*/ val,
			bmins, bmaxs, /*tessize*/ 100000, "common/caulk");
		num_done ++;
		//stringlistappendf (&list, FLOAT_LOSSLESS_FORMAT, f);
	} // for

	floats_freecontents (&list_bboxes_6);
	return num_done;
}

#define emins a->bxmins // e->bemins
#define emaxs a->bxmaxs // e->bemaxs

int collide_many_cubeit (floatlist_s *flist, brush_s *a)
{

	int starting_count = flist->count;
	//Con_PrintLinef ("Ent %d: " VECTOR3BBOX_G " testing versus floorage", entnum, VECTOR3BBOX_SEND(e->bemins,e->bemaxs) );
	for (int fidx = 0; fidx < starting_count; fidx += bbox_6) {
		vec3_t bmins, bmaxs;
		floats_vector_read_at (bmins, flist, fidx + 0);
		floats_vector_read_at (bmaxs, flist, fidx + 3);

		//Con_PrintVarTitleBBOX ("Floor initial size is ", imins, imaxs);
		if (bmaxs[2] == RECTUS_DELETED_0)
			continue; // Do not want ...


		//if (bmaxs[0] - bmins[0] == 0) continue; // Was deleted.
		Con_PrintLinef ("Floorage %d: (" VECTOR3BBOX_G ")",
			fidx / bbox_6,
			VECTOR3BBOX_SEND( bmins, bmaxs)
		);
		if (!BOX_XY_Collide (bmins, bmaxs, emins, emaxs)) {
			//Con_PrintLinef ("Entnum %d: does not collide with floorage %d",
			//	entnum,
			//	fidx / bbox_6
			//);
			continue;
		}
		//Con_PrintLinef (CON_RED "Entnum %d: collision with floorage %d",
		//	entnum,
		//	fidx / bbox_6
		//);

		flist->floats[fidx + bbox_6 - 1] = RECTUS_DELETED_0; // MARK AS DELETED
		Con_PrintLinef ("Floorage %d marked as deleted",
			fidx / bbox_6
		);

		// Everyone is a niner
		// NW N NE
		// W  X  E
		// SW S SE
		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------


		vec3_t ne0 = { bmins[0], bmins[1], -1 }; // BBOXS NW
		vec3_t ne1 = { emins[0], emins[1], -1 }; // EMINS NW

		WriteBox_Clip_To_DidWrite (flist, "ne", ne0, ne1, bmins, bmaxs);

		vec3_t nc0 = { emins[0], bmins[1], -1 }; // BBOX IS THE NORTH ONLY
		vec3_t nc1 = { emaxs[0], emins[1], -1 }; // Figure out Z at write time.

		WriteBox_Clip_To_DidWrite (flist, "n", nc0, nc1, bmins, bmaxs);

		vec3_t nw0 = { emaxs[0], bmins[1], -1 }; // BBOX TOP AND RIGHT
		vec3_t nw1 = { bmaxs[0], emins[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "nw", nw0, nw1, bmins, bmaxs);


		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------
		// |       |       |        |
		// --------------------------
//center:
  //  ;
		vec3_t cw0 = { bmins[0], emins[1], -1 }; // BBOX RIGHT ONLY
		vec3_t cw1 = { emins[0], emaxs[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "w", cw0, cw1, bmins, bmaxs);

#if 0 // CENTER GOES AWAY
		vec3_t cc0 = { emins[0], emins[1], -1 }; // E FULL
		vec3_t cc1 = { emaxs[0], emaxs[1], -1 }; //

		WriteBox (flist, xx0, xx1);
#endif
		vec3_t ce0 = { emaxs[0], emins[1], -1 }; // BBOX RIGHT ONLY
		vec3_t ce1 = { bmaxs[0], emaxs[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "e", ce0, ce1, bmins, bmaxs);
//south:
		vec3_t sw0 = { bmins[0], emaxs[1], -1 }; // BBOXS NW
		vec3_t sw1 = { emins[0], bmaxs[1], -1 }; // EMINS NW

		WriteBox_Clip_To_DidWrite (flist, "sw", sw0, sw1, bmins, bmaxs);

		vec3_t sc0 = { emins[0], emaxs[1], -1 }; // BBOX IS THE NORTH ONLY
		vec3_t sc1 = { emaxs[0], bmaxs[1], -1 }; // Figure out Z at write time.

		WriteBox_Clip_To_DidWrite (flist, "s", sc0, sc1, bmins, bmaxs);

		vec3_t se0 = { emaxs[0], emaxs[1], -1 }; // BBOX TOP AND RIGHT
		vec3_t se1 = { bmaxs[0], bmaxs[1], -1 }; //

		WriteBox_Clip_To_DidWrite (flist, "se", se0, se1, bmins, bmaxs);

		//myprintf ("%4d: " FLOAT_LOSSLESS_FORMAT, idx, f);
	} // for idx

	Con_PrintLinef (" ");
	return 1;
}
#undef emins
#undef emaxs

RELATED_ (entitylist_perform_floorage_world_only)
int entitylist_perform_cubeit (entitylist_t *plist)
{
	int num_done = 0;
	floatlist_s list_bboxes_6 = {0};
	vec3_t imins, imaxs;
	VectorCopyDestSrc (imins, plist->bbentsmins);
	VectorCopyDestSrc (imaxs, plist->bbentsmaxs);
	imaxs[2] = RECTUS_ACTIVE_1;
	Con_PrintVarTitleBBOX ("Floor initial size is ", imins, imaxs);
	floats_addvector (&list_bboxes_6, imins);
	floats_addvector (&list_bboxes_6, imaxs);

	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) {
		entityx_t	*re = &plist->entity[ex];

		if (Vector3_IsZeros(re->bedims)) {
			goto remove_me; // Point entity or something with no size (like no brushes, only decal).
		}

		// _floorage would check the classname, we don't care.
		ccs *s_classname =  entity_key_get_value(re, "classname");
		Con_PrintLinef ("Entity %d is a %s .. checking ...", ex, s_classname);

		for (int bx = 0; bx < re->brushlist.count; bx ++) {
			brush_s *a = &re->brushlist.brush[bx];
			a->is_considered_deleted = true;
			collide_many_cubeit (&list_bboxes_6, a);

		} // brush

remove_me:
		if (ex > 0)
			re->is_considered_deleted = true;

#if 1 // Let's do a single one first.
		//break; //
#endif
	} // entities that are "func_wall"

	// Add brushes of the size
	entityx_t	*dworld = &plist->entity[ENTITY_WORLD_0];
	floatlist_s *flist = &list_bboxes_6;
	entity_brush_add_with_bbox_and_texture_return_idx (dworld, plist->bbentsmins, plist->bbentsmaxs, "common/botclip");
	for (int fidx = 0; fidx < flist->count; fidx += bbox_6) {
		vec3_t bmins, bmaxs;
		floats_vector_read_at (bmins, flist, fidx + 0);
		floats_vector_read_at (bmaxs, flist, fidx + 3);
		if (bmaxs[2] == RECTUS_DELETED_0)
			continue; // Deleted if Z marked 0
		if (bmaxs[0] - bmins[0] <= 0)
			continue; // Was deleted.  Better not happen.
		bmins[2] = plist->bbentsmins[2]; // imins[2];
		bmaxs[2] = plist->bbentsmaxs[2];
		entity_brushs_add_size_texture_quadrant_num_done (dworld, quadrant_ceiling_5, /*texturename*/ "common/nodraw",
			bmins, bmaxs, /*tessize*/ 100000, "common/nodraw");
		num_done ++;
		//stringlistappendf (&list, FLOAT_LOSSLESS_FORMAT, f);
	} // for

	floats_freecontents (&list_bboxes_6);
	return num_done;
}


// keyword = "_automatic"
int entitylist_perform__decal__automatic (entitylist_t *plist)
{
	// Find appropriate _decals and drop them to floor.  <==== how?
	// Take center point and compare against all other brushes and entities.
	// Highest below collision is where to plant the _decal
	int num_done = 0;
	//int decal_automatic_index = 0; // _automatic_1
	for (int ex = AFTER_WORLD_1; ex < plist->original_ent_count; ex ++) {
		entityx_t	*e = &plist->entity[ex];
		ccs *classname = entity_key_get_value (e, "classname");

		// Check classname "func_group_this_one" or whatever.
		if (!classname || !String_Match (classname, "_decal"))
			continue; // Not a decal

		ccs *target = entity_key_get_value (e, "target");
		if (!target || !String_Match (target, "_automatic_droptofloor"))

			continue; // Not an automatic decal.

		RELATED_ (brush_or_patch_tranlate_was_processed)
		vec3_t org; qbool isok = entity_get_center_isok (e, org);
		if (!isok) {
			Con_PrintLinef ("#ent %d _decal couldn't find center", ex);

			continue;
		}

		Con_PrintLinef ("#ent %d _decal center is " VECTOR3_G, ex, VECTOR3_SEND(org));

		// Now what?
		vec3_t collide;
		qbool did_hit = entitylist_collide_down_ignoring_entnum (collide, plist, ENTITY_WORLD_0, plist->original_ent_count, org, ex);

		if (!did_hit) {
			Con_PrintLinef ("#ent %d _decal at " VECTOR3_G " did not hit anything below it",
				ex, VECTOR3_SEND(org));
			continue;
		}

		collide[2] -= 0.5; // Move it down just a little.

		Con_PrintLinef ("#ent %d _decal at " VECTOR3_G " droptofloor result = " VECTOR3_G,
			ex,
			VECTOR3_SEND(org),
			VECTOR3_SEND(collide)
		);

		// Create a point entity at collide
		va_super (targetname, 128, "_decal_target_%d", num_done);
		// unused int new_entity_index = plist->count;
		entityx_t	*d_ent_new = entitylist_add (plist); // D = DEST
		entity_add_key_value_unsafe_f (d_ent_new, "classname", "info_null");
		entity_add_key_value_unsafe_f (d_ent_new, "origin", VECTOR3_G, VECTOR3_SEND(collide) );
		entity_add_key_value_unsafe_f (d_ent_new, "targetname", targetname );

		// NOW WIRE UP THE TARGET
		//entity_add_key_value_unsafe_f (e, "target", targetname ); NO DUMMY ==> UNSAFE FOOL! == ADDS DUP
		entity_key_set_value_is_ok (e, "target", targetname); // SAFE == CHANGES

		num_done ++;
	} // entities in .map
	return num_done;
}


RELATED_ (entitylist_gen_models)
int entitylist_perform_collision_body (entitylist_t *plist, ccs *s0_plus_timestamp)
{
	// Q: Where are they created and does it matter for brush entities?
	int num_made = 0;

	stringlist_t modelstoaddlist = {0};

	for (int ex = ENTITY_WORLD_0; ex < AFTER_WORLD_1 ; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];
		//char **pval = NULL;
		//int val_idx = not_found_neg1;
		for (int keyidx = 0; keyidx < d_ent->pairslist.numstrings; keyidx += 2) {
			ccs *key = d_ent->pairslist.strings[keyidx + 0];
			ccs *val = d_ent->pairslist.strings[keyidx + 1];

			// "0" or "0 - something here" ... we do not run it.
			if (String_Starts_With_PRE (key, "_collision_body") == false)
				continue; // Doesn't start with _collision_body

			if (val[0] == '0') {
				continue; // Leading 0 means already run
			}

			// Add it!
			qbool was_added = stringlistappend_unique_didAdd (&modelstoaddlist, val);
			if (was_added == false) {
				Con_PrintLinef ("Duplicate collision body ignored: %s", val);
			}

			// Now set it to leading zero
			entity_key_idx_set_value_is_ok (d_ent, keyidx, s0_plus_timestamp);
		} // for epairs
	} // entities in .map

	// Add the accumulated entities
	for (int sidx = 0; sidx < modelstoaddlist.numstrings; sidx ++) {
		ccs *val = modelstoaddlist.strings[sidx]; // "models/vehicles/psx/van_car04.md3 188.9375 101.8125 82.25"

		entitylist_make_insert_func_collision_ent_brush_at_1 (plist, val); // Prints
		num_made ++;
	}

	// Free
	stringlistfreecontents (&modelstoaddlist);

	return num_made;
}


