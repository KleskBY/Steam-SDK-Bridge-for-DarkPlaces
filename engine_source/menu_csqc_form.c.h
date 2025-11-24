// menu_csqc_form.c.h




RELATED_ (Form_QueueDotsRefresh VM_CL_Form_Load)

// What is our scaling plan?
// We somewhat want to draw on a magnified canvas.
// The data needs saved as if working from a magnified canvas.

// Our calculation is mostly reversible (column width sizing is not)
//


RELATED_ (M_ZDev_Draw M_ZForm_Draw VM_Zircon_API_Shutdown_Reset UI_Set_f)

#include "oject.c.h"

extern int m_csqc_form_video_restarted; // If set, cleared on draw, set by m_video_restarted
int m_video_restarted = 0; // If set, cleared on M_draw
extern oject_s *csqc_form;
#include "menu_csqc_form_imed.c.h"


RELATED_ (M_Menu_DevInfo_f VM_CL_Form_Load)



RELATED_ (PRVM_ED_Global_Fly)
//void Property_Fill_Maybe (oject_s *k, things_s *p, stringlist_t *plist, qbool is_indent);

qbool UI_Dump (ccs *s_controlname, ccs *s_property_name)
{
	if (!csqc_form)
		return false; // No form.

	oject_s *k = Object_Find_Name (csqc_form, s_controlname);
	if (!k)
		return false; // No object matches
	Form_Get(f,k); // IT IS CORRECT

	const things_s *p = Thing_Find(s_property_name);
	if (!p || IsProperty(p) == false)
		return false; // No property

	if (p->w.vt != vtlist_str_20)
		return false; // Not a list
#ifdef _DEBUG
	vartype_e	property_vt		= (vartype_e)p->w.vt;
#endif
	size_t		offsetx			= p->moffsetof;
	varpack_u	*vp				= (varpack_u *)((byte *)k + offsetx);

	stringlist_condump_with_linenums (&vp->stringlist);
	return true;
}


qbool UI_Fly (ccs *s_controlname, ccs *s_property_name, char *valuebuf, size_t valuebuf_size)
{
	if (!csqc_form)
		return false; // No form.

	oject_s *k = Object_Find_Name (csqc_form, s_controlname);
	if (!k)
		return false; // No object matches
	Form_Get(f,k);

	const things_s *p = Thing_Find(s_property_name);
	if (!p || IsProperty(p) == false)
		return false; // No property

	vartype_e	property_vt		= (vartype_e)p->w.vt;
	size_t		offsetx			= p->moffsetof;
	varpack_u	*vp				= (varpack_u *)((byte *)k + offsetx);

	RELATED_ (Property_Dump_Maybe)

	char *s_propval_z = VarPack_Maybe_Quote_String_Zalloc (IsContained(k), vp, property_vt, magni_opt_null, &f->ui.magnix, q_force_true, NULL);
	VarPack_String_Add_Detail (&s_propval_z, k, vp, p);

	dp_strlcpy (valuebuf, s_propval_z, valuebuf_size);

	Mem_FreeNull_ (s_propval_z);
	return true;
}


RELATED_ (GetGlobalListServer_Count)

int Get_UI_ObjectName_AutoComp_Count (ccs *s_prefix)
{
	oject_s *f = csqc_form;
	stringlist_t list = {0};
	int num_matches = 0;



	if (csqc_form) {
		//Con_PrintLinef ("program %s not active/found", s_progname);
		//return 0; // No matches because bad program name

		if (s_prefix[0] == NULL_CHAR_0 && csqc_form->frm.editmode && csqc_form->frm.kfocused) {
			// ===> Autocomplete from nothing picks current control
			stringlistappend (&list, csqc_form->frm.kfocused->cm.name_a);
			goto getfocused_bypass;
		}

		stringlistappend (&list, f->cm.name_a);
		for_each_form_all_controls(n,f,k) // ALL CONTROLS!
			if (k->servant_owner)
				continue; // Helper scrollbar or something

			ccs *sxy = k->cm.name_a;
			stringlistappend (&list, sxy);
		for_each_end
	}
	// Add generic "list" autocompleter.
	stringlistappend (&list, "controls");
	stringlistappend (&list, "events");
	stringlistappend (&list, "focused");
	stringlistappend (&list, "help");
	stringlistappend (&list, "list"); // List all controls for current form
	stringlistappend (&list, "methods");
	stringlistappend (&list, "properties");
	stringlistappend (&list, "uncontained");

getfocused_bypass:
	stringlistsort_unique (&list);

	for (int idx = 0; idx < list.numstrings; idx++) {
		const char *sxy =  list.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		num_matches ++;
		SPARTIAL_EVAL_
	} // idx

	stringlistfreecontents (&list);
	return num_matches;
}

RELATED_ (VM_CL_Form_Property_Set)
int Get_UI_Property_AutoComp_Count (ccs *s_prefix)
{
	if (!csqc_form) {
		// Con_PrintLinef ("program %s not active/found", s_progname);
		return 0; // No matches because bad program name
	}



	//oject_s *f = csqc_form;
	stringlist_t list = {0};
	int num_matches = 0;

	stringlistappend (&list, "SetFocus"); // Methods

	for (const things_s *p = &things[0]; p->name; p ++) {
		if (!IsProperty(p))
			continue;

		ccs *sxy = p->name;
		stringlistappend (&list, sxy);
	}

	stringlistsort (&list, fs_make_unique_true);

	for (int idx = 0; idx < list.numstrings; idx++) {
		const char *sxy =  list.strings[idx];
		if (String_Starts_With_Caseless (sxy, s_prefix) == false)
			continue;

		num_matches ++;
		SPARTIAL_EVAL_
	} // idx

	stringlistfreecontents (&list);
	return num_matches;
}

//void UI_Info_f (cmd_state_t *cmd)
//{
//	// List oject types
//	Con_PrintLinef ("Control Types");
//	int idx;
//	idx = 0;
//	for (things_s *p = &things[0]; p->name; p ++) {
//		if (!IsObjectType(p))
//			continue;
//		Con_PrintLinef ("%3d: %s", idx, p->name);
//		idx ++;
//	}
//
//	Con_PrintLineEmpty ();
//	Con_PrintLinef ("Properties");
//
//	idx = 0;
//	for (things_s *p = &things[0]; p->name; p ++) {
//		if (!IsProperty(p))
//			continue;
//
//		ccs *scolor = Math_IsOdd(idx) ? "" : CON_BRONZE;
//		Con_PrintLinef ("%s%3d: %-30.30s %s", scolor, idx, p->name, VarType_For_Enum(p->w.vt) );
//		RELATED_ (things)
//		if (p->describe)
//			Con_PrintLinef ("   %s%s", scolor, p->describe);
//		idx ++;
//	}
//}




RELATED_ (PRVM_ED_Eset_f)
RELATED_ (UI_Set_f)

// ex: ui_set LblSomething Color (VALUE)
void UI_Set_f (cmd_state_t *cmd)
{
	Con_PrintLinef ("Type " CON_BRONZE "oset help" CON_WHITE " for use options");

	ccs *s_object_name = cmd_argv_parm1;
	if (cmd_argc == 2 && String_Match (s_object_name, "help")) {		// ui_set list (all controls)

		#define S_FMT_LEFT_PAD_30			"%-30.30s"		// Negative means left pad or right pad?  LEFT  The .20 truncates at 20, right?

		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset controls", "list control types like TextBox and their defaults");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset events", "list events like OnLoad");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset properties", "list properties like IsHidden and BackColor");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset methods", "list methods like SetFocus, AddItem");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset uncontained", "list methods like SetFocus, AddItem");
		Con_PrintLineEmpty ();
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset", "display current form as text");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset list", "list objects in current form");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset [text1]", "list properties for [text1]");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset [text1] [caption]", "print value of caption for [text1]");
		Con_PrintLinef (S_FMT_LEFT_PAD_30 " %s", "oset [text1] [caption] [value]", "set value of caption for [text1]");

		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "controls")) {		// ui_set list
		int idx = 0; for (const things_s *p = &things[0]; p->name; p ++) {
			if (!IsObjectType(p)) continue;
			Con_PrintLinef ("%3d: " S_FMT_LEFT_PAD_20 " ^xfc3" "%s", idx, p->name, p->sdefault ? p->sdefault : "");
			idx ++; // Only for properties
		}
		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "events")) {		// ui_set properties
		int idx = 0; for (const things_s *p = &things[0]; p->name; p ++) {
			if (!IsEvent(p)) continue;
			Con_PrintLinef ("%3d: " S_FMT_LEFT_PAD_20 " ^xfc3" "%s", idx, p->name, p->describe ? p->describe : "");
			idx ++; // Only for properties
		}
		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "methods")) {		// ui_set methods
		int idx = 0; for (things_s *p = &things[0]; p->name; p ++) {
			if (!IsMethod(p)) continue;
			Con_PrintLinef ("%3d: " S_FMT_LEFT_PAD_20 " ^xfc3" "%s", idx, p->name, p->describe ? p->describe : "");
			idx ++; // Only for properties
		}
		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "properties")) {		// ui_set properties
		int idx = 0; for (things_s *p = &things[0]; p->name; p ++) {
			if (!IsProperty(p)) continue;
				#define S_FMT_LEFT_PAD_10			"%-10.10s"		// Negative means left pad or right pad?  LEFT  The .20 truncates at 20, right?
			Con_PrintLinef ("%3d: " S_FMT_LEFT_PAD_20 " " CON_BRONZE S_FMT_LEFT_PAD_10 " ^xfc3" "%s",
				idx, p->name,
				VarType_For_Enum(p->w.vt),
				p->describe ? p->describe : "");
			idx ++; // Only for properties
		}
		Con_PrintLineEmpty ();
		Con_PrintLinef ("minteger is a metric integer that scales on-screen like FontSize or Columns Widths");
		Con_PrintLinef ("true / false are caseless accepted for integer and float properties only.");
		return;
	}

	if (!csqc_form) {
		Con_PrintLinef ("No CSQC form");
		return;
	}

	oject_s *f = csqc_form;

	if (cmd_argc == 1) {		// ui_set
		stringlist_t lines = {0};
		Form_Dump (f, &lines, DUMP_DETAIL_SAVE_FILE_0, magni_opt_null);
		//stringlistsort_no_unique (&lines);
		stringlist_condump_raw (&lines);
		stringlistfreecontents (&lines);
		Con_PrintLinef ("Type " CON_BRONZE "oset help" CON_WHITE " for use options");
		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "focused")) {		// ui_set list (all controls)
		// Names only
		ccs *sfocused = f->frm.kfocused ? f->frm.kfocused->cm.name_a : "none";
		Con_PrintLinef ("Focused: %s", sfocused);
		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "list")) {		// ui_set list (all controls)
		// Names only
		stringlist_t lines = {0};
		Form_Dump (f, &lines, DUMP_DETAIL_NAMES_ONLY_3, magni_opt_null);
		stringlist_condump_raw (&lines);
		stringlistfreecontents (&lines);
		return;
	}

	if (cmd_argc == 2 && String_Match (s_object_name, "uncontained")) {		// ui_set list (all controls)
		// Names only
		stringlist_t lines = {0};
		Form_Dump (f, &lines, DUMP_DETAIL_UNCONTAINED_4, magni_opt_null);
		stringlist_condump_raw (&lines);
		stringlistfreecontents (&lines);
		return;
	}

	oject_s *k = Object_Find_Name (f, s_object_name);

	if (!k) {
		Con_PrintLinef ("Object not found");
		return;
	}

	if (cmd_argc == 2) { // ui_set Label1
		stringlist_t lines = {0};
		Object_Dump_Recursive (k, &lines, q_idx_NULL, DUMP_DETAIL_NORECURSE_6, magni_opt_null);
		stringlist_condump_raw (&lines);
		stringlistfreecontents (&lines);
		return;
	}

	if (cmd_argc == 3 && String_Match_Caseless ("SetFocus", cmd_argv_parm2)) { // ui_set Label1 SetFocus
		if (!f->frm.editmode) {
			Con_PrintLinef ("Form not in edit mode");
			return;
		}
		Oject_Focus_Set (k);
		//Con_PrintLinef ("%s", valuebuf);
		return;
	}

	// ui_set Label1 Color
	ccs *s_property_name = cmd_argv_parm2;
	const things_s *p = Thing_Find(s_property_name);
	if (!p || IsProperty(p) == false) {
		Con_PrintLinef ("Property %s not found", s_property_name);
		return;
	}

	if (cmd_argc == 3) { // ui_set Label1 Name
		char valuebuf[1024];
		qbool ok = UI_Fly (s_object_name, s_property_name, valuebuf, sizeof(valuebuf));
		if (!ok) {
			Con_PrintLinef ("[Value get failed for %s]", s_property_name);
			return;
		}
		Con_PrintLinef ("%s", valuebuf);
		return;
	}

	if (cmd_argc == 4 && String_Match_Caseless (cmd_argv_parm3,"dump") ) { // ui_set Label1 Name

		qbool ok = UI_Dump (s_object_name, s_property_name);
		if (!ok) {
			Con_PrintLinef ("Failed");
			return;
		}

		return;
	}


	char valuebuf[1024];

	Argv_Cumulate (cmd, /*low arg num*/ 3, valuebuf, sizeof(valuebuf) );
	Con_PrintLinef ("Accumulated value string = " QUOTED_S, valuebuf);

	//ccs *s_value = cmd_argv_parm3;
	int iserr = Object_Property_Set (k, s_property_name, valuebuf); // Returns error code :(

	if (iserr != OK_NOERROR_0) {
		Con_PrintLinef (CON_RED "Failed to set property %s to %s for %s!", s_property_name, valuebuf, s_object_name);
		return;
	}

	Form_QueueRefresh(f); //Object_Refresh_Recursive (f);

	Con_PrintLinef ("Ok!");
}

oject_s *form_top (oject_s *f)
{
	oject_s *ftop = f;
	while (ftop->ui.fModalBoss) {
		ftop = ftop->ui.fModalBoss;
	}
	return ftop;
}

// How detect video mode change and what to do? DONE.
RELATED_ (Form_QueueDotsRefresh VM_CL_Form_Load)
RELATED_ (M_CSQC_Form_Key different file)
static void M_CSQC_Form_Draw (void)
{
	GoogleRobotoFont_Check ();
	oject_s *f = csqc_form;

	// No starting form.
	if (!f)
		return;

	// Closing.  Destroy the form.
	if (f->ui.form_is_closing) {
		prvm_prog_t *prog = CLVM_prog;
		int handle = UNPLUS1(f->ui.csqc_handle_plus1);
		prog->forms[handle] = NULL;
		func_t close_callback = f->ui.form_close_qcfunc;
		if (close_callback)
			CL_VM_String_Callback (f->ui.fModalBoss ? f->ui.fModalBoss->ui.csqc_handle_plus1 : 0, f->ui.form_close_qcfunc, f->ui.sMessageClose);
		Mem_FreeNull_ (f->ui.sMessageClose);
		// CSQC_Form_Destroy_VoidP sets csqc_form to up a layer.
		f = (oject_s *)CSQC_Form_Destroy_VoidP(f); // Sets csqc_form
		f = csqc_form; // Form f is now the parent form (or NULL)
	}

	// No ending form after possible close of a topmost form.
	// Or the form is hidden.
	if (!f)
		return;

#pragma message ("Baker: This does not recursively reload forms top to bottom")
	while (m_csqc_form_video_restarted) {
		// Save and restart the form updating the appropriate CSQC handle
		#pragma message ("Video restart must reload ALL CSQC forms. Should modal be cancelled.  Hmmm.")
		m_csqc_form_video_restarted = false;

		prvm_prog_t *prog = CLVM_prog;

		if (prog->loaded == false) {
			// Probably impossible.
			Con_PrintLinef (CON_RED "CSQC form without CSQC running");
			break;
		}
		//// Do something?
		//// Need to figure out the slot and update it.
		//int handle = not_found_neg1;
		//for (int j = 0; j < PRVM_MAX_OPEN_FORMS_8; j ++) {
		//	if (prog->forms[j] != csqc_form)
		//		continue;
		//	handle = j;
		//	break;
		//} // for
		//if (handle == not_found_neg1) {
		//	// Probably impossible.
		//	Con_PrintLinef (CON_RED "CSQC form can't find handle");
		//	break;
		//}

		int handle = UNPLUS1(f->ui.csqc_handle_plus1);
		func_t my_event_callback = f->ui.form_event_qcfunc;
		func_t my_close_callback = f->ui.form_close_qcfunc;
		oject_s *fModalBoss = f->ui.fModalBoss;

		char *s_z = Form_To_String_Zalloc (f, magni_opt_null);
#ifdef _DEBUG
		Clipboard_Set_Text (s_z);
#endif

		csqc_form = (oject_s *)Form_Destroy (f);
		oject_s *f2 = (oject_s *)CSQC_Form_Create_From_String_VoidP (s_z, handle, my_event_callback, my_close_callback, /*msg*/ NULL, fModalBoss);
		//f2->ui.csqc_handle_plus1 = PLUS1(handle);
		Con_PrintLinef ("Form reloaded");
		Mem_FreeNull_ (s_z);
		csqc_form = f = f2; // Update handle
		prog->forms[handle] = csqc_form;
		Con_PrintLinef ("CSQC Form handle updated");
		break;
	}

	if (f->is_hidden /*== true*/)
		return;

	oject_s *ftop = form_top(f);
	while (ftop) {
		Form_Draw (ftop);
		ftop = ftop->ui.fModalChild;
		if (ftop) {
			DrawQ_Fill(0, 0, vid_conwidth.integer, vid_conheight.integer, q_rgba_alpha75_black_4_parms, DRAWFLAG_NORMAL_0);
		}
	}
}

#if 0
void XOR_CSQC_Form_Draw (void)
{
	GoogleRobotoFont_Check ();
	oject_s *f = csqc_form;

	if (!f)
		return;

	if (f->ui.form_is_closing) {
		prvm_prog_t *prog = CLVM_prog;
		int handle = UNPLUS1(f->ui.csqc_handle_plus1);
		prog->forms[handle] = NULL;
		func_t close_callback = f->ui.form_close_qcfunc;
		if (close_callback)
			CL_VM_String_Callback (bad f->ui.fModalBoss, f->ui.form_close_qcfunc, f->ui.sMessageClose);
		Mem_FreeNull_ (f->ui.sMessageClose);
		f = (oject_s *)CSQC_Form_Destroy_VoidP(f); // Sets csqc_form
		f = csqc_form; // Form f is now the parent form (or NULL)
	}

	if (!f)
		return;

	while (m_csqc_form_video_restarted) {
		// Save and restart the form updating the appropriate CSQC handle
		#pragma message ("Video restart must reload ALL CSQC forms. Should modal be cancelled.  Hmmm.")
		m_csqc_form_video_restarted = false;

		prvm_prog_t *prog = CLVM_prog;

		if (prog->loaded == false) {
			// Probably impossible.
			Con_PrintLinef (CON_RED "CSQC form without CSQC running");
			break;
		}
		//// Do something?
		//// Need to figure out the slot and update it.
		//int handle = not_found_neg1;
		//for (int j = 0; j < PRVM_MAX_OPEN_FORMS_8; j ++) {
		//	if (prog->forms[j] != csqc_form)
		//		continue;
		//	handle = j;
		//	break;
		//} // for
		//if (handle == not_found_neg1) {
		//	// Probably impossible.
		//	Con_PrintLinef (CON_RED "CSQC form can't find handle");
		//	break;
		//}

		int handle = UNPLUS1(f->ui.csqc_handle_plus1);
		func_t my_event_callback = f->ui.form_event_qcfunc;
		func_t my_close_callback = f->ui.form_close_qcfunc;
		oject_s *fModalBoss = f->ui.fModalBoss;

		char *s_z = Form_To_String_Zalloc (f, magni_opt_null);
#ifdef _DEBUG
		Clipboard_Set_Text (s_z);
#endif

		csqc_form = (oject_s *)Form_Destroy (f);
		oject_s *f2 = (oject_s *)CSQC_Form_Create_From_String_VoidP (s_z, handle, my_event_callback, my_close_callback, /*msg*/ NULL, fModalBoss);
		//f2->ui.csqc_handle_plus1 = PLUS1(handle);
		Con_PrintLinef ("Form reloaded");
		Mem_FreeNull_ (s_z);
		csqc_form = f = f2; // Update handle
		prog->forms[handle] = csqc_form;
		Con_PrintLinef ("CSQC Form handle updated");
		break;
	}

	oject_s *ftop = form_top(f);
	while (ftop) {
		Form_Draw (ftop);
		ftop = ftop->ui.fModalChild;
		if (ftop) {
			DrawQ_Fill(0, 0, vid_conwidth.integer, vid_conheight.integer, q_rgba_alpha75_black_4_parms, DRAWFLAG_NORMAL_0);
		}
	}
}
#endif

RELATED_ (Form_QueueDotsRefresh VM_CL_Form_Load Property_Set_By_String_After_Freeing_Is_Ok)
RELATED_ (VarPack_String_Zalloc UI_Set_f UI_Fly Form_Create_From_String CL_VM_FormEvent)
RELATED_ (Form_ContextMenu_EditModeActivate O_ContextMenu_Popup_Method O_ContextMenu_KeyDown)
RELATED_ (O_ContextMenu_Leaving form_event_qcfunc Form_CSQC_Event_EZ CL_VM_FormEvent)
RELATED_ (Object_Dump_Recursive Form_Dump)
RELATED_ (CL_UpdateScreen_SCR_DrawScreen)
RELATED_ (M_CSQC_Form_Draw M_CSQC_Form_Key)

#include "menu_csqc_form_key.c.h"

// Calls QuakeC how and where?




