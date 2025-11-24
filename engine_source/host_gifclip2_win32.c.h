// host_gifclip2.c.h


byte	g2_thread_state;		// 0 inactive, 1 running, 2 exited
byte	g2_thread_exitcode;



//prog->process_handle = System_Process_Create (x_compiler, x_switches_full, fs_workingdir_null);
RELATED_ (System_Process_Create)

// Baker: I am thinking it is best if we malloc anything we use
// while a separate thread is running.  I don't think collision with the main thread for memory allocation
// is very likely.  What is that server block thread stuff we need to do to con print?

int SV_ThreadFunc_G2 (void *data)
{
	char *extensions_gif_all_files =
		"GIF Files(.gif)|*.gif|"
		"All Files(.)|*.*";

	char *FS_OpenDialog_With_Path_Extensions_Unsafe_ZAlloc (ccs *realpath, ccs *my_piped_extensions);	
	char *realpath = "c:/galaxy/zircon/textures";
	char *wtf8name_za = FS_OpenDialog_With_Path_Extensions_Unsafe_ZAlloc (realpath, extensions_gif_all_files);
	
	if (wtf8name_za == NULL) {
		Con_PrintLinef ("Dialog aborted");
		g2_thread_state = BTHREAD_EXITED_2;
		g2_thread_exitcode = 1;

		return -1;
	}
	g2_thread_state = BTHREAD_EXITED_2;
	g2_thread_exitcode = 0;
	return 0; // Success
}

static void Host_gifclip2_f(cmd_state_t *cmd)
{



//	return;

	if (g2_thread_state == 2) {
		Con_PrintLinef ("Thread completed = %d, try later", g2_thread_state);
		g2_thread_state = 0;
		return;
	}
	if (g2_thread_state) {
		Con_PrintLinef ("Thread is running state = %d, try later", g2_thread_state);
		return;
	}


	g2_thread_state = 1; // BTHREAD_RUNNING_1 = active
	g2_thread_exitcode = 0;
	svs.gifclip2_thread_id = Thread_CreateThread(SV_ThreadFunc_G2, "data string to pass");

// NO WAIT
#if 0
	RELATED_ (SV_StopThread threadstop is volatile)
	Con_PrintLinef ("Preparing to wait for thread ...");
	Thread_WaitThread (svs.gifclip2_thread_id, /*retval is int*/ 0);
	Con_PrintLinef ("Thread is over");
	Con_PrintLinef ("Thread state = %d", g2_thread_state);
	//Thread_DestroyMutex	(svs.threadmutex);
#endif
}


