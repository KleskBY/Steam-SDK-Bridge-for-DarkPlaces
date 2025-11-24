// fs_baker_dungeon.c.h

// Map tiles.

// What folder does the .map live
// What command line parms
// ** Command: C:/galaxy/zircon_beta_gcc.exe
// ** Parameters: -csg "C:\galaxy\zircon\maps\zec_park_empty"


// ** Command: C:/Program Files/J.A.C.K/quake3/q3map2_64_light_auto.exe
//** Parameters: -game nexuiz -fs_basepath "C:/galaxy/" -fs_game zircon 
//   -meta  -v -mv 1000000 -mi 6000000 -samplesize 8 "C:\galaxy\zircon\maps\zec_park_empty"

// -fs_basepath "C:/galaxy/" ... c:\quak  // basedir in unix folder trailing slash
// -fs_game     "zircon"   ..... gamedir foldername no parent path, no trailing 
// quoted full path to map in OS format (windows) with no extension.
// our executatble name




// CSG PROCESS IS HERE
void DungeonMake_f (cmd_state_t *cmd)
{
}

// Q: Is strbuf just a stringlist
// A: No. Mem_ExpandableArray_RecordAtIndex

RELATED_ (entitylist_add, FS_Shell_NonBlocking_f Replacer_Did_Change VM_buf_create)
RELATED_ (System_Process_Create)


void DungeonMake (ccs *s_basedungeon_dot_map, ccs *s)
{
	
	//data = (unsigned char *) FS_LoadFile(litfilename, tempmempool, fs_quiet_FALSE, &filesize);

}


