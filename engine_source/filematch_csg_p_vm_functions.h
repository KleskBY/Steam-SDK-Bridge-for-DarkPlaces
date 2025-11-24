// filematch_csg_p_vm_functions.h

// Baker: This does world also.

CALLERS_ (VM_map_move)
RELATED_			(brush_or_patch_tranlate_was_processed) // We call this
void entitylist_translate_brushes (entitylist_t *plist, vec3_t vadd)
{
	int num_moved = 0;
	for (int ex = ENTITY_WORLD_0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];
			qbool was_processed = brush_or_patch_tranlate_was_processed (p_brush, vadd);
			if (was_processed)
				num_moved ++;
		} // brush
	} // entities in .map

#if 0 //////////////////////////////////////////////// JUNE 2 2025
#if 0
		qbool is_origin_brush = (ex > 0) ? entity_has_originbrush (p_ent) : false;
#endif
		// Baker: Jan 1 2025 - The .map does not have origin yet.
		// This would be right to do for a compiled map.
#if 0
		if (is_origin_brush) // December 9 2024
			continue;
#endif

		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];
			if (p_brush->is_a_patch == false) {
				// REAL BRUSH
				for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
					brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];
					VectorAdd (vadd, pbr->a, pbr->a); // Translate
					VectorAdd (vadd, pbr->b, pbr->b); // Translate
					VectorAdd (vadd, pbr->c, pbr->c); // Translate
#if 1 // OCTOBER 3 2024 - TEXTURELOCK
					//             matrix factor * distance *
					// [ 1 0 0 459.715 ] [ 0 0 -1 -122.714 ] 0 0.7 0.35 134217728 0 0
					//if (String_Contains(pbr->pbrtexture, "armu")) {
					//	int j = 5;
					//}
					double xscale  = pbr->ftrail[FT_USCALE_1];  //
					double yscale  = pbr->ftrail[FT_VSCALE_2];
#if 0
					double xshift = pbr->xtra1[0] * vadd[0] / xscale;
					double yshift = pbr->xtra2[0] * vadd[2] / yscale;
#else
					double xshift = (pbr->xtra1[X1_UX_0] * -vadd[0] + pbr->xtra1[X1_UY_1] * -vadd[1] + pbr->xtra1[X1_UZ_2] * -vadd[2]) / xscale;
					double yshift = (pbr->xtra2[X2_VX_0] * -vadd[0] + pbr->xtra2[X2_VY_1] * -vadd[1] + pbr->xtra2[X2_VZ_2] * -vadd[2]) / yscale;
#endif
					double newxshift = pbr->xtra1[X1_UOFFSET_3] + xshift;
					double newyshift = pbr->xtra2[X2_VOFFSET_3] + yshift;
					pbr->xtra1[X1_UOFFSET_3] = newxshift;
					pbr->xtra2[X2_VOFFSET_3] = newyshift;

#endif
				} // row
			} else {
				// PATCH - Translate all vertexes
				for (int prow = 0; prow < p_brush->patchrowlist.count; prow ++) {
					patchrow_s *ppr = &p_brush->patchrowlist.patchrow[prow];
					for (int pcol = 0; pcol < p_brush->cols; pcol ++) {
						float *pvec = &ppr->flots[pcol * 5 + 0];
						VectorAdd (vadd, pvec, pvec); // Translate
					} // col
				} // row
			} // if
		} // brush

	} // entities in .map
#endif // JUNE 2 2025
}

RELATED_ (Generate_Dungeon_Map_File calls entitylist_append_map_with_translation calls us)
void entitylist_append_map (entitylist_t *dst_map, entitylist_t *src_map)
{
	// Add entity 0 brushes TO MAP1
	// Add entity ALL brushes and epairs to MAP1
	entitylist_brush0_append				(dst_map, src_map);	// no keys, just brushes.
	entitylist_nonworld_append				(dst_map, src_map);	// brushes and keys.
}

RELATED_ (Generate_Dungeon_Map_File) // OLDY
void entitylist_append_map_with_translation (entitylist_t *dst_map, entitylist_t *src_map, vec3_t translate_move)
{
	// Do we have a rotating door problem?  translate the origin and the brush, that's double.
	// The only way we can check is looking for common/origin texture name

	// If origin brush entity (per entity basis)
	// Do not translate the brushes.

	entitylist_translate_epairs_origin		(src_map, translate_move);
	entitylist_translate_brushes			(src_map, translate_move);
	entitylist_append_map					(dst_map, src_map);
}

// This is brush paste.  It does not do epairs or entities.
CALLERS_ (VM_map_append_map)
void entitylist_brush0_append (entitylist_t *plist, entitylist_t *paste)
{
	entityx_t	*r_ent = &paste->entity[0];	// R = READ
	entityx_t	*d_ent = &plist->entity[0];	// D = DEST

	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *r_brush = &r_ent->brushlist.brush[bx];	// READ BRUSH FROM SRC
		brush_s *d_brush = brushlist_add (&d_ent->brushlist); // CREATE BRUSH FOR DEST

		d_brush->is_a_patch = r_brush->is_a_patch;

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
//				int j = 5;
				memcpy (dpr->flots, rpr->flots, size_to_copy);
			}
		} // brush
	} // brushes in entity 0
}

CALLERS_ (VM_map_append_map)
void entitylist_nonworld_append (entitylist_t *plist, entitylist_t *paste)
{
	// ADD PASTE ENTS TO PLIST
	for (int rex = 1; rex < paste->count; rex ++) { // SKIP ENTITY 0
		entityx_t	*r_ent = &paste->entity[rex];	// R = READ
		entityx_t	*d_ent = entitylist_add(plist);	// D = DEST

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
	} // entities in .map
}

CALLERS_ (VM_map_append_map)

// CONCAT WITH NO BRUSHES - EPAIRS COPY - LIST PASTE HAS NO WORLD
void entitylist_epairs_only_concat (entitylist_t *plist, entitylist_t *paste)
{
	// ADD PASTE ENTS TO PLIST
	for (int rex = /* WORLD IS NOT ENTITY ZERO HERE */ 0; rex < paste->count; rex ++) {
		entityx_t	*r_ent = &paste->entity[rex];	// R = READ
		entityx_t	*d_ent = entitylist_add(plist);	// D = DEST

		for (int kx = 0; kx < r_ent->pairslist.numstrings; kx += 2) {
			ccs *key = r_ent->pairslist.strings[kx + 0];
			ccs *val = r_ent->pairslist.strings[kx + 1];

			stringlistappend (&d_ent->pairslist, key); // KEY
			stringlistappend (&d_ent->pairslist, val); // VALUE
		} // epairs
	} // entities in .map
}



// BakerString_Destroy_And_Null_It (&bs);

// Baker: April 15 2025 - Debugging command with no active callers.
// This copies the entities from a .map to the clipboard.
CALLERS_ (NONE as June 2 2025)
void entitylist_to_clipboard (entitylist_t *plist)
{
	baker_string_t *bsa = entitylist_maptext_bsalloc (plist);

#ifdef _DEBUG
	Clipboard_Set_Text (bsa->string);
#endif

	//Con_PrintLinef ("Clipboarded it strlen = %d -- done", (int)bsa->length);
	BakerString_Destroy_And_Null_It (&bsa);
}



RELATED_ (VM_map_move)
void entitylist_translate_epairs_origin (entitylist_t *plist, vec3_t vadd)
{
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];

			// We are only translating the origin key
			if (String_Match(key,"origin") == false)
				continue; // Not "origin"

			ccs *val = p_ent->pairslist.strings[kx + 1];

			vec3_t vorg;
#if 1
			Math_atov3 (val, /*out*/ vorg);
#else
			COM_Parse_Basic(&val);	vorg[0] = atof (com_token);
			COM_Parse_Basic(&val);	vorg[1] = atof (com_token);
			COM_Parse_Basic(&val);	vorg[2] = atof (com_token);
#endif
			VectorAdd (vadd, vorg, vorg); // Translate vorg = vadd + vorg

			// REWRITE
			char *newval = Z_StrDupf (VECTOR3_LOSSLESS, VECTOR3_SEND(vorg));  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
			Mem_FreeNull_ (p_ent->pairslist.strings[kx + 1]);
			p_ent->pairslist.strings[kx + 1] = newval;

			// ASSUME MAXIMUM OF A SINGLE "origin" per entity,
			// SO ..
			break;	// GET OUT!
		} // epairs
	} // entities in .map
}

// Returns number of brushes moved
// Baker: Surprisingly this is uncalled?  VM_map_move does entitylist_translate_brushes
RELATED_ (VM_map_move entitylist_translate_brushes)
int entity_index_translate_brushes (entitylist_t *plist, int entnum, vec3_t vadd)
{
	entityx_t	*p_ent = &plist->entity[entnum];
	int num_brushes_processed = 0;
	qbool is_origin_brush = (entnum > 0) ? entity_has_originbrush (p_ent) : false;

	if (is_origin_brush) // December 9 2024
		return num_brushes_processed; // which is zero

	for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
		brush_s *p_brush = &p_ent->brushlist.brush[bx];
		num_brushes_processed ++;
		if (p_brush->is_a_patch == false) {
			// REAL BRUSH
			for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];
				VectorAdd (vadd, pbr->a, pbr->a); // Translate
				VectorAdd (vadd, pbr->b, pbr->b); // Translate
				VectorAdd (vadd, pbr->c, pbr->c); // Translate

				#if 1 // OCTOBER 3 2024 - TEXTURELOCK
				//             matrix factor * distance *
				// [ 1 0 0 459.715 ] [ 0 0 -1 -122.714 ] 0 0.7 0.35 134217728 0 0
				//if (String_Contains(pbr->pbrtexture, "armu")) {
				//	int j = 5;
				//}
				double xscale  = pbr->ftrail[1];  //
				double yscale  = pbr->ftrail[2];
				double xshift = (pbr->xtra1[0] * -vadd[0] + pbr->xtra1[1] * -vadd[1] + pbr->xtra1[2] * -vadd[2]) / xscale;
				double yshift = (pbr->xtra2[0] * -vadd[0] + pbr->xtra2[1] * -vadd[1] + pbr->xtra2[2] * -vadd[2]) / yscale;

				double newxshift = pbr->xtra1[3] + xshift;
				double newyshift = pbr->xtra2[3] + yshift;
				pbr->xtra1[3] = newxshift;
				pbr->xtra2[3] = newyshift;

				#endif
			} // row
		} else {
			// PATCH - Translate all vertexes
			for (int prow = 0; prow < p_brush->patchrowlist.count; prow ++) {
				patchrow_s *ppr = &p_brush->patchrowlist.patchrow[prow];
				for (int pcol = 0; pcol < p_brush->cols; pcol ++) {
					float *pvec = &ppr->flots[pcol * 5 + 0];
					VectorAdd (vadd, pvec, pvec); // Translate
				} // col
			} // row
		} // if
	} // brush

	return num_brushes_processed;
}


RELATED_ (VM_map_texture_replace)
int entitylist_replace_texture (entitylist_t *plist, ccs *sfind, ccs *sreplace)
{
	int num_replaces = 0;
	for (int eidx = 0; eidx < plist->count; eidx ++) {
		entityx_t	*r_ent = &plist->entity[eidx];

		for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
			brush_s *r_brush = &r_ent->brushlist.brush[bx];

			// BRUSH OR PATCH
			for (int brow = 0; brow < r_brush->brushrowlist.count; brow ++) {
				brushrow_s *pbr = &r_brush->brushrowlist.brushrow[brow];

				if (String_Match(pbr->pbrtexture, sfind) == false)
					continue;

				Mem_FreeNull_ (pbr->pbrtexture);
				pbr->pbrtexture = Z_StrDup (sreplace);
				num_replaces ++;
			} // row
		} // brush
	} // for

	return num_replaces;
}
