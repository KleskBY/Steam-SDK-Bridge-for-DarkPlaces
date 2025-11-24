// oject_cca_properties.c.h

qbool Event_Set_By_String_Is_Ok (oject_s *k, const things_s *h, things_s *hbuiltin)
{
	RELATED_ (oject_s things_s things)

	size_t		offsetx	= h->moffsetof;
	//vartype_e	vt		= (vartype_e)h->vt;

	if (h->moffsetof == 0) {
		Con_PrintLinef ("Internal: Event %s has no offsetof", h->name);
		return false;
	}

	int is_ok = true;

	varpack_u *vp = (varpack_u *) ((byte *)(k) + offsetx);

	vp->char_p = Z_StrDup (hbuiltin->name);

	return is_ok; // is ok
}



RELATED_ (/*Many callers!*/)
// Does Size Dots use this or do something else?
// Size dots does ... kshadow->cm.relative_rect = r_shadow_new; // ADJUSTMENT HERE SIZEOIC
CALLERS_ ( Object_Create_And_Assign)
CALLERS_ ( _RunDefaultString)
CALLERS_ ( Object_Property_Set_Fmt)
CALLERS_ ( Object_Property_Set)
CALLERS_ ( Form_ContextMenu_Spawn )
CALLERS_ ( Form_Create_From_String VarPack_String_Zalloc)
// Property Get and Set must return the virtual size.

RELATED_ (VarPack_String_Zalloc UI_Set_f)

CALLERS_ (Object_Property_Set_Fmt Object_Dump_Recursive)
// How Form_Dump Save
RELATED_ (Form_Dump  Object_Dump_Recursive Property_Dump_Maybe)
qbool Property_Set_By_String_After_Freeing_Is_Ok (oject_s *k, const things_s *h, ccs *s_value, ccs *s_base64)
{
	RELATED_ (oject_s things_s things)
	Form_Get (f,k);

	size_t		offsetx	= h->moffsetof;
	vartype_e	vt		= (vartype_e)h->w.vt;

#if 1 // How to determine if a control is "finalized?"
	// UGH ... textbox clear selection upon setting text.
	if (h->enum_id == prop_text) {
		if (k->po->enum_id == class_textbox)
			if (k->cm.is_finalized)
				TextBox_Cursor_Set_Refresh (k, 0, LENGTH_0);
	}
#endif

#if 1
	if (s_base64) {
		varpack_u *vp = (varpack_u *) ((byte *)(k) + offsetx);
		stringlistfreecontents (&vp->stringlist);  // CLEAN IT OUT TO ZERO

		// base64 has no failure mechanism here ...
		stringlist_append_from_base64_comma_delim_char12_decode (&vp->stringlist, s_base64);
		return true;
	}
#endif


	if (h->w.vt == 0) {
		Con_PrintLinef ("Property %s has no vartype", h->name);
		return false;
	}

	if (h->moffsetof == 0) {
		Con_PrintLinef ("Property %s has no offsetof", h->name);
		return false;
	}

	int is_ok = true;

	int s_value_slen = strlen(s_value);
	char *stemp_za = NULL;

	if (String_Is_Quoted(s_value, s_value_slen)) {
		// Remove quotes
		stemp_za = Z_StrDup_Len_Z (&s_value[1], s_value_slen - 2);
		s_value = stemp_za;
	}

	varpack_u *vp = (varpack_u *) ((byte *)(k) + offsetx);

	// These are required to full stomp and zero reset.
	//#pragma message ("oject property set TODO: detect already set condition for non-zero values?")

these_must_free_existing:
	switch (vt) {
	default:				is_ok = false; break;
	case vtstring_1:
		// Baker: Explain to me why this is ok for a NULL pointer.  Jan 18 2025
		// I think it is because I would have checked at the time.
		// A: It is maybe free and realloc macro so it is fine!

		// NOTE: Button_Finalize will turn some of these into "" if NULL
		if (!s_value[0]) {
			// Baker: Empty string shall free the variable and not realloc - Jan
			if (vp->char_p) {
				Mem_FreeNull_ (vp->char_p);
			}
			break;
		}
		Z_StrDup_Realloc (&vp->char_p, s_value);
		break;
	case mfloat_102:		// fall thru
	case vtfloat_2:
		if (String_Match_Caseless(s_value, "true"))
			vp->floatp = 1;
		else if (String_Match_Caseless(s_value, "false"))
			vp->floatp = 0;
		else
			vp->floatp = atof(s_value);
		if (vt == mfloat_102 && f /*form may not be ready*/ && f->ui.magnix.magnifier) {
			// Font Size
			vp->floatp *= f->ui.magnix.magnifier; // Magnify_ Float (font size)
			break;
		}
		break;

	case minteger_119:

	case vtinteger_19:
		if (String_Match_Caseless(s_value, "true"))
			vp->intp = 1;
		else if (String_Match_Caseless(s_value, "false"))
			vp->intp = 0;
		else
			vp->intp = atoi(s_value);
		if (vt == minteger_119 && f->ui.magnix.magnifier) {
			// Nothing uses this right now
			vp->intp *= f->ui.magnix.magnifier; // Magnify_ Integer (no uses yet)
			break;
		}
		break;

	case mrectf_10:
		{ // The float rect left/top/width/height
			stringlist_t rlist = {0};
			stringlistappend_split (&rlist, s_value, ",");

			memset (&vp->rectf, 0, sizeof(vp->rectf) );
			const char **wordray = stringlist_nullterm_add (&rlist);

			// space skip - not needed! yay!
			// atof: Function discards any whitespace characters (as determined by isspace) until
			// first non-whitespace character is found. Then it takes as many characters as possible
			// to form a valid floating-point representation and converts them to a floating-point
			// value.

			if (*wordray) vp->rectf.left	= atof(*wordray++);
			if (*wordray) vp->rectf.top		= atof(*wordray++);
			if (*wordray) vp->rectf.width	= atof(*wordray++);
			if (*wordray) vp->rectf.height	= atof(*wordray++);
			if (f->ui.magnix.magnifier) {
				// Nothing uses this yet.
				Magnify_Rectf (&vp->rectf, &f->ui.magnix, IsContained(k));
				break;
			}

			stringlistfreecontents (&rlist);
		}
		break;
	case mrecti_11:
		{ // The integer rect left/top/width/height

			stringlist_t rlist = {0};
			stringlistappend_split (&rlist, s_value, ",");

			memset (&vp->rectf, 0, sizeof(vp->recti) );
			const char **wordray = stringlist_nullterm_add (&rlist);

			// space skip - not needed! yay!
			// atof: Function discards any whitespace characters (as determined by isspace) until
			// first non-whitespace character is found. Then it takes as many characters as possible
			// to form a valid floating-point representation and converts them to a floating-point
			// value.

			if (*wordray) vp->recti.left	= atoi(*wordray++);
			if (*wordray) vp->recti.top		= atoi(*wordray++);
			if (*wordray) vp->recti.width	= atoi(*wordray++);
			if (*wordray) vp->recti.height	= atoi(*wordray++);

			if (f->ui.magnix.magnifier) {
				Magnify_Recti (&vp->recti, &f->ui.magnix, IsContained(k));
				break;
			}

			stringlistfreecontents (&rlist);
		}
		break;

//
// COMPLICATED: Color "RGB(255,255,255)" or "white" ...
//

	case vtrgb_21:
		// Word support

		// Skip the rgb
		{
			ccs *srgb		= dpstrcasestr	(s_value, "rgb("); // Find string caseless
			//String_Starts_With_Caseless is not quite the same

			if (String_Contains(s_value, "#") || String_Contains(s_value, "0x")) {
				vec3_t vcolor = {0};
				String_HTMLColor255_From_String (vcolor, s_value); // 0-255 range
				if (vcolor[0]) vcolor[0] = (vcolor[0] + 1)/256.0;
				if (vcolor[1]) vcolor[1] = (vcolor[1] + 1)/256.0;
				if (vcolor[2]) vcolor[2] = (vcolor[2] + 1)/256.0;
				VectorCopyDestSrc (vp->vec3, vcolor);
				break;
			}


			if (!srgb) {
				// Try word
				char *s_z = Z_StrDup(s_value);
				String_Edit_DeQuote (s_z);
				int was_set = false;
				RELATED_ (FS_ColorPcts_f)
				if (String_Match_Caseless (s_z, "white")) {
					VectorSet(vp->vec3, 1.0, 1.0, 1.0);
					//vp->vec3[0] = 1.0; //colorxs[0] / 255.0; // 255 = 1, 0 = 0
					//vp->vec3[1] = 1.0; //colorxs[1] / 255.0; // 255 = 1, 0 = 0
					//vp->vec3[2] = 1.0; //colorxs[2] / 255.0; // 255 = 1, 0 = 0
					was_set = true;
				} else if (String_Match_Caseless (s_z, "black")) {
					VectorSet(vp->vec3, 0, 0, 0);
					//vp->vec3[0] = 0.0; //colorxs[0] / 255.0; // 255 = 1, 0 = 0
					//vp->vec3[1] = 0.0; //colorxs[1] / 255.0; // 255 = 1, 0 = 0
					//vp->vec3[2] = 0.0; //colorxs[2] / 255.0; // 255 = 1, 0 = 0
					was_set = true;
				}

				Z_FreeNull_ (s_z);
				if (was_set)
					break;
				break;
			}

			ccs *sendparen	= strstr		(s_value, ")");

			if (srgb && sendparen && sendparen > srgb) {
				int sizeofthis = sendparen - srgb - STRINGLEN("rgb(");
				if (sizeofthis >=0) {
					ccs *s_after_rgb = srgb + STRINGLEN("rgb(");
					byte colorxs[3] = {0};

					// These are integer values
					stringlist_t rlist = {0};
					stringlistappend_split_len (&rlist, s_after_rgb, sizeofthis, ",");

					vp->vec3[0] = vp->vec3[1] = vp->vec3[2] = 0;

					const char **wordray = stringlist_nullterm_add (&rlist);
					// space skip - not needed! yay!
					// atof: Function discards any whitespace characters (as determined by isspace) until
					// first non-whitespace character is found. Then it takes as many characters as possible
					// to form a valid floating-point representation and converts them to a floating-point
					// value.

					if (*wordray) colorxs[0]	= atoi(*wordray++);
					if (*wordray) colorxs[1]	= atof(*wordray++);
					if (*wordray) colorxs[2]	= atof(*wordray++);
					stringlistfreecontents (&rlist);

					VectorSet (vp->vec3, colorxs[0] / 255.0, colorxs[1] / 255.0, colorxs[2] / 255.0);
					//vp->vec3[0] = colorxs[0] / 255.0; // 255 = 1, 0 = 0
					//vp->vec3[1] = colorxs[1] / 255.0; // 255 = 1, 0 = 0
					//vp->vec3[2] = colorxs[2] / 255.0; // 255 = 1, 0 = 0
				}
			} else {
				c_assert_msg_ (0, "Rect didn't parse");
			}
		}
		break;

//
// ARRAYS
//

	case vtlist_str_20:
		stringlistfreecontents (&vp->stringlist);  // CLEAN IT OUT TO ZERO
		{ // Comma delimited string list.

			stringlistappend_split (&vp->stringlist, s_value, ",");
		}

		break;

	case mvertflist_22:
		// Vertexes:"20 20, 500 400, 550 450"
		floats_freecontents (&vp->floatlist);
		// NOT A BAKER ARRAY ... Baker_Array_Erase (&vp->floatlist); // CLEAN IT OUT TO ZERO
		{
			int num_bad =
				floats_append_parse_space_comma_num_elements_ignored
					(&vp->floatlist, s_value, POLYGON_2D_2,  POLYGON_2D_FLOAT_SEPARATOR_SPACE, POLYGON_2D_ELEMENT_SEPARATOR_COMMA);
			if (num_bad)
				Con_PrintLinef ("Float parse %d elements ignored on parse", num_bad);
#ifdef _DEBUG
			floats_dump (&vp->floatlist);
#endif
		}
		break;

	case vtcontrolref_23:
		// We have a name.  Find the name ...
		// January 10 2025: Does this fire without them? Right now. NO!
		// Also it isn't finding the name at all.
#if 1 // January 10 2025
		{
			Form_Get(f2,k);
			oject_s *kref = Object_Find_Name (f2, s_value);
			vp->ojectp = kref;
		}
#endif
		break;

	case mint32list_124: // fall thru
	case vtint32list_24:
		// ColumnsWidths:"20 20, 500 400, 550 450"

		// THIS IS NOT A BAKER ARRAY
		///Baker_Array_Erase (&vp->int32list); // CLEAN IT OUT TO ZERO
		int32s_freecontents (&vp->int32list);
		{
			int32s_append_split_dequote (&vp->int32list, s_value, POLYGON_2D_ELEMENT_SEPARATOR_COMMA);
			//if (num_bad)
			//	Con_PrintLinef ("Int parse %d elements ignored on parse", num_bad);
#if 0 //def _DEBUG
			int32s_dump (&vp->int32list);
#endif
		}
		if (vt == mint32list_124 && f->ui.magnix.magnifier) {
			// Scale column widths
			int32s_command_scale (&vp->int32list, f->ui.magnix.magnifier); // Magnify_ Columns Widths
			break;
		}

		break;

	case vtappearance_200:
		if (String_Isin2_Caseless(s_value, "default_0", "default")) {
			vp->intp = appearance_default_0;
			break;
		}

		if (String_Isin2_Caseless(s_value, "flat_1", "flat")) {
			vp->intp = appearance_flat_1;
			break;
		}
		vp->intp = atoi(s_value);
		break;

	case vtfontnameindex_201:
		if (String_Isin2_Caseless(s_value, "default_0", "default"))	{ vp->intp = fontnameindex_default_0;	break; }
		if (String_Isin1_Caseless(s_value, "system"))	{ vp->intp = fontnameindex_system_neg1;break; }

		if (String_Isin1_Caseless(s_value, "console"))	{ vp->intp = fontnameindex_console_1;break; }
		if (String_Isin1_Caseless(s_value, "sbar"))		{ vp->intp = fontnameindex_sbar_2;break; }
		if (String_Isin1_Caseless(s_value, "notify"))	{ vp->intp = fontnameindex_notify_3;break; }
		if (String_Isin1_Caseless(s_value, "chat"))		{ vp->intp = fontnameindex_chat_4;break; }
		if (String_Isin1_Caseless(s_value, "centerprint"))	{ vp->intp = fontnameindex_centerprint_5;break; }
		if (String_Isin1_Caseless(s_value, "infobar"))	{ vp->intp = fontnameindex_infobar_6;break; }
		if (String_Isin1_Caseless(s_value, "menu"))		{ vp->intp = fontnameindex_menu_7;break; }

		if (String_Isin1_Caseless(s_value, "user0"))	{ vp->intp = fontnameindex_user0_8;	break; } // +6
		if (String_Isin1_Caseless(s_value, "user1"))	{ vp->intp = fontnameindex_user1_9;	break; } // +6
		if (String_Isin1_Caseless(s_value, "user2"))	{ vp->intp = fontnameindex_user2_10;break; } // +6
		if (String_Isin1_Caseless(s_value, "user3"))	{ vp->intp = fontnameindex_user3_11;break; } // +6
		if (String_Isin1_Caseless(s_value, "user4"))	{ vp->intp = fontnameindex_user4_12;break; } // +6
		if (String_Isin1_Caseless(s_value, "user5"))	{ vp->intp = fontnameindex_user5_13;break; } // +6
		if (String_Isin1_Caseless(s_value, "user6"))	{ vp->intp = fontnameindex_user6_14;break; } // +6
		if (String_Isin1_Caseless(s_value, "user7"))	{ vp->intp = fontnameindex_user7_15;break; } // +6

		vp->intp = atoi(s_value);
		break;

	case vtfocusstyle_202:
		if (String_Isin2_Caseless(s_value, "default_0", "default")) {
			vp->intp = focusstyle_default_0;
			break;
		}
		if (String_Isin2_Caseless(s_value, "nohighlight_1", "nohighlight")) {
			vp->intp = focusstyle_nohighlight_1;
			break;
		}
		vp->intp = atoi(s_value);
		break;
	} // sw

	Z_FreeNull_ (stemp_za);
	return is_ok; // is ok
}

#if 1
RELATED_ (CSQC_Object_Property_Set_Float_Ok)
qbool _Object_Property_Set_Float (oject_s *k, const things_s *ppv, float fval)
{
	RELATED_ (oject_s things_s things)
	//Form_Get (f,k);

	size_t		offsetx	= ppv->moffsetof;
	vartype_e	vt		= (vartype_e)ppv->w.vt;

	if (ppv->w.vt == 0 || ppv->moffsetof == 0) {
		// Baker: Jan 18 2025 - Code is too mature for this matter much, I doubt it can happen
		Con_PrintLinef ("Property %s has no vartype or no offsetof", ppv->name);
		return false;
	}

	// Baker: We are not supporting setting every property by float
	// Only the ones that are PURE numeric -- int and float.
	// Not even enums ... take anything not int or float and do it as string
	if (isin2 (vt, vtfloat_2, vtinteger_19) == false) {
		va_super (valuebuf, 16, FLOAT_LOSSLESS_FORMAT, fval);
		return Property_Set_By_String_After_Freeing_Is_Ok (k, ppv, valuebuf, NULL);
	}

	varpack_u *vp = (varpack_u *) ((byte *)(k) + offsetx);

	switch (vt) {
	case vtfloat_2:
		vp->floatp = fval;
		break;

	case vtinteger_19:
		vp->intp = fval;
		break;
	}
	return true;
}
#endif

void VarPack_String_Add_Detail (char **ps, oject_s *k, varpack_u *vp, const things_s *p)
{
	Form_Get (f, k);
	char *oldval = (*ps);
	char *newval = NULL;

	if (p->enum_id == prop_rect) {
		// RECT: Add beyonds
		rect_s rtranslatedi = vp->recti;

		if (f->ui.magnix.magnifier)
			Magnify_Reverse_Recti (&rtranslatedi, &f->ui.magnix, IsContained(k));

		newval = Mem_strdupf(devinfomempool, "%s // Beyond:%d %d", oldval, rtranslatedi.left + rtranslatedi.width, rtranslatedi.top + rtranslatedi.height);  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
		Mem_FreeNull_ (oldval);  // Free old value since we are replacing
		(*ps) = newval;
	}
	else if (p->w.vt == vtrgb_21) {
#if 0 // Now we will hint the color code instead
		// Color: Add RGB key and closest
		char s_htmlcolor[16]; // #fc3
		String_HTMLColor_To_String_Buf (vp->vec3, s_htmlcolor, sizeof(s_htmlcolor));
		char *s_color_escape_za = Color_Code_ZAlloc_Or_Null (vp->vec3);
		newval = Mem_strdupf(devinfomempool, "%s // %s " "(%sCOLOR" CON_WHITE")",
			oldval,
			s_htmlcolor,
			s_color_escape_za
		);
		Mem_FreeNull_ (s_color_escape_za);
#else
		 char *newval1 = Mem_strdupf(devinfomempool, "%s // RGB(%d,%d,%d)",
					oldval,
					(byte)(vp->vec3[0] * 255),
					(byte)(vp->vec3[1] * 255),
					(byte)(vp->vec3[2] * 255)
					);  // Jan 29 2025 -- GROSS use Z_StrDup_Realloc
#endif

		Mem_FreeNull_ (oldval); // Free old value since we are replacing
		(*ps) = newval1;
	}
}

qbool Dump_Shall_Force (oject_s *k, const things_s *p, varpack_u *vp)
{
	if (k->po->enum_id == class_label && p->enum_id == prop_autosize)
		return true;
	return q_force_false;
}

CALLERS_ (Form_Dump Form_To_String_Zalloc)
void Property_Dump_Maybe (oject_s *k, const things_s *p, stringlist_t *plist, qbool is_indent, mag_s *magni_opt, dump_detail_e detail_level)
{
	Form_Get (f, k);

	char *s_propname =  p->name;

	// We have a property - is it zero?
	vartype_e	property_vt		= (vartype_e)p->w.vt;
	size_t		offsetx			= p->moffsetof;
	varpack_u	*vp				= (varpack_u *)((byte *)k + offsetx);
	rect_s		vpx = {0};

	extern cvar_t scr_xtranslate;
	if (scr_xtranslate.string[0] && p->enum_id == prop_rect) {
		vec3_t vtrans={0};
		Math_atov3 (scr_xtranslate.string, vtrans);
		vpx = vp->recti;
		vpx.left += vtrans[0];
		vpx.top += vtrans[1];
		vp = (varpack_u *)&vpx; // Re-route
	}

	// "autosize if label control" get forced by Dump_Shall_Force (prop_autosize)
	qbool shall_force = Dump_Shall_Force(k, p, vp);
#if 0
	if (p->enum_id == prop_userstring) {
		int j = 5;
	}
#endif
	if (p->enum_id == prop_container && k->container && k->container == k->fctrl)
		return; // Waste of time to write form as container

	qbool did_base64 = false;
	/*PROPDUMP*/ char *s_propval_z = VarPack_Maybe_Quote_String_Zalloc (IsContained(k), vp, property_vt, magni_opt, &f->ui.magnix, shall_force, &did_base64);

	if (s_propval_z == NULL)
		return; // Nothing good to write (zero value, no string, etc.)

	//if (isin2(detail_level, DUMP_DETAIL_UNCONTAINED_4, DUMP_DETAIL_HELPFUL_5))
	if (detail_level != DUMP_DETAIL_SINGLE_LINE_5)
		VarPack_String_Add_Detail (&s_propval_z, k, vp, p); // Color code, enhanced rect data.

	if (did_base64) {
		size_t slen = strlen(s_propval_z);
		stringlistappendf (plist, "%s" "%s:base64:%d:%s", is_indent ? "  " : "", s_propname, (int)slen, s_propval_z);
	} else {
		stringlistappendf (plist, "%s" "%s:%s", is_indent ? "  " : "", s_propname, s_propval_z);
	}

	Z_FreeNull_ (s_propval_z);
}

CALLERS_ (Object_Dump_Recursive) RELATED_ (Event_Set_By_String_Is_Ok)



// Destroys name_a or frees an array
#ifdef _DEBUG
int num_frees = 0;
#endif
qbool Property_Destroy (oject_s *k, const things_s *h)
{
	RELATED_ (oject_s things_s things)
	qbool is_ok = true;
	size_t		offsetx	= h->moffsetof;
	vartype_e	vt		= (vartype_e)h->w.vt;
	varpack_u	*vp		= (varpack_u *) ((byte *)(k) + offsetx);

	switch (vt) {
	case vtstring_1:
		Z_FreeNull_ (vp->char_p);
#ifdef _DEBUG
		num_frees ++;
#endif
		break;

	case vtlist_str_20:
		stringlistfreecontents (&vp->stringlist);
#ifdef _DEBUG
		num_frees ++;
#endif
		break;

	case mvertflist_22:
		floats_freecontents (&vp->floatlist);
#ifdef _DEBUG
		num_frees ++;
#endif
		break;

	case mint32list_124: // fall thru
	case vtint32list_24:
		int32s_freecontents (&vp->int32list);
#ifdef _DEBUG
		num_frees ++;
#endif
		break;

	case vtcontrolref_23:
		// Just a reference.
		break;

	case mrectf_10:		// fall thru
	case mrecti_11:		// fall thru
		break;

	case mfloat_102:	// fall thru
	case vtfloat_2:		// fall thru

	case vtinteger_19:	// fall thru
	case vtrgb_21:		// fall thru
	case minteger_119:	// fall thru
	case vtappearance_200: // fall thru
	case vtfontnameindex_201: // fall thru
	case vtfocusstyle_202: // fall thru
		break;

	default:
		c_assert_msg_ (0, "Property destroy: Invalid vartype");
	} // sw

	return is_ok; // is ok
}
