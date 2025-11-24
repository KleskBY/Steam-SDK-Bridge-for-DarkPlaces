// fs.c
/*
	DarkPlaces file system

	Copyright (C) 2003-2006 Mathieu Olivier

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

#include <limits.h>
#include <fcntl.h>

#ifdef _WIN32
	# include <direct.h>
	# include <io.h>
	# include <shlobj.h>
	# include <sys/stat.h>
	# include <share.h>
#else
	# include <pwd.h>
	# include <sys/stat.h>
	# include <unistd.h>
#endif

#include "quakedef.h"

#if TARGET_OS_IPHONE
	// include SDL for IPHONEOS code
	# include <SDL.h>
#endif

#include "thread.h"

#include "fs.h"
#include "wad.h"

#include "utf8lib.h"

// Win32 requires us to add O_BINARY, but the other OSes don't have it
#ifndef O_BINARY
	# define O_BINARY 0
#endif

// In case the system doesn't support the O_NONBLOCK flag
#ifndef O_NONBLOCK
	# define O_NONBLOCK 0
#endif

// largefile support for Win32
#ifdef _WIN32
	#undef lseek
	# define lseek _lseeki64
#endif

// suppress deprecated warnings
#if _MSC_VER >= 1400
	# define read _read
	# define write _write
	# define close _close
	# define unlink _unlink
	# define dup _dup
#endif

// windows wchar helpers
#ifdef _WIN32
# define MAX_OSWPATH MAX_OSPATH
# define WPATHDEF(var) wchar var[MAX_OSWPATH]
#else
# define WPATHDEF(var) ;
#endif // WIN32

#if USE_RWOPS
# include <SDL.h>
typedef SDL_RWops *filedesc_t;
# define FILEDESC_INVALID NULL
# define FILEDESC_ISVALID(fd) ((fd) != NULL)
# define FILEDESC_READ(fd,buf,count) ((fs_offset_t)SDL_RWread(fd, buf, 1, count))
# define FILEDESC_WRITE(fd,buf,count) ((fs_offset_t)SDL_RWwrite(fd, buf, 1, count))
# define FILEDESC_CLOSE SDL_RWclose
# define FILEDESC_SEEK SDL_RWseek
static filedesc_t FILEDESC_DUP(const char *filename, filedesc_t fd) {
	filedesc_t new_fd = SDL_RWFromFile(filename, "rb");
	if (SDL_RWseek(new_fd, SDL_RWseek(fd, 0, RW_SEEK_CUR), RW_SEEK_SET) < 0) {
		SDL_RWclose(new_fd);
		return NULL;
	}
	return new_fd;
}
# define unlink(name) Con_DPrintf ("Sorry, no unlink support when trying to unlink %s.\n", (name))
#else
typedef int filedesc_t;
# define FILEDESC_INVALID -1
# define FILEDESC_ISVALID(fd) ((fd) != -1)
# define FILEDESC_READ read
# define FILEDESC_WRITE write
# define FILEDESC_CLOSE close
# define FILEDESC_SEEK lseek
static filedesc_t FILEDESC_DUP(const char *filename, filedesc_t fd) {
	return dup(fd);
}
#endif


/* This code seems to have originally been written with the assumption that
 * read(..., n) returns n on success. This is not the case (refer to
 * <https://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html>).
 * Ditto for write.
 */

/*
====================
ReadAll

Read exactly length bytes from fd into buf. If end of file is reached,
the number of bytes read is returned. If an error occurred, that error
is returned. Note that if an error is returned, any previously read
data is lost.
====================
*/
// Baker: ReadAll is used quite a bit as FILEDESC_READ
// It appears to be written to support the unusual
// method of wrapping for SDL RwOps type builds
static fs_offset_t ReadAll(const filedesc_t fd, void *const buf, const size_t length)
{
	char *const p = (char *)buf;
	size_t cursor = 0;
	do
	{
		// Baker: FILEDESC_READ is "read"
		const fs_offset_t result = FILEDESC_READ(fd, p + cursor, length - cursor );
		if (result < 0) // Error
			return result;
		if (result == 0) // EOF
			break;
		cursor += result;
	} while (cursor < length);
	return cursor;
}

/*
====================
WriteAll

Write exactly length bytes to fd from buf.
If an error occurred, that error is returned.
====================
*/
static fs_offset_t WriteAll(const filedesc_t fd, const void *const buf, const size_t length)
{
	const char *const p = (const char *)buf;
	size_t cursor = 0;
	do
	{
		const fs_offset_t result = FILEDESC_WRITE(fd, p + cursor, length - cursor);
		if (result < 0) // Error
			return result;
		cursor += result;
	} while (cursor < length);
	return cursor;
}

#undef FILEDESC_READ
#define FILEDESC_READ ReadAll
#undef FILEDESC_WRITE
#define FILEDESC_WRITE WriteAll

/** \page fs File System

All of Quake's data access is through a hierarchical file system, the contents
of the file system can be transparently merged from several sources.

The "base directory" is the path to the directory holding the quake.exe and
all game directories.  The sys_* files pass this to host_init in
quakeparms_t->basedir.  This can be overridden with the "-basedir" command
line parm to allow code debugging in a different directory.  The base
directory is only used during filesystem initialization.

The "game directory" is the first tree on the search path and directory that
all generated files (savegames, screenshots, demos, config files) will be
saved to.  This can be overridden with the "-game" command line parameter.
If multiple "-game <gamedir>" args are passed the last one is the "primary"
and files will be saved there, the rest are read-only.

*/


/*
=============================================================================

CONSTANTS

=============================================================================
*/

// Magic numbers of a ZIP file (big-endian format)
#define ZIP_DATA_HEADER	0x504B0304  // "PK\3\4"
#define ZIP_CDIR_HEADER	0x504B0102  // "PK\1\2"
#define ZIP_END_HEADER	0x504B0506  // "PK\5\6"

// Other constants for ZIP files
#define ZIP_MAX_COMMENTS_SIZE		((unsigned short)0xFFFF)
#define ZIP_END_CDIR_SIZE			22
#define ZIP_CDIR_CHUNK_BASE_SIZE	46
#define ZIP_LOCAL_CHUNK_BASE_SIZE	30

#ifdef LINK_TO_ZLIB
#include <zlib.h>

#define qz_inflate inflate
#define qz_inflateEnd inflateEnd
#define qz_inflateInit2_ inflateInit2_
#define qz_inflateReset inflateReset
#define qz_deflateInit2_ deflateInit2_
#define qz_deflateEnd deflateEnd
#define qz_deflate deflate
#define Z_MEMLEVEL_DEFAULT 8
#else

// Zlib constants (from zlib.h)
#define Z_SYNC_FLUSH	2
#define MAX_WBITS		15
#define Z_OK			0
#define Z_STREAM_END	1
#define Z_STREAM_ERROR  (-2)
#define Z_DATA_ERROR    (-3)
#define Z_MEM_ERROR     (-4)
#define Z_BUF_ERROR     (-5)
#define ZLIB_VERSION	"1.2.3"

#define Z_BINARY 0
#define Z_DEFLATED 8
#define Z_MEMLEVEL_DEFAULT 8

#define Z_NULL 0
#define Z_DEFAULT_COMPRESSION (-1)
#define Z_NO_FLUSH 0
#define Z_SYNC_FLUSH 2
#define Z_FULL_FLUSH 3
#define Z_FINISH 4

// Uncomment the following line if the zlib DLL you have still uses
// the 1.1.x series calling convention on Win32 (WINAPI)
//#define ZLIB_USES_WINAPI


/*
=============================================================================

TYPES

=============================================================================
*/

/*! Zlib stream (from zlib.h)
 * \warning: some pointers we don't use directly have
 * been cast to "void*" for a matter of simplicity
 */
typedef struct
{
	unsigned char			*next_in;	///< next input byte
	unsigned int	avail_in;	///< number of bytes available at next_in
	unsigned long	total_in;	///< total nb of input bytes read so far

	unsigned char			*next_out;	///< next output byte should be put there
	unsigned int	avail_out;	///< remaining free space at next_out
	unsigned long	total_out;	///< total nb of bytes output so far

	char			*msg;		///< last error message, NULL if no error
	void			*state;		///< not visible by applications

	void			*zalloc;	///< used to allocate the internal state
	void			*zfree;		///< used to free the internal state
	void			*opaque;	///< private data object passed to zalloc and zfree

	int				data_type;	///< best guess about the data type: ascii or binary
	unsigned long	adler;		///< adler32 value of the uncompressed data
	unsigned long	reserved;	///< reserved for future use
} z_stream;
#endif


/// inside a package (PAK or PK3)
#define QFILE_FLAG_PACKED (1 << 0)
/// file is compressed using the deflate algorithm (PK3 only)
#define QFILE_FLAG_DEFLATED (1 << 1)
/// file is actually already loaded data
#define QFILE_FLAG_DATA (1 << 2)
/// real file will be removed on close
#define QFILE_FLAG_REMOVE (1 << 3)

#define FILE_BUFF_SIZE_2048 2048
typedef struct
{
	z_stream	zstream;
	size_t		comp_length;			///< length of the compressed file
	size_t		in_ind, in_len;			///< input buffer current index and length
	size_t		in_position;			///< position in the compressed file
	unsigned char		input [FILE_BUFF_SIZE_2048];
} ztoolkit_t;

struct qfile_s
{
	int				flags;
	filedesc_t			handle;					///< file descriptor
	fs_offset_t		real_length;			///< uncompressed file size (for files opened in "read" mode)
	fs_offset_t		position;				///< current position in the file
	fs_offset_t		offset;					///< offset into the package (0 if external file)
	int				ungetc;					///< single stored character from ungetc, cleared to EOF when read

	// Contents buffer
	fs_offset_t		buff_ind, buff_len;		///< buffer current index and length
	unsigned char			buff [FILE_BUFF_SIZE_2048];

	ztoolkit_t*		ztk;	///< For zipped files.

	const unsigned char *data;	///< For data files.

	const char *f_filename; ///< Kept around for QFILE_FLAG_REMOVE, unused otherwise
};


// ------ PK3 files on disk ------ //

// You can get the complete ZIP format description from PKWARE website

typedef struct pk3_endOfCentralDir_s
{
	unsigned int signature;
	unsigned short disknum;
	unsigned short cdir_disknum;	///< number of the disk with the start of the central directory
	unsigned short localentries;	///< number of entries in the central directory on this disk
	unsigned short nbentries;		///< total number of entries in the central directory on this disk
	unsigned int cdir_size;			///< size of the central directory
	unsigned int cdir_offset;		///< with respect to the starting disk number
	unsigned short comment_size;
	fs_offset_t prepended_garbage;
} pk3_endOfCentralDir_t;


// ------ PAK files on disk ------ //
typedef struct dpackfile_s
{
	char name[56];
	int dpackfilepos, dpackfilelen;
} dpackfile_t;

typedef struct dpackheader_s
{
	char id[4];
	int dirofs;
	int dirlen;
} dpackheader_t;


/*! \name Packages in memory
 * @{
 */
/// the offset in packfile_t is the true contents offset
#define PACKFILE_FLAG_TRUEOFFS (1 << 0)
/// file compressed using the deflate algorithm
#define PACKFILE_FLAG_DEFLATED (1 << 1)
/// file is a symbolic link
#define PACKFILE_FLAG_SYMLINK (1 << 2)

typedef struct packfile_s
{
	char name [MAX_QPATH_128];
	int flags;
	fs_offset_t offset;
	fs_offset_t packsize;	///< size in the package
	fs_offset_t realsize;	///< real file size (uncompressed)
} packfile_t;

typedef struct pack_s
{
	char			filename [MAX_OSPATH];
	char			shortname [MAX_QPATH_128];
	filedesc_t		pk3_handle;
	qbool			ignorecase;  ///< PK3 ignores case
	int				numfiles;
	qbool			vpack;
	qbool			dlcache;
	packfile_t		*files;
} pack_t;
//@}

/// Search paths for files (including packages)
typedef struct searchpath_s {
	// only one of filename / pack will be used
	char filename[MAX_OSPATH];
	pack_t *pack;
	struct searchpath_s *next;
} searchpath_t;


/*
=============================================================================

FUNCTION PROTOTYPES

=============================================================================
*/

void FS_Dir_f(cmd_state_t *cmd);
void FS_Ls_f(cmd_state_t *cmd);
void FS_Which_f(cmd_state_t *cmd);

// Baker: canonicalname is a reply (LATER)
static searchpath_t *FS_FindFile (const char *name, int *index, qbool quiet);
static packfile_t *FS_AddFileToPack (const char *name, pack_t *pack,
									fs_offset_t offset, fs_offset_t packsize,
									fs_offset_t realsize, int flags);


/*
=============================================================================

VARIABLES

=============================================================================
*/

mempool_t *fs_mempool;
void *fs_mutex = NULL;

searchpath_t *fs_searchpaths = NULL;
int fs_have_qex = 0; // AURA 3.0
const char *const fs_checkgamedir_missing = "missing";

#define MAX_FILES_IN_PACK_65536	65536

char fs_userdir[MAX_OSPATH];
char fs_gamedir[MAX_OSPATH];
char fs_basedir[MAX_OSPATH]; // Baker: (has trailing slash) Current basedir : c:\quak/

static pack_t *fs_selfpack = NULL;

int fs_data_override; // Baker r0009: Super -data override
int fs_is_zircon_galaxy;

// list of active game directories (empty if not running a mod)
int fs_numgamedirs = 0;
char fs_gamedirs[MAX_GAMEDIRS_16][MAX_QPATH_128];

// list of all gamedirs with modinfo.txt
gamedir_t *fs_all_gamedirs = NULL;
int fs_all_gamedirs_count = 0;

cvar_t scr_screenshot_name = {CF_CLIENT | CF_PERSISTENT, "scr_screenshot_name","dp", "prefix name for saved screenshots (changes based on -game commandline, as well as which game mode is running; the date is encoded using strftime escapes)"};
cvar_t fs_empty_files_in_pack_mark_deletions = {CF_CLIENT | CF_SERVER, "fs_empty_files_in_pack_mark_deletions", "0", "if enabled, empty files in a pak/pk3 count as not existing but cancel the search in further packs, effectively allowing patch pak/pk3 files to 'delete' files"};
cvar_t cvar_fs_gamedir = {CF_CLIENT | CF_SERVER | CF_READONLY | CF_PERSISTENT, "fs_gamedir", "", "the list of currently selected gamedirs (use the 'gamedir' command to change this)"};

// CSG -> BSP -> VIS -> LIT
cvar_t q3map2_csg_compiler = {CF_SERVER, "q3map2_csg_compiler", "", "If emptystring, engine .exe is the compiler [Zircon]"};
cvar_t q3map2_compiler = {CF_SERVER, "q3map2_compiler", "q3map2/q3map2.exe", "If emptystring, engine .exe is the compiler [Zircon]"};
cvar_t q3map2_csg_switches = {CF_SERVER, "q3map2_csg_switches", "-csg", "q3map2 switches for bsp phase [Zircon]"};
cvar_t q3map2_bsp_switches = {CF_SERVER, "q3map2_bsp_switches", "-game nexuiz -meta -v -mv 1000000 -mi 6000000 -samplesize 8", "q3map2 switches for bsp phase [Zircon]"};
cvar_t q3map2_vis_switches = {CF_SERVER, "q3map2_vis_switches", "-game nexuiz -vis -saveprt -fast", "q3map2 switches for vis phase [Zircon]"};
cvar_t q3map2_lit_switches = {CF_SERVER, "q3map2_lit_switches", "-game nexuiz -light -v -deluxe -faster -cheapgrid -filter -gamma 0.5", "q3map2 switches for light phase [Zircon]"};
//cvar_t dungeon_lit_switches = {CF_SERVER, "dungeon_lit_switches", "-light -v -deluxe -fast   -lightmapsearchpower 4 -gamma 1.3", "q3map2 switches for light phase [Zircon]"};

cvar_t ui_export_magnification = {CF_CLIENT, "ui_export_magnification", "0", "Magnification on export [Zircon]"};
cvar_t ui_import_magnification = {CF_CLIENT, "ui_import_magnification", "1", "Auto-scale loading of CSQC UI [Zircon]"};
cvar_t ui_full_canvas = {CF_CLIENT, "ui_full_canvas", "1", "ui_full_canvas [Zircon]"};

cvar_t png_texture_suppress = {CF_CLIENT, "png_texture_suppress", "0", "Don't load PNG map textures [Zircon]"};




/*
=============================================================================

PRIVATE FUNCTIONS - PK3 HANDLING

=============================================================================
*/

#ifndef LINK_TO_ZLIB
// Functions exported from zlib
#if defined(_WIN32) && defined(ZLIB_USES_WINAPI)
# define ZEXPORT WINAPI
#else
# define ZEXPORT
#endif

static int (ZEXPORT *qz_inflate) (z_stream* strm, int flush);
static int (ZEXPORT *qz_inflateEnd) (z_stream* strm);
static int (ZEXPORT *qz_inflateInit2_) (z_stream* strm, int windowBits, const char *version, int stream_size);
static int (ZEXPORT *qz_inflateReset) (z_stream* strm);
static int (ZEXPORT *qz_deflateInit2_) (z_stream* strm, int level, int method, int windowBits, int memLevel, int strategy, const char *version, int stream_size);
static int (ZEXPORT *qz_deflateEnd) (z_stream* strm);
static int (ZEXPORT *qz_deflate) (z_stream* strm, int flush);
#endif

#define qz_inflateInit2(strm, windowBits) \
        qz_inflateInit2_((strm), (windowBits), ZLIB_VERSION, sizeof(z_stream))
#define qz_deflateInit2(strm, level, method, windowBits, memLevel, strategy) \
        qz_deflateInit2_((strm), (level), (method), (windowBits), (memLevel), (strategy), ZLIB_VERSION, sizeof(z_stream))

#ifndef LINK_TO_ZLIB
//        qz_deflateInit_((strm), (level), ZLIB_VERSION, sizeof(z_stream))

static dllfunction_t zlibfuncs[] =
{
	{"inflate",			(void **) &qz_inflate},
	{"inflateEnd",		(void **) &qz_inflateEnd},
	{"inflateInit2_",	(void **) &qz_inflateInit2_},
	{"inflateReset",	(void **) &qz_inflateReset},
	{"deflateInit2_",   (void **) &qz_deflateInit2_},
	{"deflateEnd",      (void **) &qz_deflateEnd},
	{"deflate",         (void **) &qz_deflate},
	{NULL, NULL}
};

/// Handle for Zlib DLL
static dllhandle_t zlib_dll = NULL;
#endif

#ifdef _WIN32
	static HRESULT (WINAPI *qSHGetFolderPath) (HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPWSTR pszPath);
	static dllfunction_t shfolderfuncs[] =
	{
		{"SHGetFolderPathW", (void **) &qSHGetFolderPath},
		{NULL, NULL}
	};
	static const char *shfolderdllnames [] =
	{
		"shfolder.dll",  // IE 4, or Win NT and higher
		NULL
	};
	static dllhandle_t shfolder_dll = NULL;

	const GUID qFOLDERID_SavedGames = {0x4C5C32FF, 0xBB9D, 0x43b0, {0xB5, 0xB4, 0x2D, 0x72, 0xE5, 0x4E, 0xAA, 0xA4}};
	#define qREFKNOWNFOLDERID const GUID *
	#define qKF_FLAG_CREATE 0x8000
	#define qKF_FLAG_NO_ALIAS 0x1000
	static HRESULT (WINAPI *qSHGetKnownFolderPath) (qREFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);
	static dllfunction_t shell32funcs[] =
	{
		{"SHGetKnownFolderPath", (void **) &qSHGetKnownFolderPath},
		{NULL, NULL}
	};
	static const char *shell32dllnames [] =
	{
		"shell32.dll",  // Vista and higher
		NULL
	};
	static dllhandle_t shell32_dll = NULL;

	static HRESULT (WINAPI *qCoInitializeEx)(LPVOID pvReserved, DWORD dwCoInit);
	static void (WINAPI *qCoUninitialize)(void);
	static void (WINAPI *qCoTaskMemFree)(LPVOID pv);
	static dllfunction_t ole32funcs[] =
	{
		{"CoInitializeEx", (void **) &qCoInitializeEx},
		{"CoUninitialize", (void **) &qCoUninitialize},
		{"CoTaskMemFree", (void **) &qCoTaskMemFree},
		{NULL, NULL}
	};
	static const char *ole32dllnames [] =
	{
		"ole32.dll", // 2000 and higher
		NULL
	};
	static dllhandle_t ole32_dll = NULL;
#endif

/*
====================
PK3_CloseLibrary

Unload the Zlib DLL
====================
*/
static void PK3_CloseLibrary (void)
{
#ifndef LINK_TO_ZLIB
	Sys_FreeLibrary (&zlib_dll);
#endif
}


/*
====================
PK3_OpenLibrary

Try to load the Zlib DLL
====================
*/
static qbool PK3_OpenLibrary (void)
{
#ifdef LINK_TO_ZLIB
	return true;
#else
	const char *dllnames [] =
	{
#if defined(_WIN32)
# ifdef ZLIB_USES_WINAPI
		"zlibwapi.dll",
		"zlib.dll",
# else
		"zlib1.dll",
# endif
#elif defined(MACOSX)
		"libz.dylib",
#else
		"libz.so.1",
		"libz.so",
#endif
		NULL
	};

	// Already loaded?
	if (zlib_dll)
		return true;

	// Load the DLL
	return Sys_LoadDependency (dllnames, &zlib_dll, zlibfuncs);
#endif
}

/*
====================
FS_HasZlib

See if zlib is available
====================
*/
qbool FS_HasZlib(void)
{
#ifdef LINK_TO_ZLIB
	return true;
#else
	PK3_OpenLibrary(); // to be safe
	return (zlib_dll != 0);
#endif
}

/*
====================
PK3_GetEndOfCentralDir

Extract the end of the central directory from a PK3 package
====================
*/
static qbool PK3_GetEndOfCentralDir (const char *packfile, filedesc_t packhandle, pk3_endOfCentralDir_t *eocd)
{
	fs_offset_t filesize, maxsize;
	unsigned char *buffer, *ptr;
	int ind;

	// Get the package size
	filesize = FILEDESC_SEEK (packhandle, 0, SEEK_END);
	if (filesize < ZIP_END_CDIR_SIZE)
		return false;

	// Load the end of the file in memory
	if (filesize < ZIP_MAX_COMMENTS_SIZE + ZIP_END_CDIR_SIZE)
		maxsize = filesize;
	else
		maxsize = ZIP_MAX_COMMENTS_SIZE + ZIP_END_CDIR_SIZE;
	buffer = (unsigned char *)Mem_Alloc (tempmempool, maxsize);
	FILEDESC_SEEK (packhandle, filesize - maxsize, SEEK_SET);
	if (FILEDESC_READ (packhandle, buffer, maxsize) != (fs_offset_t) maxsize)
	{
		Mem_Free (buffer);
		return false;
	}

	// Look for the end of central dir signature around the end of the file
	maxsize -= ZIP_END_CDIR_SIZE;
	ptr = &buffer[maxsize];
	ind = 0;
	while (BuffBigLong (ptr) != ZIP_END_HEADER)
	{
		if (ind == maxsize)
		{
			Mem_Free (buffer);
			return false;
		}

		ind++;
		ptr--;
	}

	memcpy (eocd, ptr, ZIP_END_CDIR_SIZE);
	eocd->signature = LittleLong (eocd->signature);
	eocd->disknum = LittleShort (eocd->disknum);
	eocd->cdir_disknum = LittleShort (eocd->cdir_disknum);
	eocd->localentries = LittleShort (eocd->localentries);
	eocd->nbentries = LittleShort (eocd->nbentries);
	eocd->cdir_size = LittleLong (eocd->cdir_size);
	eocd->cdir_offset = LittleLong (eocd->cdir_offset);
	eocd->comment_size = LittleShort (eocd->comment_size);
	eocd->prepended_garbage = filesize - (ind + ZIP_END_CDIR_SIZE) - eocd->cdir_offset - eocd->cdir_size; // this detects "SFX" zip files
	eocd->cdir_offset += eocd->prepended_garbage;

	Mem_Free (buffer);

	if (
			eocd->cdir_size > filesize ||
			eocd->cdir_offset >= filesize ||
			eocd->cdir_offset + eocd->cdir_size > filesize
	   )
	{
		// Obviously invalid central directory.
		return false;
	}

	return true;
}


/*
====================
PK3_BuildFileList

Extract the file list from a PK3 file
====================
*/
static int PK3_BuildFileList (pack_t *pack, const pk3_endOfCentralDir_t *eocd)
{
	unsigned char *central_dir, *ptr;
	unsigned int ind;
	fs_offset_t remaining;

	// Load the central directory in memory
	central_dir = (unsigned char *)Mem_Alloc (tempmempool, eocd->cdir_size);
	if (FILEDESC_SEEK (pack->pk3_handle, eocd->cdir_offset, SEEK_SET) == -1)
	{
		Mem_Free (central_dir);
		return -1;
	}
	if (FILEDESC_READ (pack->pk3_handle, central_dir, eocd->cdir_size) != (fs_offset_t) eocd->cdir_size)
	{
		Mem_Free (central_dir);
		return -1;
	}

	// Extract the files properties
	// The parsing is done "by hand" because some fields have variable sizes and
	// the constant part isn't 4-bytes aligned, which makes the use of structs difficult
	remaining = eocd->cdir_size;
	pack->numfiles = 0;
	ptr = central_dir;
	for (ind = 0; ind < eocd->nbentries; ind++)
	{
		fs_offset_t namesize, count;

		// Checking the remaining size
		if (remaining < ZIP_CDIR_CHUNK_BASE_SIZE)
		{
			Mem_Free (central_dir);
			return -1;
		}
		remaining -= ZIP_CDIR_CHUNK_BASE_SIZE;

		// Check header
		if (BuffBigLong (ptr) != ZIP_CDIR_HEADER)
		{
			Mem_Free (central_dir);
			return -1;
		}

		namesize = (unsigned short)BuffLittleShort (&ptr[28]);	// filename length

		// Check encryption, compression, and attributes
		// 1st uint8  : general purpose bit flag
		//    Check bits 0 (encryption), 3 (data descriptor after the file), and 5 (compressed patched data (?))
		//
		// LadyHavoc: bit 3 would be a problem if we were scanning the archive
		// but is not a problem in the central directory where the values are
		// always real.
		//
		// bit 3 seems to always be set by the standard Mac OSX zip maker
		//
		// 2nd uint8 : external file attributes
		//    Check bits 3 (file is a directory) and 5 (file is a volume (?))
		if ((ptr[8] & 0x21 /*33 decimal*/) == 0 && (ptr[38] & 0x18 /*24 decimal*/) == 0)
		{
			// Still enough bytes for the name?
			if (remaining < namesize || namesize >= (int)sizeof (*pack->files)) {
				Mem_Free (central_dir);
				return -1;
			}

			// WinZip doesn't use the "directory" attribute, so we need to check the name directly
			if (ptr[ZIP_CDIR_CHUNK_BASE_SIZE + namesize - 1] != '/')
			{
				char filename [sizeof (pack->files[0].name)];
				fs_offset_t offset, packsize, realsize;
				int flags;

				// Extract the name (strip it if necessary)
				namesize = min(namesize, (int)sizeof (filename) - 1);
				memcpy (filename, &ptr[ZIP_CDIR_CHUNK_BASE_SIZE], namesize);
				filename[namesize] = '\0';

				if (BuffLittleShort (&ptr[10]))
					flags = PACKFILE_FLAG_DEFLATED;
				else
					flags = 0;
				offset = (unsigned int)(BuffLittleLong (&ptr[42]) + eocd->prepended_garbage);
				packsize = (unsigned int)BuffLittleLong (&ptr[20]);
				realsize = (unsigned int)BuffLittleLong (&ptr[24]);

				switch(ptr[5]) // C_VERSION_MADE_BY_1
				{
					case 3: // UNIX_
					case 2: // VMS_
					case 16: // BEOS_
						if ((BuffLittleShort(&ptr[40]) & 0120000) == 0120000)
							// can't use S_ISLNK here, as this has to compile on non-UNIX too
							flags |= PACKFILE_FLAG_SYMLINK;
						break;
				}

				FS_AddFileToPack (filename, pack, offset, packsize, realsize, flags);
			}
		}

		// Skip the name, additionnal field, and comment
		// 1er uint16 : extra field length
		// 2eme uint16 : file comment length
		count = namesize + (unsigned short)BuffLittleShort (&ptr[30]) + (unsigned short)BuffLittleShort (&ptr[32]);
		ptr += ZIP_CDIR_CHUNK_BASE_SIZE + count;
		remaining -= count;
	}

	// If the package is empty, central_dir is NULL here
	if (central_dir != NULL)
		Mem_Free (central_dir);
	return pack->numfiles;
}


/*
====================
FS_LoadPackPK3

Create a package entry associated with a PK3 file
====================
*/
static pack_t *FS_LoadPackPK3FromFD (const char *packfile, filedesc_t packhandle, qbool silent)
{
	pk3_endOfCentralDir_t eocd;
	pack_t *pack;
	int real_nb_files;

	if (! PK3_GetEndOfCentralDir (packfile, packhandle, &eocd)) {
		if (!silent)
			Con_PrintLinef ("%s is not a PK3 file", packfile);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}

	// Multi-volume ZIP archives are NOT allowed
	if (eocd.disknum != 0 || eocd.cdir_disknum != 0) {
		Con_PrintLinef ("%s is a multi-volume ZIP archive", packfile);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}

	// We only need to do this test if MAX_FILES_IN_PACK_65536 is lesser than 65535
	// since eocd.nbentries is an unsigned 16 bits integer
#if MAX_FILES_IN_PACK_65536 < 65535 
	if (eocd.nbentries > MAX_FILES_IN_PACK_65536)
	{
		Con_PrintLinef ("%s contains too many files (%hu)", packfile, eocd.nbentries);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}
#endif

	// Create a package structure in memory
	pack = (pack_t *)Mem_Alloc(fs_mempool, sizeof (pack_t));
	pack->ignorecase = true; // PK3 ignores case
	strlcpy (pack->filename, packfile, sizeof (pack->filename));
	pack->pk3_handle = packhandle;
	pack->numfiles = eocd.nbentries;
	pack->files = (packfile_t *)Mem_Alloc(fs_mempool, eocd.nbentries * sizeof(packfile_t));

	real_nb_files = PK3_BuildFileList (pack, &eocd);
	if (real_nb_files < 0) {
		Con_PrintLinef ("%s is not a valid PK3 file", packfile);
		FILEDESC_CLOSE(pack->pk3_handle);
		Mem_Free(pack);
		return NULL;
	}

	Con_DPrintLinef ("Added packfile %s (%d files)", packfile, real_nb_files);
	return pack;
}

static filedesc_t FS_SysOpenFiledesc(const char *realpath, const char *mode, qbool nonblocking);
static pack_t *FS_LoadPackPK3 (const char *packfile)
{
	filedesc_t packhandle;
	packhandle = FS_SysOpenFiledesc (packfile, "rb", false);
	if (!FILEDESC_ISVALID(packhandle))
		return NULL;
	return FS_LoadPackPK3FromFD(packfile, packhandle, false);
}


/*
====================
PK3_GetTrueFileOffset

Find where the true file data offset is
====================
*/
static qbool PK3_GetTrueFileOffset (packfile_t *pfile, pack_t *pack)
{
	unsigned char buffer [ZIP_LOCAL_CHUNK_BASE_SIZE];
	fs_offset_t count;

	// Already found?
	if (pfile->flags & PACKFILE_FLAG_TRUEOFFS)
		return true;

	// Load the local file description
	if (FILEDESC_SEEK (pack->pk3_handle, pfile->offset, SEEK_SET) == -1)
	{
		Con_PrintLinef ("Can't seek in package %s", pack->filename);
		return false;
	}
	count = FILEDESC_READ (pack->pk3_handle, buffer, ZIP_LOCAL_CHUNK_BASE_SIZE);
	if (count != ZIP_LOCAL_CHUNK_BASE_SIZE || BuffBigLong (buffer) != ZIP_DATA_HEADER) {
		Con_PrintLinef ("Can't retrieve file %s in package %s", pfile->name, pack->filename);
		return false;
	}

	// Skip name and extra field
	pfile->offset += BuffLittleShort (&buffer[26]) + BuffLittleShort (&buffer[28]) + ZIP_LOCAL_CHUNK_BASE_SIZE;

	pfile->flags |= PACKFILE_FLAG_TRUEOFFS;
	return true;
}


#include "fs_baker_extras1.c.h" // HERE?

/*
=============================================================================

OTHER PRIVATE FUNCTIONS

=============================================================================
*/


/*
====================
FS_AddFileToPack

Add a file to the list of files contained into a package
====================
*/
static packfile_t *FS_AddFileToPack (const char *name, pack_t *pack,
									 fs_offset_t offset, fs_offset_t packsize,
									 fs_offset_t realsize, int flags)
{
	int (*strcmp_funct) (const char *str1, const char *str2);
	int left, right, middle;
	packfile_t *pfile;

	strcmp_funct = pack->ignorecase ? strcasecmp : strcmp;

	// Look for the slot we should put that file into (binary search)
	left = 0;
	right = pack->numfiles - 1;
	while (left <= right)
	{
		int diff;

		middle = (left + right) / 2;
		diff = strcmp_funct (pack->files[middle].name, name);

		// If we found the file, there's a problem
		if (!diff)
			Con_Printf ("Package %s contains the file %s several times\n", pack->filename, name);

		// If we're too far in the list
		if (diff > 0)
			right = middle - 1;
		else
			left = middle + 1;
	}

	// We have to move the right of the list by one slot to free the one we need
	pfile = &pack->files[left];
	memmove (pfile + 1, pfile, (pack->numfiles - left) * sizeof (*pfile));
	pack->numfiles++;

	strlcpy (pfile->name, name, sizeof (pfile->name));
	pfile->offset = offset;
	pfile->packsize = packsize;
	pfile->realsize = realsize;
	pfile->flags = flags;

	return pfile;
}



/*
============
FS_Path_f

============
*/
static void FS_Path_f(cmd_state_t *cmd)
{
	searchpath_t *s;

	Con_PrintLinef ("Current search path:");
	for (s=fs_searchpaths ; s ; s=s->next) {
		if (s->pack) {
			if (s->pack->vpack)
				Con_PrintLinef ("%sdir (virtual pack)", s->pack->filename);
			else
				Con_PrintLinef ("%s (%d files)", s->pack->filename, s->pack->numfiles);
		}
		else
			Con_PrintLinef ("%s", s->filename);
	} // for
}

CALLERS_ (DevTabSelectOnChange)

RELATED_ (FS_Path_f)
void FS_Path_Feed (stringlist_t *plist)
{
	int count = 0;
	for (searchpath_t *s = fs_searchpaths; s; s=s->next) {
		stringlistappendf	(plist, "%d", count ++);
		if (s->pack) {
			if (s->pack->vpack)
				stringlistappendf	(plist, "%sdir (virtual pack)", s->pack->filename);
			else
				stringlistappendf	(plist, "%s (%d files)", s->pack->filename, s->pack->numfiles);
		} else
			stringlistappendf	(plist, "%s", s->filename);
	} // for
}

/*
=================
FS_LoadPackPAK
=================
*/
/*! Takes an explicit (not game tree related) path to a pak file.
 *Loads the header and directory, adding the files at the beginning
 *of the list so they override previous pack files.
 */
static pack_t *FS_LoadPackPAK (const char *packfile)
{
	dpackheader_t header;
	int i, numpackfiles;
	filedesc_t packhandle;
	pack_t *pack;
	dpackfile_t *info;

	packhandle = FS_SysOpenFiledesc(packfile, "rb", false);
	if (!FILEDESC_ISVALID(packhandle))
		return NULL;
	if (FILEDESC_READ (packhandle, (void *)&header, sizeof(header)) != sizeof(header))
	{
		Con_PrintLinef ("%s is not a packfile", packfile);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}
	if (memcmp(header.id, "PACK", 4))
	{
		Con_PrintLinef ("%s is not a packfile", packfile);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}
	header.dirofs = LittleLong (header.dirofs);
	header.dirlen = LittleLong (header.dirlen);

	if (header.dirlen % sizeof(dpackfile_t))
	{
		Con_PrintLinef ("%s has an invalid directory size", packfile);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}

	numpackfiles = header.dirlen / sizeof(dpackfile_t);

	if (numpackfiles < 0 || numpackfiles > MAX_FILES_IN_PACK_65536) {
		Con_PrintLinef ("%s has %d files", packfile, numpackfiles);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}

	info = (dpackfile_t *)Mem_Alloc(tempmempool, sizeof(*info) * numpackfiles);
	FILEDESC_SEEK (packhandle, header.dirofs, SEEK_SET);
	if (header.dirlen != FILEDESC_READ (packhandle, (void *)info, header.dirlen))
	{
		Con_PrintLinef ("%s is an incomplete PAK, not loading", packfile);
		Mem_Free(info);
		FILEDESC_CLOSE(packhandle);
		return NULL;
	}

	pack = (pack_t *)Mem_Alloc(fs_mempool, sizeof (pack_t));
	pack->ignorecase = true; // PAK is sensitive in Quake1 but insensitive in Quake2
	strlcpy (pack->filename, packfile, sizeof (pack->filename));
	pack->pk3_handle = packhandle;
	pack->numfiles = 0;
	pack->files = (packfile_t *)Mem_Alloc(fs_mempool, numpackfiles * sizeof(packfile_t));

	// parse the directory
	for (i = 0;i < numpackfiles;i++)
	{
		fs_offset_t offset = (unsigned int)LittleLong (info[i].dpackfilepos);
		fs_offset_t size = (unsigned int)LittleLong (info[i].dpackfilelen);

		// Ensure a zero terminated file name (required by format).
		info[i].name[sizeof(info[i].name) - 1] = 0;

		FS_AddFileToPack (info[i].name, pack, offset, size, size, PACKFILE_FLAG_TRUEOFFS);
	}

	Mem_Free(info);

	Con_DPrintLinef ("Added packfile %s (%d files)", packfile, numpackfiles);
	return pack;
}

/*
====================
FS_LoadPackVirtual

Create a package entry associated with a directory file
====================
*/
static pack_t *FS_LoadPackVirtual (const char *dirname)
{
	pack_t *pack;
	pack = (pack_t *)Mem_Alloc(fs_mempool, sizeof (pack_t));
	pack->vpack = true;
	pack->ignorecase = false;
	strlcpy (pack->filename, dirname, sizeof(pack->filename));
	pack->pk3_handle = FILEDESC_INVALID;
	pack->numfiles = -1;
	pack->files = NULL;
	Con_DPrintLinef ("Added packfile %s (virtual pack)", dirname);
	return pack;
}

/*
================
FS_AddPack_Fullpath
================
*/
/*! Adds the given pack to the search path.
 * The pack type is autodetected by the file extension.
 *
 * Returns true if the file was successfully added to the
 * search path or if it was already included.
 *
 * If keep_plain_dirs is set, the pack will be added AFTER the first sequence of
 * plain directories.
 *
 */
static qbool FS_AddPack_Fullpath(const char *pakfile, const char *shortname, qbool *already_loaded, qbool keep_plain_dirs, qbool dlcache)
{
	searchpath_t *search;
	pack_t *pak = NULL;
	const char *ext = FS_FileExtension(pakfile);
	size_t slen;

	for (search = fs_searchpaths; search; search = search->next) {
		if (search->pack && String_Match_Caseless(search->pack->filename, pakfile)) {
			if (already_loaded)
				*already_loaded = true;
			return true; // already loaded
		} // if
	} // for

	if (already_loaded)
		*already_loaded = false;

	if (String_Match_Caseless(ext, "pk3dir"))			pak = FS_LoadPackVirtual (pakfile);
	else if (String_Match_Caseless(ext, "dpkdir"))		pak = FS_LoadPackVirtual (pakfile);
	else if (String_Match_Caseless(ext, "pak"))		pak = FS_LoadPackPAK (pakfile);
	else if (String_Match_Caseless(ext, "pk3"))		pak = FS_LoadPackPK3 (pakfile);
	else if (String_Match_Caseless(ext, "kpf")) 		pak = FS_LoadPackPK3 (pakfile); // AURA 3.1
	else if (String_Match_Caseless(ext, "dpk")) 		pak = FS_LoadPackPK3 (pakfile);
	else if (String_Match_Caseless(ext, "obb")) 		pak = FS_LoadPackPK3 (pakfile); // android apk expansion
	else
		Con_PrintLinef (QUOTED_S " does not have a pack extension", pakfile);

	if (pak) {
		c_strlcpy(pak->shortname, shortname);

		//Con_DPrintLinef ("  Registered pack with short name %s", shortname);
		if (keep_plain_dirs) {
			// find the first item whose next one is a pack or NULL
			searchpath_t *insertion_point = 0;
			if (fs_searchpaths && !fs_searchpaths->pack) {
				insertion_point = fs_searchpaths;
				for (;;) {
					if (!insertion_point->next)
						break;
					if (insertion_point->next->pack)
						break;
					insertion_point = insertion_point->next;
				} // for
			} // if

			// If insertion_point is NULL, this means that either there is no
			// item in the list yet, or that the very first item is a pack. In
			// that case, we want to insert at the beginning...
			if (!insertion_point) {
				search = (searchpath_t *)Mem_Alloc(fs_mempool, sizeof(searchpath_t));
				search->next = fs_searchpaths;
				fs_searchpaths = search;
			}
			else
			// otherwise we want to append directly after insertion_point.
			{
				search = (searchpath_t *)Mem_Alloc(fs_mempool, sizeof(searchpath_t));
				search->next = insertion_point->next;
				insertion_point->next = search;
			}
		}
		else
		{
			search = (searchpath_t *)Mem_Alloc(fs_mempool, sizeof(searchpath_t));
			search->next = fs_searchpaths;
			fs_searchpaths = search;
		}
		search->pack = pak;
		search->pack->dlcache = dlcache;
		if (pak->vpack) {
			dpsnprintf(search->filename, sizeof(search->filename), "%s/", pakfile);
			// if shortname ends with "pk3dir" or "dpkdir", strip that suffix to make it just "pk3" or "dpk"
			// same goes for the name inside the pack structure
			slen = strlen(pak->shortname);
			if (slen >= 7)
				if (String_Match_Caseless(pak->shortname + slen - 7, ".pk3dir") || String_Match_Caseless(pak->shortname + slen - 7, ".dpkdir"))
					pak->shortname[slen - 3] = 0;
			slen = strlen(pak->filename);
			if (slen >= 7)
				if (String_Match_Caseless(pak->filename + slen - 7, ".pk3dir") || String_Match_Caseless(pak->filename + slen - 7, ".dpkdir"))
					pak->filename[slen - 3] = 0;
		}
		return true;
	}
	else
	{
		Con_PrintLinef (CON_ERROR "unable to load pak " QUOTED_S, pakfile);
		return false;
	}
}


/*
================
FS_AddPack
================
*/
/*! Adds the given pack to the search path and searches for it in the game path.
 * The pack type is autodetected by the file extension.
 *
 * Returns true if the file was successfully added to the
 * search path or if it was already included.
 *
 * If keep_plain_dirs is set, the pack will be added AFTER the first sequence of
 * plain directories.
 */
qbool FS_AddPack(const char *pakfile, qbool *already_loaded, qbool keep_plain_dirs, qbool dlcache)
{
	char fullpath[MAX_OSPATH];
	int index;
	searchpath_t *search;

	if (already_loaded)
		*already_loaded = false;

	// then find the real name...
	search = FS_FindFile(pakfile, &index, fs_quiet_true);
	if (!search || search->pack)
	{
		Con_PrintLinef ("could not find pak " QUOTED_S, pakfile);
		return false;
	}

	dpsnprintf(fullpath, sizeof(fullpath), "%s%s", search->filename, pakfile);

	return FS_AddPack_Fullpath(fullpath, pakfile, already_loaded, keep_plain_dirs, dlcache);
}


/*
================
FS_AddGameDirectory

Sets fs_gamedir, adds the directory to the head of the path,
then loads and adds pak1.pak pak2.pak ...
================
*/
static void FS_AddGameDirectory (ccs *dir)
{
	int i;
	stringlist_t list;
	searchpath_t *search;

	c_strlcpy (fs_gamedir, dir);

	stringlistinit(&list);
	stringlist_append_listdirectory (&list, "", dir);
	stringlistsort_no_unique (&list);

	// add any PAK package in the directory
	for (i = 0;i < list.numstrings;i++) {
		if (String_Match_Caseless(FS_FileExtension(list.strings[i]), "pak")) {
			FS_AddPack_Fullpath(list.strings[i], list.strings[i] + strlen(dir), NULL, false, false);
		}
	}

	// add any PK3 package in the directory
	for (i = 0;i < list.numstrings;i++) {
		if (String_Match_Caseless(FS_FileExtension(list.strings[i]), "pk3") || String_Match_Caseless(FS_FileExtension(list.strings[i]), "obb") || String_Match_Caseless(FS_FileExtension(list.strings[i]), "pk3dir")
			|| String_Match_Caseless(FS_FileExtension(list.strings[i]), "dpk") || String_Match_Caseless(FS_FileExtension(list.strings[i]), "dpkdir"))
		{
			FS_AddPack_Fullpath(list.strings[i], list.strings[i] + strlen(dir), NULL, false, false);
		}
	}

	stringlistfreecontents(&list);

	// Add the directory to the search path
	// (unpacked files have the priority over packed files)
	search = (searchpath_t *)Mem_Alloc(fs_mempool, sizeof(searchpath_t));
	c_strlcpy (search->filename, dir);
	search->next = fs_searchpaths;
	fs_searchpaths = search;
}


/*
================
FS_AddGameHierarchy
================
*/
static void FS_AddGameHierarchy (const char *dir)
{
	char vabuf[1024];
	// Add the common game directory
	FS_AddGameDirectory (va(vabuf, sizeof(vabuf), "%s%s/", fs_basedir, dir));

	if (*fs_userdir)
		FS_AddGameDirectory(va(vabuf, sizeof(vabuf), "%s%s/", fs_userdir, dir));
}


/*
============
FS_FileExtension
============
*/
const char *FS_FileExtension (const char *in)
{
	const char *separator, *backslash, *colon, *dot;

	dot = strrchr(in, '.');
	if (dot == NULL)
		return "";

	separator = strrchr(in, '/');
	backslash = strrchr(in, '\\');
	if (!separator || separator < backslash)
		separator = backslash;
	colon = strrchr(in, ':');
	if (!separator || separator < colon)
		separator = colon;

	if (separator && (dot < separator))
		return "";

	return dot + 1;
}


/*
============
FS_FileWithoutPath
============
*/
const char *FS_FileWithoutPath (const char *in)
{
	const char *separator, *backslash, *colon;

	separator = strrchr(in, '/');
	backslash = strrchr(in, '\\');
	if (!separator || separator < backslash)
		separator = backslash;
	colon = strrchr(in, ':');
	if (!separator || separator < colon)
		separator = colon;
	return separator ? separator + 1 : in;
}


/*
================
FS_ClearSearchPath
================
*/
static void FS_ClearSearchPath (void)
{
	// unload all packs and directory information, close all pack files
	// (if a qfile is still reading a pack it won't be harmed because it used
	//  dup() to get its own handle already)
	while (fs_searchpaths) {
		searchpath_t *search = fs_searchpaths;
		fs_searchpaths = search->next;
		if (search->pack && search->pack != fs_selfpack)
		{
			if (!search->pack->vpack)
			{
				// close the file
				FILEDESC_CLOSE(search->pack->pk3_handle);
				// free any memory associated with it
				if (search->pack->files)
					Mem_Free(search->pack->files);
			}
			Mem_Free(search->pack);
		}
		Mem_Free(search);
	}
}

/*
================
FS_UnloadPacks_dlcache

Like FS_ClearSearchPath() but unloads only the packs loaded from dlcache
so we don't need to use a full FS_Rescan() to prevent
content from the previous server and/or map from interfering with the next
================
*/
void FS_UnloadPacks_dlcache(void)
{
	searchpath_t *search = fs_searchpaths, *searchprev = fs_searchpaths, *searchnext;

	while (search)
	{
		searchnext = search->next;
		if (search->pack && search->pack->dlcache) {
			Con_DPrintLinef ("Unloading pack: %s", search->pack->shortname);

			// remove it from the search path list
			if (search == fs_searchpaths)
				fs_searchpaths = search->next;
			else
				searchprev->next = search->next;

			// close the file
			FILEDESC_CLOSE(search->pack->pk3_handle);
			// free any memory associated with it
			if (search->pack->files)
				Mem_Free(search->pack->files);
			Mem_Free(search->pack);
			Mem_Free(search);
		}
		else
			searchprev = search;
		search = searchnext;
	}
}

static void FS_AddSelfPack(void)
{
	if (fs_selfpack)
	{
		searchpath_t *search;
		search = (searchpath_t *)Mem_Alloc(fs_mempool, sizeof(searchpath_t));
		search->next = fs_searchpaths;
		search->pack = fs_selfpack;
		fs_searchpaths = search;
	}
}


/*
================
FS_Rescan
================
*/
CALLERS_ (FS_Init_Dir FS_Rescan_f FS_ChangeGameDirs)
void FS_Rescan (void)
{
	int i;
	qbool fs_modified = false;
	qbool is_a_reset = false;
	char gamedirbuf[MAX_INPUTLINE_16384];
	char vabuf[1024];

	if (fs_searchpaths != NULL) {
		// Baker: This statement will always hit in Zircon
		// AddSelfPack hits fs_searchpaths = self pack.
		
		is_a_reset = true;
	} else {
		// Baker: Not a reset?
	}
	FS_ClearSearchPath(); // Baker: Totally nukes.

#ifdef _DEBUG
	FS_AddSelfPack(); // Baker: Last item?
#endif
	// automatically activate gamemode for the gamedirs specified
	if (is_a_reset) {
		// Baker: Due to self pack ... this ALWAYS happens in Zircon
		FS_Rescan_COM_ChangeGameTypeForGameDirs();
	}

	// add the game-specific paths
	// gamedirname1 (typically id1)
	FS_AddGameHierarchy (gamedirname1);
	// update the com_modname (used for server info)
	if (gamedirname2 && gamedirname2[0])
		dp_strlcpy(com_modname, gamedirname2, sizeof(com_modname));
	else
		dp_strlcpy(com_modname, gamedirname1, sizeof(com_modname));

	// add the game-specific path, if any
	// (only used for mission packs and the like, which should set fs_modified)
	if (gamedirname2 && gamedirname2[0])
	{
		fs_modified = true;
		FS_AddGameHierarchy (gamedirname2);
	}

	// -game <gamedir>
	// Adds basedir/gamedir as an override game
	// LadyHavoc: now supports multiple -game directories
	// set the com_modname (reported in server info)

	*gamedirbuf = 0;
	for (i = 0; i < fs_numgamedirs; i ++) {
		fs_modified = true;
		FS_AddGameHierarchy (fs_gamedirs[i]);
		// update the com_modname (used server info)
		strlcpy (com_modname, fs_gamedirs[i], sizeof (com_modname));
		if (i)
			strlcat(gamedirbuf, va(vabuf, sizeof(vabuf), " %s", fs_gamedirs[i]), sizeof(gamedirbuf));
		else
			strlcpy(gamedirbuf, fs_gamedirs[i], sizeof(gamedirbuf));
	}
	Cvar_SetQuick(&cvar_fs_gamedir, gamedirbuf); // so QC or console code can query it

	// add back the selfpack as new first item
#ifndef _DEBUG
	FS_AddSelfPack();
#endif

	fs_have_qex = 0; // AURA 3.2
	

	#if !defined(QUALKER) && !defined(OBJECTN) && !defined(INVADERS) && !defined(DARKNESS)
		// Baker: We are going to assume basedir here.
		va_super (QuakeEX_kpf_realpath, MAX_OSPATH_EX_1024, "%s%s", fs_basedir, "QuakeEX.kpf");
		
		if (FS_File_Exists_Unsafe (QuakeEX_kpf_realpath)) {
			Con_PrintLinef ("Found QuakeEX.kpf");
			if (FS_AddPack_Fullpath (QuakeEX_kpf_realpath, /*shortname*/ "QuakeEX.kpf", fs_reply_already_loaded_null, fs_keep_plain_dirs_false, fs_is_dlcache_false)) {
				Con_PrintLinef ("Loaded QuakeEX.kpf");
				LOC_LoadFile ();
				fs_have_qex = 1;
			} else {
				Con_PrintLinef ("Load QuakeEX.kpf failed");
			}
		} else {
			Con_PrintLinef ("Did not find QuakeEX.kpf");
		}
	#endif // !QUALKER ...

	if (cls.state != ca_dedicated) {
		// set the default screenshot name to either the mod name or the
		// gamemode screenshot name
		if (strcmp(com_modname, gamedirname1))
			Cvar_SetQuick (&scr_screenshot_name, com_modname);
		else
			Cvar_SetQuick (&scr_screenshot_name, gamescreenshotname);
	} // ! dedicated

	if ((i = Sys_CheckParm("-modname")) && i < sys.argc - 1)
		c_strlcpy(com_modname, sys.argv[i+1]);

	// If "-condebug" is in the command line, remove the previous log file
	if (Sys_CheckParm ("-condebug") != 0) {
		// deletes a name from the filesystem
		// On success, zero is returned.  On error, -1 is returned, and
       // errno is set to indicate the error.
		const char *s_log = va(vabuf, sizeof(vabuf), "%s/zircon_log.log", fs_gamedir);
		//int isok = unlink (s_log) == 0;
		unlink (s_log);
	}

	// look for the pop.lmp file and set registered to true if it is found
	if (FS_FileExists("gfx/pop.lmp"))
		Cvar_SetValueQuick(&registered, 1);
	switch(gamemode)
	{
	case GAME_NORMAL_0:
	case GAME_HIPNOTIC:
	case GAME_ROGUE:
	#if !defined(QUALKER) && !defined(OBJECTN) && !defined(INVADERS) && !defined(DARKNESS)
		if (!registered.integer)
		{
			if (fs_modified)
				Con_PrintLinef("Playing shareware version, with modification.\nwarning: most mods require full quake data.");
			else
				Con_PrintLinef("Playing shareware version.");
		}
		else
			Con_PrintLinef ("Playing registered version.");
	#endif // !QUALKER
		break;

	case GAME_STEELSTORM:
		if (registered.integer)
			Con_Print("Playing registered version.\n");
		else
			Con_Print("Playing shareware version.\n");
		break;
	default:
		break;
	}

	// unload all wads so that future queries will return the new data
	W_UnloadAll();
}

// Baker: This is actually a console command.
static void FS_Rescan_f(cmd_state_t *cmd)
{
	FS_Rescan();
}

/*
================
FS_ChangeGameDirs
Baker: This NEVER occurs at start up, only in-game gamedir change.
================
*/
extern qbool vid_opened;
CALLERS_ (FS_GameDir_f CL_ParseServerInfo CL_ParseHint)
// Baker: QW and DarkPlaces do this.
qbool FS_ChangeGameDirs(int numgamedirs, char gamedirs[][MAX_QPATH_128], qbool complain, qbool failmissing)
{
	is_ingame_gamedir_change = true; // Will never be set to false.  Only false on startup.

	int i;
	const char *p;

	cl.cl_levelshot_time = 0; // Baker: April 17 2025 - To make sure.

#if 1 // Baker: Just in case.  Pure positive.  Not whatever is interfering with Quakeworld.
	size_t size_demos = sizeof(cls.demos); // "game" NEWGAME
	memset (&cls.demos, 0, size_demos);
#endif

	// Baker: It checks for no change here.
	if (fs_numgamedirs == numgamedirs) {
		for (i = 0;i < numgamedirs;i++) {
			if (String_Match_Caseless(fs_gamedirs[i], gamedirs[i]) == false)
				break;
		} // for

		if (i == numgamedirs)
			return true; // GET OUT ...already using this set of gamedirs, do nothing
	} // Check if same gamedirs

	if (numgamedirs > MAX_GAMEDIRS_16) {
		if (complain)
			Con_PrintLinef ("That is too many gamedirs (%d > %d)", numgamedirs, MAX_GAMEDIRS_16);
		return false; // too many gamedirs
	}

	// Baker: Check for missing / nasty game dirs
	for (i = 0;i < numgamedirs;i++) {
		// if string is nasty, reject it
		p = FS_CheckGameDir(gamedirs[i]);
		if (!p) {
			if (complain)
				Con_PrintLinef ("Nasty gamedir name rejected: %s", gamedirs[i]);
			return false; // nasty gamedirs
		}
		if (p == fs_checkgamedir_missing && failmissing) {
			if (complain)
				Con_PrintLinef ("Gamedir missing: %s%s/", fs_basedir, gamedirs[i]);
			return false; // missing gamedirs
		}
	} // for

	// Baker: everything ok ... do it ..

	Key_History_Write ();  // Baker r1485: close missing history loophole by writing history during gamedir change process
	Host_SaveConfig (CONFIGFILENAME);

	fs_numgamedirs = numgamedirs;
	for (i = 0;i < fs_numgamedirs;i++)
		c_strlcpy(fs_gamedirs[i], gamedirs[i]);

	// reinitialize filesystem to detect the new paks
	FS_Rescan();

	if (cls.demoplayback) {
		CL_Disconnect_Real();
		cls.demonum = 0;
	}

	// unload all sounds so they will be reloaded from the new files as needed
	S_UnloadAllSounds_f(cmd_local);

	// restart the video subsystem after the config is executed
	Cbuf_InsertText (cmd_local, NEWLINE "loadconfig" NEWLINE "vid_restart" NEWLINE NEWLINE);

	return true;
}

/*
================
FS_GameDir_f
================
*/

// Baker: What happens if dedicated server switches game?
// I see CL_ stuff here.

void FS_PurgeAll_f (cmd_state_t *cmd)
{
	char stuff_this[MAX_QPATH_128] = {0};
	if ((cls.state == ca_connected && !cls.demoplayback) || sv.active) {
		c_strlcpy (stuff_this, cl.worldbasename);
		Con_PrintLinef ("Disconnecting...");
	}

	// halt demo playback to close the file
	CL_Disconnect_Real();

#ifdef CONFIG_MENU
	Menu_Resets (); // Cursor to 0 for all the menus
	R_Shadow_ClearWorldLights ();
	r_shadow_mapname[0] = 0; // Clear the name
#endif

	Mod_FreeQ3Shaders();
	Mod_Skeletal_FreeBuffers();

	// Baker r9003: Clear models/sounds on gamedir change
	//if (is_game_switch) {
		//Mod_PurgeUnused (); // Baker .. loadmodel: stuff from prior gamedir persisted
	Mod_PurgeALL ();
#ifdef CONFIG_MENU
	S_PurgeALL (); //S_PurgeUnused();
#endif

	// Baker r9003: Clear models/sounds on gamedir change
//	is_game_switch = true;   // This does what?  Clear models thoroughly.  As opposed to video restart which shouldn't?
//	FS_ChangeGameDirs(numgamedirs, gamedirs, q_tx_complain_true, q_fail_on_missing_true);
	// is_game_switch = false; // Baker r9062: This is not where to do this for DarkPlaces Beta
	if (*stuff_this)
		Cbuf_AddTextLinef (cmd, "map %s", stuff_this);
}

static void FS_GameDir_f(cmd_state_t *cmd)
{
	int i;
	int numgamedirs;
	char gamedirs[MAX_GAMEDIRS_16][MAX_QPATH_128];

	if (Cmd_Argc(cmd) < 2) {
		if (fs_numgamedirs) {
			Con_Printf ("gamedirs active:");
			for (i = 0;i < fs_numgamedirs;i++)
				Con_Printf (" %s", fs_gamedirs[i]);
			Con_Printf ("\n");
			Con_PrintLinef ("base game      : %s", gamedirname1);
			return;
		}

		Con_PrintLinef ("base game: %s", gamedirname1);
		return;
	}

	numgamedirs = Cmd_Argc(cmd) - 1;
	if (numgamedirs > MAX_GAMEDIRS_16) {
		Con_PrintLinef ("Too many gamedirs (%d > %d)", numgamedirs, MAX_GAMEDIRS_16);
		return;
	}

	for (i = 0;i < numgamedirs;i++)
		c_strlcpy(gamedirs[i], Cmd_Argv(cmd, i+1) );

	if ((cls.state == ca_connected && !cls.demoplayback) || sv.active) {
#if 0
		// actually, changing during game would work fine, but would be stupid
		Con_PrintLinef ("Can not change gamedir while client is connected or server is running!");
		return;
#else
		// Baker r1301: Automatically disconnect if "game" / "gamedir" is used
		Con_PrintLinef ("Disconnecting...");
#endif
	}

	// halt demo playback to close the file
	CL_Disconnect_Real();

	// Baker: no server shutdown here in dpbeta?
	if (sv.active) {
		// Shutdown?
	}

#ifdef CONFIG_MENU
	RELATED_ (FS_PurgeAll_f)
	// Baker: April 14, 2025 - I would like this stuff in FS_ChangeGameDirs really
	Menu_Resets (); // Cursor to 0 for all the menus
	R_Shadow_ClearWorldLights ();
	r_shadow_mapname[0] = 0; // Clear the name
	r_texture_mapcube_name[0]; // Baker: Here is where I guess?
#if 1
	void LoadFont2(ccs *s_fontenumname);
	LoadFont2 ("default");
	LoadFont2 ("console");
	LoadFont2 ("sbar");
	LoadFont2 ("notify");
	LoadFont2 ("chat");
	LoadFont2 ("centerprint");
	LoadFont2 ("infobar");
	LoadFont2 ("menu");
#else
	// December 12 2024 - Reset the fonts if we can ...
	Cbuf_AddTextLinef (cmd, "loadfont default");
	Cbuf_AddTextLinef (cmd, "loadfont console");
	Cbuf_AddTextLinef (cmd, "loadfont sbar");
	Cbuf_AddTextLinef (cmd, "loadfont notify");
	Cbuf_AddTextLinef (cmd, "loadfont chat");
	Cbuf_AddTextLinef (cmd, "loadfont centerprint");
	Cbuf_AddTextLinef (cmd, "loadfont infobar");
	Cbuf_AddTextLinef (cmd, "loadfont menu");
#endif

#endif

	// Baker r9003: Clear models/sounds on gamedir change
	is_game_switch = true;   // This does what?  Clear models thoroughly.  As opposed to video restart which shouldn't?
	FS_ChangeGameDirs(numgamedirs, gamedirs, q_tx_complain_true, q_fail_on_missing_true);
	// is_game_switch = false; // Baker r9062: This is not where to do this for DarkPlaces Beta
}

static const char *FS_SysCheckGameDir(const char *gamedir, char *buf, size_t buflength)
{
	qbool success;
	qfile_t *f;
	stringlist_t list;
	fs_offset_t n;
	char vabuf[1024];

	stringlistinit(&list);
#ifdef _WIN32
	// We're ok here.  This checks a folder.
#endif

	stringlist_append_listdirectory(&list, gamedir, fs_pattern_all_empty_string);
	success = list.numstrings > 0;
	stringlistfreecontents(&list);

	if (success) {
		va(vabuf, sizeof(vabuf), "%smodinfo.txt", gamedir);
		f = FS_SysOpen(vabuf, "r", fs_nonblocking_false);
		if (f) {
			n = FS_Read (f, buf, buflength - 1);
			if (n >= 0)
				buf[n] = 0;
			else
				*buf = 0;
			FS_Close(f);
		}
		else
			*buf = 0;
		return buf;
	}

	return NULL;
}

/*
================
FS_CheckGameDir
================
*/
// Baker: What the hell is the return value of this?
// Baker: It returns an empty string of "" almost all the time.
// Or it returns a "missing" string or NULL
const char *FS_CheckGameDir(const char *gamedir)
{
	const char *ret;
	static char buf[8192];
	char vabuf[1024];

	if (FS_CheckNastyPath(gamedir, fs_isgamedir_true))
		return NULL;

	va(vabuf, sizeof(vabuf), "%s%s/", fs_userdir, gamedir);
	ret = FS_SysCheckGameDir(vabuf, buf, sizeof(buf));
	if (ret) {
		if (!*ret) {
			// get description from basedir
			va(vabuf, sizeof(vabuf), "%s%s/", fs_basedir, gamedir);
			ret = FS_SysCheckGameDir(vabuf, buf, sizeof(buf));
			if (ret)
				return ret;
			return "";
		}
		return ret;
	}

	RELATED_ (FS_RealFilePath_Z_Alloc)
	va(vabuf, sizeof(vabuf), "%s%s/", fs_basedir, gamedir);
	ret = FS_SysCheckGameDir(vabuf, buf, sizeof(buf));
	if (ret)
		return ret;

#if 1 // Baker: Check for a empty existing folder.
	char sgdwork[1024];
	c_strlcpy (sgdwork, fs_gamedir);					// "id1/"
	File_URL_Remove_Trailing_Unix_Slash (sgdwork);		// "id1"
	File_URL_Edit_Reduce_To_Parent_Path_Trailing_Slash (sgdwork); // Should be parent

	char strydir[1024];
	c_dpsnprintf2 (strydir, "%s%s", sgdwork, gamedir);
	int result = FS_SysFileOrDirectoryType (strydir);

	if (result == FS_FILETYPE_DIRECTORY_2) {
		return "";
	}
#endif

	return fs_checkgamedir_missing;
}


static unsigned char *FS_LoadAndCloseQFile (qfile_t *file, const char *path, mempool_t *pool, qbool quiet, fs_offset_t *filesizepointer);
// Return value true if found a total conversion or flex
static int FS_Baker_ListGameDirs_Is_Total_Conversion (void)
{
	char vabuf[1024];
	stringlist_t all_files_in_basedir_list = {0};

	// Baker: This is not working how I would expect on Windows
	// It is sending "/" to FindFirstFileW
	// which is looking through the root directory of my drive checking files
#ifdef _WIN32 // modlist.txt
	//const char *s_pwd = Sys_Getcwd_SBuf();
	if (fs_basedir[0] == NULL_CHAR_0)
		c_strlcpy (vabuf, ""); // Baker: This should work?
	else
#endif
	{
		va (vabuf, sizeof(vabuf), "%s/", fs_basedir);
	}

	stringlist_append_listdirectory (&all_files_in_basedir_list, vabuf, fs_pattern_all_empty_string); // concats, despite name

	if (fs_userdir[0]) {
		va (vabuf, sizeof(vabuf), "%s/", fs_userdir);
		stringlist_append_listdirectory (&all_files_in_basedir_list, vabuf, fs_pattern_all_empty_string);
	}

	stringlistsort (&all_files_in_basedir_list, fs_make_unique_true);

	RELATED_ (FS_RealFilePath_Z_Alloc FS_SysFileOrDirectoryType)

	stringlist_t folders_in_basedir_list = {0};

	for (int idx = 0; idx < all_files_in_basedir_list.numstrings; idx ++) {
		const char *s			= all_files_in_basedir_list.strings[idx];
		//char *sreal_za		= NULL;

		int filetype			= FS_SysFileOrDirectoryType (s);
		int is_dir				= filetype == FS_FILETYPE_DIRECTORY_2;

		if (filetype == FS_FILETYPE_NONE_0)
			continue; // Not sure, but bad ...

		if (!is_dir)
			continue;

		// Baker: bin32/bin64 are for dlls, "downloads" is for QuakeInjector
		if (String_Isin3 (s, "bin32", "bin64", "downloads"))
			continue;

		// Gamedir, possible only dir.
		s = s;
		stringlistappend (&folders_in_basedir_list, s);
	} // idx

	stringlistfreecontents (&all_files_in_basedir_list); // Done with that

	char old_fs_gamedir[1024];
	c_strlcpy (old_fs_gamedir, fs_gamedir);
	while (folders_in_basedir_list.numstrings == 1  && String_Match(folders_in_basedir_list.strings[0], "id1") == false) {
		c_strlcpy (fs_gamedir, folders_in_basedir_list.strings[0]); // We have to do this for file read to work

		c_strlcpy (mod_list_folder_name, folders_in_basedir_list.strings[0]); // "pac"

		mod_list_requires = REQUIRES_WHAT_STANDALONE_1; // Assume standalone
		c_strlcpy (mod_list_game_window_title, mod_list_folder_name); // Default to folder name.
		c_strlcpy (mod_list_server_filter_name, mod_list_folder_name); // Default to folder name.
		Mem_FreeNull_ (mod_list_game_icon_base64_zalloc); // NONE!

		// #contents.pk3 -- if found, this is a classified gamedir mod.
		va_super (tmp, 1024, "%s/#contents.pk3", mod_list_folder_name); // modinfo.txt except with underscore


		qfile_t *f = FS_SysOpen (tmp, FS_MODE_READ_BINARY_RB, fs_nonblocking_false);

		if (!f)
			break; // Not a #contents mod.

		FS_CloseNULL_ (f);

		// mod_info.txt
		va_super (tmp2, 1024, "%s/mod_info.txt", mod_list_folder_name); // modinfo.txt except with underscore
		fs_offset_t filesize;
//		f = FS_SysOpen (tmp, FS_MODE_READ_BINARY_RB, fs_nonblocking_false);
//		if (!f)
//			break;

		//char *filedata = (char *)FS_LoadAndCloseQFile(f, tmp2, zonemempool, fs_quiet_true, &filesize);
		char *filedata = (char *)FS_SysLoadFile(tmp2, zonemempool, fs_quiet_true, &filesize);

		if (!filedata)
			break;

		// We have modinfo

		// mod_list.pk3 ... note the underscore.  This is not "shitty" modlist.txt that has title.
		// "game_title" "Packard Man"			// We will take that and substitute "_" for spaces for gamenetworkfiltername, screenshot name
		// "requirements" "total_conversion"	// "total_conversion", "quake", "flexible"
		// gameuserdirname= folder we are in
		// "iconbase64" - a base64 encoded JPEG icon of 48 x 48 https://base64.guru/converter/encode/image/jpg

		const char *s;
		s = String_Worldspawn_Value_For_Key_Sbuf (filedata, "game_title");
		if (s) {
			c_strlcpy (mod_list_game_window_title, s);
			c_strlcpy (mod_list_server_filter_name, s);	// Default - underscorized mod_list_game_window_title
			String_Edit_Replace (mod_list_server_filter_name, sizeof(mod_list_server_filter_name), " ", "_");
		}

		s = String_Worldspawn_Value_For_Key_Sbuf (filedata, "requirements");
		if (s) {
			if (String_Match (s, "total_conversion")) mod_list_requires = REQUIRES_WHAT_STANDALONE_1;
			else if (String_Match (s, "quake")) mod_list_requires = REQUIRES_WHAT_QUAKE_0;
			else if (String_Match (s, "flexible")) mod_list_requires = REQUIRES_WHAT_FLEXIBLE_2;
		} // Haha

		s = String_Worldspawn_Value_For_Key_Sbuf (filedata, "iconbase64");
		if (s) {
			mod_list_game_icon_base64_zalloc = Z_StrDup (s);
		}

		// Format:
		//gamename = solo_total_conversion;		// I think this is the status bar and menu title
		//gamedirname1 = solo_total_conversion;  // gamedirname1 ... solo_total_conversion
		//gamenetworkfiltername = gamescreenshotname = gameuserdirname = solo_total_conversion;
		//gamenetworkfiltername like DarkPlaces-Quake
		break;
	} // while 1
	c_strlcpy (fs_gamedir, old_fs_gamedir);

	stringlistfreecontents (&folders_in_basedir_list); // Done with that

	return mod_list_folder_name[0] != NULL_CHAR_0;
}

static void FS_ListGameDirs(void)
{
	const char *game_mod_info;
	char vabuf[1024];

	fs_all_gamedirs_count = 0;
	
	if (fs_all_gamedirs)
		Mem_Free(fs_all_gamedirs);

	stringlist_t list = {0};

	// Baker: This is not working how I would expect on Windows
	// It is sending "/" to FindFirstFileW
	// which is looking through the root directory of my drive checking files
#ifdef _WIN32 // modlist.txt
	//const char *s_pwd = Sys_Getcwd_SBuf();
	if (fs_basedir[0])
		va(vabuf, sizeof(vabuf), "%s/", fs_basedir);
	else c_strlcpy (vabuf, ""); // Baker: This should work?
#else
	va(vabuf, sizeof(vabuf), "%s/", fs_basedir);
#endif

	stringlist_append_listdirectory (&list, vabuf, fs_pattern_all_empty_string);
	
	if (fs_userdir[0]) {
		// Baker: Prevent it from listing if not used.
		va(vabuf, sizeof(vabuf), "%s/", fs_userdir);
		stringlist_append_listdirectory(&list, vabuf, fs_pattern_all_empty_string);
	}
	stringlistsort_unique (&list);

	stringlist_t list2 = {0};
	
	for (int i = 0; i < list.numstrings; i ++) {
		
#if 0
		// Baker: This is a unique check, we make the sort above do unique
		if (i)
			if (String_Match(list.strings[i-1], list.strings[i]))
				continue;
#endif

		game_mod_info = FS_CheckGameDir(list.strings[i]);
		if (!game_mod_info)
			continue;
		if (game_mod_info == fs_checkgamedir_missing)
			continue;
		if (!*game_mod_info)
			continue;
		stringlistappend(&list2, list.strings[i]);
	}
	stringlistfreecontents(&list);

	fs_all_gamedirs = (gamedir_t *)Mem_Alloc(fs_mempool, list2.numstrings * sizeof(*fs_all_gamedirs));
	for (int i = 0; i < list2.numstrings; i ++) {
		game_mod_info = FS_CheckGameDir(list2.strings[i]);
		// all this cannot happen any more, but better be safe than sorry
		if (!game_mod_info)
			continue;
		if (game_mod_info == fs_checkgamedir_missing)
			continue;
		if (!*game_mod_info)
			continue;
		c_strlcpy (fs_all_gamedirs[fs_all_gamedirs_count].name, list2.strings[i]);
		c_strlcpy (fs_all_gamedirs[fs_all_gamedirs_count].description, game_mod_info);
		fs_all_gamedirs_count ++;
	}
}

/*
#ifdef _WIN32
#pragma comment(lib, "shell32.lib")
#include <ShlObj.h>
#endif
*/

static void COM_InsertFlags(const char *buf) {
	const char *p;
	char *q;
	const char **new_argv;
	int i = 0;
	int args_left = 256;
	new_argv = (const char **)Mem_Alloc(fs_mempool, sizeof(*sys.argv) * (sys.argc + args_left + 2));
	if (sys.argc == 0)
		new_argv[0] = "dummy";  // Can't really happen.
	else
		new_argv[0] = sys.argv[0];
	++i;
	p = buf;
	while(COM_ParseToken_Console(&p))
	{
		size_t sz = strlen(com_token) + 1; // shut up clang
		if (i > args_left)
			break;
		q = (char *)Mem_Alloc(fs_mempool, sz);
		strlcpy(q, com_token, sz);
		new_argv[i] = q;
		++i;
	}
	// Now: i <= args_left + 1.
	if (sys.argc >= 1)
	{
		memcpy((char *)(&new_argv[i]), &sys.argv[1], sizeof(*sys.argv) * (sys.argc - 1));
		i += sys.argc - 1;
	}
	// Now: i <= args_left + (sys.argc || 1).
	new_argv[i] = NULL;
	sys.argv = new_argv;
	sys.argc = i;
}

static int FS_ChooseUserDir(userdirmode_t userdirmode, char *userdir, size_t userdirsize)
{
#if defined(__IPHONEOS__)
	if (userdirmode == USERDIRMODE_HOME)
	{
		// fs_basedir is "" by default, to utilize this you can simply add your gamedir to the Resources in xcode
		// fs_userdir stores configurations to the Documents folder of the app
		strlcpy(userdir, "../Documents/", MAX_OSPATH);
		return 1;
	}
	return -1;

#elif defined(_WIN32)
	char homedir[MAX_OSWPATH];
	wchar *homedirw;
#if _MSC_VER >= 1400
	size_t homedirwlen;
#endif
	char mydocsdir[MAX_OSWPATH];
	wchar mydocsdirw[MAX_OSWPATH];
	char savedgamesdir[MAX_OSWPATH];
	wchar *savedgamesdirw;
	int fd;
	char vabuf[1024];

	userdir[0] = 0;
	switch(userdirmode)
	{
	default:
		return -1;
	case USERDIRMODE_NOHOME:
		strlcpy(userdir, fs_basedir, userdirsize);
		break;
	case USERDIRMODE_MYGAMES:
		if (!shfolder_dll)
			Sys_LoadDependency(shfolderdllnames, &shfolder_dll, shfolderfuncs);
		mydocsdir[0] = 0;
		if (qSHGetFolderPath && qSHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, mydocsdirw) == S_OK) {
			NARROW(mydocsdirw, mydocsdir);
			dpsnprintf(userdir, userdirsize, "%s/My Games/%s/", mydocsdir, gameuserdirname);
			break;
		}
#if _MSC_VER >= 1400
		_wdupenv_s(&homedirw, &homedirwlen, L"USERPROFILE");
		NARROW(homedirw, homedir);
		if (homedir[0]) {
			dpsnprintf(userdir, userdirsize, "%s/.%s/", homedir, gameuserdirname);
			free(homedirw);
			break;
		}
#else
		homedirw = _wgetenv(L"USERPROFILE");
		NARROW(homedirw, homedir);
		if (homedir[0]) {
			dpsnprintf(userdir, userdirsize, "%s/.%s/", homedir, gameuserdirname);
			break;
		}
#endif
		return -1;
	case USERDIRMODE_SAVEDGAMES:
		if (!shell32_dll)
			Sys_LoadDependency(shell32dllnames, &shell32_dll, shell32funcs);
		if (!ole32_dll)
			Sys_LoadDependency(ole32dllnames, &ole32_dll, ole32funcs);
		if (qSHGetKnownFolderPath && qCoInitializeEx && qCoTaskMemFree && qCoUninitialize)
		{
			savedgamesdir[0] = 0;
			qCoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
/*
#ifdef __cplusplus
			if (SHGetKnownFolderPath(FOLDERID_SavedGames, KF_FLAG_CREATE | KF_FLAG_NO_ALIAS, NULL, &savedgamesdirw) == S_OK)
#else
			if (SHGetKnownFolderPath(&FOLDERID_SavedGames, KF_FLAG_CREATE | KF_FLAG_NO_ALIAS, NULL, &savedgamesdirw) == S_OK)
#endif
*/
			if (qSHGetKnownFolderPath(&qFOLDERID_SavedGames, qKF_FLAG_CREATE | qKF_FLAG_NO_ALIAS, NULL, &savedgamesdirw) == S_OK)
			{
				NARROW(savedgamesdirw, savedgamesdir);
				qCoTaskMemFree(savedgamesdirw);
			}
			qCoUninitialize();
			if (savedgamesdir[0])
			{
				dpsnprintf(userdir, userdirsize, "%s/%s/", savedgamesdir, gameuserdirname);
				break;
			}
		}
		return -1;
	}
#else
	int fd;
	char *homedir;
	char vabuf[1024];
	userdir[0] = 0;
	switch(userdirmode)
	{
	default:
		return -1;
	case USERDIRMODE_NOHOME:
		strlcpy(userdir, fs_basedir, userdirsize);
		break;
	case USERDIRMODE_HOME:
		homedir = getenv("HOME");
		if (homedir)
		{
			dpsnprintf(userdir, userdirsize, "%s/.%s/", homedir, gameuserdirname);
			break;
		}
		return -1;
	case USERDIRMODE_SAVEDGAMES:
		homedir = getenv("HOME");
		if (homedir)
		{
#ifdef MACOSX
			dpsnprintf(userdir, userdirsize, "%s/Library/Application Support/%s/", homedir, gameuserdirname);
#else
			// the XDG say some files would need to go in:
			// XDG_CONFIG_HOME (or ~/.config/%s/)
			// XDG_DATA_HOME (or ~/.local/share/%s/)
			// XDG_CACHE_HOME (or ~/.cache/%s/)
			// and also search the following global locations if defined:
			// XDG_CONFIG_DIRS (normally /etc/xdg/%s/)
			// XDG_DATA_DIRS (normally /usr/share/%s/)
			// this would be too complicated...
			return -1;
#endif
			break;
		}
		return -1;
	}
#endif


#if !defined(__IPHONEOS__)

#ifdef _WIN32
	// historical behavior...
	if (userdirmode == USERDIRMODE_NOHOME && String_NOT_Match(gamedirname1, "id1"))
		return 0; // don't bother checking if the basedir folder is writable, it's annoying...  unless it is Quake on Windows where NOHOME is the default preferred and we have to check for an error case
#endif

	// see if we can write to this path (note: won't create path)
#ifdef _WIN32
	// no access() here, we must try to open the file for appending
	fd = FS_SysOpenFiledesc(va(vabuf, sizeof(vabuf), "%s%s/config.cfg", userdir, gamedirname1), "a", false);
	if (fd >= 0)
		FILEDESC_CLOSE(fd);
#else
	// on Unix, we don't need to ACTUALLY attempt to open the file
	if (access(va(vabuf, sizeof(vabuf), "%s%s/", userdir, gamedirname1), W_OK | X_OK) >= 0)
		fd = 1;
	else
		fd = -1;
#endif
	if (fd >= 0)
	{
		return 1; // good choice - the path exists and is writable
	}
	else
	{
		if (userdirmode == USERDIRMODE_NOHOME)
			return -1; // path usually already exists, we lack permissions
		else
			return 0; // probably good - failed to write but maybe we need to create path
	}
#endif
}


#include "fs_baker_csg.c.h"

static void FS_Init_Dir (void)
{
	const char *p;
	int i;
	int is_forced = false; // Baker r1001: -nohome is the behavior on Windows and Mac
	WPATHDEF(fs_basedirw);

	*fs_basedir = 0;
	*fs_userdir = 0;
	*fs_gamedir = 0;

	// -basedir <path>
	// Overrides the system supplied base directory (under GAMENAME)
// COMMANDLINEOPTION: Filesystem: -basedir <path> chooses what base directory the game data is in, inside this there should be a data directory for the game (for example id1)


	int j = Sys_CheckParm("-csg");
	if (j && j + 1 < sys.argc) {
		CSG_Perform (j);
		exit (0); // Baker: Should be unreachable.
	}

	/*int */ j = Sys_CheckParm("-q13");
	if (j && j + 1 < sys.argc) {
		Q13_Perform (j);
		exit (0); // Baker: Should be unreachable because Q13_Perform does an exit(0)
	}

	i = Sys_CheckParm ("-basedir");
	if (i && i < sys.argc-1) {
		dp_strlcpy (fs_basedir, sys.argv[i+1], sizeof (fs_basedir));
		i = (int)strlen (fs_basedir);
		if (i > 0 && (fs_basedir[i-1] == '\\' || fs_basedir[i-1] == '/'))
			fs_basedir[i-1] = 0;
		is_forced = true; // // Baker r1001: -nohome is the behavior on Windows and Mac
	}
	else
	{
// If the base directory is explicitly defined by the compilation process
#ifdef DP_FS_BASEDIR // Baker: This is not defined.
		dp_strlcpy(fs_basedir, DP_FS_BASEDIR, sizeof(fs_basedir));
#elif defined(__ANDROID__)
		dpsnprintf(fs_basedir, sizeof(fs_basedir), "/sdcard/%s/", gameuserdirname);
#elif defined(MACOSX)
		// FIXME: is there a better way to find the directory outside the .app, without using Objective-C?
		if (strstr(sys.argv[0], ".app/")) {
			char *split;
			dp_strlcpy(fs_basedir, sys.argv[0], sizeof(fs_basedir));
			split = strstr(fs_basedir, ".app/");
			if (split)
			{
				struct stat statresult;
				char vabuf[1024];
				// truncate to just after the .app/
				split[5] = 0;
				// see if gamedir exists in Resources
				if (stat(va(vabuf, sizeof(vabuf), "%s/Contents/Resources/%s", fs_basedir, gamedirname1), &statresult) == 0)
				{
					// found gamedir inside Resources, use it
					dp_strlcat(fs_basedir, "Contents/Resources/", sizeof(fs_basedir));
				}
				else
				{
					// no gamedir found in Resources, gamedir is probably
					// outside the .app, remove .app part of path
					while (split > fs_basedir && *split != '/')
						split--;
					*split = 0;
				}
			}
		}
#else // Not android mac or dpbasedir
		// use the working directory
//		getcwd (fs_basedir, sizeof(fs_basedir));
	// use the working directory
	#ifdef _WIN32
		GetCurrentDirectoryW(sizeof(fs_basedirw) / sizeof(*fs_basedirw), fs_basedirw);
		NARROW(fs_basedirw, fs_basedir);
	#else
		getcwd(fs_basedir, sizeof(fs_basedir));
	#endif
#endif
	}

	// make sure the appending of a path separator won't create an unterminated string
	memset(fs_basedir + sizeof(fs_basedir) - 2, 0, 2);
	// add a path separator to the end of the basedir if it lacks one
	if (fs_basedir[0] && fs_basedir[strlen(fs_basedir) - 1] != '/' && fs_basedir[strlen(fs_basedir) - 1] != '\\')
		dp_strlcat(fs_basedir, "/", sizeof(fs_basedir));

	// Add the personal game directory
	if ((i = Sys_CheckParm("-userdir")) && i < sys.argc - 1) {
		is_forced = true; // -userdir // Baker r1001: -nohome is the behavior on Windows and Mac
		c_dpsnprintf1 (fs_userdir, "%s/", sys.argv[i + 1]);
	} // if
	else if (Sys_CheckParm("-nohome"))
		*fs_userdir = 0; // user wants roaming installation, no userdir
	else
	{
#ifdef DP_FS_USERDIR
		// Baker: This is not the norm.
		// Someone would need to define DP_FS_USERDIR to a path
		dp_strlcpy(fs_userdir, DP_FS_USERDIR, sizeof(fs_userdir));
#else
		int dirmode;
		int highestuserdirmode = USERDIRMODE_COUNT - 1;
		int preferreduserdirmode = USERDIRMODE_COUNT - 1;
		int userdirstatus[USERDIRMODE_COUNT];
# if defined(_WIN32) || defined(MACOSX)
		// historical behavior...
		if (String_Match(gamedirname1, "id1"))
			preferreduserdirmode = USERDIRMODE_NOHOME;
# endif
		// check what limitations the user wants to impose
		if (Sys_CheckParm("-home")) preferreduserdirmode = USERDIRMODE_HOME;
		if (Sys_CheckParm("-mygames")) preferreduserdirmode = USERDIRMODE_MYGAMES;
		if (Sys_CheckParm("-savedgames")) preferreduserdirmode = USERDIRMODE_SAVEDGAMES;
		// gather the status of the possible userdirs
		for (dirmode = 0; dirmode < USERDIRMODE_COUNT; dirmode++)
		{
			userdirstatus[dirmode] = FS_ChooseUserDir((userdirmode_t)dirmode, fs_userdir, sizeof(fs_userdir));
			if (userdirstatus[dirmode] == 1)
				Con_DPrintLinef ("userdir %d = %s (writable)", dirmode, fs_userdir);
			else if (userdirstatus[dirmode] == 0)
				Con_DPrintLinef ("userdir %d = %s (not writable or does not exist)", dirmode, fs_userdir);
			else
				Con_DPrintLinef ("userdir %d (not applicable)", dirmode);
		}
		// some games may prefer writing to basedir, but if write fails we
		// have to search for a real userdir...
		if (preferreduserdirmode == 0 && userdirstatus[0] < 1)
			preferreduserdirmode = highestuserdirmode;
		// check for an existing userdir and continue using it if possible...
		for (dirmode = USERDIRMODE_COUNT - 1;dirmode > 0;dirmode--)
			if (userdirstatus[dirmode] == 1)
				break;
		// if no existing userdir found, make a new one...
		if (dirmode == 0 && preferreduserdirmode > 0) {
			for (dirmode = preferreduserdirmode; dirmode > 0; dirmode--) {
				if (userdirstatus[dirmode] >= 0)
					break;
			}
		}
		// and finally, we picked one...
		FS_ChooseUserDir((userdirmode_t)dirmode, fs_userdir, sizeof(fs_userdir));
		Con_DPrintLinef ("userdir %d is the winner", dirmode);
#endif
	}

// NOHOME IS THE BEHAVIOR EVERYWHERE EXCEPT LINUX
// Baker r1001: -nohome is the behavior on Windows and Mac
#if defined(_WIN32) || defined(MACOSX)
	// Baker 7000 nohome is the behavior on Windows unless userdir specified regardless of game
	// Or -home
	// Quake on a Mac should be simple like Windows.
	if ((i = Sys_CheckParm("-userdir")) && i < sys.argc - 1) {

	} else if ((i = Sys_CheckParm("-home"))/**/) {

	} else {
		*fs_userdir = 0; // user wants roaming installation, no userdir
	}
#endif

	// if userdir equal to basedir, clear it to avoid confusion later
	if (String_Match(fs_basedir, fs_userdir))
		fs_userdir[0] = 0;

#if 1
	// Baker: modlist.txt -- Baker this WAS broken on Windows and still is
	// slightly broken.  And moderately useless even if I invested the time to make it work.
	// The reason is it only can list the titles.
	RELATED_ (fs_all_gamedirs)
	FS_ListGameDirs(); // modlist.txt
#endif


	p = FS_CheckGameDir(gamedirname1);
	if (!p || p == fs_checkgamedir_missing)
		Con_PrintLinef (CON_WARN "WARNING: base gamedir %s%s/ not found!", fs_basedir, gamedirname1);

	if (gamedirname2) {
		p = FS_CheckGameDir(gamedirname2);
		if (!p || p == fs_checkgamedir_missing)
			Con_PrintLinef (CON_WARN "WARNING: base gamedir %s%s/ not found!", fs_basedir, gamedirname2);
	}

	// Baker r0009: Use -data directory if no -game specified and id1 does not exist
#if 1
	if ( (!p || p == fs_checkgamedir_missing) && !Sys_CheckParm ("-game") && fs_is_zircon_galaxy == false) { // GALAXY #5 (SYSINIT)

		p = FS_CheckGameDir("data");

		if (p && p != fs_checkgamedir_missing) {
			// add the gamedir to the list of active gamedirs
			c_strlcpy (fs_gamedirs[fs_numgamedirs], "data");
			fs_numgamedirs++;
			Con_PrintLinef ("Detected data directory, override to -game data");
			goto baker_data_bypass;
		}
	}
#endif

	// -game <gamedir>
	// Adds basedir/gamedir as an override game
	// LadyHavoc: now supports multiple -game directories
	for (i = 1;i < sys.argc && fs_numgamedirs < MAX_GAMEDIRS_16;i++)
	{
		if (!sys.argv[i])
			continue;
		if (String_Match (sys.argv[i], "-game") && i < sys.argc-1)
		{
			i++;
			p = FS_CheckGameDir(sys.argv[i]);
			if (!p)
				Con_PrintLinef ("WARNING: Nasty -game name rejected: %s", sys.argv[i]);
			if (p == fs_checkgamedir_missing)
				Con_PrintLinef (CON_WARN "WARNING: -game %s%s/ not found!", fs_basedir, sys.argv[i]);
			// add the gamedir to the list of active gamedirs
			strlcpy (fs_gamedirs[fs_numgamedirs], sys.argv[i], sizeof(fs_gamedirs[fs_numgamedirs]));
			fs_numgamedirs++;
		}
	}

baker_data_bypass:
	// generate the searchpath
	FS_Rescan();

	if (Thread_HasThreads())
		fs_mutex = Thread_CreateMutex();
}

/*
================
FS_Init_SelfPack
================
*/
void FS_Init_SelfPack (void)
{
	char *buf;

	// Load darkplaces.opt from the FS.
	if (!Sys_CheckParm("-noopt"))
	{
		buf = (char *) FS_SysLoadFile("darkplaces.opt", tempmempool, true, NULL);
		if (buf) {
			COM_InsertFlags(buf);
			Mem_Free(buf);
		}
	} // noopt

#ifndef USE_RWOPS // This is not defined
	// Baker: This appears to be the norm

	// Provide the SelfPack.
	if (!Sys_CheckParm("-noselfpack") && sys.selffd >= 0) {
		fs_selfpack = FS_LoadPackPK3FromFD(sys.argv[0], sys.selffd, true);
		if (fs_selfpack) { // Baker: This is returning NULL
			FS_AddSelfPack();
			if (!Sys_CheckParm("-noopt")) {
				buf = (char *) FS_LoadFile("darkplaces.opt", tempmempool, fs_quiet_true, fs_size_ptr_null);
				if (buf) {
					COM_InsertFlags(buf);
					Mem_Free(buf);
				}
			} // noopt
		}
	} // noselfpack
#endif // USE_RWOPS
}


/*
================
FS_Shutdown
================
*/
void FS_Shutdown (void)
{
	// close all pack files and such
	// (hopefully there aren't any other open files, but they'll be cleaned up
	//  by the OS anyway)
	FS_ClearSearchPath();
	Mem_FreePool (&fs_mempool);
	PK3_CloseLibrary ();

#ifdef _WIN32
	Sys_FreeLibrary (&shfolder_dll);
	Sys_FreeLibrary (&shell32_dll);
	Sys_FreeLibrary (&ole32_dll);
#endif

	if (fs_mutex)
		Thread_DestroyMutex(fs_mutex);
}

/*UNSAFE*/ static filedesc_t FS_SysOpenFiledesc(const char *realpath, const char *mode, qbool nonblocking)
{
	filedesc_t handle = FILEDESC_INVALID;
	int mod, opt;
	unsigned int ind;
	qbool dolock = false;
	WPATHDEF(filepathw); // Baker: This just does wchar var[MAX_OSWPATH /*1024*/] on Win32 and nothing if non-win32

	// Parse the mode string
	switch (mode[0]) {
		case 'r':
			mod = O_RDONLY;
			opt = 0;
			break;
		case 'w':
			mod = O_WRONLY;
			opt = O_CREAT | O_TRUNC;
			break;
		case 'a':
			mod = O_WRONLY;
			opt = O_CREAT | O_APPEND;
			break;
		default:
			Con_PrintLinef (CON_ERROR "FS_SysOpen(%s, %s): invalid mode", realpath, mode);
			return FILEDESC_INVALID;
	}
	for (ind = 1; mode[ind] != '\0'; ind++)
	{
		switch (mode[ind])
		{
			case '+':
				mod = O_RDWR;
				break;
			case 'b':
				opt |= O_BINARY;
				break;
			case 'l':
				dolock = true;
				break;
			default:
				Con_PrintLinef (CON_ERROR "FS_SysOpen(%s, %s): unknown character in mode (%c)\n",
							realpath, mode, mode[ind]);
		}
	}

	if (nonblocking)
		opt |= O_NONBLOCK;

	if (Sys_CheckParm("-readonly") && mod != O_RDONLY)
		return FILEDESC_INVALID;

#if USE_RWOPS
	if (dolock)
		return FILEDESC_INVALID;
	handle = SDL_RWFromFile(filepath, mode);
#else
# ifdef _WIN32
	// #define WIDE(str, wstr) 
	//fromwtf8(str, strlen(str), wstr, strlen(str))
	WIDE(realpath, filepathw);
#  if _MSC_VER >= 1400 // 1400 is visual studio 2005
	_wsopen_s(&handle, filepathw, mod | opt, (dolock ? ((mod == O_RDONLY) ? _SH_DENYRD : _SH_DENYRW) : _SH_DENYNO), _S_IREAD | _S_IWRITE);
#  else
	handle = _wsopen (filepathw, mod | opt, (dolock ? ((mod == O_RDONLY) ? _SH_DENYRD : _SH_DENYRW) : _SH_DENYNO), _S_IREAD | _S_IWRITE);
#  endif
# else
	handle = open (realpath, mod | opt, 0666);
	if (handle >= 0 && dolock)
	{
		struct flock l;
		l.l_type = ((mod == O_RDONLY) ? F_RDLCK : F_WRLCK);
		l.l_whence = SEEK_SET;
		l.l_start = 0;
		l.l_len = 0;
		if (fcntl(handle, F_SETLK, &l) == -1)
		{
			FILEDESC_CLOSE(handle);
			handle = -1;
		}
	}
# endif
#endif

	return handle;
}

CALLERS_ (Exclusive: Sys_ProvideSelfFD)
/*REALPATH*/ int FS_SysOpenFD(const char *realpath, const char *mode, qbool nonblocking)
{
#ifdef USE_RWOPS
	return -1;
#else
	return FS_SysOpenFiledesc(realpath, mode, nonblocking);
#endif
}



/*
===========
FS_OpenPackedFile

Open a packed file using its package file descriptor
===========
*/
static qfile_t *FS_OpenPackedFile (pack_t *pack, int pack_ind)
{
	packfile_t *pfile;
	filedesc_t dup_handle;
	qfile_t *file;

	pfile = &pack->files[pack_ind];

	// If we don't have the true offset, get it now
	if (! (pfile->flags & PACKFILE_FLAG_TRUEOFFS))
		if (!PK3_GetTrueFileOffset (pfile, pack))
			return NULL;

#ifndef LINK_TO_ZLIB
	// No Zlib DLL = no compressed files
	if (!zlib_dll && (pfile->flags & PACKFILE_FLAG_DEFLATED))
	{
		Con_PrintLinef (CON_WARN "WARNING: can't open the compressed file %s"
					"You need the Zlib DLL to use compressed files\n",
					pfile->name);
		return NULL;
	}
#endif

	// LadyHavoc: FILEDESC_SEEK affects all duplicates of a handle so we do it before
	// the dup() call to avoid having to close the dup_handle on error here
	if (FILEDESC_SEEK (pack->pk3_handle, pfile->offset, SEEK_SET) == -1)
	{
		Con_PrintLinef ("FS_OpenPackedFile: can't lseek to %s in %s (offset: %08x%08x)",
					pfile->name, pack->filename, (unsigned int)(pfile->offset >> 32), (unsigned int)(pfile->offset));
		return NULL;
	}

	dup_handle = FILEDESC_DUP (pack->filename, pack->pk3_handle);
	if (!FILEDESC_ISVALID(dup_handle))
	{
		Con_Printf ("FS_OpenPackedFile: can't dup package's handle (pack: %s)\n", pack->filename);
		return NULL;
	}

	file = (qfile_t *)Mem_Alloc (fs_mempool, sizeof (*file));
	memset (file, 0, sizeof (*file));
	file->handle = dup_handle;
	file->flags = QFILE_FLAG_PACKED;
	file->real_length = pfile->realsize;
	file->offset = pfile->offset;
	file->position = 0;
	file->ungetc = EOF;

	if (pfile->flags & PACKFILE_FLAG_DEFLATED)
	{
		ztoolkit_t *ztk;

		file->flags |= QFILE_FLAG_DEFLATED;

		// We need some more variables
		ztk = (ztoolkit_t *)Mem_Alloc (fs_mempool, sizeof (*ztk));

		ztk->comp_length = pfile->packsize;

		// Initialize zlib stream
		ztk->zstream.next_in = ztk->input;
		ztk->zstream.avail_in = 0;

		/* From Zlib's "unzip.c":
		 *
		 * windowBits is passed < 0 to tell that there is no zlib header.
		 * Note that in this case inflate *requires* an extra "dummy" byte
		 * after the compressed stream in order to complete decompression and
		 * return Z_STREAM_END.
		 * In unzip, i don't wait absolutely Z_STREAM_END because I known the
		 * size of both compressed and uncompressed data
		 */
		if (qz_inflateInit2 (&ztk->zstream, -MAX_WBITS) != Z_OK)
		{
			Con_Printf ("FS_OpenPackedFile: inflate init error (file: %s)\n", pfile->name);
			FILEDESC_CLOSE(dup_handle);
			Mem_Free(file);
			return NULL;
		}

		ztk->zstream.next_out = file->buff;
		ztk->zstream.avail_out = sizeof (file->buff);

		file->ztk = ztk;
	}

	return file;
}

/*
====================
FS_CheckNastyPath

Return true if the path should be rejected due to one of the following:
1: path elements that are non-portable
2: path elements that would allow access to files outside the game directory,
   or are just not a good idea for a mod to be using.
====================
*/
int FS_CheckNastyPath (const char *path, qbool isgamedir)
{
	// all: never allow an empty path, as for gamedir it would access the parent directory and a non-gamedir path it is just useless
	if (!path[0])
		return 2;

	// Windows: don't allow \ in filenames (windows-only), period.
	// (on Windows \ is a directory separator, but / is also supported)
	if (strstr(path, "\\"))
		return 1; // non-portable

	// Mac: don't allow Mac-only filenames - : is a directory separator
	// instead of /, but we rely on / working already, so there's no reason to
	// support a Mac-only path
	// Amiga and Windows: : tries to go to root of drive
	if (strstr(path, ":"))
		return 1; // non-portable attempt to go to root of drive

	// Amiga: // is parent directory
	if (strstr(path, "//"))
		return 1; // non-portable attempt to go to parent directory

	// all: don't allow going to parent directory (../ or /../)
	if (strstr(path, ".."))
		return 2; // attempt to go outside the game directory

	// Windows and UNIXes: don't allow absolute paths
	if (path[0] == '/')
		return 2; // attempt to go outside the game directory

	// all: don't allow . character immediately before a slash, this catches all imaginable cases of ./, ../, .../, etc
	if (strstr(path, "./"))
		return 2; // possible attempt to go outside the game directory

	// all: forbid trailing slash on gamedir
	if (isgamedir && path[strlen(path)-1] == '/')
		return 2;

	// all: forbid leading dot on any filename for any reason
	if (strstr(path, "/."))
		return 2; // attempt to go outside the game directory

	// after all these checks we're pretty sure it's a / separated filename
	// and won't do much if any harm
	return false;
}

/*
====================
FS_SanitizePath

Sanitize path (replace non-portable characters
with portable ones in-place, etc)
====================
*/
void FS_SanitizePath(char *path)
{
	for (; *path; path++)
		if (*path == '\\')
			*path = '/';
}

/*
====================
FS_FindFile

Look for a file in the packages and in the filesystem

Return the searchpath where the file was found (or NULL)
and the file index in the package if relevant
====================
*/
/*SAFE*/ static searchpath_t *FS_FindFile (const char *name, int *index, qbool quiet)
{
	searchpath_t *search;
	pack_t *pak;

	// search through the path, one element at a time
	for (search = fs_searchpaths;search;search = search->next) {
		// is the element a pak file?
		if (search->pack && !search->pack->vpack) {
			int (*strcmp_funct) (const char *str1, const char *str2);
			int left, right, middle;

			pak = search->pack;
			strcmp_funct = pak->ignorecase ? strcasecmp : strcmp;

			// Look for the file (binary search)
			left = 0;
			right = pak->numfiles - 1;
			while (left <= right) {
				int diff;

				middle = (left + right) / 2;
				diff = strcmp_funct (pak->files[middle].name, name);

				// Found it
				if (!diff) {
					if (fs_empty_files_in_pack_mark_deletions.integer && pak->files[middle].realsize == 0) {
						// yes, but the first one is empty so we treat it as not being there
						if (!quiet && developer_extra.integer)
							Con_DPrintLinef ("FS_FindFile: %s is marked as deleted", name);

						if (index != NULL)
							*index = -1;
						return NULL;
					}

					if (!quiet && developer_extra.integer)
						Con_DPrintLinef ("FS_FindFile: %s in %s",
									pak->files[middle].name, pak->filename);

					if (index != NULL)
						*index = middle;
					return search;
				}

				// If we're too far in the list
				if (diff > 0)
					right = middle - 1;
				else
					left = middle + 1;
			}
		} else {
			char netpath[MAX_OSPATH];
			dpsnprintf(netpath, sizeof(netpath), "%s%s", search->filename, name);

			if (FS_SysFileExists (netpath)) {
				if (!quiet && developer_extra.integer)
					Con_DPrintLinef ("FS_FindFile: %s", netpath);

				if (index != NULL)
					*index = -1;
				return search;
			}
		}
	}

	if (!quiet && developer_extra.integer)
		Con_DPrintLinef ("FS_FindFile: can't find %s", name);

	if (index != NULL)
		*index = -1;
	return NULL;
}


/*
===========
FS_OpenReadFile

Look for a file in the search paths and open it in read-only mode
===========
*/
// Baker: This can recursively call itself for symlinks
/*SAFE SEARCHES SEARCHPATHS*/ static qfile_t *FS_OpenReadFile (const char *filename, qbool quiet, qbool nonblocking, int symlinkLevels)
{
	searchpath_t *search;
	int pack_ind;

	search = FS_FindFile (filename, &pack_ind, quiet);

	// Not found?
	if (search == NULL)
		return NULL;

	// Found in the filesystem?
	if (pack_ind < 0) {
		// this works with vpacks, so we are fine
		char path [MAX_OSPATH];
		dpsnprintf (path, sizeof (path), "%s%s", search->filename, filename);
		return FS_SysOpen (path, "rb", nonblocking);
	}

	// So, we found it in a package...

	// Is it a PK3 symlink?
	// TODO also handle directory symlinks by parsing the whole structure...
	// but heck, file symlinks are good enough for now
	if (search->pack->files[pack_ind].flags & PACKFILE_FLAG_SYMLINK) {
		if (symlinkLevels <= 0) {
			Con_PrintLinef ("symlink: %s: too many levels of symbolic links", filename);
			return NULL;
		} else {
			char linkbuf[MAX_QPATH_128];
			fs_offset_t count;
			qfile_t *linkfile = FS_OpenPackedFile (search->pack, pack_ind);
			const char *mergeslash;
			char *mergestart;

			if (!linkfile)
				return NULL;

			count = FS_Read(linkfile, linkbuf, sizeof(linkbuf) - 1);
			FS_Close(linkfile);
			if (count < 0)
				return NULL;
			linkbuf[count] = 0;

			// Now combine the paths...
			mergeslash = strrchr(filename, '/');
			mergestart = linkbuf;

			if (!mergeslash)
				mergeslash = filename;

			while (!strncmp(mergestart, "../", 3)) {
				mergestart += 3;
				while(mergeslash > filename) {
					--mergeslash;
					if (*mergeslash == '/')
						break;
				}
			}

			// Now, mergestart will point to the path to be appended, and mergeslash points to where it should be appended
			if (mergeslash == filename) {
				// Either mergeslash == filename, then we just replace the name (done below)
			} else {
				// Or, we append the name after mergeslash;
				// or rather, we can also shift the linkbuf so we can put everything up to and including mergeslash first
				int spaceNeeded = mergeslash - filename + 1;
				int spaceRemoved = mergestart - linkbuf;
				if (count - spaceRemoved + spaceNeeded >= MAX_QPATH_128) {
					Con_DPrintLinef ("symlink: too long path rejected");
					return NULL;
				}
				memmove(linkbuf + spaceNeeded, linkbuf + spaceRemoved, count - spaceRemoved);
				memcpy(linkbuf, filename, spaceNeeded);
				linkbuf[count - spaceRemoved + spaceNeeded] = 0;
				mergestart = linkbuf;
			}

			if (!quiet && developer_loading.integer)
				Con_DPrintLinef ("symlink: %s -> %s", filename, mergestart);

			if (FS_CheckNastyPath (mergestart, false)) {
				Con_DPrintLinef ("symlink: nasty path %s rejected", mergestart);
				return NULL;
			}
			return FS_OpenReadFile(mergestart, quiet, nonblocking, symlinkLevels - 1);
		}
	}

	return FS_OpenPackedFile (search->pack, pack_ind);
}


/*
=============================================================================

MAIN PUBLIC FUNCTIONS

=============================================================================
*/

/*
====================
FS_OpenRealFile

Open a file in the userpath. The syntax is the same as fopen
Used for savegame scanning in menu, and all file writing.
====================
*/
// Baker: This is path creating file write

// Baker: This isn't suitable for read.  It only checks fs_gamedir path which if homed could be like "myname"
// So if fs_gamedir is "myname"
// What is "quake"?
/*GAMEDIR PREPENDED*/ qfile_t *FS_OpenRealFile (ccs *filename, ccs *mode /*rb or what not*/, qbool quiet)
{
	char real_path [MAX_OSPATH];

	if (FS_CheckNastyPath(filename, fs_isgamedir_false)) {
		Con_PrintLinef ("FS_OpenRealFile(" QUOTED_S ", " QUOTED_S ", %s): nasty filename rejected", filename, mode, quiet ? "true" : "false");
		return NULL;
	}

	//Con_PrintLinef ("FS_OpenRealFile fs_gamedir " QUOTED_S " " QUOTED_S, fs_gamedir, filepath);

	dpsnprintf (real_path, sizeof (real_path), "%s/%s", fs_gamedir, filename); // this is never a vpack

	// If the file is opened in "write", "append", or "read/write" mode,
	// create directories up to the file.
	if (mode[0] == 'w' || mode[0] == 'a' || strchr(mode, '+'))
		FS_CreatePath (real_path); // Baker: Only file writing attempts to create the path
	return FS_SysOpen (real_path, mode, fs_nonblocking_false);
}

// We are checking gamedir/filepath
// Baker: fs_gamedir is something like /home/myname/.zircon/
// fs_basedir <--- I think this is what we want.

/*GAMEDIR OR BASEDIR/GAMEDIRNAME1 PREPENDED*/ qfile_t *FS_OpenRealFileReadBinary (const char *filepath, char **prealpathname_zalloc) // Order .. 0: gamedir, 1: basedir
{
	if (FS_CheckNastyPath(filepath, /*isgamedir?*/ false)) {
		*prealpathname_zalloc = NULL;
		return NULL;
	}

	// 0, 1
	for (int j = 0; j < 2; j ++) {

		char real_path [MAX_OSPATH];

		//Con_PrintLinef ("FS_OpenRealFile fs_gamedir " QUOTED_S " " QUOTED_S, fs_gamedir, filepath);
		switch (j) {
		case 0:
			// Baker: homedir method
			RELATED_ (FS_AddGameDirectory)
			c_dpsnprintf2 (real_path, "%s/%s", fs_gamedir, filepath); // this is never a vpack
			break;
		case 1:
			c_dpsnprintf3 (real_path, "%s%s/%s", fs_basedir, gamedirname1, filepath); // this is never a vpack
			break;
		} // sw


		// If the file is opened in "write", "append", or "read/write" mode,
		// create directories up to the file.
		qfile_t *f = FS_SysOpen (real_path, FS_MODE_READ_BINARY_RB, fs_nonblocking_false);
		if (f) {
			*prealpathname_zalloc = Z_StrDup (real_path);
			return f;
		}

		// Only process once if -nohome (which is windows default for Zircon)
		if (fs_userdir[0] == 0)
			break;

	} // for

	*prealpathname_zalloc = NULL;
	return NULL; // Fail
}


/*
====================
FS_OpenVirtualFile

Open a file. The syntax is the same as fopen
====================
*/
/*SAFE*/ qfile_t *FS_OpenVirtualFile (const char *filepath, qbool quiet)
{
	qfile_t *result = NULL;
	if (FS_CheckNastyPath(filepath, false)) {
		Con_PrintLinef ("FS_OpenVirtualFile(" QUOTED_S ", %s): nasty filename rejected", filepath, quiet ? "true" : "false");
		return NULL;
	}

	if (fs_mutex) Thread_LockMutex(fs_mutex);
	result = FS_OpenReadFile (filepath, quiet, fs_nonblocking_false, /*symlinklevels*/ 16);
	if (fs_mutex) Thread_UnlockMutex(fs_mutex);
	return result;
}

/*
====================
FS_FileFromData

Open a file. The syntax is the same as fopen
====================
*/
qfile_t *FS_FileFromData (const unsigned char *data, const size_t size, qbool quiet)
{
	qfile_t *file;
	file = (qfile_t *)Mem_Alloc (fs_mempool, sizeof (*file));
	memset (file, 0, sizeof (*file));
	file->flags = QFILE_FLAG_DATA;
	file->ungetc = EOF;
	file->real_length = size;
	file->data = data;
	return file;
}

/*
====================
FS_Close

Close a file
====================
*/
/*FHANDLE*/ int FS_Close (qfile_t *file)
{
	if (file->flags & QFILE_FLAG_DATA)
	{
		Mem_Free(file);
		return 0;
	}

	if (FILEDESC_CLOSE (file->handle))
		return EOF;

	if (file->f_filename)
	{
		if (file->flags & QFILE_FLAG_REMOVE)
		{
			if (remove(file->f_filename) == -1)
			{
				// No need to report this. If removing a just
				// written file failed, this most likely means
				// someone else deleted it first - which we
				// like.
			}
		}

		Mem_Free((void *) file->f_filename);
	}

	if (file->ztk)
	{
		qz_inflateEnd (&file->ztk->zstream);
		Mem_Free (file->ztk);
	}

	Mem_Free (file);
	return 0;
}

void FS_RemoveOnClose(qfile_t *file)
{
	file->flags |= QFILE_FLAG_REMOVE;
}

/*
====================
FS_Write

Write "datasize" bytes into a file
====================
*/
/*FHANDLE*/ fs_offset_t FS_Write (qfile_t *file, const void *data, size_t datasize)
{
	fs_offset_t written = 0;

	// If necessary, seek to the exact file position we're supposed to be
	if (file->buff_ind != file->buff_len)
	{
		if (FILEDESC_SEEK (file->handle, file->buff_ind - file->buff_len, SEEK_CUR) == -1)
		{
			Con_PrintLinef (CON_WARN "WARNING: could not seek in %s.", file->f_filename);
		}
	}

	// Purge cached data
	FS_Purge (file);

	// Write the buffer and update the position
	// LadyHavoc: to hush a warning about passing size_t to an unsigned int parameter on Win64 we do this as multiple writes if the size would be too big for an integer (we never write that big in one go, but it's a theory)
	while (written < (fs_offset_t)datasize)
	{
		// figure out how much to write in one chunk
		fs_offset_t maxchunk = 1<<30; // 1 GiB
		int chunk = (int)min((fs_offset_t)datasize - written, maxchunk);
		int result = (int)FILEDESC_WRITE (file->handle, (const unsigned char *)data + written, chunk);
		// if at least some was written, add it to our accumulator
		if (result > 0)
			written += result;
		// if the result is not what we expected, consider the write to be incomplete
		if (result != chunk)
			break;
	}
	file->position = FILEDESC_SEEK (file->handle, 0, SEEK_CUR);
	if (file->real_length < file->position)
		file->real_length = file->position;

	// note that this will never be less than 0 even if the write failed
	return written;
}


/*
====================
FS_Read

Read up to "buffersize" bytes from a file
====================
*/
/*FHANDLE*/ fs_offset_t FS_Read (qfile_t *file, void *buffer, size_t buffersize)
{
	fs_offset_t count, done;

	if (buffersize == 0 || !buffer)
		return 0;

	// Get rid of the ungetc character
	if (file->ungetc != EOF)
	{
		((char *)buffer)[0] = file->ungetc;
		buffersize--;
		file->ungetc = EOF;
		done = 1;
	}
	else
		done = 0;

	if (file->flags & QFILE_FLAG_DATA)
	{
		size_t left = file->real_length - file->position;
		if (buffersize > left)
			buffersize = left;
		memcpy(buffer, file->data + file->position, buffersize);
		file->position += buffersize;
		return buffersize;
	}

	// First, we copy as many bytes as we can from "buff"
	if (file->buff_ind < file->buff_len)
	{
		count = file->buff_len - file->buff_ind;
		count = ((fs_offset_t)buffersize > count) ? count : (fs_offset_t)buffersize;
		done += count;
		memcpy (buffer, &file->buff[file->buff_ind], count);
		file->buff_ind += count;

		buffersize -= count;
		if (buffersize == 0)
			return done;
	}

	// NOTE: at this point, the read buffer is always empty

	// If the file isn't compressed
	if (! (file->flags & QFILE_FLAG_DEFLATED))
	{
		fs_offset_t nb;

		// We must take care to not read after the end of the file
		count = file->real_length - file->position;

		// If we have a lot of data to get, put them directly into "buffer"
		if (buffersize > sizeof (file->buff) / 2)
		{
			if (count > (fs_offset_t)buffersize)
				count = (fs_offset_t)buffersize;
			if (FILEDESC_SEEK (file->handle, file->offset + file->position, SEEK_SET) == -1)
			{
				// Seek failed. When reading from a pipe, and
				// the caller never called FS_Seek, this still
				// works fine.  So no reporting this error.
			}
			nb = FILEDESC_READ (file->handle, &((unsigned char *)buffer)[done], count);
			if (nb > 0)
			{
				done += nb;
				file->position += nb;

				// Purge cached data
				FS_Purge (file);
			}
		}
		else
		{
			if (count > (fs_offset_t)sizeof (file->buff))
				count = (fs_offset_t)sizeof (file->buff);
			if (FILEDESC_SEEK (file->handle, file->offset + file->position, SEEK_SET) == -1)
			{
				// Seek failed. When reading from a pipe, and
				// the caller never called FS_Seek, this still
				// works fine.  So no reporting this error.
			}
			nb = FILEDESC_READ (file->handle, file->buff, count);
			if (nb > 0)
			{
				file->buff_len = nb;
				file->position += nb;

				// Copy the requested data in "buffer" (as much as we can)
				count = (fs_offset_t)buffersize > file->buff_len ? file->buff_len : (fs_offset_t)buffersize;
				memcpy (&((unsigned char *)buffer)[done], file->buff, count);
				file->buff_ind = count;
				done += count;
			}
		}

		return done;
	}

	// If the file is compressed, it's more complicated...
	// We cycle through a few operations until we have read enough data
	while (buffersize > 0)
	{
		ztoolkit_t *ztk = file->ztk;
		int error;

		// NOTE: at this point, the read buffer is always empty

		// If "input" is also empty, we need to refill it
		if (ztk->in_ind == ztk->in_len)
		{
			// If we are at the end of the file
			if (file->position == file->real_length)
				return done;

			count = (fs_offset_t)(ztk->comp_length - ztk->in_position);
			if (count > (fs_offset_t)sizeof (ztk->input))
				count = (fs_offset_t)sizeof (ztk->input);
			FILEDESC_SEEK (file->handle, file->offset + (fs_offset_t)ztk->in_position, SEEK_SET);
			if (FILEDESC_READ (file->handle, ztk->input, count) != count)
			{
				Con_Printf ("FS_Read: unexpected end of file\n");
				break;
			}

			ztk->in_ind = 0;
			ztk->in_len = count;
			ztk->in_position += count;
		}

		ztk->zstream.next_in = &ztk->input[ztk->in_ind];
		ztk->zstream.avail_in = (unsigned int)(ztk->in_len - ztk->in_ind);

		// Now that we are sure we have compressed data available, we need to determine
		// if it's better to inflate it in "file->buff" or directly in "buffer"

		// Inflate the data in "file->buff"
		if (buffersize < sizeof (file->buff) / 2)
		{
			ztk->zstream.next_out = file->buff;
			ztk->zstream.avail_out = sizeof (file->buff);
			error = qz_inflate (&ztk->zstream, Z_SYNC_FLUSH);
			if (error != Z_OK && error != Z_STREAM_END)
			{
				Con_Printf ("FS_Read: Can't inflate file\n");
				break;
			}
			ztk->in_ind = ztk->in_len - ztk->zstream.avail_in;

			file->buff_len = (fs_offset_t)sizeof (file->buff) - ztk->zstream.avail_out;
			file->position += file->buff_len;

			// Copy the requested data in "buffer" (as much as we can)
			count = (fs_offset_t)buffersize > file->buff_len ? file->buff_len : (fs_offset_t)buffersize;
			memcpy (&((unsigned char *)buffer)[done], file->buff, count);
			file->buff_ind = count;
		}

		// Else, we inflate directly in "buffer"
		else
		{
			ztk->zstream.next_out = &((unsigned char *)buffer)[done];
			ztk->zstream.avail_out = (unsigned int)buffersize;
			error = qz_inflate (&ztk->zstream, Z_SYNC_FLUSH);
			if (error != Z_OK && error != Z_STREAM_END)
			{
				Con_Printf ("FS_Read: Can't inflate file\n");
				break;
			}
			ztk->in_ind = ztk->in_len - ztk->zstream.avail_in;

			// How much data did it inflate?
			count = (fs_offset_t)(buffersize - ztk->zstream.avail_out);
			file->position += count;

			// Purge cached data
			FS_Purge (file);
		}

		done += count;
		buffersize -= count;
	}

	return done;
}


/*
====================
FS_Print

Print a string into a file
====================
*/
/*FHANDLE*/ int FS_Print (qfile_t *file, const char *msg)
{
	return (int)FS_Write (file, msg, strlen (msg));
}

/*
====================
FS_Printf

Print a string into a file
====================
*/
/*FHANDLE*/ int FS_Printf(qfile_t *file, const char *format, ...)
{
	int result;
	va_list args;

	va_start (args, format);
	result = FS_VPrintf (file, format, args);
	va_end (args);

	return result;
}

/*FHANDLE*/ int FS_PrintLinef(qfile_t *file, const char *fmt, ...)
{
	int result;
	VA_EXPAND_ALLOC (text, text_slen, bufsiz, fmt);
	result = FS_Printf (file, "%s" NEWLINE, text);
	VA_EXPAND_ALLOC_FREE (text);
	return result;
}

/*
====================
FS_VPrintf

Print a string into a file
====================
*/
/*FHANDLE*/ int FS_VPrintf (qfile_t *file, const char *format, va_list ap)
{
	int len;
	fs_offset_t buff_size = MAX_INPUTLINE_16384;
	char *tempbuff;

	for (;;)
	{
		tempbuff = (char *)Mem_Alloc (tempmempool, buff_size);
		len = dpvsnprintf (tempbuff, buff_size, format, ap);
		if (len >= 0 && len < buff_size)
			break;
		Mem_Free (tempbuff);
		buff_size *= 2;
		//Sys_PrintToTerminal (va32 ("Expanding buffer to %f" NEWLINE, (double)buff_size ));
	}

	len = FILEDESC_WRITE (file->handle, tempbuff, len);
	Mem_Free (tempbuff);

	return len;
}


/*
====================
FS_Getc

Get the next character of a file
====================
*/
/*FHANDLE*/ int FS_Getc (qfile_t *file)
{
	unsigned char c;

	if (FS_Read (file, &c, 1) != 1)
		return EOF;

	return c;
}


/*
====================
FS_UnGetc

Put a character back into the read buffer (only supports one character!)
====================
*/
/*FHANDLE*/ int FS_UnGetc (qfile_t *file, unsigned char c)
{
	// If there's already a character waiting to be read
	if (file->ungetc != EOF)
		return EOF;

	file->ungetc = c;
	return c;
}


/*
====================
FS_Seek

Move the position index in a file
====================
*/
/*FHANDLE*/ int FS_Seek (qfile_t *file, fs_offset_t offset, int whence)
{
	ztoolkit_t *ztk;
	unsigned char *buffer;
	fs_offset_t buffersize;

	// Compute the file offset
	switch (whence)
	{
		case SEEK_CUR:
			offset += file->position - file->buff_len + file->buff_ind;
			break;

		case SEEK_SET:
			break;

		case SEEK_END:
			offset += file->real_length;
			break;

		default:
			return -1;
	}
	if (offset < 0 || offset > file->real_length)
		return -1;

	if (file->flags & QFILE_FLAG_DATA)
	{
		file->position = offset;
		return 0;
	}

	// If we have the data in our read buffer, we don't need to actually seek
	if (file->position - file->buff_len <= offset && offset <= file->position)
	{
		file->buff_ind = offset + file->buff_len - file->position;
		return 0;
	}

	// Purge cached data
	FS_Purge (file);

	// Unpacked or uncompressed files can seek directly
	if (! (file->flags & QFILE_FLAG_DEFLATED))
	{
		if (FILEDESC_SEEK (file->handle, file->offset + offset, SEEK_SET) == -1)
			return -1;
		file->position = offset;
		return 0;
	}

	// Seeking in compressed files is more a hack than anything else,
	// but we need to support it, so here we go.
	ztk = file->ztk;

	// If we have to go back in the file, we need to restart from the beginning
	if (offset <= file->position)
	{
		ztk->in_ind = 0;
		ztk->in_len = 0;
		ztk->in_position = 0;
		file->position = 0;
		if (FILEDESC_SEEK (file->handle, file->offset, SEEK_SET) == -1)
			Con_Printf ("IMPOSSIBLE: couldn't seek in already opened pk3 file.\n");

		// Reset the Zlib stream
		ztk->zstream.next_in = ztk->input;
		ztk->zstream.avail_in = 0;
		qz_inflateReset (&ztk->zstream);
	}

	// We need a big buffer to force inflating into it directly
	buffersize = 2 * sizeof (file->buff);
	buffer = (unsigned char *)Mem_Alloc (tempmempool, buffersize);

	// Skip all data until we reach the requested offset
	while (offset > (file->position - file->buff_len + file->buff_ind))
	{
		fs_offset_t diff = offset - (file->position - file->buff_len + file->buff_ind);
		fs_offset_t count, len;

		count = (diff > buffersize) ? buffersize : diff;
		len = FS_Read (file, buffer, count);
		if (len != count)
		{
			Mem_Free (buffer);
			return -1;
		}
	}

	Mem_Free (buffer);
	return 0;
}


/*
====================
FS_Tell

Give the current position in a file
====================
*/
/*FHANDLE*/ fs_offset_t FS_Tell (qfile_t *file)
{
	return file->position - file->buff_len + file->buff_ind;
}


/*
====================
FS_FileSize

Give the total size of a file
====================
*/
/*FHANDLE*/ fs_offset_t FS_FileSize (qfile_t *file) // See better FS_FileSizeEx_Safe
{
	return file->real_length;
}


/*
====================
FS_Purge

Erases any buffered input or output data
====================
*/
void FS_Purge (qfile_t *file)
{
	file->buff_len = 0;
	file->buff_ind = 0;
	file->ungetc = EOF;
}


/*
============
FS_LoadAndCloseQFile

Loads full content of a qfile_t and closes it.
Always appends a 0 byte.
============
*/
// Baker: darkplaces.opt and mod_info.txt
// May 28 2025 - This function is typically called with a null qfile_t handle and returns NULL.
static unsigned char *FS_LoadAndCloseQFile (qfile_t *file, const char *realpath, mempool_t *pool, qbool quiet, fs_offset_t *filesizepointer)
{
	unsigned char *buf = NULL;
	fs_offset_t filesize = 0;

	if (file)
	{
		filesize = file->real_length;
		if (filesize < 0)
		{
			Con_PrintLinef ("FS_LoadFile(" QUOTED_S ", pool, %s, filesizepointer): trying to open a non-regular file", realpath, quiet ? "true" : "false");
			FS_Close(file);
			return NULL;
		}

		buf = (unsigned char *)Mem_Alloc (pool, filesize + 1);
		buf[filesize] = '\0';
		FS_Read (file, buf, filesize);
		FS_Close (file);
		if (developer_loadingfile_fs.integer)
			Con_PrintLinef ("loaded file " QUOTED_S " (%u bytes)", realpath, (unsigned int)filesize);
	}

	if (filesizepointer)
		*filesizepointer = filesize;
	return buf;
}


/*
============
FS_LoadFile

Filename are relative to the quake directory.
Always appends a 0 byte.
============
*/
/*SAFE*/ unsigned char *FS_LoadFile (const char *path, mempool_t *pool, qbool quiet, fs_offset_t *filesizepointer)
{
	qfile_t *file = FS_OpenVirtualFile(path, quiet);
	return FS_LoadAndCloseQFile(file, path, pool, quiet, filesizepointer);
}



/*
============
FS_SysLoadFile

Filename are OS paths.
Always appends a 0 byte.
============
*/

// Baker: Everything that uses this is wonky like darkplaces.opt and mod_info.txt
// This function uses the current directory, doesn't honor -basedir or -userdir and such
/*REALPATH*/ unsigned char *FS_SysLoadFile (ccs *realpath, mempool_t *pool, qbool quiet, fs_offset_t *filesizepointer)
{
	qfile_t *file = FS_SysOpen(realpath, "rb", fs_nonblocking_false);
	return FS_LoadAndCloseQFile(file, realpath, pool, quiet, filesizepointer);
}


/*
============
FS_WriteFile

The filename will be prefixed by the current game directory
============
*/
qbool FS_WriteFileInBlocks (const char *filename, const void *const *data, const fs_offset_t *len, size_t count)
{
	qfile_t *file;
	size_t i;
	fs_offset_t lentotal;

	file = FS_OpenRealFile(filename, "wb", fs_quiet_FALSE);  // WRITE-EON .. IGNORE THIS, SUB PROCESS .. move along
	if (!file)
	{
		Con_PrintLinef ("FS_WriteFile: failed on %s", filename);
		return false;
	}

	lentotal = 0;
	for(i = 0; i < count; ++i)
		lentotal += len[i];
	Con_DPrintLinef ("FS_WriteFile: %s (%u bytes)", filename, (unsigned int)lentotal);
	for(i = 0; i < count; ++i)
		FS_Write (file, data[i], len[i]);
	FS_Close (file);
	return true;
}

qbool FS_WriteFile (const char *filename, const void *data, fs_offset_t len)
{
	return FS_WriteFileInBlocks(filename, &data, &len, 1);
}


/*
=============================================================================

OTHERS PUBLIC FUNCTIONS

=============================================================================
*/

/*
============
FS_StripExtension
============
*/
// Baker: Should be unicode safe because all unicode chars are 128+ and do not collide with < 127
void FS_StripExtension (const char *in, char *out, size_t size_out)
{
	char *last = NULL;
	char currentchar;

	if (size_out == 0)
		return;

	while ((currentchar = *in) && size_out > 1)
	{
		if (currentchar == '.')
			last = out;
		else if (currentchar == '/' || currentchar == '\\' || currentchar == ':')
			last = NULL;
		*out++ = currentchar;
		in++;
		size_out--;
	}
	if (last)
		*last = 0;
	else
		*out = 0;
}


/*
==================
FS_DefaultExtension
==================
*/
void FS_DefaultExtension (char *path, const char *extension, size_t size_path)
{
	const char *src;

	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
	src = path + strlen(path);

	while (*src != '/' && src != path) {
		if (*src == '.')
			return;                 // it has an extension
		src--;
	}

	strlcat (path, extension, size_path);
}


/*
==================
FS_FileOrDirectoryType

Look for a file in the packages and in the filesystem
==================
*/
/*SAFE*/ FS_FILETYPE_e FS_FileOrDirectoryType (const char *filename)
{
	searchpath_t *search;
	char fullpath[MAX_OSPATH];

	search = FS_FindFile (filename, fs_package_index_reply_null, fs_quiet_true);
	if (!search)
		return FS_FILETYPE_NONE_0;

	if (search->pack && !search->pack->vpack)
		return FS_FILETYPE_FILE_1; // TODO can't check directories in paks yet, maybe later

	c_dpsnprintf2 (fullpath, "%s%s", search->filename, filename);
	return FS_SysFileOrDirectoryType(fullpath);
}


/*
==================
FS_FileExists

Look for a file in the packages and in the filesystem
==================
*/
/*SAFE*/ qbool FS_FileExists (const char *filename)
{
	return (FS_FindFile (filename, NULL, true) != NULL);
}


/*
==================
FS_SysFileExists

Look for a file in the filesystem only
==================
*/
FS_FILETYPE_e FS_SysFileOrDirectoryType (const char *realpath)
{
#ifdef _WIN32
// Sajt - some older sdks are missing this define
# ifndef INVALID_FILE_ATTRIBUTES
#  define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
# endif
	WPATHDEF(pathw);
	DWORD result;

	WIDE(realpath, pathw);
	result = GetFileAttributesW(pathw);

	if (result == INVALID_FILE_ATTRIBUTES)
		return FS_FILETYPE_NONE_0;

	if (result & FILE_ATTRIBUTE_DIRECTORY)
		return FS_FILETYPE_DIRECTORY_2;

	return FS_FILETYPE_FILE_1;
#else
	struct stat buf;

	if (stat (realpath,&buf) == -1)
		return FS_FILETYPE_NONE_0;

#ifndef S_ISDIR
#define S_ISDIR(a) (((a) & S_IFMT) == S_IFDIR)
#endif
	if (S_ISDIR(buf.st_mode))
		return FS_FILETYPE_DIRECTORY_2;

	return FS_FILETYPE_FILE_1;
#endif
}

/*REALPATH*/ qbool FS_SysFileExists (const char *path)
{
	return FS_SysFileOrDirectoryType (path) != FS_FILETYPE_NONE_0;
}

/*
===========
FS_Search

Allocate and fill a search structure with information on matching filenames.
===========
*/
fssearch_t *FS_Search (const char *pattern, int caseinsensitive, int quiet, const char *packfile, int isgamedironly_in)
{
	fssearch_t *search;
	searchpath_t *searchpath;
	pack_t *pak;
	int i, basepathlength, numfiles, numchars, resultlistindex, dirlistindex;
	stringlist_t resultlist;
	stringlist_t dirlist;
	stringlist_t matchedSet, foundSet;
	const char *start, *slash, *backslash, *colon, *separator;
	char *basepath;
#if 1 // Maps gamedir only
	char sgamedironly[MAX_QPATH_128 * 2] = {0};
	int hit_gamedironly_limiter = false;
#endif
	// pattern[i] == '.' || pattern[i] == ':' || pattern[i] == '/' || pattern[i] == '\\'

	// Baker: Checking leading punctuation
	for (i = 0; isin4 (pattern[i], '.', ':', '/', '\\'); i ++)
		;

	if (i > 0) {
		Con_PrintLinef ("Don't use punctuation at the beginning of a search pattern!");
		return NULL;
	}

	stringlistinit(&resultlist);
	stringlistinit(&dirlist);
	search = NULL;
	slash = strrchr(pattern, '/');
	backslash = strrchr(pattern, '\\');
	colon = strrchr(pattern, ':');
	separator = max(slash, backslash);
	separator = max(separator, colon);
	basepathlength = separator ? (separator + 1 - pattern) : 0;
	basepath = (char *)Mem_Alloc (tempmempool, basepathlength + 1);
	if (basepathlength)
		memcpy(basepath, pattern, basepathlength);
	basepath[basepathlength] = 0;

	// search through the path, one element at a time
	for (searchpath = fs_searchpaths;searchpath;searchpath = searchpath->next) {
#if 1 // Maps gamedir only
		if (isgamedironly_in && sgamedironly[0] == 0 && !searchpath->pack) {
			// ignore the home directory for map menu
			if ( !fs_userdir[0] /*not homed*/ || String_Starts_With (searchpath->filename, fs_userdir) == false /*homed, but not a home dir*/ ) {
				c_strlcpy (sgamedironly, searchpath->filename);
			}
		}

		if (searchpath->pack) {
			if (isgamedironly_in && hit_gamedironly_limiter && String_Starts_With_Caseless (searchpath->pack->filename, sgamedironly) == false) {
				if (developer_loading.value) {
					Con_PrintLinef ("Surpassed isgamedironly limit at %s limit is %s", searchpath->pack->filename, sgamedironly);
				}
				goto failout;
			}

		} else {
			if (hit_gamedironly_limiter && String_Starts_With_Caseless (searchpath->filename, sgamedironly) == false) {
				if (developer_loading.value) {
					Con_PrintLinef ("Surpassed isgamedironly limit at %s limit is %s", searchpath->filename, sgamedironly);
				}
				goto failout;
			}

			if (hit_gamedironly_limiter == false && isgamedironly_in && String_Match_Caseless (searchpath->filename, sgamedironly)) {
				if (developer_loading.value) {
					Con_PrintLinef ("Found limit at %s limit is %s", searchpath->filename, sgamedironly);
				}
				hit_gamedironly_limiter = true;
			}
		}
#endif
		// is the element a pak file?
		if (searchpath->pack && !searchpath->pack->vpack) {
			// look through all the pak file elements
			pak = searchpath->pack;
			if (packfile) {
				if (String_NOT_Match(packfile, pak->shortname))
					continue;
			}
			for (i = 0;i < pak->numfiles;i++)
			{
				char temp[MAX_OSPATH];
				strlcpy(temp, pak->files[i].name, sizeof(temp));
				while (temp[0])
				{
					if (matchpattern(temp, (char *)pattern, true))
					{
						for (resultlistindex = 0;resultlistindex < resultlist.numstrings;resultlistindex++)
							if (String_Match(resultlist.strings[resultlistindex], temp))
								break;
						if (resultlistindex == resultlist.numstrings)
						{
							stringlistappend(&resultlist, temp);
							if (quiet == false && developer_loading.integer)
								Con_PrintLinef ("SearchPackFile: %s : %s", pak->filename, temp);
						}
					}
					// strip off one path element at a time until empty
					// this way directories are added to the listing if they match the pattern
					slash = strrchr(temp, '/');
					backslash = strrchr(temp, '\\');
					colon = strrchr(temp, ':');
					separator = temp;
					if (separator < slash)
						separator = slash;
					if (separator < backslash)
						separator = backslash;
					if (separator < colon)
						separator = colon;
					*((char *)separator) = 0;
				}
			}
		}
		else
		{
			if (packfile)
				continue;

			start = pattern;

			stringlistinit(&matchedSet);
			stringlistinit(&foundSet);
			// add a first entry to the set
			stringlistappend(&matchedSet, "");
			// iterate through pattern's path
			while (*start)
			{
				const char *asterisk, *wildcard, *nextseparator, *prevseparator;
				char subpath[MAX_OSPATH];
				char subpattern[MAX_OSPATH];

				// find the next wildcard
				wildcard = strchr(start, '?');
				asterisk = strchr(start, '*');
				if (asterisk && (!wildcard || asterisk < wildcard))
				{
					wildcard = asterisk;
				}

				if (wildcard)
				{
					nextseparator = strchr( wildcard, '/' );
				}
				else
				{
					nextseparator = NULL;
				}

				if ( !nextseparator ) {
					nextseparator = start + strlen( start );
				}

				// prevseparator points past the '/' right before the wildcard and nextseparator at the one following it (or at the end of the string)
				// copy everything up except nextseperator
				strlcpy(subpattern, pattern, min(sizeof(subpattern), (size_t) (nextseparator - pattern + 1)));
				// find the last '/' before the wildcard
				prevseparator = strrchr( subpattern, '/' );
				if (!prevseparator)
					prevseparator = subpattern;
				else
					prevseparator++;
				// copy everything from start to the previous including the '/' (before the wildcard)
				// everything up to start is already included in the path of matchedSet's entries
				strlcpy(subpath, start, min(sizeof(subpath), (size_t) ((prevseparator - subpattern) - (start - pattern) + 1)));

				// for each entry in matchedSet try to open the subdirectories specified in subpath
				for( dirlistindex = 0 ; dirlistindex < matchedSet.numstrings ; dirlistindex++ ) {
					char temp[MAX_OSPATH];
					c_strlcpy		(temp, matchedSet.strings[ dirlistindex ]);
					c_strlcat		(temp, subpath);
					stringlist_append_listdirectory	(&foundSet, searchpath->filename, temp);
				}
				if ( dirlistindex == 0 ) {
					break;
				}
				// reset the current result set
				stringlistfreecontents( &matchedSet );
				// match against the pattern
				for( dirlistindex = 0 ; dirlistindex < foundSet.numstrings ; dirlistindex++ ) {
					const char *direntry = foundSet.strings[ dirlistindex ];
					if (matchpattern(direntry, subpattern, true)) {
						stringlistappend( &matchedSet, direntry );
					}
				}
				stringlistfreecontents( &foundSet );

				start = nextseparator;
			}

			for (dirlistindex = 0;dirlistindex < matchedSet.numstrings;dirlistindex++)
			{
				const char *matchtemp = matchedSet.strings[dirlistindex];
				if (matchpattern(matchtemp, (char *)pattern, true))
				{
					for (resultlistindex = 0;resultlistindex < resultlist.numstrings;resultlistindex++)
						if (String_Match(resultlist.strings[resultlistindex], matchtemp))
							break;
					if (resultlistindex == resultlist.numstrings)
					{
						stringlistappend(&resultlist, matchtemp);
						if (!quiet && developer_loading.integer)
							Con_PrintLinef ("SearchDirFile: %s", matchtemp);
					}
				}
			}
			stringlistfreecontents( &matchedSet );
		}
	}
failout:

	if (resultlist.numstrings) {
		// SORT
		stringlistsort(&resultlist, true);
		numfiles = resultlist.numstrings;
		numchars = 0;
		for (resultlistindex = 0;resultlistindex < resultlist.numstrings;resultlistindex++)
			numchars += (int)strlen(resultlist.strings[resultlistindex]) + 1;
		search = (fssearch_t *)Z_Malloc(sizeof(fssearch_t) + numchars + numfiles * sizeof(char *));
		search->filenames = (char **)((char *)search + sizeof(fssearch_t));
		search->filenamesbuffer = (char *)((char *)search + sizeof(fssearch_t) + numfiles * sizeof(char *));
		search->numfilenames = (int)numfiles;
		numfiles = 0;
		numchars = 0;
		for (resultlistindex = 0;resultlistindex < resultlist.numstrings;resultlistindex++)
		{
			size_t textlen;
			search->filenames[numfiles] = search->filenamesbuffer + numchars;
			textlen = strlen(resultlist.strings[resultlistindex]) + 1;
			memcpy(search->filenames[numfiles], resultlist.strings[resultlistindex], textlen);
			numfiles++;
			numchars += (int)textlen;
		}
	}
	stringlistfreecontents(&resultlist);

	Mem_Free(basepath);
	return search;
}

void FS_FreeSearch(fssearch_t *search)
{
	Z_Free(search);
}

extern int con_linewidth;
static int FS_ListDirectory(const char *pattern, int oneperline)
{
	int numfiles;
	int numcolumns;
	int numlines;
	int columnwidth;
	int linebufpos;
	int i, j, k, l;
	const char *name;
	char linebuf[MAX_INPUTLINE_16384];
	fssearch_t *search;
	search = FS_Search(pattern, fs_caseless_true, fs_quiet_true, fs_pakfile_null, fs_gamedironly_false);
	if (!search)
		return 0;
	numfiles = search->numfilenames;
	if (!oneperline)
	{
		// FIXME: the names could be added to one column list and then
		// gradually shifted into the next column if they fit, and then the
		// next to make a compact variable width listing but it's a lot more
		// complicated...
		// find width for columns
		columnwidth = 0;
		for (i = 0;i < numfiles;i++)
		{
			l = (int)strlen(search->filenames[i]);
			if (columnwidth < l)
				columnwidth = l;
		}
		// count the spacing character
		columnwidth++;
		// calculate number of columns
		numcolumns = con_linewidth / columnwidth;
		// don't bother with the column printing if it's only one column
		if (numcolumns >= 2)
		{
			numlines = (numfiles + numcolumns - 1) / numcolumns;
			for (i = 0;i < numlines;i++)
			{
				linebufpos = 0;
				for (k = 0;k < numcolumns;k++)
				{
					l = i * numcolumns + k;
					if (l < numfiles)
					{
						name = search->filenames[l];
						for (j = 0;name[j] && linebufpos + 1 < (int)sizeof(linebuf);j++)
							linebuf[linebufpos++] = name[j];
						// space out name unless it's the last on the line
						if (k + 1 < numcolumns && l + 1 < numfiles)
							for (;j < columnwidth && linebufpos + 1 < (int)sizeof(linebuf);j++)
								linebuf[linebufpos++] = ' ';
					}
				}
				linebuf[linebufpos] = 0;
				Con_Printf ("%s\n", linebuf);
			}
		}
		else
			oneperline = true;
	}
	if (oneperline)
		for (i = 0;i < numfiles;i++)
			Con_Printf ("%s\n", search->filenames[i]);
	FS_FreeSearch(search);
	return (int)numfiles;
}

static void FS_ListDirectoryCmd (cmd_state_t *cmd, const char *cmdname, int oneperline)
{
	char pattern[1024] = {0};
	int has_pat  = false;

	if (Cmd_Argc(cmd) >= 3) {
		Con_PrintLinef ("usage:" NEWLINE "%s [path/pattern]", cmdname);
		return;
	}

	if (Cmd_Argc(cmd) == 2) {
		const char *spat = Cmd_Argv(cmd, 1);
		if (String_Ends_With (spat, "*") || String_Ends_With (spat, "?") || String_Ends_With (spat, "/" ))
			has_pat = true;
		c_strlcpy (pattern, spat);
		if (String_Ends_With (pattern, "/")) {
			c_strlcat (pattern, "*");
		}
	} else {
		c_strlcpy (pattern, "*");
	}

	Con_PrintLinef ("Trying with " QUOTED_S "%s" , pattern, has_pat ? "" : " (* ? supported)");

	int num_found = FS_ListDirectory (pattern, oneperline);
	if (!num_found) {
		Con_PrintLinef ("No files found.");
		if (has_pat == false) {
			c_strlcat (pattern, "*");
			Con_PrintLinef ("Trying with " QUOTED_S, pattern);
			if (!FS_ListDirectory (pattern, oneperline)) {
		Con_PrintLinef ("No files found.");
}
		}
	} else if (num_found == 1 && has_pat ==false  ) {
		Con_PrintLinef ("Note to list a folder, add trailing /");
	}
}

void FS_Dir_f(cmd_state_t *cmd)
{
	FS_ListDirectoryCmd(cmd, "dir", fs_one_per_line_true);
}

void FS_Ls_f(cmd_state_t *cmd)
{
	FS_ListDirectoryCmd(cmd, "ls", fs_one_per_line_false);
}

char *FS_FileWhich_Ta (ccs *filename, /*optional*/ int *p_type)
{
	int index;
	searchpath_t *sp = FS_FindFile(filename, &index, fs_quiet_true);
	int prefix_length = strlen (fs_basedir);
	if (!sp) {
		NOT_MISSING_ASSIGN (p_type, FS_FILETYPE_NONE_0);
		return NULL;
	}

	if (sp->pack) {
		if (sp->pack->vpack) {
			NOT_MISSING_ASSIGN (p_type, FS_FILETYPE_VPACK_3);
			return NULL; // No virtual support at this time.
		}
		
		NOT_MISSING_ASSIGN (p_type, FS_FILETYPE_PACK_4);
		
		if (!String_Starts_With (sp->pack->filename, fs_basedir)) {
			Con_DPrintLinef ("Pack %s does not start with fs_basedir %s", sp->pack->filename, fs_basedir);
			return NULL;
		}
		
		ccs *safter_basedir = &sp->pack->filename[prefix_length];
		ccs *next_slash = String_Skip_Char (safter_basedir, '/');
		if (!next_slash) {
			Con_DPrintLinef ("No next slash in %s", safter_basedir);
			return NULL;
		}
		int length = next_slash - safter_basedir;
		char *sout_za = Z_StrDup_Len_Z (safter_basedir, length);
		return sout_za;
	}

	NOT_MISSING_ASSIGN (p_type, FS_FILETYPE_FILE_1);
	
	ccs *amalgam_ta = T_StrDupf ("%s%s", sp->filename, filename);
	//ccs *safter_basedir_za = Z_StrDupf ("%s%s", sp->filename, filename);
	if (!String_Starts_With (amalgam_ta, fs_basedir)) {
		Con_DPrintLinef ("Pack %s does not start with fs_basedir %s", amalgam_ta, fs_basedir);
		Mem_FreeNull_ (amalgam_ta);
		return NULL;
	}
	
	ccs *safter_basedir = &sp->pack->filename[prefix_length];
	ccs *next_slash = String_Skip_Char (safter_basedir, '/');
	if (!next_slash) {
		Con_DPrintLinef ("No next slash in %s", amalgam_ta);
		Mem_FreeNull_ (amalgam_ta);
		return NULL;
	}
	int length = next_slash - safter_basedir;
	char *sout_za = Z_StrDup_Len_Z(safter_basedir, length);
	Mem_FreeNull_ (amalgam_ta);
	return sout_za;

}

void FS_Which_f(cmd_state_t *cmd)
{
	CMD_USAGE_NUM_ARGS_DESCRIBE_ (1, "<file>", "reports where the file is")

	ccs *filename = cmd_argv_parm1;
	int index;
	searchpath_t *sp = FS_FindFile(filename, &index, fs_quiet_true);
	if (!sp) {
		Con_PrintLinef ("%s isn't anywhere", filename);
		return;
	}

	if (sp->pack) {
		if (sp->pack->vpack)	Con_PrintLinef ("%s is in virtual package %sdir", filename, sp->pack->shortname);
			else				Con_PrintLinef ("%s is in package %s", filename, sp->pack->shortname);
		return;
	}

	Con_PrintLinef ("%s is file %s%s", filename, sp->filename, filename);
}


/*SAFE*/ ccs *FS_WhichPack (ccs *filename)
{
	int index;
	searchpath_t *sp = FS_FindFile(filename, &index, fs_quiet_true);
	if (sp && sp->pack)
		return sp->pack->shortname;
	else if (sp)
		return "";
	else
		return 0;
}

/*
====================
FS_IsRegisteredQuakePack

Look for a proof of purchase file file in the requested package

If it is found, this file should NOT be downloaded.
====================
*/
qbool FS_IsRegisteredQuakePack(const char *name)
{
	searchpath_t *search;
	pack_t *pak;

	// search through the path, one element at a time
	for (search = fs_searchpaths;search;search = search->next) {
		if (search->pack && !search->pack->vpack && String_Match_Caseless(FS_FileWithoutPath(search->filename), name))
			// TODO do we want to support vpacks in here too?
		{
			int (*strcmp_funct) (const char *str1, const char *str2);
			int left, right, middle;

			pak = search->pack;
			strcmp_funct = pak->ignorecase ? strcasecmp : strcmp;

			// Look for the file (binary search)
			left = 0;
			right = pak->numfiles - 1;
			while (left <= right)
			{
				int diff;

				middle = (left + right) / 2;
				diff = strcmp_funct (pak->files[middle].name, "gfx/pop.lmp");

				// Found it
				if (!diff)
					return true;

				// If we're too far in the list
				if (diff > 0)
					right = middle - 1;
				else
					left = middle + 1;
			}

			// we found the requested pack but it is not registered quake
			return false;
		}
	}

	return false;
}

int FS_CRCFile(const char *filename, size_t *filesizepointer)
{
	int crc = -1;
	unsigned char *filedata;
	fs_offset_t filesize;
	if (filesizepointer)
		*filesizepointer = 0;
	if (!filename || !*filename)
		return crc;
	filedata = FS_LoadFile(filename, tempmempool, fs_quiet_true, &filesize);
	if (filedata)
	{
		if (filesizepointer)
			*filesizepointer = filesize;
		crc = CRC_Block(filedata, filesize);
		Mem_Free(filedata);
	}
	return crc;
}

unsigned char *FS_Deflate(const unsigned char *data, size_t size, size_t *deflated_size, int level, mempool_t *mempool)
{
	z_stream strm;
	unsigned char *out = NULL;
	unsigned char *tmp;

	*deflated_size = 0;
#ifndef LINK_TO_ZLIB
	if (!zlib_dll)
		return NULL;
#endif

	memset(&strm, 0, sizeof(strm));
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	if (level < 0)
		level = Z_DEFAULT_COMPRESSION;

	if (qz_deflateInit2(&strm, level, Z_DEFLATED, -MAX_WBITS, Z_MEMLEVEL_DEFAULT, Z_BINARY) != Z_OK)
	{
		Con_Printf ("FS_Deflate: deflate init error!\n");
		return NULL;
	}

	strm.next_in = (unsigned char *)data;
	strm.avail_in = (unsigned int)size;

	tmp = (unsigned char *) Mem_Alloc(tempmempool, size);
	if (!tmp)
	{
		Con_Printf ("FS_Deflate: not enough memory in tempmempool!\n");
		qz_deflateEnd(&strm);
		return NULL;
	}

	strm.next_out = tmp;
	strm.avail_out = (unsigned int)size;

	if (qz_deflate(&strm, Z_FINISH) != Z_STREAM_END)
	{
		Con_Printf ("FS_Deflate: deflate failed!\n");
		qz_deflateEnd(&strm);
		Mem_Free(tmp);
		return NULL;
	}

	if (qz_deflateEnd(&strm) != Z_OK)
	{
		Con_Printf ("FS_Deflate: deflateEnd failed\n");
		Mem_Free(tmp);
		return NULL;
	}

	if (strm.total_out >= size)
	{
		Con_Printf ("FS_Deflate: deflate is useless on this data!\n");
		Mem_Free(tmp);
		return NULL;
	}

	out = (unsigned char *) Mem_Alloc(mempool, strm.total_out);
	if (!out)
	{
		Con_Printf ("FS_Deflate: not enough memory in target mempool!\n");
		Mem_Free(tmp);
		return NULL;
	}

	*deflated_size = (size_t)strm.total_out;

	memcpy(out, tmp, strm.total_out);
	Mem_Free(tmp);

	return out;
}

static void AssertBufsize(sizebuf_t *buf, int length)
{
	if (buf->cursize + length > buf->maxsize)
	{
		int oldsize = buf->maxsize;
		unsigned char *olddata;
		olddata = buf->data;
		buf->maxsize += length;
		buf->data = (unsigned char *) Mem_Alloc(tempmempool, buf->maxsize);
		if (olddata)
		{
			memcpy(buf->data, olddata, oldsize);
			Mem_Free(olddata);
		}
	}
}

unsigned char *FS_Inflate(const unsigned char *data, size_t size, size_t *inflated_size, mempool_t *mempool)
{
	int ret;
	z_stream strm;
	unsigned char *out = NULL;
	unsigned char tmp[2048];
	unsigned int have;
	sizebuf_t outbuf;

	*inflated_size = 0;
#ifndef LINK_TO_ZLIB
	if (!zlib_dll)
		return NULL;
#endif

	memset(&outbuf, 0, sizeof(outbuf));
	outbuf.data = (unsigned char *) Mem_Alloc(tempmempool, sizeof(tmp));
	outbuf.maxsize = sizeof(tmp);

	memset(&strm, 0, sizeof(strm));
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	if (qz_inflateInit2(&strm, -MAX_WBITS) != Z_OK)
	{
		Con_Printf ("FS_Inflate: inflate init error!\n");
		Mem_Free(outbuf.data);
		return NULL;
	}

	strm.next_in = (unsigned char *)data;
	strm.avail_in = (unsigned int)size;

	do
	{
		strm.next_out = tmp;
		strm.avail_out = sizeof(tmp);
		ret = qz_inflate(&strm, Z_NO_FLUSH);
		// it either returns Z_OK on progress, Z_STREAM_END on end
		// or an error code
		switch(ret)
		{
			case Z_STREAM_END:
			case Z_OK:
				break;

			case Z_STREAM_ERROR:
				Con_Print("FS_Inflate: stream error!\n");
				break;
			case Z_DATA_ERROR:
				Con_Print("FS_Inflate: data error!\n");
				break;
			case Z_MEM_ERROR:
				Con_Print("FS_Inflate: mem error!\n");
				break;
			case Z_BUF_ERROR:
				Con_Print("FS_Inflate: buf error!\n");
				break;
			default:
				Con_Print("FS_Inflate: unknown error!\n");
				break;

		}
		if (ret != Z_OK && ret != Z_STREAM_END)
		{
			Con_Printf ("Error after inflating %u bytes\n", (unsigned)strm.total_in);
			Mem_Free(outbuf.data);
			qz_inflateEnd(&strm);
			return NULL;
		}
		have = sizeof(tmp) - strm.avail_out;
		AssertBufsize(&outbuf, max(have, sizeof(tmp)));
		SZ_Write(&outbuf, tmp, have);
	} while(ret != Z_STREAM_END);

	qz_inflateEnd(&strm);

	out = (unsigned char *) Mem_Alloc(mempool, outbuf.cursize);
	if (!out)
	{
		Con_Printf ("FS_Inflate: not enough memory in target mempool!\n");
		Mem_Free(outbuf.data);
		return NULL;
	}

	memcpy(out, outbuf.data, outbuf.cursize);
	Mem_Free(outbuf.data);

	*inflated_size = (size_t)outbuf.cursize;

	return out;
}



/*
================
FS_InitOnce
================
*/

void FS_InitOnce(void)
{
	fs_mempool = Mem_AllocPool("file management", 0, NULL);

	FS_InitOnce_Commands();

	PK3_OpenLibrary ();

	// initialize the self-pack (must be before FS_InitOnce_COM_InitGameType_Read_Exe_CmdLine_Parms as it may add command line options)
	FS_Init_SelfPack();

	// Baker: GALAXY ... this depends on a free laying file "zircon/gfx/qplaque.png"
	// If detected ...
	// Only zircon folder is added to 
	// GALAXY #1 --- Keyname menu is different ... bindnames = zirconbindnames;
	// GALAXY #2 --- Keynames are proper cased in key menu.
	// GALAXY #3 --- r_water_resolutionmultiplier 1.0 default is different for setting #0.
	// GALAXY #4 --- gamename is "Zircon X" instead of Zircon Beta-Quake
	// GALAXY #5 --- we do not check the data folder.
	// GALAXY #6 --- gamedirname1 = gamenetworkfiltername = "zircon" this overrides the "id1"

	
#ifdef __ANDROID__
	fs_is_zircon_galaxy = FS_File_Exists_Unsafe (	"/sdcard/zircon/gfx/qplaque.png"); // Baker .. this must use realpath
#else
	va_super (galaxy_test_file_realpath, MAX_OSPATH_EX_1024, "%s" "zircon/gfx/qplaque.png", fs_basedir);
	fs_is_zircon_galaxy = FS_File_Exists_Unsafe(galaxy_test_file_realpath);
#endif

#if 1
	RELATED_ (COM_SetGameType)
	RELATED_ (gamedirname1 /*when this fucker get set? A: COM_SetGameType */ customgamedirname1 is command line parameter)

	// Baker: Find every folder.  If it isn't bin32/bin64/downloads
	// Check for #contents.pk3
	if (FS_Baker_ListGameDirs_Is_Total_Conversion ()) { // modlist.txt
		// Do stuff here?
		//int j = 5;
		//char *s = mod_list_folder_name; // Like "pac"
	}
#endif

	// detect gamemode from commandline options or executable name
	FS_InitOnce_COM_InitGameType_Read_Exe_CmdLine_Parms();


	FS_Init_Dir();
}



// Returns -1 if not file
/*SAFE*/ fs_offset_t FS_FileSizeEx_Safe (ccs *filename)
{
	qfile_t *f = FS_OpenRealFile (filename, "rb", fs_quiet_FALSE);
	if (!f)
		return not_found_neg1;
	fs_offset_t filesize = FS_FileSize (f);
	FS_Close (f);
	return filesize;
}


/*SAFE*/ qbool FS_IsRealFile (ccs *filename)
{
	int index; // Why?
	// Baker: FS_FindFile doesn't alloc a search.
	searchpath_t *sp = FS_FindFile(filename, &index, fs_quiet_true);
	if (!sp) {
		return false; //Con_PrintLinef ("%s isn't anywhere", filename);
	}
	if (sp->pack) {
		if (sp->pack->vpack)			
			return false;  //Con_PrintLinef ("%s is in virtual package %sdir", filename, sp->pack->shortname);
		else
			return false; //Con_PrintLinef ("%s is in package %s", filename, sp->pack->shortname);
	}
	else
		return true;
}

// UNSAFE

/*
====================
FS_SysOpen

Internal function used to create a qfile_t and open the relevant non-packed file on disk
====================
*/
/*REALPATH*/ qfile_t *FS_SysOpen (const char *realpath, const char *mode, qbool nonblocking)
{
	qfile_t *file;

	file = (qfile_t *)Mem_Alloc (fs_mempool, sizeof (*file));
	file->ungetc = EOF;
	file->handle = FS_SysOpenFiledesc(realpath, mode, nonblocking);
	if (!FILEDESC_ISVALID(file->handle)) {
		Mem_Free (file);
		return NULL;
	}

	file->f_filename = Mem_strdup(fs_mempool, realpath);

	file->real_length = FILEDESC_SEEK (file->handle, 0, SEEK_END);

	// For files opened in append mode, we start at the end of the file
	if (mode[0] == 'a')
		file->position = file->real_length;
	else
		FILEDESC_SEEK (file->handle, 0, SEEK_SET);

	return file;
}

qbool FS_IsDirectory (ccs *filename)
{
	int type = FS_FileOrDirectoryType (filename);
	qbool is_directory = type == FS_FILETYPE_DIRECTORY_2;
	return is_directory;
}


/*REALPATH*/ static void FS_mkdir (const char *realpath)
{
	WPATHDEF(pathw); // Baker: This just does wchar var[MAX_OSWPATH /*1024*/] on Win32 and nothing if non-win32

	if (Sys_CheckParm("-readonly"))
		return;

	WIDE(realpath, pathw); // Baker: win: fromwtf8 Non-Win: Do nothing

#ifdef _WIN32
	if (_wmkdir (pathw) == -1)
#else
	if (mkdir (realpath, 0777) == -1)
#endif
	{
		// No logging for this. The only caller is FS_CreatePath (which
		// calls it in ways that will intentionally produce EEXIST),
		// and its own callers always use the directory afterwards and
		// thus will detect failure that way.
	}
}

/*REALPATH*/ qbool FS_chdir (const char *realpath)
{
	WPATHDEF(pathw); // Baker: This just does wchar var[MAX_OSWPATH /*1024*/] on Win32 and nothing if non-win32

	WIDE(realpath, pathw); // Baker: win: fromwtf8 Non-Win: Do nothing

#ifdef _WIN32
	
	int result = _wchdir (pathw); // Baker: We are in the "c:/galaxy/zircon" here!  (not c:\galaxy!)
#else
	// Upon successful completion, the chdir subroutine returns a value of 0. Otherwise, a value of -1
	int result = chdir (fs_basedir);
#endif
	if (result != 0) {
		Con_PrintLinef (CON_BRONZE "FS_chdir: chdir %s failed", realpath);
		return false;
	}
	return true;
}

/*
============
FS_CreatePath

Only used for FS_OpenRealFile.
============
*/
CALLERS_ (FS_OpenRealFile)
/*REALPATH*/ void FS_CreatePath (char *realpath)
{
	char *ofs, save;

	for (ofs = realpath + 1 ; *ofs ; ofs ++)
	{
		if (*ofs == '/' || *ofs == '\\')
		{
			// create the directory
			save = *ofs;
			*ofs = 0;
			FS_mkdir (realpath);
			*ofs = save;
		}
	}
}

qbool FS_WriteFileQuick (ccs *filename, ccs *data, size_t datalen)
{
	qfile_t *f = FS_OpenRealFile(filename, "wb", fs_quiet_FALSE); // WRITE-EON - Curl begin
	if (!f)
		return false;
	
	FS_Write (f, data, datalen);
	FS_Close(f);
	return true;
}


qbool FS_WriteFileStringQuick (ccs *filename, ccs *text)
{
	return FS_WriteFileQuick (filename, text, strlen(text) );
}



// Unix epoch time
/*SAFE*/ double FS_RealFile_Time (ccs *filename)
{
	// FRAGILE - What happens with weirdo characters?
	// _wstat
	char realfilename[MAX_OSPATH_EX_1024];
	ccs *x_gamedir = fs_numgamedirs ? fs_gamedirs[fs_numgamedirs - 1] : gamedirname1; // Con_PrintVarString (x_gamedir);
	c_strlcpy (realfilename, fs_basedir); // Has trail, right?
	c_strlcat (realfilename, x_gamedir);
    c_strlcat (realfilename, filename);
	File_URL_Edit_SlashesForward_Like_Unix (realfilename);
	double filetime_since1970 = FS_File_DateTime_Unsafe (filename);
	return filetime_since1970;
	//f = FS_OpenRealFile (path_scripts_map_n_dot_shader, "rb", fs_quiet_FALSE);  // WRITE-EON pak this map
	//// Is file or directory?
	//struct stat st_buf = {0};

	//int status = stat (path_to_file, &st_buf );
	//if (status != 0)
	//	return 0;

	//return (double)st_buf.st_mtime;
}

#include "fs_file_url_string_functions.c.h"
#include "fs_unsafe.c.h"

void FS_InitOnce_Commands(void)
{
	Cvar_RegisterVariable (&scr_screenshot_name);
	Cvar_RegisterVariable (&fs_empty_files_in_pack_mark_deletions);
	Cvar_RegisterVariable (&cvar_fs_gamedir);

	Cvar_RegisterVariable (&q3map2_csg_compiler);
	Cvar_RegisterVariable (&q3map2_compiler);

	Cvar_RegisterVariable (&q3map2_csg_switches);
	Cvar_RegisterVariable (&q3map2_bsp_switches);
	Cvar_RegisterVariable (&q3map2_vis_switches);
	Cvar_RegisterVariable (&q3map2_lit_switches);

	Cmd_AddCommand(CF_SHARED, "gamedir", FS_GameDir_f, "changes active gamedir list (can take multiple arguments), not including base directory (example usage: gamedir ctf)");
	Cmd_AddCommand(CF_SHARED, "game", FS_GameDir_f, "changes active gamedir list (can take multiple arguments), not including base directory (example usage: gamedir ctf) [Zircon]"); // Baker r1203: "game" command like Quakespasm/FitzQuake 0.85, Quakespasm and most singleplayer engines use this command for "gamedir" switching
	Cmd_AddCommand(CF_SHARED, "purgeall", FS_PurgeAll_f, "Purges all models, textures, sounds [Zircon]");
	Cmd_AddCommand(CF_SHARED, "fs_rescan", FS_Rescan_f, "rescans filesystem for new pack archives and any other changes");
	Cmd_AddCommand(CF_SHARED, "path", FS_Path_f, "print searchpath (game directories and archives)");
	Cmd_AddCommand(CF_SHARED, "dir", FS_Dir_f, "list files in searchpath matching an * filename pattern, one per line");
	Cmd_AddCommand(CF_SHARED, "dirpat", FS_DirPat_f, "[fssearch_t] list files in searchpath matching an * filename pattern, one per line");
	Cmd_AddCommand(CF_SHARED, "pwd", FS_Pwd_f, "what is current working directory [Zircon]");  // Baker r3101: pwd command to say the current directory
	Cmd_AddCommand(CF_SHARED, "zipinfo", FS_Zipinfo_f, "zipinfo <file> list files in a zip [Zircon]");
	Cmd_AddCommand(CF_SHARED, "ls", FS_Ls_f, "list files in searchpath matching an * filename pattern, multiple per line");
	Cmd_AddCommand(CF_SHARED, "which", FS_Which_f, "accepts a file name as argument and reports where the file is taken from");

#ifdef CONFIG_MENU
	Cmd_AddCommand(CF_SHARED, "parse", FS_Parse_f, "parse a string or parse clipboard [Zircon]");
	Cmd_AddCommand(CF_SHARED, "bitatomize", FS_BitAtomize_f, "break an integer down into bits [Zircon]");
	Cmd_AddCommand(CF_SHARED, "constants", FS_Constants_f, "constants [Zircon]");
	Cmd_AddCommand(CF_SHARED, "autocvars_from_clipboard", FS_Autocvars_From_Clip, "Convert autocvar text into QuakeC commands [Zircon]");
	Cmd_AddCommand(CF_SHARED, "divisors", FS_Divisors_f, "print whole number divisors for a number [Zircon]");
	//void UI_Info_f (cmd_state_t *cmd);
	void UI_Set_f (cmd_state_t *cmd);

	Cvar_RegisterVariable (&ui_export_magnification);
	Cvar_RegisterVariable (&ui_import_magnification);
	
	Cvar_RegisterVariable (&ui_full_canvas);

	Cvar_RegisterVariable (&png_texture_suppress);
	
	
	
	//Cmd_AddCommand(CF_SHARED, "ui_info", UI_Info_f, "dump ui control types and properties [Zircon]");
	Cmd_AddCommand(CF_SHARED, "oset", UI_Set_f, "[name] [property] [value] set an object property [Zircon]");
	Cmd_AddCommand(CF_SHARED, "repeat", FS_Repeat_f, "execute a command or alias many times [Zircon]");
#ifdef _DEBUG
	Cmd_AddCommand(CF_SHARED, "d_repeat", FS_D_Repeat_f, "[start] [beyond] [increment] Debug command [Zircon]");
	Cmd_AddCommand(CF_SHARED, "d_printf", FS_D_Printf_f, "[start] [beyond] [increment] Debug command [Zircon]");
#endif
	Cmd_AddCommand(CF_SHARED, "colorpcts", FS_ColorPcts_f, "Converts rgb 0-255 colors to pct colors to clipboard [Zircon]");
	Cmd_AddCommand(CF_SHARED, "base64clipboard", FS_Base64Clipboard_f, "takes text on clipboard and converts it to base 64 [Zircon]");
	Cmd_AddCommand(CF_SHARED, "base64compressedclipboard", FS_Base64ClipboardCompressed_f, "takes text on clipboard, zip shrinks it and converts it to base 64 [Zircon]");
	Cmd_AddCommand(CF_SHARED, "base64decompressedclipboard", FS_Base64ClipboardDeCompressed_f, "takes text on clipboard, zip shrinks it and converts it to base 64 [Zircon]");
	Cmd_AddCommand(CF_SHARED, "jpegsplit", FS_JpegSplit_f, "jpegsplit <folder> (test) -- or --  <folder> go (run conversion!) --- load all TGA/PNG in supplied folder, write them as .jpg to same directory including any _alpha jpegs.  DarkPlaces pattern matching hates periods '.' in path names, beware! [Zircon]");
	Cmd_AddCommand(CF_SHARED, "jpegresample", FS_JpegResample_f, "jpegresample <folder> <w> <h> Resample images write jpeg [Zircon]");
	Cmd_AddCommand(CF_SHARED, "jpegcheck", FS_JpegCheck_f, "jpegcheck <folder> Check all jpg and jpeg and report compression savings.  [Zircon]");

	void FS_GifToShader_f (cmd_state_t *cmd); // Needed at the moment Nov 21 2024
	void FS_Inventory_f (cmd_state_t *cmd); // Needed at the moment Nov 21 2024
	Cmd_AddCommand(CF_SHARED, "giftoshader", FS_GifToShader_f, "giftoshader [file.gif] makes animated gif [Zircon]");

#if 1
	Cmd_AddCommand(CF_SHARED, "inventorydump", FS_Inventory_f, "inventory [Zircon]"); // collides with galaxy "inventory" command
#endif

#if 0 // These experiments are not solidified enough
	Cmd_AddCommand(CF_SHARED, "vegetationmake", FS_VegetationMake_f, "vegetationmake <image> - creates a vegetation model using texture name [Zircon]");
	Cmd_AddCommand(CF_SHARED, "mapgenpoints", FS_Mapgenpoints_f, "mapgenpoints x0 y0 z0 x1 y1 z21 -- creates map brushes and copies to clipboard with textures/common/alpha_000 [Zircon]");
	Cmd_AddCommand(CF_SHARED, "mapgenmask", FS_Mapgenmask_f, "mapgenpoints [file.tga] [x] [y] [w] [h] [scale32] -- creates map points from image and creates alpha set brushes for pasting into .map [Zircon]");
	Cmd_AddCommand(CF_SHARED, "objmake", FS_ObjMake_f, "objmake [file.txt] makes animated gif [Zircon]"); // Strip this.
#endif

#if 1 //def SHELLENABLE // Baker: In the long run, do not want to have this 
	if (Sys_CheckParm ("-shell")) {
		
		
		Cmd_AddCommand(CF_SHARED, "shell", FS_Shell_NonBlocking_f, "execute a command line [optional args], Zircon will not wait for it to complete [Zircon]");
		Cmd_AddCommand(CF_SHARED, "shell_blocking", FS_Shell_Blocking_f, "execute a command line and wait for it to complete [optional args] [Zircon]");
	}
#endif // 1
	//void FS_Dungeon_f (cmd_state_t *cmd);
	//Cmd_AddCommand(CF_SHARED, "dungeon", FS_Dungeon_f, "dungeon test [Zircon]");

#endif // CONFIG_MENU
}
