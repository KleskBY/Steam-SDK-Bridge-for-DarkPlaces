// menu_csqc_form_key.c.h

RELATED_ (Object_Draw Form_MouseButtonAction m_csqc_form_32 VM_CL_Form_Property_Get)
RELATED_ (ZForm_MouseMove still relevant! Property_Set_By_String_After_Freeing_Is_Ok)

// Baker: Oddly key is ascii (scancode) and ascii is unicode -- this convention inherited from Quake obviously.
static void M_CSQC_Form_Key (cmd_state_t *cmd, int key, int ascii, int isdown)
{
	oject_s *f = csqc_form;

	if (!f || f->is_hidden /*== true*/)
		return;

	if (!ui_full_canvas.integer /*d:1*/) {
		// This is not the norm
		in_windowmouse_x = in_windowmouse_x * vid_conwidth.integer / (float)vid.width;
		in_windowmouse_y = in_windowmouse_y * vid_conheight.integer / (float)vid.height;
	}

	if (f->is_keypreview) {
		va_super (skey_isdown, 16, "%d|%d", key, isdown);
		Form_CSQC_Event_EZ (f, "KeyPreview", 0, skey_isdown);
		if (f->didconsumekey) {
			// The form's keypreview processed the key, therefore we do nothing.
			f->didconsumekey = 0;
			return;
		}
	}

	switch (key) {

	case K_ESCAPE:
		// Determine context
		if (isdown) {
			if (f && f->frm.kpopup) {
				// Context menu is active.
				Form_KeyDown (f, key, ascii, isdown);
				break; // GET OUT
			}

			KeyDest_Set (key_game); menu_state_set_nova (m_none);  // simply leave menu
			//M_Menu_Main_f(cmd); up 1 level
			RELATED_ (M_Menu_Main_f)
		}
		break;

	case K_MOUSE1:
		RELATED_ (ZForm_MouseMove)
		Form_MouseButtonAction (f, key, ascii, isdown);
		break;

	case K_MOUSE2:
		if (isdown) {
			Form_CSQC_Mouse2Down (f);
		}
		else {
			Form_CSQC_Mouse2Up (f);
		}
		break;

	case K_TAB:
		if (isdown)
		Form_Focus_Next (f, KM_SHIFT ?  -1 :1);
		break;

	default:
		Form_KeyDown (f, key, ascii, isdown);
		break;
	} // sw

	
	//q = CL_VM_InputEvent(down ? 0 : 1, key, ascii);

}

