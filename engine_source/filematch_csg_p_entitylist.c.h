// filematch_csg_p_entitylist.c.h

RELATED_ (entity_update_bbox_from_brushes_was_processed)
CALLERS_ (entitylist_parsemap_metric) // WE ARE BOSS
void entitylist_metric_world_brushes_nova (entitylist_t *plist)
{
	int num_processed = 0;

	entityx_t	*p_world = &plist->entity[ENTITY_WORLD_0];

	qbool was_processed 
		= entity_update_bbox_from_brushes_was_processed (p_world);

	// We could be dealing with something weird like all func groups.
	if (was_processed)
		num_processed ++;

	vec3_t bbemins, bbemaxs;
	VectorCopyDestSrc (bbemins, p_world->bemins);
	VectorCopyDestSrc (bbemaxs, p_world->bemaxs);
	
	qbool did_any_ents = false;
	for (int ex = AFTER_WORLD_1; ex < plist->count; ex ++) {
		entityx_t	*d_ent = &plist->entity[ex];

		qbool was_processed = entity_update_bbox_from_brushes_was_processed (d_ent);
		if (!was_processed)
			continue;

		// EXPANDING ENTITY METRIC
		bounds_expand	(d_ent->bemins, bbemins, bbemaxs); // Yes, bbemaxs is what expands
		bounds_expand	(d_ent->bemaxs, bbemins, bbemaxs);

		did_any_ents = true;
		num_processed ++;
	} // entities that are "func_wall"

	if (1 /*did_any_ents*/) {
		// Could be all world and point entities -- no func_group and no walls/doors/etc.
		VectorCopyDestSrc	(plist->bbentsmins, bbemins);
		VectorCopyDestSrc	(plist->bbentsmaxs, bbemaxs);
		VectorSubtract		(plist->bbentsmaxs, plist->bbentsmins, plist->bbentsdims);

	}

	//return num_processed;
}


// What's the option thing?  No quadrants
// plist is always a model.

// Results goes to plist so it is required to be world.

// recursive to brush quadrants
void entity_identify_brush_quadrants (entityx_t *d_ent)
{
	for (int brush_idx = 0; brush_idx < d_ent->brushlist.count; brush_idx ++) {
		brush_s *d_brush = &d_ent->brushlist.brush[brush_idx];
		if (d_brush->is_a_patch || !d_brush->brushrowlist.count)
			continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

		brush_box_identify_face_quadrants (d_brush);

		// Now determine quadrants of this brush and update world (plist quadrants)
		// WEST
		while (1) {
			int best_brush_idx = UNPLUS1(d_ent->quadrants_brushnums_plus1[quadrant_west_0]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				d_ent->quadrants_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			// The west is the least east = lowest Y maxs.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmaxs[0] < p_best->bxmaxs[0]) { // MORE
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// EAST
		while (1) {
			int best_brush_idx = UNPLUS1(d_ent->quadrants_brushnums_plus1[quadrant_east_1]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				d_ent->quadrants_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			// The east is the least west = highest X mins.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmins[0] > p_best->bxmins[0]) { // MORE
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// SOUTH
		while (1) {
			int best_brush_idx = UNPLUS1(d_ent->quadrants_brushnums_plus1[quadrant_south_2]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			// The south is the least north = lowest Y maxs.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmaxs[1] < p_best->bxmaxs[1]) { // MORE
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// NORTH
		while (1) {
			// The NORTH is the exclusive least south.  That means the mins[1] is the highest -- will beat them all.
			int best_brush_idx = UNPLUS1(d_ent->quadrants_brushnums_plus1[quadrant_north_3]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				d_ent->quadrants_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			// The north is the least south = highest Y mins.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmins[1] > p_best->bxmins[1]) { // MORE
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// FLOOR
		while (1) {
			int best_brush_idx = UNPLUS1(d_ent->quadrants_brushnums_plus1[quadrant_floor_4]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				d_ent->quadrants_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);
				break;
			}
			// The floor is the least high = lowest Z maxs.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmaxs[2] < p_best->bxmaxs[2]) { // MORE
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);

				break;
			}

			break;
		}

		// CEILING
		while (1) {
			int best_brush_idx = UNPLUS1(d_ent->quadrants_brushnums_plus1[quadrant_ceiling_5]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				d_ent->quadrants_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			// The ceiling is the most high = highest Z mins.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmins[2] > p_best->bxmins[2]) { // MORE
				// Automatic win
				d_ent->quadrants_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			break;
		}
#ifdef _DEBUG
		int j = 5; // Check d_brush
#endif
	} // each brush idx

}

void entity_layoutbox_identify_world_brush_quadrants (entitylist_t *plist)
{
	entityx_t *d_ent = &plist->entity[ENTITY_WORLD_0];

	for (int brush_idx = 0; brush_idx < d_ent->brushlist.count; brush_idx ++) {
		brush_s *d_brush = &d_ent->brushlist.brush[brush_idx];
		if (d_brush->is_a_patch || !d_brush->brushrowlist.count)
			continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

		brush_box_identify_face_quadrants (d_brush);

		// Now determine quadrants of this brush and update world (plist quadrants)
		// WEST
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_west_0]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			// The west is the least east = lowest Y maxs.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmaxs[0] < p_best->bxmaxs[0]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// EAST
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_east_1]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			// The east is the least west = highest X mins.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmins[0] > p_best->bxmins[0]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// SOUTH
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_south_2]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			// The south is the least north = lowest Y maxs.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmaxs[1] < p_best->bxmaxs[1]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// NORTH
		while (1) {
			// The NORTH is the exclusive least south.  That means the mins[1] is the highest -- will beat them all.
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_north_3]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			// The north is the least south = highest Y mins.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmins[1] > p_best->bxmins[1]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// FLOOR
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_floor_4]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);
				break;
			}
			// The floor is the least high = lowest Z maxs.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmaxs[2] < p_best->bxmaxs[2]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);

				break;
			}

			break;
		}

		// CEILING
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			// The ceiling is the most high = highest Z mins.
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
			if (d_brush->bxmins[2] > p_best->bxmins[2]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			break;
		}
#ifdef _DEBUG
		int j = 5; // Check d_brush
#endif
	} // each brush idx

}

// This operates on a model.
#if 0
void entitylist_layout_identify_walls_lightrod (entitylist_t *plist)
{

	entityx_t	*p_ent_world = &plist->entity[ENTITY_WORLD_0];
	entity_metric_entity_brushes_and_brushrows7 (p_ent_world, plist);

	// QUADRANTS
	for (int brush_idx = 0; brush_idx < p_ent_world->brushlist.count; brush_idx ++) {
		brush_s *p_brush = &p_ent_world->brushlist.brush[brush_idx];
		if (p_brush->is_a_patch || !p_brush->brushrowlist.count)
			continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

		// Now determine quadrants of this brush and update world (plist quadrants)
		// WEST
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_west_0]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			// The west is the least east = lowest Y maxs.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmaxs[0] < p_best->bxmaxs[0]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// EAST
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_east_1]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			// The east is the least west = highest X mins.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmins[0] > p_best->bxmins[0]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// SOUTH
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_south_2]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			// The south is the least north = lowest Y maxs.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmaxs[1] < p_best->bxmaxs[1]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// NORTH
		while (1) {
			// The NORTH is the exclusive least south.  That means the mins[1] is the highest -- will beat them all.
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_north_3]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			// The north is the least south = highest Y mins.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmins[1] > p_best->bxmins[1]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// FLOOR
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_floor_4]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);
				break;
			}
			// The floor is the least high = lowest Z maxs.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmaxs[2] < p_best->bxmaxs[2]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);

				break;
			}

			break;
		}

		// CEILING
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			// The ceiling is the most high = highest Z mins.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmins[2] > p_best->bxmins[2]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			break;
		}
#ifdef _DEBUG
		int j = 5; // Check p_brush
#endif
	} // each brush idx

	//int j = 5; // Check plist
}
#endif


qbool entitylist_parsemap_metric (entitylist_t *plist, ccs *txt)
{
	qbool isok = entitylist_parsemaptxt (plist, txt);
	if (!isok) return false;
	// metric
	entitylist_metric_world_brushes_nova			(plist);
	//entity_layoutbox_identify_world_brush_quadrants (&list_model);
	return true;
}

RELATED_ (entitylist_nonworld_append)

// ONLY TRANSLATING THE ORIGIN KEY.
void entity_translate_with_metric (entityx_t *d_ent, vec3_t move_amount_add)
{
	int num_moved = 0;
	// Translate entity brushes.
	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		brush_s *d_brush = &d_ent->brushlist.brush[bx];
		qbool was_processed = brush_or_patch_tranlate_was_processed (d_brush, move_amount_add);
		if (was_processed)
			num_moved ++;
	} // brush
	RELATED_ (entitylist_translate_epairs_origin)
	for (int kx = 0; kx < d_ent->pairslist.numstrings; kx += 2) {
		ccs *key = d_ent->pairslist.strings[kx + 0];

		// We are only translating the origin key
		if (String_Match(key,"origin") == false)
			continue; // Not "origin"

		ccs *val = d_ent->pairslist.strings[kx + 1];

		vec3_t vorg; Math_atov3 (val, /*out*/ vorg);
		VectorAdd (move_amount_add, vorg, vorg); // Translate vorg = vadd + vorg

		// REWRITE
		char *newval = Z_StrDupf (VECTOR3_LOSSLESS, VECTOR3_SEND(vorg));  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
		Mem_FreeNull_ (d_ent->pairslist.strings[kx + 1]);
		d_ent->pairslist.strings[kx + 1] = newval;

		// ASSUME MAXIMUM OF A SINGLE "origin" per entity,
		// SO ..
		break;	// GET OUT!
	} // epairs


	entity_update_bbox_from_brushes_was_processed (d_ent);// entity_metric_update(d_ent);
}

int entitylist_find_entity_by_pointer (entitylist_t *plist, entityx_t *efind)
{
	for (int eidx = ENTITY_WORLD_0; eidx < plist->count; eidx ++) {
		entityx_t	*r_ent = &plist->entity[eidx];
		if (r_ent == efind)
			return eidx;
	} // for ent
	return not_found_neg1;
}

// UNSAFE because might be a duplicate key

void entity_add_key_value_unsafe_f (entityx_t *d_ent, ccs *key, ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	stringlistappend (&d_ent->pairslist, key); // KEY
	stringlistappend (&d_ent->pairslist, text); // VALUE
	VA_EXPAND_ALLOC_FREE (text);
}

entityx_t *entitylist_add_pasting_this_ent (entitylist_t *plist, const entityx_t *r_ent /*paste source*/) // June 8 2025
{
	entityx_t	*d_ent = entitylist_add(plist);	// D = DEST

	VectorCopyDestSrc (d_ent->bemins, r_ent->bemins);
	VectorCopyDestSrc (d_ent->bemaxs, r_ent->bemaxs);
	VectorCopyDestSrc (d_ent->bedims, r_ent->bedims);

	for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
		ccs *key = r_ent->pairslist.strings[kx + 0];
		ccs *val = r_ent->pairslist.strings[kx + 1];

		stringlistappend (&d_ent->pairslist, key); // KEY
		stringlistappend (&d_ent->pairslist, val); // VALUE
	} // epairs

	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[bx];	// READ BRUSH FROM SRC
		brush_s *d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST

		d_brush->is_a_patch = r_brush->is_a_patch;

		VectorCopyDestSrc (d_brush->bxmins, r_brush->bxmins);
		VectorCopyDestSrc (d_brush->bxmaxs, r_brush->bxmaxs);
		VectorCopyDestSrc (d_brush->bxdims, r_brush->bxdims);

		if (r_brush->is_a_patch) {
			// If patch, copy patch stuff ...
			d_brush->texture = Z_StrDup (r_brush->texture);
			d_brush->rows = r_brush->rows;
			d_brush->cols = r_brush->cols;
			d_brush->other3[0] = r_brush->other3[0];
			d_brush->other3[1] = r_brush->other3[1];
			d_brush->other3[2] = r_brush->other3[2];
		}

		if (r_brush->is_a_patch == false) {
			// REAL BRUSH
			for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
				const brushrow_s *rbr = &r_brush->brushrowlist.brushrow[brow];
				brushrow_s *dbr = brushrowlist_add(&d_brush->brushrowlist);

				memcpy (dbr, rbr, sizeof(*rbr));
				dbr->pbrtexture = Z_StrDup (rbr->pbrtexture);
				
				VectorCopyDestSrc (dbr->bfmins, rbr->bfmins);
				VectorCopyDestSrc (dbr->bfmaxs, rbr->bfmaxs);
				VectorCopyDestSrc (dbr->bfdims, rbr->bfdims);
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
	return d_ent;
}

// Baker: The entity here is a func_group like "func_group_wall_north".
// We expect a box.
void entity_add_alpha_brushes (entityx_t *d_ent, vec3_t worldmins, vec3_t worldmaxs, vec3_t worldsize)
{
	for (int corner = 0; corner < 8; corner ++) { // Do all 8 corners.
		qbool xhi = (corner & 1) ? true : false;
		qbool yhi = (corner & 2) ? true : false; 
		qbool zhi = (corner & 4) ? true : false;

		vec3_t brmins, brmaxs, point3d;
		point3d[0] = xhi ? d_ent->bemaxs[0] : d_ent->bemins[0];
		point3d[1] = yhi ? d_ent->bemaxs[1] : d_ent->bemins[1];
		point3d[2] = zhi ? d_ent->bemaxs[2] : d_ent->bemins[2];

		VectorCopyDestSrc (brmins, point3d);
		VectorCopyDestSrc (brmaxs, point3d);

		for (int d = 0; d < 3; d ++) {
			brmins[d] -= 48;
			brmaxs[d] += 48;
		} // for

		float f = (point3d[0] - worldmins[0])/worldsize[0];
		f = bound (0, f, 1);
		va_super (texturename, 128, "common/alpha_%1.2f", f);
		String_Edit_Replace (texturename, sizeof(texturename), ".", "_");
		entity_brush_add_with_bbox_and_texture (d_ent, brmins, brmaxs, texturename);
		// %1.3f 0
	}

}



