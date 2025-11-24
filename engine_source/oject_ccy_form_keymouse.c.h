// oject_ccy_form_keymouse.c.h

CALLERS_ (ZForm_MouseMove)


qbool Form_CSQC_Event_With_Mouse_EZ (oject_s *k, ccs *s_event_name, float fval, ccs *s_value_string, float x, float y)
{
	prvm_prog_t *prog = CLVM_prog;

	if (!k->fctrl->ui.csqc_handle_plus1)
		return false;
	vec3_t mousepos = {x,y,0};
	VectorCopyDestSrc (PRVM_clientglobalvector(v_forward), mousepos);  // src -> dest
	return Form_CSQC_Event_EZ (k, s_event_name, fval, s_value_string);
}


qbool Form_CSQC_Event_EZ (oject_s *k, ccs *s_event_name, float fval, ccs *s_value_string)
{
	if (!k->fctrl->ui.csqc_handle_plus1)
		return false;

	Form_Get(f,k);
	return CL_VM_FormEvent (
		UNPLUS1(f->ui.csqc_handle_plus1),
		f->ui.form_event_qcfunc,
		k->cm.name_a,
		s_event_name,  // OnClick or whatever
		fval,
		s_value_string
	);
}


oject_s *Form_MouseUp (oject_s *f, int key, int ascii)
{
	int x = in_windowmouse_x, y = in_windowmouse_y;
	oject_s *k_mouse = f->frm.kmousedown;

	if (k_mouse == NULL) {
		
		// Double click is an extra event
		if (f->frm.kmousedown_isdouble) {
			// Jan 26 2025 - Can this happen?
			Form_CSQC_Event_With_Mouse_EZ (f, "DoubleClick", 0, NULL, x, y); // MOUSE1 (FORM)
			f->frm.kmousedown_isdouble = false;
		}
		else {
			Form_CSQC_Event_With_Mouse_EZ (f, "Click", 0, NULL, x, y); // MOUSE1 (FORM)
		}

		return NULL;
	}

	switch (k_mouse->po->enum_id) {
	case class_scrollbar:	O_ScrollBar_MouseButtonAction(k_mouse,x,y,MOUSEUP_0); break;
	case class_listview:	O_ListView_MouseButtonAction(k_mouse,x,y,MOUSEUP_0); break;
	//case class_tabselect:		O_TabSelect_MouseButtonAction (k_mouse, x, y,MOUSEUP_0);
	default:				break;
	} // sw

	if (f->frm.kmousedown) {
		// Assume it was processed, but still do a polite release
		oject_s *k = f->frm.kmousedown;
			k->mousedown_thing = 0;
			k->mousedown_when = 0;
			k->mousedown_at.x = k->mousedown_at.y = 0 ;
			k->mousedown_thumb_into.x = k->mousedown_thumb_into.y = 0;	// How many px into thumb are we?
			k->mousedown_thumb_at_mousedown_px.x = k->mousedown_thumb_at_mousedown_px.y = 0;	// How many px into thumb are we?
			k->mousedown_forced_thumb_pos.x = k->mousedown_forced_thumb_pos.y = 0;	// How many px into thumb are we?


			k->ui.mouse_down_time = 0;
			k->ui.mouse_down_origin.x = 0;
			k->ui.mouse_down_origin.y = 0;
			k->ui.mouse_down_thingi = 0;
			k->ui.mouse_down_valuei = 0;
			k->ui.is_move_thresh_met = 0;
			k->ui.mousepointer = mousepointer_arrow_default_0;
			//Vid_Cursor_Set (k->ui.mousepointer);
			RECT_SET (k->ui.mouse_down_rect, 0,0,0,0);

			f->frm.kmousedown = NULL;
			
			if (f->frm.kmousedown_isdouble) {
				Form_CSQC_Event_With_Mouse_EZ (k, "DoubleClick", 0, NULL, x, y); // MOUSE1 (FORM)
				f->frm.kmousedown_isdouble = false;
			} else {
				Form_CSQC_Event_With_Mouse_EZ (k, "Click", 0, NULL, x, y); // MOUSE1 (CONTROL)
			}
	}

	//Con_PrintLinef ("Mouse release");
	return NULL;
}


oject_s *Form_Mouse_Move (oject_s *f, int x, int y)
{
	oject_s *k_mouse = f->frm.kmousedown;

	if (k_mouse == NULL) {
		// mouse button not pressed!
		oject_s *k = Oject_Hit_Drawn(f, x, y, isbuttonaction_false);

		if (k) { // MOUSEMOVE WITHOUT MOUSEDOWN
			switch (k->po->enum_id) {
			//case class_scrollbar:	O_ScrollBar_MouseMove(k_mouse,x,y); break;
			case class_listview:	k = O_ListView_MouseMove (k,x,y); break;
			case class_sizedot:		k = O_SizeDot_MouseMove (k,x,y); break;
			case class_textbox:		k = O_TextBox_MouseMove (k,x,y); break;
			default:				k = NULL; break; // Not.
			} // sw
		}

		mousepointer_e mpdo =
			k ?
			k->ui.mousepointer : mousepointer_arrow_default_0;
		Vid_Cursor_Set (mpdo);

		return NULL;
	}

	//mousepointer_e ret = mousepointer_invalid_0;

	// MOUSE DOWN DURING MOVE
mousedown_mousemove:
	switch (k_mouse->po->enum_id) {
	case class_listview:	O_ListView_MouseMove	(k_mouse, x, y); break;
	case class_scrollbar:	O_ScrollBar_MouseMove	(k_mouse, x, y); break;
	case class_sizedot:		O_SizeDot_MouseMove		(k_mouse, x, y); break;
	case class_textbox:		O_TextBox_MouseMove		(k_mouse, x, y); break;
	} // sw

	//if (mpwanted == 0) {
	//	Vid_Cursor_Reset ();
	//} else {

	//}
	mousepointer_e mpdo =
		k_mouse ?
		k_mouse->ui.mousepointer : mousepointer_arrow_default_0;
	Vid_Cursor_Set (mpdo);


	return NULL;
}


oject_s *Form_Mouse2Down (oject_s *f)
{
	// For now, disallow mouse2 action if mouse1 is doing something already.
	if (f->frm.kmousedown)
		return NULL;

	// Jan 29 2025 List Type controls need to treat this as a click.

	return Form_ContextMenu_EditModeActivate (f); // DEVINFO
}

oject_s *Form_CSQC_Mouse2Up (oject_s *f)
{
	// For now, disallow mouse2 action if mouse1 is doing something already.
	if (f->frm.kmousedown)
		return NULL;

	int x = in_windowmouse_x, y = in_windowmouse_y;
	oject_s *khit = Oject_Hit_Drawn(f, x, y, isbuttonaction_true);

	Form_CSQC_Event_With_Mouse_EZ (khit ? khit : f, "Click", /*right*/ 1, NULL, x, y); // FOR REAL

	return f;
}


oject_s *Form_CSQC_Mouse2Down (oject_s *f)
{
	// For now, disallow mouse2 action if mouse1 is doing something already.
	if (f->frm.kmousedown)
		return NULL;

#if 1
	// January 29 - CSQC Forms + Listview
	if (f->ui.csqc_handle_plus1 && KM_SHIFT == false ) {
		// CSQC forms only ... for now ....
		// Treat it mostly like a left click.
		Form_MouseButtonAction (f, /*key*/ 0, /*ascii*/ 0, /*down*/ MOUSEDOWN_1);
		
		// But RightClick doesn't get mouse focus right now.  Maybe it will later?
		f->frm.kmousedown = NULL; 
		return f;
	}
#endif


	if (KM_SHIFT) {
		return Form_ContextMenu_EditModeActivate (f); // CSQC EDITMODE CONTEXTOID SHOW
	}

	// ACCEPT MOUSE 2
	//int x = in_windowmouse_x, y = in_windowmouse_y;

	//oject_s *k = Oject_Hit_Drawn(f, x, y, isbuttonaction_true);

	////if (k)
	////	Form_CSQC_Event_EZ (f, "Click", /*right*/ 1, EVENT_VALSTR_NULL);

	////f->frm.kmousedown = k; // NO.

	return f;
}

oject_s *Form_Mouse2Up (oject_s *f) // DEVINFO
{
	// For now, disallow mouse2 action if mouse1 is doing something already.
	if (f->frm.kmousedown)
		return NULL;


#if 0
    int x = in_windowmouse_x, y = in_windowmouse_y;
	oject_s *k = Oject_Hit_Drawn(f, x, y, isbuttonaction_true);
#endif
	//Form_CSQC_Event_EZ (k ? k : f, "Click", /*right*/ 1, NULL); // No .. Devinfo

	return f;
}

// Caller must ensure f is not null
// LEFT button comes here.
oject_s *Form_MouseButtonAction(oject_s *f, int key, int ascii, int isdown)
{
	int x = in_windowmouse_x, y = in_windowmouse_y;

	if (isdown == false)
		return Form_MouseUp (f, key, ascii);

	oject_s *k = Oject_Hit_Drawn(f, x, y, isbuttonaction_true);

	if (!k)
		return NULL;

	int is_new_focus;

	if (f->frm.editmode)
		// EDIT MODE: Objects that don't normally focus can get focused
		// except NOT servants (scrollbars, sizing hint, etc.)
		// Context menus -- a servant -- don't receive their focus in this manner
		// and disappear on lost focus so that exception doesn't matter here.
		is_new_focus = f->frm.kfocused != k && k->servant_owner == NULL && k != f;
	else
		is_new_focus = f->frm.kfocused != k && Oject_Will_Focus_Currently(k);

	// Lost Focus event
	if (is_new_focus) {
		Oject_Focus_Set (k);
	}

	// Disabled controls will not receive any events and cannot be "downed"
	// but they do absorb mouse clicks.
	if (k->is_disabled)
		goto disabled_skip;

	switch (k->po->enum_id) { // ISDOWN ONLY -- ISUP handled above by transfer to Form_MouseUp
	case class_button:		O_Button_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_contextmenu:	O_ContextMenu_MouseButtonAction(k, x, y, MOUSEDOWN_1); break;
	case class_gridview:	O_GridView_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_listbox:		O_ListBox_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_listview:	O_ListView_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_scrollbar:	O_ScrollBar_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_sizedot:		O_SizeDot_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_tabselect:	O_TabSelect_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	case class_textbox:		O_TextBox_MouseButtonAction (k, x, y, MOUSEDOWN_1); break;
	} // sw

	f->frm.kmousedown = k; // EXCLUSIVE SET

	// Check if under 0.5 since -- if so double it.
	if (f->frm.kmousedown_time && ((host.realtime - f->frm.kmousedown_time) <= DOUBLE_CLICK_0_5) )
		f->frm.kmousedown_isdouble = true;
	else 
		f->frm.kmousedown_isdouble = false;
	f->frm.kmousedown_time = host.realtime;

disabled_skip:
	return k; // If we hit the form that doesn't count.
}

RELATED_ (Form_MouseButtonAction)
oject_s *Form_KeyDown (oject_s *f, int key, int ascii, int is_down)
{
	// Disabled controls should not have keyboard focus.
	oject_s *k = f->frm.kfocused;
	if (!k) return NULL;

	// Baker: January 16 2025 - K_TAB does NOT come here goes to Form_Focus_Next
	RELATED_ (Form_Focus_Next)

	if (!is_down) {
		switch (k->po->enum_id) { // UP
		case class_textbox:		return O_TextBox_KeyUp(k, key, ascii);
		} // sw
		return NULL;
	}

	if (f->frm.editmode && k->po->enum_id != class_contextmenu)
		return O_SizeDot_KeyDown(k, key, ascii);

	switch (k->po->enum_id) { // IS DOWN
	case class_button:		return O_Button_KeyDown(k, key, ascii);
	case class_contextmenu:	return O_ContextMenu_KeyDown(k, key, ascii);
	case class_gridview:	return O_GridView_KeyDown(k, key, ascii);
	case class_listbox:		return O_ListBox_KeyDown(k, key, ascii);
	case class_listview:	return O_ListView_KeyDown(k, key, ascii);
	case class_tabselect:	return O_TabSelect_KeyDown(k, key, ascii);
	case class_textbox:		return O_TextBox_KeyDown(k, key, ascii);
	} // sw

	return NULL;
}

