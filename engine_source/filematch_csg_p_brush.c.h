// filematch_csg_p_brush.c.h

// Returns false if a patch ...
qbool brush_or_patch_tranlate_was_processed (brush_s *p_brush, vec3_t vadd)
{
	if (p_brush->is_a_patch) {
		// PATCH - Translate all vertexes
		for (int prow = 0; prow < p_brush->patchrowlist.count; prow ++) {
			patchrow_s *ppr = &p_brush->patchrowlist.patchrow[prow];
			for (int pcol = 0; pcol < p_brush->cols; pcol ++) {
				float *pvec = &ppr->flots[pcol * 5 + 0];
				VectorAdd (vadd, pvec, pvec); // Translate
			} // col
		} // row
		return true;
	}

	// REAL BRUSH
	for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
		brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];
		VectorAdd (vadd, pbr->a, pbr->a); // Translate
		VectorAdd (vadd, pbr->b, pbr->b); // Translate
		VectorAdd (vadd, pbr->c, pbr->c); // Translate
		// OCTOBER 3 2024 - TEXTURELOCK
		double xscale  = pbr->ftrail[FT_USCALE_1];
		double yscale  = pbr->ftrail[FT_VSCALE_2];
		double xshift = (pbr->xtra1[X1_UX_0] * -vadd[0] + pbr->xtra1[X1_UY_1] * -vadd[1] + pbr->xtra1[X1_UZ_2] * -vadd[2]) / xscale;
		double yshift = (pbr->xtra2[X2_VX_0] * -vadd[0] + pbr->xtra2[X2_VY_1] * -vadd[1] + pbr->xtra2[X2_VZ_2] * -vadd[2]) / yscale;
		double newxshift = pbr->xtra1[X1_UOFFSET_3] + xshift;
		double newyshift = pbr->xtra2[X2_VOFFSET_3] + yshift;
		pbr->xtra1[X1_UOFFSET_3] = newxshift;
		pbr->xtra2[X2_VOFFSET_3] = newyshift;
	} // row

	return true;
}

// An entity can have multiple brushes.
// Returns true on success
// Better be a brush.


// No Interaction with patches, returns false
RELATED_ (superior is brush_update_bbox_from_rows_was_processed)
qbool brush_get_bbox_was_processed (brush_s *p_brush, vec3_t brmins, vec3_t brmaxs)
{
	if (p_brush->is_a_patch)
		return false; // PATCH, SKIP

	qbool was_ok = false;
	VectorSet (brmins,  999999,   999999,   999999);
	VectorSet (brmaxs, -999999,  -999999,  -999999);

	// REAL BRUSH
	for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
		brushrow_s *p_face = &p_brush->brushrowlist.brushrow[brow];

		// EXPANDING BRUSH METRIX
		bounds_expand (p_face->a, brmins, brmaxs);
		bounds_expand (p_face->b, brmins, brmaxs);
		bounds_expand (p_face->c, brmins, brmaxs);

		// FIGURING BBOX OF ACTUAL FACE
		VectorSet (p_face->bfmins,  999999,   999999,   999999);
		VectorSet (p_face->bfmaxs, -999999,  -999999,  -999999);

		bounds_expand (p_face->a, p_face->bfmins, p_face->bfmaxs);
		bounds_expand (p_face->b, p_face->bfmins, p_face->bfmaxs);
		bounds_expand (p_face->c, p_face->bfmins, p_face->bfmaxs);

		if (!was_ok) was_ok = true;
	} // brush row

	return was_ok;
}

#if 0
CALLERS_ ()
void brush_metric_for_faces (brush_s *p_brush)
{
	// Must calc brush metrics first for normalize.

	// Take a brush with 6 rows.  Subtract the vertexes for a b c.
	for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
		brushrow_s *p_row = &p_brush->brushrowlist.brushrow[brow];

		// Make it zero based.
		VectorSubtract (p_row->a, p_brush->bxmins, p_row->a); // c = a - b
		VectorSubtract (p_row->b, p_brush->bxmins, p_row->b); // c = a - b
		VectorSubtract (p_row->c, p_brush->bxmins, p_row->c); // c = a - b

		qbool was_ok = brushrow_get_bbox_was_ok (p_row);

		if (!was_ok)
			continue;

		p_row->bfdims[0] = p_row->bfmaxs[0] - p_row->bfmins[0];
		p_row->bfdims[1] = p_row->bfmaxs[1] - p_row->bfmins[1];
		p_row->bfdims[2] = p_row->bfmaxs[2] - p_row->bfmins[2];

	} // brow in .map

	// Now determine quadrants
	for (int brow_idx = 0; brow_idx < p_brush->brushrowlist.count; brow_idx ++) {
		brushrow_s *p_row = &p_brush->brushrowlist.brushrow[brow_idx];

		// WEST
		while (1) {
			int best_face_idx = UNPLUS1(p_brush->quadrants_brush_faces_plus1[quadrant_west_0]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				p_brush->quadrants_brush_faces_plus1[quadrant_west_0] = PLUS1(brow_idx);
				break;
			}

			// The west is the least east = lowest X maxs.
			brushrow_s *p_best = &p_brush->brushrowlist.brushrow[best_face_idx];
			if (p_row->bfmaxs[0] < p_best->bfmaxs[0]) { // GREATER
				p_brush->quadrants_brush_faces_plus1[quadrant_west_0] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// EAST
		while (1) {
			int best_face_idx = UNPLUS1(p_brush->quadrants_brush_faces_plus1[quadrant_east_1]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				p_brush->quadrants_brush_faces_plus1[quadrant_east_1] = PLUS1(brow_idx);
				break;
			}
			// The east is the least west = highest X mins.
			brushrow_s *p_best = &p_brush->brushrowlist.brushrow[best_face_idx];
			if (p_row->bfmins[0] > p_best->bfmins[0]) { // GREATER
				p_brush->quadrants_brush_faces_plus1[quadrant_east_1] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// SOUTH
		while (1) {
			int best_face_idx = UNPLUS1(p_brush->quadrants_brush_faces_plus1[quadrant_south_2]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win
				p_brush->quadrants_brush_faces_plus1[quadrant_south_2] = PLUS1(brow_idx);
				break;
			}
			// The south is the least north = lowest Y maxs.
			brushrow_s *p_best = &p_brush->brushrowlist.brushrow[best_face_idx];
			if (p_row->bfmaxs[1] < p_best->bfmaxs[1]) { // GREATER
				p_brush->quadrants_brush_faces_plus1[quadrant_south_2] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// NORTH
		while (1) {
			int best_face_idx = UNPLUS1(p_brush->quadrants_brush_faces_plus1[quadrant_north_3]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				p_brush->quadrants_brush_faces_plus1[quadrant_north_3] = PLUS1(brow_idx);
				break;
			}
			// The north is the least wouth = highest Y mins.
			brushrow_s *p_best = &p_brush->brushrowlist.brushrow[best_face_idx];
			if (p_row->bfmins[1] > p_best->bfmins[1]) { // GREATER
				p_brush->quadrants_brush_faces_plus1[quadrant_north_3] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// FLOOR
		while (1) {
			int best_face_idx = UNPLUS1(p_brush->quadrants_brush_faces_plus1[quadrant_floor_4]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				p_brush->quadrants_brush_faces_plus1[quadrant_floor_4] = PLUS1(brow_idx);
				break;
			}
			// The floor is the least high = lowest Z maxs.
			brushrow_s *p_best = &p_brush->brushrowlist.brushrow[best_face_idx];
			if (p_row->bfmaxs[2] < p_best->bfmaxs[2]) { // GREATER
				p_brush->quadrants_brush_faces_plus1[quadrant_floor_4] = PLUS1(brow_idx);

				break;
			}

			break;
		}

		// CEILING
		while (1) {
			int best_face_idx = UNPLUS1(p_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				p_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5] = PLUS1(brow_idx);
				break;
			}
			// The ceiling is the least downwards = highest Z mins.
			brushrow_s *p_best = &p_brush->brushrowlist.brushrow[best_face_idx];
			if (p_row->bfmins[2] > p_best->bfmins[2]) { // GREATER
				p_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5] = PLUS1(brow_idx);
				break;
			}
			break;
		}
#ifdef _DEBUG
		int j = 6;
#endif
	} // brow in .map
#ifdef _DEBUG
	int j = 6;
#endif
}
#endif

// For a 6 sided brush, identifies quadrants
RELATED_ (entitylist_metric_world_brushes_nova)

RELATED_ (entity_layoutbox_identify_world_brush_quadrants) // Creates brush quadrants for world.

void brush_box_identify_face_quadrants (brush_s *d_brush)
{
	// Now determine quadrants for each row
	for (int brow_idx = 0; brow_idx < d_brush->brushrowlist.count; brow_idx ++) {
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[brow_idx];

		// WEST
		while (1) {
			int best_face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_west_0]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				d_brush->quadrants_brush_faces_plus1[quadrant_west_0] = PLUS1(brow_idx);
				break;
			}

			// The west is the least east = lowest X maxs.
			brushrow_s *p_best = &d_brush->brushrowlist.brushrow[best_face_idx];
			if (d_face->bfmaxs[0] < p_best->bfmaxs[0]) { // GREATER
				d_brush->quadrants_brush_faces_plus1[quadrant_west_0] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// EAST
		while (1) {
			int best_face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_east_1]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				d_brush->quadrants_brush_faces_plus1[quadrant_east_1] = PLUS1(brow_idx);
				break;
			}
			// The east is the least west = highest X mins.
			brushrow_s *p_best = &d_brush->brushrowlist.brushrow[best_face_idx];
			if (d_face->bfmins[0] > p_best->bfmins[0]) { // GREATER
				d_brush->quadrants_brush_faces_plus1[quadrant_east_1] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// SOUTH
		while (1) {
			int best_face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_south_2]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win
				d_brush->quadrants_brush_faces_plus1[quadrant_south_2] = PLUS1(brow_idx);
				break;
			}
			// The south is the least north = lowest Y maxs.
			brushrow_s *p_best = &d_brush->brushrowlist.brushrow[best_face_idx];
			if (d_face->bfmaxs[1] < p_best->bfmaxs[1]) { // GREATER
				d_brush->quadrants_brush_faces_plus1[quadrant_south_2] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// NORTH
		while (1) {
			int best_face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_north_3]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				d_brush->quadrants_brush_faces_plus1[quadrant_north_3] = PLUS1(brow_idx);
				break;
			}
			// The north is the least wouth = highest Y mins.
			brushrow_s *p_best = &d_brush->brushrowlist.brushrow[best_face_idx];
			if (d_face->bfmins[1] > p_best->bfmins[1]) { // GREATER
				d_brush->quadrants_brush_faces_plus1[quadrant_north_3] = PLUS1(brow_idx);
				break;
			}
			break;
		}

		// FLOOR
		while (1) {
			int best_face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_floor_4]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				d_brush->quadrants_brush_faces_plus1[quadrant_floor_4] = PLUS1(brow_idx);
				break;
			}
			// The floor is the least high = lowest Z maxs.
			brushrow_s *p_best = &d_brush->brushrowlist.brushrow[best_face_idx];
			if (d_face->bfmaxs[2] < p_best->bfmaxs[2]) { // GREATER
				d_brush->quadrants_brush_faces_plus1[quadrant_floor_4] = PLUS1(brow_idx);

				break;
			}

			break;
		}

		// CEILING
		while (1) {
			int best_face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5]); // 0 ==> -1
			if (best_face_idx == not_found_neg1) {
				// Automatic win by default
				d_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5] = PLUS1(brow_idx);
				break;
			}
			// The ceiling is the least downwards = highest Z mins.
			brushrow_s *p_best = &d_brush->brushrowlist.brushrow[best_face_idx];
			if (d_face->bfmins[2] > p_best->bfmins[2]) { // GREATER
				d_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5] = PLUS1(brow_idx);
				break;
			}
			break;
		}
#ifdef _DEBUG
		int j = 6;
#endif
	} // brow in .map

	if (d_brush->quadrants_brush_faces_plus1[quadrant_north_3]) {
		int face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_north_3]);
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[face_idx];
		Flag_Add_To (d_face->nsewcf_bits, nsewcf_north_1);
	}

	if (d_brush->quadrants_brush_faces_plus1[quadrant_south_2]) {
		int face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_south_2]);
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[face_idx];
		Flag_Add_To (d_face->nsewcf_bits, nsewcf_south_2);
	}

	if (d_brush->quadrants_brush_faces_plus1[quadrant_east_1]) {
		int face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_east_1]);
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[face_idx];
		Flag_Add_To (d_face->nsewcf_bits, nsewcf_east_3);
	}

	if (d_brush->quadrants_brush_faces_plus1[quadrant_west_0]) {
		int face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_west_0]);
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[face_idx];
		Flag_Add_To (d_face->nsewcf_bits, nsewcf_west_4);
	}

	if (d_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5]) {
		int face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_ceiling_5]);
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[face_idx];
		Flag_Add_To (d_face->nsewcf_bits, nsewcf_ceil_5);
	}

	if (d_brush->quadrants_brush_faces_plus1[quadrant_floor_4]) {
		int face_idx = UNPLUS1(d_brush->quadrants_brush_faces_plus1[quadrant_floor_4]);
		brushrow_s *d_face = &d_brush->brushrowlist.brushrow[face_idx];
		Flag_Add_To (d_face->nsewcf_bits, nsewcf_floor_6);
	}


#ifdef _DEBUG
	int j = 6;
#endif
}


