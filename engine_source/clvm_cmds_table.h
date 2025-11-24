// clvm_cmds_table.h
//   #    FN			 #      prototype  ext
DEF(NULL,					, 0)		// #0 NULL function (not callable) (QUAKE)
DEF(VM_CL_makevectors,		, 1)		// #1 void(vector ang) makevectors (QUAKE)
DEF(VM_CL_setorigin,		, 2)		// #2 void(entity e, vector o) setorigin (QUAKE)
DEF(VM_CL_setmodel,			, 3)		// #3 void(entity e, string m) setmodel (QUAKE)
DEF(VM_CL_setsize,			, 4)		// #4 void(entity e, vector min, vector max) setsize (QUAKE)
DEF(NULL,					, 5)		// #5 void(entity e, vector min, vector max) setabssize (QUAKE)
DEF(VM_break,				, 6)		// #6 void() break (QUAKE)
DEF(VM_random,				, 7)		// #7 float() random (QUAKE)
DEF(VM_CL_sound,			, 8)		// #8 void(entity e, float chan, string samp, float volume, float atten[, float pitchchange[, float flags]]) sound (QUAKE)
DEF(VM_normalize,			, 9)		// #9 vector(vector v) normalize (QUAKE)
DEF(VM_error,				, 10 )		// #10 void(string e) error (QUAKE)
DEF(VM_objerror,			, 11 )		// #11 void(string e) objerror (QUAKE)
DEF(VM_vlen,				, 12 )		// #12 float(vector v) vlen (QUAKE)
DEF(VM_vectoyaw,			, 13 )		// #13 float(vector v) vectoyaw (QUAKE)
DEF(VM_CL_spawn,			, 14 )		// #14 entity() spawn (QUAKE)
DEF(VM_remove,				, 15 )		// #15 void(entity e) remove (QUAKE)
DEF(VM_CL_traceline,		, 16 )		// #16 void(vector v1, vector v2, float tryents, entity ignoreentity) traceline (QUAKE)
DEF(NULL,					, 17 )		// #17 entity() checkclient (QUAKE)
DEF(VM_find,				, 18 )		// #18 entity(entity start, .string fld, string match) find (QUAKE)
DEF(VM_precache_sound,		, 19 )		// #19 void(string s) precache_sound (QUAKE)
DEF(VM_CL_precache_model,	, 20 )		// #20 void(string s) precache_model (QUAKE)
DEF(NULL,					, 21 )		// #21 void(entity client, string s, ...) stuffcmd (QUAKE)
DEF(VM_CL_findradius,		, 22 )		// #22 entity(vector org, float rad) findradius (QUAKE)
DEF(NULL,					, 23 )		// #23 void(string s, ...) bprint (QUAKE)
DEF(NULL,					, 24 )		// #24 void(entity client, string s, ...) sprint (QUAKE)
DEF(VM_dprint,				, 25 )		// #25 void(string s, ...) dprint (QUAKE)
DEF(VM_ftos,				, 26 )		// #26 string(float f) ftos (QUAKE)
DEF(VM_vtos,				, 27 )		// #27 string(vector v) vtos (QUAKE)
DEF(VM_coredump,			, 28 )		// #28 void() coredump (QUAKE)
DEF(VM_traceon,				, 29 )		// #29 void() traceon (QUAKE)
DEF(VM_traceoff,			, 30 )		// #30 void() traceoff (QUAKE)
DEF(VM_eprint,				, 31 )		// #31 void(entity e) eprint (QUAKE)
DEF(VM_CL_walkmove,			, 32 )		// #32 float(float yaw, float dist[, float settrace]) walkmove (QUAKE)
DEF(NULL,					, 33 )		// #33 (QUAKE)
DEF(VM_CL_droptofloor,		, 34 )		// #34 float() droptofloor (QUAKE)
DEF(VM_CL_lightstyle,		, 35 )		// #35 void(float style, string value) lightstyle (QUAKE)
DEF(VM_rint,				, 36 )		// #36 float(float v) rint (QUAKE)
DEF(VM_floor,				, 37 )		// #37 float(float v) floor (QUAKE)
DEF(VM_ceil,				, 38 )		// #38 float(float v) ceil (QUAKE)
DEF(NULL,					, 39 )		// #39 (QUAKE)
DEF(VM_CL_checkbottom,		, 40 )		// #40 float(entity e) checkbottom (QUAKE)
DEF(VM_CL_pointcontents,	, 41 )		// #41 float(vector v) pointcontents (QUAKE)
DEF(NULL,					, 42 )		// #42 (QUAKE)
DEF(VM_fabs,				, 43 )		// #43 float(float f) fabs (QUAKE)
DEF(NULL,					, 44 )		// #44 vector(entity e, float speed) aim (QUAKE)
DEF(VM_cvar,				, 45 )		// #45 float(string s) cvar (QUAKE)
DEF(VM_localcmd_local,		, 46 )		// #46 void(string s) localcmd (QUAKE)
DEF(VM_nextent,				, 47 )		// #47 entity(entity e) nextent (QUAKE)
DEF(VM_CL_particle,			, 48 )		// #48 void(vector o, vector d, float color, float count) particle (QUAKE)
DEF(VM_changeyaw,			, 49 )		// #49 void() ChangeYaw (QUAKE)
DEF(NULL,					, 50 )		// #50 (QUAKE)
DEF(VM_vectoangles,			, 51 )		// #51 vector(vector v) vectoangles (QUAKE)
DEF(NULL,					, 52 )		// #52 void(float to, float f) WriteByte (QUAKE)
DEF(NULL,					, 53 )		// #53 void(float to, float f) WriteChar (QUAKE)
DEF(NULL,					, 54 )		// #54 void(float to, float f) WriteShort (QUAKE)
DEF(NULL,					, 55 )		// #55 void(float to, float f) WriteLong (QUAKE)
DEF(NULL,					, 56 )		// #56 void(float to, float f) WriteCoord (QUAKE)
DEF(NULL,					, 57 )		// #57 void(float to, float f) WriteAngle (QUAKE)
DEF(NULL,					, 58 )		// #58 void(float to, string s) WriteString (QUAKE)
DEF(NULL,					, 59 )		// #59 (QUAKE)
DEF(VM_sin,					, 60 )		// #60 float(float f) sin (DP_QC_SINCOSSQRTPOW)
DEF(VM_cos,					, 61 )		// #61 float(float f) cos (DP_QC_SINCOSSQRTPOW)
DEF(VM_sqrt,				, 62 )		// #62 float(float f) sqrt (DP_QC_SINCOSSQRTPOW)
DEF(VM_changepitch,			, 63 )		// #63 void(entity ent) changepitch (DP_QC_CHANGEPITCH)
DEF(VM_CL_tracetoss,		, 64 )		// #64 void(entity e, entity ignore) tracetoss (DP_QC_TRACETOSS)
DEF(VM_etos,				, 65 )		// #65 string(entity ent) etos (DP_QC_ETOS)
DEF(NULL,					, 66 )		// #66 (QUAKE)
DEF(NULL,					, 67 )		// #67 void(float step) movetogoal (QUAKE)
DEF(VM_precache_file,		, 68 )		// #68 string(string s) precache_file (QUAKE)
DEF(VM_CL_makestatic,		, 69 )		// #69 void(entity e) makestatic (QUAKE)
DEF(NULL,					, 70 )		// #70 void(string s) changelevel (QUAKE)
DEF(NULL,					, 71 )		// #71 (QUAKE)
DEF(VM_cvar_set,			, 72 )		// #72 void(string var, string val) cvar_set (QUAKE)
DEF(NULL,					, 73 )		// #73 void(entity client, strings) centerprint (QUAKE)
DEF(VM_CL_ambientsound,		, 74 )		// #74 void(vector pos, string samp, float vol, float atten) ambientsound (QUAKE)
DEF(VM_CL_precache_model,	, 75 )		// #75 string(string s) precache_model2 (QUAKE)
DEF(VM_precache_sound,		, 76 )		// #76 string(string s) precache_sound2 (QUAKE)
DEF(VM_precache_file,		, 77 )		// #77 string(string s) precache_file2 (QUAKE)
DEF(NULL,					, 78 )		// #78 void(entity e) setspawnparms (QUAKE)
DEF(NULL,					, 79 )		// #79 void(entity killer, entity killee) logfrag (QUAKEWORLD)
DEF(NULL,					, 80 )		// #80 string(entity e, string keyname) infokey (QUAKEWORLD)
DEF(VM_stof,				, 81 )		// #81 float(string s) stof (FRIK_FILE)
DEF(NULL,					, 82 )		// #82 void(vector where, float set) multicast (QUAKEWORLD)
DEF(NULL,					, 83 )		// #83 (QUAKE)
DEF(NULL,					, 84 )		// #84 (QUAKE)
DEF(NULL,					, 85 )		// #85 (QUAKE)
DEF(NULL,					, 86 )		// #86 (QUAKE)
DEF(NULL,					, 87 )		// #87 (QUAKE)
DEF(NULL,					, 88 )		// #88 (QUAKE)
DEF(NULL,					, 89 )		// #89 (QUAKE)
DEF(VM_CL_tracebox,			, 90 )		// #90 void(vector v1, vector min, vector max, vector v2, float nomonsters, entity forent) tracebox (DP_QC_TRACEBOX)
DEF(VM_randomvec,			, 91 )		// #91 vector() randomvec (DP_QC_RANDOMVEC)
DEF(VM_CL_getlight,			, 92 )		// #92 vector(vector org) getlight (DP_QC_GETLIGHT)
DEF(VM_registercvar,		, 93 )		// #93 float(string name, string value) registercvar (DP_REGISTERCVAR)
DEF(VM_min,					, 94 )		// #94 float(float a, floats) min (DP_QC_MINMAXBOUND)
DEF(VM_max,					, 95 )		// #95 float(float a, floats) max (DP_QC_MINMAXBOUND)
DEF(VM_bound,				, 96 )		// #96 float(float minimum, float val, float maximum) bound (DP_QC_MINMAXBOUND)
DEF(VM_pow,					, 97 )		// #97 float(float f, float f) pow (DP_QC_SINCOSSQRTPOW)
DEF(VM_findfloat,			, 98 )		// #98 entity(entity start, .float fld, float match) findfloat (DP_QC_FINDFLOAT)
DEF(VM_checkextension,		, 99 )		// #99 float(string s) checkextension (the basis of the extension system)

// FrikaC and Telejano range #100-#199
DEF(NULL,					, 100)		// #100
DEF(NULL,					, 101)		// #101
DEF(NULL,					, 102)		// #102
DEF(NULL,					, 103)		// #103
DEF(NULL,					, 104)		// #104
DEF(NULL,					, 105)		// #105
DEF(NULL,					, 106)		// #106
DEF(NULL,					, 107)		// #107
DEF(NULL,					, 108)		// #108
DEF(NULL,					, 109)		// #109
DEF(VM_fopen,				, 110)		// #110 float(string filename, float mode) fopen (FRIK_FILE)
DEF(VM_fclose,				, 111)		// #111 void(float fhandle) fclose (FRIK_FILE)
DEF(VM_fgets,				, 112)		// #112 string(float fhandle) fgets (FRIK_FILE)
DEF(VM_fputs,				, 113)		// #113 void(float fhandle, string s) fputs (FRIK_FILE)
DEF(VM_strlen,				, 114)		// #114 float(string s) strlen (FRIK_FILE)
DEF(VM_strcat,				, 115)		// #115 string(string s, string...) strcat (FRIK_FILE)
DEF(VM_substring,			, 116)		// #116 string(string s, float start, float length) substring (FRIK_FILE)
DEF(VM_stov,				, 117)		// #117 vector(string) stov (FRIK_FILE)
DEF(VM_strzone,				, 118)		// #118 string(string s) strzone (FRIK_FILE)
DEF(VM_strunzone,			, 119)		// #119 void(string s) strunzone (FRIK_FILE)
DEF(NULL,					, 120)		// #120
DEF(NULL,					, 121)		// #121
DEF(NULL,					, 122)		// #122
DEF(NULL,					, 123)		// #123
DEF(NULL,					, 124)		// #124
DEF(NULL,					, 125)		// #125
DEF(NULL,					, 126)		// #126
DEF(NULL,					, 127)		// #127
DEF(NULL,					, 128)		// #128
DEF(NULL,					, 129)		// #129
DEF(NULL,					, 130)		// #130
DEF(NULL,					, 131)		// #131
DEF(NULL,					, 132)		// #132
DEF(NULL,					, 133)		// #133
DEF(NULL,					, 134)		// #134
DEF(NULL,					, 135)		// #135
DEF(NULL,					, 136)		// #136
DEF(NULL,					, 137)		// #137
DEF(NULL,					, 138)		// #138
DEF(NULL,					, 139)		// #139
DEF(NULL,					, 140)		// #140
DEF(NULL,					, 141)		// #141
DEF(NULL,					, 142)		// #142
DEF(NULL,					, 143)		// #143
DEF(NULL,					, 144)		// #144
DEF(NULL,					, 145)		// #145
DEF(NULL,					, 146)		// #146
DEF(NULL,					, 147)		// #147
DEF(NULL,					, 148)		// #148
DEF(NULL,					, 149)		// #149 -- Baker: crossproduct goes here August 21 2025?  No can be written in QuakeC in mathlib.h

// Move, append, compile, add entity.

//float entnum = map_entity_add(h);
//map_entity_key_set_value (h, entnum, "classname", "shambler");
//map_entity_key_set_value (h, entnum, "origin", "300 300 300");

DEF(VM_map_load,			, 150)		// #150 float map_load (string filename) = #150; // index is 1, 0 = invalid.  Returns 0 on failure.
DEF(VM_map_move,			, 151)		// #151 float map_move (float hmap, vector move) = #151;
DEF(VM_map_append_map,		, 152)		// #152 float map_append_map (float hmapdest, float hmapsrc) = #152;
DEF(VM_map_save_as,			, 153)		// #153 float map_save_as (float hmap, string filename) = #153;
DEF(VM_map_clipboard_set,	, 154)		// #154 float map_clipboard_set (float hmap) = #154;

DEF(VM_map_compile_with_callback,, 155)	// #155 map_compile_set_callback (float hmap, string filename, function callback) = #155;
DEF(VM_map_prefix_fields_values,, 156)	// #156 float map_prefix_fields_values (float hmap, string fieldscomma, string prefix) = #156; // returns number changed. // ex: float numchanges = map_prefix_fields_values(hmap, "target,targetname", "block1_");
DEF(VM_map_close,			, 157)		// #157 float map_close (float hmap) = #157; // Return value is true.  Set handle to 0.
DEF(VM_map_entity_count,	, 158)		// #158 float map_entity_count (float hmap) = #158; // Return number of entities in the map.
DEF(VM_map_find_entity,		, 159)		// #159 /*entnum*/ float map_find_entity (float hmap, string key, string value) = #159; // Returns entity number, 0 is world, failure returns -1

DEF(VM_map_entity_key_count,, 160)		// #160 float map_entity_key_count (float hmap, float entnum) = #160; // Returns entity number, 0 is world, failure returns -1
DEF(VM_map_entity_key_for_index,, 161)	// #161 string map_entity_key_for_index (float hmap, float entnum, float keyindex) = #161; // Returns entity number, 0 is world, failure returns -1
DEF(VM_map_entity_key_get_value,, 162)	// #162 string map_entity_get_key_value (float hmap, float entnum, string key) = #162;
DEF(VM_map_entity_key_set_value,, 163)	// #163 float map_entity_set_key_value (float hmap, float entnum, string key, string value) = #163; // returns 1 if changed a value, 2 if added the key
DEF(VM_map_entity_get_mins_maxs,, 164)	// #164 float map_entity_get_mins_maxs (float hmap, float entnum, __inout vector mins, __inout vector maxs) = #164;
DEF(NULL,						, 165)	// #165 vector map_entity_get_maxs (float hmap, float entnum) = #165;
DEF(VM_map_entity_keyvalue_for_index,, 166) // #166 string map_entity_keyvalue_for_index (float hmap, float entnum, float keyindex) = #166; // Returns entity number, 0 is world, failure returns -1
DEF(NULL,					, 167)		// #167 vector map_entity_move (float hmap, float entnum, vector move) = #167;
DEF(NULL,					, 168)		// #168 vector map_entity_move (float hmap, float entnum) = #168;
DEF(VM_map_entity_add,		, 169)		// #169 float map_text_entity_add (float hmap, string float entnum) = #169; // Adds a key/value entity (brushless), returns index.

// Baker: Limits, fool!  Do you have them?  Let's not go full retard.
DEF(NULL,					, 170)		// #170 //VM_map_texture_find,
DEF(VM_map_texture_replace,	, 171)		// #171 //VM_map_texture_replace,

DEF(NULL,					, 172)		// #172
DEF(NULL,					, 173)		// #173
DEF(NULL,					, 174)		// #174
DEF(NULL,					, 175)		// #175
DEF(NULL,					, 176)		// #176
DEF(VM_localsound,			, 177)		// #177
DEF(NULL,					, 178)		// #178
DEF(NULL,					, 179)		// #179

RELATED_ (VM_map_load)

// ZIRCON_FORMS_V1
#ifdef CONFIG_MENU
	DEF(	VM_CL_Form_Load,		, 180)			// #180
	DEF(	VM_CL_Form_Property_Set_String,, 181)	// #181
	DEF(	VM_CL_Form_Property_Get_String,, 182)	// #182
	DEF(	VM_CL_Form_Close,		, 183)			// #183
	DEF(	VM_CL_Form_Command,		, 184)			// #184
	DEF(	VM_CL_Form_Property_Set_Float,, 185)	// #185
	DEF(	VM_CL_Form_Property_Get_Float,, 186)	// #186
#else
// Dedicated server build ...
	DEF(NULL,,180)	// 180
	DEF(NULL,,181)	// 181
	DEF(NULL,,182)	// 182
	DEF(NULL,,183)	// 183
	DEF(NULL,,184)	// 184
	DEF(NULL,,185)	// 185
	DEF(NULL,,186)	// 186
#endif

DEF(NULL,					, 187)		// #187 VM_CL_Form_Command_String? string returning
DEF(NULL,					, 188)		// #188
DEF(NULL,					, 189)		// #189
DEF(VM_FS_FileInfo,			, 190)		// #190
DEF(VM_FS_GetGameDir,		, 191)		// #191
DEF(VM_stobase64,			, 192)		// #192
DEF(VM_base64tos,			, 193)		// #193
DEF(NULL,					, 194)		// #194
DEF(VM_Inventory_Reload,	, 195)		// #195
DEF(VM_Inventory_GetString,	, 196)		// #196
DEF(VM_Inventory_GetFloat,	, 197)		// #197
DEF(VM_Inventory_GetVector,	, 198)		// #198
DEF(VM_CL_modelframecount,	, 199)		// #199 - FTEQW although for reasons unknown -- PF_Fixme

// FTEQW range #200-#299
DEF(NULL,					, 200)		// #200
DEF(NULL,					, 201)		// #201
DEF(NULL,					, 202)		// #202
DEF(NULL,					, 203)		// #203
DEF(NULL,					, 204)		// #204
DEF(NULL,					, 205)		// #205
DEF(NULL,					, 206)		// #206
DEF(NULL,					, 207)		// #207
DEF(NULL,					, 208)		// #208
DEF(NULL,					, 209)		// #209
DEF(NULL,					, 210)		// #210
DEF(NULL,					, 211)		// #211
DEF(NULL,					, 212)		// #212
DEF(NULL,					, 213)		// #213
DEF(NULL,					, 214)		// #214
DEF(NULL,					, 215)		// #215
DEF(NULL,					, 216)		// #216
DEF(NULL,					, 217)		// #217
DEF(VM_bitshift,			, 218)		// #218 float(float number, float quantity) bitshift (EXT_BITSHIFT)
DEF(NULL,					, 219)		// #219
DEF(NULL,					, 220)		// #220
DEF(VM_strstrofs,			, 221)		// #221 float(string str, string sub[, float startpos]) strstrofs (FTE_STRINGS)
DEF(VM_str2chr,				, 222)		// #222 float(string str, float ofs) str2chr (FTE_STRINGS)
DEF(VM_chr2str,				, 223)		// #223 string(float c, ...) chr2str (FTE_STRINGS)
DEF(VM_strconv,				, 224)		// #224 string(float ccase, float calpha, float cnum, string s, ...) strconv (FTE_STRINGS)
DEF(VM_strpad,				, 225)		// #225 string(float chars, string s, ...) strpad (FTE_STRINGS)
DEF(VM_infoadd,				, 226)		// #226 string(string info, string key, string value, ...) infoadd (FTE_STRINGS)
DEF(VM_infoget,				, 227)		// #227 string(string info, string key) infoget (FTE_STRINGS)
DEF(VM_strncmp,				, 228)		// #228 float(string s1, string s2, float len) strncmp (FTE_STRINGS)
DEF(VM_strncasecmp,			, 229)		// #229 float(string s1, string s2) strcasecmp (FTE_STRINGS)
DEF(VM_strncasecmp,			, 230)		// #230 float(string s1, string s2, float len) strncasecmp (FTE_STRINGS)
DEF(NULL,					, 231)		// #231
DEF(NULL,					, 232)		// #232 void(float index, float type, .void field) SV_AddStat (EXT_CSQC)
DEF(NULL,					, 233)		// #233
DEF(NULL,					, 234)		// #234
DEF(NULL,					, 235)		// #235
DEF(NULL,					, 236)		// #236
DEF(NULL,					, 237)		// #237
DEF(NULL,					, 238)		// #238
DEF(NULL,					, 239)		// #239
DEF(VM_CL_checkpvs,			, 240)		// #240
DEF(NULL,					, 241)		// #241
DEF(NULL,					, 242)		// #242
DEF(NULL,					, 243)		// #243
DEF(NULL,					, 244)		// #244
DEF(VM_modulo,				, 245)		// #245
DEF(NULL,					, 246)		// #246
DEF(NULL,					, 247)		// #247
DEF(NULL,					, 248)		// #248
DEF(NULL,					, 249)		// #249
DEF(NULL,					, 250)		// #250
DEF(NULL,					, 251)		// #251
DEF(NULL,					, 252)		// #252
DEF(NULL,					, 253)		// #253
DEF(NULL,					, 254)		// #254
DEF(NULL,					, 255)		// #255
DEF(NULL,					, 256)		// #256
DEF(NULL,					, 257)		// #257
DEF(NULL,					, 258)		// #258
DEF(NULL,					, 259)		// #259
DEF(NULL,					, 260)		// #260
DEF(NULL,					, 261)		// #261
DEF(NULL,					, 262)		// #262
DEF(VM_CL_skel_create,		, 263)		// #263 float(float modlindex) skel_create = #263; // (FTE_CSQC_SKELETONOBJECTS) create a skeleton (be sure to assign this value into .skeletonindex for use), returns skeleton index (1 or higher) on success, returns 0 on failure  (for example if the modelindex is not skeletal), it is recommended that you create a new skeleton if you change modelindex.
DEF(VM_CL_skel_build,		, 264)		// #264 float(float skel, entity ent, float modlindex, float retainfrac, float firstbone, float lastbone) skel_build = #264; // (FTE_CSQC_SKELETONOBJECTS) blend in a percentage of standard animation, 0 replaces entirely, 1 does nothing, 0.5 blends half, etc, and this only alters the bones in the specified range for which out of bounds values like 0,100000 are safe (uses .frame, .frame2, .frame3, .frame4, .lerpfrac, .lerpfrac3, .lerpfrac4, .frame1time, .frame2time, .frame3time, .frame4time), returns skel on success, 0 on failure
DEF(VM_CL_skel_get_numbones,, 265)		// #265 float(float skel) skel_get_numbones = #265; // (FTE_CSQC_SKELETONOBJECTS) returns how many bones exist in the created skeleton
DEF(VM_CL_skel_get_bonename,, 266)		// #266 string(float skel, float bonenum) skel_get_bonename = #266; // (FTE_CSQC_SKELETONOBJECTS) returns name of bone (as a tempstring)
DEF(VM_CL_skel_get_boneparent,, 267)	// #267 float(float skel, float bonenum) skel_get_boneparent = #267; // (FTE_CSQC_SKELETONOBJECTS) returns parent num for supplied bonenum, -1 if bonenum has no parent or bone does not exist (returned value is always less than bonenum, you can loop on this)
DEF(VM_CL_skel_find_bone,	, 268)		// #268 float(float skel, string tagname) skel_find_bone = #268; // (FTE_CSQC_SKELETONOBJECTS) get number of bone with specified name, 0 on failure, tagindex (bonenum+1) on success, same as using gettagindex on the modelindex
DEF(VM_CL_skel_get_bonerel,	, 269)		// #269 vector(float skel, float bonenum) skel_get_bonerel = #269; // (FTE_CSQC_SKELETONOBJECTS) get matrix of bone in skeleton relative to its parent - sets v_forward, v_right, v_up, returns origin (relative to parent bone)
DEF(VM_CL_skel_get_boneabs,	, 270)		// #270 vector(float skel, float bonenum) skel_get_boneabs = #270; // (FTE_CSQC_SKELETONOBJECTS) get matrix of bone in skeleton in model space - sets v_forward, v_right, v_up, returns origin (relative to entity)
DEF(VM_CL_skel_set_bone,	, 271)		// #271 void(float skel, float bonenum, vector org) skel_set_bone = #271; // (FTE_CSQC_SKELETONOBJECTS) set matrix of bone relative to its parent, reads v_forward, v_right, v_up, takes origin as parameter (relative to parent bone)
DEF(VM_CL_skel_mul_bone,	, 272)		// #272 void(float skel, float bonenum, vector org) skel_mul_bone = #272; // (FTE_CSQC_SKELETONOBJECTS) transform bone matrix (relative to its parent) by the supplied matrix in v_forward, v_right, v_up, takes origin as parameter (relative to parent bone)
DEF(VM_CL_skel_mul_bones,	, 273)		// #273 void(float skel, float startbone, float endbone, vector org) skel_mul_bones = #273; // (FTE_CSQC_SKELETONOBJECTS) transform bone matrices (relative to their parents) by the supplied matrix in v_forward, v_right, v_up, takes origin as parameter (relative to parent bones)
DEF(VM_CL_skel_copybones,	, 274)		// #274 void(float skeldst, float skelsrc, float startbone, float endbone) skel_copybones = #274; // (FTE_CSQC_SKELETONOBJECTS) copy bone matrices (relative to their parents) from one skeleton to another, useful for copying a skeleton to a corpse
DEF(VM_CL_skel_delete,		, 275)		// #275 void(float skel) skel_delete = #275; // (FTE_CSQC_SKELETONOBJECTS) deletes skeleton at the beginning of the next frame (you can add the entity, delete the skeleton, renderscene, and it will still work)
DEF(VM_CL_frameforname,		, 276)		// #276 float(float modlindex, string framename) frameforname = #276; // (FTE_CSQC_SKELETONOBJECTS) finds number of a specified frame in the animation, returns -1 if no match found
DEF(VM_CL_frameduration,	, 277)		// #277 float(float modlindex, float framenum) frameduration = #277; // (FTE_CSQC_SKELETONOBJECTS) returns the intended play time (in seconds) of the specified framegroup, if it does not exist the result is 0, if it is a single frame it may be a small value around 0.1 or 0.
DEF(NULL,					, 278)		// #278
DEF(NULL,					, 279)		// #279
DEF(NULL,					, 280)		// #280
DEF(NULL,					, 281)		// #281
DEF(NULL,					, 282)		// #282
DEF(NULL,					, 283)		// #283
DEF(NULL,					, 284)		// #284
DEF(NULL,					, 285)		// #285
DEF(NULL,					, 286)		// #286
DEF(NULL,					, 287)		// #287
DEF(NULL,					, 288)		// #288
DEF(NULL,					, 289)		// #289
DEF(NULL,					, 290)		// #290
DEF(NULL,					, 291)		// #291
DEF(NULL,					, 292)		// #292
DEF(NULL,					, 293)		// #293
DEF(NULL,					, 294)		// #294
DEF(NULL,					, 295)		// #295
DEF(NULL,					, 296)		// #296
DEF(NULL,					, 297)		// #297
DEF(NULL,					, 298)		// #298
DEF(NULL,					, 299)		// #299

// CSQC range #300-#399

DEF(VM_CL_R_ClearScene,		, 300)		// #300 void() clearscene (EXT_CSQC)
DEF(VM_CL_R_AddEntities,	, 301)		// #301 void(float mask) addentities (EXT_CSQC)
DEF(VM_CL_R_AddEntity,		, 302)		// #302 void(entity ent) addentity (EXT_CSQC)
DEF(VM_CL_R_SetView,		, 303)		// #303 float(float property, ...) setproperty (EXT_CSQC)
DEF(VM_CL_R_RenderScene,	, 304)		// #304 void() renderscene (EXT_CSQC)
DEF(VM_CL_R_AddDynamicLight,, 305)		// #305 void(vector org, float radius, vector lightcolours) adddynamiclight (EXT_CSQC)
DEF(VM_CL_R_PolygonBegin,	, 306)		// #306 void(string texturename, float flag, float is2d[NYI: , float lines]) R_BeginPolygon
DEF(VM_CL_R_PolygonVertex,	, 307)		// #307 void(vector org, vector texcoords, vector rgb, float alpha) R_PolygonVertex
DEF(VM_CL_R_PolygonEnd,		, 308)		// #308 void() R_EndPolygon
DEF(VM_CL_R_SetView,		, 309)		// #309 float(float property) getproperty (EXT_CSQC)
DEF(VM_CL_unproject,		, 310)		// #310 vector (vector v) cs_unproject (EXT_CSQC)
DEF(VM_CL_project,			, 311)		// #311 vector (vector v) cs_project (EXT_CSQC)
DEF(NULL,					, 312)		// #312
DEF(NULL,					, 313)		// #313
DEF(NULL,					, 314)		// #314
DEF(VM_drawline,			, 315)		// #315 void(float width, vector pos1, vector pos2, float flag) drawline (EXT_CSQC)
DEF(VM_iscachedpic,			, 316)		// #316 float(string name) iscachedpic (EXT_CSQC)
DEF(VM_precache_pic,		, 317)		// #317 string(string name, float trywad) precache_pic (EXT_CSQC)
DEF(VM_getimagesize,		, 318)		// #318 vector(string picname) draw_getimagesize (EXT_CSQC)
DEF(VM_freepic,				, 319)		// #319 void(string name) freepic (EXT_CSQC)
DEF(VM_drawcharacter,		, 320)		// #320 float(vector position, float character, vector scale, vector rgb, float alpha, float flag) drawcharacter (EXT_CSQC)
DEF(VM_drawstring,			, 321)		// #321 float(vector position, string text, vector scale, vector rgb, float alpha[, float flag]) drawstring (EXT_CSQC, DP_CSQC)
DEF(VM_drawpic,				, 322)		// #322 float(vector position, string pic, vector size, vector rgb, float alpha[, float flag]) drawpic (EXT_CSQC)
DEF(VM_drawfill,			, 323)		// #323 float(vector position, vector size, vector rgb, float alpha, float flag) drawfill (EXT_CSQC)
DEF(VM_drawsetcliparea,		, 324)		// #324 void(float x, float y, float width, float height) drawsetcliparea
DEF(VM_drawresetcliparea,	, 325)		// #325 void(void) drawresetcliparea
DEF(VM_drawcolorcodedstring,, 326)		// #326 float drawcolorcodedstring(vector position, string text, vector scale, vector rgb, float alpha, float flag) (EXT_CSQC)
DEF(VM_stringwidth,			, 327)		// #327 // FIXME is this okay? Baker: This is #468 in menu_qc
DEF(VM_drawsubpic,			, 328)		// #328 // FIXME is this okay? Baker: This is #469 in menu_qc
DEF(VM_drawrotpic,			, 329)		// #329 // FIXME is this okay? Baker: This is #470 in menu_qc
DEF(VM_CL_getstatf,			, 330)		// #330 float(float stnum) getstatf (EXT_CSQC)
DEF(VM_CL_getstati,			, 331)		// #331 float(float stnum) getstati (EXT_CSQC)
DEF(VM_CL_getstats,			, 332)		// #332 string(float firststnum) getstats (EXT_CSQC)
DEF(VM_CL_setmodelindex,	, 333)		// #333 void(entity e, float mdlindex) setmodelindex (EXT_CSQC)
DEF(VM_CL_modelnameforindex,, 334)		// #334 string(float mdlindex) modelnameforindex (EXT_CSQC)
DEF(VM_CL_particleeffectnum,, 335)		// #335 float(string effectname) particleeffectnum (EXT_CSQC)
DEF(VM_CL_trailparticles,	, 336)		// #336 void(entity ent, float effectnum, vector start, vector end) trailparticles (EXT_CSQC)
DEF(VM_CL_pointparticles,	, 337)		// #337 void(float effectnum, vector origin [, vector dir, float count]) pointparticles (EXT_CSQC)
DEF(VM_centerprint,			, 338)		// #338 void(string s, ...) centerprint (EXT_CSQC)
DEF(VM_print,				, 339)		// #339 void(string s, ...) print (EXT_CSQC, DP_SV_PRINT)
DEF(VM_keynumtostring,		, 340)		// #340 string(float keynum) keynumtostring (EXT_CSQC)
DEF(VM_stringtokeynum,		, 341)		// #341 float(string keyname) stringtokeynum (EXT_CSQC)
DEF(VM_getkeybind,			, 342)		// #342 string(float keynum[, float bindmap]) getkeybind (EXT_CSQC)
DEF(VM_CL_setcursormode,	, 343)		// #343 void(float usecursor) setcursormode (DP_CSQC)
DEF(VM_CL_getmousepos,		, 344)		// #344 vector() getmousepos (DP_CSQC)
DEF(VM_CL_getinputstate,	, 345)		// #345 float(float framenum) getinputstate (EXT_CSQC)
DEF(VM_CL_setsensitivityscale,, 346)		// #346 void(float sens) setsensitivityscale (EXT_CSQC)
DEF(VM_CL_runplayerphysics,	, 347)		// #347 void() runstandardplayerphysics (EXT_CSQC)
DEF(VM_CL_getplayerkey,		, 348)		// #348 string(float playernum, string keyname) getplayerkeyvalue (EXT_CSQC)
DEF(VM_CL_isdemo,			, 349)		// #349 float() isdemo (EXT_CSQC)
DEF(VM_isserver,			, 350)		// #350 float() isserver (EXT_CSQC)  (returns sv.active)
DEF(VM_CL_setlistener,		, 351)		// #351 void(vector origin, vector forward, vector right, vector up) SetListener (EXT_CSQC)
DEF(VM_CL_registercmd,		, 352)		// #352 void(string cmdname) registercommand (EXT_CSQC)
DEF(VM_wasfreed,			, 353)		// #353 float(entity ent) wasfreed (EXT_CSQC) (should be availabe on server too)
DEF(VM_CL_serverkey,		, 354)		// #354 string(string key) serverkey (EXT_CSQC)
DEF(VM_CL_videoplaying,		, 355)		// #355
DEF(VM_findfont,			, 356)		// #356 float(string fontname) loadfont (DP_GFX_FONTS)
DEF(VM_loadfont,			, 357)		// #357 float(string fontname, string fontmaps, string sizes, float slot) loadfont (DP_GFX_FONTS)
DEF(VM_CL_loadcubemap,		, 358)		// #358 void(string cubemapname) loadcubemap (DP_GFX_)
DEF(NULL,					, 359)		// #359
DEF(VM_CL_ReadByte,			, 360)		// #360 float() readbyte (EXT_CSQC)
DEF(VM_CL_ReadChar,			, 361)		// #361 float() readchar (EXT_CSQC)
DEF(VM_CL_ReadShort,		, 362)		// #362 float() readshort (EXT_CSQC)
DEF(VM_CL_ReadLong,			, 363)		// #363 float() readlong (EXT_CSQC)
DEF(VM_CL_ReadCoord,		, 364)		// #364 float() readcoord (EXT_CSQC)
DEF(VM_CL_ReadAngle,		, 365)		// #365 float() readangle (EXT_CSQC)
DEF(VM_CL_ReadString,		, 366)		// #366 string() readstring (EXT_CSQC)
DEF(VM_CL_ReadFloat,		, 367)		// #367 float() readfloat (EXT_CSQC)
DEF(NULL,					, 368)		// #368
DEF(NULL,					, 369)		// #369
DEF(NULL,					, 370)		// #370
DEF(NULL,					, 371)		// #371
DEF(NULL,					, 372)		// #372
DEF(NULL,					, 373)		// #373
DEF(NULL,					, 374)		// #374
DEF(NULL,					, 375)		// #375
DEF(NULL,					, 376)		// #376
DEF(NULL,					, 377)		// #377
DEF(NULL,					, 378)		// #378
DEF(NULL,					, 379)		// #379
DEF(NULL,					, 380)		// #380
DEF(NULL,					, 381)		// #381
DEF(NULL,					, 382)		// #382
DEF(NULL,					, 383)		// #383
DEF(NULL,					, 384)		// #384
DEF(NULL,					, 385)		// #385
DEF(NULL,					, 386)		// #386
DEF(NULL,					, 387)		// #387
DEF(NULL,					, 388)		// #388
DEF(NULL,					, 389)		// #389
DEF(NULL,					, 390)		// #390
DEF(NULL,					, 391)		// #391
DEF(NULL,					, 392)		// #392
DEF(NULL,					, 393)		// #393
DEF(NULL,					, 394)		// #394
DEF(NULL,					, 395)		// #395
DEF(NULL,					, 396)		// #396
DEF(NULL,					, 397)		// #397
DEF(NULL,					, 398)		// #398
DEF(NULL,					, 399)		// #399
// LadyHavoc's range #400-#499
DEF(VM_CL_copyentity,		, 400)		// #400 void(entity from, entity to) copyentity (DP_QC_COPYENTITY)
DEF(NULL,					, 401)		// #401 void(entity ent, float colors) setcolor (DP_QC_SETCOLOR)
DEF(VM_findchain,			, 402)		// #402 entity(.string fld, string match) findchain (DP_QC_FINDCHAIN)
DEF(VM_findchainfloat,		, 403)		// #403 entity(.float fld, float match) findchainfloat (DP_QC_FINDCHAINFLOAT)
DEF(VM_CL_effect,			, 404)		// #404 void(vector org, string modelname, float startframe, float endframe, float framerate) effect (DP_SV_EFFECT)
DEF(VM_CL_te_blood,			, 405)		// #405 void(vector org, vector velocity, float howmany) te_blood (DP_TE_BLOOD)
DEF(VM_CL_te_bloodshower,	, 406)		// #406 void(vector mincorner, vector maxcorner, float explosionspeed, float howmany) te_bloodshower (DP_TE_BLOODSHOWER)
DEF(VM_CL_te_explosionrgb,	, 407)		// #407 void(vector org, vector color) te_explosionrgb (DP_TE_EXPLOSIONRGB)
DEF(VM_CL_te_particlecube,	, 408)		// #408 void(vector mincorner, vector maxcorner, vector vel, float howmany, float color, float gravityflag, float randomveljitter) te_particlecube (DP_TE_PARTICLECUBE)
DEF(VM_CL_te_particlerain,	, 409)		// #409 void(vector mincorner, vector maxcorner, vector vel, float howmany, float color) te_particlerain (DP_TE_PARTICLERAIN)
DEF(VM_CL_te_particlesnow,	, 410)		// #410 void(vector mincorner, vector maxcorner, vector vel, float howmany, float color) te_particlesnow (DP_TE_PARTICLESNOW)
DEF(VM_CL_te_spark,			, 411)		// #411 void(vector org, vector vel, float howmany) te_spark (DP_TE_SPARK)
DEF(VM_CL_te_gunshotquad,	, 412)		// #412 void(vector org) te_gunshotquad (DP_QUADEFFECTS1)
DEF(VM_CL_te_spikequad,		, 413)		// #413 void(vector org) te_spikequad (DP_QUADEFFECTS1)
DEF(VM_CL_te_superspikequad,, 414)		// #414 void(vector org) te_superspikequad (DP_QUADEFFECTS1)
DEF(VM_CL_te_explosionquad,	, 415)		// #415 void(vector org) te_explosionquad (DP_QUADEFFECTS1)
DEF(VM_CL_te_smallflash,	, 416)		// #416 void(vector org) te_smallflash (DP_TE_SMALLFLASH)
DEF(VM_CL_te_customflash,	, 417)		// #417 void(vector org, float radius, float lifetime, vector color) te_customflash (DP_TE_CUSTOMFLASH)
DEF(VM_CL_te_gunshot,		, 418)		// #418 void(vector org) te_gunshot (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_spike,			, 419)		// #419 void(vector org) te_spike (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_superspike,	, 420)		// #420 void(vector org) te_superspike (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_explosion,		, 421)		// #421 void(vector org) te_explosion (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_tarexplosion,	, 422)		// #422 void(vector org) te_tarexplosion (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_wizspike,		, 423)		// #423 void(vector org) te_wizspike (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_knightspike,	, 424)		// #424 void(vector org) te_knightspike (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_lavasplash,	, 425)		// #425 void(vector org) te_lavasplash (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_teleport,		, 426)		// #426 void(vector org) te_teleport (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_explosion2,	, 427)		// #427 void(vector org, float colorstart, float colorlength) te_explosion2 (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_lightning1,	, 428)		// #428 void(entity own, vector start, vector end) te_lightning1 (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_lightning2,	, 429)		// #429 void(entity own, vector start, vector end) te_lightning2 (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_lightning3,	, 430)		// #430 void(entity own, vector start, vector end) te_lightning3 (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_CL_te_beam,			, 431)		// #431 void(entity own, vector start, vector end) te_beam (DP_TE_STANDARDEFFECTBUILTINS)
DEF(VM_vectorvectors,		, 432)		// #432 void(vector dir) vectorvectors (DP_QC_VECTORVECTORS)
DEF(VM_CL_te_plasmaburn,	, 433)		// #433 void(vector org) te_plasmaburn (DP_TE_PLASMABURN)
DEF(VM_getsurfacenumpoints,	, 434)		// #434 float(entity e, float s) getsurfacenumpoints (DP_QC_GETSURFACE)
DEF(VM_getsurfacepoint,		, 435)		// #435 vector(entity e, float s, float n) getsurfacepoint (DP_QC_GETSURFACE)
DEF(VM_getsurfacenormal,	, 436)		// #436 vector(entity e, float s) getsurfacenormal (DP_QC_GETSURFACE)
DEF(VM_getsurfacetexture,	, 437)		// #437 string(entity e, float s) getsurfacetexture (DP_QC_GETSURFACE)
DEF(VM_getsurfacenearpoint,	, 438)		// #438 float(entity e, vector p) getsurfacenearpoint (DP_QC_GETSURFACE)
DEF(VM_getsurfaceclippedpoint,, 439)		// #439 vector(entity e, float s, vector p) getsurfaceclippedpoint (DP_QC_GETSURFACE)
DEF(NULL,					, 440)		// #440 void(entity e, string s) clientcommand (KRIMZON_SV_PARSECLIENTCOMMAND)
DEF(VM_tokenize,			, 441)		// #441 float(string s) tokenize (KRIMZON_SV_PARSECLIENTCOMMAND)
DEF(VM_argv,				, 442)		// #442 string(float n) argv (KRIMZON_SV_PARSECLIENTCOMMAND)
DEF(VM_CL_setattachment,	, 443)		// #443 void(entity e, entity tagentity, string tagname) setattachment (DP_GFX_QUAKE3MODELTAGS)
DEF(VM_search_begin,		, 444)		// #444 float(string pattern, float caseinsensitive, float quiet) search_begin (DP_QC_FS_SEARCH)
DEF(VM_search_end,			, 445)		// #445 void(float handle) search_end (DP_QC_FS_SEARCH)
DEF(VM_search_getsize,		, 446)		// #446 float(float handle) search_getsize (DP_QC_FS_SEARCH)
DEF(VM_search_getfilename,	, 447)		// #447 string(float handle, float num) search_getfilename (DP_QC_FS_SEARCH)
DEF(VM_cvar_string,			, 448)		// #448 string(string s) cvar_string (DP_QC_CVAR_STRING)
DEF(VM_findflags,			, 449)		// #449 entity(entity start, .float fld, float match) findflags (DP_QC_FINDFLAGS)
DEF(VM_findchainflags,		, 450)		// #450 entity(.float fld, float match) findchainflags (DP_QC_FINDCHAINFLAGS)
DEF(VM_CL_gettagindex,		, 451)		// #451 float(entity ent, string tagname) gettagindex (DP_QC_GETTAGINFO)
DEF(VM_CL_gettaginfo,		, 452)		// #452 vector(entity ent, float tagindex) gettaginfo (DP_QC_GETTAGINFO)
DEF(NULL,					, 453)		// #453 void(entity clent) dropclient (DP_SV_DROPCLIENT)
DEF(NULL,					, 454)		// #454 entity() spawnclient (DP_SV_BOTCLIENT)
DEF(NULL,					, 455)		// #455 float(entity clent) clienttype (DP_SV_BOTCLIENT)
DEF(NULL,					, 456)		// #456 void(float to, string s) WriteUnterminatedString (DP_SV_WRITEUNTERMINATEDSTRING)
DEF(VM_CL_te_flamejet,		, 457)		// #457 void(vector org, vector vel, float howmany) te_flamejet (DP_TE_FLAMEJET)
DEF(NULL,					, 458)		// #458
DEF(VM_ftoe,				, 459)		// #459 entity(float num) entitybyindex (DP_QC_EDICT_NUM)
DEF(VM_buf_create,			, 460)		// #460 float() buf_create (DP_QC_STRINGBUFFERS)
DEF(VM_buf_del,				, 461)		// #461 void(float bufhandle) buf_del (DP_QC_STRINGBUFFERS)
DEF(VM_buf_getsize,			, 462)		// #462 float(float bufhandle) buf_getsize (DP_QC_STRINGBUFFERS)
DEF(VM_buf_copy,			, 463)		// #463 void(float bufhandle_from, float bufhandle_to) buf_copy (DP_QC_STRINGBUFFERS)
DEF(VM_buf_sort,			, 464)		// #464 void(float bufhandle, float sortpower, float backward) buf_sort (DP_QC_STRINGBUFFERS)
DEF(VM_buf_implode,			, 465)		// #465 string(float bufhandle, string glue) buf_implode (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_get,			, 466)		// #466 string(float bufhandle, float string_index) bufstr_get (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_set,			, 467)		// #467 void(float bufhandle, float string_index, string str) bufstr_set (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_add,			, 468)		// #468 float(float bufhandle, string str, float order) bufstr_add (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_free,			, 469)		// #469 void(float bufhandle, float string_index) bufstr_free (DP_QC_STRINGBUFFERS)
DEF(NULL,					, 470)		// #470 void(float index, float type, .void field) SV_AddStat (EXT_CSQC)
DEF(VM_asin,				, 471)		// #471 float(float s) VM_asin (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_acos,				, 472)		// #472 float(float c) VM_acos (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_atan,				, 473)		// #473 float(float t) VM_atan (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_atan2,				, 474)		// #474 float(float c, float s) VM_atan2 (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_tan,					, 475)		// #475 float(float a) VM_tan (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_strlennocol,			, 476)		// #476 float(string s) : DRESK - String Length (not counting color codes) (DP_QC_STRINGCOLORFUNCTIONS)
DEF(VM_strdecolorize,		, 477)		// #477 string(string s) : DRESK - Decolorized String (DP_QC_STRINGCOLORFUNCTIONS)
DEF(VM_strftime,			, 478)		// #478 string(float uselocaltime, string format, ...) (DP_QC_STRFTIME)
DEF(VM_tokenizebyseparator,	, 479)		// #479 float(string s) tokenizebyseparator (DP_QC_TOKENIZEBYSEPARATOR)
DEF(VM_strtolower,			, 480)		// #480 string(string s) VM_strtolower (DP_QC_STRING_CASE_FUNCTIONS)
DEF(VM_strtoupper,			, 481)		// #481 string(string s) VM_strtoupper (DP_QC_STRING_CASE_FUNCTIONS)
DEF(VM_cvar_defstring,		, 482)		// #482 string(string s) cvar_defstring (DP_QC_CVAR_DEFSTRING)
DEF(VM_CL_pointsound,		, 483)		// #483 void(vector origin, string sample, float volume, float attenuation) pointsound (DP_SV_POINTSOUND)
DEF(VM_strreplace,			, 484)		// #484 string(string search, string replace, string subject) strreplace (DP_QC_STRREPLACE)
DEF(VM_strireplace,			, 485)		// #485 string(string search, string replace, string subject) strireplace (DP_QC_STRREPLACE)
DEF(VM_getsurfacepointattribute,, 486)	// #486 vector(entity e, float s, float n, float a) getsurfacepointattribute
DEF(VM_gecko_create,		, 487)		// #487 float gecko_create( string name )
DEF(VM_gecko_destroy,		, 488)		// #488 void gecko_destroy( string name )
DEF(VM_gecko_navigate,		, 489)		// #489 void gecko_navigate( string name, string URI )
DEF(VM_gecko_keyevent,		, 490)		// #490 float gecko_keyevent( string name, float key, float eventtype )
DEF(VM_gecko_movemouse,		, 491)		// #491 void gecko_mousemove( string name, float x, float y )
DEF(VM_gecko_resize,		, 492)		// #492 void gecko_resize( string name, float w, float h )
DEF(VM_gecko_get_texture_extent,, 493)	// #493 vector gecko_get_texture_extent( string name )
DEF(VM_crc16,				, 494)		// #494 float(float caseinsensitive, string s, ...) crc16 = #494 (DP_QC_CRC16)
DEF(VM_cvar_type,			, 495)		// #495 float(string name) cvar_type = #495; (DP_QC_CVAR_TYPE)
DEF(VM_numentityfields,		, 496)		// #496 float() numentityfields = #496; (QP_QC_ENTITYDATA)
DEF(VM_entityfieldname,		, 497)		// #497 string(float fieldnum) entityfieldname = #497; (DP_QC_ENTITYDATA)
DEF(VM_entityfieldtype,		, 498)		// #498 float(float fieldnum) entityfieldtype = #498; (DP_QC_ENTITYDATA)
DEF(VM_getentityfieldstring,, 499)		// #499 string(float fieldnum, entity ent) getentityfieldstring = #499; (DP_QC_ENTITYDATA)
DEF(VM_putentityfieldstring,, 500)		// #500 float(float fieldnum, entity ent, string s) putentityfieldstring = #500; (DP_QC_ENTITYDATA)
DEF(VM_CL_ReadPicture,		, 501)		// #501 string() ReadPicture = #501;
DEF(VM_CL_boxparticles,		, 502)		// #502 void(float effectnum, entity own, vector origin_from, vector origin_to, vector dir_from, vector dir_to, float count) boxparticles (DP_CSQC_BOXPARTICLES)
DEF(VM_whichpack,			, 503)		// #503 string(string) whichpack = #503;
DEF(VM_CL_GetEntity,		, 504)		// #504 float(float entitynum, float fldnum) getentity = #504; vector(float entitynum, float fldnum) getentityvec = #504;
DEF(NULL,					, 505)		// #505
DEF(NULL,					, 506)		// #506
DEF(NULL,					, 507)		// #507
DEF(NULL,					, 508)		// #508
DEF(NULL,					, 509)		// #509
DEF(VM_uri_escape,			, 510)		// #510 string(string in) uri_escape = #510;
DEF(VM_uri_unescape,		, 511)		// #511 string(string in) uri_unescape = #511;
DEF(VM_etof,				, 512)		// #512 float(entity ent) num_for_edict = #512 (DP_QC_NUM_FOR_EDICT)
DEF(VM_uri_get,				, 513)		// #513 float(string uri, float id, [string post_contenttype, string post_delim, [float buf]]) uri_get = #513; (DP_QC_URI_GET, DP_QC_URI_POST)
DEF(VM_tokenize_console,	, 514)		// #514 float(string str) tokenize_console = #514; (DP_QC_TOKENIZE_CONSOLE)
DEF(VM_argv_start_index,	, 515)		// #515 float(float idx) argv_start_index = #515; (DP_QC_TOKENIZE_CONSOLE)
DEF(VM_argv_end_index,		, 516)		// #516 float(float idx) argv_end_index = #516; (DP_QC_TOKENIZE_CONSOLE)
DEF(VM_buf_cvarlist,		, 517)		// #517 void(float buf, string prefix, string antiprefix) buf_cvarlist = #517; (DP_QC_STRINGBUFFERS_CVARLIST)
DEF(VM_cvar_description,	, 518)		// #518 float(string name) cvar_description = #518; (DP_QC_CVAR_DESCRIPTION)
DEF(VM_gettime,				, 519)		// #519 float(float timer) gettime = #519; (DP_QC_GETTIME) // Baker: GETTIME_FRAMESTART or realtime or ...
DEF(VM_keynumtostring,		, 520)		// #520 string keynumtostring(float keynum)
DEF(VM_findkeysforcommand,	, 521)		// #521 string findkeysforcommand(string command[, float bindmap])
DEF(VM_CL_InitParticleSpawner,, 522)	// #522 void(float max_themes) initparticlespawner (DP_CSQC_SPAWNPARTICLE)
DEF(VM_CL_ResetParticle,	, 523)		// #523 void() resetparticle (DP_CSQC_SPAWNPARTICLE)
DEF(VM_CL_ParticleTheme,	, 524)		// #524 void(float theme) particletheme (DP_CSQC_SPAWNPARTICLE)
DEF(VM_CL_ParticleThemeSave,, 525)		// #525 void() particlethemesave, void(float theme) particlethemeupdate (DP_CSQC_SPAWNPARTICLE)
DEF(VM_CL_ParticleThemeFree,, 526)		// #526 void() particlethemefree (DP_CSQC_SPAWNPARTICLE)
DEF(VM_CL_SpawnParticle,	, 527)		// #527 float(vector org, vector vel, [float theme]) particle (DP_CSQC_SPAWNPARTICLE)
DEF(VM_CL_SpawnParticleDelayed,, 528)	// #528 float(vector org, vector vel, float delay, float collisiondelay, [float theme]) delayedparticle (DP_CSQC_SPAWNPARTICLE)
DEF(VM_loadfromdata,		, 529)		// #529
DEF(VM_loadfromfile,		, 530)		// #530
DEF(VM_CL_setpause,			, 531)		// #531 float(float ispaused) setpause = #531 (DP_CSQC_SETPAUSE)
DEF(VM_log,					, 532)		// #532 // Baker: logarithm
DEF(VM_getsoundtime,		, 533)		// #533 float(entity e, float channel) getsoundtime = #533; (DP_SND_GETSOUNDTIME)
DEF(VM_soundlength,			, 534)		// #534 float(string sample) soundlength = #534; (DP_SND_GETSOUNDTIME)
DEF(VM_buf_loadfile,        , 535)      // #535 float(string filename, float bufhandle) buf_loadfile (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(VM_buf_writefile,       , 536)      // #536 float(float filehandle, float bufhandle, float startpos, float numstrings) buf_writefile (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(VM_bufstr_find,         , 537)      // #537 float(float bufhandle, string match, float matchrule, float startpos) bufstr_find (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(VM_matchpattern,        , 538)      // #538 float(string s, string pattern, float matchrule) matchpattern (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(NULL,					, 539)		// #539
DEF(VM_physics_enable,		, 540)		// #540 void(entity e, float physics_enabled) physics_enable = #540; (DP_PHYSICS_ODE)
DEF(VM_physics_addforce,	, 541)		// #541 void(entity e, vector force, vector relative_ofs) physics_addforce = #541; (DP_PHYSICS_ODE)
DEF(VM_physics_addtorque,	, 542)		// #542 void(entity e, vector torque) physics_addtorque = #542; (DP_PHYSICS_ODE)
DEF(NULL,					, 543)		// #543
DEF(NULL,					, 544)		// #544
DEF(NULL,					, 545)		// #545
DEF(NULL,					, 546)		// #546
DEF(NULL,					, 547)		// #547
DEF(NULL,					, 548)		// #548
DEF(NULL,					, 549)		// #549
DEF(NULL,					, 550)		// #550
DEF(NULL,					, 551)		// #551
DEF(NULL,					, 552)		// #552
DEF(NULL,					, 553)		// #553
DEF(NULL,					, 554)		// #554
DEF(NULL,					, 555)		// #555
DEF(NULL,					, 556)		// #556
DEF(NULL,					, 557)		// #557
DEF(NULL,					, 558)		// #558
DEF(NULL,					, 559)		// #559
DEF(NULL,					, 560)		// #560
DEF(NULL,					, 561)		// #561
DEF(NULL,					, 562)		// #562
DEF(NULL,					, 563)		// #563
DEF(NULL,					, 564)		// #564
DEF(NULL,					, 565)		// #565
DEF(VM_CL_findbox,			, 566)		// #566 entity(vector mins, vector maxs) findbox = #566; (DP_QC_FINDBOX)

#if 111  // - M1
	DEF(	NULL,			, 567)		// #567
#else
	// Baker: DarkPlaces Beta broke physics in early 2023 and this was part of the broken physics
	// Zircon does not use the broken DarkPlaces Beta physics changes -- which I have fallen straight through a lift
	// as it was going up. If they ever use that engine for Xonotic, I imagine they'll end up fixing the bug.
	DEF(VM_nudgeoutofsolid,	, 567)		// #567 float(entity ent) nudgeoutofsolid = #567; (DP_QC_NUDGEOUTOFSOLID)
#endif

DEF(NULL,					, 568)		// #568
DEF(NULL,					, 569)		// #569
DEF(NULL,					, 570)		// #570
DEF(NULL,					, 571)		// #571
DEF(NULL,					, 572)		// #572
DEF(NULL,					, 573)		// #573
DEF(NULL,					, 574)		// #574
DEF(NULL,					, 575)		// #575
DEF(NULL,					, 576)		// #576
DEF(NULL,					, 577)		// #577
DEF(NULL,					, 578)		// #578
DEF(NULL,					, 579)		// #579
DEF(NULL,					, 580)		// #580
DEF(NULL,					, 581)		// #581
DEF(NULL,					, 582)		// #582
DEF(NULL,					, 583)		// #583
DEF(NULL,					, 584)		// #584
DEF(NULL,					, 585)		// #585
DEF(NULL,					, 586)		// #586
DEF(NULL,					, 587)		// #587
DEF(NULL,					, 588)		// #588
DEF(NULL,					, 589)		// #589
DEF(NULL,					, 590)		// #590
DEF(NULL,					, 591)		// #591
DEF(NULL,					, 592)		// #592
DEF(NULL,					, 593)		// #593
DEF(NULL,					, 594)		// #594
DEF(NULL,					, 595)		// #595
DEF(NULL,					, 596)		// #596
DEF(NULL,					, 597)		// #597
DEF(NULL,					, 598)		// #598
DEF(NULL,					, 599)		// #599
DEF(NULL,					, 600)		// #600
DEF(NULL,					, 601)		// #601
DEF(NULL,					, 602)		// #602
DEF(NULL,					, 603)		// #603
DEF(NULL,					, 604)		// #604
DEF(VM_callfunction,		, 605)		// #605
DEF(VM_writetofile,			, 606)		// #606
DEF(VM_isfunction,			, 607)		// #607
DEF(NULL,					, 608)		// #608
DEF(NULL,					, 609)		// #609
DEF(VM_findkeysforcommand,	, 610)		// #610 string findkeysforcommand(string command[, float bindmap])
DEF(NULL,					, 611)		// #611
DEF(NULL,					, 612)		// #612
DEF(VM_parseentitydata,		, 613)		// #613
DEF(NULL,					, 614)		// #614
DEF(NULL,					, 615)		// #615
DEF(NULL,					, 616)		// #616
DEF(NULL,					, 617)		// #617
DEF(NULL,					, 618)		// #618
DEF(NULL,					, 619)		// #619
DEF(NULL,					, 620)		// #620
DEF(NULL,					, 621)		// #621
DEF(NULL,					, 622)		// #622
DEF(NULL,					, 623)		// #623
DEF(VM_CL_getextresponse,	, 624)		// #624 string getextresponse(void) // Baker: Part of FTE_CSQC_SERVERBROWSER
DEF(NULL,					, 625)		// #625
DEF(NULL,					, 626)		// #626
DEF(VM_sprintf,             , 627)      // #627 string sprintf(string format, ...)
DEF(VM_getsurfacenumtriangles,, 628)	// #628 float(entity e, float s) getsurfacenumpoints (DP_QC_GETSURFACETRIANGLE)
DEF(VM_getsurfacetriangle,	, 629)		// #629 vector(entity e, float s, float n) getsurfacepoint (DP_QC_GETSURFACETRIANGLE)
DEF(VM_setkeybind,			, 630)		// #630 float(float key, string bind[, float bindmap]) setkeybind
DEF(VM_getbindmaps,			, 631)		// #631 vector(void) getbindmap
DEF(VM_setbindmaps,			, 632)		// #632 float(vector bm) setbindmap
DEF(NULL,					, 633)		// #633
DEF(NULL,					, 634)		// #634
DEF(NULL,					, 635)		// #635
DEF(NULL,					, 636)		// #636
DEF(NULL,					, 637)		// #637
DEF(VM_CL_RotateMoves,		, 638)		// #638
DEF(VM_digest_hex,			, 639)		// #639
DEF(VM_CL_V_CalcRefdef,		, 640)		// #640 void(entity e) V_CalcRefdef (DP_CSQC_V_CALCREFDEF)
DEF(NULL,					, 641)		// #641
DEF(VM_coverage,			, 642)		// #642
DEF(NULL,					, 643)
DEF(NULL,					, 644)
DEF(VM_steam_achivment_unlock,, 645)
DEF(VM_steam_achivment_lock, , 646)
DEF(VM_steam_open_url, , 647)
#ifdef KSTEAM
	NULL,						// #643
	NULL,						// #644
	NULL,						// #645
	NULL,						// #646
	NULL,						// #647
	NULL,						// #648
	NULL,						// #649
	// WRATH range (#650-#???)
	#if 0
		VM_fcopy,					// #650 float(string fnfrom, string fnto) fcopy (EXT_WRATH)
		VM_frename,					// #651 float (string fnold, string fnnew) frename (EXT_WRATH)
		VM_fremove,					// #652 float (string fname) fremove (EXT_WRATH)
		VM_fexists,					// #653 float (string fname) fexists (EXT_WRATH)
		VM_rmtree,					// #654 float (string path) rmtree (EXT_WRATH)
	#else
		NULL, // 650
		NULL, // 651
		NULL, // 652
		NULL, // 653
		NULL, // 654
	#endif
	NULL,						// #655
	NULL,						// #656
	NULL,						// #657
	NULL,						// #658
	NULL,						// #659
	NULL,						// #660
	NULL,						// #661
	NULL,						// #662
	NULL,						// #663
	NULL,						// #664
	NULL,						// #665
	NULL,						// #666
	NULL,						// #667
	NULL,						// #668
	NULL,						// #669
	NULL,						// #670
	NULL,						// #671
	NULL,						// #672
	NULL,						// #673
	NULL,						// #674
	NULL,						// #675
	NULL,						// #676
	NULL,						// #677
	NULL,						// #678
	NULL,						// #679
	NULL,						// #680
	NULL,						// #681
	NULL,						// #682
	NULL,						// #683
	NULL,						// #684
	NULL,						// #685
	NULL,						// #686
	NULL,						// #687
	NULL,						// #688
	NULL,						// #689
	NULL,						// #690
	NULL,						// #691
	NULL,						// #692
	NULL,						// #693
	NULL,						// #694
	NULL,						// #695
	NULL,						// #696
	NULL,						// #697
	NULL,						// #698
	NULL,						// #699
	NULL,						// #700
	NULL,						// #701
	NULL,						// #702
	NULL,						// #703
	NULL,						// #704
	NULL,						// #705
	NULL,						// #706
	NULL,						// #707
	NULL,						// #708
	NULL,						// #709
	NULL,						// #710
	NULL,						// #711
	NULL,						// #712
	NULL,						// #713
	NULL,						// #714
	NULL,						// #715
	NULL,						// #716
	NULL,						// #717
	NULL,						// #718
	NULL,						// #719
	NULL,						// #720
	NULL,						// #721
	NULL,						// #722
	NULL,						// #723
	NULL,						// #724
	NULL,						// #725
	NULL,						// #726
	NULL,						// #727
	NULL,						// #728
	NULL,						// #729
	VM_CL_stachievement_unlock,	// #730 void(string achievement_id) stachievement_unlock (EXT_STEAM_REKI)
	VM_CL_stachievement_query,	// #731 void(string achievement_id) stachievement_query (EXT_STEAM_REKI)
	VM_CL_ststat_setvalue,		// #732 void(string stat_id, float value) ststat_setvalue (EXT_STEAM_REKI)
	VM_CL_ststat_increment,		// #733 void(string stat_id, float value) ststat_increment (EXT_STEAM_REKI)
	VM_CL_ststat_query,			// #734 void(string stat_id) ststat_query (EXT_STEAM_REKI)
	VM_CL_stachievement_register,// #735 void(string achievement_id) stachievement_register (EXT_STEAM_REKI)
	VM_CL_ststat_register,		// #736 void(string stat_id) ststat_register (EXT_STEAM_REKI)
	NULL,	    				// #737
	NULL,						// #738
	NULL,						// #739
	VM_CL_controller_query,	    // #740 void(float index) controller_query (EXT_CONTROLLER_REKI)
	VM_CL_controller_rumble,	// #741 void(float index, float lowmult, float highmult, float msec) controller_rumble (EXT_CONTROLLER_REKI)
	VM_CL_controller_rumbletriggers,// #742 void(float index, float leftmult, float rightmult, float msec) controller_rumbletriggers (EXT_CONTROLLER_REKI)
	NULL,						// #743
	NULL,						// #744
	NULL,						// #745
	NULL,						// #746
	NULL,						// #747
	NULL,						// #748
	NULL,						// #749
	NULL

#endif // KSTEAM

#undef DEF
