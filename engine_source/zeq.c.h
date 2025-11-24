// eq.c.h

int equat_calc_result (equat_t *eq, ccs *s_equation, double *pout, char *errbuff, size_t errbuffsize);

#include "zeq_tests.c.h"

// Doubles only
RELATED_ (node_free)



void SCR_atan2_f (cmd_state_t *cmd)
{
	if (Cmd_Argc (cmd) != 3) {
		Con_PrintLinef ("atan2 requires 2 args");
		return;
	}
	double y = atof(Cmd_Argv(cmd, 1));
	double x = atof(Cmd_Argv(cmd, 2));
	double dbl_result = atan2(y,x);
	Con_PrintLinef ("atan2(%s, %s) = %g", Cmd_Argv(cmd, 1), Cmd_Argv(cmd, 2), dbl_result);

		//va_super (s_result, 1024, FLOAT_LOSSLESS_FORMAT, (float)dbl_result);
		//Cvar_SetValueQuick (&_last, dbl_result);
		//Con_Printf ("_last = %s", s_result);

	_Cvar_Set_IsNotify_IsClipboard_String (&_val, q_notifyinfobar_true, eq_clipboard_set.integer /*d:1*/ != 0, q_shall_print_true,
		FLOAT_LOSSLESS_FORMAT, 
		(float)dbl_result
	);

}

void SCR_hex_f (cmd_state_t *cmd)
{
	if (Cmd_Argc (cmd) <= 1) {
		Con_PrintLinef ("Decimal to hex: example " QUOTED_STR("hex 33") );
	}

	ccs *s = Cmd_Argv (cmd, 1);
	int ival = atoi(s);

	Con_PrintLinef ("Decimal %d is hex %x", ival, ival);

}

void SCR_eq_f (cmd_state_t *cmd)
{
	if (Cmd_Argc (cmd) <= 1) {
		Con_PrintLinef ("equation calculator: example " QUOTED_STR("eq 2 + 2") " or " QUOTED_STR("eq cos(2)") );
			Con_Printf ("functions:");
			cfunctions_condump_as_line ();
			Con_PrintLinef ("%s", "");
		
		Con_Printf ("constants:");
			cconstants_condump_as_line ();
			Con_PrintLinef ("%s", "");

		Con_Printf ("operators: ");
			Con_PrintLinef ("%s", "~ ! !! * / % + - << >> < <= > >= == != & ^ | && || pow");
		
		Con_PrintLinef ("All values are 64-bit floating point (double)");
		Con_PrintLinef ("All bitwise operations as 32-bit signed integer (like QuakeC)");
		Con_PrintLinef ("%s", "% modulus operator keeps floating remainder (like QuakeC) 4.1 % 4 == 0.1");		
		return;
	}

#if 0
	baker_string_t *bs_eq = BakerString_Create_Malloc ("");
	for (int j = 1; j < Cmd_Argc(cmd); j ++) {
		ccs *sxy = Cmd_Argv (cmd, j);
		if (j > 1)
			BakerString_CatC (bs_eq, " ");
		BakerString_CatC (bs_eq, sxy);
	}
#else
	char s_args_all[1024];
	Argv_Cumulate (cmd, 1, s_args_all, sizeof(s_args_all) );
	baker_string_t *bs_eq = BakerString_Create_Malloc (s_args_all);
#endif


	if (developer_execstring.integer)
		Con_PrintLinef (CON_BRONZE "Equation: " QUOTED_S, bs_eq->string);

	double dbl_result = -9999;
	char errbuf[2048];
	equat_t _eq = {0};

	// Equation contents are freed by processor.  The struct itself obviously not.
	int errcode = equat_calc_result (&_eq, bs_eq->string, &dbl_result, errbuf, sizeof(errbuf));

	if (errcode) {
		Con_PrintLinef ("%s", errbuf);

		//equat_condump_tree (eq);
	} else {
		_Cvar_Set_IsNotify_IsClipboard_String (&_val, q_notifyinfobar_true, eq_clipboard_set.integer /*d:1*/ != 0, q_shall_print_true,
			FLOAT_LOSSLESS_FORMAT, 
			(float)dbl_result
		);

		//va_super (s_result, 1024, FLOAT_LOSSLESS_FORMAT, (float)dbl_result);
		//Cvar_SetValueQuick (&_last, dbl_result);
		//Con_Printf ("_last = %s", s_result);
		//if (eq_clipboard_set.integer) {
		//	Clipboard_Set_Text (s_result); // eq_clipboard_set.integer d:1
		//	Con_PrintLinef ("%s", " (copied to clipboard)");
		//} else {
		//	Con_PrintLinef ("%s", "");
		//}
	}

	BakerString_Destroy_And_Null_It (&bs_eq);
}




