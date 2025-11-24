// fs_unsafe.c.h
// Baker: These do not use searchpath


qbool FS_File_Exists_Unsafe (ccs *realpath) // Returns false if does not exist or if is directory
{
	FS_FILETYPE_e ftype = FS_SysFileOrDirectoryType (realpath);
	return ftype == FS_FILETYPE_FILE_1;
}

// If either a file or directory by this name exists, return true.
qbool FS_File_Or_Directory_Exists_Unsafe (ccs *realpath) // Returns false if does not exist or if is directory
{
	FS_FILETYPE_e ftype = FS_SysFileOrDirectoryType (realpath);
	return ftype != FS_FILETYPE_NONE_0;
}

qbool FS_Directory_Exists_Unsafe (ccs *realpath) // Returns false if does not exist or if is directory
{
	FS_FILETYPE_e ftype = FS_SysFileOrDirectoryType (realpath);
	return ftype == FS_FILETYPE_DIRECTORY_2;
}

	//char *extensions_gif_all_files =
	//	"GIF Files(.gif)|*.gif|"
	//	"All Files(.)|*.*";
#ifdef _WIN32
#include "utf8lib.h"
#include <Commdlg.h> // Never needed this before?

char *FS_OpenDialog_With_Path_Extensions_Unsafe_ZAlloc (ccs *realpath, ccs *my_piped_extensions)
{
	va_super (windows_realpath, MAX_OSPATH_EX_1024, "%s", realpath);
	File_URL_Edit_SlashesBack_Like_Windows (windows_realpath);
	File_URL_Remove_Trailing_Windows_Slash (windows_realpath);


	wchar_t FilenameW[MAX_OSPATH_EX_1024] = {0};
	wchar_t FilterSpecW[MAX_OSPATH_EX_1024] = {0};
	wchar_t StartDirectoryW[MAX_OSPATH_EX_1024] = {0};
	Windows_String_Wide_Piper (my_piped_extensions, (byte *)FilterSpecW, sizeof(FilterSpecW));
	//char FilterSpec[] ="\0*.obj\0Text Files(.txt)\0.txt\0All Files(.)\0*.*\0";
	OPENFILENAMEW ofn = {0};

	WIDE(windows_realpath,StartDirectoryW);

#if 0
	stringlist_t list_bytedump = {0};
	stringlist_bytedump (&list_bytedump, (byte *)FilterSpecWW, sizeof(FilterSpecWW));
	stringlist_condump_with_linenums (&list_bytedump);
	stringlistfreecontents (&list_bytedump);
#endif

	ofn.lStructSize  = sizeof(ofn);
	ofn.hwndOwner    = NULL;
	ofn.lpstrInitialDir = StartDirectoryW;  // starting folder
	ofn.lpstrFilter	 = FilterSpecW; // L"Object Files(.obj)\0*.obj\0Text Files(.txt)\0.txt\0All Files(.)\0*.*\0";
	ofn.lpstrFile    = FilenameW;
	ofn.nMaxFile     = (unsigned int)1000000;
	ofn.lpstrTitle   = L"Select a File";
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (!GetOpenFileNameW(&ofn)) {
		return NULL;
	}

	// How to iterate through multiple results ...

	char	wtf8filename[MAX_OSPATH * 4];
	NARROW (FilenameW, wtf8filename);

	Con_PrintLinef ("Folder for results: %s", wtf8filename);

	// Windows uses UTF-16 encoding for wchar_t,

	// Baker: Operate under the assumption double null terminated.
	// Stage 1: Find that double null termination.
	//stringlist_t list = {0};

	wchar_t *ptrW = ofn.lpstrFile;

	ptrW[ofn.nFileOffset-1] = 0;
	ptrW += ofn.nFileOffset;
	int count = 0;
	while (*ptrW) {
		char swtf8[MAX_OSPATH_EX_1024];
		//std::wcout << L"File: " << ptr << std::endl;
		NARROW (ptrW, swtf8);
		Con_PrintLinef ("%4d: %s", count, swtf8);
		ptrW += (lstrlenW(ptrW)+1);

		count ++;
	}

	return (char *)Z_StrDup (wtf8filename);
}
#else
char *FS_OpenDialog_With_Path_Extensions_Unsafe_ZAlloc (ccs *realpath, ccs *my_piped_extensions)
{
	return NULL;
}
#endif // !WIN32


#ifdef _WIN32
#include "utf8lib.h"
#include <Commdlg.h> // Never needed this before?

char *FS_OpenDialog_Unsafe_ZAlloc (void)
{
	wchar_t FilenameW[MAX_OSPATH] = {0};
	char *piper =
		"Object Files(.obj)|*.obj|"
		"Text Files(.txt)|*.txt|"
		"All Files(.)|*.*";

	wchar_t FilterSpecW[MAX_OSPATH] = {0};
	Windows_String_Wide_Piper (piper, (byte *)FilterSpecW, sizeof(FilterSpecW));
	//char FilterSpec[] ="\0*.obj\0Text Files(.txt)\0.txt\0All Files(.)\0*.*\0";
	OPENFILENAMEW ofn = {0};

#if 0
	stringlist_t list_bytedump = {0};
	stringlist_bytedump (&list_bytedump, (byte *)FilterSpecWW, sizeof(FilterSpecWW));
	stringlist_condump_with_linenums (&list_bytedump);
	stringlistfreecontents (&list_bytedump);
#endif

	ofn.lStructSize  = sizeof(ofn);
	ofn.hwndOwner    = NULL;
	ofn.lpstrInitialDir = // starting folder
	ofn.lpstrFilter	 = FilterSpecW; //FilterSpecW; //L"Object Files(.obj)\0*.obj\0Text Files(.txt)\0.txt\0All Files(.)\0*.*\0";
	ofn.lpstrFile    = FilenameW;
	ofn.nMaxFile     = (unsigned int)1000000;
	ofn.lpstrTitle   = L"Select a File";
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (!GetOpenFileNameW(&ofn)) {
		return NULL;
	}

	// How to iterate through multiple results ...

	char	wtf8filename[MAX_OSPATH * 4];
	NARROW (FilenameW, wtf8filename);

	// Baker: Operate under the assumption double null terminated.
	// Stage 1: Find that double null termination.


//wchar_t* ptr = ofn.lpstrFile;
//ptr[ofn.nFileOffset-1] = 0;
//std::wcout << L"Directory: " << ptr << std::endl;
//ptr += ofn.nFileOffset;
//while (*ptr)
//{
//    std::wcout << L"File: " << ptr << std::endl;
//    ptr += (lstrlenW(ptr)+1);
//}

	return (char *)Z_StrDup (wtf8filename);
}
#else
char *FS_OpenDialog_Unsafe_ZAlloc (void)
{
	return NULL;
}
#endif // !WIN32

// Baker r3102: "folder" command
#ifdef _WIN32
#include "Shellapi.h" // Never needed this before?

qbool __Sys_Folder_Open_Folder_Must_Exist (ccs *realpath_)
{
	char realpath_windows_style[MAX_OSPATH_EX_1024];
	c_strlcpy (realpath_windows_style, realpath_);
	File_URL_Edit_SlashesBack_Like_Windows (realpath_windows_style);

	wchar filepathw[MAX_OSPATH_EX_1024] = {0};
	WIDE(realpath_windows_style, filepathw);

	int ret = (uint64_t) ShellExecuteW(0, L"Open", L"explorer.exe", filepathw, NULL, SW_NORMAL);
	qbool isok = (ret > 32);
	return isok;
}
#else // !_WIN32

// Folder must exist.  It must be a folder.
qbool __Sys_Folder_Open_Folder_Must_Exist (const char *realpath)
{
#ifdef CORE_XCODE // MACOSX
    int _Shell_Folder_Open_Folder_Must_Exist (const char *realpath);
    return _Shell_Folder_Open_Folder_Must_Exist(realpath);
#else
	//  xdg-open is a desktop-independent tool for configuring the default applications of a user
	if ( fork() == 0) {
		execl ("/usr/bin/xdg-open", "xdg-open", realpath, (char *)0);
		//cleanup_on_exit();  /* clean up before exiting */
		exit(3);
	}
#endif // LINUX etc
	return false;
}
#endif // _WIN32


// Trailing slash or without, don't matter.
qbool FS_Folder_Open_Is_Ok_Unsafe (const char *realpath)
{
	// Check if folder?
	char realpath_no_trailing_slash[MAX_OSPATH_EX_1024];
	c_strlcpy (realpath_no_trailing_slash, realpath);
	File_URL_Remove_Trailing_Unix_Slash (realpath_no_trailing_slash);

	if (FS_Directory_Exists_Unsafe (realpath_no_trailing_slash) == false) {
		//logd ("Folder " QUOTED_S " does not exist to show", path_to_file);
		return false;
	}

	return __Sys_Folder_Open_Folder_Must_Exist (realpath_no_trailing_slash);
}

#ifdef _WIN32
// empty
char *FS_Getcwd_No_Trailing_Slash_Unsafe_TempAlloc (void)
{
	char workingdirwtf8[MAX_OSPATH_EX_1024];
	wchar workingdirW[MAX_OSPATH_EX_1024];

	if (!GetCurrentDirectoryW (sizeof(workingdirW), workingdirW)) // No trailing slash in this or getcwd
		return NULL;

	NARROW (workingdirW, workingdirwtf8);

	if (workingdirwtf8[strlen(workingdirwtf8)-1] == '/')
		workingdirwtf8[strlen(workingdirwtf8)-1] = 0;

	File_URL_Edit_SlashesForward_Like_Unix (workingdirwtf8);

	return Mem_strdup(tempmempool, workingdirwtf8); // CMDCVARMEM but why?
}
#else
char *FS_Getcwd_No_Trailing_Slash_Unsafe_TempAlloc (void)
{
	char workingdir[MAX_OSPATH_EX_1024] = {0};

	if (getcwd (workingdir, sizeof(workingdir) - 1))
		return Mem_strdup(tempmempool, workingdir); // CMDCVARMEM but why?

	return NULL;
}
#endif // !_WIN32

byte *FS_File_To_Bytes_Unsafe_ZAlloc (ccs *realpath, fs_offset_t *filesizepointer)
{
	byte *buf = NULL;
	fs_offset_t filesize = 0;

	qfile_t *f = FS_SysOpen (realpath, FS_MODE_READ_BINARY_RB, fs_nonblocking_false);

	if (f) {
		if (filesize < 0) {
			FS_Close(f);
			return NULL;
		}

		filesize = f->real_length;

		buf = (byte *)Mem_Alloc (zonemempool, filesize + 1);
		buf[filesize] = '\0';
		FS_Read (f, buf, filesize);
		FS_Close (f);
//		if (developer_loadingfile_fs.integer)
//			Con_PrintLinef ("loaded file " QUOTED_S " (%u bytes)", path, (unsigned int)filesize);
	}

	if (filesizepointer)
		*filesizepointer = filesize;
	return buf;
}


char *FS_File_To_String_Unsafe_Ex_ZAlloc (ccs *realpath, fs_offset_t *filesizepointer)
{
	return (char *)FS_File_To_Bytes_Unsafe_ZAlloc (realpath, filesizepointer);
}



char *FS_LoadFileString_Unsafe_ZAlloc (ccs *realpath)
{
	fs_offset_t filesizepointer = 0;
	return (char *)FS_File_To_String_Unsafe_Ex_ZAlloc (realpath, &filesizepointer);
}

byte *FS_File_To_Bytes_Unsafe_Malloc (ccs *realpath, fs_offset_t *pBytesread)
{
	FILE *f = NULL;
	if (pBytesread)
		(*pBytesread) = 0;

	#ifdef _WIN32
		wchar_t filenameW[MAX_OSPATH_EX_1024];
		WIDE(realpath, filenameW);
		f = _wfopen(filenameW, L"rb");
	#else
		//handle = open (filepath, mod | opt, 0666);
		f = fopen (realpath, "rb");
	#endif

	if (f == NULL) {
		return NULL;
	}

	// Get the file size
	fseek		(f, 0, SEEK_END);
	fs_offset_t file_size = ftell(f);
	rewind		(f);

	// Allocate memory for the buffer, including space for null terminator
	byte *buffer_malloc = (byte *)calloc(1, file_size + ONE_CHAR_1);

	// Read the file content into the buffer
	size_t read_count = fread(buffer_malloc, ONE_CHAR_1, file_size, f);

	buffer_malloc[read_count] = 0;

	if (pBytesread)
		(*pBytesread) = read_count;

	fclose(f);

	return buffer_malloc;
}

#ifdef _WIN32
char *FS_File_Binary_URL_Unsafe_ZAlloc (void)
{
	wchar pathw[MAX_OSPATH] = {0};
    int result = GetModuleFileNameW(NULL, pathw, MAX_OSPATH - 2); // The size of the lpFilename buffer, in WCHARs. Baker: -2 so last character cannot be non-null

    if (result == 0) {
		return NULL; // Error getting module file name.
	}

	char pathwtf8[MAX_OSPATH];
	NARROW (pathw, pathwtf8);

	File_URL_Edit_SlashesForward_Like_Unix (pathwtf8);

	return Z_StrDup (pathwtf8);
}
#else
// Linux ... ?
char *FS_File_Binary_URL_Unsafe_ZAlloc (void)
{
	char binary_url[MAX_OSPATH];

	pid_t pid = getpid();
	int length;

	char linkname[MAX_OSPATH];
	c_dpsnprintf1 (linkname, "/proc/%d/exe", pid);

	length = readlink (linkname, binary_url, sizeof(binary_url)-1);

	// In case of an error, leave the handling up to the caller
	if (length == -1 || length >= (int)sizeof(binary_url) )
		return NULL;

	binary_url[length] = 0;

	return Z_StrDup (binary_url);
}

#endif


qbool FS_Bytes_To_File_With_Size_Is_Ok_Unsafe (ccs *realpath, const byte *data, fs_offset_t datalen)
{
	qfile_t *f = FS_SysOpen(realpath, "wb", fs_nonblocking_false); // rb to handle unix line endings on windows too

	if (!f)
		return false;

	FS_Write (f, data, datalen); // uses size_t
	FS_Close(f);
	return true;
}


// Baker: This tries userdir (as gamedir) followed by basedir
/*SAFE*/ char *FS_Get_RealFile_Realpath_Safe_ZAlloc (ccs *filename)
{
	char *realpathname_zalloc = NULL;
	qfile_t	*f = FS_OpenRealFileReadBinary (filename, &realpathname_zalloc);
	if (!f) {
		// Con_PrintLinef ("Could not FS_OpenRealFile %s", filename);
		return NULL;
	}

	FS_CloseNULL_ (f); // We don't need it open
	return realpathname_zalloc;
}

qbool FS_SaveFileString_Is_Ok_Unsafe (ccs *realpath, ccs *text)
{
	fs_offset_t text_slen = strlen(text);
	int isok = FS_Bytes_To_File_With_Size_Is_Ok_Unsafe (realpath, (const byte *)text, text_slen);
	return isok;
}

qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Safe (ccs *filename, char *timestring_buf, size_t timestring_size)
{
	char *realpath_za = FS_Get_RealFile_Realpath_Safe_ZAlloc (filename);
	if (!realpath_za)
		return false; // Apparently does not exist.

	qbool isok = FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Unsafe (realpath_za, timestring_buf, timestring_size);

	Mem_FreeNull_ (realpath_za);
	return isok;
}

// Returns file creation date time in seconds since midnight 1970
double FS_File_DateTime_Unsafe (ccs *realpath)
{
#ifdef _WIN32
   struct  _stat64 st;

	WPATHDEF(filenameW);
	WIDE(realpath, filenameW);
    if (_wstat64(filenameW, &st) == 0) {
		return (double)st.st_mtime;
    } else {
		return 0;
    }

#else
	struct stat st_buf = {0};

	int status = stat (realpath, &st_buf );
	if (status != 0)
		return 0;
	return (double)st_buf.st_mtime;
#endif

}


// "02/02/2024 10:12 AM"

//qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Safe (ccs *filename, char *timestring_buf, size_t timestring_size)
//{
//	va_super (realpath, MAX_OSPATH_EX_1024, "%s%s", fs_gamedir);// *fs_userdir ? fs_userdir : fs_basedir, filename);
//	return FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Unsafe(realpath, timestring_buf, timestring_size);
//}

qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Unsafe (ccs *realpath, char *timestring_buf, size_t timestring_size)
{
	//char *realpathname_zalloc = NULL;
#if 0
	// Fill menu
	char realfilename[MAX_OSPATH_EX_1024];
	//ccs *x_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1; // Con_PrintVarString (x_gamedir);
	c_strlcpy (realfilename, fs_basedir); // Has trail, right?
	c_strlcat (realfilename, x_gamedir); // No trail
	c_strlcat (realfilename, "/");
    c_strlcat (realfilename, filename);
	File_URL_Edit_SlashesForward_Like_Unix (realfilename);
#endif

	double sav_filetime_since1970 = FS_File_DateTime_Unsafe(realpath);

	if (sav_filetime_since1970 == 0) {
		timestring_buf[0] = NULL_CHAR_0;
		return false;
	}

	time_t		rawtime			= (time_t)(double)sav_filetime_since1970;
	struct tm	*tmx			= localtime(&rawtime);

	//Mem_FreeNull_ (realpathname_zalloc);

	//File_Time (
	//// "20241005 10:12 AM"
	//time_t crt_time;
	//time (&crt_time);

//#if _MSC_VER >= 1400
//	struct tm crt_tm;
//#else
//	struct tm *crt_tm;
//#endif

	#if _MSC_VER >= 1400 // 1400 is Visual Studio 2005, btw ...
		//localtime_s (&crt_tm, &crt_time);
		strftime (timestring_buf, timestring_size, "%Y/%m/%d %I:%M %p", tmx);
	#else
		//crt_tm = localtime (&crt_time);
		strftime (timestring_buf, timestring_size, "%Y/%m/%d %I:%M %p", tmx);
	#endif

	return true;
}
