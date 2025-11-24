// filematch_intlist.c.h


// Sets the ilist.count to specified number (adds items until hit, values not set but new entries will be zero)
void int32s_count_set (int32list_s *ilist, int newcount_in)
{
	int newcount = newcount_in + 1;//ilist->count + 3 + 1; // + 1 null terminated last entry
	if (newcount >= ilist->maxsize) {
		while (ilist->maxsize <= newcount) {
			ilist->maxsize += 4096;
		}
		ilist->ints = (int *)Mem_Realloc (zonemempool, ilist->ints, sizeof(int) * ilist->maxsize);
	}
	if (newcount_in > ilist->count)
		ilist->count = newcount_in;
}

void int32s_freecontents (int32list_s *ilist)
{
	// Unlike stringlist_t, no strings to free.
	ilist->count = 0;
	ilist->maxsize = 0;
	Z_FreeNull_ (ilist->ints);
}

qbool int32s_add1_unique_didadd (int32list_s *ilist, int p)
{
	for (int idx = 0; idx < ilist->count; idx ++) {
		int d = ilist->ints[idx];
		if (d == p)
			return false; // Duplicate
	} // for idx

	int32s_add1 (ilist, p);
	return true;
}


RELATED_ (stringlistsort)
static int int32s_sort_cmp(const void *a, const void *b)
{
	const int *pinta = (const int *)a;
	const int *pintb = (const int *)b;
	int ia = *pinta;
	int ib = *pintb;
	return ia - ib; // Baker: Otherwise it is b-a
}


void int32s_sort_custom (int32list_s *list, qbool uniq, int myfunc(const void *, const void *) )
{
	if (list->count < 1) // Baker: Did I add this or was it already in DarkPlaces? A: DarkPlaces had it.
		return;

	qsort(&list->ints[0], list->count, sizeof(list->ints[0]), myfunc);

	// If Make Unique ...
	if (uniq) {
		// i: the item to read
		// j: the item last written
	int i, j;
		for (i = 1, j = 0; i < list->count; i ++) {
			//char *save;
			//if (String_Match_Caseless(list->strings[i], list->strings[j]))
			//	continue;
			//j ++;
			//save = list->strings[j];
			//list->strings[j] = list->strings[i];
			//list->strings[i] = save;
			int saved_int;
			//if (String_Match_Caseless(list->strings[i], list->strings[j]))
			//	continue;
			if (list->ints[i] == list->ints[j])
				continue;
			j ++;
			saved_int = list->ints[j];
			list->ints[j] = list->ints[i];
			list->ints[i] = saved_int;
		}
		// Baker: This is a page array.  No need to free.
		//for(i = j + 1; i < list->count; i ++) {
		//	if (list->strings[i])
		//		Z_Free(list->strings[i]);
		//}
		list->count = j+1;
	}
}

void int32s_sort (int32list_s *list, qbool uniq)
{
	int32s_sort_custom (list, uniq, int32s_sort_cmp);
}

// Returns not_found_neg1 if not found
int int32s_find_index (int32list_s *ilist, int find_int)
{
	for (int idx = 0; idx < ilist->count; idx ++) {
		int d = ilist->ints[idx];
		if (d == find_int)
			return idx;
	} // for
	return not_found_neg1;
}

void int32s_add1 (int32list_s *ilist, int p)
{
	int newcount = ilist->count + 1 + 1; // + 1 null terminated last entry
	if (newcount >= ilist->maxsize) {
		ilist->maxsize += 4096;
		ilist->ints = (int *)Mem_Realloc (zonemempool, ilist->ints, sizeof(int) * ilist->maxsize);
	}

	ilist->ints[ilist->count++] = p;
}

void int32s_add2 (int32list_s *ilist, int px, int py)
{
	int newcount = ilist->count + 2 + 1; // + 1 null terminated last entry
	if (newcount >= ilist->maxsize) {
		ilist->maxsize += 4096;
		ilist->ints = (int *)Mem_Realloc (zonemempool, ilist->ints, sizeof(int) * ilist->maxsize);
	}

	ilist->ints[ilist->count++] = px;
	ilist->ints[ilist->count++] = py;
}

void int32s_add3 (int32list_s *ilist, int px, int py, int pz)
{
#ifdef _DEBUG
	if (!in_range_beyond (1, px, 10000) || !in_range_beyond (1, py, 10000), !in_range_beyond (1, pz, 10000)) {
		int j = 5;
	}
#endif

	int newcount = ilist->count + 3 + 1; // + 1 null terminated last entry
	if (newcount >= ilist->maxsize) {
		ilist->maxsize += 4096;
		ilist->ints = (int *)Mem_Realloc (zonemempool, ilist->ints, sizeof(int) * ilist->maxsize);
	}

	ilist->ints[ilist->count++] = px;
	ilist->ints[ilist->count++] = py;
	ilist->ints[ilist->count++] = pz;
}

void int32s_command_scale (int32list_s *ilist, int iscale)
{
	for (int idx = 0; idx < ilist->count; idx ++) {
		int d = ilist->ints[idx];
		ilist->ints[idx] = d * iscale;
	} // for idx
}

void int32s_print (int32list_s *ilist, void (*myprintf)(const char *, ...))
{
	for (int idx = 0; idx < ilist->count; idx ++) {
		int d = ilist->ints[idx];
		myprintf ("%4d: %d", idx, d);
	} // for idx
}

void int32s_dump (int32list_s *ilist)
{
	int32s_print (ilist, Con_PrintLinef);
}



int int32s_append_split_dequote (int32list_s *ilist, ccs *s_value, ccs *s_delimiter_elements)
{
	int num_elements_ignored = 0;

	stringlist_t rlist = {0};
	// Split the element pairs using comma split
	{
		char *s_z = Z_StrDup(s_value);
		String_Edit_DeQuote(s_z);
		stringlistappend_split (&rlist, s_z, s_delimiter_elements /*COMMA*/ );
		Z_FreeNull_ (s_z); // DISCHARGED
	}

	for (int n = 0; n < rlist.numstrings; n ++) {
		// SPLIT ON DELIMITER
		char *selem = rlist.strings[n];
		int d = atoi(selem);
		int32s_add1 (ilist, d);
	} // while

	stringlistfreecontents(&rlist);

	return num_elements_ignored;
}

void floats_freecontents (floatlist_s *flist)
{
	// Unlike stringlist_t, no strings to free.
	flist->count = 0;
	flist->maxsize = 0;
	Z_FreeNull_ (flist->floats);
}

void voids_freecontents (voidlist_s *vlist)
{
	// Unlike stringlist_t, no strings to free.
	vlist->count = 0;
	vlist->maxsize = 0;
	Z_FreeNull_ (vlist->vloats);
}

RELATED_ (stringlistappend)


void voids_add1 (voidlist_s *vlist, const void *p)
{
	int newcount = vlist->count + 1 + 1; // + 1 null terminated last entry
	if (newcount >= vlist->maxsize) {
		vlist->maxsize += 4096;
		vlist->vloats = (const void **)Mem_Realloc (zonemempool, vlist->vloats, sizeof(ccs *) * vlist->maxsize);
	}

	vlist->vloats[vlist->count++] = (const void *)p;
}

void floats_add1 (floatlist_s *flist, float p)
{
	int newcount = flist->count + 1 + 1; // + 1 null terminated last entry
	if (newcount >= flist->maxsize) {
		flist->maxsize += 4096;
		flist->floats = (float *)Mem_Realloc (zonemempool, flist->floats, sizeof(float) * flist->maxsize);
	}

	flist->floats[flist->count++] = p;
}

void floats_add2 (floatlist_s *flist, float px, float py)
{
	int newcount = flist->count + 2 + 1; // + 1 null terminated last entry
	if (newcount >= flist->maxsize) {
		flist->maxsize += 4096;
		flist->floats = (float *)Mem_Realloc (zonemempool, flist->floats, sizeof(float) * flist->maxsize);
	}

	flist->floats[flist->count++] = px;
	flist->floats[flist->count++] = py;
}

void floats_add3 (floatlist_s *flist, float px, float py, float pz)
{
	int newcount = flist->count + 3 + 1; // + 1 null terminated last entry
	if (newcount >= flist->maxsize) {
		flist->maxsize += 4096;
		flist->floats = (float *)Mem_Realloc (zonemempool, flist->floats, sizeof(float) * flist->maxsize);
	}

	flist->floats[flist->count++] = px;
	flist->floats[flist->count++] = py;
	flist->floats[flist->count++] = pz;
}


void floats_print (floatlist_s *flist, void (*myprintf)(const char *, ...))
{
	for (int idx = 0; idx < flist->count; idx ++) {
		float f = flist->floats[idx];
		myprintf ("%4d: " FLOAT_LOSSLESS_FORMAT, idx, f);
	} // for idx
}

RELATED_ (stringlistprint)

void floats_dump (floatlist_s *flist)
{
	floats_print (flist, Con_PrintLinef);
}

char *int32s_join_delim_zalloc (int32list_s *ilist, ccs *s_delimiter_elements, int shall_quote)
{
	stringlist_t list = {0};
	for (int n = 0; n < ilist->count; n ++) {
		int d = ilist->ints[n];
		stringlistappendf (&list, "%d", d);
	} // for

	// Now join the spaced floats with commas
	char *s_comma_join_z = stringlist_join_delim_zalloc (&list, s_delimiter_elements, shall_quote);
	stringlistfreecontents (&list); // done with this, thank you

	return s_comma_join_z;
}


char *int32s_join_delim_mag_zalloc (int magni_factor, int32list_s *ilist, ccs *s_delimiter_elements, int shall_quote)
{
	stringlist_t list = {0};
	for (int n = 0; n < ilist->count; n ++) {
		int d = ilist->ints[n];
		stringlistappendf (&list, "%d", d * magni_factor);
	} // for

	// Now join the spaced floats with commas
	char *s_comma_join_z = stringlist_join_delim_zalloc (&list, s_delimiter_elements, shall_quote);
	stringlistfreecontents (&list); // done with this, thank you

	return s_comma_join_z;
}

char *int32s_join_delim_mag_reverse_zalloc (int magni_factor, int32list_s *ilist, ccs *s_delimiter_elements, int shall_quote)
{
	stringlist_t list = {0};
	for (int n = 0; n < ilist->count; n ++) {
		int d = ilist->ints[n];
		stringlistappendf (&list, "%d", d / magni_factor);
	} // for

	// Now join the spaced floats with commas
	char *s_comma_join_z = stringlist_join_delim_zalloc (&list, s_delimiter_elements, shall_quote);
	stringlistfreecontents (&list); // done with this, thank you

	return s_comma_join_z;
}

char *floats_join_delim12_delim_zalloc (floatlist_s *flist, int number_per_vert, ccs *s_delimiter12, ccs *s_delimiter_elements, int shall_quote)
{
	if (number_per_vert == 0)
		return NULL;

	if (false == in_range(0, number_per_vert, 12)) // Too many or negative
		return NULL; // limit is 12

	if ( (flist->count modulo number_per_vert) != 0)
		return NULL;

	int num_groups = flist->count / number_per_vert;
	stringlist_t list = {0};
	for (int n = 0; n < flist->count; n ++) {
		float f = flist->floats[n];
		stringlistappendf (&list, FLOAT_LOSSLESS_FORMAT, f);
	} // for

	int idx = 0;
	// In groups of x, create a string
	stringlist_t group_of_spaced_floats = {0}; //
	for (int grp = 0; grp < num_groups; grp ++, idx += number_per_vert) {
		// Assume fairly reasonable size
		char buf[1024];
		c_strlcpy (buf, "");
		for (int seq = 0; seq < number_per_vert; seq ++) {
			int jidx = idx + seq;
			ccs *s = list.strings[jidx];

			if (seq > 0)
				c_strlcat (buf, s_delimiter12); // precat
			c_strlcat (buf, s);
		}
		stringlistappend (&group_of_spaced_floats, buf);
	} // for

	// Now join the spaced floats with commas
	char *s_comma_join_z = stringlist_join_delim_zalloc (&group_of_spaced_floats, s_delimiter_elements, shall_quote);

	stringlistfreecontents (&group_of_spaced_floats); // done with this, thank you
	stringlistfreecontents (&list); // done with this, thank you

	return s_comma_join_z;
}

char *floats_join_delim12_delim_mag_zalloc (floatlist_s *flist, int number_per_vert, ccs *s_delimiter12, ccs *s_delimiter_elements, int shall_quote, mag_s *magni_opt)
{
	if (number_per_vert == 0)
		return NULL;

	if (false == in_range(0, number_per_vert, 12)) // Too many or negative
		return NULL; // limit is 12

	if ( (flist->count modulo number_per_vert) != 0)
		return NULL;

	int num_groups = flist->count / number_per_vert;
	stringlist_t list = {0};
	for (int n = 0; n < flist->count; n ++) {
		float f = flist->floats[n];
		if (magni_opt) {
			// Are these points?  Yes.
			// Punt for now ...
			// We would need to do more than magnify
			// For mod 0 would would need to add offset x
			// For mod 1 would would need to add offset y
			// I'm assuming these are 2D
		}
		stringlistappendf (&list, FLOAT_LOSSLESS_FORMAT, f);
	} // for

	int idx = 0;
	// In groups of x, create a string
	stringlist_t group_of_spaced_floats = {0}; //
	for (int grp = 0; grp < num_groups; grp ++, idx += number_per_vert) {
		// Assume fairly reasonable size
		char buf[1024];
		c_strlcpy (buf, "");
		for (int seq = 0; seq < number_per_vert; seq ++) {
			int jidx = idx + seq;
			ccs *s = list.strings[jidx];

			if (seq > 0)
				c_strlcat (buf, s_delimiter12); // precat
			c_strlcat (buf, s);
		}
		stringlistappend (&group_of_spaced_floats, buf);
	} // for

	// Now join the spaced floats with commas
	char *s_comma_join_z = stringlist_join_delim_zalloc (&group_of_spaced_floats, s_delimiter_elements, shall_quote);

	stringlistfreecontents (&group_of_spaced_floats); // done with this, thank you
	stringlistfreecontents (&list); // done with this, thank you

	return s_comma_join_z;
}

// Vertexes:"20 20, 500 400, 550 450"
// delimiter12 " " is the separator between the numbers in a vertex
// delimter_elements "," is the separator between the vertex doublets or triplets
// number_per_vert 2 means 2D where there are 2 numbers like "20 20" for a vertex

// Returns number of warnings.
int floats_append_parse_space_comma_num_elements_ignored (floatlist_s *flist, ccs *s_value, int number_per_vert, ccs *s_delimiter12, ccs *s_delimiter_elements)
{
	int num_elements_ignored = 0;

	stringlist_t rlist = {0};
	// Split the element pairs using comma split
	{
		char *s_z = Z_StrDup(s_value);
		String_Edit_DeQuote(s_z);
		stringlistappend_split (&rlist, s_z, s_delimiter_elements /*COMMA*/ );
		Z_FreeNull_ (s_z); // DISCHARGED
	}

	//const char **wordray = stringlist_nullterm_add (&rlist);
	stringlist_t tmplist = {0};

	for (int n = 0; n < rlist.numstrings; n ++) {
		// SPLIT ON SPACES
		char *selem = rlist.strings[n];
		String_Edit_Trim(selem);

		stringlistappend_split (&tmplist, selem, s_delimiter12 /*SPACE*/ );
		if (tmplist.numstrings == number_per_vert) {
			for (int idx = 0; idx < tmplist.numstrings; idx ++) {
				char *spx = tmplist.strings[idx];
				float f = atof(spx);
				floats_add1 (flist, f );
			}
		} else {
			num_elements_ignored ++;
		}
		stringlistfreecontents(&tmplist);
	} // while

	stringlistfreecontents(&rlist);

	return num_elements_ignored;
}


RELATED_ (stringlist_condump_ilist)
void int32s_countstrings (int32list_s *ilist, stringlist_t *plist, stringlist_t *plist_indexor)
{
	// Add a zero for each indexor entry
	for (int psidx = 0; psidx < plist_indexor->numstrings; psidx ++) {
		//ccs *sxy =  plist_indexor->strings[psidx];
		int32s_add1 (ilist, 0);
	} // for list

	// For each plist, increment the indexor
	for (int psidx = 0; psidx < plist->numstrings; psidx ++) {
		ccs *sxy =  plist->strings[psidx];
		int index = stringlist_find_index (plist_indexor,sxy);
		ilist->ints[index] = ilist->ints[index] + 1;
	} // for list
}
