// fs_baker_csg_decals.c.h

// ENTITY:	"info_decal_start"
// ACTION:	Emits a series of decals
// PURPOSE: Roads

	// OLDY STYLE with own .MAP parser.
	if (String_Contains (sinza, "info_decal_start")) {
		Con_PrintLinef ("Detected " "info_decal_start" " in .map (for decal roads), checking ...");
		baker_string_t *bsout = CSG_Process_BSAlloc(sinza);
		if (!bsout)	Perform_Exit_1_Error("CSG_Process_BSAlloc failed to create bsout"); // error processing the map for some reason.

		int isok2 = FS_SaveFileString_Is_Ok_Unsafe (mappath, bsout->string);
		BakerString_Destroy_And_Null_It (&bsout);
		if (isok2 == false) Perform_Exit_1_Error("Save file string failed."); // Couldn't write file

		{ // REFRESH SOURCE Free the map data -- then reload it
			Mem_FreeNull_ (sinza); sinza = FS_LoadFileString_Unsafe_ZAlloc (mappath); if (!sinza) Perform_Exit_1_Error("Reload FS_LoadFileString_Unsafe_ZAlloc sinza failed"); // Couldn't reopen map
		}
	} // while



