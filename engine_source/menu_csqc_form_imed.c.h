// menu_csqc_form_imed.c.h

CALLERS_ (VM_Zircon_API_Shutdown_Reset)

void VM_Reset_CSQC_Form_Clear_Reference (void)
{
	// PRVM is shutting down the CSQC.  It will delete the form.
	if (csqc_form) {
		csqc_form = NULL;
		KeyDest_Set (key_game); menu_state_set_nova (m_none);  // simply leave menu
	}
}

CALLERS_ (VM_CL_Form_Load /*on vid_restart for reload:*/ M_CSQC_Form_Draw)
void *CSQC_Form_Create_From_String_VoidP (ccs *s, int handle, func_t my_event_callback, func_t my_close_callback, ccs *sMessage, void *fModalBoss)
{
	oject_s *f = Form_Create_From_String (s, !!ui_import_magnification.integer, PLUS1(handle), my_event_callback, my_close_callback, sMessage, fModalBoss);
	//void *ptr = (void *)Form_Create_From_String (s);
	if (f == NULL) {
		// Parse error
		return NULL;
	}
#if 0
	f->ui.csqc_handle_plus1 = PLUS1(handle);
	f->ui.form_event_qcfunc = mycallback;
#endif
	if (f) {
		f->servo.kcontextmenu->fontsize = f->fontsize;// servo.kcontextmenu
	}

	return (void *)f;
}

CALLERS_ (VM_Zircon_API_Shutdown_Reset VM_CL_Form_Close)
void *CSQC_Form_Destroy_VoidP (void *_f)
{
	oject_s *f = (oject_s *)_f;
	oject_s *fparent = f->ui.fModalBoss;
	RELATED_ (M_Menu_DevInfo_f)

	// Must switch focus now ...
	if (csqc_form == f) {
		csqc_form = fparent;
		if (fparent) {
			fparent->ui.fModalChild = NULL;
		}
		f->ui.fModalBoss = NULL;
	}
	f = Form_Life(f); // Destroys it
	if (!csqc_form) {
		// Close the menu!
		KeyDest_Set (key_game);
	}
	return (void *)f; // Parent
}

//void CSQC_Form_Prepare_Close (void *_f, void **pfparent, func_t *pClose)
//{
//	oject_s *f = (oject_s *)_f;
//	*pfparent	= f->ui.fModalBoss;
//	*pClose		= f->ui.form_close_qcfunc;
//}

RELATED_ (VM_CL_Form_Close)
void CSQC_Form_Queue_Close (void *_f, ccs *sMessage)
{
	oject_s *f = (oject_s *)_f;
	f->ui.sMessageClose = Z_StrDup (sMessage);
	f->ui.form_is_closing = true;
//	*pfparent	= f->ui.fModalBoss;
//	*pClose		= f->ui.form_close_qcfunc;
}

// Should be called Form_Show
CALLERS_ (VM_CL_Form_Load /*for now*/)
void CSQC_Form_Show (void *_f)
{
	oject_s *f = (oject_s *)_f;
	if (csqc_form) {
		// Is it modal?
		if (csqc_form->ui.fModalChild != f) {
			Con_PrintLinef ("%s: CSQC already set!", __func__);
		}
		else {
			// Modal form loaded.
		}
	}

	csqc_form = f;
#if 1 // Feb 12 2025 - XOR_CSQC_Form_Draw
	//RELATED_ (XOR_CSQC_Form_Draw)
	KeyDest_Set (key_menu);
	menu_state_reenter = 0;
	menu_state_set_nova (m_csqc_form_32);
	Con_CloseConsole_If_Client();
#endif
}

// Returns > 0 for success, < 0 for failure
int CSQC_Object_Method (void *_f, /*opt*/ ccs *s_object_name, ccs *s_method_name, /*opt*/ ccs *s_value)
{
	oject_s *f = (oject_s *)_f;

	oject_s *k = (s_object_name == NULL) ? f : Object_Find_Name (f, s_object_name);
	if (!k) {
		Con_PrintLinef ("%s object not found", s_object_name);
		return -10;
	}

	//int method_idx = Method_Find_Index (s_method_name);
	//if (method_idx == not_found_neg1) {

	if (s_value) goto method_with_value;

method_without_value:

	if (String_Match_Caseless (s_method_name, "ListClear")) {
		stringlistfreecontents (&k->list_strings_a);
		k->selectedindex = not_found_neg1;
	} else if (String_Match_Caseless (s_method_name, "FileListClear")) {
		stringlistfreecontents (&k->filelist_a);
	} else if (String_Match_Caseless (s_method_name, "ImageListClear")) {
		stringlistfreecontents (&k->imagelist_a);
//	} else if (String_Match_Caseless (s_method_name, "Popup")) {
//		O_ContextMenu_Popup_Method (k);
//		//f->frm.kfocused = k; //?
	} else if (String_Match_Caseless (s_method_name, "SetFocus")) {
		f->frm.kfocused = k;
	} else {
		goto method_fail;
	}

	Form_QueueRefresh(f);
	return 1;

method_with_value:
	RELATED_ (Property_Set_By_String_After_Freeing_Is_Ok)
	if (String_Match_Caseless (s_method_name, "AddItem")) {
		stringlistappend (&k->list_strings_a, s_value);
	} else if (String_Match_Caseless (s_method_name, "AddImage")) {
		stringlistappend (&k->imagelist_a, s_value);
	} else if (String_Match_Caseless (s_method_name, "AddFile")) {
		stringlistappend (&k->filelist_a, s_value);
	} else if (String_Match_Caseless (s_method_name, "Popup")) {
		Z_StrDupf_Realloc (&k->userstring, "%s", s_value);
		O_ContextMenu_Popup_Method (k);
		f->frm.kfocused = k; //?
	} else if (String_Match_Caseless (s_method_name, "ListAppendFileLines")) {
		// ListFromFileLines
		if (!stringlistappendfilelines_did_load(&k->list_strings_a, s_value, /*base1?*/ true, /*is column number*/ true)) {
			stringlistappend (&k->list_strings_a, "");
			stringlistappendf (&k->list_strings_a, "%s not found", s_value);
		} // Ender
	} else if (String_Match_Caseless (s_method_name, "TextWrap")) {
		// Plan: Clear the list
		// Determine width
		// Render each word GetAWord <-- return series of whitespace or a series of non-whitespace.

	} else {
		goto method_fail;
	}
	Form_QueueRefresh(f);
	return 1;


method_fail:
	Con_PrintLinef ("%s method not found", s_method_name);
	return -11;
}

RELATED_ (UI_Set_f)


qbool CSQC_Object_Property_Get_Ok (void *_f, ccs *s_object_name, ccs *s_property_name, int listindex, ccs *s_key_name, char *valuebuf, size_t valuebuf_size)
{
	oject_s *f = (oject_s *)_f;

	if (s_key_name) {
		// 
		if (Math_IsOdd ( f->list_strings_a.numstrings)) {
			VM_WarningLinef (CLVM_prog, "%s: Key set count is odd (requires even) for " QUOTED_S, "GetPropertyString", s_key_name);
			return false;
		}
		int idx = stringlist_find_index_columns (&f->list_strings_a, s_key_name, 2 /*columns*/);
		if (idx == not_found_neg1) {
			// Not found, return blank
			dpsnprintf (valuebuf, valuebuf_size, "%s", "");
		} else {
			int val_idx = idx +1;
			if (in_range_beyond(0, val_idx, f->list_strings_a.numstrings) == false) {
				VM_WarningLinef (CLVM_prog, "%s: Key set out of bounds index " QUOTED_S, "GetPropertyString", s_key_name);
				return false;
			}
			ccs *s =  f->list_strings_a.strings[val_idx];
			dpsnprintf (valuebuf, valuebuf_size, "%s", s);
		}
		return true;
	}


	oject_s *k = (s_object_name == NULL) ? f : Object_Find_Name (f, s_object_name);
	if (!k) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s object not found", "GetPropertyString", s_object_name, s_property_name);
		return false;
	}

	// Bullshit properties (non-existent in "things" list but we pretend they exist)
	if (String_Match_Caseless (s_property_name, "ListCount") && listindex == not_found_neg1) {
		dpsnprintf (valuebuf, valuebuf_size, "%d", k->list_strings_a.numstrings);
		return true;
	}

	// Bullshit properties (non-existent in "things" list but we pretend they exist)
	if (String_Match_Caseless (s_property_name, "ImageListCount") && listindex == not_found_neg1) {
		dpsnprintf (valuebuf, valuebuf_size, "%d", k->imagelist_a.numstrings);
		return true;
	}

	if (String_Match_Caseless (s_property_name, "FileListCount") && listindex == not_found_neg1) {
		dpsnprintf (valuebuf, valuebuf_size, "%d", k->filelist_a.numstrings);
		return true;
	}

	if (String_Match_Caseless (s_property_name, "ControlType") && listindex == not_found_neg1) {
		dp_strlcpy (valuebuf, k->po->name, valuebuf_size);
		return true;
	}

	if (String_Match_Caseless (s_property_name, "ControlIndex") && listindex == not_found_neg1) {
		// Find it in real time.
		int ControlIndex = Object_Get_ControlIndex (k);
		dpsnprintf (valuebuf, valuebuf_size, "%d", ControlIndex);
		return true;
	}

	if (!s_object_name && String_Match_Caseless (s_property_name, "ControlCount") && listindex == not_found_neg1) {
		dpsnprintf (valuebuf, valuebuf_size, "%d", f->frm.controls_a->numitems);
		return true;
	}

	if (!s_object_name && String_Match_Caseless (s_property_name, "Handle") && listindex == not_found_neg1) {
		dpsnprintf (valuebuf, valuebuf_size, "%d", f->ui.csqc_handle_plus1);
		return true;
	}

	if (!s_object_name && String_Match_Caseless (s_property_name, "Focused") && listindex == not_found_neg1) {
		if (f->frm.kfocused)
			dpsnprintf (valuebuf, valuebuf_size, "%s", f->frm.kfocused->cm.name_a);
		else
			dpsnprintf (valuebuf, valuebuf_size, "%s", f->cm.name_a);
		return true;
	}

	if (!s_object_name && String_Match_Caseless (s_property_name, "Controls") && listindex != not_found_neg1) {
		// Return name of the control.
		if (in_range_beyond (0, listindex, f->frm.controls_a->numitems) == false) {
			VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
				"GetPropertyString", f->cm.name_a, s_property_name, listindex, f->frm.controls_a->numitems);
			return false;
		}
		oject_s *kk = f->frm.controls_a->pointers_array[listindex];
		dp_strlcpy (valuebuf, kk->cm.name_a, valuebuf_size);
		return true;
	}

	// Stupid Alias for ListView ...
	if (k->po->enum_id == class_listview && String_Match_Caseless (s_property_name, "SelectedRow"))
		s_property_name = "SelectedIndex"; 

	const things_s *p = Thing_Find(s_property_name);
	if (!p || IsProperty(p) == false) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s property not found",
			"GetPropertyString", s_object_name, s_property_name);
		return false;
	}

	if (listindex != not_found_neg1) {
		// Baker: What is this?
		if (p->enum_id == prop_list) {
			qbool ok = in_range_beyond (0, listindex, k->list_strings_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"GetPropertyString", s_object_name, s_property_name, listindex, k->list_strings_a.numstrings);
				return false;
			}
			dp_strlcpy (valuebuf, k->list_strings_a.strings[listindex], valuebuf_size);
			return true;
		}
		if (p->enum_id == prop_filelist) {
			qbool ok = in_range_beyond (0, listindex, k->filelist_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"GetPropertyString", s_object_name, s_property_name, listindex, k->filelist_a.numstrings);
				return false;
			}
			dp_strlcpy (valuebuf, k->filelist_a.strings[listindex], valuebuf_size);
			return true;
		}
		if (p->enum_id == prop_imagelist) {
			qbool ok = in_range_beyond (0, listindex, k->imagelist_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"GetPropertyString", s_object_name, s_property_name, listindex, k->imagelist_a.numstrings);
				return false;
			}
			dp_strlcpy (valuebuf, k->imagelist_a.strings[listindex], valuebuf_size);
			return true;
		}
		return false;
	}

	vartype_e	property_vt		= (vartype_e)p->w.vt;
	size_t		offsetx			= p->moffsetof;
	varpack_u	*vp				= (varpack_u *)((byte *)k + offsetx);

	RELATED_ (Property_Dump_Maybe)

	char *s_propval_z = VarPack_String_Zalloc (IsContained(k), vp, property_vt, magni_opt_null, &f->ui.magnix, q_force_true, NULL);

	dp_strlcpy (valuebuf, s_propval_z, valuebuf_size);

	Mem_FreeNull_ (s_propval_z);
	return true;
}


qbool CSQC_Object_Property_Set_Float_Ok (void *_f, ccs *s_object_name, ccs *s_property_name, int listindex, ccs *s_key_name, float fval)
{
	oject_s *f = (oject_s *)_f;

	oject_s *k = (s_object_name == NULL) ? f : Object_Find_Name (f, s_object_name);
	if (!k) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s object not found", "SetPropertyFloat", s_object_name, s_property_name);
		return false;
	}

#if 1 // FIXME
	if (s_key_name) {
		// 
		if (Math_IsOdd ( f->list_strings_a.numstrings)) {
			VM_WarningLinef (CLVM_prog, "%s: Key set count is odd (requires even) for " QUOTED_S " = " FLOAT_LOSSLESS_FORMAT, "SetPropertyFloat", s_key_name, fval);
			return false;
		}
		int idx = stringlist_find_index_columns (&f->list_strings_a, s_key_name, 2 /*columns*/);
		if (idx == not_found_neg1) {
			// Not found, add.
			stringlistappend (&f->list_strings_a, s_key_name);
			stringlistappendf (&f->list_strings_a, FLOAT_LOSSLESS_FORMAT, fval);
		} else {
			int val_idx = idx +1;
			if (in_range_beyond(0, val_idx, f->list_strings_a.numstrings) == false) {
				VM_WarningLinef (CLVM_prog, "%s: Key set out of bounds for " QUOTED_S " = " FLOAT_LOSSLESS_FORMAT, "SetPropertyFloat", s_key_name, fval);
				return false;
			}
			char *sold =  f->list_strings_a.strings[val_idx];
			char *snew = Z_StrDupf (FLOAT_LOSSLESS_FORMAT, fval);
			Mem_FreeNull_ (sold);
			f->list_strings_a.strings[val_idx] = snew;
		}
		return true;
	}
#endif

#if 1
	// Stupid Alias for ListView ...
	if (k->po->enum_id == class_listview && String_Match_Caseless (s_property_name, "SelectedRow"))
		s_property_name = "SelectedIndex"; 

	const things_s *p = Thing_Find(s_property_name);
	if (!p || IsProperty(p) == false) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s property not found", "GetPropertyFloat", s_object_name, s_property_name);
		return false;
	}
#endif

	if (listindex != not_found_neg1) {
		// Baker: What is this?
		if (p->enum_id == prop_list) {
			qbool ok = in_range_beyond (0, listindex, k->list_strings_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"SetPropertyString", s_object_name, s_property_name, listindex, k->list_strings_a.numstrings);
				return false;
			}
			Z_StrDupf_Realloc (&k->list_strings_a.strings[listindex], FLOAT_LOSSLESS_FORMAT, fval);
			return true;
		}
		if (p->enum_id == prop_filelist) {
			qbool ok = in_range_beyond (0, listindex, k->filelist_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"SetPropertyString", s_object_name, s_property_name, listindex, k->filelist_a.numstrings);
				return false;
			}
			Z_StrDupf_Realloc (&k->filelist_a.strings[listindex], FLOAT_LOSSLESS_FORMAT, fval);
			return true;
		}
		if (p->enum_id == prop_imagelist) {
			qbool ok = in_range_beyond (0, listindex, k->imagelist_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"SetPropertyString", s_object_name, s_property_name, listindex, k->imagelist_a.numstrings);
				return false;
			}
			Z_StrDupf_Realloc (&k->imagelist_a.strings[listindex], FLOAT_LOSSLESS_FORMAT, fval);
			return true;
		}
		return false;
	}

	qbool ok = _Object_Property_Set_Float (k, p, fval); // Exclusive

	if (!ok) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s set to " FLOAT_LOSSLESS_FORMAT " failed",
			"GetPropertyFloat", s_object_name, s_property_name, fval);
		return false;
	}

	Form_QueueRefresh(f);

	return true;
}


qbool CSQC_Object_Property_Set_Ok (void *_f, ccs *s_object_name, ccs *s_property_name, int listindex, ccs *s_key_name, ccs *s_value)
{
	oject_s *f = (oject_s *)_f;

	oject_s *k = (s_object_name == NULL) ? f : Object_Find_Name (f, s_object_name);

	if (!k) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s to " QUOTED_S " object " QUOTED_S " not found", "SetPropertyString", s_object_name, s_property_name, s_value, s_object_name);
		return false;
	}

	if (s_key_name) {
		// 
		if (Math_IsOdd ( f->list_strings_a.numstrings)) {
			VM_WarningLinef (CLVM_prog, "%s: Key set count is odd (requires even) for " QUOTED_S " = " QUOTED_S, "SetPropertyString", s_key_name, s_value);
			return false;
		}
		int idx = stringlist_find_index_columns (&f->list_strings_a, s_key_name, 2 /*columns*/);
		if (idx == not_found_neg1) {
			// Not found, add.
			stringlistappend (&f->list_strings_a, s_key_name);
			stringlistappend (&f->list_strings_a, s_value);
		} else {
			int val_idx = idx +1;
			if (in_range_beyond(0, val_idx, f->list_strings_a.numstrings) == false) {
				VM_WarningLinef (CLVM_prog, "%s: Key set out of bounds for " QUOTED_S " = " QUOTED_S, "SetPropertyString", s_key_name, s_value);
				return false;
			}
			char *sold =  f->list_strings_a.strings[val_idx];
			char *snew = Z_StrDup (s_value);
			Mem_FreeNull_ (sold);
			f->list_strings_a.strings[val_idx] = snew;
		}
		return true;
	}



	// Stupid Alias for ListView ...
	if (k->po->enum_id == class_listview && String_Match_Caseless (s_property_name, "SelectedRow"))
		s_property_name = "SelectedIndex"; 

	const things_s *p = Thing_Find(s_property_name);
	if (!p || IsProperty(p) == false) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s to " QUOTED_S " property " QUOTED_S " not found", "SetPropertyString", s_object_name, s_property_name, s_value, s_property_name);
		return false;
	}

	if (listindex != not_found_neg1) {
		// Baker: What is this?
		if (p->enum_id == prop_list) {
			qbool ok = in_range_beyond (0, listindex, k->list_strings_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"SetPropertyString", s_object_name, s_property_name, listindex, k->list_strings_a.numstrings);
				return false;
			}
			Z_StrDupf_Realloc (&k->list_strings_a.strings[listindex], "%s", s_value);
			return true;
		}
		if (p->enum_id == prop_filelist) {
			qbool ok = in_range_beyond (0, listindex, k->filelist_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"SetPropertyString", s_object_name, s_property_name, listindex, k->filelist_a.numstrings);
				return false;
			}
			Z_StrDupf_Realloc (&k->filelist_a.strings[listindex], "%s", s_value);
			return true;
		}
		if (p->enum_id == prop_imagelist) {
			qbool ok = in_range_beyond (0, listindex, k->imagelist_a.numstrings);
			if (ok == false) {
				VM_WarningLinef (CLVM_prog, "%s: %s:%s index %d out of range count = %d",
					"SetPropertyString", s_object_name, s_property_name, listindex, k->imagelist_a.numstrings);
				return false;
			}
			Z_StrDupf_Realloc (&k->imagelist_a.strings[listindex], "%s", s_value);
			return true;
		}
		return false;
	}

	int iserr = Object_Property_Set (k, s_property_name, s_value);

	if (iserr != OK_NOERROR_0) {
		VM_WarningLinef (CLVM_prog, "%s: %s:%s to " QUOTED_S " property set failed", "SetPropertyString", s_object_name, s_property_name, s_value);
		return false;
	}

	Form_QueueRefresh(f);
	//Object_Refresh_Recursive (f);

	return true;
}



