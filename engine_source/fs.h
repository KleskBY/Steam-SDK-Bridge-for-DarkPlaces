// fs.h

/*
	DarkPlaces file system

	Copyright (C) 2003-2005 Mathieu Olivier

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA
*/

#ifndef FS_H
#define FS_H

#include <stddef.h>
#include <stdarg.h>
#include "qtypes.h"
#include "qdefs.h"
#include "zone.h"

// ------ Types ------ //

typedef struct qfile_s qfile_t;
typedef int64_t fs_offset_t;

#define FS_FILETYPE_NONE_0 0
#define FS_FILETYPE_FILE_1 1
#define FS_FILETYPE_DIRECTORY_2 2

#if 1 // Baker: July 11 2025 - Extra
#define FS_FILETYPE_VPACK_3 3
#define FS_FILETYPE_PACK_4 4
#endif

#define FS_FILETYPE_e int

// ------ Variables ------ //

extern char fs_gamedir [MAX_OSPATH];
extern char fs_basedir [MAX_OSPATH];
extern char fs_userdir [MAX_OSPATH];

extern int fs_data_override; // Baker r0009: Super -data override
extern int fs_is_zircon_galaxy; // Baker: Explain how we know this?
// zircon/gfx/qplaque.png

// list of active game directories (empty if not running a mod)
#define MAX_GAMEDIRS_16 16
extern int fs_numgamedirs;
extern char fs_gamedirs[MAX_GAMEDIRS_16][MAX_QPATH_128];

typedef struct vfs_s
{
	char gamedir[MAX_OSPATH];
	char basedir[MAX_OSPATH];
	char userdir[MAX_OSPATH];
	int numgamedirs;
	char gamedirs[MAX_GAMEDIRS_16][MAX_QPATH_128];
} vfs_t;

// ------ Main functions ------ //

// IMPORTANT: the file path is automatically prefixed by the current game directory for
// each file created by FS_WriteFile, or opened in "write" or "append" mode by FS_OpenRealFile

typedef struct {
	char	searchpathx[MAX_QPATH_128 * 2];
} loadinfo_s;


qbool FS_AddPack(ccs *pakfile, qbool *already_loaded, qbool keep_plain_dirs, qbool dlcache); // already_loaded may be NULL if caller does not care
ccs *FS_WhichPack(ccs *filename); // Baker: CURL, VM_whichpack, SV_Download_f, Curl_SendRequirements_SendRequirement use this
char *FS_FileWhich_Ta (ccs *filename, /*optional*/ int *p_type);
void FS_CreatePath (char *realpath);
int FS_SysOpenFD(ccs *realpath, ccs *mode, qbool nonblocking); // uses absolute path
qfile_t *FS_SysOpen (ccs *realpath, ccs *mode, qbool nonblocking); // uses absolute path

RELATED_ (FS_LoadFile)
/*GAMEDIR*/ qfile_t *FS_OpenRealFile (ccs *filename, ccs *mode, qbool quiet);

void FS_Path_Feed (stringlist_t *plist); // Baker: devinfo

// Baker: If successful, prealpathname_zalloc is set to the real path used
// We use this to allow us to check date and time of save files to see how old they are
// for the user
/*GAMEDIR OR BASEDIR/GAMEDIRNAME1 PREPENDED*/ qfile_t *FS_OpenRealFileReadBinary (ccs *filepath, char **prealpathname_zalloc);

qfile_t *FS_OpenVirtualFile (ccs *filepath, qbool quiet);
qfile_t *FS_FileFromData (const unsigned char *data, const size_t size, qbool quiet);
int FS_Close (qfile_t *file);
void FS_RemoveOnClose(qfile_t *file); // Baker: May 24 2025 - some server autodemo stuff
fs_offset_t FS_Write (qfile_t *file, const void *data, size_t datasize);
fs_offset_t FS_Read (qfile_t *file, void *buffer, size_t buffersize);
int FS_Print(qfile_t *file, ccs *msg);
int FS_Printf(qfile_t *file, ccs *fmt, ...) DP_FUNC_PRINTF(2);
int FS_PrintLinef(qfile_t *file, ccs *fmt, ...) DP_FUNC_PRINTF(2);
int FS_VPrintf(qfile_t *file, ccs *fmt, va_list ap);
int FS_Getc (qfile_t *file);
int FS_UnGetc (qfile_t *file, unsigned char c);
int FS_Seek (qfile_t *file, fs_offset_t offset, int whence);
fs_offset_t FS_Tell (qfile_t *file);
fs_offset_t FS_FileSize (qfile_t *file);
void FS_Purge (qfile_t *file);
ccs *FS_FileWithoutPath (ccs *in);
ccs *FS_FileExtension (ccs *in);
int FS_CheckNastyPath (ccs *path, qbool isgamedir);
void FS_SanitizePath (char *path);

extern ccs *const fs_checkgamedir_missing; // "(missing)"
ccs *FS_CheckGameDir(ccs *gamedir); // returns NULL if nasty, fs_checkgamedir_missing (exact pointer) if missing

typedef struct
{
	char name[MAX_OSPATH];
	char description[8192];
}
gamedir_t;
extern gamedir_t *fs_all_gamedirs; // terminated by entry with empty name
extern int fs_all_gamedirs_count;

qbool FS_ChangeGameDirs(int numgamedirs, char gamedirs[][MAX_QPATH_128], qbool complain, qbool failmissing);
qbool FS_IsRegisteredQuakePack(ccs *name);

int FS_CRCFile(ccs *filename, size_t *filesizepointer);
void FS_UnloadPacks_dlcache(void);
void FS_Rescan(void);

RELATED_ (stringlist_t) // Different from fssearch_s in structure
typedef struct fssearch_s
{
	int numfilenames;
	char **filenames;
	// array of filenames
	char *filenamesbuffer;
} __fssearch_t;

// Baker +int isgamedironly
fssearch_t *FS_Search(ccs *pattern, int caseinsensitive, int quiet, ccs *packfile, int isgamedironly);

#if 1 // Baker: My rarely used macros
void FS_FreeSearch(fssearch_t *search);
#define FS_FreeSearch_Null_(t) \
	if (t) { \
		FS_FreeSearch(t); \
		t = NULL; \
	} // Ender

#define FS_LoadFile_Quiet_Temp(path) \
	(char *)FS_LoadFile(path, tempmempool, fs_quiet_FALSE, fs_size_ptr_null)
#endif // Baker: My rarely used macros


byte *FS_LoadFile (ccs *path, mempool_t *pool, qbool quiet, fs_offset_t *filesizepointer);

#define FS_LoadFileQuickSize(s_filename, addrofsize) (char *)FS_LoadFile (s_filename, tempmempool, fs_quiet_true, addrofsize) // file_to_string FS_File_To_String
#define FS_LoadFileQuick(s_filename) (char *)FS_LoadFile (s_filename, tempmempool, fs_quiet_true, fs_size_ptr_null) // file_to_string

qbool FS_WriteFileQuick (ccs *filename, ccs *data, size_t datalen); // FS_File_To_String string_to_file
qbool FS_WriteFileStringQuick (ccs *filename, ccs *text); // FS_File_To_String string_to_file String_To_File


byte *FS_SysLoadFile (ccs *realpath, mempool_t *pool, qbool quiet, fs_offset_t *filesizepointer);
qbool FS_WriteFileInBlocks (ccs *filename, const void *const *data, const fs_offset_t *len, size_t count);
qbool FS_WriteFile (ccs *filename, const void *data, fs_offset_t len);

qbool FS_chdir (ccs *realpath);


// ------ Other functions ------ //

void FS_StripExtension (ccs *in, char *out, size_t size_out);
void FS_DefaultExtension (char *path, ccs *extension, size_t size_path);

#define FS_FILETYPE_NONE_0 0
#define FS_FILETYPE_FILE_1 1
#define FS_FILETYPE_DIRECTORY_2 2
/// Look for a file in the packages and in the filesystem
FS_FILETYPE_e FS_FileOrDirectoryType (ccs *filename);
/// Look for a file in the filesystem only
FS_FILETYPE_e FS_SysFileOrDirectoryType (ccs *realpath);		// only look for files outside of packages

/// Returns its canonical name (same case as used in the pack) if found, else NULL.
/// If the file is found outside a pak, this will be the same pointer as passed in.
#if 0 // LATER
	#define FS_FileExists(s) (!!FS_FileExistsGetCanonicalName(s))
	ccs *FS_FileExistsGetCanonicalName (ccs *filename);
	/// Look for a file in the filesystem only
#endif // 0

/// Look for a file in the packages and in the filesystem
qbool FS_FileExists (ccs *filename);		// the file can be into a package
#define FS_File_Exists(filename) FS_FileExists(filename)
qbool FS_SysFileExists (ccs *filename);	// only look for files outside of packages

qbool FS_File_TimeString_Fill_MM_DD_YY_HH_MM_AM_Safe (ccs *filename, char *timestring_buf, size_t timestring_size);

unsigned char *FS_Deflate(const unsigned char *data, size_t size, size_t *deflated_size, int level, mempool_t *mempool);
unsigned char *FS_Inflate(const unsigned char *data, size_t size, size_t *inflated_size, mempool_t *mempool);

qbool FS_HasZlib(void);

void FS_Init_SelfPack(void);
void FS_InitOnce(void);
void FS_Shutdown(void);
void FS_InitOnce_Commands(void);

extern int fs_have_qex;

#define FS_MODE_WRITE_TEXT_W_DO_NOT_USE			"w" // We don't want to use this.  We want binary, text mode may mess with the newlines.
#define FS_MODE_WRITE_BINARY_WB					"wb"
#define FS_MODE_APPEND_BINARY_AB				"ab"
#define FS_MODE_READ_BINARY_RB					"rb"
#define FS_MODE_READ_AND_WRITE_BINARY_R_PLUS_B	"r+b"


// Baker: Flex_Writef writes to either file *f or strcats to a higher performance "baker_string_t"
// that supports much faster string concatentation
void Flex_Writef (ccs *fmt, ...) DP_FUNC_PRINTF(1);


char *FS_RealFilePath_Z_Alloc (ccs *s_quake_file); // returns NULL if not a real file or not found

extern char mod_list_folder_name[1024];			// modlist.txt .. 
extern char mod_list_game_window_title[1024];		// modlist.txt
extern char mod_list_server_filter_name[1024];	// modlist.txt
extern char *mod_list_game_icon_base64_zalloc;	// Permanent!

extern int	mod_list_requires;				// g_requires_quake 0 = YES, 1 = STANDAALONE, 2 = FLEXIBLE.

#define REQUIRES_WHAT_QUAKE_0		0		// DEFAULT
#define REQUIRES_WHAT_STANDALONE_1	1
#define REQUIRES_WHAT_FLEXIBLE_2	2

extern stringlist_t baker_gamelist_names_ignore_char1; // char1 is our enum

void String_Worldspawn_Values_stringlistappend (stringlist_t *plist, ccs *s_entities_string);

extern struct cvar_s q3map2_csg_compiler;
extern struct cvar_s q3map2_compiler;
extern struct cvar_s q3map2_csg_switches;
extern struct cvar_s q3map2_bsp_switches;
extern struct cvar_s q3map2_vis_switches;
extern struct cvar_s q3map2_lit_switches;

// Baker:
qbool FS_IsDirectory (ccs *filename);
qbool FS_IsRealFile (ccs *filename); // Returns true if file is in filesystem
fs_offset_t FS_FileSizeEx_Safe (ccs *filename);
double FS_RealFile_Time (ccs *filename);

qbool File_TimeString_Fill_MM_DD_YY_HH_MM_AM (ccs *filename, char *timestring, size_t timestring_size) ;

double FS_FileTime_Since_1970_Safe (ccs *filename);

///////////////////////////////////////////////////////////////////////////////
//  FILE INFORMATION: Baker - These functions operate on a path_to_file
///////////////////////////////////////////////////////////////////////////////

void File_URL_Edit_Default_Extension (char *path_to_file, ccs *dot_new_extension, size_t bufsize);

char *File_URL_Edit_Reduce_To_Parent_Path_Trailing_Slash (char *path_to_file);
char *File_URL_Edit_Reduce_To_Parent_Path_No_Trailing_Slash (char *path_to_file);

char *File_URL_Edit_SlashesForward_Like_Unix (char *windows_path_to_file);
char *File_URL_Edit_SlashesBack_Like_Windows (char *unix_path_to_file);

char *File_URL_Edit_Remove_Extension (char *path_to_file);
char *File_URL_Edit_Strip_Set_Extension (char *path_to_file, ccs *ext, size_t path_to_file_sizeof);

ccs *File_URL_SkipPath (ccs *path_to_file); // last path component

char *File_URL_Remove_Trailing_Unix_Slash (char *path_to_file);
char *File_URL_Remove_Trailing_Windows_Slash (char *path_to_file);

// Returns extension with . (like .png) after last slash if exists, returns NULL if nothing found.
ccs *File_URL_GetExtension_With_Dot (ccs *path_to_file); // my_image.png ==> .png (NULL if nothing found)



#include "fs_unsafe.h"


#endif // ! FS_H

