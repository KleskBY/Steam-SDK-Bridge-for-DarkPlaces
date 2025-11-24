// console_autocomplete_function.c.h

// return value is keyline_pos (trivally used to set keyline_pos)
// is_console better be true, this is not made for chat mode
// is_from_nothing means someone did "map " and pressed CTRL-SPACE
//       and we are autocompleting from thin air completing from 100% of context (so if would be all maps, for instance)

//#pragma message ("Baker: So many string functions and actions here are likely not UTF8 safe")
//#pragma message ("and neither is the standard DarkPlaces autocomplete, but ours does more and uses more functions")

// 2 EXITS

#include "console_autocomplete_function_enums.h"

int Get_UI_ObjectName_AutoComp_Count (ccs *s_prefix);
int Get_UI_Property_AutoComp_Count (ccs *s_prefix);

extern cvar_t prvm_sv_gamecommands; // Baker r7103 gamecommand autocomplete
extern cvar_t prvm_cl_gamecommands; // Baker r7103 gamecommand autocomplete

qbool UI_Fly (ccs *s_controlname, ccs *s_propyname, char *valuebuf, size_t valuebuf_size);;

#define COMMAND ac->s_command0_a
#define MAX_WORDS_16 16


int Con_CompleteCommandLine_Zircon(cmd_state_t *cmd, qbool is_console, qbool is_shifted, qbool is_from_nothing)
{
	autocomplete_t		*ac = &_g_autocomplete;	// Our global
	char	*oldspartial = 						// Old value of this.  If NULL, pick first result
				ac->s_search_partial_a;			// And if NULL, spam matching commands/cvars to console.

	char	value_autofill[1024];				// For autofill of cvar or prvm_edictset

	char	*must_end_autocomplete = NULL;		// Search terminated, likely due to ZERO or ONE result
												// Clear stuff and perform Partial_Reset ();

	qbool	do_fly_submit = false;				// r_replacemaptexture immediate replacement.

	ccs		**list[4] = {0, 0, 0, 0};

	char	*start_command = &key_line[1];		// After bracket "]map e2" (will get moved if ; found)
	int		saved_cursor_char =					// We null it out for string operations, then restore it.
				key_line[key_linepos];


	// Ok plan ... fully formed data up here!
	key_line[key_linepos] = NULL_CHAR_0;		// NULL AT CURSOR

	// Baker: October 30 2024 - This works due to null termination above.
	// Baker: January 3 2025 - A semi colon in a quote string is one thing that could mess this up.
	char	*s_last_semicolon_before = dp_strstr_reverse(&key_line[1], ";");

	// "color 4; map e2" --> in this situation, s_command0 is "map" not "color"

	// Move start_command forward after last semi-colon before cursor if there is a semi-colon
	if (s_last_semicolon_before)
		start_command = String_Skip_WhiteSpace_Including_Space(&s_last_semicolon_before[1]);

	// Baker: October 30 2024 - Last
	char words_count = 0;
	char words[MAX_WORDS_16][256]; // example: words[1] == "server" 
	char whole_line[1024];

	c_strlcpy (whole_line, start_command); // Depds: start_command s_last_semicolon_before
	{
		ccs *temp_data = start_command;
		for (int idx = 0; idx < MAX_WORDS_16; idx ++) {
			if (!COM_ParseToken_Console (&temp_data))
				break;
			//size_t sz =sizeof(words[idx]); // Veryify is 256 ... it is verified
			c_strlcpy (words[idx], com_token);
			words_count ++;
			if (developer_extra.integer)
				Con_PrintLinef ("Word %03d: " QUOTED_S, idx, words[idx]);
		}
	}

	// We have filled in WORDS.  Restore NULLed out character.
	key_line[key_linepos] = saved_cursor_char;

	// We are continuing a previous autocomplete
	int is_previous_autocomplete = ac->s_search_partial_a != NULL;
	if (ac->s_search_partial_a)
		goto autocomplete_go;

	ac->p_text_partial_start		= NULL;
	ac->p_text_completion_start		= NULL;
	ac->p_text_beyond_autocomplete	= NULL;
	ac->is_at_first_arg				= 0;
	ac->is_from_nothing				= is_from_nothing;
	ac->searchtype					= 0;
	ac->search_partial_offset		= 0;
	freenull_ (ac->s_command0_a);
	freenull_ (ac->s_completion_a);

	// NEW AUTOCOMPLETE

	// Look back from cursor and find semi-colon to locate start of current search term
	int search_partial_offset = key_linepos;

	while (--search_partial_offset) {
		int k = key_line[search_partial_offset];
		if (k == '\"' || k == ';' || k == ' ' || k == '\'')
			break;
	}
	search_partial_offset ++;

	ac->search_partial_offset = search_partial_offset;


	// Baker: October 30 2024 - This is START OF ITEM TO COMPLETE "map e4" is "e4"
	ac->p_text_partial_start = &key_line[ac->search_partial_offset];		// what we wish to find

	// Baker: October 30 2024 - This is BEYOND THE CURRENT SEARCH TERM.
	ac->p_text_beyond_autocomplete = &key_line[key_linepos];

	setstr (ac->text_after_autocomplete_a, ac->p_text_beyond_autocomplete);

	//
	// LINE TERMINATED AT CURSOR START ...
	//


	/*int*/ saved_cursor_char = ac->p_text_beyond_autocomplete[0];
	ac->p_text_beyond_autocomplete[0] = NULL_CHAR_0;

	if (ac->p_text_partial_start[0] == 0 && ac->is_from_nothing == false) {
		// Rejected empty autocomplete.
		ac->p_text_beyond_autocomplete[0] = saved_cursor_char;
exit_possible:
		return key_linepos; // Does this happen? "map " <-- press TAB, yes
	}

	// This process only works because we null terminated at cursor
	setstr (ac->s_search_partial_a, ac->p_text_partial_start);

#if 1 // July 29 2025
	// Baker: What happens when 2nd complete is not from nothing?
	// A: We never come here at all and start at autocomplete_go:
	if (ac->s_search_partial_a[0] && is_previous_autocomplete == false)
		ac->is_from_nothing = false;
#endif

	// Baker: words_count - 1 is where we are

	int in_arg_number = words_count - 1; // 0 is the command
	int is_currently_nothing = false;
	if (isin2(*ac->p_text_partial_start, NULL_CHAR_0, SPACE_CHAR_32)) {
		// "map " .. shows 1 arg but we are completing #2 from nothing.
		is_currently_nothing = true;
		in_arg_number ++;
	}

	char *space = strchr(&start_command[1], ' '); // Find first space after the command

	// Check if the first space after the command is where the partial is
	// "map "
	ac->is_at_first_arg = space && ac->p_text_partial_start == &space[1];

	if (space) {
		int saved = space[0];
		space[0] = NULL_CHAR_0;
		setstr (ac->s_command0_a, start_command); // Length?
		space[0] = saved;
	}

	ac->is_at_first_arg = ac->s_command0_a && &space[1] == ac->p_text_partial_start;

	#include "console_autocomplete_function_determine.c.h" // Switch

	ac->p_text_beyond_autocomplete[0] = saved_cursor_char;

	//
	// LINE TERMINATED AT CURSOR END ...
	//


autocomplete_go:

	// Reset these
	freenull_(ac->s_match_after_a);		freenull_(ac->s_match_alphalast_a);
	freenull_(ac->s_match_alphatop_a);	freenull_(ac->s_match_before_a);

	char *s = ac->s_search_partial_a;

	// First time autocomplete prints a list, 2nd time does not ..
	int is_quiet = oldspartial ? true : false;

	if (ac->searchtype)	{
		#include "console_autocomplete_function_switch.c.h"
	} // searchtype

	int c;
unidentified:
	// Unidentified completion - do cvars/commands/aliases -- count number of possible matches and print them
	c = Cmd_CompleteCountPossible(cmd, s, ac->is_from_nothing);
	if (c && is_quiet == false) {
		Con_PrintLinef (NEWLINE "%d possible command%s", c, (c > 1) ? "s: " : ":");
		Cmd_CompleteCommandPrint(cmd, s, ac->is_from_nothing);
	}
	int v = Cvar_CompleteCountPossible(cmd->cvars, s, CF_CLIENT | CF_SERVER, ac->is_from_nothing);
	if (v && is_quiet == false) {
		Con_PrintLinef (NEWLINE "%d possible variable%s", v, (v > 1) ? "s: " : ":");
		Cvar_CompleteCvarPrint(cmd->cvars, s, CF_CLIENT | CF_SERVER, is_from_nothing);
	}
	int a = Cmd_CompleteAliasCountPossible(cmd, s, ac->is_from_nothing);
	if (a && is_quiet == false) {
		Con_PrintLinef (NEWLINE "%d possible alias%s", a, (a > 1) ? "es: " : ":");
		Cmd_CompleteAliasPrint(cmd, s, ac->is_from_nothing);
	}

	if (c + v + a == 0) {
		// No possible matches
		must_end_autocomplete = "No possible alias, command, variable match";
		goto exit_out;
	}

	const char *s_match = "";

	if (c)	s_match = *(list[0] = Cmd_CompleteBuildList(cmd, s, ac->is_from_nothing));
	if (v)	s_match = *(list[1] = Cvar_CompleteBuildList(cmd->cvars, s, CF_CLIENT | CF_SERVER, ac->is_from_nothing));
	if (a)	s_match = *(list[2] = Cmd_CompleteAliasBuildList(cmd, s, ac->is_from_nothing));

	int common_length, j;
	for (common_length = (int)strlen(s); ; common_length++) {
		const char **listitems;
		for (j = 0; j < 3; j++) {
			if (list[j]) {
				for (listitems = list[j]; *listitems; listitems ++) {
					if ((*listitems)[common_length] != s_match[common_length]) {
						goto search_completed;
					}
				} // for
			} // if
		} // for

		// all possible matches share this character, so we continue...
		if (s_match[common_length] == NULL_CHAR_0) {
			// if all matches ended at the same position, stop (this means there is only one match)
			goto search_completed;
		}
	} // for

search_completed:

	// SET COMPLETION
	if (oldspartial == NULL) {
		// No previous autocomplete, we are doing it for the first time so pick the alpha top
		setstr (ac->s_completion_a, ac->s_match_alphatop_a);
	} else if (is_shifted) {
		// We are shifted so use the "alpha before"
		if (ac->s_match_before_a == NULL) {
			 // Nothing before this item, loop around to last
			setstr (ac->s_completion_a, ac->s_match_alphalast_a);
		} else {
			setstr (ac->s_completion_a, ac->s_match_before_a);
		}
	} else {
		// We are not shifted, use the "alpha after"
		if (ac->s_match_after_a == NULL) {
			// Nothing after this item, loop around to top
			setstr (ac->s_completion_a, ac->s_match_alphatop_a);
		} else {
			setstr (ac->s_completion_a, ac->s_match_after_a);
		}
	}

	// We may skip printing if we only had one match, we still do the completion
	char *fill;
	int actual_len;
	int fill_len;
one_match_skip:

	// Baker: We are always adding a space after the autocomplete.

	fill = ac->s_completion_a;
	actual_len = (int)strlen(fill);
 	fill_len = actual_len + ONE_CHAR_1;

	// prevent a buffer overrun by limiting cmd_len according to remaining space
	fill_len = Smallest(fill_len, (int)sizeof(key_line) - ONE_CHAR_1 - ac->search_partial_offset);
	if (fill) {
		// start of complete .. copy the "cmd" (completion) over
		key_linepos = ac->search_partial_offset;
		memcpy (&key_line[key_linepos], fill, actual_len);
		key_line[key_linepos + fill_len - ONE_CHAR_1] = SPACE_CHAR_32;
		key_linepos += fill_len;

		// if there is only one match, add a space after it
		int is_only_one = oldspartial == NULL
			&& String_Match (ac->s_match_alphalast_a, ac->s_match_alphatop_a);
		if (key_linepos < (int)sizeof(key_line) - 1 && is_only_one) {
			// Only first partial complete shall do this
//			key_line[key_linepos ++] = ' ';
			must_end_autocomplete = "Only one match";
		} // if
	}

	//if (do_fly_submit /*ac->searchtype == 40*/) {
	if (ac->searchtype == search1_sky_6 && is_currently_nothing) { // IT IS CORRECT
		// do fly submit
		ccs *s_submit = &key_line[1];
		Cbuf_AddTextLinef (cmd, "%s", s_submit);
	}
	else if (ac->searchtype == search2_r_replacemaptexture_254) {
		// do fly submit
		ccs *s_submit = &key_line[1];
		Cbuf_AddTextLinef (cmd, "%s", s_submit);
	}

	// use strlcat to avoid a buffer overrun
	key_line[key_linepos] = 0;
	c_strlcat (key_line, ac->text_after_autocomplete_a);

	// free the command, cvar, and alias lists
	for (j = 0; j < 4; j++) {
		if (list[j]) Mem_Free((void *)list[j]);
	}

exit_out:

	if (must_end_autocomplete) {
		// Search terminated, likely due to ZERO or ONE result
		// Clear stuff
		Partial_Reset ();
	} else {
		// Save stuff

	}
	return key_linepos;
}

#undef COMMAND


