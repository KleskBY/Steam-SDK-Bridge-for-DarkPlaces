// prvm_inventory.h


// Returns < 0 on failure.

// What else do we do?
void VM_Inventory_Read (prvm_prog_t *prog);
void VM_Inventory_Get (prvm_prog_t *prog);

RELATED_ (VM_CL_Form_Load FS_Inventory_f)

//float(float mdlidx) modelframecount = #0:modelframecount; /*
RELATED_ (VM_CL_frameforname VM_CL_modelframecount)


// Restore game must call this
// float inventoryreload (float slot, string filename) = #195; // Slot is 1 to 8
// Returns num columns setting v_forward to rows/columns
void VM_Inventory_Reload (prvm_prog_t *prog)
{	
	VM_SAFEPARMCOUNTRANGE(2, 2, VM_Inventory_Read); // 0: slot plus 1 1: filename

	// Set return to failure
	PRVM_G_FLOAT(OFS_RETURN) = -1;

	int handlep1 = (int)PRVM_G_FLOAT(OFS_PARM0); // 1
	int handle = UNPLUS1(handlep1);

	ccs *filename = PRVM_G_STRING(OFS_PARM1);  VM_CheckEmptyString(prog, filename); // Baker: Host error if first character is whitespace or NULL char 0

	if (in_range_beyond(0, handle, PRVM_MAX_INVENTORY_8) == false) {		
		VM_WarningLinef (prog, "%s: handle invalid = %d, max 1 to %d in %s", 
			__func__, handle + 1, PRVM_MAX_OPEN_FORMS_8, prog->name);
		return;
	}

	stringlist_t *plist = &prog->invent[handle];
	stringlistfreecontents (plist);

	char *s_zalloc = FS_LoadFile_Quiet_Temp (filename);
	if (!s_zalloc) {
		VM_WarningLinef (prog, "%s: Could not read filename %s in %s", 
			__func__, filename, prog->name);
		return;
	}
	int numcolumns = stringlistappend_csv_nl_tab (plist, s_zalloc);

	Mem_FreeNull_ (s_zalloc);

	prog->invent_num_columns[handle] = numcolumns;

	vec3_t v = {0};
	v[2] = plist->numstrings; // Cells
	v[1] = prog->invent_num_columns[handle]; // Cols
	v[0] = v[2] / prog->invent_num_columns[handle]; // Rows
	
	VectorCopyDestSrc (PRVM_gameglobalvector(v_forward), v); // Rows, Cols, Cells

	PRVM_G_FLOAT(OFS_RETURN) = numcolumns;
}

RELATED_ (VM_CL_GetEntity)

// string getinv (float slot, string key, string column) = #196;
// string s = getinv(1, "longsword", "model"); // Case sensitive

// If we do this where this is stored in a QuakeC struct when inventory is updated
// The lookups will be minimized
void VM_Inventory_GetString (prvm_prog_t *prog)
{
	char string16384[VM_STRINGTEMP_LENGTH_16384];

	// s = getinv(1, "longsword", "model"); // Returns "model/
	// 0: handle 1: name (row first column) 2: column ==> models/backpack
	VM_SAFEPARMCOUNTRANGE(3, 3, VM_Inventory_GetString);

	PRVM_G_INT(OFS_RETURN) = OFS_NULL; // In case of error

	int handle = UNPLUS1((int)PRVM_G_FLOAT(OFS_PARM0));
	if (in_range_beyond(0, handle, PRVM_MAX_INVENTORY_8) == false) {		
		VM_WarningLinef (prog, "%s: handle invalid = %d, max 1 to %d in %s", 
			__func__, handle + 1, PRVM_MAX_OPEN_FORMS_8, prog->name);
		return;
	}

	ccs *sname = PRVM_G_STRING(OFS_PARM1);  VM_CheckEmptyString(prog, sname); // Baker: Host error if first character is whitespace or NULL char 0
	ccs *scolumn = PRVM_G_STRING(OFS_PARM2);  VM_CheckEmptyString(prog, scolumn); // Baker: Host error if first character is whitespace or NULL char 0

	stringlist_t *plist = &prog->invent[handle];
	int numcolumns = prog->invent_num_columns[handle];

	int rownumber = stringlist_find_row_number (plist, sname, numcolumns);
	if (rownumber == not_found_neg1) {		
		VM_WarningLinef (prog, "%s: could not find row for %s in %s", 
			__func__, sname, prog->name);
		return;
	}
	int columnnumber = stringlist_find_column_number (plist, scolumn, numcolumns);
	if (rownumber == not_found_neg1) {		
		VM_WarningLinef (prog, "%s: could not column for %s in %s", 
			__func__, scolumn, prog->name);
		return;
	}

	ccs *svalue = plist->strings[rownumber * numcolumns + columnnumber];
	c_strlcpy (string16384, svalue);
	PRVM_G_INT(OFS_RETURN) = PRVM_SetTempString(prog, string16384);
}

void VM_Inventory_GetFloat (prvm_prog_t *prog)
{
	// s = getinv(1, "longsword", "model"); // Returns "model/
	// 0: handle 1: name (row first column) 2: column ==> models/backpack
	VM_SAFEPARMCOUNTRANGE(3, 3, VM_Inventory_GetFloat);

	PRVM_G_FLOAT(OFS_RETURN) = -1; // In case of error

	int handle = UNPLUS1((int)PRVM_G_FLOAT(OFS_PARM0));
	if (in_range_beyond(0, handle, PRVM_MAX_INVENTORY_8) == false) {		
		VM_WarningLinef (prog, "%s: handle invalid = %d, max 1 to %d in %s", 
			__func__, handle + 1, PRVM_MAX_OPEN_FORMS_8, prog->name);
		return;
	}

	ccs *sname = PRVM_G_STRING(OFS_PARM1);  VM_CheckEmptyString(prog, sname); // Baker: Host error if first character is whitespace or NULL char 0
	ccs *scolumn = PRVM_G_STRING(OFS_PARM2);  VM_CheckEmptyString(prog, scolumn); // Baker: Host error if first character is whitespace or NULL char 0

	stringlist_t *plist = &prog->invent[handle];
	int numcolumns = prog->invent_num_columns[handle];

	int rownumber = stringlist_find_row_number (plist, sname, numcolumns);
	if (rownumber == not_found_neg1) {		
		VM_WarningLinef (prog, "%s: could not find row for %s in %s", 
			__func__, sname, prog->name);
		return;
	}
	int columnnumber = stringlist_find_column_number (plist, scolumn, numcolumns);
	if (rownumber == not_found_neg1) {		
		VM_WarningLinef (prog, "%s: could not column for %s in %s", 
			__func__, scolumn, prog->name);
		return;
	}

	ccs *svalue = plist->strings[rownumber * numcolumns + columnnumber];

	PRVM_G_FLOAT(OFS_RETURN) = atof(svalue);
}


void VM_Inventory_GetVector (prvm_prog_t *prog)
{
	// s = getinv(1, "longsword", "model"); // Returns "model/
	// 0: handle 1: name (row first column) 2: column ==> models/backpack
	VM_SAFEPARMCOUNTRANGE(3, 3, VM_Inventory_GetFloat);

	PRVM_G_VECTOR(OFS_RETURN)[0] = -1; // In case of error

	int handle = UNPLUS1((int)PRVM_G_FLOAT(OFS_PARM0));
	if (in_range_beyond(0, handle, PRVM_MAX_INVENTORY_8) == false) {		
		VM_WarningLinef (prog, "%s: handle invalid = %d, max 1 to %d in %s", 
			__func__, handle + 1, PRVM_MAX_OPEN_FORMS_8, prog->name);
		return;
	}

	ccs *sname = PRVM_G_STRING(OFS_PARM1);  VM_CheckEmptyString(prog, sname); // Baker: Host error if first character is whitespace or NULL char 0
	ccs *scolumn = PRVM_G_STRING(OFS_PARM2);  VM_CheckEmptyString(prog, scolumn); // Baker: Host error if first character is whitespace or NULL char 0

	stringlist_t *plist = &prog->invent[handle];
	int numcolumns = prog->invent_num_columns[handle];

	int rownumber = stringlist_find_row_number (plist, sname, numcolumns);
	if (rownumber == not_found_neg1) {		
		VM_WarningLinef (prog, "%s: could not find row for %s in %s", 
			__func__, sname, prog->name);
		return;
	}
	int columnnumber = stringlist_find_column_number (plist, scolumn, numcolumns);
	if (rownumber == not_found_neg1) {		
		VM_WarningLinef (prog, "%s: could not column for %s in %s", 
			__func__, scolumn, prog->name);
		return;
	}

	ccs *svalue = plist->strings[rownumber * numcolumns + columnnumber];

	Math_atov_vm(svalue, PRVM_G_VECTOR(OFS_RETURN));
	//PRVM_G_FLOAT(OFS_RETURN) = atof(svalue);
}

