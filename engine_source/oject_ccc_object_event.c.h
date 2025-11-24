// oject_ccc_object_event.c.h

RELATED_ (event_entries)
exofn_t Event_Find_Function(ccs *s_eventname)
{
	for (int j = 0; j < (int)ARRAY_COUNT(event_entries); j ++) {
		event_entry_s *zi = &event_entries[j];
		if (String_Match_Caseless(zi->name, s_eventname)) {
			return zi->fn;// (f, k, listidx, s);
		}
	}
	//Con_PrintLinef ("zexeccmd: command function " QUOTED_S " not found", s_fn);
	return NULL;
}


RELATED_ (event_entries DevInit Object_Draw_Recursive_Not_Form)
// Baker: At this time no one calls this recursively
// That will eventually change with containers.
CALLERS_ (O_TabSelect_MouseButtonAction Lister_KeyDown) // OOF!
void Object_Event_OnChange_Recursive (oject_s *k, int shall_recurse)
{
	// Baker: Form_Create_From_String -> Form_Event_Onload fires this for everything
	// The only reason it doesn't fire for everything is lack of CSQC handle
	Form_Get (f,k);
	while (f->ui.csqc_handle_plus1) {
		while (k->cm.name_a && k->cm.name_a[0] && k->po->enum_id != class_rectangle) {
			// Construct function name like ListViewKeys
			//va_super (s_funcname, 256, "%s_%s", k->cm.name_a, "OnChange");
			RELATED_ (CL_VM_FormEvent VM_map_compile_with_callback SV_CheckContentsTransition)
			//	s_funcname k->cm.name_a
			Form_CSQC_Event_EZ (k, "OnChange", k->selectedindex, EVENT_VALSTR_NULL);
			//CL_VM_FormEvent (UNPLUS1(f->ui.csqc_handle_plus1), f->ui.form_event_qcfunc, k->cm.name_a, "OnChange", k->selectedindex, NULL);
			break;
		}
#if 0
		return; // OH SHIT THIS DOES NOT RECURSE! Jan 17 2025
#endif
		break;
	}

	if (k->eve.onchange_a && k->eve.onchange_a[0]) {
		exofn_t onclick_fn = Event_Find_Function(k->eve.onchange_a);
		if (onclick_fn) {
			char *s_error_za = onclick_fn (k);
			if (s_error_za) {
				Con_PrintLinef ("ERROR: %s: " QUOTED_S, k->eve.onchange_a, s_error_za);
				Z_FreeNull_ (s_error_za);
			} // if error
		} else {
			// NOT found
			c_assert_msg_ (0, "Object_Event_OnChange not found!");
		}
	} // if text set

	if (shall_recurse) {
		//Form_Get (f,k);
		for_each_form_control_with_container_as_no_servants (n, f, k, kcontained)
			//for (int NN = 0; NN < f->frm.controls_a->numitems; NN ++) {
			//oject_s *kcontained = f->frm.controls_a->pointers_array[NN];
			if (kcontained->container != k) continue;
			if (kcontained->servant_owner) continue;
			if (kcontained->is_hidden) continue; // Don't draw hidden.
			Object_Event_OnChange_Recursive (kcontained, shall_recurse);
			//}
		for_each_end
	}
}


