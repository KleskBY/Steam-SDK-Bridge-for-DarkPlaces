// filematch_csg_p_brushrow.c.h

CALLERS_ (brush_metric_for_faces)
qbool brushrow_get_bbox_was_ok (brushrow_s *p_face)
{
//	qbool was_ok = false;
	VectorSet (p_face->bfmins,  999999,   999999,   999999);
	VectorSet (p_face->bfmaxs, -999999,  -999999,  -999999);

	// brushrow_s *p_brushrow_vertex = &p_brush->brushrowlist.brushrow[brow];
	bounds_expand (p_face->a, p_face->bfmins, p_face->bfmaxs);
	bounds_expand (p_face->b, p_face->bfmins, p_face->bfmaxs);
	bounds_expand (p_face->c, p_face->bfmins, p_face->bfmaxs);

	return true;
}
