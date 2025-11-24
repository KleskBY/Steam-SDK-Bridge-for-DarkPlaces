// fs_unsafe.h



//
// UNSAFE
//

char *FS_Get_RealFile_Realpath_Safe_ZAlloc (ccs *filename); // Safe to unsafe.

qbool FS_File_Exists_Unsafe (ccs *realpath);
qbool FS_Directory_Exists_Unsafe (ccs *realpath);
qbool FS_File_Or_Directory_Exists_Unsafe (ccs *realpath);

qbool FS_Folder_Open_Is_Ok_Unsafe (ccs *realpath);
char *FS_Getcwd_No_Trailing_Slash_Unsafe_TempAlloc (void);
char *FS_File_Binary_URL_Unsafe_ZAlloc (void);

char *FS_OpenDialog_Unsafe_ZAlloc (void);

char *FS_LoadFileString_Unsafe_ZAlloc (ccs *realpath);
char *FS_File_To_String_Unsafe_Ex_ZAlloc (ccs *realpath, fs_offset_t *filesizepointer);
byte *FS_File_To_Bytes_Unsafe_ZAlloc (ccs *realpath, fs_offset_t *filesizepointer);

byte *FS_File_To_Bytes_Unsafe_Malloc (ccs *realpath, fs_offset_t *pBytesread); // Can alloc before file memory pool

qbool FS_SaveFileString_Is_Ok_Unsafe (ccs *realpath, ccs *text);
qbool FS_Bytes_To_File_With_Size_Is_Ok_Unsafe (ccs *realpath, const byte *data, fs_offset_t datalen);

double FS_File_DateTime_Unsafe (ccs *realpath);
qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Unsafe (ccs *realpath, char *timestring_buf, size_t timestring_size);
//qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Safe (ccs *filename, char *timestring_buf, size_t timestring_size);

//// Baker: If successful, prealpathname_zalloc is set to the real path used
//// We use this to allow us to check date and time of save files to see how old they are
//// for the user
//
//void FS_CreatePath_Unsafe (ccs *realpath);
//qfile_t *FS_SysOpen_Unsafe (ccs *realpath, ccs *mode, qbool nonblocking); // uses absolute path
//
//
//

//
//// Baker: This one so we can use before initializing filesystem for zircon_command_line
//byte *FS_File_To_Bytes_Unsafe_ZAlloc (ccs *realpath, fs_offset_t *filesizepointer); // UNSAFE (because absolute path)
//byte *FS_File_To_Bytes_Unsafe_Malloc (ccs *realpath, fs_offset_t *pBytesread);
//
//
//qbool FS_SaveFileString_Is_Ok_Unsafe (ccs *realpath, ccs *text);
//double FS_File_DateTime_Unsafe (ccs *realpath);
//char *FS_File_Binary_URL_Unsafe_ZAlloc (void);
//qbool FS_Bytes_To_File_With_Size_Is_Ok_Unsafe (ccs *realpath, byte *data, fs_offset_t datalen);
//
//// Baker: Should get Unix time since 1970 from filename and fill in HH MM DD
//qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Safe (ccs *filename, char *timestring, size_t timestring_size);
//
////qbool FS_Is_Existing_File_Unsafe (ccs *absoluteunsafepath); // Returns false if does not exist or if is directory
//FS_FILETYPE_e FS_File_Or_Dir_Exists_Unsafe (ccs *realpath);
//qbool FS_File_Only_Exists_Unsafe (ccs *realpath);
//qbool FS_Directory_Exists_Unsafe (ccs *realpath);
//
///// Look for a file in the filesystem only
//int FS_SysFileOrDirectoryType_Unsafe (ccs *realpath);		// only look for files outside of packages
//
//qfile_t *FS_OpenRealFile_Unsafe (ccs *realpath, ccs *mode /*rb or what not*/);
