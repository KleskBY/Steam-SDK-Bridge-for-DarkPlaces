// oject_bbx_image.c.h




oject_s *O_Image_Draw (oject_s *k)
{
	//if (k->image_name_a && !k->image_name_a[0]) {
	//	int j = 5; // Never hits
	//}
	if (k->cm.kcutout) {
		if (k->image_name_a) {
			cachepic_t *p = Draw_CachePic_Flags (k->image_name_a, CACHEPICFLAG_NOTPERSISTENT);
			// Cutout ... child tells container about it on refresh.
			// For now, rounded is not supported.
			Draw_ImageCachePic_ST01 (&k->r_cutout_top, p, k->imagecolor, k->imagealpha, k->r_cutout_top_st0, k->r_cutout_top_st1);
			Draw_ImageCachePic_ST01 (&k->r_cutout_left, p, k->imagecolor, k->imagealpha, k->r_cutout_left_st0, k->r_cutout_left_st1);
			Draw_ImageCachePic_ST01 (&k->r_cutout_right, p, k->imagecolor, k->imagealpha, k->r_cutout_right_st0, k->r_cutout_right_st1);
			Draw_ImageCachePic_ST01 (&k->r_cutout_bottom, p, k->imagecolor, k->imagealpha, k->r_cutout_bottom_st0, k->r_cutout_bottom_st1);		
		}
		return k;
	}
	if (k->backalpha) {
		Draw_Rect	(&k->r_screen, k->backcolor, k->backalpha);
	}

	if (k->image_name_a) {
		cachepic_t *p = Draw_CachePic_Flags (k->image_name_a, CACHEPICFLAG_NOTPERSISTENT);
		Draw_ImageCachePic (&k->r_screen, p, k->imagecolor, k->imagealpha);
	}
	return k;
}


