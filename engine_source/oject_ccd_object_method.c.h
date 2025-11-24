// oject_ccd_object_method.c.h

typedef struct {
	ccs		*name;
	exofn_t	fn;
	int		is_quakec;
} method_entry_s;

#define DOENTRY(x) { STRINGIFY(x), /*x*/ NULL }
method_entry_s method_entries[] = {
	//{ "DevInit",	DevInit },
	//{ "DevOnIndex", DevOnIndex },
	{"", NULL }, // Method 0 unavailable.
	DOENTRY(DevInit),
	DOENTRY(DevTabSelectOnChange),
};
#undef DOENTRY

int Method_Find_Index (ccs *s_method)
{
	for (int j = 1; j < (int)ARRAY_COUNT(method_entries); j ++) {
		method_entry_s *zi = &method_entries[j];
		if (String_Match_Caseless(zi->name, s_method)) {
			return j; //zi->fn;// (f, k, listidx, s);
		}
	}
	//Con_PrintLinef ("zexeccmd: command function " QUOTED_S " not found", s_fn);
	return not_found_neg1;
}

// Jan 23 2025 - BTW this ignores containers.
// Makes an object the first object in the control list, draws first so gets covered
// by any other object drawn in front of it.
// This ignore containers so it sucks.
// This causes the control index to change so it sucks.
// Deal with it for now.

void Object_Method_ZOrder_Front (oject_s *kzlast)
{
	Form_Get (f, kzlast);
	int fidx = Form_Find_Idx(f, kzlast);
	int fcount = f->frm.controls_a->numitems;
	if (fidx == fcount -1)
		return; // Already last

	f->frm.controls_a->pointers_array[fidx] = NULL;
	// So we nulled out #20 or #0
	//0 = 1, 1 = 0
	//1 = 2, 2 = 0
	//for count-1 to 1

	// Shift all down from fidx to end.  We could memcpy to do it better.
	// WATCHOUT must do --> f->frm.controls_a->numitems - 1 or we write out of bounds
	// because array[count - 1 + 1] is beyond limit
	for (int jidx = fidx; jidx < f->frm.controls_a->numitems - 1; jidx ++) {
		//oject_s *kthis  = f->frm.controls_a->pointers_array[jidx + 0];
		//c_assert_msg_ (kthis == NULL, "Object_Method_ZOrder_Front this shouldn't be null");
		oject_s *kafter = f->frm.controls_a->pointers_array[jidx + 1];
		f->frm.controls_a->pointers_array[jidx + 0] = kafter;
		f->frm.controls_a->pointers_array[jidx + 1] = NULL;
	} // for

	f->frm.controls_a->pointers_array[f->frm.controls_a->numitems - 1] = kzlast;
}


int Object_Get_ControlIndex (oject_s *k)
{
	Form_Get(f,k);
	for (int kidx = 0; kidx < f->frm.controls_a->numitems; kidx ++) {
		oject_s *kx = f->frm.controls_a->pointers_array[kidx];
		if (kx == k)
			return kidx;
	} // for
	return not_found_neg1;
}

void Object_Method_ZOrder_Back (oject_s *kzlast)
{
	Form_Get (f, kzlast);
	int fidx = Form_Find_Idx(f, kzlast);
//	int fcount = f->frm.controls_a->numitems;

	if (fidx == 0) // fcount -1)
		return; // Already first // Already last

	// We are #18 .. We need to become #0
	// If there are 20
	// 0 to 17 move up +1
	// We become #0

	f->frm.controls_a->pointers_array[fidx] = NULL;
	// So we nulled out #20 or #0
	//0 = 1, 1 = 0
	//1 = 2, 2 = 0
	//for count-1 to 1

	// Shift all down from fidx to end.  We could memcpy to do it better.
	// WATCHOUT must do --> f->frm.controls_a->numitems - 1 or we write out of bounds
	// because array[count - 1 + 1] is beyond limit
	for (int jidx = fidx - 1; jidx >= 0; jidx --) {
		//oject_s *kthis  = f->frm.controls_a->pointers_array[jidx + 0];
		//c_assert_msg_ (kthis == NULL, "Object_Method_ZOrder_Front this shouldn't be null");
		oject_s *kbefore = f->frm.controls_a->pointers_array[jidx];
		f->frm.controls_a->pointers_array[jidx + 0] = NULL;
		f->frm.controls_a->pointers_array[jidx + 1] = kbefore;
	} // for

	f->frm.controls_a->pointers_array[0] = kzlast;
}


void Object_Method_Hide (oject_s *k)
{
}

void Object_Method_Show (oject_s *k)
{
}

void Object_Method_Enable (oject_s *k)
{
}

void Object_Method_Disable (oject_s *k)
{
}




