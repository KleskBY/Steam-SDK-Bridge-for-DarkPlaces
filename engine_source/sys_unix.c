// sys_unix.c

#if !defined(_WIN32) && !defined(MACOSX) && !defined(CORE_SDL)

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include <signal.h>

#include "darkplaces.h"



// =======================================================================
// General routines
// =======================================================================

void Sys_Shutdown (void)
{
	fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NONBLOCK);

	fflush(stdout);
}

/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_Error (const char *error, ...)
{
	va_list argptr;
	char text[MAX_INPUTLINE_16384];

// change stdin to non blocking
	fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NONBLOCK);

	va_start (argptr,error);
	dpvsnprintf (text, sizeof (text), error, argptr);
	va_end (argptr);

	Con_PrintLinef (CON_ERROR "Engine Error: %s", text);

	//Host_Shutdown ();
	exit (1);
}

void Sys_PrintToTerminal(const char *text)
{
	if(sys.outfd < 0)
		return;
	// BUG: for some reason, NDELAY also affects stdout (1) when used on stdin (0).
	// this is because both go to /dev/tty by default!
	{
		int origflags = fcntl (sys.outfd, F_GETFL, 0);
		fcntl (sys.outfd, F_SETFL, origflags & ~O_NONBLOCK);

		while(*text)
		{
			fs_offset_t written = (fs_offset_t)write(sys.outfd, text, (int)strlen(text));
			if(written <= 0)
				break; // sorry, I cannot do anything about this error - without an output
			text += written;
		}
		fcntl (sys.outfd, F_SETFL, origflags);
	}
	//fprintf(stdout, "%s", text);
}

char *Sys_ConsoleInput(void)
{
	static char text[MAX_INPUTLINE_16384];
	static unsigned int len = 0;
	fd_set fdset;
	struct timeval timeout;
	FD_ZERO(&fdset);
	FD_SET(0, &fdset); // stdin
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select (1, &fdset, NULL, NULL, &timeout) != -1 && FD_ISSET(0, &fdset))
	{
		len = read (0, text, sizeof(text) - 1);
		if (len >= 1)
		{
			// rip off the \n and terminate
			// div0: WHY? console code can deal with \n just fine
			// this caused problems with pasting stuff into a terminal window
			// so, not ripping off the \n, but STILL keeping a NUL terminator
			text[len] = 0;
			return text;
		}
	}

	return NULL;
}

// Returns 1 on success, 0 on failure
qbool Sys_Clipboard_Set_Text_Is_Ok(const char *text_to_clipboard)
{
	return false; // Dedicated server, this fails
}

char *Sys_Clipboard_Get_Data_Limited_ZAlloc (void)
{
	return NULL;
}

int main (int argc, char **argv)
{
	signal(SIGFPE, SIG_IGN);
	sys.selffd = -1;
	sys.argc = argc;
	sys.argv = (const char **)argv;
	Sys_ProvideSelfFD();

	// COMMANDLINEOPTION: sdl: -noterminal disables console output on stdout
	if(Sys_CheckParm("-noterminal"))
		sys.outfd = -1;
	// COMMANDLINEOPTION: sdl: -stderr moves console output to stderr
	else if(Sys_CheckParm("-stderr"))
		sys.outfd = 2;
	else
		sys.outfd = 1;

	fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) | O_NONBLOCK);

	// used by everything
	Memory_Init();

	Host_Main();

	Sys_Quit(0);

	return 0;
}

qbool sys_supportsdlgetticks = false;
unsigned int Sys_SDL_GetTicks (void)
{
	Sys_Error("Called Sys_SDL_GetTicks on non-SDL target");
	return 0;
}
void Sys_SDL_Delay (unsigned int milliseconds)
{
	Sys_Error("Called Sys_SDL_Delay on non-SDL target");
}


qbool Sys_ShellExecute_Wait (ccs *s_exename, ccs *s_args_or_null, ccs *working_directory_url_unix)
{
	//va_super (s_cmdline, 1024, "./%s %s", s_exename, s_args_or_null);
	va_super (s_cmdline, 1024, "%s %s", s_exename, s_args_or_null);
	Con_PrintLinef ("About to run = %s", s_cmdline);
	int status_code = system(s_cmdline); //"./foo 1 2 3");
	Con_PrintLinef ("Status code = %d", status_code);
	return status_code != -1; //

	//system return value ...
	//RETURN VALUE The value returned is -1 on error (e.g. fork(2) failed), and the return status of the
	//	command otherwise. This latter return status is in the format specified in wait(2). Thus, the
	//	exit code of the command will be WEXITSTATUS(status). In case /bin/sh could not be executed,
	//	the exit status will be that of a command that does exit(127). If the value of command is NULL,
	//	system() returns nonzero if the shell is available, and zero if not.
}

char *Sys_Clipboard_Get_Text_ZAlloc (void) // UNIX
{
	return NULL;
}

qbool Sys_Clipboard_Set_Image_BGRA_Is_Ok (const unsigned *bgra, int width, int height)
{
	return false;
}

#endif  // !CORE_SDL
