// filematch_csg_p_entity.c.h

#if 0
CALLERS_ (entitylist_metric_world_brushes_with_walls)
void entity_metric_entity_brushes_and_brushrows (entityx_t *d_ent, /*optional*/ entitylist_t *plist)
{

	// Do the faces...
	if (plist) {
		for (int brush_idx = 0; brush_idx < d_ent->brushlist.count; brush_idx ++) {
			brush_s *p_brush = &d_ent->brushlist.brush[brush_idx];
			if (p_brush->is_a_patch || !p_brush->brushrowlist.count)
				continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

			brush_metric_for_faces (p_brush); // Here just because ...
#ifdef _DEBUG
			int j = 5;
#endif
		} // working the faces real quick ...
	} // bmakefacequadrants


	for (int brush_idx = 0; brush_idx < d_ent->brushlist.count; brush_idx ++) {
		brush_s *p_brush = &d_ent->brushlist.brush[brush_idx];
		if (p_brush->is_a_patch || !p_brush->brushrowlist.count)
			continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

		brush_metric_for_faces (p_brush); // Here just because ...

		vec3_t brmins, brmaxs;
		qbool was_processed = brush_get_bbox_was_processed (p_brush, brmins, brmaxs);

		if (!was_processed) // Patch probably ...
			continue;

		VectorCopyDestSrc (p_brush->bxmins, brmins);
		VectorCopyDestSrc (p_brush->bxmaxs, brmaxs);
		VectorSubtract (p_brush->bxmaxs, p_brush->bxmins, p_brush->bxdims);

		if (!plist)
			continue;

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
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
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
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
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
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
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
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
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
			brush_s *p_best = &d_ent->brushlist.brush[best_brush_idx];
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

}
#endif

RELATED_ (VM_map_entity_get_mins_maxs) // Wowza!
qbool entity_expand_bounds_did_brushes (entityx_t *r_ent, vec3_t brmins, vec3_t brmaxs)
{
	qbool did_any_brushes = false;

	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[bx];

		if (r_brush->is_a_patch)
			continue; // PATCH, SKIP

		if (r_brush->brushrowlist.count && did_any_brushes == false)
			did_any_brushes = true;

		// REAL BRUSH
		for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
			brushrow_s *pbr = &r_brush->brushrowlist.brushrow[brow];
			bounds_expand (pbr->a, brmins, brmaxs);
			bounds_expand (pbr->b, brmins, brmaxs);
			bounds_expand (pbr->c, brmins, brmaxs);
		} // brush row
	} // brush

	return did_any_brushes;
}

CALLERS_ (entitylist_perform_layout_from_boxes entity_add_brush_originbrush_from_bbox)
qbool entity_brush_add_with_bbox_and_texture (entityx_t *p_ent, const vec3_t brmins, const vec3_t brmaxs, ccs *texturename)
{
	vec3_t brdims; VectorSubtract (brmaxs, brmins, brdims);
	brush_s *p_brush = brushlist_add (&p_ent->brushlist); // CREATE BRUSH FOR DEST
	//vec3_t brdims2; VectorSubtract (brmaxs, brmins, brdims2);

	if (brdims[0] <= 0 || brdims[1] <= 0 || brdims[2] <= 0)
		return false;

#define XH brmaxs[0]
#define YH brmaxs[1]
#define ZH brmaxs[2]
#define XL brmins[0]
#define YL brmins[1]
#define ZL brmins[2]

	brushrow_s *br = brushrowlist_add (&p_brush->brushrowlist);
	//          ( [XH] [YH] [ZH] )                ( [XH] [YH] [ZL] )          ( [XH] [YL] [ZH] )
	VectorSet (br->a, XH, YH, ZH); VectorSet (br->b, XH, YH, ZL); VectorSet (br->c, XH, YL, ZH);
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	//553648128 16512 0
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 2 - all numbers identical to 1 except abc
	//( [XL] [YL] [ZH] ) ( [XL] [YL] [ZL] ) ( [XL] [YH] [ZH] ) common/caulk [ 0 1 0 -20.4 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YL, ZH); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XL, YH, ZH);
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 3
	//( [XH] [YL] [ZH] ) ( [XH] [YL] [ZL] ) ( [XL] [YL] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XH, YL, ZL); VectorSet (br->c, XL, YL, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 4 - all numbers identical to 3 except abc
	// ( [XL] [YH] [ZH] ) ( [XL] [YH] [ZL] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZH); VectorSet (br->b, XL, YH, ZL); VectorSet (br->c, XH, YH, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 5
	// ( [XL] [YH] [ZL] ) ( [XL] [YL] [ZL] ) ( [XH] [YH] [ZL] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZL); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XH, YH, ZL);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 6 - all ident to 5 except a b c
	// ( [XH] [YL] [ZH] ) ( [XL] [YL] [ZH] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XL, YL, ZH); VectorSet (br->c, XH, YH, ZH);
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

#undef XH
#undef YH
#undef ZH
#undef XL
#undef YL
#undef ZL

	return true; // isok
}

CALLERS_ (entitylist_perform_layout_from_boxes entity_add_brush_originbrush_from_bbox)
int entity_brush_add_with_bbox_and_texture_return_idx (entityx_t *p_ent, const vec3_t brmins, const vec3_t brmaxs, 
													   ccs *texturename)
{
	vec3_t brdims; VectorSubtract (brmaxs, brmins, brdims);
	//vec3_t brdims2; VectorSubtract (brmaxs, brmins, brdims2);

	if (brdims[0] <= 0 || brdims[1] <= 0 || brdims[2] <= 0)
		return not_found_neg1;

	int newidx = p_ent->brushlist.count;
	brush_s *p_brush = brushlist_add (&p_ent->brushlist); // CREATE BRUSH FOR DEST

#define XH brmaxs[0]
#define YH brmaxs[1]
#define ZH brmaxs[2]
#define XL brmins[0]
#define YL brmins[1]
#define ZL brmins[2]
	//quadrant_west_0,
	//quadrant_east_1,
	//quadrant_south_2,
	//quadrant_north_3,
	//quadrant_floor_4,
	//quadrant_ceiling_5,

	brushrow_s *br;

	br = brushrowlist_add (&p_brush->brushrowlist); // 2 - all numbers identical to 1 except abc
	//( [XL] [YL] [ZH] ) ( [XL] [YL] [ZL] ) ( [XL] [YH] [ZH] ) common/caulk [ 0 1 0 -20.4 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YL, ZH); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XL, YH, ZH); // WEST (NO XH)
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist);
	//          ( [XH] [YH] [ZH] )                ( [XH] [YH] [ZL] )          ( [XH] [YL] [ZH] )
	VectorSet (br->a, XH, YH, ZH); VectorSet (br->b, XH, YH, ZL); VectorSet (br->c, XH, YL, ZH); // EAST (NO XL)
	Vector4Set (br->xtra1, 0, 1, 0, -20.4 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	//553648128 16512 0
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");


	br = brushrowlist_add (&p_brush->brushrowlist); // 3
	//( [XH] [YL] [ZH] ) ( [XH] [YL] [ZL] ) ( [XL] [YL] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XH, YL, ZL); VectorSet (br->c, XL, YL, ZH); // YH MISS (SOUTH)
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 4 - all numbers identical to 3 except abc
	// ( [XL] [YH] [ZH] ) ( [XL] [YH] [ZL] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 0 -1 6.29999 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZH); VectorSet (br->b, XL, YH, ZL); VectorSet (br->c, XH, YH, ZH); // YL MISS (NORTH)
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, 0, -1, 6.29999 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 5
	// ( [XL] [YH] [ZL] ) ( [XL] [YL] [ZL] ) ( [XH] [YH] [ZL] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XL, YH, ZL); VectorSet (br->b, XL, YL, ZL); VectorSet (br->c, XH, YH, ZL); // ZH MISS (FLOOR)
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

	br = brushrowlist_add (&p_brush->brushrowlist); // 6 - all ident to 5 except a b c
	// ( [XH] [YL] [ZH] ) ( [XL] [YL] [ZH] ) ( [XH] [YH] [ZH] ) common/caulk [ 1 0 0 20 ] [ 0 -1 0 20.4 ] 0 0.5 0.5 0 160 0
	VectorSet (br->a, XH, YL, ZH); VectorSet (br->b, XL, YL, ZH); VectorSet (br->c, XH, YH, ZH); // ZL MISS (CEIL)
	Vector4Set (br->xtra1, 1, 0, 0, 20 ); Vector4Set (br->xtra2, 0, -1, 0, 20.4 );
	Vector6Set (br->ftrail, 0, 0.5, 0.5, 553648128, 16512, 0); br->trail_count = 6;
	br->pbrtexture = Z_StrDup (texturename); //"common/origin");

#undef XH
#undef YH
#undef ZH
#undef XL
#undef YL
#undef ZL

	return newidx; // isok
}

int entity_brushs_add_size_texture_quadrant_num_done (entityx_t *p_ent, quadrant6_e quadrant_wanted, ccs *texturename, const vec3_t brmins, const vec3_t brmaxs, 
 float tessSize, ccs *caulktex)
{
	int num_done = 0;
	for (float x0 = brmins[0]; x0 < brmaxs[0]; x0 += tessSize) {
		if (x0 >= brmaxs[0]) 
			continue;
		float x1 = x0 + tessSize; if (x1 > brmaxs[0]) x1 = brmaxs[0];
		for (float y0 = brmins[1]; y0 < brmaxs[1]; y0 += tessSize) {
			if (y0 >= brmaxs[1]) 
				continue;
			float y1 = y0 + tessSize; if (y1 > brmaxs[1]) y1 = brmaxs[1];
			for (float z0 = brmins[2]; z0 < brmaxs[2]; z0 += tessSize) {
				if (z0 >= brmaxs[2]) 
					continue;
				float z1 = z0 + tessSize; if (z1 > brmaxs[2]) z1 = brmaxs[2];
				vec3_t brmins2 = {x0, y0, z0};
				vec3_t brmaxs2 = {x1, y1, z1};

				int brush_idx1 = entity_brush_add_with_bbox_and_texture_return_idx(p_ent, brmins2, brmaxs2, caulktex);
				brush_s *n_brush = &p_ent->brushlist.brush[brush_idx1];
				brushrow_s *n_face = &n_brush->brushrowlist.brushrow[quadrant_wanted];
				face_retexture_z (n_face, texturename);
				num_done ++;
			} // z
		} // y
	} // x
	return num_done;
}


// Return idx
//#include <math.h>
int entity_brush_add_alpha_with_texturename (entityx_t *d_ent, vec3_t point3d, float f_alpha)
{
	vec3_t brmins, brmaxs;
	VectorCopyDestSrc (brmins, point3d);
	VectorCopyDestSrc (brmaxs, point3d);

	for (int d = 0; d < 3; d ++) {
		brmins[d] -= 4;
		brmaxs[d] += 4;
	} // for

	if (f_alpha == 0) {
		// printf("The number is negative (including negative zero).\n"); 
		f_alpha = 0; // Negative zero fix.
	}

	va_super (texturename, 128, "common/alpha_%1.2f", f_alpha);
	String_Edit_Replace (texturename, sizeof(texturename), ".", "_");
	return entity_brush_add_with_bbox_and_texture_return_idx (d_ent, brmins, brmaxs, texturename);
}

int entity_brush_add_vstr_alpha_with_texturename (entityx_t *d_ent, ccs *spoint_to_atov, float f_alpha)
{
	vec3_t point3d; Math_atov3 (spoint_to_atov, point3d);

	return entity_brush_add_alpha_with_texturename (d_ent, point3d, f_alpha);
}


CALLERS_ (entitylist_perform_layout_from_boxes)

// originmake
qbool entity_add_brush_originbrush_from_bbox (entityx_t *p_ent, vec3_t brmins, vec3_t brmaxs)
{
	return entity_brush_add_with_bbox_and_texture (p_ent, brmins, brmaxs, "common/origin");
}

// Q: How to metric? entitylist_metric_world_brushes_nova entity_layoutbox_identify_world_brush_quadrants
qbool entity_add_brush_bbox_with_texture_mirroring_face(entityx_t *p_ent, vec3_t brmins, vec3_t brmaxs, brushrow_s *r_face)
{
	return entity_brush_add_with_bbox_and_texture (p_ent, brmins, brmaxs, "common/origin");
}

CALLERS_ (lightrod)
ccs *entity_get_first_texture (entityx_t *p_ent)
{
	for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
		brush_s *p_brush = &p_ent->brushlist.brush[bx];

		if (p_brush->is_a_patch)
			continue; // PATCH, SKIP

		// REAL BRUSH
		for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
			brushrow_s *p_brushrow_vertex = &p_brush->brushrowlist.brushrow[brow];
			return p_brushrow_vertex->pbrtexture;
		} // brush row
	} // brush

	return NULL;
}

CALLERS_ (VM_map_move ==> entity_index_translate_brushes ==> here)
// Dig through brushes, if any textures is "common/origin" then it is.
qbool entity_has_originbrush(entityx_t *r_ent)
{
	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[bx];

		// BRUSH OR PATCH
		for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
			brushrow_s *pbr = &r_brush->brushrowlist.brushrow[brow];

			if (String_Match(pbr->pbrtexture, "common/origin"))
				return true;
		} // row
	} // brush
	return false;
}


//qbool entity_update_bbox_from_brushes_was_processed (entityx_t *d_ent)
//{
//	vec3_t brmins, brmaxs;
//	
//	VectorSet (brmins,  999999,   999999,   999999);
//	VectorSet (brmaxs, -999999,  -999999,  -999999);
//	qbool was_processed = entity_update_bbox_from_brushes_was_processed (d_ent, brmins, brmaxs);
//	if (!was_processed)
//		return false;
//	
//	VectorCopyDestSrc (d_ent->bbemins, brmins);
//	VectorCopyDestSrc (d_ent->bbemaxs, brmaxs);
//	VectorSubtract (d_ent->bbemaxs, d_ent->bbemins, d_ent->bbedims);
//	return true;
//}

qbool brush_update_bbox_from_rows_was_processed (brush_s *d_brush)
{
	VectorSet (d_brush->bxmins,  999999,   999999,   999999);
	VectorSet (d_brush->bxmaxs, -999999,  -999999,  -999999);

	// REAL BRUSH
	for (int brow = 0; brow < d_brush->brushrowlist.count; brow ++) {
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[brow];
		
		// FIGURING BBOX OF ACTUAL FACE
		VectorSet (d_face->bfmins,  999999,   999999,   999999);
		VectorSet (d_face->bfmaxs, -999999,  -999999,  -999999);

		bounds_expand	(d_face->a, d_face->bfmins, d_face->bfmaxs);
		bounds_expand	(d_face->b, d_face->bfmins, d_face->bfmaxs);
		bounds_expand	(d_face->c, d_face->bfmins, d_face->bfmaxs);

		// FINALIZE FACE DIMENSIONS
		VectorSubtract	(d_face->bfmaxs, d_face->bfmins, d_face->bfdims);

		// EXPANDING BRUSH METRIX
		bounds_expand	(d_face->bfmins, d_brush->bxmins, d_brush->bxmaxs);
		bounds_expand	(d_face->bfmaxs, d_brush->bxmins, d_brush->bxmaxs);
	} // brush row

	// FINALIZE BRUSH DIMENSIONS
	VectorSubtract		(d_brush->bxmaxs, d_brush->bxmins, d_brush->bxdims);
	return true;
}

// Returns false if entity has no brushes.
CALLERS_ (entitylist_perform_layout_from_boxes)
qbool entity_update_bbox_from_brushes_was_processed (entityx_t *d_ent)
{
	if (!d_ent->brushlist.count)
		return false;

	qbool did_find_any_brushes = false;

	vec3_t bbemins, bbemaxs;
	VectorSet (bbemins,  999999,   999999,   999999);
	VectorSet (bbemaxs, -999999,  -999999,  -999999);

	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		brush_s *d_brush = &d_ent->brushlist.brush[bx];

		if (d_brush->is_a_patch) {
			// PATCH - Translate all vertexes
			if (d_brush->patchrowlist.count && did_find_any_brushes == false)
				did_find_any_brushes = true;

			VectorSet (d_brush->bxmins,  999999,   999999,   999999);
			VectorSet (d_brush->bxmaxs, -999999,  -999999,  -999999);

			for (int prow = 0; prow < d_brush->patchrowlist.count; prow ++) {
				patchrow_s *ppr = &d_brush->patchrowlist.patchrow[prow];

				// FIGURING BBOX OF PATCH
				VectorSet (ppr->bpmins,  999999,   999999,   999999);
				VectorSet (ppr->bpmaxs, -999999,  -999999,  -999999);

				for (int pcol = 0; pcol < d_brush->cols; pcol ++) {
					float *pvec = &ppr->flots[pcol * 5 + 0];
					//VectorAdd (vadd, pvec, pvec); // Translate
					bounds_expand	(pvec, ppr->bpmins, ppr->bpmaxs);
					bounds_expand	(pvec, ppr->bpmins, ppr->bpmaxs);
					bounds_expand	(pvec, ppr->bpmins, ppr->bpmaxs);

					// FINALIZE FACE DIMENSIONS
					VectorSubtract	(ppr->bpmaxs, ppr->bpmins, ppr->bpdims);

					// EXPANDING BRUSH METRIX
					bounds_expand	(ppr->bpmins, d_brush->bxmins, d_brush->bxmaxs);
					bounds_expand	(ppr->bpmaxs, d_brush->bxmins, d_brush->bxmaxs);

				} // col
			} // row

			// FINALIZE PATCH DIMENSIONS
			VectorSubtract		(d_brush->bxmaxs, d_brush->bxmins, d_brush->bxdims);

			// EXPANDING ENTITY METRIC
			bounds_expand	(d_brush->bxmins, bbemins, bbemaxs); // Yes, bbemaxs is what expands
			bounds_expand	(d_brush->bxmaxs, bbemins, bbemaxs);

			continue; // WE ARE DONE WITH THIS BRUSH
		}
		
		if (d_brush->brushrowlist.count && did_find_any_brushes == false)
			did_find_any_brushes = true;

		VectorSet (d_brush->bxmins,  999999,   999999,   999999);
		VectorSet (d_brush->bxmaxs, -999999,  -999999,  -999999);

		// REAL BRUSH
		for (int brow = 0; brow < d_brush->brushrowlist.count; brow ++) {
			brushrow_s *d_face = &d_brush->brushrowlist.brushrow[brow];
			
			// FIGURING BBOX OF ACTUAL FACE
			VectorSet (d_face->bfmins,  999999,   999999,   999999);
			VectorSet (d_face->bfmaxs, -999999,  -999999,  -999999);

			bounds_expand	(d_face->a, d_face->bfmins, d_face->bfmaxs);
			bounds_expand	(d_face->b, d_face->bfmins, d_face->bfmaxs);
			bounds_expand	(d_face->c, d_face->bfmins, d_face->bfmaxs);

			// FINALIZE FACE DIMENSIONS
			VectorSubtract	(d_face->bfmaxs, d_face->bfmins, d_face->bfdims);

			// EXPANDING BRUSH METRIX
			bounds_expand	(d_face->bfmins, d_brush->bxmins, d_brush->bxmaxs);
			bounds_expand	(d_face->bfmaxs, d_brush->bxmins, d_brush->bxmaxs);
		} // brush row


		// FINALIZE BRUSH DIMENSIONS
		VectorSubtract		(d_brush->bxmaxs, d_brush->bxmins, d_brush->bxdims);

		// EXPANDING ENTITY METRIC
		bounds_expand	(d_brush->bxmins, bbemins, bbemaxs); // Yes, bbemaxs is what expands
		bounds_expand	(d_brush->bxmaxs, bbemins, bbemaxs);
	} // brush

patch_skip:
	// ADD A ORIGIN BRUSH from MINS/MAXS BBOX
	if (did_find_any_brushes) {
		// Remember, a patch only entity like a func_wall is possible
		// And we don't know how to metric that yet.
		VectorCopyDestSrc	(d_ent->bemins, bbemins);
		VectorCopyDestSrc	(d_ent->bemaxs, bbemaxs);
		VectorSubtract		(d_ent->bemaxs, d_ent->bemins, d_ent->bedims);
	}
	return did_find_any_brushes;
}



RELATED_ (entitylist_metric_world_brushes_nova)
void entity_metric_update (entityx_t *d_ent)
{
	//entityx_t	*d_ent = &plist->entity[ex];
	qbool was_processed = entity_update_bbox_from_brushes_was_processed (d_ent);
	if (!was_processed)
		return;

//	// EXPANDING ENTITY METRIC
//	bounds_expand	(d_ent->bemins, bbemins, bbemaxs); // Yes, bbemaxs is what expands
//	bounds_expand	(d_ent->bemaxs, bbemins, bbemaxs);
}
