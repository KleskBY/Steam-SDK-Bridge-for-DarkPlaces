// host_dthread_win32.c.h

// See also:
RELATED_ (DTHREAD)



byte	dungeon_thread_state;		// 0 inactive, 1 running, 2 exited
byte	dungeon_thread_exitcode;


//prog->process_handle = System_Process_Create (x_compiler, x_switches_full, fs_workingdir_null);
RELATED_ (System_Process_Create)
int SV_ThreadFunc_Dungeon (void *data)
{
	char *s_data = (char *)data;

	char *s_pipe = strstr (s_data, "|");
	if (!s_pipe)
		return -1;
	char *x_compiler = s_data;
	char *x_switches_full = &s_pipe[1];
	s_pipe[0] = NULL_CHAR_0;


#ifdef _WIN32
	File_URL_Edit_SlashesBack_Like_Windows (x_compiler);
	// Problem is that this pops a window
	RELATED_ (VM_map_compile_with_callback)
	//int is_ok = Sys_ShellExecute_Wait (x_compiler, x_switches_full);
	sys_handle_t process_handle = System_Process_Create (x_compiler, x_switches_full, fs_workingdir_null);

	DebugPrintLinef ("Thread started = %s", s_data);
	DebugPrintLinef ("Exec = %s", x_compiler);
	DebugPrintLinef ("Args = %s", x_switches_full);
	DebugPrintLinef ("Process handle = %p", process_handle);
	int exit_code = 0;
	while (1) {
		int result = System_Process_Is_Still_Running_Neg1_Error(process_handle, &exit_code);

		if (result == false) {
			// Process completed with exit code
			break;
		} else if (result == not_found_neg1) {
			// Can't find process ... interpret as error
			if (!exit_code)
				exit_code = -9999;
			break;
		}

		// Still running
		//sleep (1);
		continue;
	}

	DebugPrintLinef ("Exit code = %d", exit_code);
#else
	int is_ok = Sys_ShellExecute_Wait (x_compiler, x_switches_full);
	if (!is_ok) {
		// Warning or what?
	}
#endif

	// Exit code = how?  GetExitCodeProcess
	DebugPrintLinef ("Process complete", x_compiler);

	dungeon_thread_state = BTHREAD_EXITED_2;
	dungeon_thread_exitcode = 1;
	return 0; // Success
}

static void DThread_f(cmd_state_t *cmd)
{
	//svs.dungeon_thread_mutex_lock = Thread_CreateMutex();
#ifdef _WIN32
	ccs *s_exe = "dungeon/q3map2/q3map2_64_light_auto.exe"
#else
	ccs *s_exe = "dungeon/q3map2/q3map2.x86_64"
#endif
					"|"
	"-fs_game dungeon -game nexuiz -meta -v -mv 1000000 -mi 6000000 -samplesize 8 \"dungeon/maps/my_map\"";
	va_super (s_exe_dispose, 1024, "%s", s_exe);

	if (dungeon_thread_state) {
		Con_PrintLinef ("Thread is running state = %d, try later", dungeon_thread_state);
		return;
	}

	dungeon_thread_state = BTHREAD_RUNNING_1; // 1 = active
	dungeon_thread_exitcode = 0;
	svs.dungeon_thread_id = Thread_CreateThread(SV_ThreadFunc_Dungeon, s_exe_dispose);

	RELATED_ (SV_StopThread threadstop is volatile)
	// Wait for the thread to finish
	//SDL_WaitThread (svs.dungeon_thread_id, NULL);
	//Thread_WaitThread (svs.dungeon_thread_id, NULL);
	Con_PrintLinef ("Preparing to wait for thread ...");
	Thread_WaitThread (svs.dungeon_thread_id, /*retval is int*/ 0);
	Con_PrintLinef ("Thread is over");
	Con_PrintLinef ("Thread state = %d", dungeon_thread_state);
	//Thread_DestroyMutex	(svs.threadmutex);
}


