// filematch_csg_parse_and_write.c.h

baker_string_t *entitylist_maptext_bsalloc (entitylist_t *plist)
{
//#pragma message ("Add a Zircon preprocess key to the worldspawn keys")
	//baker_string_t *bs_maptext = BakerString_Create_Malloc ("");
	baker_string_t *bs_maptext = BakerString_Create_Malloc_With_MegaBytes (100); // 100 MB
	
	for (int ex = 0; ex < plist->count; ex ++) {
		entityx_t	*p_ent = &plist->entity[ex];
		if (p_ent->is_considered_deleted)
			continue; // July 5 2025
		// Print keys
		//BakerString_CatCFmt (bs_maptext, "{ // Entity %d" NEWLINE, ex);
		BakerString_CatCFmt (bs_maptext, "{" NEWLINE);
		for (int kx = 0; kx < p_ent->pairslist.numstrings; kx += 2) {
			ccs *key = p_ent->pairslist.strings[kx + 0];
			ccs *val = p_ent->pairslist.strings[kx + 1];
			char sline[4096];
			c_dpsnprintf2 (sline, QUOTED_S " " QUOTED_S, key, val);
			BakerString_CatCFmt (bs_maptext, "%s" NEWLINE, sline);
		} // epairs
		for (int bx = 0; bx < p_ent->brushlist.count; bx ++) {
			brush_s *p_brush = &p_ent->brushlist.brush[bx];

			if (p_brush->is_considered_deleted) continue; // June 15 2025: Baker -- Deleted brush support

			BakerString_CatCFmt (bs_maptext, "{" NEWLINE);

			if (p_brush->is_a_patch == false) {
				// REAL BRUSH
				for (int brow = 0; brow < p_brush->brushrowlist.count; brow ++) {
					brushrow_s *pbr = &p_brush->brushrowlist.brushrow[brow];

					BakerString_CatCFmt (bs_maptext,
						"( " VECTOR3_G  " ) "
						"( " VECTOR3_G  " ) "
						"( " VECTOR3_G  " ) "
						" %s "
						"[ " VECTOR4_G " ] "
						"[ " VECTOR4_G " ] "
						VECTOR3_G,
						 VECTOR3_SEND(pbr->a),
						 VECTOR3_SEND(pbr->b),
						 VECTOR3_SEND(pbr->c),
						 pbr->pbrtexture,
						 VECTOR4_SEND(pbr->xtra1),
						 VECTOR4_SEND(pbr->xtra2),
						 VECTOR3_SEND(pbr->ftrail)
						 );

					// Any extras add a space
					for (int j = 3; j < pbr->trail_count; j ++) {
						BakerString_CatCFmt (bs_maptext, " " FLOAT_LOSSLESS_FORMAT, pbr->ftrail[j] );
					}
					BakerString_CatC (bs_maptext, NEWLINE);
				} // brushrow
			} else {
i_am_patch:
					BakerString_CatCFmt (bs_maptext, "patchDef2" NEWLINE);
					BakerString_CatCFmt (bs_maptext, "{" NEWLINE);
					// caves/3i_sand2
					BakerString_CatCFmt (bs_maptext, "%s" NEWLINE, p_brush->texture);
					// ( 9 3 0 0 0 )
					BakerString_CatCFmt (bs_maptext, "( " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT " " FLOAT_LOSSLESS_FORMAT " )" NEWLINE, p_brush->rows, p_brush->cols, p_brush->other3[0], p_brush->other3[1], p_brush->other3[2]);
					BakerString_CatCFmt (bs_maptext, "(" NEWLINE);

					for (int prow = 0; prow < p_brush->patchrowlist.count; prow ++) {
						patchrow_s *ppr = &p_brush->patchrowlist.patchrow[prow];
						BakerString_CatCFmt (bs_maptext, "( " NEWLINE);
						for (int pcol = 0; pcol < p_brush->cols; pcol ++) {
							// These don't perfectly match J.A.C.K. output which has extra zeroes
							// in the decimal places sometimes.
							BakerString_CatCFmt (bs_maptext, "( %g %g %g %8g %8g )" NEWLINE,
								ppr->flots[pcol * 5 + 0],
								ppr->flots[pcol * 5 + 1],
								ppr->flots[pcol * 5 + 2],
								ppr->flots[pcol * 5 + 3],
								ppr->flots[pcol * 5 + 4]
							);
						}
						BakerString_CatCFmt (bs_maptext, " )" NEWLINE);
					}

					BakerString_CatCFmt (bs_maptext, ")" NEWLINE);
					BakerString_CatCFmt (bs_maptext, "}" NEWLINE);
			}

			BakerString_CatCFmt (bs_maptext, "}" NEWLINE);
		} // brush
		BakerString_CatCFmt (bs_maptext, "}" NEWLINE);
	} // entities in .map
	return bs_maptext;
}


