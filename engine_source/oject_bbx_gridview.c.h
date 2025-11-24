// oject_bbx_gridview.c.h

// GridCellWidth
// GridCellHeight
// GridColumns
// RowsColumns
// Margin?
// ImageList ... a simple list of strings.
// Values?  An integer list.
// It needs to be like a spread sheet.
// It needs option to display hard column and row toppers.

oject_s *O_GridView_Spawn (oject_s *k)
{

	return k;//Lister_Spawn(k);
}

CALLERS_ (Property_Set_Float class_label O_Label_Draw)

RELATED_ (ViewCalc_Set)
oject_s *O_GridView_Draw (oject_s *k)
{
	Draw_Rect	(&k->r_screen, k->backcolor, alpha_1_0);
	//Draw_Rect	(&k->r_interior, k->backcolor, alpha_1_0);
	//
	//rect_s r_cursor = k->viewcalc.r_row_first;	// updated in refresh
	//int start_idx	= k->viewcalc.first_vis_row;
	//int count		= k->viewcalc.rowcount;
	//int beyond_idx	= start_idx + count;

	//for (int n = start_idx; n < beyond_idx; n ++, r_cursor.top += k->viewcalc.rowheight) {
	//	const char *s = k->list_strings_a.strings[n];
	//	
	//	int		is_selected		= (n == k->selectedindex);
	//	vec_t	*forecolor		= is_selected ? k->forecolorselected : k->forecolor;

	//	if (is_selected)
	//		Draw_Rect	(&r_cursor, k->backcolorselected, alpha_1_0);
	//	Draw_String	(&r_cursor, s, k->fontsize, forecolor, alpha_1_0);
	//} // for

	return k;
}



//// Inheritance from form is important -- fontsize, etc.
//
//oject_s *O_GridView_Refresh (oject_s *k)
//{
//	return k; //Lister_Refresh (k);
//}


oject_s *O_GridView_MouseButtonAction (oject_s *k, int x, int y, int isdown)
{
	return k;//Lister_MouseButtonAction (k, x, y, isdown);
}


// Return NULL if input not processed.
oject_s *O_GridView_KeyDown (oject_s *k, int key, int ascii)
{
	RELATED_ (Lister_KeyDown)
	//int listcount = Lister_ListCount (k);
	//if (listcount == 0) return NULL; // No listcount

	//qbool is_paging = false; // Paging, we want the new index to be top or bottom.
	//int oldsel = k->selectedindex;

	//switch (key) {
	//case K_UPARROW:		k->selectedindex --;	break;
	//case K_DOWNARROW:	k->selectedindex ++;	break;
	//case K_MWHEELUP:	k->selectedindex -=1; break;
	//case K_MWHEELDOWN:	k->selectedindex +=1; break;
	//case K_HOME:		k->selectedindex = 0; break;
	//case K_PGUP:		k->selectedindex -= k->viewcalc.pageamount; is_paging = true; break;
	//case K_PGDN:		k->selectedindex += k->viewcalc.pageamount; is_paging = true; break;
	//case K_END:			k->selectedindex = listcount - 1; break;

	//default:		return NULL; // NO ACTION
	//} // sw

	//// If we came here, we did something
	//vclamp (k->selectedindex, 0, listcount - 1);

	//int pagesize = 0;
	//if (is_paging) {
	//	pagesize = k->selectedindex-oldsel;
	//}
	//Lister_VisRangeCheck_Refresh (k, pagesize);

	//if (oldsel != k->selectedindex) {
	//	Object_Event_OnChange_Recursive (k, shallrecurse_false);
	//}

	return k;
}


