// oject_bbx_imagegrid.c.h

// First stop is no scroll list.
// Need to calculate the cell positions
// Draw the image list


// It is ok for the selected idx to fall out of range if the scrollbar did it.

//
// 0  =====   -8  BOB     8 =======
//    BOB		  ======    MARY
//    MARY        MARY      TODD

RELATED_ (O_ListView_ColumnDrag Lister_Spawn Object_Draw_Recursive_Not_Form)
oject_s *O_ImageGrid_Draw (oject_s *k)
{
	//k->gridcolumns, k->gridrows, k->gridcellpadding, k->gridcellspacing;
	//k->rowscount, k->columncount;
	Draw_Rect		(&k->r_screen, k->backcolor, k->backalpha);

	int iter_x = k->gridcellwidth + k->gridcellspacing;
	int iter_y = k->gridcellheight + k->gridcellspacing;
	int idx = 0;
	for (int r = 0; r < k->gridrows; r ++) {

		for (int c = 0; c < k->gridcolumns; c ++, idx ++) {

			rect_s rcell = k->tb.r_button;
			rect_s rimag = k->tb.r_leftbar;

			rcell.left += (iter_x * c);
			rcell.top += (iter_y * r);
			ccs *simg = NULL;
			if (idx < k->imagelist_a.numstrings && k->imagelist_a.strings[idx][0]) {
				simg = k->imagelist_a.strings[idx];
			}

			k->tb.r_button;
			Draw_Rect (&rcell, k->gridcellbackcolor, k->backalpha);
			if (simg) {
				cachepic_t *p = Draw_CachePic_Flags (simg, CACHEPICFLAG_NOTPERSISTENT);
				Draw_ImageCachePic (&rimag, p, k->imagecolor, k->imagealpha);
			}

		}

	}
	return k;
}

RELATED_ (Object_Refresh_Recursive)
oject_s *O_ImageGrid_Refresh (oject_s *k)
{
#ifdef _DEBUG
	RELATED_ (___VarObjectFont)
	VarMyFont(myfont); // Picks the font index
#endif
//	int listcount = k->imagelist_a.numstrings;

	//k->gridcolumns, k->gridrows, k->gridcellpadding, k->gridcellspacing;
	//k->rowscount, k->columncount;
	RELATED_ (O_Rectangle_Refresh)
	k->tb.r_button.left		= k->cm.relative_rect.left + k->gridmargin;
	k->tb.r_button.top		= k->cm.relative_rect.top + k->gridmargin;
	k->tb.r_button.width	= k->gridcellwidth;
	k->tb.r_button.height	= k->gridcellheight;;


	// r_leftbar As interior .. will move k->gridcellwidth + k->gridcellspacing
	k->tb.r_leftbar = k->tb.r_button;
	RECT_MOVE (k->tb.r_leftbar, k->gridcellpadding, k->gridcellpadding);
	k->tb.r_leftbar.width -= k->gridcellpadding * 2;
	k->tb.r_leftbar.height -= k->gridcellpadding * 2;

#ifdef _DEBUG
	int canvaswidth = k->gridmargin + (k->gridcolumns * k->gridcellwidth) +
		( (k->gridcolumns - 1) * k->gridcellspacing) + k->gridmargin;

	int canvasheight = k->gridmargin + (k->gridrows * k->gridcellheight) +
		( (k->gridrows - 1) * k->gridcellspacing) + k->gridmargin;
#endif
	return k;
}

