// oject_ccx_form_parse.c.h

// Space delimited default values.
CALLERS_ (Object_Create_And_Assign)


oject_s *Form_Create_From_String (ccs *s, qbool with_magnification, int handle_plus_1, func_t my_event_callback, func_t my_close_callback, ccs *sMessage, void *fModalBoss)
{
	//size_t oject_sizeof = sizeof(oject_s);  Jan 20 2025 - 1280
	qbool is_quirks_mode_allowed = true;
	qbool in_quirks_mode = false;
	oject_s *formy = Form_Create();

#if 1
	if (handle_plus_1) {
		// Don't crash devinfo
		formy->ui.csqc_handle_plus1 = handle_plus_1;
		formy->ui.form_event_qcfunc = my_event_callback;
		formy->ui.form_close_qcfunc = my_close_callback;
		
		if (fModalBoss) {
			formy->ui.fModalBoss = (oject_s *)fModalBoss;
			formy->ui.fModalBoss->ui.fModalChild = formy;
		}

		int handle = UNPLUS1(handle_plus_1);
		CLVM_prog->forms[handle] = formy;
		//CLVM_prog->form_callbacks[handle] = 0;
		//CLVM_prog->form_callbacks[handle] = mycallback;

		Form_CSQC_Event_EZ (formy, "SendHandle", formy->ui.csqc_handle_plus1, EVENT_VALSTR_NULL);
		//CL_VM_FormEvent (UNPLUS1(formy->ui.csqc_handle_plus1), formy->ui.form_event_qcfunc, formy->cm.name_a, "SendHandle", formy->ui.csqc_handle_plus1, NULL);
	}
#endif

	if (with_magnification) {
		Magnify_From_Video (&formy->ui.magnix);
	}
	stringlist_t words = {0};
	int32list_s  linenumlist = {0};
	stringlistappend_tokenize_qcquotes (&words, &linenumlist, s); // 16384 WEAKNESS

	oject_s *k = NULL;

	const char **wordray = stringlist_nullterm_add (&words);
	const char **wordstart = wordray;
	for (/*nada*/ ; *wordray; *wordray ++) {
		if (DO_PRINT) { DebugPrintLinef ("Word %s", *wordray); }

		ccs *oldword = *wordray;

		if (String_Match(oldword, ":")) {
			int idx = wordray - wordstart;
			int ipos = linenumlist.ints[idx];
			int linenum = String_Find_Count (s, "\n", ipos);
			Con_PrintLinef (CON_ERROR "Colon found " QUOTED_S " at linenum %d num words in is %d", *wordray, linenum + 1, idx);
			break;
		}

		const things_s *thing = Thing_Find (*wordray);

		// Baker: We must be at a object, property or event.
		// If we don't know what it is, go to "quirks mode"
		// Ignore properties we don't know, but don't ignore object types?
		if (thing == NULL) {
			if (is_quirks_mode_allowed) {
				if (wordray[1] && wordray[1] && String_Match (wordray[1], PROPERTY_COLON_DELIM)) {
					Con_PrintLinef ("Entering quirks mode for unknown property " QUOTED_S
						" and skipping " QUOTED_S " " QUOTED_S, wordray[0], wordray[1], wordray[2]);
					in_quirks_mode = true;
					*wordray ++; // Skip this one
					*wordray ++; // Skip the property value
					continue;
				}
			} // quirks mode
			int idx = wordray - wordstart;
			int ipos = linenumlist.ints[idx];
			int linenum = String_Find_Count (s, "\n", ipos);
			Con_PrintLinef (CON_ERROR "Form Parse: Unknown control or property " QUOTED_S " at linenum %d", *wordray, linenum + 1);
			break;
		}

		if (IsObjectType(thing)) {
			if (thing->enum_id == class_form_1)
				k = formy;
			else {
				Object_Parse_Done (k); // Finalizes the previous object
				k = NULL;
				SET___ Object_Create_And_Assign (&k, formy, /*kcan:*/ formy, SERVANT_TO_NULL, thing);
			}
			continue;
		}

		if (IsProperty(thing)) {
#ifdef _DEBUG // Used in debug builds only
			ccs *s_prop = *wordray;
#endif // _DEBUG
			// Advance past property
			*wordray ++; Word_Check_Fail_Message ("No more words after property");

			if (String_Match(*wordray, PROPERTY_COLON_DELIM) == false) {
				Con_PrintLinef (CON_ERROR "No colon after property");
				break;
			}

			// Advance past semi-colon
			*wordray ++; Word_Check_Fail_Message ("No more words after property colon");

			// PROCESS PROPERTY
			ccs *s_value = *wordray;
			
#if 1
			// List:base64:dsfsfdsdfsdfdsf
			if (isin1(thing->w.vt, vtlist_str_20) && String_Match_Caseless(s_value, "base64")) {
				// Advance past "base64" value
				*wordray ++; Word_Check_Fail_Message ("No more words after property"); // colon

				if (String_Match(*wordray, PROPERTY_COLON_DELIM) == false) {
					Con_PrintLinef (CON_ERROR "No colon after property");
					break;
				}

				// Advance past semi-colon
				*wordray ++; Word_Check_Fail_Message ("No more words after property colon"); // ssize

				// PROCESS BASE64 size
#ifdef _DEBUG
				ccs *s_size = *wordray;
#endif
				// Advance past "size" value
				*wordray ++; Word_Check_Fail_Message ("No more words after property"); // colon

				if (String_Match(*wordray, PROPERTY_COLON_DELIM) == false) {
					Con_PrintLinef (CON_ERROR "No colon after property");
					break;
				}

				// Advance past semi-colon
				*wordray ++; Word_Check_Fail_Message ("No more words after property colon"); // ssize

				// PROCESS PROPERTY
				ccs *s_base64 = *wordray;

				Property_Set_By_String_After_Freeing_Is_Ok (k, thing, NULL, s_base64);
				continue;
			}
#endif

			if (DO_PRINT) {DebugPrintLinef ("Property %s = " QUOTED_S, s_prop, s_value); }
			/*unused qbool is_ok =*/ Property_Set_By_String_After_Freeing_Is_Ok (k, thing, s_value, NULL);
			continue;
		}

		Con_PrintLinef (CON_ERROR "%s isn't anything", *wordray);
		break;
	} // for words

	int is_ok = *wordray == NULL;

	if (is_ok) {
		Object_Parse_Done (k); // Finalizes the previous object
		Form_Event_Onload (formy); // Runs before anything refreshes
		Form_Finalize_Refresh (formy);
	} else {
		oject_s *Form_Destroy (oject_s *f);
		formy = Form_Destroy (formy);

		// Must destroy these too ..
		//formy->ui.csqc_handle_plus1 = handle_plus_1;
		//formy->ui.form_event_qcfunc = mycallback;
		prvm_prog_t *prog = CLVM_prog;

		int handle = UNPLUS1(handle_plus_1);

		// Handle >= 0 is always true because our plus 1 handles start at ONE -- but GCC thinks the final handle could be below zero.
		if (handle >= 0) { // Baker: This is to dodge the following GCC warning that does not apply ...
            prog->forms[handle] = NULL; // GCC: is kicking a warning but it is invalid -- it thinks handle might be -1. Jan 20 2025
            //prog->form_callbacks[handle] = 0; // GCC: is kicking a warning but it is invalid -- it thinks handle might be -1. Jan 20 2025
		}

		//Form_CSQC_Event_EZ (formy, "SendHandle", formy->ui.csqc_handle_plus1, EVENT_VALSTR_NULL);

	}

	Con_DPrintLinef ("Form_Create_From_String: %s Parse was %s", in_quirks_mode ? "^6(QUIRKS MODE)^7" : "", is_ok ? "^5" "GOOD" : CON_ERROR "BAD");
	stringlistfreecontents (&words);
	int32s_freecontents (&linenumlist);

	if (formy && formy->wasinitialized == false) {
		// Prevent video mode change from firing this again.
		Form_CSQC_Event_EZ (formy, "InitializedWithMessage", formy->ui.csqc_handle_plus1, sMessage);
		formy->wasinitialized = true;
	}

	return formy;
}

