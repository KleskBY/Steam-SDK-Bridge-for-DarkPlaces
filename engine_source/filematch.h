// filematch.h

/*
Copyright (C) 2006-2021 DarkPlaces contributors

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef FILEMATCH_H
#define FILEMATCH_H

#include "qtypes.h"

typedef struct stringlist_s {
#if 0
	mempool_t *list_mempool;
#endif
	/// maxstrings changes as needed, causing reallocation of strings[] array
	int maxstrings;
	int numstrings;
	char **strings;

} stringlist_t;

// Baker: floatlist_s
// To store floats with intention of list to be null terminated
// at all times IF count > 0
// So ..
// while (*floats++)
// .. would work

typedef struct _floatlist_s {
	float	*floats;
	int		maxsize;
	int		count;
} floatlist_s;

typedef struct _voidlist_s {
	const void	**vloats;
	int			maxsize;
	int			count;
} voidlist_s;

void voids_add1 (voidlist_s *vlist, const void *p);
void voids_freecontents (voidlist_s *vlist);


void floats_add1 (floatlist_s *flist, float p);
void floats_add2 (floatlist_s *flist, float px, float py);
void floats_add3 (floatlist_s *flist, float px, float py, float pz);

#define floats_addvector(plist, v) floats_add3(plist,v[0],v[1],v[2])

void floats_print (floatlist_s *flist, void (*myprintf)(const char *, ...));
void floats_dump (floatlist_s *flist);

#define floats_vector_read_at(vsetme,flist,idx) \
	vsetme[0] = flist->floats[idx + 0]; \
	vsetme[1] = flist->floats[idx + 1]; \
	vsetme[2] = flist->floats[idx + 2] // Ender


char *floats_join_delim12_delim_zalloc (floatlist_s *flist, int number_per_vert, ccs *s_delimiter12, ccs *s_delimiter_elements, int shall_quote);

typedef struct {
	int magnifier;	// Q: Is vid_magnification an integer always: YES
	int offset_xx;
	int offset_yy;
} mag_s;

char *floats_join_delim12_delim_mag_zalloc (floatlist_s *flist, int number_per_vert, ccs *s_delimiter12, ccs *s_delimiter_elements, int shall_quote, mag_s *magni_opt);

// Returns 0 if the parse of all elements went as expected
int floats_append_parse_space_comma_num_elements_ignored (floatlist_s *flist, ccs *s_value, int number_per_vert, ccs *s_delimiter12, ccs *s_delimiter_elements);

void floats_freecontents (floatlist_s *flist);

typedef struct _int32list_s {
	int		*ints;
	int		maxsize;
	int		count;
} int32list_s;

void int32s_add1 (int32list_s *ilist, int p);
int int32s_find_index (int32list_s *ilist, int find_int);
void int32s_add2 (int32list_s *ilist, int px, int py);
void int32s_add3 (int32list_s *ilist, int px, int py, int pz);
qbool int32s_add1_unique_didadd (int32list_s *ilist, int p);

void int32s_sort (int32list_s *list, qbool uniq);
void int32s_print (int32list_s *ilist, void (*myprintf)(const char *, ...));
void int32s_dump (int32list_s *ilist);
void int32s_command_scale (int32list_s *ilist, int iscale);

void int32s_count_set (int32list_s *ilist, int newcount); // increases only.

int int32s_append_split_dequote (int32list_s *ilist, ccs *s_value, ccs *s_delimiter_elements);
char *int32s_join_delim_zalloc (int32list_s *ilist, ccs *s_delimiter_elements, int shall_quote);

char *int32s_join_delim_mag_zalloc (int magni_factor, int32list_s *ilist, ccs *s_delimiter_elements, int shall_quote); // SIZEOIC
char *int32s_join_delim_mag_reverse_zalloc (int magni_factor, int32list_s *ilist, ccs *s_delimiter_elements, int shall_quote);




void int32s_freecontents (int32list_s *ilist);


int matchpattern(const char *in, const char *pattern, int caseinsensitive);
int matchpattern_with_separator(const char *in, const char *pattern, int caseinsensitive, const char *separators, qbool wildcard_least_one);

// Baker: stringlist is zonemempool
void stringlistinit(stringlist_t *list);
void stringlistfreecontents(stringlist_t *list);
void stringlistappend(stringlist_t *list, const char *text);
void stringlistappend_len (stringlist_t *list, const char *text, int text_len);
int stringlistappend_split_delimiter_linenumbers (stringlist_t *plist, ccs *s, ccs *delimiter);

#define stringlist_last(LISTX) LISTX.strings[LISTX.numstrings - 1]
#define stringlist_lastp(PLISTX) PLISTX->strings[PLISTX->numstrings - 1]


int stringlistappendfilelines_did_load (stringlist_t *plist, ccs *file, qbool is_base1, int is_number_column);

const char **stringlist_nullterm_add (stringlist_t *list);

qbool stringlistappend_unique_didAdd (stringlist_t *plist, ccs *s);
qbool stringlistappendf_unique_didAdd (stringlist_t *plist, ccs *fmt, ...) DP_FUNC_PRINTF(2);
qbool stringlistappend_unique_evens_didAdd (stringlist_t *plist, ccs *text); // Does evens only

void stringlistappendf (stringlist_t *list, ccs *fmt, ...)  DP_FUNC_PRINTF(2);
void stringlistappendf2 (stringlist_t *list, ccs *s, ccs *fmt, ...) DP_FUNC_PRINTF(3);

void stringlistappend2 (stringlist_t *list, ccs *s, ccs *s2);
void stringlistappend3 (stringlist_t *list, ccs *s, ccs *s2, ccs *s3);
void stringlistappend4 (stringlist_t *list, ccs *s, ccs *s2, ccs *s3, ccs *s4);
void stringlistappend5 (stringlist_t *list, ccs *s, ccs *s2, ccs *s3, ccs *s4, ccs *s5);


// Moved to darkplaces.h so fssearch_t is defined
void stringlistappendlist (stringlist_t *plist, const stringlist_t *add_these); // Baker: Add a list to a list
void stringlistappendlistunique (stringlist_t *plist, const stringlist_t *add_these);

//void stringlistprint (stringlist_t *list, const char *title_optional, void (*myprintf)(const char *, ...) );
void stringlistprint_with_title (stringlist_t *list, void (*myprintf)(ccs *, ...), ccs *fmt, ...) DP_FUNC_PRINTF(3);
void stringlist_replace_at_index (stringlist_t *list, int idx, const char *text);
void stringlistsort(stringlist_t *list, qbool uniq);
#define stringlistsort_no_unique(plist) stringlistsort (plist, fs_make_unique_false)
#define stringlistsort_unique(plist) stringlistsort (plist, fs_make_unique_true)
void stringlist_condump_with_linenums (stringlist_t *plist);
void stringlist_condump_with_linenums_band (stringlist_t *plist, int numcolumns);
void stringlist_condump_raw (stringlist_t *plist);
void stringlistsort_custom(stringlist_t *list, qbool uniq, int myfunc(const void *, const void *) );

int stringlist_quote_or_comma_decode_count (stringlist_t *plist);
int stringlist_quote_or_comma_encode_count (stringlist_t *plist);

// Returns NULL if no commas or quotes.
char *stringlist_to_base64_comma_delim_char12_encode_zalloc (stringlist_t *plist, qbool shall_force);
int stringlist_append_from_base64_comma_delim_char12_decode (stringlist_t *plist, ccs *sbase64);

// Returns first idx that does not match (-1 means no differences).  If lists different size returns 0.
int stringlist_compare (stringlist_t *plist1, stringlist_t *plist2);

void stringlist_bytedump (stringlist_t *plist, const byte *src, size_t src_sizeof);
void stringlist_stringdump (stringlist_t *plist, ccs *src);

void stringlistappend_blob (stringlist_t *list, const byte *blob, size_t blobsize); // Put binary data in a stringlist

void stringlistsort_substring(stringlist_t *list, qbool uniq, int startpos, int slength); // Substring sort

void stringlist_append_listdirectory(stringlist_t *list, ccs *basepath, ccs *filepattern);

/*JOIN*/
void stringlist_join_buf (stringlist_t *p_stringlist, ccs *s_delimiter, char *buf, size_t buflen); // JOIN

// Free with: baker_string_t *bs = NULL; if (bs) BakerString_Destroy_And_Null_It (&bs);
baker_string_t *stringlist_join_lines_bkalloc (stringlist_t *plist);
baker_string_t *stringlist_join_bkalloc (stringlist_t *p_stringlist, ccs *s_delimiter);

char *stringlist_join_lines_zalloc (stringlist_t *plist);
char *stringlist_join_spaced_zalloc (stringlist_t *plist);
char *stringlist_join_delim_zalloc (stringlist_t *plist, ccs *s_delimiter, int shall_quote);

/*SPLIT*/
void stringlistappend_split_len (stringlist_t *plist, ccs *text_in, int text_strlen, ccs *s_delimiter);
void stringlistappend_split (stringlist_t *plist, ccs *text_in, ccs *s_delimiter);

int stringlistappend_from_dir_pattern (stringlist_t *p_stringlist, const char *s_optional_dir_no_slash, const char *s_optional_dot_extension, int wants_strip_extension);
int stringlistappend_from_pattern (stringlist_t *p_stringlist, ccs *s_pattern);
int stringlistappend_dir_matches (stringlist_t *p_stringlist, ccs *s_prefix);
struct fssearch_s;
void stringlistappendfssearch (stringlist_t *plist, fssearch_t *t); // t == NULL is ok and handled
void stringlistappendfssearch_modulo (stringlist_t *plist, fssearch_t *t, int modulation); // 2 = 50%, 4 = 25%
void stringlistappend_search_pattern (stringlist_t *plist, const char *s_pattern);


/*LINE WORD -> SPLIT*/

// Now requires something to help with line numbers.
void stringlistappend_tokenize_qcquotes (stringlist_t *plist, int32list_s *linelist, const char *text);

/*LINE SPLIT*/
void stringlistappend_textlines_cr_scrub (stringlist_t *plist, const char *text_in);
#define stringlistappend_split_lines_cr_scrub(plist, s) stringlistappend_textlines_cr_scrub(plist, s)

void stringlistappend_textlines_len_cr_scrub (stringlist_t *plist, const char *text_in, int text_strlen);


int stringlistappend_csv_nl_tab (stringlist_t *plist, const char *s);

void stringlist_from_delim (stringlist_t *p_stringlist, const char *s_space_delimited);
int stringlist_find_index (stringlist_t *p_stringlist, ccs *s_find_this);
#define stringlist_has_string(plist,s) (stringlist_find_index(plist,s) != not_found_neg1)

int stringlist_find_index_columns (stringlist_t *p_stringlist, ccs *s_find_this, int numcolumns);

qbool stringlist_string_endswithany (ccs *s, stringlist_t *plist);

// Feb 18 2025
int stringlist_find_column_number (stringlist_t *p_stringlist, ccs *s_find_this, int numcolumns);
int stringlist_find_row_number (stringlist_t *p_stringlist, ccs *s_find_this, int numcolumns);

// SEPT 28 2024

// https://developer.valvesoftware.com/wiki/MAP_(file_format)
// ( x1 y1 z1 ) ( x2 y2 z2 ) ( x3 y3 z3 ) path/texturename
//  [ Ux Uy Uz Uoffset ] [ Vx Vy Vz Voffset ] rotation Uscale Vscale SurfaceFlag ContentsFlag Value
//
RELATED_ (entitylist_translate_brushes)

#define FT_ROTATION_0	0
#define FT_USCALE_1		1
#define FT_VSCALE_2		2

#define X1_UX_0			0
#define X1_UY_1			1
#define X1_UZ_2			2
#define X1_UOFFSET_3	3

#define X2_VX_0			0
#define X2_VY_1			1
#define X2_VZ_2			2
#define X2_VOFFSET_3	3

// Baker: May 23 2025 - to the best of my knowledge, UOFFSET 3 and VOFFSET 3 are texture sized pixels.
// This means we need to know the texture size.

//    0 degrees:	ambientcg/Rock051 [ 1 0 0 0 ] [ 0 0 -1 0 ] 0 0.5 0.5
// rot 45:0 degrees:ambientcg/Rock051 [ 0.707107 0 -0.707107 0 ] [ -0.707107 0 -0.707107 0 ] 45 0.5 0.5

// texture is on Y plane                cos(30) 0 sin(30)    -sin(30) 0 -cos(30)
// tx at 0 pixels is ... ?
// ty at 0 pixels is ... ?
// rot 30:0 degrees:ambientcg/Rock051 [ 0.866025 0 -0.5 0 ] [ -0.5 0 -0.866025 0 ] 30 0.5 0.5

// Z is the most likely
// Z 45 degrees:	ambientcg/Rock051 [ 0.707107 0.707107 0 104.706 ] [ 0 0 -1 0 ] 0 0.5 0.5  cos sin
// X 45 degrees:	ambientcg/Rock051 [ 1 0 0 0 ] [ 0 0.707107 -0.707107 -195.216 ] 0 0.5 0.5 (tilt back like ceiling) tx unaffected, ty affected
// Y 45 degrees:	ambientcg/Rock051 [ 0.707107 0 -0.707107 512 ] [ -0.707107 0 -0.707107 212.077 ] 0 0.5 0.5

#define nsewcf_north_1	1
#define nsewcf_south_2	2
#define nsewcf_east_3	4
#define nsewcf_west_4	8
#define nsewcf_ceil_5	16
#define nsewcf_floor_6	32

typedef struct _brushrow_st {
	vec3_t a;			// ( 480 704 -260 )
	vec3_t b;			// ( 480 672 -260 )
	vec3_t c;			// ( 480 704 -132 )
	char *pbrtexture;	// common/caulk
	vec4_t xtra1;		// [ 0 1 0 0 ]
	vec4_t xtra2;		// [ 0 0 -1 0 ]
	int trail_count;	// There are 3 or more at the end... supposedly it can go up to 8
//( 32768 -34816 10240 ) ( 32768 -34816 -512 ) ( 32768 34816 10240 ) sky/treefall_nite_sky [ 0 1 0 -0 ]
//		[ 0 0 -1 -0 ] 0 0.5 0.5 // 3 numbers
//( 34816 34816 10240 ) ( 34816 34816 -512 ) ( 34816 -34816 10240 ) common/caulk [ 0 1 0 -0 ] [ 0 0 -1 -0 ]
//		              0 0.5 0.5 0 160 0 // 6 numbers

	double ftrail[8];	// -0 1 1 0 160 0   // rotation Uscale Vscale SurfaceFlag ContentsFlag Value
//
	vec3_t			bfmins;
	vec3_t			bfmaxs;
	vec3_t			bfdims;
	int				nsewcf_bits;

} brushrow_s;
// If bfmins 0

#define face_retexture_z(NFACE,TEXTURENAME) \
	Mem_FreeNull_ (NFACE->pbrtexture); \
	NFACE->pbrtexture = Z_StrDup (TEXTURENAME) // Ender


typedef struct _brushrowlist_st {
	int				maxsize;
	int				count;
	brushrow_s		*brushrow;	// PAGE
} brushrowlist_t;

typedef struct _patchrow_st {
// ( -126 -192 128 8 0 ) ( -126 -192 384 8 -6 ) ( -126 -192 640 8 -12 )
	float flots[300];

#if 1 // JULY 30 2025
	vec3_t			bpmins;
	vec3_t			bpmaxs;
	vec3_t			bpdims;
#endif
} patchrow_s;

// list of // ( -126 -192 128 8 0 ) ( -126 -192 384 8 -6 ) ( -126 -192 640 8 -12 )
typedef struct _patchrowlist_st {
	int maxsize;
	int count;
	patchrow_s		*patchrow;	// PAGE
} patchrowlist_t;

typedef enum _quadrant6 {
	quadrant_none_neg1 = -1,
	quadrant_west_0 = 0,
	quadrant_east_1 = 1,
	quadrant_south_2 = 2,
	quadrant_north_3 = 3,
	quadrant_floor_4 = 4,
	quadrant_ceiling_5 = 5,
	quadrant_MAXCOUNT_6 = 6, // Used to dim arrays only.
	quadrant_triangle_friend_7 = 7,
	quadrant_adjacent_any_8 = 8,
	quadrant_diag_adjacent_any_9 = 9,
	// quadrant_lightrod_6,
} quadrant6_e;

#define BRUSH_IS_5_SIDED_TRI(pbrush) (pbrush->brushrowlist.count == (3 + 2))
#define BRUSH_IS_5_or_6_SIDED(pbrush) (isin2(pbrush->brushrowlist.count, 5,6))	// TRI or RECT -- if flat bottom with perpendicular bottom verts as sides

typedef enum {
	nsew_bits_solid_neg1	= -1,
	nsew_bits_none_0		= 0,
	nsew_bits_north_1		= 1,
	nsew_bits_south_2		= 2,
	nsew_bits_east_4		= 4,
	nsew_bits_west_8		= 8,
	nsew_bits_northeast_16	= 16,	// Diag
	nsew_bits_northwest_32	= 32,	// Diag
	nsew_bits_southeast_64	= 64,	// Diag
	nsew_bits_southwest_128	= 128,	// Diag
	nsew_bits_ceil_256		= 256,
	nsew_bits_floor_512		= 512,
} nsew_bits_e;


typedef struct _brush_st {
	brushrowlist_t	brushrowlist;
	int				is_a_patch;

	char			*texture;	// liquids_lava/lava_blue_LX_1000_alpha_50  bare texture
	float			rows;		// ( 9 3 536870920 16 1000 ) always 5 per column
	float			cols;
	float			other3[3];
	patchrowlist_t	patchrowlist; // ( -126 -192 128 8 0 ) ( -126 -192 384 8 -6 ) ( -126 -192 640 8 -12 )
//
	vec3_t			bxmins;	RELATED_ (entitylist_metric_world_brushes_with_walls) // will fill it in.
	vec3_t			bxmaxs;	// will it do brush rows? Yes
	vec3_t			bxdims; // It does not do entities.  That includes func_group.

	int				quadrants_brush_faces_plus1[quadrant_MAXCOUNT_6];		// LRTB CEIL FLOOR
	qbool			is_considered_deleted;

//
	int				temp_num_adjacent_diffs;
	int				temp_num_adjacent_sames;
	int				temp_mark_bits;
} brush_s;

//	patchDef2
//	{
//		liquids_lava/lava_blue_LX_1000_alpha_50
//		( 9 3 536870920 16 1000 )
//		(
//			(
//				( -126 -192 128 0 0 )
//				( -126 -192 384 0 -6 )
//				( -126 -192 640 0 -12 )
//			)
//			(MORE LIKE THESE)
//			(
//				( -126 -192 128 8 0 ) ( -126 -192 384 8 -6 ) ( -126 -192 640 8 -12 )
//			)
//		)
//	}
//}

typedef struct _brushlist_st {
	/// maxstrings changes as needed, causing reallocation of strings[] array
	int				maxsize;
	int				count;
	brush_s			*brush;		// PAGE
} brushlist_t;

typedef struct _entity_st { // p_ent
	stringlist_t	pairslist;
	brushlist_t		brushlist;

	int				quadrants_brushnums_plus1[quadrant_MAXCOUNT_6];		// LRTB CEIL FLOOR

	int				is_considered_deleted;
	vec3_t			bemins;	RELATED_ (entitylist_metric_world_brushes_with_walls) // will fill it in.
	vec3_t			bemaxs;	// will it do brush rows? Yes
	vec3_t			bedims; // It does not do entities.  That includes func_group.
} entityx_t;


typedef struct _entitylist_st {
	/// maxstrings changes as needed, causing reallocation of strings[] array
	int				maxsize;
	int				count;
	entityx_t		*entity;						// p_ent (PAGE)
	int				stage;							// stage_csg .. etc.
	int				func_t_stored_int;				// Dungeon callback for something.
	int				quadrants_world_brushnums_plus1[quadrant_MAXCOUNT_6];		// LRTB CEIL FLOOR
	int				func_lightrod_entnum;			// Must be non-zero

	// Baker: October 15 2025 - this should almost never be used.  It's dangerous.  Per loop variable before is best.
	int /*danger*/	original_ent_count; /*ALWAZSET*/// Avoid searching new entities.
													// Note: modelgen's insert at 1 breaks this.
													// Could reload the map after that.

	vec3_t			bbentsmins;	// entitylist_parsemap_metric must run
	vec3_t			bbentsmaxs;
	vec3_t			bbentsdims;
} entitylist_t;

#if 0 // FEB325
// Player rep including sort.
// Put player possessions into a list.


typedef struct _entity_st { // p_ent
	stringlist_t	listcolumns;
	brushlist_t		brushlist;
} inv_item_t;

typedef struct _inventorylist_t {
	/// maxstrings changes as needed, causing reallocation of strings[] array
	int				maxsize;
	int				count;
	entityx_t		*entity; // p_ent (PAGE)

	int				numcolumns;

	// Baker: OW ...
	func_t			SV_SendCallback;		// Turn the players possession into string
	func_t			CL_ReceiveCallback;		// Read that string
	func_t			ReadRowCallback;
	func_t			CL_CommandCallback;

} inventorylist_t;

#endif


void brushrowlistfreecontents (brushrowlist_t *list);
brushrow_s *brushrowlist_add (brushrowlist_t *list);

void brushlistfreecontents(brushlist_t *list);
brush_s *brushlist_add (brushlist_t *list);

void patchrowlistfreecontents (patchrowlist_t *list);
patchrow_s *patchrowlist_add (patchrowlist_t *list);

void entitylistfreecontents(entitylist_t *list);
entityx_t *entitylist_add (entitylist_t *list);
void entitylist_print_console (entitylist_t *plist); // Baker: Recursive printing brushes and patches to console.

// Baker: If either of these fail, use entitylistfreecontents to free the struct.
// A failed load, particularly a parse error could have a partially handled map.
qbool entitylist_parsemaptxt (entitylist_t *plist, ccs *txt);
qbool entitylist_parse_mapfile_is_ok (entitylist_t *plist, ccs *filename);
qbool entitylist_parsemap_metric (entitylist_t *plist, ccs *txt); // Runs metric

//void entitylist_metric_world_brushes_with_walls (entitylist_t *plist);

void entitylist_to_clipboard (entitylist_t *plist);

void entitylist_translate_epairs_origin (entitylist_t *list, vec3_t vadd);
void entitylist_translate_brushes (entitylist_t *list, vec3_t vadd);


int entity_index_translate_brushes (entitylist_t *list, int entnum, vec3_t vadd); // Because origin brushes

void entitylist_brush0_append (entitylist_t *list, entitylist_t *paste);
void entitylist_nonworld_append (entitylist_t *list, entitylist_t *paste);
void entitylist_epairs_only_concat (entitylist_t *plist, entitylist_t *paste); // Only epairs -- no brushes.

//void entitylist_add_pasting_this_ent (entitylist_t *plist, const entityx_t *r_ent_paste_src); // June 8 2025
entityx_t *entitylist_add_pasting_this_ent (entitylist_t *plist, const entityx_t *r_ent_paste); // June 8 2025
void entity_translate_with_metric (entityx_t *d_ent, vec3_t move_amount_add); // June 8 2025


void entity_add_key_value_unsafe_f (entityx_t *d_ent, ccs *key, ccs *fmt, ...) DP_FUNC_PRINTF(3);

int entitylist_find_entity_by_pointer (entitylist_t *plist, entityx_t *efind);

qbool entitylist_collide_down_ignoring_entnum (vec3_t collide, entitylist_t *plist, int start_idx, int beyond_idx, vec3_t org, int ignoreme);

void entitylist_key_set_replace_val (entitylist_t *plist, int entnum, ccs *key_force, ccs *val_force);

int entitylist_perform_atomize_entities_num_made (entitylist_t *plist, ccs *s0_plus_timestamp);
int entitylist_perform_clone_as_trigger_entities_num_made (entitylist_t *plist, ccs *s0_plus_timestamp);
int entitylist_perform_clone_classname_num_made (entitylist_t *plist, ccs *s0_plus_timestamp);

int entitylist_perform__decal__automatic (entitylist_t *plist_map);
int entitylist_perform_collision_body (entitylist_t *plist, ccs *s0_plus_timestamp);

void entitylist_nonworld_setthis (entitylist_t *plist, ccs *key_force, ccs *val_force);
void entitylist_map_include_nonworld_set (entitylist_t *plist, stringlist_t *plistpairset);

//void entitylist_prefix_epairs (entitylist_t *plist, ccs *prefix);
int entitylist_prefix_epairs_targetnames (entitylist_t *plist, ccs *prefix, stringlist_t *plist_prefixes);

int entitylist_epairs_find_model_gen_entitynum (entitylist_t *plist, ccs *prefix);
void entitylist_gen_models (entitylist_t *plist, int ex /*entnum*/);
int entitylist_perform_brushfacer_world_only (entitylist_t *plist, int *pnum_faces);
int entitylist_perform_concat_ents_clipboard (entitylist_t *pe_dest);
int entitylist_perform_concat_ents_string (entitylist_t *pe_dest, ccs *s);
int entitylist_perform_concat_ents_intlist32 (entitylist_t *pe_dest, int32list_s *ilist);

int entitylist_caulk_world_regular_textures (entitylist_t *plist);
void face_replace_texture (brushrow_s *pbr, ccs *sreplace);

int entitylist_perform_unbrushfacer_scratch1_world_only (entitylist_t *plist);


int entitylist_perform_floorage_world_only (entitylist_t *plist);
int entitylist_perform_cubeit (entitylist_t *plist);
int entitylist_perform_sky_encase_brushes_added (entitylist_t *plist, ccs *s0_plus_timestamp);
int entitylist_perform_originmake_num_made (entitylist_t *plist, ccs *svaluetowrite);

// UNLIKELY TO BE -CSG MORE LIKELy TO BE console csgtool

int entitylist_perform_adjacents_num_done (entitylist_t *plist);
int entitylist_perform_flatzerosettexture_num_done (entitylist_t *plist, float z);
int entitylist_perform_infodump (entitylist_t *plist);
int entitylist_perform_pincommonterrain_num_done (entitylist_t *plist);
int entitylist_perform_noflat_to_flat_num_done (entitylist_t *plist);
int entitylist_perform_weld_num_done (entitylist_t *plist);
int entitylist_perform_removenondominanttexturebrushes (entitylist_t *plist);


qbool entity_expand_bounds_did_brushes (entityx_t *r_ent, vec3_t brmins, vec3_t brmaxs);

// next is nearly a dup of entity_expand_bounds_did_brushes
qbool entity_update_bbox_from_brushes_was_processed (entityx_t *d_ent); // entity_metric_update

qbool brush_get_bbox_was_processed (brush_s *p_brush, vec3_t brmins, vec3_t brmaxs);

void entity_make_brush_bbox (entityx_t *p_ent, vec3_t brmins, vec3_t brmaxs);

qbool entity_key_get_vector_isok (entityx_t *r_ent, ccs *keyname_wanted, vec3_t dest);

//void entity_brush_add_with_bbox_and_texture (entityx_t *p_ent, vec3_t brmins, vec3_t brmaxs, ccs *texturename);
//void entity_make_brush_bbox_with_texture2 (entitylist_t *plist_map, entitylist_t *plist_model, entityx_t *p_ent, vec3_t brmins, vec3_t brmaxs, ccs *texturename);

ccs *entity_key_get_value (entityx_t *r_ent, ccs *keyname_wanted);

#define AFTER_WORLD_1	1
#define ENTITY_WORLD_0	0

void brmins_maxs_init (vec3_t brmins, vec3_t brmaxs);

// BakerString_Destroy_And_Null_It (&bs);
baker_string_t *entitylist_maptext_bsalloc (entitylist_t *plist);

baker_string_t *CSG_Process_BSAlloc (ccs *datasrc); // Returns a (baker_string_t *) or NULL if no data

// Returns NULL or value for key for the entity
ccs *entitykeys_find_value (entityx_t *e, ccs *keyname);



qbool entitylist_write_to_file (entitylist_t *plist, ccs *filename);
int entitylist_parse_mapfile_errorcode (entitylist_t *plist, ccs *filename);

//int entitylist_parsemaptxt_old_q1 (entitylist_t *plist, ccs *txt); // TODO: Or maybe not.

int entitylist_change_textures_q1 (entitylist_t *plist, ccs *s_sky_texture, ccs *s_textures_folder); // Returns a (baker_string_t *) or NULL if no data

// Returns entity number or -1
#define START_IDX_0 0
#define EntForNum(list,num) &list.entity[num]
int entity_key_set_value_is_ok (entityx_t *d_ent, ccs *keyname_wanted, ccs *value_to_set);
int entity_key_set_value_is_ok_force (entityx_t *d_ent, ccs *keyname_wanted, ccs *value_to_set);

void entity_add_alpha_brushes (entityx_t *d_ent, vec3_t worldmins, vec3_t worldmaxs, vec3_t worldsize);

qbool entity_key_set_vector_force (entityx_t *d_ent, ccs *keyname_wanted, const vec3_t v);
ccs *entity_key_get_value (entityx_t *r_ent, ccs *keyname_wanted);


int entitylist_add_entity (entitylist_t *plist);

//int entity_key_rename_is_ok (entityx_t *r_ent, ccs *keyname_wanted, ccs *value_to_set);
int entitylist_perform_classname_renames (entitylist_t *plist);
int entitylist_perform_layout_from_boxes (entitylist_t *plist_map); // _layout_from_boxes
int entitylist_perform_floor_everything_return_num_moved (entitylist_t *plist, ccs *s_floorvalue);
int entitylist_perform_heightmap_num_alpha_brushes_added (entitylist_t *plist, ccs *s0_plus_timestamp);

qbool entity_keypair_add_unsafe (entityx_t *d_ent, ccs *key, ccs *val);

void PathWork_CSG(int csg_argnum, char *mappath_fill, size_t mappath_fill_size);

/*ent idx*/ int entitylist_find_key_value (entitylist_t *plist, ccs *key, ccs *val); // entity_find entity find
///*ent idx*/ int entitylist_find_key_value_ex (entitylist_t *plist, ccs *key, ccs *val, int start_idx);
/*ent idx*/ int entitylist_find_key_value_ex (entitylist_t *plist, ccs *key_to_find, ccs *val_to_find, int start_idx, int beyond_idx);
/*ent idx*/ int entitylist_find_key_any_value (entitylist_t *plist, ccs *key_to_find, int start_idx, int beyond_idx, ccs **pVal);

qbool entity_has_originbrush(entityx_t *r_ent);
int entitylist_replace_texture (entitylist_t *plist, ccs *sfind, ccs *sreplace);

// entitylist_map_paste_at
void entitylist_append_map_with_translation (entitylist_t *dst_map, entitylist_t *src_map, vec3_t translate_move);
void entitylist_append_map (entitylist_t *dst_map, entitylist_t *src_map);

// bakerarray_s

typedef struct {
	int		batchsize;					// Defaults to BAKER_ARRAY_BATCHSIZE_DEFAULT_128

	int		numitems;
	int		maxitems;

	void	**pointers_array;			// union?
	size_t	datasize;
} bakerarray_s;

#define DEF_COMPAT_ARRAY_STRUCT(MYARRAYDATATYPE_S,MYPTRTYPE) \
	typedef struct _ ## MYARRAYDATATYPE_S {											\
		int					batchsize;		/* If not specified, defaults 128 */	\
		int					numitems;												\
		int					maxitems;												\
		MYPTRTYPE			**pointers_array;			/* */						\
		size_t				datasize;												\
	} MYARRAYDATATYPE_S // ENDER

// Baker: This is a struct declaration ... an array of object_s
// Example: DEF_COMPAT_ARRAY_STRUCT (oject_array_st, struct _oject_s);
// Creates an array of _oject_s and that array type is oject_array_st
// See::#define oject_array_s	struct _oject_array_st

//int idx = BakerArray_NewIndex(k->fctrl->frm.drawn_list_a);
//NPA__ Baker_Array_Erase (f->frm.drawn_list_a);

#define BAKER_ARRAY_BATCHSIZE_DEFAULT_128	128 // Fool

int bakerarray_ptr_erase(bakerarray_s *a); // f->frm.drawn_list_a uses this to clear reference pointers
int bakerarray_ptr_new_item_index(bakerarray_s *a);

#define BakerArray_Erase(ba) \
	bakerarray_ptr_erase ((bakerarray_s *) ba)

// See: int idx = BakerArray_NewIndex(f->frm.controls_a);
#define BakerArray_NewIndex(ba) \
	bakerarray_ptr_new_item_index((bakerarray_s *)ba);

#define BakerArray_Item_At(ray, n) (ray).pointers_array[n]

// Opposite of Owned is reference array where we do not own the contexts (like frm.drawn_list_a)
#define BakerArray_Owned_Clear(bastruct) \
	/* mazebbox_array_s - BakerArray clear */ \
	{ \
		for (int idx = 0; idx < bastruct.numitems; idx ++) { \
			/*mazebbox_s*/ void *boxo = bastruct.pointers_array[idx]; \
			Mem_FreeNull_ (boxo); \
		} \
		Mem_FreeNull_ (bastruct.pointers_array); \
		size_t clearamt = sizeof(bastruct); \
		memset (&bastruct, 0, clearamt); /* sizeof (bastruct) */ \
	} // Ender

#define BakerArray_New_Empty_Item(PSTRUCTTYPE_STAR,VARNAME,BASTRUCT) \
	PSTRUCTTYPE_STAR VARNAME; /*mazebbox_s *newbox*/ \
	{ \
		int newidx9 = BakerArray_NewIndex (&(BASTRUCT)); \
		size_t szwanted2 = sizeof(**BASTRUCT.pointers_array); \
		VARNAME = (PSTRUCTTYPE_STAR)Mem_Alloc (zonemempool, szwanted2); \
		BASTRUCT.pointers_array[newidx9] = VARNAME; \
	} // Ender


#include "equat.h"


void stringlistappend_face_vertex_unique (stringlist_t *plist, const brushrow_s *face);

#define arch2_space_split_isok(splitme,s0,s1) \
	_arch2_string_space_split_isok(splitme,s0,sizeof(s0),s1,sizeof(s1))

#define arch5_space_split_isok(splitme,s0,s1,s2,s3,s4) \
	_arch5_string_space_split_isok(splitme,s0,sizeof(s0),s1,sizeof(s1),s2,sizeof(s2),s3,sizeof(s3),s4,sizeof(s4))

qbool _arch2_string_space_split_isok (ccs *split_this, char *dst0, size_t dst0_sizeof, char *dst1, size_t dst1_sizeof);
qbool _arch5_string_space_split_isok (ccs *split_this, char *dst0, size_t dst0_sizeof, char *dst1, size_t dst1_sizeof, char *dst2, size_t dst2_sizeof, char *dst3, size_t dst3_sizeof, char *dst4, size_t dst4_sizeof);

void stringlist_condump_ilist (stringlist_t *plist, int32list_s *ilist);
void int32s_countstrings (int32list_s *ilist, stringlist_t *plist, stringlist_t *plist_indexor);
void stringlist_condump_autocount_uniques (stringlist_t *plist);
ccs *stringlist_find_most_common_index (stringlist_t *plist, ccs *ignoring_this);
void stringlist_append_entity_textures (stringlist_t *plist, entityx_t *r_ent);
RELATED_ (stringlist_append_functions_unique)


#endif // FILEMATCH_H
