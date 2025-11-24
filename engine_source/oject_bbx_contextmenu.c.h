// oject_bbx_contextmenu.c.h

// Baker: Jan 29 2025 - Requires a double click to fire one of the event items, otherwise just highlights

// Context menu / 3  (String, StringValue, Function)

RELATED_ (Form_ContextMenu_Spawn Form_Mouse2Down O_ContextMenu_Leaving)
oject_s *O_Context_Menu_Exec(oject_s *k)
{
	int listidx = k->selectedindex * CM_SERIES_COUNT_4; // CONTEXTOID

	//char *s_show	= k->list_strings_a.strings[listidx + CM_0_TEXT_0];
	//char *s_checked	= k->list_strings_a.strings[listidx + CM_1_CHECKED_1];
	char *s_str		= k->list_strings_a.strings[listidx + CM_2_STRING_ID_2];
	char *s_fn		= k->list_strings_a.strings[listidx + CM_3_FUNCSTRING_3];

	RELATED_ (Form_ContextMenu_Spawn)

	O_ContextMenu_Leaving (k, s_str); // Inside O_Context_Menu_Exec

	zexeccmd (k->fctrl, k, listidx, s_fn, s_str);

	return k;
}




RELATED_ (Lister_ListCount)

oject_s *O_ContextMenu_Spawn (oject_s *k) { return Lister_Spawn(k); }

RELATED_ (Form_ContextMenu_Spawn Form_Mouse2Down)
oject_s *O_ContextMenu_Draw (oject_s *k)
{
	VarMyFont(myfont);
	Draw_Rect	(&k->r_interior, k->backcolor, alpha_1_0);

	rect_s	r_button	= k->tb.r_button;
	rect_s	r_text		= k->txta.r_textarea;

	Draw_Rect	(&k->tb.r_leftbar, color3_gray_75, alpha_0_5);

	int cap = (k->list_strings_a.numstrings / 4) * 4; // Avoid out of bounds if list screwed.
	for (int nn = 0; nn < cap; nn += CM_SERIES_COUNT_4, r_button.top += k->tb.buttonstep, r_text.top += k->tb.buttonstep) {
		const char *s = k->list_strings_a.strings[nn];
		const char *sc = k->list_strings_a.strings[nn + CM_1_CHECKED_1];
		int ischecked = sc[0] == '1';
		int		trueidx			= nn / CM_SERIES_COUNT_4;
		int		is_selected		= (trueidx == k->selectedindex);
		vec_t	*backcolor		= is_selected ? k->backcolorselected : k->backcolor;
		vec_t	*forecolor		= is_selected ? k->forecolorselected : k->forecolor;

		if (k->is_rounded)	Draw_RectRound	(&r_button, backcolor, alpha_1_0);
		else				Draw_Rect		(&r_button, backcolor, alpha_1_0);

		if (ischecked) {
			rect_s r = { k->tb.r_leftbar.left, r_button.top, k->tb.r_leftbar.width, r_button.height };
			Draw_Rect		(&r, color3_blue, alpha_1_0);
		}
		Draw_String	(&r_text, s, k->fontsize, forecolor, alpha_1_0);
	} // for


	return k;
}




// Baker: The bounds check here is more complex.
// Devinfo uses full canvas (vid.width)
// CSQC might use full canvas or might not ui_full_canvas.integer
// I think vid_conwidth is safe here.
oject_s *O_ContextMenu_Activate_At (oject_s *k, int x, int y)
{
	Form_Get(f,k);
	oject_s *kmnu = k;// f->servo.kcontextmenu; DIFF1
	//int x = in_windowmouse_x, y = in_windowmouse_y; DIFF2

	int max_x = vid.width;
	int max_y = vid.height;

	// If context menu opened in top half, drop it down.
	// Otherwise drop it up.
	O_ContextMenu_Refresh_Plus_Early (kmnu);

	int tophalf_click = y < vid.height / 2;
	kmnu->cm.relative_rect.left = x;
	kmnu->cm.relative_rect.top = tophalf_click ? y : y - kmnu->cm.relative_rect.height;

	/// Baker: Jan 19 2025 - Context Menu doesn't know its width yet, make it recalc


	if (RECT_RIGHTOF(kmnu->cm.relative_rect) > max_x /* vid.width*/)
		kmnu->cm.relative_rect.left = max_x /*vid.width*/ - kmnu->cm.relative_rect.width;
	else if (kmnu->cm.relative_rect.left < 0)
		kmnu->cm.relative_rect.left = 0;
	if (RECT_BOTTOMOF(kmnu->cm.relative_rect) > max_y /*vid.height*/)
		kmnu->cm.relative_rect.top = max_y/* vid.height*/ - kmnu->cm.relative_rect.height;
	else if (kmnu->cm.relative_rect.top < 0)
		kmnu->cm.relative_rect.top = 0;

	//if (kmnu->is_hidden == false) {
	oject_s *kold = f->frm.kfocused;// f->frm.kcontextoldfocus;
	//if (f->frm.kfocused->po->enum_id != class_contextmenu)
	//		f->frm.kcontextoldfocus = f->frm.kfocused;
	//	else {
	//		// Do not set old focus ever to context menu, leave it the same.
	//		int j = 5;
	//	}
	//}

	Oject_Focus_Set (kmnu);

	Oject_Hidden_Set (kmnu, false /*!kmnu->is_hidden*/); // CONTEXTOID SHOW
	f->frm.kpopup = kmnu; // O_ContextMenu_Leaving (START!)

	if (kold && kold->po->enum_id != class_contextmenu) {
		f->frm.kcontextoldfocus = kold; //f->frm.kfocused; // CONTEXTOID Store old focus (SHOW PT 2)
	}

	O_CheckBox_Refresh (kmnu); // Jan 29 2025 -- EMPTY all commented out!
	Form_QueueRefresh (f); // This is the only form refresh available right now!
	return k;
}

oject_s *Form_ContextMenu_EditModeActivate (oject_s *f)
{
	int x = in_windowmouse_x, y = in_windowmouse_y;
	return O_ContextMenu_Activate_At (f->servo.kcontextmenu, x, y);
}

RELATED_ (CSQC_Object_Method EditModeActivate)
// Uses mouse origin.  Q: What would keyboard do?  Different method maybe?
oject_s *O_ContextMenu_Popup_Method (oject_s *k)
{
	Form_Get(f,k); // IT IS CORRECT
	int x = in_windowmouse_x, y = in_windowmouse_y;
	O_ContextMenu_Activate_At (k, x, y);
	return k;
}

RELATED_ (O_TabSelect_Refresh_Plus_Early)
oject_s *O_ContextMenu_Refresh_Plus_Early (oject_s *k)
{
	VarMyFont(myfont);
	k->tb.stringwidthmaxpx	= stringlist_maxwidthint(&k->list_strings_a, myfont, k->fontsize);

	// Assume infinite size on finite canvas
	float	descender_pct			= myfont->ft_baker_descend_pct;
	float	descender_reduce_pct	= 1 - bound(0,k->fontdescendpct,1);
	int		descender_reduce_px		= ceil(descender_pct * descender_reduce_pct * k->fontsize);

	k->tb.cellpaddingx_i	= ceil(k->cellpaddingxpct * k->fontsize);
	k->tb.cellpaddingy_i	= ceil(k->cellpaddingypct * k->fontsize);
	k->tb.cellspacing_i		= ceil(k->cellspacingypct * k->fontsize);

	int left_extra = ceil(k->fontsize);

	k->tb.r_button.left		= k->cm.relative_rect.left;
	k->tb.r_button.top		= k->cm.relative_rect.top;
	k->tb.r_button.width	= (k->tb.cellpaddingx_i * 2) + left_extra + k->tb.stringwidthmaxpx;

	if (k->cm.relative_rect.width > k->tb.r_button.width) {
		k->tb.r_button.width = k->cm.relative_rect.width; // Are you sure?
	}

	k->tb.r_button.height	= (k->tb.cellpaddingy_i * 2) + ceil(k->fontsize) - descender_reduce_px;
	k->tb.buttonstep		= k->tb.r_button.height + k->tb.cellspacing_i;

	rect_s r_box = k->tb.r_button;

	// Compute bottom - due to cellspacing stepping is clearest

	int numrows = Lister_ListCount(k);
	for (int nn = 1; nn < numrows; nn ++, r_box.top += k->tb.buttonstep)
		; // Nada

	k->tb.r_leftbar = k->tb.r_button;
	k->tb.r_leftbar.width = left_extra;
	k->tb.r_leftbar.height = RECT_BOTTOMOF(r_box) - k->tb.r_button.top;

	RECT_SET (k->txta.r_textarea,
		k->cm.relative_rect.left + left_extra + k->tb.cellpaddingx_i,
		k->cm.relative_rect.top + k->tb.cellpaddingy_i,
		k->tb.stringwidthmaxpx,
		ceil(k->fontsize)
	);

autosize_go:
	k->cm.relative_rect.width = r_box.width;
	k->cm.relative_rect.height = RECT_BOTTOMOF(r_box) - k->cm.relative_rect.top;
	return k;
}

static double _lastime;
oject_s *O_ContextMenu_MouseButtonAction (oject_s *k, int x, int y, int isdown)
{
	if (isdown == false)
		return NULL;

	double clickdelta = host.realtime - _lastime;

	int is_doubleclick =  (_lastime && clickdelta < DOUBLE_CLICK_0_5);

	_lastime = host.realtime;

	int newidx = Rect_Hit_Boxes_No_Miss (&k->tb.r_button, x, y, k->tb.buttonstep, k->list_strings_a.numstrings);

	if (newidx == not_found_neg1)
		return k;

	if (newidx != k->selectedindex) {
		k->selectedindex = newidx; Object_Event_OnChange_Recursive (k, shallrecurse_false);
		//k->ui.mouse_down_time = host.realtime;
		//k->ui.mouse_down_thingi =
//		Lister_VisRangeCheck_Refresh (k, /*movecount*/ 0);
	}

	if (is_doubleclick) {
		oject_s *k_gets_focus = k->fctrl->frm.kcontextoldfocus;// kcontextoldfocus gets cleared
		// Baker: Feb 5 2025 - Edit mode plus no control with focus is "at your own risk for now".
		// In fact, edit mode is at your own risk because it zorders and with containers it can
		// be very bad.
		//if (!k_gets_focus)
		// k_gets_focus = k->fctrl;
		if (k->selectedindex != not_found_neg1)
			O_Context_Menu_Exec (k); // O_ContextMenu_Leaving double click

		Oject_Focus_Set (k_gets_focus);
		//k->fctrl->frm.kcontextoldfocus = NULL;
	}

	return k;

}

CALLERS_ (Oject_Focus_Set O_Context_Menu_Exec)

// Focus set might as well be cancel.
oject_s *O_ContextMenu_Leaving (oject_s *k, ccs *svalue)
{
	Form_Get(f,k);
	// Reply will be dialog name and "DialogCancelled" with string of NULL
	f->frm.kpopup = NULL;
	k->fctrl->frm.kcontextoldfocus = NULL;
	if (k->fctrl->ui.csqc_handle_plus1) {
		if (svalue) {
			//char *sz = Z_StrDupf ("%s: sname,
			// Reply will be dialog name event "DialogCancelled" with value of svalue
			Form_CSQC_Event_EZ (k, "PopupClosed", 0, svalue);
			//Mem_FreeNull_ (sz);
		} else {
			// Reply will be dialog name event "DialogCancelled" with value of svalue
			Form_CSQC_Event_EZ (k, "PopupClosed", 0, EVENT_VALSTR_NULL);
		}
	}
	return k;
}


// Return NULL if input not processed.
RELATED_ (EditModeActivate O_ContextMenu_Popup_Method O_ContextMenu_KeyDown) //CONTEXTOID
oject_s *O_ContextMenu_KeyDown (oject_s *k, int key, int ascii)
{
#if 1 // Baker: Jan 16 2025 - This crashes and might always have.  Doesn't seem to have vc filled
	oject_s *kreply = Lister_KeyDown(k,key,ascii);

	if (kreply) return kreply; // CONTEXTOID .. NULL on show.  Editmode must already be on.  2X event?
#endif

	// NOT HANDLED
	switch (key) {
	case K_ENTER:
		// The event CANNOT fire here.  Should wait until next frame?
		{
			oject_s *k_gets_focus = k->fctrl->frm.kcontextoldfocus;// kcontextoldfocus gets cleared
			if (k->selectedindex != not_found_neg1)
				O_Context_Menu_Exec (k); // O_ContextMenu_Leaving ENTER  CONTEXTOID FIRE <========

			Oject_Focus_Set (k_gets_focus); // CONTEXTOID ... oldfocusneeds set HIDE
		}
		break;

	case K_ESCAPE:
		Oject_Focus_Set (k->fctrl->frm.kcontextoldfocus); // O_ContextMenu_Leaving Escape CONTEXTOID HIDE <========
		return k;

	default:		return NULL; // NO ACTION
	} // sw

	return NULL;
}


