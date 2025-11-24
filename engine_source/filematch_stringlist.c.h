// filematch_stringlist.c.h


void stringlistappend_face_vertex_unique (stringlist_t *plist, const brushrow_s *face)
{
	va_super (svectora, 128, VECTOR3_G, VECTOR3_SEND(face->a));
	//qbool wasaddeda =
	stringlistappend_unique_didAdd (plist, svectora);

	va_super (svectorb, 128, VECTOR3_G, VECTOR3_SEND(face->b));
	//qbool wasaddedb =
	stringlistappend_unique_didAdd (plist, svectorb);

	va_super (svectorc, 128, VECTOR3_G, VECTOR3_SEND(face->c));
	//qbool wasaddedc =
	stringlistappend_unique_didAdd (plist, svectorc);
}


qbool _arch5_string_space_split_isok (ccs *split_this,
	char *dst0, size_t dst0_sizeof, char *dst1, size_t dst1_sizeof,
	char *dst2, size_t dst2_sizeof, char *dst3, size_t dst3_sizeof,
	char *dst4, size_t dst4_sizeof)
{
	qbool bisok = false;
	stringlist_t list = {0};
	stringlistappend_split (&list, split_this, SPACER);
	if (list.numstrings == 5) {
		strlcpy (dst0, list.strings[0], dst0_sizeof);
		strlcpy (dst1, list.strings[1], dst1_sizeof);
		strlcpy (dst2, list.strings[2], dst2_sizeof);
		strlcpy (dst3, list.strings[3], dst3_sizeof);
		strlcpy (dst4, list.strings[4], dst4_sizeof);
		bisok = true;
	}
	stringlistfreecontents (&list);
	return bisok;
}

qbool _arch2_string_space_split_isok (ccs *split_this, char *dst0, size_t dst0_sizeof, char *dst1, size_t dst1_sizeof)
{
	qbool bisok = false;
	stringlist_t list = {0};
	stringlistappend_split (&list, split_this, SPACER);
	if (list.numstrings >= 2) {
		strlcpy (dst0, list.strings[0], dst0_sizeof);
		strlcpy (dst1, list.strings[1], dst1_sizeof);
		bisok = true;
	}
	stringlistfreecontents (&list);
	return bisok;
}

char *stringlist_join_delim_zalloc (stringlist_t *plist, ccs *s_delimiter, int shall_quote)
{
	baker_string_t *bs_malloc = stringlist_join_bkalloc (plist, s_delimiter);
	char *s_za = NULL;
	switch (shall_quote) {
	case fs_quoted_auto_2:
		if (String_Is_All_AlphaNumeric_Underscore(bs_malloc->string)) {
			// No need to quote
			s_za = Z_StrDup_Len_Z (bs_malloc->string, bs_malloc->length);
			break;
		}
		// Isn't alpha -- fall through for quoted

	case fs_quoted_true:
		s_za = Z_StrDupf (QUOTED_S, bs_malloc->string);
		break;

	default:
		// Shall not quote
		s_za = Z_StrDup_Len_Z (bs_malloc->string, bs_malloc->length);
	} // sw

	// bs_malloc can't be null here, baker_string inits with an empty string alloc
	if (bs_malloc) BakerString_Destroy_And_Null_It (&bs_malloc);
	return s_za;
}

// Returns not_found_neg1 if not found
static int _stringlist_find_index (stringlist_t *p_stringlist, ccs *s_find_this, int caseinsensitive)
{
	typedef int (*comparemethod_fn_t) (const char *s1, const char *s2);
	comparemethod_fn_t compare_fn = caseinsensitive ? strcasecmp : strcmp;
	for (int idx = 0; idx < p_stringlist->numstrings; idx ++) {
		char *sxy = p_stringlist->strings[idx];
		if (compare_fn (sxy, s_find_this) == 0)
			return idx;
	} // for
	return not_found_neg1;
}

qbool stringlist_string_endswithany (ccs *s, stringlist_t *plist)
{
	for (int idx = 0; idx < plist->numstrings; idx ++) {
		char *this_suffix = plist->strings[idx];
		if (String_Ends_With (s, this_suffix))
			return true;
	} // for
	return false;
}

// CSQC Key/Values for forums uses this.
int stringlist_find_index_columns (stringlist_t *p_stringlist, ccs *s_find_this, int numcolumns)
{
	for (int idx = 0; idx < p_stringlist->numstrings; idx += numcolumns) {
		char *sxy = p_stringlist->strings[idx];
		//if (compare_fn (sxy, s_find_this) == 0)
		if (String_Match (sxy, s_find_this) )
			return idx;
	} // for
	return not_found_neg1;
}

int stringlist_find_column_number (stringlist_t *p_stringlist, ccs *s_find_this, int numcolumns)
{
	for (int idx = 0; idx < numcolumns && idx < p_stringlist->numstrings; idx ++) {
		char *sxy = p_stringlist->strings[idx];
		//if (compare_fn (sxy, s_find_this) == 0)
		if (String_Match (sxy, s_find_this) )
			return idx;
	} // for
	return not_found_neg1;
}

// Ignores first row that is column headers.
// Returns 1 or greater or -1 if not found
int stringlist_find_row_number (stringlist_t *p_stringlist, ccs *s_find_this, int numcolumns)
{
	for (int idx = numcolumns; idx < p_stringlist->numstrings; idx += numcolumns) {
		char *sxy = p_stringlist->strings[idx];
		//if (compare_fn (sxy, s_find_this) == 0)
		if (String_Match (sxy, s_find_this) )
			return (idx / numcolumns);
	} // for
	return not_found_neg1;
}

int stringlist_find_index_caseless (stringlist_t *p_stringlist, ccs *s_find_this)
{
	return _stringlist_find_index (p_stringlist, s_find_this, fs_caseless_true);
}

int stringlist_find_index (stringlist_t *p_stringlist, ccs *s_find_this)
{
	return _stringlist_find_index (p_stringlist, s_find_this, fs_caseless_false);
}

// Baker: 2024 April 20 - No callers, we were going to use for intermap, then did something else
void stringlist_from_delim (stringlist_t *p_stringlist, ccs *s_space_delimited)
{
	// This process depends on this s_space_delimited having items.
	if (s_space_delimited[0] == NULL_CHAR_0)
		return;

	const char	*s_space_delim		= " ";
	int			s_len			= (int)strlen(s_space_delimited);
	int			s_delim_len		= (int)strlen(s_space_delim);

	// Baker: This works the searchpos against s_space_delimited
	// finding the delimiter (space) and adding a list item until there are no more spaces
	// (an iteration with no space adds the rest of the string.

	// Baker: have we tested this against a single item without a space to see what happens?
	// It looks like it can handle that.

	// BUILD LIST

	int			searchpos		= 0;
	while (1) {
		char s_this_copy[MAX_INPUTLINE_16384];
		const char	*space_pos	= strstr (&s_space_delimited[searchpos], s_space_delim); // string_find_pos_start_at(s, s_delim, searchpos);
		int			endpos		= (space_pos == NULL) ? (s_len - 1) : ( (space_pos - s_space_delimited) - 1); // (commapos == not_found_neg1) ? (s_len -1) : (commapos -1);
		int			this_w		= (endpos - searchpos + 1); // string_range_width (searchpos, endpos); (endpos - startpos + 1)

		memcpy (s_this_copy, &s_space_delimited[searchpos], this_w);
		s_this_copy[this_w] = NULL_CHAR_0; // term

		stringlistappend (p_stringlist, s_this_copy);

		// If no space found, we added the rest of the string as an item, so get out!
		if (space_pos == NULL)
			break;

		searchpos = (space_pos - s_space_delimited) + s_delim_len;
	} // while
}

// Supply null or zero length string for optionals
// Returns num matches
int stringlistappend_from_dir_pattern (stringlist_t *p_stringlist, ccs *s_optional_dir_no_slash, ccs *s_optional_dot_extension, int wants_strip_extension)
{
	fssearch_t	*t;
	char		s_pattern[1024];
	int			num_matches = 0;
	int			j;
	int			is_ext = s_optional_dot_extension && s_optional_dot_extension[0];

	const char	*s_ext = is_ext ? s_optional_dot_extension : "";

	if (s_optional_dir_no_slash) {
			// "%s/*%s"
			c_strlcpy (s_pattern, s_optional_dir_no_slash);
			c_strlcat (s_pattern, "/");
			c_strlcat (s_pattern, "*");
			c_strlcat (s_pattern, s_ext);
	}
	else	c_dpsnprintf1 (s_pattern, "*%s", s_ext);

	t = FS_Search(s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	if (t && t->numfilenames > 0) {
		for (j = 0; j < t->numfilenames; j ++) {
			char *sxy = t->filenames[j];
			if (wants_strip_extension)
				File_URL_Edit_Remove_Extension (sxy);
			stringlistappend (p_stringlist, sxy);
			num_matches ++;
		} // for
	} // if

	if (t) FS_FreeSearch(t);

	return num_matches;
}

int stringlistappend_dir_matches (stringlist_t *p_stringlist, ccs *s_prefix)
{
	fssearch_t	*t;
	int			num_matches = 0;
	va_super (s_pattern, MAX_QPATH_128, "%s*", s_prefix);

	t = FS_Search(s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	if (t && t->numfilenames > 0) {
		for (int j = 0; j < t->numfilenames; j ++) {
			char *sxy = t->filenames[j];
			int type = FS_FileOrDirectoryType (sxy);
			if (type != FS_FILETYPE_DIRECTORY_2)
				continue;
			stringlistappendf (p_stringlist, "%s/", sxy);
			num_matches ++;
		} // for
	} // if

	if (t) FS_FreeSearch(t);

	return num_matches;
}

int stringlistappend_from_pattern (stringlist_t *p_stringlist, ccs *s_pattern)
{
	int num_matches = 0;
	fssearch_t *t = FS_Search(s_pattern, fs_caseless_true,
		fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	if (!t)
		return 0;

	for (int j = 0; j < t->numfilenames; j ++) {
		char *sxy = t->filenames[j];
		stringlistappend (p_stringlist, sxy);
		num_matches ++;
	} // for

	if (t) FS_FreeSearch(t);

	return num_matches;
}

// a little strings system
void stringlistinit(stringlist_t *list)
{
	memset(list, 0, sizeof(*list));
}

void stringlistfreecontents(stringlist_t *list)
{
	int i;
	for (i = 0;i < list->numstrings;i++)
	{
		if (list->strings[i])
			Z_Free(list->strings[i]);
		list->strings[i] = NULL;
	}
	list->numstrings = 0;
	list->maxstrings = 0;
	if (list->strings)
		Z_Free(list->strings);
	list->strings = NULL;
}

//void stringlistprint (stringlist_t *list, const char *title_optional, void (*myprintf)(const char *, ...) )
void stringlistprint_with_title (stringlist_t *list, void (*myprint_line_f)(ccs *, ...), ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	
	myprint_line_f ("%s = %d", /*title =*/ text, list->numstrings);

	for (int idx = 0; idx < list->numstrings; idx ++) {
		ccs *s = list->strings[idx];
		myprint_line_f ("%4d: %s", idx, s);
	} // for idx

	VA_EXPAND_ALLOC_FREE (text);
}

// Baker: Does not increase numstrings
const char **stringlist_nullterm_add (stringlist_t *list)
{
	char **oldstrings;

	if (list->numstrings >= list->maxstrings) {
		oldstrings = list->strings;
		list->maxstrings += 4096;
		list->strings = (char **) Z_Malloc(list->maxstrings * sizeof(*list->strings));
		if (list->numstrings)
			memcpy(list->strings, oldstrings, list->numstrings * sizeof(*list->strings));
		if (oldstrings)
			Z_Free(oldstrings);
	}

	list->strings[list->numstrings] = NULL; //Z_StrDup(text);
	//list->numstrings++;
	return (const char **)list->strings;
}


int stringlistappendfilelines_did_load (stringlist_t *plist, ccs *file, qbool is_base1, int is_number_column)
{
	fs_offset_t filesize = 0;
	char *sa = (char *)FS_LoadFile(file, tempmempool, fs_quiet_true, &filesize);
	// FS_LoadFile Always appends a 0 byte.
	if (!sa)
		return false;

	//String_Edit_Replace_Char (sa, CARRIAGE_RETURN_CHAR_13, NEWLINE_CHAR_10, /*countreply*/ NULL);
	char *String_Edit_Whitespace_To_Space_Except_Newline (char *s_edit);
	String_Edit_Whitespace_To_Space_Except_Newline (sa);

	if (is_number_column) {
		stringlist_t listugly = {0};
		stringlistappend_split (&listugly, sa, NEWLINE);
		for (int n = 0; n < listugly.numstrings; n ++) {
			if (is_base1)
				stringlistappendf (plist, "%d", n + 1);
			else
				stringlistappendf (plist, "%d", n);
			stringlistappend (plist, listugly.strings[n]);
		} // for
		stringlistfreecontents (&listugly);
	}
	else {
		stringlistappend_split (plist, sa, NEWLINE);
	}

	Mem_FreeNull_ (sa);
	return true;
}

int stringlistappend_split_delimiter_linenumbers (stringlist_t *plist, ccs *s, ccs *delimiter)
{
	//String_Edit_Whitespace_To_Space_Except_Newline (sa);
	stringlist_t listugly = {0};
	stringlistappend_split (&listugly, s, delimiter);
	for (int n = 0; n < listugly.numstrings; n ++) {
		stringlistappendf (plist, "%d", n);
		stringlistappend (plist, listugly.strings[n]);
	} // for
	stringlistfreecontents (&listugly);

	return true;
}


void stringlistappend (stringlist_t *list, const char *text)
{
	//size_t textlen;
	char **oldstrings;

	if (list->numstrings >= list->maxstrings) {
		oldstrings = list->strings;
		list->maxstrings += 4096;
		list->strings = (char **) Z_Malloc(list->maxstrings * sizeof(*list->strings));
		if (list->numstrings)
			memcpy(list->strings, oldstrings, list->numstrings * sizeof(*list->strings));
		if (oldstrings)
			Z_Free(oldstrings);
	}

	list->strings[list->numstrings] = Z_StrDup(text);
	list->numstrings++;
}

void stringlistappendf (stringlist_t *list, const char *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	stringlistappend (list, text);
	VA_EXPAND_ALLOC_FREE (text);
}

qbool stringlistappend_unique_didAdd (stringlist_t *plist, ccs *text)
{
	qbool shall_add = true;
	for (int idx = 0; idx < plist->numstrings; idx ++) {
		char *sxy = plist->strings[idx];
		if (String_Match (sxy, text)) {
			shall_add = false;
			break; // DUP
		}
	} // for

	if (shall_add)
		stringlistappend (plist, text);
	return shall_add;
}

qbool stringlistappend_unique_evens_didAdd (stringlist_t *plist, ccs *text)
{
	qbool shall_add = true;
	for (int idx = 0; idx < plist->numstrings; idx += 2) {
		char *sxy = plist->strings[idx];
		if (String_Match (sxy, text)) {
			shall_add = false;
			break; // DUP
		}
	} // for

	if (shall_add)
		stringlistappend (plist, text);
	return shall_add;
}


qbool stringlistappendf_unique_didAdd (stringlist_t *plist, ccs *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);

	qbool didadd = stringlistappend_unique_didAdd(plist, text);

	VA_EXPAND_ALLOC_FREE (text);
	return didadd;
}

void stringlistappendf2 (stringlist_t *list, ccs *s, const char *fmt, ...)
{
	stringlistappend (list, s);
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	stringlistappend (list, text);
	VA_EXPAND_ALLOC_FREE (text);
}

void stringlistappend2 (stringlist_t *list, ccs *s, ccs *s2)
{
	stringlistappend (list, s);
	stringlistappend (list, s2);
}

void stringlistappend3 (stringlist_t *list, ccs *s, ccs *s2, ccs *s3)
{
	stringlistappend (list, s);
	stringlistappend (list, s2);
	stringlistappend (list, s3);
}

void stringlistappend4 (stringlist_t *list, ccs *s, ccs *s2, ccs *s3, ccs *s4)
{
	stringlistappend (list, s);
	stringlistappend (list, s2);
	stringlistappend (list, s3);
	stringlistappend (list, s4);
}

void stringlistappend5 (stringlist_t *list, ccs *s, ccs *s2, ccs *s3, ccs *s4, ccs *s5)
{
	stringlistappend (list, s);
	stringlistappend (list, s2);
	stringlistappend (list, s3);
	stringlistappend (list, s4);
	stringlistappend (list, s5);
}

void stringlistappend_len (stringlist_t *list, const char *text, int text_len)
{
	char **oldstrings;

	if (list->numstrings >= list->maxstrings) {
		oldstrings = list->strings;
		list->maxstrings += 4096;
		list->strings = (char **) Z_Malloc(list->maxstrings * sizeof(*list->strings));
		if (list->numstrings)
			memcpy(list->strings, oldstrings, list->numstrings * sizeof(*list->strings));
		if (oldstrings)
			Z_Free(oldstrings);
	}

	list->strings[list->numstrings] = (char *)Z_MemDup_Z (text, text_len);
	list->numstrings++;
}

RELATED_ (VM_tokenize_console) // VM_tokenize_console does have way to identify column


void stringlistappend_tokenize_qcquotes (stringlist_t *plist, int32list_s *plinelist, const char *text)
{
	const char *p = text;
	int num_tokens = 0;
	while (1) {
		// skip whitespace here to find token start pos
		while(*p && ISWHITESPACE(*p))
			p ++;

		const char *startpos = p;//tokens_startpos[num_tokens] = p - tokenize_string;
//		if (!COM_ParseToken_Console(&p))
//			break;

		if (!COM_ParseToken_QuakeC(&p, /*newline?*/ false))
			break;

		//tokens_endpos[num_tokens] = p - tokenize_string;

		//if (!COM_ParseToken_Console(&p))
		//	break;

		// Baker: Let's keep quotes so we know isn't a name or something

//		if (plinelist) {
			//jan182025 int ipos = (int)(startpos - text);
			//jan182025 int linenum = String_Find_Count (text, "\n", ipos);

		if (*startpos == '\"') {
			va_super  (tmp, sizeof(com_token) + 2, QUOTED_S, com_token); // +2 quotes
			stringlistappend (plist, tmp);

			if (plinelist) {
				//int ipos = (int)(startpos - text);
				//int linenum = String_Find_Count (text, "\n", ipos);
				int32s_add1 (plinelist, (int)(startpos - text));
			}
		}
		else {
			stringlistappend (plist, com_token);
			if (plinelist) {
				//int ipos = (int)(startpos - text);
				//int linenum = String_Find_Count (text, "\n", ipos);
				int32s_add1 (plinelist, (int)(startpos - text));
			}

		}
		num_tokens ++;
	} // while

}

void stringlistappend_split_len (stringlist_t *plist, const char *text_inx, int text_strlen, const char *s_delimiter)
{
	// Because strstr won't honor the length, we must copy to buffer
	char *s_z = Z_StrDup_Len_Z (text_inx, text_strlen);


	int delimiter_slen = strlen(s_delimiter);
	const char *sbeyond = s_z + text_strlen;
	const char *cursor = s_z;
	const char *p = NULL;
	while ( p = strstr(cursor, s_delimiter) ) {
		int seglen = p - cursor; // p is beyond
//#ifdef _DEBUG
//		char *zz = Z_StrDup_Len_Z (cursor,seglen);
//		if (String_Contains(zz, "klook")) {
//			int j = 5;
//		}
//
//
//#endif

		stringlistappend_len (plist, cursor, seglen);
		cursor = p + delimiter_slen;//ONE_CHAR_1;
		//Z_FreeNull_(zz);

	}
	// Baker: Feb 18 2025 - This was ignore 0 length final units
	// Change to if any unit, there is one left to write?
	// No ... always one left to write.
#if 0
	if (cursor != sbeyond) {
#endif
	if (1) {
		// Hit the trail
		int seglen = sbeyond - cursor;
//#ifdef _DEBUG
//		char *zz = Z_StrDup_Len_Z (sbeyond,seglen);
//		if (String_Contains(zz, "klook")) {
//			int j = 5;
//		}
//
//
//#endif
		stringlistappend_len (plist, cursor, seglen);
		//Z_FreeNull_(zz);
	}
	Mem_FreeNull_ (s_z);
}

void stringlistappend_split (stringlist_t *plist, ccs *text_in, ccs *s_delimiter)
{
	stringlistappend_split_len (plist, text_in, strlen(text_in), s_delimiter);
}

void stringlistappend_textlines_len_cr_scrub (stringlist_t *plist, const char *text_in, int text_strlen)
{
	// Must make a copy to remove the carriage returns.
	char *text_alloc = Z_StrDup_Len_Z(text_in, text_strlen);

	// Baker: elim carriage returns
	String_Edit_Replace (text_alloc, text_strlen + ONE_CHAR_1, CARRIAGE_RETURN, "");

	int refreshed_strlen = strlen(text_alloc); // It may have changed after replace.

	char *sbeyond = text_alloc + refreshed_strlen;
	char *cursor = text_alloc;
	char *p;
	while ( p = strstr(cursor, NEWLINE) ) {
		// Baker: Feb 18 2025 - What is reason for this?
		// Answer this is grabbing substring from master ... this is correct ... move along
		int seglen = p - cursor; // p is beyond
		stringlistappend_len (plist, cursor, seglen);
		cursor = p + ONE_CHAR_1;
	}
	if (cursor != sbeyond) {
		// Hit the trail
		int seglen = sbeyond - cursor;
		stringlistappend_len (plist, cursor, seglen);
	}

	Z_FreeNull_ (text_alloc);
}

void stringlistappend_textlines_cr_scrub (stringlist_t *plist, const char *text_in)
{
	int text_len = strlen (text_in);
	stringlistappend_textlines_len_cr_scrub (plist,text_in,text_len);
}

int stringlistappend_csv_nl_tab (stringlist_t *plist, const char *s)
{
	qbool isbad = false;
	stringlist_t lines = {0};
	int num_columns = 0;
	stringlistappend_textlines_cr_scrub (&lines, s);

	for (int row = 0; row < lines.numstrings; row ++) {
		char *sline = lines.strings[row];
		// If begins with // or empty line ... ignore it.
		if (String_Starts_With (sline, "//")) {
			Con_PrintLinef ("Ignored commented line at row0 %d", row);
			continue;
		}
		if (String_Is_All_Whitespace_0_32 (sline)) {
			Con_PrintLinef ("Ignored all whitespace line at row0 %d", row);
			continue;
		}
		stringlist_t listcols = {0};
		stringlistappend_split (&listcols, sline, TABBER);
		int cols_here = listcols.numstrings;
		if (num_columns) {
			if (num_columns != cols_here) {
				Con_PrintLinef (CON_RED "Bad: " CON_WHITE "Column count at at row0 %d is %d but expected %d", row, cols_here, num_columns);
				isbad = true;
				goto noparse;
			}
		}

		num_columns = cols_here;

		for (int col = 0; col < listcols.numstrings; col ++) {
			char *sxy = listcols.strings[col];
			stringlistappend (plist, sxy);
		}

noparse:
		stringlistfreecontents (&listcols);
		if (isbad)
			break;
	}

	stringlistfreecontents (&lines);
	if (isbad)
		return false;
	return num_columns;
}



void stringlistappend_blob (stringlist_t *list, const byte *blob, size_t blobsize)
{
	char **oldstrings;

	// Baker: This reallocs in batches of 4096
	if (list->numstrings >= list->maxstrings) {
		oldstrings = list->strings;
		list->maxstrings += 4096;
		list->strings = (char **) Z_Malloc(list->maxstrings * sizeof(*list->strings));
		if (list->numstrings)
			memcpy (list->strings, oldstrings, list->numstrings * sizeof(*list->strings));
		if (oldstrings)
			Z_Free(oldstrings);
	}
	//textlen = strlen(text) + 1;
	list->strings[list->numstrings] = (char *) Z_Malloc(blobsize);
	memcpy (list->strings[list->numstrings], blob, blobsize);
	list->numstrings++;
}

void stringlistappendlist (stringlist_t *plist, const stringlist_t *add_these)
{
	for (int idx = 0; idx < add_these->numstrings; idx++) {
		char *sxy = add_these->strings[idx];
		stringlistappend (plist, sxy);
	}

}

void stringlistappendlistunique (stringlist_t *plist, const stringlist_t *add_these)
{
	for (int sidx = 0; sidx < add_these->numstrings; sidx++) {
		char *sxy = add_these->strings[sidx];
		stringlistappend_unique_didAdd (plist, sxy);
	} // each stringlist item
}

void stringlistappendfssearch (stringlist_t *plist, fssearch_t *t)
{
	if (!t)
		return;

	for (int idx = 0; idx < t->numfilenames; idx++) {
		char *sxy = t->filenames[idx];
		stringlistappend (plist, sxy);
	}

}

// If provided 2 ... do_it = (index mod 2) == 0
// This allows 2 = do half of them.
// This allows 3 = do 1/3 of them
// For fast JPEG
void stringlistappendfssearch_modulo (stringlist_t *plist, fssearch_t *t, int modulation)
{
	if (!modulation) 
		return;
	if (!t)
		return;

	for (int idx = 0; idx < t->numfilenames; idx++) {
		char *sxy = t->filenames[idx];
		if ((idx % modulation) != 0)
			continue;
		stringlistappend (plist, sxy);
	}

}


void stringlistappend_search_pattern (stringlist_t *plist, const char *s_pattern)
{
	fssearch_t	*t = FS_Search (s_pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);

	if (!t) return;

	// Baker: Entries should be the entire file name like
	// "sound/ambience/thunder.wav" or whatever

	stringlistappendfssearch (plist, t);

	if (t) FS_FreeSearch(t);
}

void stringlist_replace_at_index (stringlist_t *list, int idx, const char *text)
{
	if (in_range_beyond (0, idx, list->numstrings) == false) {
		Con_PrintLinef (CON_ERROR "string list at %d is out of bounds of 0 to %d", idx, list->numstrings);
		return;
	}
	Z_Free (list->strings[idx]);

	list->strings[idx] = Z_StrDup(text);
}

static int stringlistsort_cmp(const void *a, const void *b)
{
	return strcasecmp(*(const char **)a, *(const char **)b);
}

static int g_sstart = 0;
static int g_slength = 0;
static int stringlistsort_start_length_cmp(const void *_a, const void *_b)
{
	const char *a = *(const char **)_a;
	const char *b = *(const char **)_b;

	char *sa = Z_StrDup (a);
	char *sb = Z_StrDup (b);
	File_URL_Edit_Remove_Extension (sa);
	File_URL_Edit_Remove_Extension (sb);

	int result = 0;
#if 1
	// Safety checks
	int slena = strlen (sa);
	int slenb = strlen (sb);

	if (slena < g_sstart + g_slength) {
		result = strcasecmp(sa, sb); // This is not right, but repeatably consistent in the sort order
		goto failout; // Too short
	}

	if (slenb < g_sstart + g_slength) {
		result = strcasecmp(sa, sb); // This is not right, but repeatably consistent in the sort order
		goto failout; // Too short
	}
#endif

	memmove (sa, &sa[g_sstart], g_slength + 1); // +1 to copy the null too
	memmove (sb, &sb[g_sstart], g_slength + 1); // +1 to copy the null too

	result = strcasecmp(sa, sb);

failout:

	Mem_FreeNull_ (sa);
	Mem_FreeNull_ (sb);

	return result;
}

void stringlistsort_custom(stringlist_t *list, qbool uniq, int myfunc(const void *, const void *) );


void stringlistsort(stringlist_t *list, qbool uniq)
{
	stringlistsort_custom (list, uniq, stringlistsort_cmp);
}

void stringlistsort_substring (stringlist_t *list, qbool uniq, int startpos, int slength)
{
	g_sstart = startpos;
	g_slength = slength;

	stringlistsort_custom (list, uniq, stringlistsort_start_length_cmp);
}

void stringlist_condump_with_linenums (stringlist_t *plist)
{
	for (int idx = 0; idx < plist->numstrings; idx++) {
		char *sxy = plist->strings[idx];

		Con_PrintLinef ("%4d: " QUOTED_S, idx, sxy);
	} // for
}

// Assumed that plist is unique and that ilist is index list of those
RELATED_ (int32s_countstrings)
void stringlist_condump_ilist (stringlist_t *plist, int32list_s *ilist)
{
	for (int psidx = 0; psidx < plist->numstrings; psidx ++) {
		ccs *sxy =  plist->strings[psidx];
		int count_of_this_string = ilist->ints[psidx];
		Con_PrintLinef ("%4d: %5d " QUOTED_S, psidx, count_of_this_string, sxy);
	} // for
}

ccs *stringlist_find_most_common_index (stringlist_t *plist, ccs *ignoring_this)
{
	stringlist_t list_unique = {0};
	stringlistappendlistunique (&list_unique, plist);
	stringlistsort_unique (&list_unique);

	int32list_s list_inums = {0};
	int32s_countstrings (&list_inums, plist, &list_unique);

	qbool done = false;
	int imax = -9999999;
	ccs *most_common = NULL;
	for (int sidx = 0; sidx < list_inums.count; sidx ++) {
		int myint = list_inums.ints[sidx];
		ccs *sxy =  list_unique.strings[sidx];
		if (String_Match (sxy, ignoring_this))
			continue;

		if (done == false || myint > imax ) {
			imax = myint;
			done = true;
			most_common = sxy;
		}
	} // for sidx

	ccs *ret = NULL;
	if (done) {

		// Find one of them in main list
		for (int psidx = 0; psidx < plist->numstrings; psidx ++) {
			ccs *sxy =  plist->strings[psidx];
			if (String_Match (sxy, most_common)) {
				ret = sxy;
				break;
			}
		}
	}
	stringlistfreecontents (&list_unique);
	int32s_freecontents (&list_inums);

	return ret;
}

void stringlist_condump_autocount_uniques (stringlist_t *plist)
{
	stringlist_t list_unique = {0};

	stringlistappendlistunique (&list_unique, plist);
	stringlistsort_unique (&list_unique);

	int32list_s list_inums = {0};
	int32s_countstrings (&list_inums, plist, &list_unique);

	stringlist_condump_ilist (&list_unique, &list_inums);

	stringlistfreecontents (&list_unique);
	int32s_freecontents (&list_inums);

}


void stringlist_condump_with_linenums_band (stringlist_t *plist, int numcolumns)
{
	for (int idx = 0; idx < plist->numstrings; idx++) {
		char *sxy = plist->strings[idx];

		if (idx % numcolumns == 0)
			Con_PrintLinef (CON_BRONZE "%4d: " QUOTED_S, idx, sxy);
		else
			Con_PrintLinef ("%4d: " QUOTED_S, idx, sxy);
	} // for
}

void stringlist_condump_raw (stringlist_t *plist)
{
	for (int idx = 0; idx < plist->numstrings; idx++) {
		char *sxy = plist->strings[idx];

		Con_PrintLinef ("%s", sxy);
	} // for
}

// Find all character 1 and character 2 ==> quote and comma
// For crappily joining a string list comma delimited
// Which will then comma join and turn into base 64.
int stringlist_quote_or_comma_encode_count (stringlist_t *plist)
{
	int count = 0;
	for (int idx = 0; idx < plist->numstrings; idx++) {
		char *sxy = plist->strings[idx];
		int scount = 0;
		String_Edit_Replace_Char (sxy, CHAR_DQUOTE_34, CHAR_1, &scount);
		String_Edit_Replace_Char (sxy, ',', CHAR_2, &scount);
		if (scount)
			count ++;
	} // for

	return count;
}

int stringlist_quote_or_comma_decode_count (stringlist_t *plist)
{
	int count = 0;
	for (int idx = 0; idx < plist->numstrings; idx++) {
		char *sxy = plist->strings[idx];
		int scount = 0;
		String_Edit_Replace_Char (sxy, CHAR_1, CHAR_DQUOTE_34, &scount);
		String_Edit_Replace_Char (sxy, CHAR_2, ',', &scount);
		if (scount)
			count ++;
	} // for

	return count;
}

RELATED_ (stringlist_append_from_base64_comma_delim_char12_decode)
CALLERS_ (VarPack_String_Zalloc)
char *stringlist_to_base64_comma_delim_char12_encode_zalloc (stringlist_t *plist, qbool shall_force)
{
	// Step 1:
	int num_modified_count = stringlist_quote_or_comma_encode_count(plist); // Baker: FUCKS WITH LIST!
	if (num_modified_count == 0 && shall_force == false)
		return NULL; // Not necessary
	baker_string_t *bsa = NULL;
	bsa = stringlist_join_bkalloc (plist, ",");
	// Ok now base64
	size_t base64len = 0;
	char *s_base64_calloc = base64_encode_calloc ((unsigned char *)bsa->string, bsa->length, &base64len); // malloc
	char *sza = Z_StrDup (s_base64_calloc); // Make it Zallocated.
	freenull_ (s_base64_calloc);
	BakerString_Destroy_And_Null_It (&bsa);

	// Baker: This is unfucking the list.  We fucked with the actual list in step #1.
	num_modified_count = stringlist_quote_or_comma_decode_count(plist); // Baker: This unfucks the LIST.
	return sza;
}



void stringlist_bytedump (stringlist_t *plist, const byte *src, size_t src_sizeof)
{
	for (size_t n = 0; n < src_sizeof; n ++) {
		byte sxy = src[n];
		stringlistappendf (plist, "%4d: ch = %4d %c", (int)n, (int)sxy, (int)sxy);
	} // n
}

void stringlist_stringdump (stringlist_t *plist, ccs *src)
{
	size_t slen = strlen(src);
	stringlist_bytedump (plist, (const byte *)src, slen);
}

int stringlist_compare (stringlist_t *plist1, stringlist_t *plist2)
{
	if (plist1->numstrings != plist2->numstrings)
		return 0;

	for (int idx = 0; idx < plist1->numstrings; idx++) {
		char *sxy1 = plist1->strings[idx];
		char *sxy2 = plist2->strings[idx];
		if (String_Match (sxy1, sxy2) == false)
			return idx;
	}
	return not_found_neg1;
}

// Baker: The "12" is that we horrifically encode commas and quotes for Form strings
// This allows Form strings with DQUOTES (34) and COMMAS to save.
int stringlist_append_from_base64_comma_delim_char12_decode (stringlist_t *plist, ccs *sbase64)
{
	size_t decodesize = 0;
	char *s_alloc = (char *)base64_decode_calloc (sbase64, &decodesize);

	stringlistappend_split (plist, s_alloc, ",");

	int num_modified = stringlist_quote_or_comma_decode_count(plist);

	freenull_ (s_alloc);
	return num_modified;
}

qbool stringlist_is_quote_or_comma (stringlist_t *plist)
{
	for (int idx = 0; idx < plist->numstrings; idx++) {
		char *sxy = plist->strings[idx];
		if (strchr(sxy, CHAR_DQUOTE_34))
			return true;
		if (strchr(sxy, ','))
			return true;
		//Con_PrintLinef ("%s", sxy);
	} // for
	return false;
}


void stringlistsort_custom(stringlist_t *list, qbool uniq, int myfunc(const void *, const void *) )
{
	if (list->numstrings < 1) // Baker: Did I add this or was it already in DarkPlaces? A: DarkPlaces had it.
		return;

	qsort(&list->strings[0], list->numstrings, sizeof(list->strings[0]), myfunc);

	// If Make Unique ...
	if (uniq) {
		// i: the item to read
		// j: the item last written
	int i, j;
		for (i = 1, j = 0; i < list->numstrings; ++i)
		{
			char *save;
			if (String_Match_Caseless(list->strings[i], list->strings[j]))
				continue;
			++j;
			save = list->strings[j];
			list->strings[j] = list->strings[i];
			list->strings[i] = save;
		}
		for(i = j + 1; i < list->numstrings; i ++) {
			if (list->strings[i])
				Z_Free(list->strings[i]);
		}
		list->numstrings = j+1;
	}
}


void stringlistsort_n_columns(stringlist_t *list, int numcolumns, int column, qbool uniq)
{
	// Ok .. We want to be able to sort multi column sort
	// This is pita.  Use index to sort.
	// We do this somewhere ...
	// Load games menu is sortable by column
	RELATED_ (saveload_sort)
	// It has several string lists.
	// It sorts g_saves_indexes_4byte
	// We can do better, right?
	// Do this when you are actually going to use it.
}


// operating system specific code
static void adddirentry(stringlist_t *list, const char *path, const char *name)
{
	if (String_NOT_Match(name, ".") && String_NOT_Match(name, "..")) {
		char temp[MAX_OSPATH];
		dpsnprintf( temp, sizeof( temp ), "%s%s", path, name );
		stringlistappend(list, temp);
	}
}

#ifdef _WIN32
// Baker: This concats the results
// Baker: basepath is something like "c:\galaxy// or zircon/
//   and path is the Quake local folder.
// Outdated mostly --- May 1 2025 -- see stringlistappend_search_pattern instead
// This has advantage of being able to search outside gamedir when looking for gamedirs
// but otherwise this function sucks.
void stringlist_append_listdirectory(stringlist_t *list, ccs *basepath, ccs *filepattern)
{
	#define BUFSIZE 4096
	char pattern[BUFSIZE] = {0};
	wchar patternw[BUFSIZE] = {0};
	char filename[BUFSIZE] = {0};
	wchar *filenamew;
	int lenw = 0;
	WIN32_FIND_DATAW n_file;
	HANDLE hFile;
	c_strlcpy (pattern, basepath);
	c_strlcat (pattern, filepattern);
	c_strlcat (pattern, "*");
	fromwtf8(pattern, (int)strlen(pattern), patternw, BUFSIZE);
	// ask for the directory listing handle
	hFile = FindFirstFileW(patternw, &n_file);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	do {
		filenamew = n_file.cFileName;
		lenw = 0;
		while(filenamew[lenw] != 0) ++lenw;
		towtf8(filenamew, lenw, filename, BUFSIZE);
		adddirentry (list, filepattern, filename);
	} while (FindNextFileW(hFile, &n_file) != 0);
	FindClose(hFile);
	#undef BUFSIZE
}
#else
void stringlist_append_listdirectory(stringlist_t *list, ccs *basepath, ccs *filepattern)
{
	char fullpath[MAX_OSPATH];
	DIR *dir;
	struct dirent *ent;
	dpsnprintf(fullpath, sizeof(fullpath), "%s%s", basepath, filepattern);
#ifdef __ANDROID__
	// SDL currently does not support listing assets, so we have to emulate
	// it. We're using relative paths for assets, so that will do.
	if (basepath[0] != '/')
	{
		char listpath[MAX_OSPATH];
		qfile_t *listfile;
		dpsnprintf(listpath, sizeof(listpath), "%sls.txt", fullpath);
		char *buf = (char *) FS_SysLoadFile(listpath, tempmempool, true, NULL);
		if (!buf)
			return;
		char *p = buf;
		for (;;)
		{
			char *q = strchr(p, '\n');
			if (q == NULL)
				break;
			*q = 0;
			adddirentry(list, path, p);
			p = q + 1;
		}
		Mem_Free(buf);
		return;
	}
#endif
	dir = opendir(fullpath);
	if (!dir)
		return;

	while ((ent = readdir(dir)))
		adddirentry (list, filepattern, ent->d_name);
	closedir(dir);
}
#endif

// Baker: Usage:
// stringlist_t matchedSet;
// stringlistinit	(&matchedSet); // this does not allocate, memset 0
//
// stringlist_from_delim (&matchedSet, mystring);
//// SORT plus unique-ify
//stringlistsort (&matchedSet, fs_make_unique_true);
//stringlistfreecontents( &matchedSet );

// Baker: Takes a stringlist and turns it in a single string, spaces between at moment
// Example ?  No callers at moment.
// This was going to be used, then another idea came up
// But code is not quite finished (SAY WHY BASTARD!)

// Baker: This looks like "join" and it does work.
void stringlist_join_buf (stringlist_t *p_stringlist, ccs *s_delimiter, char *buf, size_t buflen)
{
	buf[0] = 0;
	strlcpy (buf, "", buflen);
	for (int idx = 0; idx < p_stringlist->numstrings; idx ++) {
		char *sxy = p_stringlist->strings[idx];
		if (idx > 0)
			strlcat (buf, " ", buflen);
		strlcat (buf, sxy, buflen);
	} // for
}

	//if (k_save) {
	//	// Baker: This could happen if somehow error occurs that exits function
	//	BakerString_Destroy_And_Null_It (&k_save); // nulls k_save
	//}

baker_string_t *stringlist_join_bkalloc (stringlist_t *plist, ccs *s_delimiter)
{
	baker_string_t *bs_malloc = BakerString_Create_Malloc ("");
	int slen_delim = strlen(s_delimiter);
	for (int idx = 0; idx < plist->numstrings; idx ++) {
		char *s = plist->strings[idx];
		if (idx > 0)
			BakerString_Cat_No_Collide (bs_malloc, slen_delim, s_delimiter);
		BakerString_Cat_No_Collide (bs_malloc, strlen(s), s);
	} // for
	return bs_malloc;
}

RELATED_ (baker_string_t)
baker_string_t *stringlist_join_lines_bkalloc (stringlist_t *plist)
{
	baker_string_t *bs_malloc = stringlist_join_bkalloc (plist, NEWLINE);
	return bs_malloc;
}

// 2 = auto
char *stringlist_join_lines_zalloc (stringlist_t *plist)
{
	baker_string_t *bs_malloc = stringlist_join_bkalloc (plist, NEWLINE);
	char *s_za = NULL;
	//switch (shall_quote) {
	//case fs_quoted_auto_2:
	//	if (String_Is_All_AlphaNumeric_Underscore(bs_malloc->string)) {
	//		s_za = Z_StrDup_Len_Z (bs_malloc->string, bs_malloc->length);
	//		break;
	//	}
	//	// Isn't alpha -- fall through for quoted

	//case fs_quoted_true:
	//	s_za = Z_StrDupf (QUOTED_S, bs_malloc->string);
	//	break;

	//default:
		s_za = Z_StrDup_Len_Z (bs_malloc->string, bs_malloc->length);
//	} // sw

	// bs_malloc can't be null here, baker_string inits with an empty string alloc
	if (bs_malloc) BakerString_Destroy_And_Null_It (&bs_malloc);
	return s_za;
}

char *stringlist_join_spaced_zalloc (stringlist_t *plist)
{
	baker_string_t *bs_malloc = stringlist_join_bkalloc (plist, " ");
	char *s_za = Z_StrDup_Len_Z (bs_malloc->string, bs_malloc->length);

	// bs_malloc can't be null here, baker_string inits with an empty string alloc
	if (bs_malloc) BakerString_Destroy_And_Null_It (&bs_malloc);
	return s_za;
}

void stringlist_append_entity_textures (stringlist_t *plist, entityx_t *r_ent)
{
	for (int bx = 0; bx < r_ent->brushlist.count; bx ++) {
		brush_s *a = &r_ent->brushlist.brush[bx];

		for (int brow = 0; brow < a->brushrowlist.count; brow ++) {
			brushrow_s *pbr = &a->brushrowlist.brushrow[brow];
			stringlistappend (plist, pbr->pbrtexture);
		} // faces
	} // brush
}
