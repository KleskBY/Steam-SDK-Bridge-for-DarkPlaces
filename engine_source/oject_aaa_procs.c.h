// oject_aaa_procs.c.h




void Label_Rect_Edit_Align (rect_s *r_text, const rect_s *r_area, alignment_e alignment)
{
	switch ((int)alignment / 3) {
	case 0:		r_text->top = r_area->top;
				break;

	case 1:		r_text->top = r_area->top + (r_area->height / 2.0 - r_text->height / 2.0);
				break;

	case 2:		r_text->top = RECT_BOTTOMOF (*r_area) - r_text->height; // right
				break;
	}

	switch ((int)alignment % 3) {
	case 0:		r_text->left = r_area->left;
				break;

	case 1:		r_text->left = r_area->left + (r_area->width / 2.0 - r_text->width / 2.0);
				break;

	case 2:		r_text->left = RECT_RIGHTOF (*r_area) - r_text->width;
				break;
	}
}

void Label_Rect_Edit_AlignF (frect *r_text, const frect *r_areaf, alignment_e alignment)
{
	switch ((int)alignment / 3) {
	case 0:		r_text->top = r_areaf->top;
				break;

	case 1:		r_text->top = r_areaf->top + (r_areaf->height / 2.0 - r_text->height / 2.0);
				break;

	case 2:		r_text->top = RECT_BOTTOMOF (*r_areaf) - r_text->height; // right
				break;
	}

	switch ((int)alignment % 3) {
	case 0:		r_text->left = r_areaf->left;
				break;

	case 1:		r_text->left = r_areaf->left + (r_areaf->width / 2.0 - r_text->width / 2.0);
				break;

	case 2:		r_text->left = RECT_RIGHTOF (*r_areaf) - r_text->width;
				break;
	}
}

//// No callers May 2 2024
//float Refresh_List_Longest_StringWidthPx (oject_s *k)
//{
//	// Longest item!
//	float stringmaxwidthpx = 0;
//	for (int n = 0; n < k->list_strings_a.numstrings; n ++) {
//		const char *s = semp(k->list_strings_a.strings[n]);
//
//		float sw = Draw_StringWidth (zdev_dpfont, s, k->fontsize);
//
//		if (sw > stringmaxwidthpx) stringmaxwidthpx = sw;
//	} // for
//	return stringmaxwidthpx;
//}

#define DP_MAX_POLYGON_VERTS_64 64 // Baker: Not true in DP Beta
#define m_num_segments_61 (DP_MAX_POLYGON_VERTS_64 - 3)





// returns xy of pct -- pos = circlepos (circle_center_pos, r, 1 / m_num_segments_61);
void circle_setpos_for_pct (/*modify*/ vec3_t out_modified, const vec3_t circle_center_pos, float r, float pct)
{
	pct=pct -0.25;
	float theta = 2.0 * M_PI * pct;
	out_modified[0] = (r * cos(theta)) + circle_center_pos[0];
	out_modified[1] = (r * sin(theta)) + circle_center_pos[1];
}


// Supply x, y, w, h and parent rect
// pr2_mine is filled
void Rect_Calc_Clipped_Unclipped_From_XYWH (rect_s *pr_mine_unclipped,
 rect_s *pr_mine_clipped, const rect_s *pr_parent_clipped, int left, int top, int width, int height)
{
	// If parent was culled, so are we
	qbool is_parent_culled = pr_parent_clipped->width <= 0;
	qbool are_we_culled = is_parent_culled;

	// Set the controls unclipped area
	RECT_SET (*pr_mine_unclipped, left, top, width, height);

	// Clip area begins as actual area
	*pr_mine_clipped = *pr_mine_unclipped;

	// If not parent culled, see if we are.
	if (is_parent_culled == false) {
		// Clipped area not written if culled.
		int res = Rect_Clip_Area_Result (pr_mine_unclipped, pr_parent_clipped,
			pr_mine_clipped);
		if (res == CULLED_1)
			are_we_culled = true;
	}

	// Any cull, we set width to 0
	if (are_we_culled)
		pr_mine_clipped->width = RECT_CULLED_ZERO;
}

const char *VarType_For_Enum(int vt)
{
	switch (vt) {
	case 0: return "[illegal vartype zero]";
	case vtstring_1: return "string";
	case vtfloat_2: return "float";
	case vtinteger_19: return "integer";

	case mrectf_10: return "rectf";
	case mrecti_11: return "recti";
	case vtlist_str_20: return "list";
	case vtrgb_21: return "rgbfloat3";
	case mvertflist_22: return "vertslist";
	case vtcontrolref_23: return "controlref";
	case vtint32list_24: return "intslist";

	// SIZEOIC x 3
	case mfloat_102: return "mfloat"; //		= 102,	// metric float .. FontSize
	case minteger_119: return "minteger";	// = 119,	// Metric integer .. Width (Who uses this)
	case mint32list_124: return "mintslist"; //	= 124,	// Column sizes

	case vtappearance_200: return "appearance (int)";
	case vtfontnameindex_201: return "fontname (int)";
	case vtfocusstyle_202: return "focusstyle (int)";
	}
	return "[unknown vartype]";
}


void Things_Audit_Debug (void)
{
	// Making sure non-zero moffsetof are unique
	// Making sure names are unique
	// Making sure enum_id as unique if not certain values.

	int count1, count2;
	stringlist_t list = {0};

names:

	for (const things_s *p = &things[1]; p->name; p ++) { // Start at 1
		stringlistappend (&list, p->name); // Names
	} // for each property

	count1 = list.numstrings;
	stringlistsort (&list, fs_make_unique_false);
	char *za1 = stringlist_join_spaced_zalloc(&list);
	stringlistsort (&list, fs_make_unique_true);
	count2 = list.numstrings;

	if (count1 != count2) {
		// Who is missing?
		char *za2 = stringlist_join_spaced_zalloc(&list);
		va_super (tmp, 32768, "%s" NEWLINE "%s", za1, za2);
		Clipboard_Set_Text (tmp); // Things audit

		c_assert_msg_ (0, "NAMES count1 is not count2 there is probably a duplicate");
		Z_FreeNull_ (za2);
	}
	Z_FreeNull_ (za1);

	stringlistfreecontents (&list);


moffsetofs: // PART2

	for (const things_s *p = &things[1]; p->name; p ++) { // Start at 1
		if (p->moffsetof > 0)
			stringlistappendf (&list, "%d", (int)p->moffsetof);
	} // for each property

	count1 = list.numstrings;
	stringlistsort (&list, fs_make_unique_false);
	za1 = stringlist_join_spaced_zalloc(&list);
	stringlistsort (&list, fs_make_unique_true);
	count2 = list.numstrings;

	if (count1 != count2) {
		// Who is missing?
		char *za2 = stringlist_join_spaced_zalloc(&list);
		va_super (tmp, 32768, "%s" NEWLINE "%s", za1, za2);
		Clipboard_Set_Text (tmp); // Things audit

		c_assert_msg_ (0, "moffset count1 is not count2");
		Z_FreeNull_ (za2);
	}
	Z_FreeNull_ (za1);
	stringlistfreecontents (&list);

enumids: // part3 enum id

	for (const things_s *p = &things[1]; p->name; p ++) { // Start at 1
		if (isin2 (p->enum_id, thing_none_0, builtin_func))
			continue; // ignore these
		if (p->enum_id > 0)
			stringlistappendf (&list, "%d", (int)p->enum_id);
	} // for each property

	count1 = list.numstrings;
	stringlistsort (&list, fs_make_unique_false);
	za1 = stringlist_join_spaced_zalloc(&list);
	stringlistsort (&list, fs_make_unique_true);
	count2 = list.numstrings;

	if (count1 != count2) {
		// Who is missing?
		char *za2 = stringlist_join_spaced_zalloc(&list);
		va_super (tmp, 32768, "%s" NEWLINE "%s", za1, za2);
		Clipboard_Set_Text (tmp); // Things audit

		c_assert_msg_ (0, "enumid count1 is not count2");
		Z_FreeNull_ (za2);
	}
	Z_FreeNull_ (za1);
	stringlistfreecontents (&list);

}

static const things_s *Thing_Find(const char *s)
{
	for (const things_s *p = &things[0]; p->name; p ++) {
		if (!p->enum_id || false == String_Match_Caseless (s, p->name))
			continue;

		return p;
	}
	return NULL;
}

const things_s *Thing_Find_By_Enum_Id (int idwanted)
{
	for (const things_s *p = &things[0]; p->name; p ++) {
		if (p->enum_id == 0 || p->enum_id != idwanted )
			continue;

		return p; // enum_id match
	}
	return NULL;

}


qbool VarPack_Is_Zero (varpack_u *vp, vartype_e property_vt)
{
	//size_t packsize = 0;
	switch (property_vt) {
	case vtstring_1:
		if (vp->char_p && vp->char_p[0] == NULL_CHAR_0)
			return true; // Button_Finalize will make a "" for simplicity.
		return vp->char_p == NULL;

	case mfloat_102: // fall thru SIZEOIC x 3
	case vtfloat_2:			return vp->floatp == 0;

	case mrectf_10:	return
							vp->rectf.left == 0 &&
							vp->rectf.top == 0 &&
							vp->rectf.width == 0 &&
							vp->rectf.height == 0;

	case mrecti_11:		return
							vp->recti.left == 0 &&
							vp->recti.top == 0 &&
							vp->recti.width == 0 &&
							vp->recti.height == 0;


	case minteger_119: // fall thru
	case vtinteger_19:	return vp->intp == 0;

	case vtlist_str_20:	return vp->stringlist.numstrings == 0;
	case vtrgb_21:		return vp->vec3[0] == 0 &&
								vp->vec3[1] == 0 &&
								vp->vec3[2] == 0;

	//case mfloatlist_122: // fall thru
	case mvertflist_22:	return vp->floatlist.count == 0;

	case vtcontrolref_23:
		return vp->voidp == NULL;

	case mint32list_124: // fall thru
	case vtint32list_24:	return vp->int32list.count == 0;

	case vtappearance_200:	return vp->intp == 0;
	case vtfontnameindex_201: return vp->intp == 0;
	case vtfocusstyle_202:	return vp->intp == 0;

	default:
		// ERROR!
		c_assert_msg_ (0,"VarPack is Zero does not know vartype in switch"); // Force break
		break;
	}
	return true;
}



// Returns NULL on zero values or zero varpack (or error)
// Write to file style
RELATED_ (Property_Set_By_String_After_Freeing_Is_Ok UI_Set_f)

// How do we encode quotes?
// I think the answer is that we don't?
CALLERS_ (UI_Fly Property_Dump_Maybe)

// DUMP and FLY
char *VarPack_Maybe_Quote_String_Zalloc(qbool is_contained, varpack_u *vp, vartype_e property_vt, mag_s *magni_opt, mag_s *magni_reverse, qbool shall_force, qbool *p_did_base64)
{
	if (!shall_force) {
		if (VarPack_Is_Zero (vp, property_vt) )
			return NULL;
	}
	char *s_propval_z = VarPack_String_Zalloc (is_contained, vp, property_vt, magni_opt, magni_reverse, shall_force, p_did_base64);

	// DOUBLE QUOTES TO SINGLE QUOTES
	{
		qbool is_dquote = strchr(s_propval_z, CHAR_DQUOTE_34) != NULL;

		if (is_dquote) {
			int count = 0;
			String_Edit_Replace_Char (s_propval_z, CHAR_DQUOTE_34, CHAR_SQUOTE_39, &count);
		}
	}
#if 0
	if (property_vt == vtstring_1 && s_propval_z[0] == 0) {
		int j = 5;
	}
#endif
	qbool must_quote = false;
	if (isin2 (property_vt, mrectf_10, mrecti_11))
		must_quote = true; // These types requires quotes for dump.  What about fly?  QuakeC?

	if (isin4 (property_vt, vtlist_str_20, mvertflist_22, vtint32list_24, mint32list_124))
		if (!p_did_base64 || !(*p_did_base64))
			must_quote = true; // These types requires quotes for dump.  What about fly?  QuakeC?

	// QUOTE IF WHITESPACE
	// Or if it is empty and forced .. force checked above
	if (must_quote == false)
		if (s_propval_z[0] == 0 || String_Has_Whitespace(s_propval_z) || strchr(s_propval_z, ':'))
			must_quote = true;

	if (must_quote) {
		char *s_old = s_propval_z;
		char *s_new = Mem_strdupf(devinfomempool, QUOTED_S, s_propval_z);
		s_propval_z = s_new;
		Mem_FreeNull_ (s_old);
	}
	return s_propval_z;
}

// Jan 23 2025 - This function shall not quote ever.
// Caller is responsible for quoting.
// Likewise the other function shall never dequote.
CALLERS_ (UI_Fly CSQC_Object_Property_Get_Ok Property_Dump_Maybe)
char *VarPack_String_Zalloc(qbool is_contained, varpack_u *vp, vartype_e property_vt, mag_s *magni_opt, mag_s *magni_reverse, qbool shall_force, qbool *p_did_base64)
{
	//#pragma message ("oject properties. Special character encoding?  Handle quotes and commas?")
	// Shall force indicates to write the string even if the property is zero
	if (!shall_force) {
		if (VarPack_Is_Zero (vp, property_vt) )
			return NULL;
	}

	switch (property_vt) {
	case vtstring_1:
		// vtstring forced output shall reply an empty string
		if (!vp->char_p)
			return Mem_strdup(devinfomempool, ""); // FORCED PRINT OF NON-ALLOCATED VALUE

		return Mem_strdup(devinfomempool, vp->char_p);

	case mfloat_102: // fall thru SIZEOIC x 3
		if (magni_opt) {
			return Mem_strdupf(devinfomempool, FLOAT_LOSSLESS_FORMAT, vp->floatp * magni_opt->magnifier);
		}
		if (magni_reverse && magni_reverse->magnifier) {
			float val = vp->floatp / magni_reverse->magnifier; // Magnify_Reverse Fontsize
			return Mem_strdupf(devinfomempool, FLOAT_LOSSLESS_FORMAT, val);
		}

		// Else ... fall thru
	case vtfloat_2:
		return Mem_strdupf(devinfomempool, FLOAT_LOSSLESS_FORMAT, vp->floatp);

	case minteger_119:
		if (magni_opt) {
			return Mem_strdupf(devinfomempool, "%d", vp->intp * magni_opt->magnifier);
		}
		if (magni_reverse && magni_reverse->magnifier) {
			int val = vp->intp / magni_reverse->magnifier; // Magnify_Reverse Integer
			return Mem_strdupf(devinfomempool, "%d", val);
		}
		// Else ... fall thru

	case vtinteger_19:
		return Mem_strdupf(devinfomempool, "%d", vp->intp);

	case mrectf_10:
		RELATED_ (PRVM_UglyValueString) // Using that style

		if (magni_opt) {
			// Little bit of work here.
			frect rtranslatedf = vp->rectf;
			Magnify_Rectf (&rtranslatedf, magni_opt, is_contained);
			return Mem_strdupf(devinfomempool, RECTF_PRINTF, RECT_SEND(rtranslatedf));
		}
		if (magni_reverse && magni_reverse->magnifier) {
			if (is_contained) {
				// No offset translation
				frect rtranslatedf = vp->rectf;
				Magnify_Reverse_Rectf (&rtranslatedf, magni_reverse, is_contained);
				return Mem_strdupf(devinfomempool, RECTF_PRINTF, RECT_SEND(rtranslatedf));
			} else {
				frect rtranslatedf = vp->rectf;
				Magnify_Reverse_Rectf (&rtranslatedf, magni_reverse, is_contained);
				return Mem_strdupf(devinfomempool, RECTF_PRINTF, RECT_SEND(rtranslatedf));
			}
		} // reverse

		return Mem_strdupf(devinfomempool, RECTF_PRINTF, RECT_SEND(vp->rectf));

	case mrecti_11:
		RELATED_ (PRVM_UglyValueString) // Using that style
		if (magni_opt) {
			// Little bit of work here.

			rect_s rtranslatedi = vp->recti;
			Magnify_Recti (&rtranslatedi, magni_opt, is_contained);
			//	vp->recti.left	* magni_opt->magnifier + magni_opt->offset_xx,
			//	vp->recti.top	* magni_opt->magnifier + magni_opt->offset_yy,
			//	vp->recti.width	* magni_opt->magnifier,
			//	vp->recti.height* magni_opt->magnifier
			//};
			return Mem_strdupf(devinfomempool, RECTI_PRINTF, RECT_SEND(rtranslatedi));
		}
		if (magni_reverse && magni_reverse->magnifier) {
			rect_s rtranslatedi = vp->recti;
			Magnify_Reverse_Recti (&rtranslatedi, magni_reverse, is_contained);
			return Mem_strdupf(devinfomempool, RECTI_PRINTF, RECT_SEND(rtranslatedi));
		}
		return Mem_strdupf(devinfomempool, RECTI_PRINTF, RECT_SEND(vp->recti));

	case vtlist_str_20:
		if (!vp->stringlist.numstrings)
			return Mem_strdup(devinfomempool, ""); // FORCED PRINT OF NON-ALLOCATED VALUE

		if (p_did_base64) {
			// Requests encoding base64 for things that would parse poorly.
			char *sbase64list_za = stringlist_to_base64_comma_delim_char12_encode_zalloc(&vp->stringlist, q_force_false);
			if (sbase64list_za) {
				*p_did_base64 = true;

#if 0 // Baker: Jan 28 2025 - Tests indicate it reproduces it correctly.
				stringlist_t list2 = {0};
				stringlist_append_from_base64_comma_delim_char12_decode (&list2, sbase64list_za);
				int idxmismatch = stringlist_compare (&vp->stringlist, &list2);
				if (idxmismatch != not_found_neg1) {
					int j = 5;
				}
				stringlistfreecontents (&list2);
#endif
				return sbase64list_za; // It has quotes and commas!
			}
		}
		return stringlist_join_delim_zalloc (&vp->stringlist, ",", fs_quoted_false); // Formerly: fs_quoted_auto_2

	case vtrgb_21:
		// Ok we have something like RGB(255, 255, 255)
#if 0 // Baker: We no longer output as "RGB(r,g,b)" by default.
		return Mem_strdupf(devinfomempool, 
			DQUOTE "RGB(%d,%d,%d)" DQUOTE,
			(byte)(vp->vec3[0] * 255),
			(byte)(vp->vec3[1] * 255),
			(byte)(vp->vec3[2] * 255)
			);
#else
		return String_HTMLColor_To_String_Za (vp->vec3, /*compress*/ true);

#endif

	case mvertflist_22:
		if (!vp->floatlist.count)
			return Mem_strdup(devinfomempool, ""); // FORCED PRINT OF NON-ALLOCATED VALUE
		#pragma message ("This is something to fix floats verts I assume 2D what is example?")
		// Baker: Polygon points not ready at this time.
		if (magni_opt) {
			// DO THIS
		}
		return floats_join_delim12_delim_zalloc (&vp->floatlist, POLYGON_2D_2,
			POLYGON_2D_FLOAT_SEPARATOR_SPACE,
			POLYGON_2D_ELEMENT_SEPARATOR_COMMA,
			//fs_quoted_true // QUOTE THE BASTARD
			fs_quoted_false // QUOTE THE BASTARD
		);


	case mint32list_124: // fall thru
		if (!vp->int32list.count)
			return Mem_strdup(devinfomempool, ""); // FORCED PRINT OF NON-ALLOCATED VALUE
		if (magni_opt) {
			// Column Widths
			return int32s_join_delim_mag_zalloc (magni_opt->magnifier,
				&vp->int32list, COLUMN_SEPARATOR_COMMA,
				fs_quoted_false);
		}
		if (magni_reverse && magni_reverse->magnifier) {
			return int32s_join_delim_mag_reverse_zalloc (magni_reverse->magnifier,
				&vp->int32list, COLUMN_SEPARATOR_COMMA,
				fs_quoted_false);

		}

		// Else ... fall thru

	case vtint32list_24:
		if (!vp->int32list.count)
			return Mem_strdup(devinfomempool, ""); // FORCED PRINT OF NON-ALLOCATED VALUE
		return int32s_join_delim_zalloc (&vp->int32list, COLUMN_SEPARATOR_COMMA, fs_quoted_false);

	case vtcontrolref_23:

		if (vp->voidp) {
			oject_s *k = (oject_s *)vp->voidp;
			if (k->cm.name_a)
				return Mem_strdup(devinfomempool, k->cm.name_a);
			else
				return Mem_strdupf(devinfomempool, "Unnamed_%s", k->po->name); // not supposed to happen

		}
		return Mem_strdup(devinfomempool, "NoControlReference"); // Should not much happen because string request is supposed to be when a value is present

	case vtappearance_200:
		if (vp->intp == appearance_flat_1)
			return Mem_strdup(devinfomempool, "Flat"); // flat_1
		return Mem_strdupf(devinfomempool, "%d", vp->intp);

	case vtfontnameindex_201:
		// user0 to user7 - See CSQC.h
		if (vp->intp == fontnameindex_system_neg1)		return Mem_strdup(devinfomempool, "System");
		if (vp->intp == fontnameindex_default_0)		return Mem_strdup(devinfomempool, "Default_0");
		if (vp->intp == fontnameindex_console_1)		return Mem_strdup(devinfomempool, "Console");
		if (vp->intp == fontnameindex_sbar_2)			return Mem_strdup(devinfomempool, "Sbar");
		if (vp->intp == fontnameindex_notify_3)			return Mem_strdup(devinfomempool, "Notify");
		if (vp->intp == fontnameindex_chat_4)			return Mem_strdup(devinfomempool, "Chat");
		if (vp->intp == fontnameindex_centerprint_5)	return Mem_strdup(devinfomempool, "Centerprint");
		if (vp->intp == fontnameindex_infobar_6)		return Mem_strdup(devinfomempool, "Infobar");
		if (vp->intp == fontnameindex_menu_7)			return Mem_strdup(devinfomempool, "Menu");
		if (vp->intp == fontnameindex_user0_8)			return Mem_strdup(devinfomempool, "User0");
		if (vp->intp == fontnameindex_user1_9)			return Mem_strdup(devinfomempool, "User1");
		if (vp->intp == fontnameindex_user2_10)			return Mem_strdup(devinfomempool, "User2");
		if (vp->intp == fontnameindex_user3_11)			return Mem_strdup(devinfomempool, "User3");
		if (vp->intp == fontnameindex_user4_12)			return Mem_strdup(devinfomempool, "User4");
		if (vp->intp == fontnameindex_user5_13)			return Mem_strdup(devinfomempool, "User5");
		if (vp->intp == fontnameindex_user6_14)			return Mem_strdup(devinfomempool, "User6");
		if (vp->intp == fontnameindex_user7_15)			return Mem_strdup(devinfomempool, "User7");
		return Mem_strdupf(devinfomempool, "%d", vp->intp);

	case vtfocusstyle_202:
		if (vp->intp == focusstyle_nohighlight_1)
			return Mem_strdup(devinfomempool, "NoHighLight");
		return Mem_strdupf(devinfomempool, "%d", vp->intp);

	default:
		// ERROR!
		c_assert_msg_ (0, "Vartype not in switch"); // Force break

		break;
	}

	return NULL;
}


qbool Pointer_Is_Inside (const void *p, const void *p_large, size_t sizeof_p_large)
{
	int64_t p_large_0 = (int64_t)p_large;
	int64_t p_large_1 = (int64_t)p_large + sizeof_p_large;
	int64_t p_x = (int64_t)p;
	return in_range(p_large_0, p_x, p_large_1);
}

int Rect_Clip_Area_Result (const rect_s *pr_rect, const rect_s *pr_visible, rect_s *pr_clipped_out)
{
	int pr_visible_rightof	= RECT_RIGHTOF(*pr_visible);
	int pr_visible_bottomof	= RECT_BOTTOMOF(*pr_visible);
	int pr_rect_rightof		= RECT_RIGHTOF(*pr_rect);
	int pr_rect_bottomof	= RECT_BOTTOMOF(*pr_rect);

	if (pr_visible_rightof  <= pr_rect->left)		return CULLED_1; // RECT is entirely right of visible
	if (pr_visible_bottomof <= pr_rect->top)		return CULLED_1; // RECT is entirely below visible area
	if (pr_rect_rightof     <= pr_visible->left)	return CULLED_1; // RECT is entirely left of visible
	if (pr_rect_bottomof	<= pr_visible->top)		return CULLED_1; // RECT is entirely above visible

	// int original_x = *dst_x, original_y = *dst_y, original_w = *dst_w, original_h = *dst_h;
	int x1 = CLAMP(pr_visible->left, pr_rect->left,        pr_visible_rightof - 1);
	int y1 = CLAMP(pr_visible->top,  pr_rect->top,         pr_visible_bottomof - 1);
	int x2 = CLAMP(pr_visible->left, pr_rect_rightof - 1,  pr_visible_rightof - 1);
	int y2 = CLAMP(pr_visible->top,  pr_rect_bottomof - 1, pr_visible_bottomof - 1);


	RECT_SET_FROM_XYXY (*pr_clipped_out, x1, y1, x2, y2);

	// Note: This recalculation is a waste if didn't clip.
	qbool is_clipped = false == RECT_DO_MATCH(*pr_rect, *pr_clipped_out);

	// ZIDE has some sort of clip reduction area calc, I don't think we need.

	if (is_clipped) {
		return CLIPPED_2; // CLIPPED
	}

	return UNCULLED_0; // Not culled.
}

int ViewCalc_RowCollide (viewcalc_s *vc, int x, int y)
{
	rect_s r_cursor = vc->r_row_first;	// updated in refresh
	int beyond_idx	= vc->first_vis_row + vc->rowcount;

	for (int n = vc->first_vis_row; n < beyond_idx; n ++, r_cursor.top += vc->rowheight) {
		if (RECT_HIT (r_cursor, x, y)) return n;
	} // for
	return not_found_neg1;
}

// k->tb.bottomof_last
viewcalc_s *ViewCalc_Set (viewcalc_s *vc, const rect_s *pr_interior, int scrolldown, int listcount)
{
	int rowheight_saved = vc->rowheight;

	memset (vc, 0, sizeof(*vc));

	vc->rowheight				= rowheight_saved; //rowheight_in; //ceil(fontsize);

	if (listcount == 0)
		return vc;

	vc->num_visiblef			= pr_interior->height / (float)vc->rowheight;

	//#pragma message ("What if first and last row are cutoff?")
	vc->num_visible_partials	= ceil(vc->num_visiblef);

	// Validate all of these
	vc->is_first_row_cutoff		= (scrolldown modulo vc->rowheight) != 0;
	vc->first_vis_row			= scrolldown / vc->rowheight;
	vc->first_full_row			= vc->first_vis_row + vc->is_first_row_cutoff;

	// Debatable because not clamped to list.
	vc->y_beyond				= scrolldown + pr_interior->height;

	// Debatable ... if the adjusted last_partial_row becomes "culled", this is wrong
	vc->is_cutoff_bottom		= (vc->y_beyond modulo vc->rowheight) != 0; // 59 + 1 = 60 mod 20 = 0

	// These get adjusted in a moment ..
	vc->last_full_row			= (vc->y_beyond / vc->rowheight) - 1; // minus 1 because zero based
	vc->last_partial_row		= vc->last_full_row + vc->is_cutoff_bottom;

	if (vc->last_full_row >= listcount - 1)		vc->last_full_row = listcount - 1; // CLAMP for integral
	if (vc->last_partial_row >= listcount - 1)	vc->last_partial_row = listcount - 1; // Just in case this can happen Nov 26 2018

	vc->rowcount = range_length(vc->first_vis_row, vc->last_partial_row);
	vc->pageamount = (int)vc->num_visiblef - 1;
	vclamp_lo (vc->pageamount, 1);

	// The max scroll allowed requires 1 item showing.
	if (listcount <= (int)vc->num_visiblef )
		vc->viewportmax = 0; // no scroll is allowed because we see it all
	else {
		//(int)vc->num_visiblef
		// 200 items
		// 20.5 rows visible
		// 200 - 20 = 180
		//            181 199
		vc->viewportmax = (listcount - (int)vc->num_visiblef ) * vc->rowheight;
	}

	//#pragma message ("We could factor in possibility of xscroll here for listview");

	vc->show_scrollbar = listcount > (int)vc->num_visiblef;

	return vc;
}

RELATED_ (O_ListView_Draw)

int colwidthx(oject_s *k, int col)
{
	int is_entry = k->columnwidths_a.count > col;
	int w = is_entry ? k->columnwidths_a.ints[col] : LV_DEFAULT_WIDTH_100;
	vclamp_lo (w, LV_MIN_WIDTH_25);
	return w;
}

int columnleftscreenx(oject_s* k, int colwanted)
{
	rect_s r_cursor = k->xl.r_header;
	r_cursor.left -= k->r_scrollport.left; // Scroll

	for (int col = 0; col < colwanted; col ++, r_cursor.left += r_cursor.width) {
		r_cursor.width	= colwidthx (k,col);
	}
	return r_cursor.left;
}

ccs *colheaderx(oject_s *k, int col)
{
	char *s_header = k->columnheaders_a.numstrings > col ? k->columnheaders_a.strings[col] : "";
	return s_header;
}

int colwidths(oject_s *k, int *pis_phantom, int *pphantom_width)
{
	int tot = 0;
	for (int col = 0; col < k->columncount; col ++) {
		int w = colwidthx(k,col);
		tot += w;
	} //
	int is_phantom = tot < k->r_interior.width;
	(*pphantom_width) = 0;
	if (is_phantom) {
		int phantom_amount =  k->r_interior.width - tot;
		tot += phantom_amount;
		(*pphantom_width) = phantom_amount;
	}
	(*pis_phantom) = is_phantom;
	return tot;
}

int colwidths_no_phantom(oject_s *k)
{
	int tot = 0;
	for (int col = 0; col < k->columncount; col ++) {
		int w = colwidthx(k,col);
		tot += w;
	} //
	return tot;
}

int rowcol_2idx(oject_s *k, int row, int col)
{
	int idx = row * k->columncount + col;
	return idx;
}

void swapi (int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}


void TextBox_Edit_Insert_Char_At (oject_s *k, int ch_insert, int sel_left)
{
	int slen = TXT_STRLEN(k);
//	int slen_after_cursor = slen - sel_left;
	char *zbef = Z_StrDup_Len_Z (k->text_a, sel_left);
	char *zaft = Z_StrDup_Len_Z (&k->text_a[sel_left], slen - sel_left);

	Z_StrDupf_Realloc (&k->text_a, "%s%c%s",
		zbef,
		ch_insert,
		zaft
	);
	Z_FreeNull_ (zbef);
	Z_FreeNull_ (zaft);
}

int TextBox_Edit_Insert_String_At (oject_s *k, const char *s_insert, int sel_left)
{
	int slen = TXT_STRLEN(k);
	char *zbef = Z_StrDup_Len_Z (k->text_a, sel_left);
	char *zaft = Z_StrDup_Len_Z (&k->text_a[sel_left], slen - sel_left);

	int newcursor = sel_left + strlen(s_insert);
	Z_StrDupf_Realloc (&k->text_a, "%s%s%s",
		zbef,
		s_insert,
		zaft
	);

	Z_FreeNull_ (zbef);
	Z_FreeNull_ (zaft);
	return newcursor;
}

void TextBox_Edit_Delete_At_NumChars (oject_s *k, int sel_left, int sel_count)
{
	int slen = TXT_STRLEN(k);
	int sel_right = sel_left + sel_count;
	char *zbef = Z_StrDup_Len_Z (k->text_a, LENGTH___ sel_left);
	char *zaft = Z_StrDup_Len_Z (&k->text_a[sel_right], slen-sel_right);

	Z_StrDupf_Realloc (&k->text_a, "%s%s",
		zbef,
		zaft
	);

	Z_FreeNull_ (zbef);
	Z_FreeNull_ (zaft);
}

//int KS_Query_FindPos_Word_Prev_At (const ksubstr *ks, int sel_start)
int String_Find_Previous_Word_At (const char *s, int selstart)
{
	if (selstart == 0)
		return 0;

	int ch = s[selstart];

	// Bob Mary Paul
	int pos_space = 0;
	if (!isdigit(ch) || !isalpha(ch)) {
		pos_space = selstart;
		goto find_alpha;
	}

	// Find space
	pos_space = 0;
	for (int p = selstart - 1; p >= 0; p --) {
		int chx = s[p];
		if (chx != SPACE_CHAR_32)
			continue;

		pos_space = p;
		break;
	} // for

find_alpha:
	if (!pos_space)
		return 0;

	// Find alpha
	int pos_alpha = 0;
	for (int p = pos_space - 1; p >= 0; p --) {
		int chx = s[p];
		if (!isdigit(chx) && !isalpha(chx))
			continue;

		pos_alpha = p;
		break;
	} // for

	// Find space
	pos_space = 0;
	for (int p = pos_alpha - 1; p >= 0; p --) {
		int chx = s[p];
		if (chx != SPACE_CHAR_32)
			continue;

		pos_space = p;
		break;
	} // for

	if (!pos_space)
		return 0;

	return pos_space + ONE_CHAR_1; // WHY?  Because we don't want the space, we want the alpha
}

// In alpha -> find space, find alpha
// In space -> find alpha
// Bob Mary Todd
int String_Find_Next_Word_At (const char *s, int selstart)
{
	int slen = strlen(s);
	if (selstart == slen)
		return slen;

	int ch = s[selstart];
//	int newpos = not_found_neg1;

	// Bob Mary Paul
	int pos_space = slen;
	if (!isdigit(ch) && !isalpha(ch)) {
		// If not in alpha
		pos_space = selstart;
		goto find_alpha;
	}

	// Find space
	pos_space = slen;
	for (int p = selstart + 1; p < slen; p ++) {
		int chx = s[p];
		if (chx != SPACE_CHAR_32)
			continue;

		pos_space = p;
		break;
	} // for

find_alpha:
	if (pos_space == slen)
		return slen;

	// Find alpha
	int pos_alpha = slen;
	for (int p = pos_space + 1; p < slen; p ++) {
		int chx = s[p];
		if (!isdigit(chx) && !isalpha(chx))
			continue;

		pos_alpha = p;
		break;
	} // for

	if (pos_alpha == slen)
		return slen;

	return pos_alpha;
}


float stringlist_maxwidthf(stringlist_t *plist, dp_font_t *dpf, float fontsizef)
{
	float stringmaxwidthf = 0;
	for (int n = 0; n < plist->numstrings; n ++) {
		const char *s = plist->strings[n];

		float sw = Draw_StringWidthInt (dpf, s, fontsizef);

		if (sw > stringmaxwidthf) stringmaxwidthf = sw;
	} // for
	return stringmaxwidthf;
}
float stringlist_maxwidthint(stringlist_t *plist, dp_font_t *dpf, float fontsizef)
{
	return ceil(stringlist_maxwidthf(plist, dpf, fontsizef));
}

int Rect_Hit_Boxes (rect_s *pr, int x, int y, int step_y, int count)
{
	rect_s	r_box	= *pr;

	for (int n = 0; n < count; n ++, r_box.top += step_y) {
		if (RECT_HIT(r_box, x, y))
			return n;
	} // for
	return not_found_neg1;
}

int Rect_Hit_Boxes_No_Miss (rect_s *pr, int x, int y, int step_y, int count)
{
	rect_s	r_box	= *pr;
	r_box.height = step_y;

	for (int n = 0; n < count; n ++, r_box.top += step_y) {
		if (RECT_HIT(r_box, x, y))
			return n;
	} // for
	return not_found_neg1;
}

void Rect_Directional_Bound_Mins (rect_s *pr, int x, int y, int w, int h)
{
	// This acts weird so it's wrong ...
	// It needs to limit via directional!


	//if (pr->left < x) {
	//	int delta_x = x - pr->left;
	//	pr->left += delta_x;
	//	pr->width -= delta_x;
	//}

	//if (pr->top < y) {
	//	int delta_y = y - pr->top;
	//	pr->top += delta_y;
	//	pr->height -= delta_y;
	//}

	if (pr->width < w) {
		pr->width = w;
	}

	if (pr->height < h) {
		pr->height = h;
	}

}


void Rect_Directional_Adjust (rect_s *pr, alignment_e directional, int ax, int ay)
{
	if ((int)directional == CM_MIDDLE_DOT_4) {
		// MOVE!
		pr->left += ax;
		pr->top += ay;
		return;
	}

	switch (directional modulo 3) { // 0 1 2
	case 0: /*left*/	pr->left += ax; pr->width -= ax; break;		// 0, 3, 6
	case 1: /*middle*/	/*none!*/	break;				// 1  4  7
	case 2: /*right*/	pr->width += ax; break;						// 2  5  8
	}

	switch (directional / 3) {
	case 0: /*north*/	pr->top += ay; pr->height -= ay; break;		// 0 1 2
	case 1: /*equator*/	/*none*/ break;				// 3 4 5
	case 2: /*south*/	pr->height += ay; break;					// 6 7 8
	}
}



void XY_Set_Dot (xy_pairi_s *pxy, rect_s *psrc, alignment_e directional)
{
	switch (directional % 3) {
	case 0: /*left*/	pxy->x = psrc->left; break;
	case 1: /*middle*/	pxy->x = psrc->left + psrc->width/2.0; break;
	case 2: /*right*/	pxy->x = RECT_RIGHT(*psrc); break;
	}

	switch (directional / 3) {
	case 0: /*north*/	pxy->y = psrc->top; break;
	case 1: /*equator*/	pxy->y = psrc->top + psrc->height/2.0; break;
	case 2: /*south*/	pxy->y = RECT_BOTTOM(*psrc); break;
	}
}

void Magnify_Rectf (frect *prf, mag_s *mag, qbool is_contained)
{
	prf->left	*= mag->magnifier;
	prf->top	*= mag->magnifier;
	prf->width  *= mag->magnifier;
	prf->height *= mag->magnifier;
	if (!is_contained) {
		prf->left += mag->offset_xx;
		prf->top  += mag->offset_yy;
	}
}

void Magnify_Recti (rect_s *prf, mag_s *mag, qbool is_contained)
{
	prf->left	*= mag->magnifier;
	prf->top	*= mag->magnifier;
	prf->width  *= mag->magnifier;
	prf->height *= mag->magnifier;
	if (!is_contained) {
		prf->left += mag->offset_xx;
		prf->top  += mag->offset_yy;
	}
}

void Magnify_Reverse_Rectf (frect *prf, mag_s *mag, qbool is_contained)
{
	if (!is_contained) {
		prf->left -= mag->offset_xx;
		prf->top  -= mag->offset_yy;
	}
	prf->left	/= mag->magnifier;
	prf->top	/= mag->magnifier;
	prf->width  /= mag->magnifier;
	prf->height /= mag->magnifier;
}

void Magnify_Reverse_Recti (rect_s *prf, mag_s *mag, qbool is_contained)
{
	if (!is_contained) {
		prf->left -= mag->offset_xx;
		prf->top  -= mag->offset_yy;
	}
	prf->left	/= mag->magnifier;
	prf->top	/= mag->magnifier;
	prf->width  /= mag->magnifier;
	prf->height /= mag->magnifier;
}

