// sys_sdl_process.c.h

#ifdef _WIN32
void showcd(wchar_t * caption)
{
    wchar_t buffer[512];

    if (GetCurrentDirectoryW(512, buffer) == 0)
    {
        DWORD err = GetLastError();
        MessageBoxW(NULL, L"GetCurrentDirectory failed", caption, MB_OK);
        ExitProcess(err);
    }

    buffer[511] = L'\0';
    MessageBoxW(NULL, buffer, caption, MB_OK);
}
#endif // _WIN32


// Returns NULL on failure
#ifdef _WIN32
sys_handle_t System_Process_Create_As_BatFile_Make_Log (
	ccs *path_to_file_unix, 
	ccs *args_not_unix_but_your_os, 
	/*optional*/ ccs *working_directory_url_unix, 
	ccs *path_to_log
)
{
	char	windows_path_to_file[MAX_OSPATH];
	char	windows_working_dir[MAX_OSPATH];

	RELATED_ (WIDE fromwtf8)
	// Sanity check
	if (FS_File_Exists_Unsafe (path_to_file_unix) == false) {
		// logd ("path_to_file_unix " QUOTED_S " doesn't exist", path_to_file_unix);
		return NULL;
	}

	if (working_directory_url_unix && FS_Directory_Exists_Unsafe (working_directory_url_unix) == false) {
		// logd ("working_directory_url_unix " QUOTED_S " doesn't exist", working_directory_url_unix);
		return NULL;
	}

	// 1. Construct windows version: dest: windows_path_to_file src: path_to_file_unix (supplied)
	c_strlcpy (windows_path_to_file, path_to_file_unix);
	File_URL_Edit_SlashesBack_Like_Windows (windows_path_to_file); // Need to "Windows-ize" for WIN32 call

	// If working directory specified, use that otherwise use binary's path as working dir
	if (working_directory_url_unix) {
		c_strlcpy (windows_working_dir, working_directory_url_unix); // So we can Windows-ize the path (c:/mypath --> c:\mypath)
		// UNIX slashes supplied by function
	}
	//else  // No working directory provided, working directory is the path of the windows_path_to_file
	//{
	//	c_strlcpy (windows_working_dir, windows_path_to_file);
	//	File_URL_Edit_Reduce_To_Parent_Path_NoTrailingSlash (windows_working_dir); // Strip off file name to parent path.
	//	File_URL_Edit_SlashesBack_Like_Windows (windows_working_dir);
	//	// Converted binary path to Unix Slashes
	//}

	File_URL_Remove_Trailing_Windows_Slash (windows_working_dir);

	{
		//STARTUPINFO si = {0};
		STARTUPINFOW si = {0};
		PROCESS_INFORMATION pi;
		char 		command_line[MAX_OSPATH_EX_1024];
		int ret;

		wchar_t commandlineW[MAX_OSPATH_EX_1024];
		WIDE (command_line, commandlineW);

		wchar_t workingdirW[MAX_OSPATH_EX_1024];
		WIDE (windows_working_dir, workingdirW);

		// Construct command line ...
		c_dpsnprintf2 (command_line, QUOTED_S " " "%s", windows_path_to_file, args_not_unix_but_your_os);

		//logd ("Process command line (native format) " QUOTED_S, command_line); // Get rid of this after testing
		
		si.cb = sizeof(si);
		si.wShowWindow = SW_HIDE /*SW_SHOWMINNOACTIVE*/;
		si.dwFlags = STARTF_USESHOWWINDOW;
		
		ret = CreateProcessW (
			NULL, 					//  _In_opt_     LPCTSTR lpApplicationName,
			commandlineW,			//  _Inout_opt_  LPTSTR lpCommandLine,
			NULL,					//  _In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
			NULL,					//  _In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
			FALSE,					//  _In_         BOOL bInheritHandles,
			GetPriorityClass(GetCurrentProcess()),
									//  _In_         DWORD dwCreationFlags,
			NULL,					//  _In_opt_     LPVOID lpEnvironment,
			working_directory_url_unix ? workingdirW : NULL,	//  _In_opt_     LPCTSTR lpCurrentDirectory,
			&si, 					//  _In_         LPSTARTUPINFO lpStartupInfo,
			&pi						//  _Out_        LPPROCESS_INFORMATION lpProcessInformation
		);

		if (ret) return pi.hProcess; // Return the handle
	}
	//logd ("Create process failed");
	return 0;
}
#else
// Not using the WIN32 version
#endif // !__WIN32

// Returns NULL on failure
#ifdef _WIN32
sys_handle_t System_Process_Create (ccs *path_to_file_unix, ccs *args_not_unix_but_your_os,  
 /*optional*/ ccs *working_directory_url_unix) // UNICODE WEAKNESS
{
	char	windows_path_to_file[MAX_OSPATH];
	char	windows_working_dir[MAX_OSPATH];

	RELATED_ (WIDE fromwtf8)
	// Sanity check
	if (FS_File_Exists_Unsafe (path_to_file_unix) == false) {
		// logd ("path_to_file_unix " QUOTED_S " doesn't exist", path_to_file_unix);
		return NULL;
	}

	if (working_directory_url_unix && FS_Directory_Exists_Unsafe (working_directory_url_unix) == false) {
		// logd ("working_directory_url_unix " QUOTED_S " doesn't exist", working_directory_url_unix);
		return NULL;
	}

	// 1. Construct windows version: dest: windows_path_to_file src: path_to_file_unix (supplied)
	c_strlcpy (windows_path_to_file, path_to_file_unix);
	File_URL_Edit_SlashesBack_Like_Windows (windows_path_to_file); // Need to "Windows-ize" for WIN32 call

	// If working directory specified, use that otherwise use binary's path as working dir
	if (working_directory_url_unix) {
		c_strlcpy (windows_working_dir, working_directory_url_unix); // So we can Windows-ize the path (c:/mypath --> c:\mypath)
		// UNIX slashes supplied by function
	}
	//else  // No working directory provided, working directory is the path of the windows_path_to_file
	//{
	//	c_strlcpy (windows_working_dir, windows_path_to_file);
	//	File_URL_Edit_Reduce_To_Parent_Path_NoTrailingSlash (windows_working_dir); // Strip off file name to parent path.
	//	File_URL_Edit_SlashesBack_Like_Windows (windows_working_dir);
	//	// Converted binary path to Unix Slashes
	//}

	if (String_Ends_With_Caseless (windows_working_dir, "/")) { // TRAILING SLASH REMOVER except we own windows_working_dir
		int slen = (int)strlen (windows_working_dir);
		if (slen <= (int)sizeof(windows_working_dir /*1024*/)) { // Otherwise it is truncated ... which is bad.
			windows_working_dir[slen - 1] = 0; // String len is 10, we null at 9 to reduce length to 8.
		}
	} // May 18 2020 - Remove trailing slash V2


	{
		STARTUPINFO si = {0};
		PROCESS_INFORMATION pi;
		char 		command_line[1024];
		int ret;

		// Construct command line ...
		c_dpsnprintf2 (command_line, QUOTED_S " " "%s", windows_path_to_file, args_not_unix_but_your_os);

		//logd ("Process command line (native format) " QUOTED_S, command_line); // Get rid of this after testing
		si.cb = sizeof(si);
		si.wShowWindow = SW_HIDE /*SW_SHOWMINNOACTIVE*/;
		si.dwFlags = STARTF_USESHOWWINDOW;
		
		ret = CreateProcess (
			NULL, 					//  _In_opt_     LPCTSTR lpApplicationName,
			command_line,			//  _Inout_opt_  LPTSTR lpCommandLine,
			NULL,					//  _In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
			NULL,					//  _In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
			FALSE,					//  _In_         BOOL bInheritHandles,
			GetPriorityClass(GetCurrentProcess()),
									//  _In_         DWORD dwCreationFlags,
			NULL,					//  _In_opt_     LPVOID lpEnvironment,
			working_directory_url_unix ? windows_working_dir : NULL,	//  _In_opt_     LPCTSTR lpCurrentDirectory,
			&si, 					//  _In_         LPSTARTUPINFO lpStartupInfo,
			&pi						//  _Out_        LPPROCESS_INFORMATION lpProcessInformation
		);

		if (ret) return pi.hProcess; // Return the handle
	}
	//logd ("Create process failed");
	return 0;
}
#else
sys_handle_t System_Process_Create (ccs *path_to_file_unix, ccs *args_not_unix_but_your_os,  
 /*optional*/ ccs *working_directory_url_unix) // UNICODE WEAKNESS
{
	return 0;
}
#endif // !_WIN32

// Don't give this a null pid
#ifdef _WIN32
int System_Process_Is_Still_Running_Neg1_Error (sys_handle_t pid, /*reply*/ int *p_exit_code)
{
	DWORD ExitCode;

	// DEBUG_ASSERT (pid);
	if (pid == NULL)
		return not_found_neg1;

	if (GetExitCodeProcess((HANDLE)pid, &ExitCode) == false) {
		return not_found_neg1; // Error
	}

	if (ExitCode == STILL_ACTIVE) // 0x00000103L
		return true; // Still running

	NOT_MISSING_ASSIGN (p_exit_code, ExitCode);

	return false; // No longer running; completed
}
#else
int System_Process_Is_Still_Running_Neg1_Error (sys_handle_t pid, /*reply*/ int *p_exit_code)
{
	return -1;
}
#endif // !WIN32

// Baker: I think this forces process termination specifying an exit code
// This is specific on Windows for console applications
// For Windows apps, use System_Process_Close_Did_Close -- I am guessing.
#ifdef _WIN32
qbool System_Process_Terminate_Did_Terminate_Console_App (sys_handle_t pid)
{
	if (TerminateProcess ((HANDLE)pid, 0))
		return true;

	return false;
}
#else
#endif // !_WIN32

#ifdef _WIN32
qbool System_Process_Close_Did_Close (sys_handle_t pid)
{
	// Send WM_CLOSE to ask process to quit
	if (CloseHandle ( (HANDLE)pid)) {
		return true;
	}

	return false;
}
#else
#endif

#ifdef _WIN32
#include <shellapi.h>
//HINSTANCE ShellExecuteA(
//  [in, optional] HWND   hwnd,
//  [in, optional] LPCSTR lpOperation,
//  [in]           LPCSTR lpFile,
//  [in, optional] LPCSTR lpParameters,
//  [in, optional] LPCSTR lpDirectory,
//  [in]           INT    nShowCmd
//);

// NONBLOCKING.  PROCESS WILL NOT COMPLETE.

// Baker: 
// THIS IS NOT USED.
// Reason: We need a process handle.  That's why.
RELATED_ (System_Process_Create <-- use that instead)
qbool Sys_ShellExecute_NonBlocking (ccs *s_exename_, ccs *s_args_or_null, ccs *working_directory_url_unix) // UNICODE WEAKNESS
{
	char s_exename[MAX_OSPATH_EX_1024];
	c_strlcpy (s_exename, s_exename_);
	File_URL_Edit_SlashesBack_Like_Windows (s_exename);

	ccs *windir = NULL;
	char _windir[MAX_OSPATH_EX_1024];
	if (working_directory_url_unix) {
		c_strlcpy (_windir, working_directory_url_unix);
		File_URL_Edit_SlashesBack_Like_Windows (_windir); // Need to "Windows-ize" for WIN32 call
		windir = &_windir[0];
	}

	size_t result = (size_t)ShellExecute (
		/*hwnd*/ NULL,		// Parent window
         NULL,				// NULL does "open"
         s_exename,			// C:\\WINDOWS\\System32\\CALC.EXE"
         s_args_or_null,	// Parameters
         windir,			// Working directory - "c:\quake\" - If this value is NULL, the current working directory is used
         SW_SHOWDEFAULT
	);

	// If the function succeeds, it returns a value greater than 32.
	return (result > 32);

}
#else
// We aren't even using the WIN32 version
#endif // !WIN32


// This is blocking version ...
#ifdef _WIN32
qbool Sys_ShellExecute_Wait (ccs *s_exename_, ccs *s_args_or_null, ccs *working_directory_url_unix) // UNICODE WEAKNESS
{
	ccs *windir = NULL;
	char _windir[MAX_OSPATH_EX_1024];
	if (working_directory_url_unix) {
		c_strlcpy (_windir, working_directory_url_unix);
		File_URL_Edit_SlashesBack_Like_Windows (_windir); // Need to "Windows-ize" for WIN32 call
		windir = &_windir[0];
	}

	char s_exename[MAX_OSPATH_EX_1024];
	c_strlcpy (s_exename, s_exename_);
	File_URL_Edit_SlashesBack_Like_Windows (s_exename);
//	size_t result = (size_t)ShellExecute (
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = s_exename;// "c:\\MyProgram.exe";        
	ShExecInfo.lpParameters = s_args_or_null; // "";   
	ShExecInfo.lpDirectory = windir;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL; 
	if (!ShellExecuteEx(&ShExecInfo))
		return false;
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	CloseHandle(ShExecInfo.hProcess);
	return true;
}
#else
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
#endif // !_WIN32




