#ifdef CORE_SDL

#ifdef _WIN32
	#include <io.h> // Include this BEFORE darkplaces.h because it uses strncpy which trips DP_STATIC_ASSERT
	#include "conio.h"
#else
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/time.h>
#endif // ! WIN32

#ifdef __ANDROID__
	#include <android/log.h>
#endif // __ANDROID__

#include <signal.h>

// Include this BEFORE darkplaces.h because it breaks wrapping
// _Static_assert. Cloudwalk has no idea how or why so don't ask.

#if defined(CODEBLOCKS_LINUX_IDE) || (defined(_MSC_VER) && _MSC_VER < 1900)
	#include <SDL2/SDL.h>
#else
	#include <SDL.h>
#endif // Certain IDEs

#include "darkplaces.h"


#ifdef _WIN32
	#ifdef _MSC_VER
		#pragma comment(lib, "sdl2.lib")
		#pragma comment(lib, "sdl2main.lib")

		#pragma comment(lib, "shell32.lib")
	#endif // _MSC_VER
#endif //  _WIN32


// =======================================================================
// General routines
// =======================================================================

void Sys_Shutdown (void)
{
#ifdef __ANDROID__
	Sys_AllowProfiling(false);
#endif
#ifndef _WIN32
	fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NONBLOCK);
#endif // _WIN32
	fflush(stdout);
	SDL_Quit();
}

static qbool nocrashdialog;
/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_Error (const char *error, ...) // Baker: This is really SysErrorLinef
{
	va_list argptr;
	char text[MAX_INPUTLINE_16384];

// change stdin to non blocking
#ifndef _WIN32
	fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NONBLOCK);
#endif // !_WIN32

	va_start (argptr,error);
	dpvsnprintf (text, sizeof (text), error, argptr);
	va_end (argptr);

	Con_PrintLinef (CON_ERROR "Engine Error: %s", text);

	if (!nocrashdialog)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Engine Error", text, NULL);

	//Host_Shutdown ();
	exit (1);
}


#ifdef _WIN32
#ifdef _DEBUG


// Baker: This will actually print to the debug console in Visual Studio
void DebugPrintLinef (const char *fmt, ...)
{
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	OutputDebugString (text);
	OutputDebugString (NEWLINE);
	VA_EXPAND_ALLOC_FREE (text);
}
#endif // _DEBUG
#endif // _WIN32


// Baker: Sys_PrintToTerminal use is rare and novel
void Sys_PrintToTerminal_Unix (ccs *text);

//qbool is_csg_tool = false;


void Sys_PrintToTerminal (ccs *text)
{
	#ifdef _WIN32
	#ifdef CONFIG_MENU
		Sys_PrintToTerminal_WinQuake (text); // WriteFile STDOUT
	#else
		Sys_PrintToTerminal_Win32 (text); // write(sys.outfd, text, (int)strlen(text))
	#endif
	#elif defined(__ANDROID__)
		#define CORE_ANDROID_LOG_TAG "CoreMain"
		__android_log_print(ANDROID_LOG_INFO, CORE_ANDROID_LOG_TAG, "%s", text);
	#else
		Sys_PrintToTerminal_Unix (text);
	#endif // !WIN32
}

char *Sys_ConsoleInput (void) // CONSOLUS
{
	static char text[MAX_INPUTLINE_16384];
	int len = 0;

#ifdef _WIN32
	int c;

	// read a line out
	while (_kbhit ()) {
		c = _getch ();
		_putch (c);

		if (c == '\r') {
			text[len] = 0;
			_putch ('\n');
			len = 0;
			return text;
		} // carriage return

		if (c == CHAR_BACKSPACE_8) {
			if (len) {
				_putch (' ');
				_putch (c);
				len--;
				text[len] = 0;
			}
			continue;
		} // backspace

		text[len] = c;
		len++;
		text[len] = 0;
		if (len == sizeof (text))
			len = 0;
	}
#else // !_WIN32
	fd_set fdset;
	struct timeval timeout;
	FD_ZERO(&fdset);
	FD_SET(0, &fdset); // stdin
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select (1, &fdset, NULL, NULL, &timeout) != -1 && FD_ISSET(0, &fdset))
	{
		len = read (0, text, sizeof(text));
		if (len >= 1)
		{
			// rip off the \n and terminate
			text[len-1] = 0;
			return text;
		}
	}
#endif // !_WIN32
	return NULL;
}


// Baker r8001: zircon_command_line.txt support
#define MAX_NUM_Q_ARGVS_50	50
static int fake_argc; char *fake_argv[MAX_NUM_Q_ARGVS_50];

int super_exit_code = 0;

int main (int argc, char *argv[])
{
	signal(SIGFPE, SIG_IGN);

#ifdef __ANDROID__
	Sys_AllowProfiling(true);
#endif

	sys.selffd = -1;
	sys.argc = argc;
	sys.argv = (const char **)argv;

	//
    // Baker: zircon_command_line.txt
    //

#ifdef CORE_XCODE // MACOSX
	// This is to make zircon_command_line.txt work for a Mac .app
    if (strstr(sys.argv[0], ".app/") && strstr(sys.argv[0], "/Xcode/") == NULL) {
        char *split;
        strlcpy(fs_basedir, com_argv[0], sizeof(fs_basedir));
        split = strstr(fs_basedir, ".app/");

        // Baker: find first '/' after .app, 0 it out
        while (split > fs_basedir && *split != '/')
            split--;
        *split = 0;

        // Change to dir of .app, should make zircon_command_line.txt work
        chdir (fs_basedir);
    } // if .app in name, should be 100%
#endif // XCODE

	if (sys.argc == 1 /*only the exe*/) {
		const char *s_fp =
#ifdef __ANDROID__
			"/sdcard/zircon/"
#endif
			"zircon_command_line.txt";
		RELATED_ (FS_Init_Dir) // csg executed there
		int as_csg_tool = Sys_CheckParm("-csg");

		while (as_csg_tool == false) {
			fs_offset_t bytes_read = 0;
			char *data_malloc = (char *)FS_File_To_Bytes_Unsafe_Malloc (s_fp, &bytes_read);
			if (!data_malloc)
				break;

			// Terminate it at a comment
			if (String_Contains (data_malloc, "//")) {
				char *s_start = (char *)strstr (data_malloc, "//");
				*s_start = 0; // null it out
			}

#ifdef _WIN32
			ccs *old_arg0 = sys.argv[0]; // Baker: Windows it is imperative that this is .EXE name for selffd
#endif
			va_super (cmdline_fake, MAX_INPUTLINE_16384, "quake_engine %s", data_malloc); // arg0 is engine and ignored by Sys_CheckParm
				String_Edit_Whitespace_To_Space (cmdline_fake); // Make tabs, newlines into spaces.
				String_Command_String_To_Argv (/*destructive edit*/ cmdline_fake, &fake_argc, fake_argv, MAX_NUM_Q_ARGVS_50);
				sys.argc = fake_argc;
				sys.argv = (const char **)fake_argv;

			freenull_ (data_malloc);
#ifdef _WIN32
			sys.argv[0] = old_arg0; // Baker: Windows it is imperative that this is .EXE name for selffd
#endif
			break;
		} // while
	} // if 1 cmd arg the exe

	//
    // End Baker: zircon_command_line.txt
    //

	// Sys_Error this early in startup might screw with automated
	// workflows or something if we show the dialog by default.
	nocrashdialog = true;

	Sys_ProvideSelfFD(); // Baker: windows is setting sys.selffd to 3 so we have a handle

	// COMMANDLINEOPTION: -nocrashdialog disables "Engine Error" crash dialog boxes
	if (!Sys_CheckParm("-nocrashdialog"))
		nocrashdialog = false;
	// COMMANDLINEOPTION: sdl: -noterminal disables console output on stdout
	if (Sys_CheckParm("-noterminal"))
		sys.outfd = -1;
	// COMMANDLINEOPTION: sdl: -stderr moves console output to stderr
	else if (Sys_CheckParm("-stderr"))
		sys.outfd = 2;
	else
		sys.outfd = 1;

#ifndef _WIN32
	fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) | O_NONBLOCK);
#endif // !_WIN32

	// we don't know which systems we'll want to init, yet...
	SDL_Init(0);

	// used by everything
	Memory_Init();

	Host_Main();
	Sys_Quit(0);

	return super_exit_code /*0*/;
}

qbool sys_supportsdlgetticks = true;
unsigned int Sys_SDL_GetTicks (void)
{
	return SDL_GetTicks();
}
void Sys_SDL_Delay (unsigned int milliseconds)
{
	SDL_Delay(milliseconds);
}

#include "utf8lib.h"

#include "sys_sdl_clipboard.c.h"
#include "sys_sdl_winquake.c.h"		// For "WinQuake" style server. c:\quake\zircon.exe -dedicated 8 +map start
#include "sys_sdl_process.c.h"

#endif // CORE_SDL

