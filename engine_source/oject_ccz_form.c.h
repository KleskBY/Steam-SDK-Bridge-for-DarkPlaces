// oject_ccw_form_parse.c.h

RELATED_ (Lister_Spawn O_ContextMenu_Draw)

oject_s *Form_ContextMenu_Spawn (oject_s *f)
{

	// Baker: CM_MIDDLE_DOT_4 comes first, covers everything and doesn't draw.
	{
		int dd = CM_MIDDLE_DOT_4;
		oject_s *kdot = Object_Create_And_Assign_By_ClassEnum(class_sizedot, &f->servo.kdots[dd], f,
		SERVANT_KCAN_NULL, /*serv owner*/ f);

		kdot->direction = dd;
		kdot->is_hidden = true;
	}

	for (int dd = 0; dd < CM_DOTCOUNT_COUNT_9; dd ++) {
		if (dd == CM_MIDDLE_DOT_4)
			continue;

		oject_s *kdot = Object_Create_And_Assign_By_ClassEnum(class_sizedot, &f->servo.kdots[dd], f,
			SERVANT_KCAN_NULL, /*serv owner*/ f);

		kdot->direction = dd;
		kdot->is_hidden = true;
	}

//kmu_is_zsupreme_or_wont_work_in_edit_mode: CONTEXTOID SPAWN
	oject_s *kmnu = Object_Create_And_Assign_By_ClassEnum(class_contextmenu, &f->servo.kcontextmenu, f, SERVANT_KCAN_NULL, /*serv owner*/ f);

		RECT_SET (kmnu->cm.relative_rect, 200, 200, 200, 0);
		kmnu->cellspacingypct = 0;

		RELATED_ (O_ContextMenu_Draw)
		VectorCopyDestSrc (kmnu->backcolor, color3_gray_875);
		VectorCopyDestSrc (kmnu->backcolorselected, color3_red);
		//RELATED_ (O_Rectangle_Refresh)
		const things_s *po_list = Thing_Find_By_Enum_Id (prop_list);
		Property_Set_By_String_After_Freeing_Is_Ok(kmnu, po_list,
			//CM_0_TEXT_0
			//CM_1_CHECKED_1
			//CM_2_STRING_ID_2
			//CM_3_FUNCSTRING_3
			"Form State To Clipboard,0,Print,ObjectCommand,"
			" ,0,Option2String,Option2FunctionString,"
			" ,0,Option3String,Option3FunctionString,"
			"-,0,DASH,DASH,"
			"Edit Mode,0,EditModeString,EditModeToggle",
			NULL // No trailing comma
		);

		kmnu->is_hidden = true;

	return kmnu;
}

oject_s *Form_Draw (oject_s *f)
{
	GoogleRobotoFont_Check ();

refresh_check:
	if (f->frm.refreshneeded || f->frm.refreshdots) {
		Form_Finalize_Refresh (f);
		f->frm.refreshneeded = 0;
		//f->frm.refreshdots = 0;  Done in the loop
	}

//	if (f->frm.kfocused == f)
//		int j = 5;
//	if (f->frm.kfocused == NULL)
//		int j = 5;


	// PREDRAW - A hook might do a refresh
	if (f->frm.k_mousehook && host.realtime >= f->frm.hook_nextthink) {
		// ACTIVE HOOK
		mouse_hook_fn_t hook_fn = f->frm.hook_fn;
		oject_s *k_hook = f->frm.k_mousehook;

		// CLEAR THE HOOK.  Function might set it again.
		f->frm.k_mousehook = NULL;
		f->frm.hook_fn = NULL;
		f->frm.hook_nextthink = 0;

		RELATED_ (O_ScrollBar_MouseHook_Fire)
		hook_fn (k_hook); // FIRE!
	}

	// ACTUAL DRAW - FORM IS NOT in the list of drawn
	RELATED_ (drawn_list_a)
	for_each_form_control_with_container_as_no_servants (n, f, f, k)
	    if (k->is_hidden) continue; // Don't draw hidden.
		Object_Draw_Recursive_Not_Form (k);
	for_each_end

	// Any form servants have container null just like a form does.
	// All servants are zebras (Z-Order on top which means they draw last)
	for_each_form_servant_owner_is_k (n, f, /*servant owner*/ f, kservant)
//		if (kservant->po->enum_id == class_rectangle)
//			int j = 5;

		if (kservant->is_hidden) continue; // Don't draw hidden.
		Object_Draw_Recursive_Not_Form (kservant);
	for_each_end


	Draw_Clip_Clear (); // Submits draw buffer
	return f;
}


CALLERS_ (/*Object_Parse_Done*/  Form_Create_From_String /*Finalize Refresh*/)
RELATED_ (Object_Event_OnChange_Recursive)
void Form_Event_Onload (oject_s *f)
{
	if (f->eve.onload_a && f->eve.onload_a[0]) {
		exofn_t onload_fn = Event_Find_Function(f->eve.onload_a);
		if (onload_fn) {
			char *s_error_za = onload_fn (f);
			if (s_error_za) {
				Con_PrintLinef ("ERROR: %s: " QUOTED_S, f->eve.onload_a, s_error_za);
				Z_FreeNull_ (s_error_za);
			}
		} else {
			c_assert_msg_ (0, "Form_Event_Onload not found!");
		}
	}

	Object_Event_OnChange_Recursive (f, shallrecurse_true); // Form ONLOAD
}


void Form_Finalize_Refresh (oject_s *f)
{
	GoogleRobotoFont_Check ();

	// Allow the form to be in the array.
	NPA__ BakerArray_Erase (f->frm.drawn_list_a);

	// Seal the form
	Object_Finalize_Recursive (f); // Runs finalize function
	Object_Refresh_Recursive (f); // Refresh the footprint

	if (f->frm.kfocused)
		if (f->frm.kfocused->is_disabled || f->frm.kfocused->is_hidden)
			f->frm.kfocused = NULL;

	// If no focus set, set focus to first one that we can
	if (f->frm.kfocused == NULL) {
		for_each_form_all_controls (n, f, k)
			if (Oject_Will_Focus_Currently(k) == false) continue;
#if 1 // Jan 16 2025 - Route all focus through ...
			Oject_Focus_Set (k);
#else
		f->frm.kfocused = k;
#endif

//			if (k == f)
//				int j = 5;
			break;
		for_each_end
	} // find something to focus
}


void Form_Print_Zones(oject_s *f)
{
	for (int j = 0; j < f->frm.drawn_list_a->numitems; j ++) {
		oject_s *k = f->frm.drawn_list_a->pointers_array[j];

		Con_PrintLinef ("%4d " S_FMT_LEFT_PAD_20 " r = " RECTI_4PRINTF " clipped r = " RECTI_4PRINTF,
			j, k->po->name, RECT_SEND(k->cm.relative_rect), RECT_SEND(k->r_screen_clipped));
	} // for
}

void Form_Dump (oject_s *f, stringlist_t *plist, dump_detail_e detail_level, mag_s *magni_opt)
{
	int idx = 0;
	Object_Dump_Recursive (f, plist, &idx, detail_level, magni_opt);
	extern cvar_t scr_xtranslate; if (scr_xtranslate.string[0]) { Cvar_SetQuick (&scr_xtranslate, ""); }
}

CALLERS_ (Form_Single_Line_To_String_Zalloc _zcmdObjectCommand)
void Form_Dump_Single_Line (oject_s *f, stringlist_t *plist, dump_detail_e detail_level, mag_s *magni_opt)
{

	Object_Dump_Less_Recursive (f, plist, magni_opt);
	extern cvar_t scr_xtranslate; if (scr_xtranslate.string[0]) { Cvar_SetQuick (&scr_xtranslate, ""); }
}

void Form_Draw_Dump (oject_s *f, stringlist_t *plist)
{
	for (int j = 0; j < f->frm.drawn_list_a->numitems; j ++) {
		oject_s *kdrawn = f->frm.drawn_list_a->pointers_array[j];
		stringlistappendf (plist, "%4d: " S_FMT_LEFT_PAD_20 " " RECTI_4PRINTF ,
			j,
			kdrawn->cm.name_a,
			RECT_SEND (kdrawn->r_screen_clipped)
		);
	}
}

// This does ALL controls
static int sForm_Get_Focused_Idx (oject_s *f, oject_s *kfocused, int *p_num_can_focus, oject_s **pk_last)
{
	int kfocus_idx = not_found_neg1;

	for_each_form_all_controls(n,f,k) // ALL CONTROLS!
		if (Oject_Will_Focus_Currently(k) == false) continue;

		*pk_last = k;
		(*p_num_can_focus) ++;
		if (k == kfocused)
			kfocus_idx = n; // Found it.
	for_each_end

	return kfocus_idx;
}

int Form_Find_Idx (oject_s *f, oject_s *kfind)
{
	for_each_form_all_controls(n,f,k) // ALL CONTROLS!
		if (k == kfind)
			return n;
	for_each_end

	return not_found_neg1;
}

static oject_s *sFind_Next_Prev_Focus_Control(oject_s *f, oject_s *k_oldfocus, int focusidxstart, int dir)
{
	int iters = 0;
	for (int n = focusidxstart + dir; iters < f->frm.controls_a->numitems; n+= dir, iters ++) {
		if (dir > 0 && n >=  f->frm.controls_a->numitems)
			n = 0; // Loop high
		else if (dir < 0 && n < 0)
			n = f->frm.controls_a->numitems - 1; // Loop low

		oject_s *k = f->frm.controls_a->pointers_array[n];

		if (Oject_Will_Focus_Currently (k) == false || k == k_oldfocus)
			continue;

		// Found next or prev control that can focus
		return k;
	} // for

	return NULL;
}



void Form_Focus_Next (oject_s *f, int dir)
{ c_assert_ (isin2(dir, -1, 1));

	oject_s *k_oldfocus = f->frm.kfocused;

	// find our index so we can cycle through starting there.
	oject_s *k_last = NULL;
	int numcanfocus = 0, focusidxstart = sForm_Get_Focused_Idx(f, k_oldfocus, &numcanfocus, &k_last);

	if (numcanfocus <= 1) {
		// Focus change impossible if 1 or less sub object
		// However, with form editing this needs handled.
		Oject_Focus_Set (k_last);
		return;
	}

	oject_s *k_newfocus = sFind_Next_Prev_Focus_Control(f, k_oldfocus, focusidxstart, dir);

	// Lost Focus event
	Oject_Focus_Set (k_newfocus);
}




RELATED_ (Object_Destroy_Recursive)
oject_s *Form_Life (oject_s *f)
{
	if (f == CREATE_NULL) {
		// Create Form
		oject_s *form_new = NULL;
		Object_Create_And_Assign_By_ClassEnum (class_form_1, &form_new, FORM_NULL, FORM_KCAN_NULL, SERVANT_TO_NULL);
		form_new->fctrl = form_new;
#if 1
#if 1 // August 8 2025
		if (!devinfomempool) {
			devinfomempool = Mem_AllocPool("devinfo", /*flags*/ 0, /*parent*/ NULL);
		}
#endif
		NPA__ LIFE_ALLO___ form_new->frm.controls_a = (oject_array_s *)Mem_Alloc_SizeOf(devinfomempool, *form_new->frm.controls_a);
		NPA__ LIFE_ALLO___ form_new->frm.drawn_list_a = (oject_array_s *)Mem_Alloc_SizeOf(devinfomempool, *form_new->frm.drawn_list_a);
#else
		NPA__ LIFE_ALLO___ form_new->frm.controls_a = (oject_array_s *)ZAalloc_VoidP_SizeOf(*form_new->frm.controls_a);
		NPA__ LIFE_ALLO___ form_new->frm.drawn_list_a = (oject_array_s *)ZAalloc_VoidP_SizeOf(*form_new->frm.drawn_list_a);
#endif
		// Baker: January 9 2025 - This would be better served being created at the
		// very end so it can inherit the font size.
		Form_ContextMenu_Spawn (form_new);

		RECT_SET (form_new->cm.relative_rect, 0, 0, vid.width, vid.height);
		return form_new;
	}

	// DESTROY ALL CONTROLS (the form is not in its own list, we destroy it last).
	for_each_form_all_controls(n,f,k_needed) // IT IS CORRECT
		f->frm.controls_a->pointers_array[n] = Object_Destroy_Not_Recursive (f->frm.controls_a->pointers_array[n]);
	for_each_end

#ifdef _DEBUG
	extern int num_frees;
	int j = num_frees;
#endif

	LIFE_FREE___ NPA__ Mem_FreeNull_ (f->frm.controls_a->pointers_array); // Baker_Array_Erase
	LIFE_FREE___ NPA__ Mem_FreeNull_ (f->frm.controls_a);
	LIFE_FREE___ NPA__ Mem_FreeNull_ (f->frm.drawn_list_a->pointers_array); // Baker_Array_Erase
	LIFE_FREE___ NPA__ Mem_FreeNull_ (f->frm.drawn_list_a);
	f = Object_Destroy_Not_Recursive (f);

	Vid_Cursor_Set (mousepointer_arrow_default_0);
	return f; // NULL
}

oject_s *Form_Destroy (oject_s *f)
{
	return Form_Life(f); // Destroys it
}


CALLERS_ (Form_Create_From_String)
oject_s *Form_Create (void)
{
	return Form_Life(CREATE_NULL); // Creates it
}


void Form_Hook_Set (oject_s *f, oject_s *k, mouse_hook_fn_t fn, timey_dbl_t hookwhen)
{
	f->frm.k_mousehook = k;
	f->frm.hook_fn = fn;
	f->frm.hook_nextthink = hookwhen;
}

void Form_Hook_Clear(oject_s *f, oject_s *k)
{
	if (f->frm.k_mousehook && f->frm.k_mousehook != k) {
		//int j = 5;
		return; // A control can only clear its own
	}
	f->frm.k_mousehook = NULL;
	f->frm.hook_fn = NULL;
	f->frm.hook_nextthink = 0;
}


