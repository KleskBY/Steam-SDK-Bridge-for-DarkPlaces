// objsplit_adjust.c.h

RELATED_ (Mod_OBJ_Load R_ShaderPrint_f SV_ShowModel_f CL_EffectInfo_Dump_f R_Pak_This_Map_f)

typedef struct {
	char		*sname_za;		// The name of the group
	char		*susemtl_za;	// The name of the texture
	int32list_s	 facelist9;		// f  1039/1039/1039 1040/1040/1040 1041/1041/1041 (for triangles)
	float minz[3];// = { 9999999,  9999999,  9999999};
	float maxz[3];// = {-9999999, -9999999, -9999999};
	float sizz[3];
	float cent[3];
} gfaceset_s;

#define MASK_USED_NEG1 -1
#define MASK_UNUSED_ZERO_0 0
typedef struct {

	char *mtllib_za;

	gfaceset_s faceset[256];
	int faceset_count;

	int num_vertexs, num_faces;
	float minz[3];// = { 9999999,  9999999,  9999999};
	float maxz[3];// = {-9999999, -9999999, -9999999};
	float sizz[3];
	float cent[3];

	floatlist_s	v3; // v 0.123 0.234 0.345 1.0 v x y z w (yes w)
	floatlist_s vt3; // vt s t w // (yes w)
	floatlist_s vn3;
	int32list_s masklist;
	int ifilterplus1;	// -1 MEANS EVERYTHING EXCEPT 1  1 MEANS ONLY 1

	gfaceset_s *factive;
} gobj_s;

void facesetfreecontents (gobj_s *g)
{
	for (int n = 0; n < g->faceset_count; n ++) {
		gfaceset_s *faceitem = &g->faceset[n];
		mfreenull_ (faceitem->sname_za);
		mfreenull_ (faceitem->susemtl_za);
		int32s_freecontents (&faceitem->facelist9);
	}
	g->faceset_count = 0;
	mfreenull_ (g->mtllib_za);
	floats_freecontents (&g->v3);
	floats_freecontents (&g->vn3);
	floats_freecontents (&g->vt3);
	int32s_freecontents (&g->masklist);
	g->num_vertexs = 0, g->num_faces = 0;
	g->minz[0] = g->minz[1] = g->minz[2] = 9999999;
	g->maxz[0] = g->maxz[1] = g->maxz[2] = -9999999;
	g->sizz[0] = g->sizz[1] = g->sizz[2] = -1;
	g->cent[0] = g->cent[1] = g->cent[2] = -1;
	g->factive = NULL;
}

void facefill_recalc_face (gobj_s *g, int facegroup)
{
	gfaceset_s *facey = &g->faceset[facegroup];
	//FS_PrintLinef (f, "# group %d of %d", facegroup + 1, g->faceset_count);

	int32list_s *ints;
	ints = &facey->facelist9;// ccs *stitle = "vn";

	facey->minz[0] = facey->minz[1] = facey->minz[2] = 9999999;
	facey->maxz[0] = facey->maxz[1] = facey->maxz[2] = -9999999;
	facey->sizz[0] = facey->sizz[1] = facey->sizz[2] = -1;
	facey->cent[0] = facey->cent[1] = facey->cent[2] = -1;

	for (int fidx = 0; fidx < ints->count; fidx += 9) {
		// Grab each vertex and maxify
		for (int n = 0; n < 9; n += 3) {
			// vertex_index/texture_index/normal_index
			int triplet_vertex_id1			= ints->ints[fidx + n + 0]; // from face
			//int triplet_vertextext_id1	= ints->ints[fidx + n + 1]; // from face
			//int triplet_vertexnorm_id1	= ints->ints[fidx + n + 2]; // from face

			int triplet_idx0 = UNPLUS1(triplet_vertex_id1);
			int vertex_idx0 = triplet_idx0 * 3;

			float f0 = g->v3.floats[vertex_idx0 + 0];
			float f1 = g->v3.floats[vertex_idx0 + 1];
			float f2 = g->v3.floats[vertex_idx0 + 2];

			if (facey->maxz[0] < f0) facey->maxz[0] = f0;
			if (facey->maxz[1] < f1) facey->maxz[1] = f1;
			if (facey->maxz[2] < f2) facey->maxz[2] = f2;
			if (facey->minz[0] > f0) facey->minz[0] = f0;
			if (facey->minz[1] > f1) facey->minz[1] = f1;
			if (facey->minz[2] > f2) facey->minz[2] = f2;

		} // for
	} // fidx

	// Calc size, center
	facey->sizz[0] = facey->maxz[0] - facey->minz[0], facey->sizz[1] = facey->maxz[1] - facey->minz[1], facey->sizz[2] = facey->maxz[2] - facey->minz[2];
	facey->cent[0] = facey->minz[0] + facey->sizz[0] / 2.0, facey->cent[1] = facey->minz[1] + facey->sizz[1] / 2.0, facey->cent[2] = facey->minz[2] + facey->sizz[2] / 2.0;
}


void facefill_from_lines (gobj_s *g, stringlist_t *plines, float scaleup_or_zero)
{
	facesetfreecontents (g); // Reset, 99999s



	tokenize_console_s tcm = {0}, *tcx = &tcm;

	for (int n = 0; n < plines->numstrings; n ++) {
		char *s = plines->strings[n];

		int argc = Tokenize_Console_16384_Za_Return_Argc (tcx, s); // Clears tcx first

		ccs *arg0 = tcx->tokens_za[0];
		if (!arg0)
			continue;

		ccs *arg1 = tcx->tokens_za[1], *arg2 = tcx->tokens_za[2], *arg3 = tcx->tokens_za[3];

		if (String_Match (arg0, "v") ) { // v 131.965729 -75.185654 -260.450256
			float f0 = atof(arg1), f1 = atof(arg2), f2 = atof(arg3);
			floats_add3 (&g->v3, f0, f1, f2);
			g->num_vertexs ++;
			continue;
		}

		if (String_Match (arg0, "vt") ) { // vt  0.759000 0.616000 0.000000
			float f0 = atof(arg1), f1 = atof(arg2), f2 = atof(arg3);
			floats_add3 (&g->vt3, f0, f1, f2);
			continue;
		}

		if (String_Match (arg0, "vn") ) { // vn  0.000000 -0.000000 -1.000000
			float f0 = atof(arg1), f1 = atof(arg2), f2 = atof(arg3);
			floats_add3 (&g->vn3, f0, f1, f2);
			continue;
		}

		if (String_Match (arg0, "f") ) { // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
			// We are praying for f  1/1/1 2/2/2 3/3/3 (triangle)
			if (argc != 4) {
				Con_PrintLinef ("Line %d", n);
				Con_PrintLinef ("Text %s", s);
				Con_PrintLinef ("Face argc != 4");
				continue;
			}

			// These are integer values
			stringlist_t rlist = {0};
			stringlistappend_split (&rlist, arg1, "/");
			stringlistappend_split (&rlist, arg2, "/");
			stringlistappend_split (&rlist, arg3, "/");

			if (rlist.numstrings != 9) {
				Con_PrintLinef ("Line %d", n);
				Con_PrintLinef ("Text %s", s);
				Con_PrintLinef ("rlist.numstrings != 9");
				stringlistfreecontents (&rlist);
				continue;
			}

			ccs *s0 = rlist.strings[0],	*s1 = rlist.strings[1],	*s2 = rlist.strings[2];
			ccs *s3 = rlist.strings[3],	*s4 = rlist.strings[4],	*s5 = rlist.strings[5];
			ccs *s6 = rlist.strings[6],	*s7 = rlist.strings[7],	*s8 = rlist.strings[8];
			int32s_add3 (&g->factive->facelist9, atoi(s0), atoi(s1), atoi(s2));
			int32s_add3 (&g->factive->facelist9, atoi(s3), atoi(s4), atoi(s5));
			int32s_add3 (&g->factive->facelist9, atoi(s6), atoi(s7), atoi(s8));

			stringlistfreecontents (&rlist);
			continue;
		}

		if (String_Match (arg0, "g") && argc > 1) {
			Con_PrintLinef ("New group with name %s", arg1);
			Con_PrintLinef ("Factive was %s %d", g->factive ? g->factive->sname_za : "(NULL)", g->faceset_count - 1);
			if (g->factive) {
				Con_PrintLinef ("Closing %s with # faces = %d", g->factive->sname_za, g->factive->facelist9.count);
				Con_PrintLinef ("Closing %s with # faces / 9 = %f", g->factive->sname_za, g->factive->facelist9.count / 9.0);
			}
#if 0 // DEBUG
			gfaceset_s *faceitem = g->factive = &g->faceset[g->faceset_count];
#endif
			Con_PrintLinef ("New group with name %s idx %d", arg1, g->faceset_count);
			g->faceset_count ++;
			Z_StrDup_Realloc (&g->factive->sname_za, arg1);

			// g

			// 2916
			continue;
		}

		if (String_Match (arg0, "mtllib") && argc > 1) {
			Z_StrDup_Realloc (&g->mtllib_za, arg1);
			continue;
		}

		if (String_Match (arg0, "usemtl") && argc > 1) {
			if (g->factive) {
				Z_StrDup_Realloc (&g->factive->susemtl_za, arg1);
				Con_PrintLinef ("usemtl %s for %s", g->factive->susemtl_za, g->factive->sname_za);
			} else {
				Con_PrintLinef ("usemtl without factive %s", arg1);
			}
			continue;
		}

	} // for

	Tokenize_Console_16384_FreeContents (tcx); // CLOSED

	if (g->v3.count modulo 3) {
		Con_PrintLinef ("Bad verts count not div 3 %d", g->v3.count);
	}

	// Calc mins, maxs
	for (int n = 0; n < g->v3.count; n += 3) {
		float f0 = g->v3.floats[n + 0];
		float f1 = g->v3.floats[n + 1];
		float f2 = g->v3.floats[n + 2];
		if (g->maxz[0] < f0) g->maxz[0] = f0; if (g->maxz[1] < f1) g->maxz[1] = f1; if (g->maxz[2] < f2) g->maxz[2] = f2;
		if (g->minz[0] > f0) g->minz[0] = f0; if (g->minz[1] > f1) g->minz[1] = f1; if (g->minz[2] > f2) g->minz[2] = f2;
	} // for

	// Calc size, center
	g->sizz[0] = g->maxz[0] - g->minz[0], g->sizz[1] = g->maxz[1] - g->minz[1], g->sizz[2] = g->maxz[2] - g->minz[2];
	g->cent[0] = g->minz[0] + g->sizz[0] / 2.0, g->cent[1] = g->minz[1] + g->sizz[1] / 2.0, g->cent[2] = g->minz[2] + g->sizz[2] / 2.0;

	// Print summaries

	Con_PrintLinef ("# Zircon .obj export");
	Con_PrintLinef ("#");
	Con_PrintLinef ("mtllib %s", g->mtllib_za);
	Con_PrintLinef ("g");
	Con_PrintLinef ("v  count %8d div 3 %f", g->v3.count, g->v3.count / 3.0);
	Con_PrintLinef ("vt count %8d div 3 %f", g->vt3.count, g->vt3.count / 3.0);
	Con_PrintLinef ("vn count %8d div 3 %f", g->vn3.count, g->vn3.count / 3.0);

	Con_PrintLinef ("#");
	Con_PrintLinef ("mins " VECTOR3_5d1F, VECTOR3_SEND(g->minz) );
	Con_PrintLinef ("maxs " VECTOR3_5d1F, VECTOR3_SEND(g->maxz) );
	Con_PrintLinef ("size " VECTOR3_5d1F, VECTOR3_SEND(g->sizz) );
	Con_PrintLinef ("cent " VECTOR3_5d1F, VECTOR3_SEND(g->cent) );

	// Face groups
	//g Propeller
	//usemtl models/vehicles/helo1_palette.png
	//f  1/1/1 2/2/2 3/3/3

	for (int j = 0; j < g->faceset_count; j ++) {
		gfaceset_s *faceitem = &g->faceset[j];

		Con_PrintLinef ("g %s // idx %d", faceitem->sname_za, j);
		Con_PrintLinef ("usemtl %s", faceitem->susemtl_za);
		Con_PrintLinef ("faces = %d (div 9 = %f)", faceitem->facelist9.count, faceitem->facelist9.count / 9.0);

		// Calc group size
		// Calc group offset
	}

	Con_PrintLinef ("Centering verts ...");
	// Center verts
	for (int n = 0; n < g->v3.count; n += 3) {
		float f0 = g->v3.floats[n + 0];
		float f1 = g->v3.floats[n + 1];
		float f2 = g->v3.floats[n + 2];

		// Center first ..
		f0 -= g->cent[0];	f1 -= g->cent[1];	f2 -= g->cent[2];

		// Then scale
		if (scaleup_or_zero) {
			f0 *= scaleup_or_zero;	f1 *= scaleup_or_zero;	f2 *= scaleup_or_zero;
		}

		g->v3.floats[n + 0] = f0;
		g->v3.floats[n + 1] = f1;
		g->v3.floats[n + 2] = f2;
	} // for

	// Set mask count
	int32s_count_set (&g->masklist, g->v3.count);

	for (int n = 0; n < g->masklist.count; n ++) {
		g->masklist.ints[n] = MASK_UNUSED_ZERO_0;
	}

	for (int facegroup = 0; facegroup < g->faceset_count; facegroup ++) {
		gfaceset_s *facey = &g->faceset[facegroup];

		facefill_recalc_face (g, facegroup);

		Con_PrintLinef ("Group %d %s:", facegroup + 1, facey->sname_za);
		Con_PrintLinef ("mins " VECTOR3_5d1F, VECTOR3_SEND(facey->minz) );
		Con_PrintLinef ("maxs " VECTOR3_5d1F, VECTOR3_SEND(facey->maxz) );
		Con_PrintLinef ("size " VECTOR3_5d1F, VECTOR3_SEND(facey->sizz) );
		Con_PrintLinef ("cent " VECTOR3_5d1F, VECTOR3_SEND(facey->cent) );
	} // for

}

void facefill_from_string (gobj_s *g, ccs *s, float scaleup_or_zero)
{
	stringlist_t lines_list = {0};
	stringlistappend_split_lines_cr_scrub (&lines_list, s);

	facefill_from_lines (g, &lines_list, scaleup_or_zero); // resets

	stringlistfreecontents (&lines_list);
}

void face_reidx_setplus1(gobj_s *g, int *pd, int junk)
{
	int vertex_triplet_idx1 = *pd;
	int triplet_idx0 = UNPLUS1(vertex_triplet_idx1);
	int vertex_idx0 = triplet_idx0 * 3;
	int newidx = g->masklist.ints[vertex_idx0];
	// f 1/1/1 .. find the re-number.  So 303 becomes maybe 32
	*pd = newidx;// g->masklist.ints[n + 0]
}

void facefill_write_to_stream (gobj_s *g, qfile_t *f)
{
	// PLAN

	//# Noesis .obj export.
	//#
	//mtllib unused.mtl
	//g

	FS_PrintLinef (f, "# Zircon .obj export");
	FS_PrintLinef (f, "#");
	FS_PrintLinef (f, "mtllib %s", g->mtllib_za);
	FS_PrintLinef (f, "#");
	FS_PrintLinef (f, "# verts %8d", g->v3.count / 3);
	FS_PrintLinef (f, "# texco %8d", g->vt3.count / 3);
	FS_PrintLinef (f, "# norma %8d", g->vn3.count / 3);
	FS_PrintLinef (f, "#");
	FS_PrintLinef (f, "# mins   " VECTOR3_5d1F, VECTOR3_SEND(g->minz) );
	FS_PrintLinef (f, "# maxs   " VECTOR3_5d1F, VECTOR3_SEND(g->maxz) );
	FS_PrintLinef (f, "# size   " VECTOR3_5d1F, VECTOR3_SEND(g->sizz) );
	FS_PrintLinef (f, "# center " VECTOR3_5d1F, VECTOR3_SEND(g->cent) );
	FS_PrintLinef (f, "#");
	FS_PrintLinef (f, "# Groups = %d", g->faceset_count);
	int numtriangles = 0;
	for (int facegroup = 0; facegroup < g->faceset_count; facegroup ++) {
		gfaceset_s *facey = &g->faceset[facegroup];

		facefill_recalc_face (g, facegroup);

		FS_PrintLinef (f, "# Group %d %s:", facegroup, facey->sname_za);
		FS_PrintLinef (f, "#  triangles = %d", facey->facelist9.count / 9);
		FS_PrintLinef (f, "#  mins " VECTOR3_5d1F, VECTOR3_SEND(facey->minz) );
		FS_PrintLinef (f, "#  maxs " VECTOR3_5d1F, VECTOR3_SEND(facey->maxz) );
		FS_PrintLinef (f, "#  size " VECTOR3_5d1F, VECTOR3_SEND(facey->sizz) );
		FS_PrintLinef (f, "#  cent " VECTOR3_5d1F, VECTOR3_SEND(facey->cent) );
		numtriangles += (facey->facelist9.count / 9);
	} // for
	FS_PrintLinef (f, "# Total Triangles = %d", numtriangles);

	FS_PrintLinef (f, "g");

	// v 132.761963 -72.606422 -259.659790

	floatlist_s *fs; ccs *stitle;

	fs = &g->v3; stitle = "v";
	for (int n = 0; n < fs->count; n += 3) {
		if (g->ifilterplus1 && g->masklist.ints[n + 0] == MASK_UNUSED_ZERO_0)
			continue; // EXCLUDED

		float f0 = fs->floats[n + 0], f1 = fs->floats[n + 1], f2 = fs->floats[n + 2];
		FS_PrintLinef (f, "%s %f %f %f", stitle, f0, f1, f2);
	}
	fs = &g->vt3; stitle = "vt";
	for (int n = 0; n < fs->count; n += 3) {
		if (g->ifilterplus1 && g->masklist.ints[n + 0] == MASK_UNUSED_ZERO_0)
			continue; // EXCLUDED
		float f0 = fs->floats[n + 0], f1 = fs->floats[n + 1], f2 = fs->floats[n + 2];
		FS_PrintLinef (f, "%s %f %f %f", stitle, f0, f1, f2);
	}
	fs = &g->vn3; stitle = "vn";
	for (int n = 0; n < fs->count; n += 3) {
		if (g->ifilterplus1 && g->masklist.ints[n + 0] == MASK_UNUSED_ZERO_0)
			continue; // EXCLUDED

		float f0 = fs->floats[n + 0], f1 = fs->floats[n + 1], f2 = fs->floats[n + 2];
		FS_PrintLinef (f, "%s %f %f %f", stitle, f0, f1, f2);
	}

	// Face groups
	//g Propeller
	//usemtl models/vehicles/helo1_palette.png
	//f  1/1/1 2/2/2 3/3/3

	for (int facegroup = 0; facegroup < g->faceset_count; facegroup ++) {
		if (g->ifilterplus1 > 0) {
			// 1, 2, 3 ... means only that group
			int filter_idx0 = UNPLUS1(g->ifilterplus1);
			if (facegroup != filter_idx0)
				continue; // This group does not match "ONLY" group
		}

		if (g->ifilterplus1 < 0) {
			// 1, 2, 3 ... means only that group
			int filter_idx0 = UNPLUS1(abs(g->ifilterplus1));
			if (facegroup == filter_idx0)
				continue; // -GROUP means exclude that group
		}

		gfaceset_s *facey = &g->faceset[facegroup];

		FS_PrintLinef (f, "# group %d of %d", facegroup + 1, g->faceset_count);
		FS_PrintLinef (f, "g %s", facey->sname_za);
		FS_PrintLinef (f, "usemtl %s", facey->susemtl_za);

		int32list_s *ints;
		ints = &facey->facelist9;// ccs *stitle = "vn";
		for (int n = 0; n < ints->count; n += 9) {
			int d0 = ints->ints[n + 0], d1 = ints->ints[n + 1], d2 = ints->ints[n + 2];
			int d3 = ints->ints[n + 3], d4 = ints->ints[n + 4], d5 = ints->ints[n + 5];
			int d6 = ints->ints[n + 6], d7 = ints->ints[n + 7], d8 = ints->ints[n + 8];


			if (g->ifilterplus1) {
				// REIDX
				// So I have
				// f  2542/2542/2542 2543/2543/2543 2544/2544/2544
				// It probably writes as 1/1/1 2/2/2 3/3/3
				face_reidx_setplus1(g, &d0, n + 0);
				face_reidx_setplus1(g, &d1, n + 1);
				face_reidx_setplus1(g, &d2, n + 2);
				face_reidx_setplus1(g, &d3, n + 3);
				face_reidx_setplus1(g, &d4, n + 4);
				face_reidx_setplus1(g, &d5, n + 5);
				face_reidx_setplus1(g, &d6, n + 6);
				face_reidx_setplus1(g, &d7, n + 7);
				face_reidx_setplus1(g, &d8, n + 8);
			}

			FS_PrintLinef (f, "f %d/%d/%d %d/%d/%d %d/%d/%d", d0, d1, d2, d3, d4, d5, d6, d7, d8);
		} // for
	} // for

	FS_PrintLinef (f, "g");
}

// Set MASK ONLY

void facefill_calc_filter (gobj_s *g, int filter_idx1)
{

	int32list_s *ints;
	for (int facegroup = 0; facegroup < g->faceset_count; facegroup ++) {
		if (filter_idx1 > 0) {
			// 1, 2, 3 ... means only that group
			int filter_idx0 = UNPLUS1(filter_idx1);
			if (facegroup != filter_idx0)
				continue; // This group does not match "ONLY" group
		}

		if (filter_idx1 < 0) {
			// 1, 2, 3 ... means only that group
			int filter_idx0 = UNPLUS1(abs(filter_idx1));
			if (facegroup == filter_idx0)
				continue; // -GROUP means exclude that group
		}

		gfaceset_s *facey = &g->faceset[facegroup];
		ints = &facey->facelist9;// ccs *stitle = "vn";

		for (int fidx = 0; fidx < ints->count; fidx += 9) {
			// Grab each vertex and maxify
			for (int n = 0; n < 9; n += 3) {
				// vertex_index/texture_index/normal_index
				int triplet_vertex_id1			= ints->ints[fidx + n + 0]; // from face
				//int triplet_vertextext_id1	= ints->ints[fidx + n + 1]; // from face
				//int triplet_vertexnorm_id1	= ints->ints[fidx + n + 2]; // from face

				int triplet_idx0 = UNPLUS1(triplet_vertex_id1);
				int vertex_idx0 = triplet_idx0 * 3;

				g->masklist.ints[vertex_idx0 + 0] = MASK_USED_NEG1;

				//float f0 = g->v3.floats[vertex_idx0 + 0];
			} // for
		} // fidx
	} // facegroup

	// REIDX!
	int idx1 = 1; // The current value is the NEXT one assigned
	for (int n = 0; n < g->masklist.count; n += 3) {
		//int ndiv3 = n / 3;
		if (g->masklist.ints[n + 0] == MASK_USED_NEG1) {
			g->masklist.ints[n + 0] = idx1; idx1 ++; // ASSIGNED
		}
	} // for
}


// objmodelsplit models/vehicles/helo1.obj propeller_2
void CL_OBJModelSplit_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) < 2) {          // 1          2                 3
		Con_PrintLinef ("Usage: %s [obj file name] [group to split] [suffix]", Cmd_Argv(cmd, 0));
		return;
	}

	int is_write_file = isin2(Cmd_Argc(cmd), 3,4); // Go
	ccs *filter = Cmd_Argv(cmd, 2); // If too few args what is this ANSWER ""
	ccs *trailer = Cmd_Argc(cmd) == 4 ? Cmd_Argv(cmd, 3) : "_out";
	int ifilterp1 = atoi(filter);//Cmd_Argv(cmd, 2); // If too few args what is this

	//ccs *s_group = Cmd_Argv(cmd, 2); //

	const char *s_filename = Cmd_Argv(cmd, 1);
	fs_offset_t filesize;
	char *filedata_za = (char *)FS_LoadFile (s_filename, tempmempool, fs_quiet_true, &filesize);

	if (filedata_za == NULL) {
		Con_PrintLinef ("Couldn't open " QUOTED_S, s_filename);
		return;
	}

	gobj_s _g = {0}, *g = &_g;

	facefill_from_string (g, filedata_za, /*scale*/ 0);

	if (is_write_file) {
		if (ifilterp1) {
			g->ifilterplus1 = ifilterp1;
			facefill_calc_filter (g, g->ifilterplus1);
		}

		char s_filenameout[MAX_QPATH_128];
		c_strlcpy (s_filenameout, s_filename);
		File_URL_Edit_Remove_Extension (s_filenameout);
		//c_strlcat (s_filenameout, "_out");
		c_strlcat (s_filenameout, trailer); // "_out");
		c_strlcat (s_filenameout, ".obj");

		qfile_t *f = FS_OpenRealFile (s_filenameout, "wb", fs_quiet_FALSE); // WRITE-EON obj model adjust
		if (!f) {
			Con_PrintLinef ("Couldn't open file " QUOTED_S, s_filenameout);
			goto file_open_write_fail;
		}

		facefill_write_to_stream (g, f);

		FS_CloseNULL_ (f);

		Con_PrintLinef (CON_BRONZE "Wrote %s", s_filenameout);
	}

file_open_write_fail:
	facesetfreecontents (g); // Reset, 99999s

	Mem_FreeNull_ (filedata_za);
}



// Stage 1, print the file names
void CL_OBJModelAdjust2_f (cmd_state_t *cmd)
{
	if (Cmd_Argc(cmd) == 1) {
		Con_PrintLinef ("Usage: %s [obj file name] [scale]", Cmd_Argv(cmd, 0));
		return;
	}

	int is_write_file = Cmd_Argc(cmd) == 3;
	float scaleup = 0;
	if (Cmd_Argc(cmd) == 3) {
		scaleup = atof (Cmd_Argv(cmd, 2));
		if (scaleup <= 0) {
			Con_PrintLinef ("Invalid scale of %f", scaleup);
			return;
		}
		Con_PrintLinef ("Requested scaling: %f", scaleup);
	}

// New plan ... read every line, if it is a "v" line special action
	const char *s_filename = Cmd_Argv(cmd, 1);
	fs_offset_t filesize;
	char *filedata_za = (char *)FS_LoadFile (s_filename, tempmempool, fs_quiet_true, &filesize);

	if (filedata_za == NULL) {
		Con_PrintLinef ("Couldn't open " QUOTED_S, s_filename);
		return;
	}

	char s_filenameout[MAX_QPATH_128];
	c_strlcpy (s_filenameout, s_filename);
	File_URL_Edit_Remove_Extension (s_filenameout);
	c_strlcat (s_filenameout, "_out");
	c_strlcat (s_filenameout, ".obj");

	tokenize_console_s tcm = {0}, *tcx = &tcm;

	stringlist_t lines_list = {0};
	stringlistappend_split_lines_cr_scrub (&lines_list, filedata_za);

	int num_vertexs = 0, num_faces = 0;
	float minz[3] = { 9999999,  9999999,  9999999};
	float maxz[3] = {-9999999, -9999999, -9999999};

	for (int n = 0; n < lines_list.numstrings; n ++) {
		char *s = lines_list.strings[n];
		Tokenize_Console_16384_FreeContents (tcx);
		int argc = Tokenize_Console_16384_Za_Return_Argc (tcx, s);

		ccs *arg0 = tcx->tokens_za[0];

		if (!arg0) continue;
		ccs *arg1 = tcx->tokens_za[1];
		ccs *arg2 = tcx->tokens_za[2];
		ccs *arg3 = tcx->tokens_za[3];

		if (String_Match (arg0, "v") ) {
			// v
			float f0 = atof(arg1);
			float f1 = atof(arg2);
			float f2 = atof(arg3);

			if (maxz[0] < f0) maxz[0] = f0; if (maxz[1] < f1) maxz[1] = f1; if (maxz[2] < f2) maxz[2] = f2;
			if (minz[0] > f0) minz[0] = f0; if (minz[1] > f1) minz[1] = f1; if (minz[2] > f2) minz[2] = f2;

			num_vertexs ++;
		} else if (String_Match (arg0, "f") ) {
			// f
			num_faces ++;
		} else if (String_Match (arg0, "g") && argc > 1) {
			// g
			Con_PrintLinef ("New group with name %s", arg1);
		}
	} // for

	Tokenize_Console_16384_FreeContents (tcx);

	float sizz[3] = {0};
	float cent[3] = {0};
	sizz[0] = maxz[0] - minz[0];
	sizz[1] = maxz[1] - minz[1];
	sizz[2] = maxz[2] - minz[2];

	cent[0] = minz[0] + sizz[0] / 2;
	cent[1] = minz[1] + sizz[1] / 2;
	cent[2] = minz[2] + sizz[2] / 2;

	Con_PrintLinef ("Found %d vertexes", num_vertexs);
	Con_PrintLinef ("Found %d faces", num_faces);

	Con_PrintLinef ("=======");
	Con_PrintLinef ("Mins   is " VECTOR3_5d1F, VECTOR3_SEND(minz) );
	Con_PrintLinef ("Maxs   is " VECTOR3_5d1F, VECTOR3_SEND(maxz) );
	Con_PrintLinef ("Size   is " VECTOR3_5d1F, VECTOR3_SEND(sizz) );
	Con_PrintLinef ("Center is " VECTOR3_5d1F, VECTOR3_SEND(cent) );
	Con_PrintLinef ("=======");

	Con_PrintLinef ("To center, we subtract " VECTOR3_5d1F, VECTOR3_SEND(cent) );

	int model_idx = SV_ModelIndex (s_filename, PRECACHE_MODE_0); // Baker: 0 means we are not precaching

	if (model_idx != 0) {
		model_t	*mod = SV_GetModelByIndex(model_idx);
		Con_PrintLinef ("Model is precached as # %d", model_idx);
		Con_PrintLinef (" mod->surfmesh.num_triangles:   %d", mod->surfmesh.num_triangles);
		Con_PrintLinef (" mod->surfmesh.num_vertices:    %d", mod->surfmesh.num_vertices);
	} else {
		Con_PrintLinef ("Model is not precached");
	}

	if (is_write_file == false) {
		Con_PrintLinef ("Skipping file write because scale not specified");
		goto file_open_write_fail;
	}

	qfile_t *f = FS_OpenRealFile (s_filenameout, "wb", fs_quiet_FALSE); // WRITE-EON obj model adjust
	if (!f) {
		Con_PrintLinef ("Couldn't open file " QUOTED_S, s_filenameout);
		goto file_open_write_fail;
	}

	Con_PrintLinef ("Writing to " QUOTED_S, s_filenameout);

	for (int n = 0; n < lines_list.numstrings; n ++) {
		char *s = lines_list.strings[n];
		Tokenize_Console_16384_FreeContents (tcx);
		int argc = Tokenize_Console_16384_Za_Return_Argc (tcx, s);

		ccs *arg0 = tcx->tokens_za[0];
		if (argc >= 3 && arg0 && String_Match (arg0, "v") ) {
			float f0 = atof(tcx->tokens_za[1]);
			float f1 = atof(tcx->tokens_za[2]);
			float f2 = atof(tcx->tokens_za[3]);

			// Center first ..
			f0 -= cent[0];
			f1 -= cent[1];
			f2 -= cent[2];

			// Then scale
			f0 *= scaleup;
			f1 *= scaleup;
			f2 *= scaleup;

			FS_Printf (f, "v %f %f %f" NEWLINE, f0, f1, f2);
		} // "v"
		else {
			FS_Printf (f, "%s"  NEWLINE, s);
		}
	} // for

	FS_Close (f); f = NULL;

file_open_write_fail:
	Tokenize_Console_16384_FreeContents (tcx);
	stringlistfreecontents (&lines_list);
	Mem_FreeNull_ (filedata_za);
}
