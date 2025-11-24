// mvm_cmds_table.h


DEF(NULL,							 , 0		) //   #0 NULL function (not callable)
DEF(VM_checkextension,				 , 1		) //   #1
DEF(VM_error,						 , 2		) //   #2
DEF(VM_objerror,					 , 3		) //   #3
DEF(VM_print,						 , 4		) //   #4
DEF(VM_bprint,						 , 5		) //   #5
DEF(VM_sprint,						 , 6		) //   #6
DEF(VM_centerprint,					 , 7		) //   #7
DEF(VM_normalize,					 , 8		) //   #8
DEF(VM_vlen,						 , 9		) //   #9
DEF(VM_vectoyaw,					, 10		) //  #10
DEF(VM_vectoangles,					, 11		) //  #11
DEF(VM_random,						, 12		) //  #12
DEF(VM_localcmd_local,				, 13		) //  #13
DEF(VM_cvar,						, 14		) //  #14
DEF(VM_cvar_set,					, 15		) //  #15
DEF(VM_dprint,						, 16		) //  #16
DEF(VM_ftos,						, 17		) //  #17
DEF(VM_fabs,						, 18		) //  #18
DEF(VM_vtos,						, 19		) //  #19
DEF(VM_etos,						, 20		) //  #20
DEF(VM_stof,						, 21		) //  #21
DEF(VM_spawn,						, 22		) //  #22
DEF(VM_remove,						, 23		) //  #23
DEF(VM_find,						, 24		) //  #24
DEF(VM_findfloat,					, 25		) //  #25
DEF(VM_findchain,					, 26		) //  #26
DEF(VM_findchainfloat,				, 27		) //  #27
DEF(VM_precache_file,				, 28		) //  #28
DEF(VM_precache_sound,				, 29		) //  #29
DEF(VM_coredump,					, 30		) //  #30
DEF(VM_traceon,						, 31		) //  #31
DEF(VM_traceoff,					, 32		) //  #32
DEF(VM_eprint,						, 33		) //  #33
DEF(VM_rint,						, 34		) //  #34
DEF(VM_floor,						, 35		) //  #35
DEF(VM_ceil,						, 36		) //  #36
DEF(VM_nextent,						, 37		) //  #37
DEF(VM_sin,							, 38		) //  #38
DEF(VM_cos,							, 39		) //  #39
DEF(VM_sqrt,						, 40		) //  #40
DEF(VM_randomvec,					, 41		) //  #41
DEF(VM_registercvar,				, 42		) //  #42
DEF(VM_min,							, 43		) //  #43
DEF(VM_max,							, 44		) //  #44
DEF(VM_bound,						, 45		) //  #45
DEF(VM_pow,							, 46		) //  #46
DEF(VM_M_copyentity,				, 47		) //  #47
DEF(VM_fopen,						, 48		) //  #48
DEF(VM_fclose,						, 49		) //  #49
DEF(VM_fgets,						, 50		) //  #50
DEF(VM_fputs,						, 51		) //  #51
DEF(VM_strlen,						, 52		) //  #52
DEF(VM_strcat,						, 53		) //  #53
DEF(VM_substring,					, 54		) //  #54
DEF(VM_stov,						, 55		) //  #55
DEF(VM_strzone,						, 56		) //  #56
DEF(VM_strunzone,					, 57		) //  #57
DEF(VM_tokenize,					, 58		) //  #58
DEF(VM_argv,						, 59		) //  #59
DEF(VM_isserver,					, 60		) //  #60
DEF(VM_clientcount,					, 61		) //  #61
DEF(VM_clientstate,					, 62		) //  #62
DEF(NULL,							, 63		) //  #63 FIXME -- Baker: What does this comment mean?
DEF(VM_changelevel,					, 64		) //  #64
DEF(VM_localsound,					, 65		) //  #65
DEF(VM_M_getmousepos,				, 66		) //  #66
DEF(VM_gettime,						, 67		) //  #67
DEF(VM_loadfromdata,				, 68		) //  #68
DEF(VM_loadfromfile,				, 69		) //  #69
DEF(VM_modulo,						, 70		) //  #70
DEF(VM_cvar_string,					, 71		) //  #71
DEF(VM_crash,						, 72		) //  #72
DEF(VM_stackdump,					, 73		) //  #73
DEF(VM_search_begin,				, 74		) //  #74
DEF(VM_search_end,					, 75		) //  #75
DEF(VM_search_getsize,				, 76		) //  #76
DEF(VM_search_getfilename,			, 77		) //  #77
DEF(VM_chr,							, 78		) //  #78
DEF(VM_itof,						, 79		) //  #79 // Baker: float 	etof(entity ent) = #79;
DEF(VM_ftoe,						, 80		) //  #80
DEF(VM_itof,						, 81		) //  #81 isString // Baker: Looks like integer to float function
DEF(VM_altstr_count,				, 82		) //  #82
DEF(VM_altstr_prepare,				, 83		) //  #83
DEF(VM_altstr_get,					, 84		) //  #84
DEF(VM_altstr_set,					, 85		) //  #85
DEF(VM_altstr_ins,					, 86		) //  #86
DEF(VM_findflags,					, 87		) //  #87
DEF(VM_findchainflags,				, 88		) //  #88
DEF(VM_cvar_defstring,				, 89		) //  #89

// deactivate support for model rendering in the menu until someone has time to do it right [3/2/2008 Andreas]
#if 0
	DEF(VM_CL_setmodel,				, 90		) // #90 void(entity e, string m) setmodel (QUAKE)
	DEF(VM_CL_precache_model,		, 91		) // #91 void(string s) precache_model (QUAKE)
	DEF(VM_CL_setorigin,			, 92		) // #92 void(entity e, vector o) setorigin (QUAKE)
#else
	DEF(NULL,						, 90		) // #90
	DEF(NULL,						, 91		) // #91
	DEF(NULL,						, 92		) // #92
#endif

DEF(NULL,							, 93		) //  #93
DEF(NULL,							, 94		) //  #94
DEF(NULL,							, 95		) //  #95
DEF(NULL,							, 96		) //  #96
DEF(NULL,							, 97		) //  #97
DEF(NULL,							, 98		) //  #98
DEF(NULL,							, 99		) //  #99
DEF(NULL,							, 100		) // #100
DEF(NULL,							, 101		) // #101
DEF(NULL,							, 102		) // #102
DEF(NULL,							, 103		) // #103
DEF(NULL,							, 104		) // #104
DEF(NULL,							, 105		) // #105
DEF(NULL,							, 106		) // #106
DEF(NULL,							, 107		) // #107
DEF(NULL,							, 108		) // #108
DEF(NULL,							, 109		) // #109
DEF(NULL,							, 110		) // #110
DEF(NULL,							, 111		) // #111
DEF(NULL,							, 112		) // #112
DEF(NULL,							, 113		) // #113
DEF(NULL,							, 114		) // #114
DEF(NULL,							, 115		) // #115
DEF(NULL,							, 116		) // #116
DEF(NULL,							, 117		) // #117
DEF(NULL,							, 118		) // #118
DEF(NULL,							, 119		) // #119
DEF(NULL,							, 120		) // #120
DEF(NULL,							, 121		) // #121
DEF(NULL,							, 122		) // #122
DEF(NULL,							, 123		) // #123
DEF(NULL,							, 124		) // #124
DEF(NULL,							, 125		) // #125
DEF(NULL,							, 126		) // #126
DEF(NULL,							, 127		) // #127
DEF(NULL,							, 128		) // #128
DEF(NULL,							, 129		) // #129
DEF(NULL,							, 130		) // #130
DEF(NULL,							, 131		) // #131
DEF(NULL,							, 132		) // #132
DEF(NULL,							, 133		) // #133
DEF(NULL,							, 134		) // #134
DEF(NULL,							, 135		) // #135
DEF(NULL,							, 136		) // #136
DEF(NULL,							, 137		) // #137
DEF(NULL,							, 138		) // #138
DEF(NULL,							, 139		) // #139
DEF(NULL,							, 140		) // #140
DEF(NULL,							, 141		) // #141
DEF(NULL,							, 142		) // #142
DEF(NULL,							, 143		) // #143
DEF(NULL,							, 144		) // #144
DEF(NULL,							, 145		) // #145
DEF(NULL,							, 146		) // #146
DEF(NULL,							, 147		) // #147
DEF(NULL,							, 148		) // #148
DEF(NULL,							, 149		) // #149
DEF(NULL,							, 150		) // #150
DEF(NULL,							, 151		) // #151
DEF(NULL,							, 152		) // #152
DEF(NULL,							, 153		) // #153
DEF(NULL,							, 154		) // #154
DEF(NULL,							, 155		) // #155
DEF(NULL,							, 156		) // #156
DEF(NULL,							, 157		) // #157
DEF(NULL,							, 158		) // #158
DEF(NULL,							, 159		) // #159
DEF(NULL,							, 160		) // #160
DEF(NULL,							, 161		) // #161
DEF(NULL,							, 162		) // #162
DEF(NULL,							, 163		) // #163
DEF(NULL,							, 164		) // #164
DEF(NULL,							, 165		) // #165
DEF(NULL,							, 166		) // #166
DEF(NULL,							, 167		) // #167
DEF(NULL,							, 168		) // #168
DEF(NULL,							, 169		) // #169
DEF(NULL,							, 170		) // #170
DEF(NULL,							, 171		) // #171
DEF(NULL,							, 172		) // #172
DEF(NULL,							, 173		) // #173
DEF(NULL,							, 174		) // #174
DEF(NULL,							, 175		) // #175
DEF(NULL,							, 176		) // #176
DEF(NULL,							, 177		) // #177
DEF(NULL,							, 178		) // #178
DEF(NULL,							, 179		) // #179
DEF(NULL,							, 180		) // #180
DEF(NULL,							, 181		) // #181
DEF(NULL,							, 182		) // #182
DEF(NULL,							, 183		) // #183
DEF(NULL,							, 184		) // #184
DEF(NULL,							, 185		) // #185
DEF(NULL,							, 186		) // #186
DEF(NULL,							, 187		) // #187
DEF(NULL,							, 188		) // #188
DEF(NULL,							, 189		) // #189
DEF(VM_FS_FileInfo,					, 190		) // #190
DEF(VM_FS_GetGameDir,				, 191		) // #191
DEF(NULL,							, 192		) // #192
DEF(NULL,							, 193		) // #193
DEF(NULL,							, 194		) // #194
DEF(NULL,							, 195		) // #195
DEF(NULL,							, 196		) // #196
DEF(NULL,							, 197		) // #197
DEF(NULL,							, 198		) // #198
DEF(NULL,							, 199		) // #199
DEF(NULL,							, 200		) // #200
DEF(NULL,							, 201		) // #201
DEF(NULL,							, 202		) // #202
DEF(NULL,							, 203		) // #203
DEF(NULL,							, 204		) // #204
DEF(NULL,							, 205		) // #205
DEF(NULL,							, 206		) // #206
DEF(NULL,							, 207		) // #207
DEF(NULL,							, 208		) // #208
DEF(NULL,							, 209		) // #209
DEF(NULL,							, 210		) // #210
DEF(NULL,							, 211		) // #211
DEF(NULL,							, 212		) // #212
DEF(NULL,							, 213		) // #213
DEF(NULL,							, 214		) // #214
DEF(NULL,							, 215		) // #215
DEF(NULL,							, 216		) // #216
DEF(NULL,							, 217		) // #217
DEF(NULL,							, 218		) // #218
DEF(NULL,							, 219		) // #219
DEF(NULL,							, 220		) // #220
DEF(VM_strstrofs,					, 221		) // #221 float(string str, string sub[, float startpos]) strstrofs (FTE_STRINGS)
DEF(VM_str2chr,						, 222		) // #222 float(string str, float ofs) str2chr (FTE_STRINGS)
DEF(VM_chr2str,						, 223		) // #223 string(float c, ...) chr2str (FTE_STRINGS)
DEF(VM_strconv,						, 224		) // #224 string(float ccase, float calpha, float cnum, string s, ...) strconv (FTE_STRINGS)
DEF(VM_strpad,						, 225		) // #225 string(float chars, string s, ...) strpad (FTE_STRINGS)
DEF(VM_infoadd,						, 226		) // #226 string(string info, string key, string value, ...) infoadd (FTE_STRINGS)
DEF(VM_infoget,						, 227		) // #227 string(string info, string key) infoget (FTE_STRINGS)
DEF(VM_strncmp,						, 228		) // #228 float(string s1, string s2, float len) strncmp (FTE_STRINGS)
DEF(VM_strncasecmp,					, 229		) // #229 float(string s1, string s2) strcasecmp (FTE_STRINGS)
DEF(VM_strncasecmp,					, 230		) // #230 float(string s1, string s2, float len) strncasecmp (FTE_STRINGS)
DEF(NULL,							, 231		) // #231
DEF(NULL,							, 232		) // #232
DEF(NULL,							, 233		) // #233
DEF(NULL,							, 234		) // #234
DEF(NULL,							, 235		) // #235
DEF(NULL,							, 236		) // #236
DEF(NULL,							, 237		) // #237
DEF(NULL,							, 238		) // #238
DEF(NULL,							, 239		) // #239
DEF(NULL,							, 240		) // #240
DEF(NULL,							, 241		) // #241
DEF(NULL,							, 242		) // #242
DEF(NULL,							, 243		) // #243
DEF(NULL,							, 244		) // #244
DEF(NULL,							, 245		) // #245
DEF(NULL,							, 246		) // #246
DEF(NULL,							, 247		) // #247
DEF(NULL,							, 248		) // #248
DEF(NULL,							, 249		) // #249
DEF(NULL,							, 250		) // #250
DEF(NULL,							, 251		) // #251
DEF(NULL,							, 252		) // #252
DEF(NULL,							, 253		) // #253
DEF(NULL,							, 254		) // #254
DEF(NULL,							, 255		) // #255
DEF(NULL,							, 256		) // #256
DEF(NULL,							, 257		) // #257
DEF(NULL,							, 258		) // #258
DEF(NULL,							, 259		) // #259
DEF(NULL,							, 260		) // #260
DEF(NULL,							, 261		) // #261
DEF(NULL,							, 262		) // #262
DEF(NULL,							, 263		) // #263
DEF(NULL,							, 264		) // #264
DEF(NULL,							, 265		) // #265
DEF(NULL,							, 266		) // #266
DEF(NULL,							, 267		) // #267
DEF(NULL,							, 268		) // #268
DEF(NULL,							, 269		) // #269
DEF(NULL,							, 270		) // #270
DEF(NULL,							, 271		) // #271
DEF(NULL,							, 272		) // #272
DEF(NULL,							, 273		) // #273
DEF(NULL,							, 274		) // #274
DEF(NULL,							, 275		) // #275
DEF(NULL,							, 276		) // #276
DEF(NULL,							, 277		) // #277
DEF(NULL,							, 278		) // #278
DEF(NULL,							, 279		) // #279
DEF(NULL,							, 280		) // #280
DEF(NULL,							, 281		) // #281
DEF(NULL,							, 282		) // #282
DEF(NULL,							, 283		) // #283
DEF(NULL,							, 284		) // #284
DEF(NULL,							, 285		) // #285
DEF(NULL,							, 286		) // #286
DEF(NULL,							, 287		) // #287
DEF(NULL,							, 288		) // #288
DEF(NULL,							, 289		) // #289
DEF(NULL,							, 290		) // #290
DEF(NULL,							, 291		) // #291
DEF(NULL,							, 292		) // #292
DEF(NULL,							, 293		) // #293
DEF(NULL,							, 294		) // #294
DEF(NULL,							, 295		) // #295
DEF(NULL,							, 296		) // #296
DEF(NULL,							, 297		) // #297
DEF(NULL,							, 298		) // #298
DEF(NULL,							, 299		) // #299
// deactivate support for model rendering in the menu until someone has time to do it right [3/2/2008 Andreas]

// CSQC range #300-#399
#if 0
	DEF(VM_CL_R_ClearScene,						) // #300 void() clearscene (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_AddEntities,					) // #301 void(float mask) addentities (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_AddEntity,						) // #302 void(entity ent) addentity (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_SetView,						) // #303 float(float property, ...) setproperty (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_RenderScene,					) // #304 void() renderscene (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_AddDynamicLight,				) // #305 void(vector org, float radius, vector lightcolours) adddynamiclight (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_PolygonBegin,					) // #306 void(string texturename, float flag[, float is2d, float lines]) R_BeginPolygon (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_PolygonVertex,					) // #307 void(vector org, vector texcoords, vector rgb, float alpha) R_PolygonVertex (DP_QC_RENDER_SCENE)
	DEF(VM_CL_R_PolygonEnd,						) // #308 void() R_EndPolygon
	DEF(NULL/*VM_CL_R_LoadWorldModel*/,			) // #309 void(string modelname) R_LoadWorldModel

	// TODO: rearrange and merge all builtin lists and share as many extensions as possible between all VM instances [1/27/2008 Andreas]

	DEF(VM_CL_setattachment,		, 310		) // #310 void(entity e, entity tagentity, string tagname) setattachment (DP_GFX_QUAKE3MODELTAGS) (DP_QC_RENDER_SCENE)
	DEF(VM_CL_gettagindex,			, 311		) // #311 float(entity ent, string tagname) gettagindex (DP_QC_GETTAGINFO) (DP_QC_RENDER_SCENE)
	DEF(VM_CL_gettaginfo,			, 312		) // #312 vector(entity ent, float tagindex) gettaginfo (DP_QC_GETTAGINFO) (DP_QC_RENDER_SCENE)
#else
	// CSQC range #300-#399
	DEF(NULL,						, 300		) // #300
	DEF(NULL,						, 301		) // #301
	DEF(NULL,						, 302		) // #302
	DEF(NULL,						, 303		) // #303
	DEF(NULL,						, 304		) // #304
	DEF(NULL,						, 305		) // #305
	DEF(NULL,						, 306		) // #306
	DEF(NULL,						, 307		) // #307
	DEF(NULL,						, 308		) // #308
	DEF(NULL,						, 309		) // #309
	DEF(NULL,						, 310		) // #310
	DEF(NULL,						, 311		) // #311
	DEF(NULL,						, 312		) // #312
#endif

DEF(NULL,							, 313		) // #313
DEF(NULL,							, 314		) // #314
DEF(NULL,							, 315		) // #315
DEF(NULL,							, 316		) // #316
DEF(NULL,							, 317		) // #317
DEF(NULL,							, 318		) // #318
DEF(NULL,							, 319		) // #319
DEF(NULL,							, 320		) // #320
DEF(NULL,							, 321		) // #321
DEF(NULL,							, 322		) // #322
DEF(NULL,							, 323		) // #323
DEF(NULL,							, 324		) // #324
DEF(NULL,							, 325		) // #325
DEF(NULL,							, 326		) // #326
DEF(NULL,							, 327		) // #327
DEF(NULL,							, 328		) // #328
DEF(NULL,							, 329		) // #329
DEF(NULL,							, 330		) // #330
DEF(NULL,							, 331		) // #331
DEF(NULL,							, 332		) // #332
DEF(NULL,							, 333		) // #333
DEF(NULL,							, 334		) // #334
DEF(NULL,							, 335		) // #335
DEF(NULL,							, 336		) // #336
DEF(NULL,							, 337		) // #337
DEF(NULL,							, 338		) // #338
DEF(NULL,							, 339		) // #339
DEF(VM_keynumtostring,				, 340		) // #340 string keynumtostring(float keynum)
DEF(VM_stringtokeynum,				, 341		) // #341 float stringtokeynum(string key)
DEF(VM_getkeybind,					, 342		) // #342 string(float keynum[, float bindmap]) getkeybind (EXT_CSQC)
DEF(NULL,							, 343		) // #343
DEF(NULL,							, 344		) // #344
DEF(NULL,							, 345		) // #345
DEF(NULL,							, 346		) // #346
DEF(NULL,							, 347		) // #347
DEF(NULL,							, 348		) // #348
DEF(VM_CL_isdemo,					, 349		) // #349
DEF(NULL,							, 350		) // #350
DEF(NULL,							, 351		) // #351
DEF(VM_M_registercommand,			, 352		) // #352 void(string cmdname)
DEF(VM_wasfreed,					, 353		) // #353 float(entity ent) wasfreed
DEF(NULL,							, 354		) // #354
DEF(VM_CL_videoplaying,				, 355		) // #355
DEF(VM_findfont,					, 356		) // #356 float(string fontname) loadfont (DP_GFX_FONTS)
DEF(VM_loadfont,					, 357		) // #357 float(string fontname, string fontmaps, string sizes, float slot) loadfont (DP_GFX_FONTS)
DEF(NULL,							, 358		) // #358
DEF(NULL,							, 359		) // #359
DEF(NULL,							, 360		) // #360
DEF(NULL,							, 361		) // #361
DEF(NULL,							, 362		) // #362
DEF(NULL,							, 363		) // #363
DEF(NULL,							, 364		) // #364
DEF(NULL,							, 365		) // #365
DEF(NULL,							, 366		) // #366
DEF(NULL,							, 367		) // #367
DEF(NULL,							, 368		) // #368
DEF(NULL,							, 369		) // #369
DEF(NULL,							, 370		) // #370
DEF(NULL,							, 371		) // #371
DEF(NULL,							, 372		) // #372
DEF(NULL,							, 373		) // #373
DEF(NULL,							, 374		) // #374
DEF(NULL,							, 375		) // #375
DEF(NULL,							, 376		) // #376
DEF(NULL,							, 377		) // #377
DEF(NULL,							, 378		) // #378
DEF(NULL,							, 379		) // #379
DEF(NULL,							, 380		) // #380
DEF(NULL,							, 381		) // #381
DEF(NULL,							, 382		) // #382
DEF(NULL,							, 383		) // #383
DEF(NULL,							, 384		) // #384
DEF(NULL,							, 385		) // #385
DEF(NULL,							, 386		) // #386
DEF(NULL,							, 387		) // #387
DEF(NULL,							, 388		) // #388
DEF(NULL,							, 389		) // #389
DEF(NULL,							, 390		) // #390
DEF(NULL,							, 391		) // #391
DEF(NULL,							, 392		) // #392
DEF(NULL,							, 393		) // #393
DEF(NULL,							, 394		) // #394
DEF(NULL,							, 395		) // #395
DEF(NULL,							, 396		) // #396
DEF(NULL,							, 397		) // #397
DEF(NULL,							, 398		) // #398
DEF(NULL,							, 399		) // #399
DEF(NULL,							, 400		) // #400
DEF(VM_M_WriteByte,					, 401		) // #401
DEF(VM_M_WriteChar,					, 402		) // #402
DEF(VM_M_WriteShort,				, 403		) // #403
DEF(VM_M_WriteLong,					, 404		) // #404
DEF(VM_M_WriteAngle,				, 405		) // #405
DEF(VM_M_WriteCoord,				, 406		) // #406
DEF(VM_M_WriteString,				, 407		) // #407
DEF(VM_M_WriteEntity,				, 408		) // #408
DEF(NULL,							, 409		) // #409
DEF(NULL,							, 410		) // #410
DEF(NULL,							, 411		) // #411
DEF(NULL,							, 412		) // #412
DEF(NULL,							, 413		) // #413
DEF(NULL,							, 414		) // #414
DEF(NULL,							, 415		) // #415
DEF(NULL,							, 416		) // #416
DEF(NULL,							, 417		) // #417
DEF(NULL,							, 418		) // #418
DEF(NULL,							, 419		) // #419
DEF(NULL,							, 420		) // #420
DEF(NULL,							, 421		) // #421
DEF(NULL,							, 422		) // #422
DEF(NULL,							, 423		) // #423
DEF(NULL,							, 424		) // #424
DEF(NULL,							, 425		) // #425
DEF(NULL,							, 426		) // #426
DEF(NULL,							, 427		) // #427
DEF(NULL,							, 428		) // #428
DEF(NULL,							, 429		) // #429
DEF(NULL,							, 430		) // #430
DEF(NULL,							, 431		) // #431
DEF(NULL,							, 432		) // #432
DEF(NULL,							, 433		) // #433
DEF(NULL,							, 434		) // #434
DEF(NULL,							, 435		) // #435
DEF(NULL,							, 436		) // #436
DEF(NULL,							, 437		) // #437
DEF(NULL,							, 438		) // #438
DEF(NULL,							, 439		) // #439
DEF(VM_buf_create,					, 440		) // #440 float() buf_create (DP_QC_STRINGBUFFERS)
DEF(VM_buf_del,						, 441		) // #441 void(float bufhandle) buf_del (DP_QC_STRINGBUFFERS)
DEF(VM_buf_getsize,					, 442		) // #442 float(float bufhandle) buf_getsize (DP_QC_STRINGBUFFERS)
DEF(VM_buf_copy,					, 443		) // #443 void(float bufhandle_from, float bufhandle_to) buf_copy (DP_QC_STRINGBUFFERS)
DEF(VM_buf_sort,					, 444		) // #444 void(float bufhandle, float sortpower, float backward) buf_sort (DP_QC_STRINGBUFFERS)
DEF(VM_buf_implode,					, 445		) // #445 string(float bufhandle, string glue) buf_implode (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_get,					, 446		) // #446 string(float bufhandle, float string_index) bufstr_get (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_set,					, 447		) // #447 void(float bufhandle, float string_index, string str) bufstr_set (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_add,					, 448		) // #448 float(float bufhandle, string str, float order) bufstr_add (DP_QC_STRINGBUFFERS)
DEF(VM_bufstr_free,					, 449		) // #449 void(float bufhandle, float string_index) bufstr_free (DP_QC_STRINGBUFFERS)
DEF(NULL,							, 450		) // #450
DEF(VM_iscachedpic,					, 451		) // #451 draw functions...
DEF(VM_precache_pic,				, 452		) // #452
DEF(VM_freepic,						, 453		) // #453
DEF(VM_drawcharacter,				, 454		) // #454
DEF(VM_drawstring,					, 455		) // #455
DEF(VM_drawpic,						, 456		) // #456
DEF(VM_drawfill,					, 457		) // #457
DEF(VM_drawsetcliparea,				, 458		) // #458
DEF(VM_drawresetcliparea,			, 459		) // #459
DEF(VM_getimagesize,				, 460		) // #460
DEF(VM_cin_open,					, 461		) // #461
DEF(VM_cin_close,					, 462		) // #462
DEF(VM_cin_setstate,				, 463		) // #463
DEF(VM_cin_getstate,				, 464		) // #464
DEF(VM_cin_restart, 				, 465		) // #465
DEF(VM_drawline,					, 466		) // #466
DEF(VM_drawcolorcodedstring,		, 467		) // #467
DEF(VM_stringwidth,					, 468		) // #468
DEF(VM_drawsubpic,					, 469		) // #469
DEF(VM_drawrotpic,					, 470		) // #470
DEF(VM_asin,						, 471		) // #471 float(float s) VM_asin (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_acos,						, 472		) // #472 float(float c) VM_acos (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_atan,						, 473		) // #473 float(float t) VM_atan (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_atan2,						, 474		) // #474 float(float c, float s) VM_atan2 (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_tan,							, 475		) // #475 float(float a) VM_tan (DP_QC_ASINACOSATANATAN2TAN)
DEF(VM_strlennocol,					, 476		) // #476 float(string s) : DRESK - String Length (not counting color codes) (DP_QC_STRINGCOLORFUNCTIONS)
DEF(VM_strdecolorize,				, 477		) // #477 string(string s) : DRESK - Decolorized String (DP_QC_STRINGCOLORFUNCTIONS)
DEF(VM_strftime,					, 478		) // #478 string(float uselocaltime, string format, ...) (DP_QC_STRFTIME)
DEF(VM_tokenizebyseparator,			, 479		) // #479 float(string s) tokenizebyseparator (DP_QC_TOKENIZEBYSEPARATOR)
DEF(VM_strtolower,					, 480		) // #480 string(string s) VM_strtolower : DRESK - Return string as lowercase
DEF(VM_strtoupper,					, 481		) // #481 string(string s) VM_strtoupper : DRESK - Return string as uppercase
DEF(NULL,							, 482		) // #482
DEF(NULL,							, 483		) // #483
DEF(VM_strreplace,					, 484		) // #484 string(string search, string replace, string subject) strreplace (DP_QC_STRREPLACE)
DEF(VM_strireplace,					, 485		) // #485 string(string search, string replace, string subject) strireplace (DP_QC_STRREPLACE)
DEF(NULL,							, 486		) // #486
DEF(VM_gecko_create,				, 487		) // #487 float gecko_create( string name )
DEF(VM_gecko_destroy,				, 488		) // #488 void gecko_destroy( string name )
DEF(VM_gecko_navigate,				, 489		) // #489 void gecko_navigate( string name, string URI )
DEF(VM_gecko_keyevent,				, 490		) // #490 float gecko_keyevent( string name, float key, float eventtype )
DEF(VM_gecko_movemouse,				, 491		) // #491 void gecko_mousemove( string name, float x, float y )
DEF(VM_gecko_resize,				, 492		) // #492 void gecko_resize( string name, float w, float h )
DEF(VM_gecko_get_texture_extent,	, 493		) // #493 vector gecko_get_texture_extent( string name )
DEF(VM_crc16,						, 494		) // #494 float(float caseinsensitive, string s, ...) crc16 = #494 (DP_QC_CRC16)
DEF(VM_cvar_type,					, 495		) // #495 float(string name) cvar_type = #495; (DP_QC_CVAR_TYPE)
DEF(VM_numentityfields,				, 496		) // #496 float() numentityfields = #496; (QP_QC_ENTITYDATA)
DEF(VM_entityfieldname,				, 497		) // #497 string(float fieldnum) entityfieldname = #497; (DP_QC_ENTITYDATA)
DEF(VM_entityfieldtype,				, 498		) // #498 float(float fieldnum) entityfieldtype = #498; (DP_QC_ENTITYDATA)
DEF(VM_getentityfieldstring,		, 499		) // #499 string(float fieldnum, entity ent) getentityfieldstring = #499; (DP_QC_ENTITYDATA)
DEF(VM_putentityfieldstring,		, 500		) // #500 float(float fieldnum, entity ent, string s) putentityfieldstring = #500; (DP_QC_ENTITYDATA)
DEF(NULL,							, 501		) // #501
DEF(NULL,							, 502		) // #502
DEF(VM_whichpack,					, 503		) // #503 string(string) whichpack = #503;
DEF(NULL,							, 504		) // #504
DEF(NULL,							, 505		) // #505
DEF(NULL,							, 506		) // #506
DEF(NULL,							, 507		) // #507
DEF(NULL,							, 508		) // #508
DEF(NULL,							, 509		) // #509
DEF(VM_uri_escape,					, 510		) // #510 string(string in) uri_escape = #510;
DEF(VM_uri_unescape,				, 511		) // #511 string(string in) uri_unescape = #511;
DEF(VM_etof,						, 512		) // #512 float(entity ent) num_for_edict = #512 (DP_QC_NUM_FOR_EDICT)
DEF(VM_uri_get,						, 513		) // #513 float(string uri, float id, [string post_contenttype, string post_delim, [float buf]]) uri_get = #513; (DP_QC_URI_GET, DP_QC_URI_POST)
DEF(VM_tokenize_console,			, 514		) // #514 float(string str) tokenize_console = #514; (DP_QC_TOKENIZE_CONSOLE)
DEF(VM_argv_start_index,			, 515		) // #515 float(float idx) argv_start_index = #515; (DP_QC_TOKENIZE_CONSOLE)
DEF(VM_argv_end_index,				, 516		) // #516 float(float idx) argv_end_index = #516; (DP_QC_TOKENIZE_CONSOLE)
DEF(VM_buf_cvarlist,				, 517		) // #517 void(float buf, string prefix, string antiprefix) buf_cvarlist = #517; (DP_QC_STRINGBUFFERS_CVARLIST)
DEF(VM_cvar_description,			, 518		) // #518 float(string name) cvar_description = #518; (DP_QC_CVAR_DESCRIPTION)
DEF(NULL,							, 519		) // #519
DEF(NULL,							, 520		) // #520
DEF(NULL,							, 521		) // #521
DEF(NULL,							, 522		) // #522
DEF(NULL,							, 523		) // #523
DEF(NULL,							, 524		) // #524
DEF(NULL,							, 525		) // #525
DEF(NULL,							, 526		) // #526
DEF(NULL,							, 527		) // #527
DEF(NULL,							, 528		) // #528
DEF(NULL,							, 529		) // #529
DEF(NULL,							, 530		) // #530
DEF(NULL,							, 531		) // #531
DEF(VM_log,							, 532		) // #532
DEF(VM_getsoundtime,				, 533		) // #533 float(entity e, float channel) getsoundtime = #533; (DP_SND_GETSOUNDTIME)
DEF(VM_soundlength,					, 534		) // #534 float(string sample) soundlength = #534; (DP_SND_GETSOUNDTIME)
DEF(VM_buf_loadfile,                , 535       ) // #535 float(string filename, float bufhandle) buf_loadfile (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(VM_buf_writefile,               , 536       ) // #536 float(float filehandle, float bufhandle, float startpos, float numstrings) buf_writefile (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(VM_bufstr_find,                 , 537       ) // #537 float(float bufhandle, string match, float matchrule, float startpos) bufstr_find (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(VM_matchpattern,                , 538       ) // #538 float(string s, string pattern, float matchrule) matchpattern (DP_QC_STRINGBUFFERS_EXT_WIP)
DEF(NULL,							, 539		) // #539
DEF(NULL,							, 540		) // #540
DEF(NULL,							, 541		) // #541
DEF(NULL,							, 542		) // #542
DEF(NULL,							, 543		) // #543
DEF(NULL,							, 544		) // #544
DEF(NULL,							, 545		) // #545
DEF(NULL,							, 546		) // #546
DEF(NULL,							, 547		) // #547
DEF(NULL,							, 548		) // #548
DEF(NULL,							, 549		) // #549
DEF(NULL,							, 550		) // #550
DEF(NULL,							, 551		) // #551
DEF(NULL,							, 552		) // #552
DEF(NULL,							, 553		) // #553
DEF(NULL,							, 554		) // #554
DEF(NULL,							, 555		) // #555
DEF(NULL,							, 556		) // #556
DEF(NULL,							, 557		) // #557
DEF(NULL,							, 558		) // #558
DEF(NULL,							, 559		) // #559
DEF(NULL,							, 560		) // #560
DEF(NULL,							, 561		) // #561
DEF(NULL,							, 562		) // #562
DEF(NULL,							, 563		) // #563
DEF(NULL,							, 564		) // #564
DEF(NULL,							, 565		) // #565
DEF(NULL,							, 566		) // #566
DEF(NULL,							, 567		) // #567
DEF(NULL,							, 568		) // #568
DEF(NULL,							, 569		) // #569
DEF(NULL,							, 570		) // #570
DEF(NULL,							, 571		) // #571
DEF(NULL,							, 572		) // #572
DEF(NULL,							, 573		) // #573
DEF(NULL,							, 574		) // #574
DEF(NULL,							, 575		) // #575
DEF(NULL,							, 576		) // #576
DEF(NULL,							, 577		) // #577
DEF(NULL,							, 578		) // #578
DEF(NULL,							, 579		) // #579
DEF(NULL,							, 580		) // #580
DEF(NULL,							, 581		) // #581
DEF(NULL,							, 582		) // #582
DEF(NULL,							, 583		) // #583
DEF(NULL,							, 584		) // #584
DEF(NULL,							, 585		) // #585
DEF(NULL,							, 586		) // #586
DEF(NULL,							, 587		) // #587
DEF(NULL,							, 588		) // #588
DEF(NULL,							, 589		) // #589
DEF(NULL,							, 590		) // #590
DEF(NULL,							, 591		) // #591
DEF(NULL,							, 592		) // #592
DEF(NULL,							, 593		) // #593
DEF(NULL,							, 594		) // #594
DEF(NULL,							, 595		) // #595
DEF(NULL,							, 596		) // #596
DEF(NULL,							, 597		) // #597
DEF(NULL,							, 598		) // #598
DEF(NULL,							, 599		) // #599
DEF(NULL,							, 600		) // #600
DEF(VM_M_setkeydest,				, 601		) // #601 void setkeydest(float dest)
DEF(VM_M_getkeydest,				, 602		) // #602 float getkeydest(void)
DEF(VM_M_setmousetarget,			, 603		) // #603 void setmousetarget(float trg)
DEF(VM_M_getmousetarget,			, 604		) // #604 float getmousetarget(void)
DEF(VM_callfunction,				, 605		) // #605 void callfunction(...)
DEF(VM_writetofile,					, 606		) // #606 void writetofile(float fhandle, entity ent)
DEF(VM_isfunction,					, 607		) // #607 float isfunction(string function_name)
DEF(VM_M_getresolution,				, 608		) // #608 vector getresolution(float number, [float forfullscreen])
DEF(VM_keynumtostring,				, 609		) // #609 string keynumtostring(float keynum)
DEF(VM_findkeysforcommand,			, 610		) // #610 string findkeysforcommand(string command[, float bindmap])
DEF(VM_M_getserverliststat,			, 611		) // #611 float gethostcachevalue(float type)
DEF(VM_M_getserverliststring,		, 612		) // #612 string gethostcachestring(float type, float hostnr)
DEF(VM_parseentitydata,				, 613		) // #613 void parseentitydata(entity ent, string data)
DEF(VM_stringtokeynum,				, 614		) // #614 float stringtokeynum(string key)
DEF(VM_M_resetserverlistmasks,		, 615		) // #615 void resethostcachemasks(void)
DEF(VM_M_setserverlistmaskstring,	, 616		) // #616 void sethostcachemaskstring(float mask, float fld, string str, float op)
DEF(VM_M_setserverlistmasknumber,	, 617		) // #617 void sethostcachemasknumber(float mask, float fld, float num, float op)
DEF(VM_M_resortserverlist,			, 618		) // #618 void resorthostcache(void)
DEF(VM_M_setserverlistsort,			, 619		) // #619 void sethostcachesort(float fld, float descending)
DEF(VM_M_refreshserverlist,			, 620		) // #620 void refreshhostcache(void)
DEF(VM_M_getserverlistnumber,		, 621		) // #621 float gethostcachenumber(float fld, float hostnr)
DEF(VM_M_getserverlistindexforkey,	, 622		) // #622 float gethostcacheindexforkey(string key)
DEF(VM_M_addwantedserverlistkey,	, 623		) // #623 void addwantedhostcachekey(string key)
DEF(VM_CL_getextresponse,			, 624		) // #624 string getextresponse(void)
DEF(VM_netaddress_resolve,			, 625		) // #625 string netaddress_resolve(string, float)
DEF(VM_M_getgamedirinfo,			, 626		) // #626 string getgamedirinfo(float n, float prop)
DEF(VM_sprintf,						, 627		) // #627 string sprintf(string format, ...)
DEF(NULL,							, 628		) // #628
DEF(NULL,							, 629		) // #629
DEF(VM_setkeybind,					, 630		) // #630 float(float key, string bind[, float bindmap]) setkeybind
DEF(VM_getbindmaps,					, 631		) // #631 vector(void) getbindmap
DEF(VM_setbindmaps,					, 632		) // #632 float(vector bm) setbindmap
DEF(VM_M_crypto_getkeyfp,			, 633		) // #633 string(string addr) crypto_getkeyfp
DEF(VM_M_crypto_getidfp,			, 634		) // #634 string(string addr) crypto_getidfp
DEF(VM_M_crypto_getencryptlevel,	, 635		) // #635 string(string addr) crypto_getencryptlevel
DEF(VM_M_crypto_getmykeyfp,			, 636		) // #636 string(float addr) crypto_getmykeyfp
DEF(VM_M_crypto_getmyidfp,			, 637		) // #637 string(float addr) crypto_getmyidfp
DEF(NULL,							, 638		) // #638
DEF(VM_digest_hex,					, 639		) // #639
DEF(NULL,							, 640		) // #640
DEF(VM_M_crypto_getmyidstatus,		, 641		) // #641 float(float i) crypto_getmyidstatus
DEF(VM_coverage,					, 642		) // #642
DEF(VM_M_crypto_getidstatus,		, 643		) // #643 float(string addr) crypto_getidstatus

#ifdef KSTEAM
	DEF(NULL,									) // #644
	DEF(NULL,									) // #645
	DEF(NULL,									) // #646
	DEF(NULL,									) // #647
	DEF(NULL,									) // #648
	DEF(NULL,									) // #649
	// WRATH range (#650-#???)
	#if 0
		DEF(VM_fcopy,							) // #650 float(string fnfrom, string fnto) fcopy (EXT_WRATH)
		DEF(VM_frename,							) // #651 float (string fnold, string fnnew) frename (EXT_WRATH)
		DEF(VM_fremove,							) // #652 float (string fname) fremove (EXT_WRATH)
		DEF(VM_fexists,							) // #653 float (string fname) fexists (EXT_WRATH) // Baker: Xonotic has this function as QC, uses fopen read to test.
		DEF(VM_rmtree,							) // #654 float (string path) rmtree (EXT_WRATH)
	#else
		DEF(NULL,								) // #650
		DEF(NULL,								) // #651
		DEF(NULL,								) // #652
		DEF(NULL,								) // #653
		DEF(NULL,								) // #654
	#endif

	DEF(NULL,									) // #655
	DEF(NULL,									) // #656
	DEF(NULL,									) // #657
	DEF(NULL,									) // #658
	DEF(NULL,									) // #659
	DEF(NULL,									) // #660
	DEF(NULL,									) // #661
	DEF(NULL,									) // #662
	DEF(NULL,									) // #663
	DEF(NULL,									) // #664
	DEF(NULL,									) // #665
	DEF(NULL,									) // #666
	DEF(NULL,									) // #667
	DEF(NULL,									) // #668
	DEF(NULL,									) // #669
	DEF(NULL,									) // #670
	DEF(NULL,									) // #671
	DEF(NULL,									) // #672
	DEF(NULL,									) // #673
	DEF(NULL,									) // #674
	DEF(NULL,									) // #675
	DEF(NULL,									) // #676
	DEF(NULL,									) // #677
	DEF(NULL,									) // #678
	DEF(NULL,									) // #679
	DEF(NULL,									) // #680
	DEF(NULL,									) // #681
	DEF(NULL,									) // #682
	DEF(NULL,									) // #683
	DEF(NULL,									) // #684
	DEF(NULL,									) // #685
	DEF(NULL,									) // #686
	DEF(NULL,									) // #687
	DEF(NULL,									) // #688
	DEF(NULL,									) // #689
	DEF(NULL,									) // #690
	DEF(NULL,									) // #691
	DEF(NULL,									) // #692
	DEF(NULL,									) // #693
	DEF(NULL,									) // #694
	DEF(NULL,									) // #695
	DEF(NULL,									) // #696
	DEF(NULL,									) // #697
	DEF(NULL,									) // #698
	DEF(NULL,									) // #699
	DEF(NULL,									) // #700
	DEF(NULL,									) // #701
	DEF(NULL,									) // #702
	DEF(NULL,									) // #703
	DEF(NULL,									) // #704
	DEF(NULL,									) // #705
	DEF(NULL,									) // #706
	DEF(NULL,									) // #707
	DEF(NULL,									) // #708
	DEF(NULL,									) // #709
	DEF(NULL,									) // #710
	DEF(NULL,									) // #711
	DEF(NULL,									) // #712
	DEF(NULL,									) // #713
	DEF(NULL,									) // #714
	DEF(NULL,									) // #715
	DEF(NULL,									) // #716
	DEF(NULL,									) // #717
	DEF(NULL,									) // #718
	DEF(NULL,									) // #719
	DEF(NULL,									) // #720
	DEF(NULL,									) // #721
	DEF(NULL,									) // #722
	DEF(NULL,									) // #723
	DEF(NULL,									) // #724
	DEF(NULL,									) // #725
	DEF(NULL,									) // #726
	DEF(NULL,									) // #727
	DEF(NULL,									) // #728
	DEF(NULL,									) // #729
	DEF(VM_M_stachievement_unlock,				) // #730 void(string achievement_id) stachievement_unlock (EXT_STEAM_REKI)
	DEF(VM_M_stachievement_query,				) // #731 void(string achievement_id) stachievement_query (EXT_STEAM_REKI)
	DEF(VM_M_ststat_setvalue,					) // #732 void(string stat_id, float value) ststat_setvalue (EXT_STEAM_REKI)
	DEF(VM_M_ststat_increment,					) // #733 void(string stat_id, float value) ststat_increment (EXT_STEAM_REKI)
	DEF(VM_M_ststat_query,						) // #734 void(string stat_id) ststat_query (EXT_STEAM_REKI)
	DEF(VM_M_stachievement_register,			) // #735 void(string achievement_id) stachievement_register (EXT_STEAM_REKI)
	DEF(VM_M_ststat_register,					) // #736 void(string stat_id) ststat_register (EXT_STEAM_REKI)
	DEF(NULL,	   								) // #737
	DEF(NULL,									) // #738
	DEF(NULL,									) // #739
	DEF(VM_M_controller_query,	    			) // #740 void(float index) controller_query (EXT_CONTROLLER_REKI)
	DEF(VM_M_controller_rumble,					) // #741 void(float index, float lowmult, float highmult, float msec) controller_rumble (EXT_CONTROLLER_REKI)
	DEF(VM_M_controller_rumbletriggers,			) // #742 void(float index, float leftmult, float rightmult, float msec) controller_rumbletriggers (EXT_CONTROLLER_REKI)
	DEF(NULL,									) // #743
	DEF(NULL,									) // #744
	DEF(NULL,									) // #745
	DEF(NULL,									) // #746
	DEF(NULL,									) // #747
	DEF(NULL,									) // #748
	DEF(NULL,									) // #749
	DEF(NULL
#endif // KSTEAM

#undef DEF