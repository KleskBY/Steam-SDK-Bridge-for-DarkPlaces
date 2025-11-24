// filematch.c

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

#include "darkplaces.h"

#ifdef _WIN32
#include "utf8lib.h"
#endif

// LadyHavoc: some portable directory listing code I wrote for lmp2pcx, now used in darkplaces to load id1/*.pak and such...

int matchpattern(const char *in, const char *pattern, int caseinsensitive)
{
	return matchpattern_with_separator(in, pattern, caseinsensitive, "/\\:", false);
}

// wildcard_least_one: if true * matches 1 or more characters
//                     if false * matches 0 or more characters
int matchpattern_with_separator(const char *in, const char *pattern, int caseinsensitive, const char *separators, qbool wildcard_least_one)
{
	int c1, c2;
	while (*pattern)
	{
		switch (*pattern)
		{
		case 0:
			return 1; // end of pattern
		case '?': // match any single character
			if (*in == 0 || strchr(separators, *in))
				return 0; // no match
			in++;
			pattern++;
			break;
		case '*': // match anything until following string
			if (wildcard_least_one)
			{
				if (*in == 0 || strchr(separators, *in))
					return 0; // no match
				in++;
			}
			pattern++;
			while (*in)
			{
				if (strchr(separators, *in))
					break;
				// see if pattern matches at this offset
				if (matchpattern_with_separator(in, pattern, caseinsensitive, separators, wildcard_least_one))
					return 1;
				// nope, advance to next offset
				in++;
			}
			break;
		default:
			if (*in != *pattern)
			{
				if (!caseinsensitive)
					return 0; // no match
				c1 = *in;
				if (c1 >= 'A' && c1 <= 'Z')
					c1 += 'a' - 'A';
				c2 = *pattern;
				if (c2 >= 'A' && c2 <= 'Z')
					c2 += 'a' - 'A';
				if (c1 != c2)
					return 0; // no match
			}
			in++;
			pattern++;
			break;
		}
	}
	if (*in)
		return 0; // reached end of pattern but not end of input
	return 1; // success
}



#include "filematch_stringlist.c.h"
#include "filematch_intlist.c.h"
#include "filematch_csg.c.h"
#include "filematch_csg_perform.c.h"
#include "eq_zparse.c.h" // Totally "eq"


RELATED_ (stringlistappend)
int bakerarray_ptr_new_item_index(bakerarray_s *a)
{
	if ( (a->numitems + 1) >= a->maxitems) {
		int mybatchsize = a->batchsize ? a->batchsize : BAKER_ARRAY_BATCHSIZE_DEFAULT_128;
		a->maxitems += mybatchsize;
		byte	*olddata	= (byte	*)&a->pointers_array[0];
		size_t	oldsize		= a->datasize;
		size_t	newsize		= a->maxitems * sizeof(void *);//a->mysizeof;
		a->pointers_array = (void **)Mem_Alloc(zonemempool, newsize); // Z_Malloc(newsize);
		if (olddata) {
			memmove (a->pointers_array, olddata, oldsize);
			Mem_Free (olddata);
		}
		a->datasize = newsize; // Fix? Baker: Jan 12 2025 - Apparently.  Wow, fool!
	} // Realloc

	return a->numitems ++; // We return 1 less than what we can accomodate
}

// This clears the pointers, but does NOT realloc anything -- maxitems remains the same.
int bakerarray_ptr_erase(bakerarray_s *a)
{
	if (a->numitems) {
		size_t	current_size = a->numitems * sizeof(void *);
		memset (a->pointers_array, 0, current_size);
	}
	a->numitems = 0;
	return 0;
}

