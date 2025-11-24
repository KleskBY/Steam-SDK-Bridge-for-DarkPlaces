// oject_bbx_rectangle.c.h


// Draw enters with clip set to screen
// Q: Does this use the "draw list"?
RELATED_ (O_ListView_ColumnDrag Lister_Spawn Object_Draw_Recursive_Not_Form)
oject_s *O_Rectangle_Draw (oject_s *k)
{
	if (k->is_cutout)
		return k;

	if (k->is_outlined) {
		Draw_Rect		(&k->r_cutout_left, k->backcolor, k->backalpha);
		Draw_Rect		(&k->r_cutout_right, k->backcolor, k->backalpha);
		Draw_Rect		(&k->r_cutout_top, k->backcolor, k->backalpha);
		Draw_Rect		(&k->r_cutout_bottom, k->backcolor, k->backalpha);
		//Draw_Rect		(&k->r_cutout_left, color3_blue, k->backalpha);
		//Draw_Rect		(&k->r_cutout_right, color3_red, k->backalpha);
		//Draw_Rect		(&k->r_cutout_top, color3_green, k->backalpha);
		//Draw_Rect		(&k->r_cutout_bottom, color3_purple, k->backalpha);
	} else

	if (k->cm.kcutout) {
		// Cutout ... child tells container about it on refresh.
		// For now, rounded is not supported.
		//Draw_Rect		(&k->r_cutout_left, color3_blue, k->backalpha);
		//Draw_Rect		(&k->r_cutout_right, color3_red, k->backalpha);
		//Draw_Rect		(&k->r_cutout_top, color3_green, k->backalpha);
		//Draw_Rect		(&k->r_cutout_bottom, color3_purple, k->backalpha);
		Draw_Rect		(&k->r_cutout_left, k->backcolor, k->backalpha);
		Draw_Rect		(&k->r_cutout_right, k->backcolor, k->backalpha);
		Draw_Rect		(&k->r_cutout_top, k->backcolor, k->backalpha);
		Draw_Rect		(&k->r_cutout_bottom, k->backcolor, k->backalpha);

	} else {
		if (k->is_rounded)	
			Draw_RectRound	(&k->r_screen, k->backcolor, k->backalpha);
		else				
			Draw_Rect		(&k->r_screen, k->backcolor, k->backalpha);
	}
	return k;
}

RELATED_ (Object_Refresh_Recursive)
oject_s *O_Rectangle_Refresh (oject_s *k)
{
	// Tell parent they have a cutout ...
	// A form cannot have a cutout.  And really only a rect can.
	if (k->is_outlined) {
		float thick = Q_rint(k->cellpaddingxpct * k->cm.relative_rect.width);

		rect_s r_inner = {thick, thick, k->cm.relative_rect.width - thick * 2, k->cm.relative_rect.height - thick * 2};
		rect_s r_top	= {0,0, k->cm.relative_rect.width, r_inner.top};
		int bottom_height = k->cm.relative_rect.height - RECT_BOTTOMOF (r_inner);
		rect_s r_bottom = {0, RECT_BOTTOMOF(r_inner), k->cm.relative_rect.width, bottom_height};
		rect_s r_left = {0, r_inner.top, 
			r_inner.left, r_inner.height};
		int right_width = k->cm.relative_rect.width - RECT_RIGHTOF(r_inner);
		rect_s r_right = {RECT_RIGHTOF(r_inner), r_inner.top, right_width, r_inner.height};

		RECT_MOVE (r_top, k->r_screen.left, k->r_screen.top);
		RECT_MOVE (r_bottom, k->r_screen.left, k->r_screen.top);
		RECT_MOVE (r_left, k->r_screen.left, k->r_screen.top);
		RECT_MOVE (r_right, k->r_screen.left, k->r_screen.top);
		k->r_cutout_top = r_top;
		k->r_cutout_left = r_left;
		k->r_cutout_bottom = r_bottom;
		k->r_cutout_right = r_right;

	} else


	if (k->is_cutout && k->container != k->fctrl) {
		oject_s *kpar = k->container;
		if (kpar->r_screen.width == 0 || kpar->r_screen.height == 0) {
			Con_PrintLinef ("Cutoff with width or height == 0");
			return k;
		}
		k->container->cm.kcutout = k;

		// Assume parent is refreshed?  Apparently.
		rect_s r_top	= {0,0, kpar->cm.relative_rect.width, k->cm.relative_rect.top};
		int bottom_height = kpar->cm.relative_rect.height - RECT_BOTTOMOF (k->cm.relative_rect);
		rect_s r_bottom = {0, RECT_BOTTOMOF(k->cm.relative_rect), kpar->cm.relative_rect.width, bottom_height};
		rect_s r_left = {0, k->cm.relative_rect.top, 
			k->cm.relative_rect.left, k->cm.relative_rect.height};
		int right_width = kpar->cm.relative_rect.width - RECT_RIGHTOF( k->cm.relative_rect);
		rect_s r_right = {RECT_RIGHTOF(k->cm.relative_rect), k->cm.relative_rect.top, right_width, k->cm.relative_rect.height};

		
		if (isin1(kpar->po->enum_id, class_image)) {
			kpar->r_cutout_top_st0[0] = 0;
			kpar->r_cutout_top_st0[1] = 0;
			kpar->r_cutout_top_st1[0] = RECT_RIGHTOF(r_top) / (double)kpar->r_screen.width; // left / width of image.
			kpar->r_cutout_top_st1[1] = RECT_BOTTOMOF(r_top)/ (double)kpar->r_screen.height;

			kpar->r_cutout_left_st0[0] = r_left.left / (double)kpar->r_screen.width;
			kpar->r_cutout_left_st0[1] = r_left.top / (double)kpar->r_screen.height;
			kpar->r_cutout_left_st1[0] = RECT_RIGHTOF(r_left) / (double)kpar->r_screen.width;
			kpar->r_cutout_left_st1[1] = RECT_BOTTOMOF(r_left) / (double)kpar->r_screen.height;

			kpar->r_cutout_right_st0[0] = r_right.left / (double)kpar->r_screen.width;
			kpar->r_cutout_right_st0[1] = r_right.top / (double)kpar->r_screen.height;
			kpar->r_cutout_right_st1[0] = RECT_RIGHTOF(r_right)/ (double)kpar->r_screen.width;
			kpar->r_cutout_right_st1[1] = RECT_BOTTOMOF(r_right)/ (double)kpar->r_screen.height;

			kpar->r_cutout_bottom_st0[0] = r_bottom.left / (double)kpar->r_screen.width;
			kpar->r_cutout_bottom_st0[1] = r_bottom.top / (double)kpar->r_screen.height;
			kpar->r_cutout_bottom_st1[0] = 1.0;
			kpar->r_cutout_bottom_st1[1] = 1.0;
		} // 
		RECT_MOVE (r_top, kpar->r_screen.left, kpar->r_screen.top);
		RECT_MOVE (r_bottom, kpar->r_screen.left, kpar->r_screen.top);
		RECT_MOVE (r_left, kpar->r_screen.left, kpar->r_screen.top);
		RECT_MOVE (r_right, kpar->r_screen.left, kpar->r_screen.top);
		kpar->r_cutout_top = r_top;
		kpar->r_cutout_left = r_left;
		kpar->r_cutout_bottom = r_bottom;
		kpar->r_cutout_right = r_right;

		//va_super (scutpos, 128, "%f %f", RECT_RIGHTOF(r_left) * ( vid_conwidth.integer / (float)vid.width) , RECT_BOTTOMOF(r_top) * ( vid_conheight.integer / (float)vid.height) );
		//va_super (scutsize, 128, "%f %f", k->cm.relative_rect.width * ( vid_conwidth.integer / (float)vid.width), k->cm.relative_rect.height * ( vid_conheight.integer / (float)vid.height) );
		va_super (scutpos, 128, "%d %d", RECT_RIGHTOF(r_left) , RECT_BOTTOMOF(r_top)  );
		va_super (scutsize, 128, "%d %d", k->cm.relative_rect.width , k->cm.relative_rect.height  );
		//va_super (scutsize, 128, "%d %d", k->cm.relative_rect.width , k->cm.relative_rect.height  );

		extern cvar_t scr_xcutoutpos, scr_xcutoutsize;
		Cvar_SetQuick (&scr_xcutoutpos, scutpos);
		Cvar_SetQuick (&scr_xcutoutsize, scutsize);
	}
	

	//RECT_SET (kline->cm.relative_rect, 200, 200, 200, 200);
	return k;
}






