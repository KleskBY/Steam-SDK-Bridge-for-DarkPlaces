// sys_sdl_winquake.c.h

#ifdef _WIN32

	#ifdef CONFIG_MENU
		qbool b_is_alloced = false;
		// Client run as dedicated server with -dedicated (-dedicated)
		void Sys_PrintToTerminal_WinQuake (const char *text)
		{
			DWORD dummy;
			extern HANDLE houtput;

			if (b_is_alloced)
				OutputDebugString(text);

			if ((houtput != 0) && (houtput != INVALID_HANDLE_VALUE))
				WriteFile(houtput, text, (DWORD) strlen(text), &dummy, NULL);
		}

	#else
		// For a theoretical Windows dedicated server
		void Sys_PrintToTerminal_Win32 (const char *text)
		{
			if (sys.outfd < 0)
				return;

			#define write _write

			while (*text) {
				fs_offset_t written = (fs_offset_t)write(sys.outfd, text, (int)strlen(text));
				if (written <= 0)
					break; // sorry, I cannot do anything about this error - without an output
				text += written;
			} // while
		}
	#endif
#else
	// Baker: Got sick of all the #ifdefs
	void Sys_PrintToTerminal_Unix (const char *text)
	{
		if (sys.outfd < 0)
			return;

		// BUG: for some reason, NDELAY also affects stdout (1) when used on stdin (0).
		// this is because both go to /dev/tty by default!

		int origflags = fcntl (sys.outfd, F_GETFL, 0);
		fcntl (sys.outfd, F_SETFL, origflags & ~O_NONBLOCK);

		while (*text) {
			fs_offset_t written = (fs_offset_t)write(sys.outfd, text, (int)strlen(text));
			if (written <= 0)
				break; // sorry, I cannot do anything about this error - without an output
			text += written;
		} // while

		fcntl (sys.outfd, F_SETFL, origflags);
	}

#endif // ! _WIN32

#ifdef _WIN32

HANDLE				hinput, houtput;

void Sys_Console_Init_HOutput_WIN32 (void)
{
	if (b_is_alloced == false) {
		b_is_alloced= true;
		AttachConsole(ATTACH_PARENT_PROCESS);
	}
	houtput = GetStdHandle (STD_OUTPUT_HANDLE); // Is returning 0
}

void Sys_Console_InitOnce_WinQuake (void) // June 1 2025 - only -dedicated comes here
{
	houtput = GetStdHandle (STD_OUTPUT_HANDLE);
	hinput = GetStdHandle (STD_INPUT_HANDLE);

	// LadyHavoc: can't check cls.state because it hasn't been initialized yet
	// if (cls.state == ca_dedicated)
	if (Sys_CheckParm("-dedicated")) {
		//if ((houtput == 0) || (houtput == INVALID_HANDLE_VALUE)) // LadyHavoc: on Windows XP this is never 0 or invalid, but hinput is invalid
		{
			if (!AllocConsole ())
				Sys_Error ("Couldn't create dedicated server console (error code %x)", (unsigned int)GetLastError());
			houtput = GetStdHandle (STD_OUTPUT_HANDLE);
			hinput = GetStdHandle (STD_INPUT_HANDLE);
		}
		if ((houtput == 0) || (houtput == INVALID_HANDLE_VALUE))
			Sys_Error ("Couldn't create dedicated server console");

	}
}

char *Sys_ConsoleInput_WinQuake (void)
{
	static char text[MAX_INPUTLINE_16384];
	static int len;
	INPUT_RECORD recs[1024];
	DWORD numread, numevents, dummy;

	// Baker: We are 100% dedicated here
	// if (cls.state != ca_dedicated)
	//	return NULL;

	for ( ;; ) {
		if (!GetNumberOfConsoleInputEvents (hinput, &numevents)) {
			// cls.state = ca_disconnected;
			Sys_Error ("Error getting # of console events (error code %x)", (unsigned int)GetLastError());
		}

		if (numevents <= 0)
			break;

		if (!ReadConsoleInput(hinput, recs, 1, &numread)) {
			//cls.state = ca_disconnected;
			Sys_Error ("Error reading console input (error code %x)", (unsigned int)GetLastError());
		}

		if (numread != 1) {
			//cls.state = ca_disconnected;
			Sys_Error ("Couldn't read console input (error code %x)", (unsigned int)GetLastError());
		}

		if (recs[0].EventType == KEY_EVENT ) {
			int ch = recs[0].Event.KeyEvent.uChar.AsciiChar;
			int dws = recs[0].Event.KeyEvent.dwControlKeyState;
			// LEFT_CTRL_PRESSED 0x0008 RIGHT_CTRL_PRESSED 0x0004

			if (recs[0].Event.KeyEvent.bKeyDown == 0) {
				if ( (ch == 22 || ch == 118) && (dws & LEFT_CTRL_PRESSED /*0x0008*/)) {
					ccs *s = Clipboard_Get_Text_Line_Static (); // CTRL-V hack
					//logc ("Pasting %d", s);
					int slen = (int)strlen(s);
					if (slen) {
						// We need to jam it into the text buffer
						for (int n = 0; n < slen; n ++) {
							ch = s[n];
							WriteFile (houtput, &ch, 1, &dummy, NULL);
							text[len] = ch, len = (len + 1) & 0xff; // Wrap
						} // end for
					} // if slen

					break;
				} // Pastey - Jan 27 2020 - CTRL-V paste hack
				continue;
			}

			switch (ch)
			{
			case '\r': // Enter via carriage return character
				WriteFile (houtput, "\r\n", 2, &dummy, NULL); // We need a carriage return?

				if (len) {
					text[len] = 0, len = 0;

					return text; // Strip trailing newline, reset length to 0.
				}
				else if (1) // sc_return_on_enter)
				{
				// special case to allow exiting from the error handler on Enter
					text[0] = '\r';
					text[1] = 0;
					len = 0;
					return text;
				}
				break;

			case '\b': // Backspace character
				WriteFile (houtput, "\b \b", 3, &dummy, NULL);
				if (len)
					len--;
				break;

			default:
				if (ch >= 32) {
					WriteFile (houtput, &ch, 1, &dummy, NULL);
					text[len] = ch, len = (len + 1) & 0xff; // Wrap
				}
				break;
			} // End switch
		} // Endif

	} // End of for

	return NULL;
}

#endif // _WIN32





