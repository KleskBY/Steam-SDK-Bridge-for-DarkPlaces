// oject_bbx_button.c.h

#define PRESS_AMOUNT_2 2

// Jan 28 2025 - r_button, r_text and friends need to be in screen coordinates.
// Using relative rect to put them in the right place.

oject_s *O_Button_Draw (oject_s *k)
{
	VarMyFont(myfont);

	rect_s	r_button	= k->tb.r_button;
	rect_s	r_text		= k->txta.r_textarea;

	const char *s = k->caption_a;

	qbool is_down = k->fctrl->frm.kmousedown == k;

	if (k->ui.button_down_end_time > host.realtime)
		is_down = true;
	else if (k->ui.button_down_end_time)
		k->ui.button_down_end_time = 0; // End

	if (is_down) {
		r_text.left		+= PRESS_AMOUNT_2;
		r_text.top		+= PRESS_AMOUNT_2;
	}

	qbool is_focused = IsFocused(k);

	vec_t	*backcolor		= is_focused ? k->backcolorselected : k->backcolor;
	vec_t	*forecolor		= is_focused ? k->forecolorselected : k->forecolor;

	if (k->is_rounded)	Draw_RectRound	(&r_button, backcolor, k->backalpha);
	else				Draw_Rect		(&r_button, backcolor, k->backalpha);

	if (k->image_name_a) {
		rect_s	r_image		= k->tb.r_leftbar;
		if (is_down)
			r_image.left += PRESS_AMOUNT_2, r_image.top += PRESS_AMOUNT_2;

		cachepic_t *p = Draw_CachePic_Flags (k->image_name_a, CACHEPICFLAG_NOTPERSISTENT);
		Draw_ImageCachePic (&r_image, p, k->imagecolor, k->imagealpha);
	}

	Draw_String	(&r_text, s, k->fontsize, forecolor, alpha_1_0);

	return k;
}


oject_s *O_Button_Finalize (oject_s *k)
{
	if (k->caption_a == NULL)				// If we are lacking text, spawn it.
		k->caption_a = Z_StrDup ("");
	return k;

}

RELATED_ (Object_Refresh_Recursive)



oject_s *O_Button_Refresh (oject_s *k)
{
	VarMyFont(myfont);
	k->tb.stringwidthmaxpx	= Draw_StringWidthInt (myfont, k->caption_a, k->fontsize); // Ceil

	k->tb.cellpaddingx_i	= ceil(k->cellpaddingxpct * k->fontsize);
	k->tb.cellpaddingy_i	= ceil(k->cellpaddingypct * k->fontsize);

	// If image take a square and subtract

	k->tb.r_button.left		= k->cm.relative_rect.left;
	k->tb.r_button.top		= k->cm.relative_rect.top;
	k->tb.r_button.width	= k->cm.relative_rect.width;// - 2;// - (k->tb.cellpaddingx_i * 2);
	k->tb.r_button.height	= k->cm.relative_rect.height;// - 2;// - (k->tb.cellpaddingy_i * 2);
	// + ceil(k->fontsize) - descender_reduce_px;
	
	



	// The text area gets centered in the r_screen area - shouldn't it be the button area?
	rect_s r_text_canvas = k->tb.r_button;

	if (k->image_name_a) {
		r_text_canvas.left += k->tb.cellpaddingx_i + ceil(k->fontsize) + k->tb.cellpaddingx_i;
		r_text_canvas.width -= k->tb.cellpaddingx_i + ceil(k->fontsize) + k->tb.cellpaddingx_i;

		RECT_SET (k->txta.r_textarea,
			0/*->cm.relative_rect.left + k->tb.cellpaddingx_i*/,
			0/*k->cm.relative_rect.top + k->tb.cellpaddingy_i*/,
			k->tb.stringwidthmaxpx, //k->tb.r_button.width - (k->tb.cellpaddingx_i * 2),
			ceil(k->fontsize)// + descender_reduce_px,
		);

		Label_Rect_Edit_Align (&k->txta.r_textarea, &r_text_canvas, (alignment_e) alignment_middle_left_3 /*k->alignment*/);

		RECT_SET (k->tb.r_leftbar,
			k->tb.r_button.left + k->tb.cellpaddingx_i /*->cm.relative_rect.left + k->tb.cellpaddingx_i*/,
			k->txta.r_textarea.top, //  0/*k->cm.relative_rect.top + k->tb.cellpaddingy_i*/,
			ceil(k->fontsize), //k->tb.r_button.width - (k->tb.cellpaddingx_i * 2),
			ceil(k->fontsize)// + descender_reduce_px,
		);
	} else {
		RECT_SET (k->txta.r_textarea,
			0/*->cm.relative_rect.left + k->tb.cellpaddingx_i*/,
			0/*k->cm.relative_rect.top + k->tb.cellpaddingy_i*/,
			k->tb.stringwidthmaxpx, //k->tb.r_button.width - (k->tb.cellpaddingx_i * 2),
			ceil(k->fontsize)// + descender_reduce_px,
		);

		Label_Rect_Edit_Align (&k->txta.r_textarea, &r_text_canvas, (alignment_e) alignment_middle_center_4 /*k->alignment*/);
	}

	float	descender_pct			= myfont->ft_baker_descend_pct;
	float	descender_reduce_pct	= 1 - bound(0,k->fontdescendpct,1);
	int		descender_reduce_px		= ceil(descender_pct * descender_reduce_pct * k->fontsize);

	k->txta.r_textarea.top += descender_reduce_px; // Move down a little.

	AdjustLeftTopToScreenK (k->tb.r_button);
	AdjustLeftTopToScreenK (k->txta.r_textarea);
	AdjustLeftTopToScreenK (k->tb.r_leftbar);

	return k;
}




oject_s *O_Button_MouseButtonAction (oject_s *k, int x, int y, int isdown)
{
	//K_GET_RELATIVE_XY (x,y);

	if (!isdown)
		return NULL;

	if (!k->ui.button_down_end_time)
		return NULL; // If pressed appearance, do not allow another.

	// How does click event fire?
	// ONLY for keypress ...k->ui.button_down_end_time = host.realtime + BUTTON_PRESS_TIME_0_3;

	return k;
}

oject_s *O_Button_KeyDown (oject_s *k, int key, int ascii)
{
	//Form_Get(f,k);
	switch (key) {
	case K_ENTER:
					if (!k->ui.button_down_end_time) {
						k->ui.button_down_end_time = host.realtime + BUTTON_PRESS_TIME_0_3;
						// Enter fires click event with no mouse coords
						Form_CSQC_Event_With_Mouse_EZ (k, "Click", 0, NULL, -1, -1); // MOUSE1 (FORM)
					}
					break;

	default:		return NULL; // NO ACTION
	} // sw
	return k;
}

