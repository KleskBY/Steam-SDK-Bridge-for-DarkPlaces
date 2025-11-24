// oject_bbx_rectangle.c.h


// Draw enters with clip set to screen
// Q: Does this use the "draw list"?
RELATED_ (O_ListView_ColumnDrag Lister_Spawn Object_Draw_Recursive_Not_Form)

RELATED_ (EditModeSetDotsObject)

oject_s *O_SizeDot_Draw (oject_s *k)
{
	if (k->direction == CM_MIDDLE_DOT_4)
		return k; // Middle dot doesn't draw.  It moves.

	//Draw_Rect	(&k->r_screen, k->backcolor, alpha_1_0);
	Draw_Rect (&k->tb.r_button, k->backcolor, alpha_1_0);
	

	return k;
}

RELATED_ (Object_Refresh_Recursive O_TabSelect_Refresh_Plus_Early)
oject_s *O_SizeDot_Refresh (oject_s *k)
{
	// We have an origin, now set the position

	if (k->direction != alignment_middle_center_4) {
		xy_pairi_s *xy = &k->cm.dot_origin;
		int dsize = DDOT_SIZE_8 * 2;
		RECT_SET (k->cm.relative_rect, 
			xy->x - dsize / 2, 
			xy->y - dsize / 2, 
			dsize, 
			dsize
		);
		rect_s r_area = { RECT_SEND_ISO_CONTRACT(k->cm.relative_rect, 4) };
		k->tb.r_button = r_area;
	}
	return k;
}


mousepointer_e cursor_for_dir (int dir, int isdown)
{
	switch (dir) {
	case alignment_top_left_0:		return mousepointer_size_nwse_5;
	case alignment_top_center_1:	return mousepointer_size_northso_8;
	case alignment_top_right_2:		return mousepointer_size_nesw_6;
	case alignment_middle_left_3:	return mousepointer_size_weast_7;
	case alignment_middle_center_4:

		return isdown ? mousepointer_size_all_move_9 : mousepointer_arrow_default_0; // return mousepointer_move_6;
	case alignment_middle_right_5:	return mousepointer_size_weast_7;
	case alignment_bottom_left_6:	return mousepointer_size_nesw_6;
	case alignment_bottom_center_7:	return mousepointer_size_northso_8;
	case alignment_bottom_right_8:	return mousepointer_size_nwse_5;
	}
	return mousepointer_baker_column_20;
}

RELATED_ (Form_MouseButtonAction O_ListView_ColumnDrag EditModeSetDotsObject)
oject_s *O_SizeDot_PlacementDrag (oject_s *k, int x, int y, int isdone)
{
	Form_Get (f,k);
	// Do we get silly unrelated mouseups?  I bet we do
	int		delta_x		= x - k->ui.mouse_down_origin.x;
	int 	delta_y		= y - k->ui.mouse_down_origin.y;
	oject_s *kshadow = f->frm.kfocused;

	// Calculate new size of shadowed object based on right bottom for the moment.
	rect_s r_shadow_new = k->ui.mouse_down_shadow_rect;//  kshadow->cm.relative_rect;

	Rect_Directional_Adjust (&r_shadow_new, (alignment_e) k->direction, delta_x, delta_y);
	//Rect_Directional_Bound_Mins (&r_shadow_original, 5, 5, 16, 16);

	kshadow->cm.relative_rect = r_shadow_new; // ADJUSTMENT HERE SIZEOIC

	Form_QueueDotsRefresh (f); // f->frm.refreshdots ++

	if (isdone) {
		k->ui.mousepointer = mousepointer_arrow_default_0;
	} else {
		k->ui.mousepointer = cursor_for_dir(k->direction, isdown_true);
	}
	//Vid_SetWindowTitlef ("Mousepointer %d", k->ui.mousepointer);
	return k;
}

// isdown:  f->frm.kmousedown is SET by Form_MouseButtonAction AFTER we exit
// !isdown: f->frm.kmousedown should be us.
RELATED_ (O_ScrollBar_MouseButtonAction O_ListView_ColumnDrag Oject_Focus_Set )

oject_s *O_SizeDot_MouseMove (oject_s *k, int x, int y)
{
	if (k->fctrl->frm.kmousedown == k)
		return O_SizeDot_PlacementDrag (k, x, y, isdone_false);

	k->ui.mousepointer = cursor_for_dir(k->direction, isdown_false);// mousepointer_size_both_10;
	return k;
}


oject_s *O_SizeDot_MouseButtonAction (oject_s *k, int x, int y, int isdown)
{
	if (isdown == false) {
		k->ui.mousepointer = mousepointer_arrow_default_0;
		return O_SizeDot_PlacementDrag (k,x,y, isdone_true);
	}

	Form_Get (f, k);

	// We need to zorder is bastard now
	oject_s *kshadow = f->frm.kfocused;
	Object_Method_ZOrder_Front (kshadow);

	k->ui.mouse_down_time = host.realtime;
	XY_SET (k->ui.mouse_down_origin, x, y);

	k->ui.mouse_down_rect = k->cm.relative_rect;
	k->ui.mouse_down_shadow_rect = kshadow->cm.relative_rect;

	// NO HOOK THAT IS A PERPETUAL THUMP WITH NO MOUSEMOVEMENT
	Form_QueueRefresh (f);
	return k;
}

// January 16 2025: Nudge
RELATED_ (O_TextBox_KeyDown)

// K is the control focused here ...
oject_s *O_SizeDot_KeyDown (oject_s *k, int key, int ascii)
{
	Form_Get(f,k);
	oject_s *k_mouse = f->frm.kmousedown;
	if (k_mouse)
		return k; // Refused.  Pick one or the other, don't do both.

	qbool didact = false;

	switch (key) {
	case K_DELETE:
		// We could set is hidden here and rename to deleted
		//Object_Property_Set_Fmt (k, "IsHidden", "False");
		Object_Property_Set(k, "IsHidden", "True");
		Form_Focus_Next (f, -1);
		break;

	case '-':
		Object_Method_ZOrder_Front (k);
		break;

	case '=':
		Object_Method_ZOrder_Back (k);
		break;

	case 'c':
		if (KM_CTRL) {
			oject_s *kcan = k->fctrl == f ? f : k->container;
			oject_s *dpk = NULL;

			oject_s *knew = 
				Object_Create_And_Assign_By_ClassEnum (class_rectangle, &dpk, f,kcan, /*servant to*/ NULL);
			Object_Property_Set(knew, "BackColor", "#f00");
			va_super (s_rect, 32, "%d,%d,%d,%d", RECT_SEND(k->cm.relative_rect));
			Object_Property_Set(knew, "Rect", s_rect);
			Oject_Focus_Set (knew); // Is this right?
		}

		//didact = true; k->cm.relative_rect.left ++;
		break;
	}

	if (KM_CTRL) goto ctrl_only;
	if (KM_SHIFT) goto shift_only;

	switch (key) {
	case K_LEFTARROW:
		didact = true; k->cm.relative_rect.left --;
		break;

	case K_RIGHTARROW:
		didact = true; k->cm.relative_rect.left ++;
		break;

	case K_UPARROW:							
		didact = true; k->cm.relative_rect.top --;
		break;

	case K_DOWNARROW:
		didact = true; k->cm.relative_rect.top ++;
		break;
	}
	goto processed_key;

shift_only:

	// SHIFTED
	switch (key) {
	case K_LEFTARROW: // SHIFTED - Extend left
		didact = true; k->cm.relative_rect.left --; k->cm.relative_rect.width ++;
		break;

	case K_RIGHTARROW: // SHIFTED - Extend right
		didact = true; k->cm.relative_rect.width ++;
		break;

	case K_UPARROW: // SHIFTED - Extend up			
		didact = true; k->cm.relative_rect.top --; k->cm.relative_rect.height ++;
		break;

	case K_DOWNARROW: // SHIFTED
		didact = true; k->cm.relative_rect.height ++;
		break;
	}
	goto processed_key;

ctrl_only:

	// CONTROL
	switch (key) {
	case K_LEFTARROW: // SHIFTED - Extend left
		didact = true; k->cm.relative_rect.left ++; k->cm.relative_rect.width --;
		break;

	case K_RIGHTARROW: // SHIFTED - Extend right
		didact = true; k->cm.relative_rect.width --;
		break;

	case K_UPARROW: // SHIFTED - Extend up			
		didact = true; k->cm.relative_rect.top ++; k->cm.relative_rect.height --;
		break;

	case K_DOWNARROW: // SHIFTED
		didact = true; k->cm.relative_rect.height --;
		break;
	}
	goto processed_key;

processed_key:

	if (didact) {
		Form_QueueDotsRefresh (f); // f->frm.refreshdots ++
		Form_QueueRefresh (f);
	}

	return k;
}










