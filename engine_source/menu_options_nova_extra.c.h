// menu_options2.c.h

extern cvar_t sbar_quake;
extern cvar_t r_viewmodel_quake;
extern cvar_t r_waterdeform;
extern cvar_t cl_bobmodel_speed;
extern cvar_t cl_bobmodel_classic;
extern cvar_t cl_bob;


const char *get_overbright4_text (int level)
{
	switch (level) {
	default:
	case  0:	return "DarkPlaces";
	case  1:	return "Quakespasm";
	case  2:	return "Minimum Light 3";
//	case  3:	return "Minimum Light 0.5";
	}
}

const char *get_bobbing2_text (int level)
{
	switch (level) {
	default:
	case  0:	return "Quake";
	case  1:	return "DarkPlaces";
	case  2:	return "Off";
	}
}

int get_bobbing2_rot()
{
	if (cl_bob.value == 0) return 2; // Off
	if (cl_bobmodel_classic.value == 0) return 1; // DarkPlaces
	return 0;
}

// Get the current level.  DarkPlaces 0, Quakespasm 1
int get_overbright4_rot()
{
	if (r_minlight.value == 3) return 2;
	//if (r_minlight.value == 0.5) return 3;
	if (gl_overbright_models.value) return 1;	// Off
	return 0;
}


static void set_overbright4 (int level)
{
	switch (level) {
	default:
	case 0:  Cbuf_AddTextLine (cmd_local, "gl_overbright_models 0; r_minlight 0"); break; // DarkPlaces
	case 1:  Cbuf_AddTextLine (cmd_local, "gl_overbright_models 1; r_minlight 0"); break; // Quakespasm
	case 2:  Cbuf_AddTextLine (cmd_local, "gl_overbright_models 0; r_minlight 3"); break; // Minimum Light 3
	//case 3:  Cbuf_AddTextLine (cmd_local, "gl_overbright_models 0; r_minlight 0.5"); break; // Minimum Light 0.5
	} // sw
}

static void set_bobbing2 (int level)
{
	switch (level) {
	default:
	case 0: // Quake
		Cbuf_AddTextLine(cmd_local, "cl_bobmodel_classic 1; cvar_reset cl_bob");
		break;

	case 1:  // DarkPlaces
		Cbuf_AddTextLine (cmd_local, "cl_bobmodel_classic 0; cvar_reset cl_bob");
		break;


	case 2:  // Off
		Cbuf_AddTextLine (cmd_local, "cvar_reset cl_bobmodel_classic; cl_bob 0");
		break;
	} // sw

}

// Gun position
const char *get_gunpos3_text (int level)
{
	switch (level) {
	default:
	case  0:	return "DarkPlaces ";
	case  1:	return "Quake";
	}
}

int get_gunpos3_rot()
{
	if (!r_viewmodel_quake.value) return 0;	// Off
	return 1;
}


static void set_gunpos3 (int level)
{
	switch (level) {
	default:
	case 0:  Cbuf_AddTextLine (cmd_local, "r_viewmodel_quake 0"); break; // DarkPlaces
	case 1:  Cbuf_AddTextLine (cmd_local, "r_viewmodel_quake 1"); break; // Quake
	} // sw
}


// Warp
const char *get_waterwarp_text (int level)
{
	switch (level) {
	default:
	case  0:	return "off";
	case  2:	return "twisty (2)";
	case  1:	return "classic (1)";
	}
}

int set_waterwarp6()
{
	if (!r_waterwarp.value) return 0;
	if (r_waterwarp.value >= 2) return 2;
	return 1;
}


int get_waterwarp_rot(void)
{
	if (!r_waterwarp.value) return 0;
	if (r_waterwarp.value == 2) return 2;
	return 1;
}


// Warp
const char *get_waterdeform_text (int level)
{
	switch (level) {
	default:
	case  0:	return "off";
	case  2:	return "liquids (2)";
	case  1:	return "water/slime (1)";
	}
}

int get_deform_rot(void)
{
	if (!r_waterdeform.value) return 0;
	if (r_waterdeform.value >= 2) return 2;
	return 1;
}


const char *get_statusbar4_text(int level)
{
	switch (level) {
	default:
	case  0:	return "DarkPlaces";
	case  2:	return "Re-Release";
	case  1:	return "Quake Backtile";
	}
}

int get_statusbar4_rot (void)
{
	if (!sbar_quake.value) return 0;
	if (sbar_quake.value >= 2) return 2;
	return 1;	
}


static void set_statusbar4 (int level)
{
	switch (level) {
	default:
	case 0:		Cbuf_AddTextLine (cmd_local, "sbar_quake 0");	break; // DarkPlaces
	case 1:		Cbuf_AddTextLine (cmd_local, "sbar_quake 1");	break; // Traditional
	case 2:		Cbuf_AddTextLine (cmd_local, "sbar_quake 2");	break; // Remaster
	} // sw
}


// Show fps
const char *get_showfps5_text (int level)
{
	switch (level) {
	default:
	case  0:	return "off";
	case  1:	return "on";
	case  2:	return "top corner";
	}
}

int get_showfps5_rot()
{
	if (!showfps.value) return 0;
	if (showfps.value < 0) return 2;
	return 1;	
}


static void set_showfps5 (int level)
{
	switch (level) {
	default:
	case 0: Cbuf_AddTextLine (cmd_local, "showfps 0"); break;
	case 1:	Cbuf_AddTextLine (cmd_local, "showfps 1"); break;
	case 2: Cbuf_AddTextLine (cmd_local, "showfps -1"); break;
	} // sw
}



const char *get_effects0_text (int level)
{
	switch (level) {
	case 0:		return "1/6 -Dynamic Light";
	case 1:		return "2/6  Vanilla -gloss -decals";
	default:
	case 2:		return "3/6  Default";
	case 3:		return "4/6 +Bloom, +Deluxe";
	case 4:		return "5/6 +Offset Mapping";
	case 5:		return "6/6 +Realtime Light";
	}
}

int get_effects0_rot()
{
	if (cl_particles_quality.value <= 0.85 ) return 0;
	if (cl_particles_quality.value <= 0.995 ) return 1;
	if (cl_particles_quality.value <= 1.005 ) return 2;
	if (cl_particles_quality.value <= 1.015 ) return 3;
	if (cl_particles_quality.value <= 1.025 ) return 4;
	return 5;
}




