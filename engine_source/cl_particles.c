/*
Copyright (C) 1996-1997 Id Software, Inc.

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

// cl_particles.c
#include "quakedef.h"

#include "cl_collision.h"
#include "image.h"
#include "r_shadow.h"

// must match ptype_e values
particletype_t particletype[pt_total] =
{
	{PBLEND_INVALID,	PORIENT_INVALID,				false}, //pt_dead (should never happen)
	{PBLEND_ALPHA_0,	PORIENT_BILLBOARD_0,			false}, //pt_alphastatic
	{PBLEND_ADD,		PORIENT_BILLBOARD_0,			false}, //pt_static
	{PBLEND_ADD,		PORIENT_SPARK,					false}, //pt_spark
	{PBLEND_ADD,		PORIENT_HBEAM,					false}, //pt_beam
	{PBLEND_ADD,		PORIENT_SPARK,					false}, //pt_rain
	{PBLEND_ADD,		PORIENT_ORIENTED_DOUBLESIDED,	false}, //pt_raindecal
	{PBLEND_ADD,		PORIENT_BILLBOARD_0,			false}, //pt_snow
	{PBLEND_ADD,		PORIENT_BILLBOARD_0,			false}, //pt_bubble
	{PBLEND_INVMOD,		PORIENT_BILLBOARD_0,			false}, //pt_blood
	{PBLEND_ADD,		PORIENT_BILLBOARD_0,			false}, //pt_smoke
	{PBLEND_INVMOD,		PORIENT_ORIENTED_DOUBLESIDED,	false}, //pt_decal
	{PBLEND_ALPHA_0,	PORIENT_BILLBOARD_0,			false}, //pt_entityparticle
};

#define PARTICLEEFFECT_UNDERWATER_1		1
#define PARTICLEEFFECT_NOTUNDERWATER_2	2
#define PARTICLEEFFECT_FORCENEAREST_4	4			// Baker: This is cubemap
//#define PARTICLEEFFECT_TEX2_ACTIVE_8	8			// Baker:
//#define PARTICLEEFFECT_CHILD_32			32		// Baker:
#define PARTICLEEFFECT_DEFINED			2147483648U	// Baker: This is 2 ^ 31 (2 to the 31 power)


RELATED_ (baselineparticleeffectinfo PARTICLEEFFECT_NOTUNDERWATER_2)
typedef struct particleeffectinfo_s { // Baker: EFFECTINFO particle information
	int effectnameindex; // which effect this belongs to
	// PARTICLEEFFECT_* bits
	RELATED_ (PARTICLEEFFECT_NOTUNDERWATER_2)
	int einfo_flags; // Baker: PARTICLEEFFECT_FORCENEAREST, PARTICLEEFFECT_NOTUNDERWATER
	// blood effects may spawn very few particles, so proper fraction-overflow
	// handling is very important, this variable keeps track of the fraction
	double particleaccumulator;
	// the math is: countabsolute + requestedcount * countmultiplier * quality
	// absolute number of particles to spawn, often used for decals
	// (unaffected by quality and requestedcount)
	float countabsolute;
	// multiplier for the number of particles CL_ParticleEffect was told to
	// spawn, most effects do not really have a count and hence use 1, so
	// this is often the actual count to spawn, not merely a multiplier
	float countmultiplier;
	// if > 0 this causes the particle to spawn in an evenly spaced line from
	// originmins to originmaxs (causing them to describe a trail, not a box)
	float trailspacing;
	// type of particle to spawn (defines some aspects of behavior)
	ptype_e particletype;
	// blending mode used on this particle type
	pblend_t blendmode;
	// orientation of this particle type (BILLBOARD, SPARK, BEAM, etc)
	porientation_t orientation;
	// range of colors to choose from in hex RRGGBB (like HTML color tags),
	// randomly interpolated at spawn
	unsigned int color[2];
	// a random texture is chosen in this range (note the second value is one
	// past the last choosable, so for example 8,16 chooses any from 8 up and
	// including 15)
	// if start and end of the range are the same, no randomization is done
	int tex[2];
	// range of size values randomly chosen when spawning, plus size increase over time
	float size[3];
	// range of alpha values randomly chosen when spawning, plus alpha fade
	float alpha[3];
	// how long the particle should live (note it is also removed if alpha drops to 0)
	float time[2];
	struct {
		pblend_t blendmode;
		// orientation of this particle type (BILLBOARD, SPARK, BEAM, etc)
		porientation_t orientation;
		// range of colors to choose from in hex RRGGBB (like HTML color tags),
		// randomly interpolated at spawn
		unsigned int color[2];
		// a random texture is chosen in this range (note the second value is one
		// past the last choosable, so for example 8,16 chooses any from 8 up and
		// including 15)
		// if start and end of the range are the same, no randomization is done
		int tex[2];
		// range of size values randomly chosen when spawning, plus size increase over time
		float size[3];
		// range of alpha values randomly chosen when spawning, plus alpha fade
		float alpha[3];
		// how long the particle should live (note it is also removed if alpha drops to 0)
		float time[2];
	} b;
	// how much gravity affects this particle (negative makes it fly up!)
	float gravity;
	// how much bounce the particle has when it hits a surface
	// if negative the particle is removed on impact
	float bounce;
	// if in air this friction is applied
	// if negative the particle accelerates
	float airfriction;
	// if in liquid (water/slime/lava) this friction is applied
	// if negative the particle accelerates
	float liquidfriction;
	// these offsets are added to the values given to particleeffect(), and
	// then an ellipsoid-shaped jitter is added as defined by these
	// (they are the 3 radii)
	float stretchfactor;
	// stretch velocity factor (used for sparks)
	float originoffset[3];
	float relativeoriginoffset[3];
	float velocityoffset[3];
	float relativevelocityoffset[3];
	float originjitter[3];
	float velocityjitter[3];
	float velocitymultiplier;
	// an effect can also spawn a dlight
	float lightradiusstart;
	float lightradiusfade;
	float lighttime;
	float lightcolor[3];
	qbool lightshadow;
	int lightcubemapnum;
	float lightcorona[2];
	unsigned int staincolor[2]; // note: 0x808080 = neutral (particle's own color), these are modding factors for the particle's original color!
	int staintex[2];
	float stainalpha[2];
	float stainsize[2];
	// other parameters
	float rotate[4]; // min/max base angle, min/max rotation over time
}
particleeffectinfo_t;

char particleeffectname[MAX_PARTICLEEFFECTNAME_4096][64];

int numparticleeffectinfo;
particleeffectinfo_t particleeffectinfo[MAX_PARTICLEEFFECTINFO_8192];

static int particlepalette[256];
/*
	0x000000,0x0f0f0f,0x1f1f1f,0x2f2f2f,0x3f3f3f,0x4b4b4b,0x5b5b5b,0x6b6b6b, // 0-7
	0x7b7b7b,0x8b8b8b,0x9b9b9b,0xababab,0xbbbbbb,0xcbcbcb,0xdbdbdb,0xebebeb, // 8-15
	0x0f0b07,0x170f0b,0x1f170b,0x271b0f,0x2f2313,0x372b17,0x3f2f17,0x4b371b, // 16-23
	0x533b1b,0x5b431f,0x634b1f,0x6b531f,0x73571f,0x7b5f23,0x836723,0x8f6f23, // 24-31
	0x0b0b0f,0x13131b,0x1b1b27,0x272733,0x2f2f3f,0x37374b,0x3f3f57,0x474767, // 32-39
	0x4f4f73,0x5b5b7f,0x63638b,0x6b6b97,0x7373a3,0x7b7baf,0x8383bb,0x8b8bcb, // 40-47
	0x000000,0x070700,0x0b0b00,0x131300,0x1b1b00,0x232300,0x2b2b07,0x2f2f07, // 48-55
	0x373707,0x3f3f07,0x474707,0x4b4b0b,0x53530b,0x5b5b0b,0x63630b,0x6b6b0f, // 56-63
	0x070000,0x0f0000,0x170000,0x1f0000,0x270000,0x2f0000,0x370000,0x3f0000, // 64-71
	0x470000,0x4f0000,0x570000,0x5f0000,0x670000,0x6f0000,0x770000,0x7f0000, // 72-79
	0x131300,0x1b1b00,0x232300,0x2f2b00,0x372f00,0x433700,0x4b3b07,0x574307, // 80-87
	0x5f4707,0x6b4b0b,0x77530f,0x835713,0x8b5b13,0x975f1b,0xa3631f,0xaf6723, // 88-95
	0x231307,0x2f170b,0x3b1f0f,0x4b2313,0x572b17,0x632f1f,0x733723,0x7f3b2b, // 96-103
	0x8f4333,0x9f4f33,0xaf632f,0xbf772f,0xcf8f2b,0xdfab27,0xefcb1f,0xfff31b, // 104-111
	0x0b0700,0x1b1300,0x2b230f,0x372b13,0x47331b,0x533723,0x633f2b,0x6f4733, // 112-119
	0x7f533f,0x8b5f47,0x9b6b53,0xa77b5f,0xb7876b,0xc3937b,0xd3a38b,0xe3b397, // 120-127
	0xab8ba3,0x9f7f97,0x937387,0x8b677b,0x7f5b6f,0x775363,0x6b4b57,0x5f3f4b, // 128-135
	0x573743,0x4b2f37,0x43272f,0x371f23,0x2b171b,0x231313,0x170b0b,0x0f0707, // 136-143
	0xbb739f,0xaf6b8f,0xa35f83,0x975777,0x8b4f6b,0x7f4b5f,0x734353,0x6b3b4b, // 144-151
	0x5f333f,0x532b37,0x47232b,0x3b1f23,0x2f171b,0x231313,0x170b0b,0x0f0707, // 152-159
	0xdbc3bb,0xcbb3a7,0xbfa39b,0xaf978b,0xa3877b,0x977b6f,0x876f5f,0x7b6353, // 160-167
	0x6b5747,0x5f4b3b,0x533f33,0x433327,0x372b1f,0x271f17,0x1b130f,0x0f0b07, // 168-175
	0x6f837b,0x677b6f,0x5f7367,0x576b5f,0x4f6357,0x475b4f,0x3f5347,0x374b3f, // 176-183
	0x2f4337,0x2b3b2f,0x233327,0x1f2b1f,0x172317,0x0f1b13,0x0b130b,0x070b07, // 184-191
	0xfff31b,0xefdf17,0xdbcb13,0xcbb70f,0xbba70f,0xab970b,0x9b8307,0x8b7307, // 192-199
	0x7b6307,0x6b5300,0x5b4700,0x4b3700,0x3b2b00,0x2b1f00,0x1b0f00,0x0b0700, // 200-207
	0x0000ff,0x0b0bef,0x1313df,0x1b1bcf,0x2323bf,0x2b2baf,0x2f2f9f,0x2f2f8f, // 208-215
	0x2f2f7f,0x2f2f6f,0x2f2f5f,0x2b2b4f,0x23233f,0x1b1b2f,0x13131f,0x0b0b0f, // 216-223
	0x2b0000,0x3b0000,0x4b0700,0x5f0700,0x6f0f00,0x7f1707,0x931f07,0xa3270b, // 224-231
	0xb7330f,0xc34b1b,0xcf632b,0xdb7f3b,0xe3974f,0xe7ab5f,0xefbf77,0xf7d38b, // 232-239
	0xa77b3b,0xb79b37,0xc7c337,0xe7e357,0x7fbfff,0xabe7ff,0xd7ffff,0x670000, // 240-247
	0x8b0000,0xb30000,0xd70000,0xff0000,0xfff393,0xfff7c7,0xffffff,0x9f5b53  // 248-255
*/

int		ramp1[8] = {0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65, 0x63, 0x61};
int		ramp2[8] = {0x6f, 0x6e, 0x6d, 0x6c, 0x6b, 0x6a, 0x68, 0x66};
int		ramp3[8] = {0x6d, 0x6b, 6, 5, 4, 3};

//static int explosparkramp[8] = {0x4b0700, 0x6f0f00, 0x931f07, 0xb7330f, 0xcf632b, 0xe3974f, 0xffe7b5, 0xffffff};

// particletexture_t is a rectangle in the particlefonttexture
typedef struct particletexture_s
{
	rtexture_t *texture;
	float s1, t1, s2, t2;
}
particletexture_t;

static rtexturepool_t *particletexturepool;
static rtexture_t *particlefonttexture;
static particletexture_t particletexture[MAX_PARTICLETEXTURES_256];
skinframe_t *decalskinframe;

// texture numbers in particle font
static const int tex_smoke[8] = {0, 1, 2, 3, 4, 5, 6, 7};
static const int tex_bulletdecal[8] = {8, 9, 10, 11, 12, 13, 14, 15};
static const int tex_blooddecal[8] = {16, 17, 18, 19, 20, 21, 22, 23};
static const int tex_bloodparticle[8] = {24, 25, 26, 27, 28, 29, 30, 31};
static const int tex_rainsplash = 32;
static const int tex_beam = 60;
static const int tex_bubble = 62;
static const int tex_raindrop = 61;
static const int tex_particle = 63;

#include "cl_particles_baseline.c.h"

cvar_t cl_particles = {CF_CLIENT | CF_ARCHIVE, "cl_particles", "1", "enables particle effects"};
cvar_t cl_particles_quality = {CF_CLIENT | CF_ARCHIVE, "cl_particles_quality", "1", "multiplies number of particles"};
cvar_t cl_particles_alpha = {CF_CLIENT | CF_ARCHIVE, "cl_particles_alpha", "1", "multiplies opacity of particles"};
cvar_t cl_particles_size = {CF_CLIENT | CF_ARCHIVE, "cl_particles_size", "1", "multiplies particle size"};
cvar_t cl_particles_quake = {CF_CLIENT | CF_ARCHIVE, "cl_particles_quake", "0", "makes particle effects look mostly like the ones in Quake"};
cvar_t cl_particles_blood = {CF_CLIENT | CF_ARCHIVE, "cl_particles_blood", "1", "enables blood effects"};
cvar_t cl_particles_blood_alpha = {CF_CLIENT | CF_ARCHIVE, "cl_particles_blood_alpha", "1", "opacity of blood, does not affect decals"};
cvar_t cl_particles_blood_decal_alpha = {CF_CLIENT | CF_ARCHIVE, "cl_particles_blood_decal_alpha", "1", "opacity of blood decal"};
cvar_t cl_particles_blood_decal_scalemin = {CF_CLIENT | CF_ARCHIVE, "cl_particles_blood_decal_scalemin", "1.5", "minimal random scale of decal"};
cvar_t cl_particles_blood_decal_scalemax = {CF_CLIENT | CF_ARCHIVE, "cl_particles_blood_decal_scalemax", "2", "maximal random scale of decal"};
cvar_t cl_particles_blood_bloodhack = {CF_CLIENT | CF_ARCHIVE, "cl_particles_blood_bloodhack", "1", "make certain quake particle() calls create blood effects instead"};
cvar_t cl_particles_bulletimpacts = {CF_CLIENT | CF_ARCHIVE, "cl_particles_bulletimpacts", "1", "enables bulletimpact effects"};
cvar_t cl_particles_explosions_sparks = {CF_CLIENT | CF_ARCHIVE, "cl_particles_explosions_sparks", "1", "enables sparks from explosions"};
cvar_t cl_particles_explosions_shell = {CF_CLIENT | CF_ARCHIVE, "cl_particles_explosions_shell", "0", "enables polygonal shell from explosions"};
cvar_t cl_particles_rain = {CF_CLIENT | CF_ARCHIVE, "cl_particles_rain", "1", "enables rain effects"};
cvar_t cl_particles_snow = {CF_CLIENT | CF_ARCHIVE, "cl_particles_snow", "1", "enables snow effects"};
cvar_t cl_particles_smoke = {CF_CLIENT | CF_ARCHIVE, "cl_particles_smoke", "1", "enables smoke (used by multiple effects)"};
cvar_t cl_particles_smoke_alpha = {CF_CLIENT | CF_ARCHIVE, "cl_particles_smoke_alpha", "0.5", "smoke brightness"};
cvar_t cl_particles_smoke_alphafade = {CF_CLIENT | CF_ARCHIVE, "cl_particles_smoke_alphafade", "0.55", "brightness fade per second"};
cvar_t cl_particles_sparks = {CF_CLIENT | CF_ARCHIVE, "cl_particles_sparks", "1", "enables sparks (used by multiple effects)"};
cvar_t cl_particles_bubbles = {CF_CLIENT | CF_ARCHIVE, "cl_particles_bubbles", "1", "enables bubbles (used by multiple effects)"};
cvar_t cl_particles_visculling = {CF_CLIENT | CF_ARCHIVE, "cl_particles_visculling", "0", "perform a costly check if each particle is visible before drawing"};
cvar_t cl_particles_collisions = {CF_CLIENT | CF_ARCHIVE, "cl_particles_collisions", "1", "allow costly collision detection on particles (sparks that bounce, particles not going through walls, blood hitting surfaces, etc)"};
cvar_t cl_particles_forcetraileffects = {CF_CLIENT, "cl_particles_forcetraileffects", "0", "force trails to be displayed even if a non-trail draw primitive was used (debug/compat feature)"};
#if 1 // STAIN-EON
cvar_t cl_particles_stain_mix_with_particle_color = {CF_CLIENT, "cl_particles_stain_mix_with_particle_color", "1", "Mix particle color into end result stain color [Zircon]"}; // STAIN-EON
#endif

cvar_t cl_decals = {CF_CLIENT | CF_ARCHIVE, "cl_decals", "1", "enables decals (bullet holes, blood, etc)"};
cvar_t cl_decals_time = {CF_CLIENT | CF_ARCHIVE, "cl_decals_time", "20", "how long before decals start to fade away"};
cvar_t cl_decals_fadetime = {CF_CLIENT | CF_ARCHIVE, "cl_decals_fadetime", "1", "how long decals take to fade away"};
cvar_t cl_decals_newsystem_intensitymultiplier = {CF_CLIENT | CF_ARCHIVE, "cl_decals_newsystem_intensitymultiplier", "2", "boosts intensity of decals (because the distance fade can make them hard to see otherwise)"};
cvar_t cl_decals_newsystem_immediatebloodstain = {CF_CLIENT | CF_ARCHIVE, "cl_decals_newsystem_immediatebloodstain", "2", "0: no on-spawn blood stains; 1: on-spawn blood stains for pt_blood; 2: always use on-spawn blood stains"};
cvar_t cl_decals_newsystem_bloodsmears = {CF_CLIENT | CF_ARCHIVE, "cl_decals_newsystem_bloodsmears", "1", "enable use of particle velocity as decal projection direction rather than surface normal"};
cvar_t cl_decals_models = {CF_CLIENT | CF_ARCHIVE, "cl_decals_models", "0", "enables decals on animated models"};
cvar_t cl_decals_bias = {CF_CLIENT | CF_ARCHIVE, "cl_decals_bias", "0.125", "distance to bias decals from surface to prevent depth fighting"};
cvar_t cl_decals_max = {CF_CLIENT | CF_ARCHIVE, "cl_decals_max", "4096", "maximum number of decals allowed to exist in the world at once"};


RELATED_ (CL_Particles_ParseEffectInfo)
#include "cl_particles_effectinfo_load.c.h"


/*
===============
CL_InitParticles
===============
*/


void CL_ReadPointFile_f(cmd_state_t *cmd);


void CL_Particles_Shutdown (void)
{
}

void CL_SpawnDecalParticleForSurface(int hitent, const vec3_t org, const vec3_t normal, int color1, int color2, int texnum, float size, float alpha);
void CL_SpawnDecalParticleForPoint(const vec3_t org, float maxdist, float size, float alpha, int texnum, int color1, int color2);

// list of all 26 parameters:
// ptype - any of the pt_ enum values (pt_static, pt_blood, etc), see ptype_e near the top of this file
// pcolor1,pcolor2 - minimum and maximum ranges of color, randomly interpolated to decide particle color
// ptex - any of the tex_ values such as tex_smoke[rand()&7] or tex_particle
// psize - size of particle (or thickness for PARTICLE_SPARK and PARTICLE_*BEAM)
// palpha - opacity of particle as 0-255 (can be more than 255)
// palphafade - rate of fade per second (so 256 would mean a 256 alpha particle would fade to nothing in 1 second)
// ptime - how long the particle can live (note it is also removed if alpha drops to nothing)
// pgravity - how much effect gravity has on the particle (0-1)
// pbounce - how much bounce the particle has when it hits a surface (0-1), -1 makes a blood splat when it hits a surface, 0 does not even check for collisions
// px,py,pz - starting origin of particle
// pvx,pvy,pvz - starting velocity of particle
// pfriction - how much the particle slows down per second (0-1 typically, can slowdown faster than 1)
// blendmode - one of the PBLEND_ values
// orientation - one of the PARTICLE_ values
// staincolor1, staincolor2: minimum and maximum ranges of stain color, randomly interpolated to decide stain color (-1 to use none)
// staintex: any of the tex_ values such as tex_smoke[rand()&7] or tex_particle (-1 to use none)
// stainalpha: opacity of the stain as factor for alpha
// stainsize: size of the stain as factor for palpha
// angle: base rotation of the particle geometry around its center normal
// spin: rotation speed of the particle geometry around its center normal

CALLERS_ (Many)
CALLERS_ (CL_ParticleEffect_Fallback plus other SVC totalling 65 instances )

#include "cl_particles_new_particle.c.h"

static void CL_ImmediateBloodStain(particle_t *part)
{
	vec3_t v;
	int staintex;

	// blood creates a splash at spawn, not just at impact, this makes monsters bloody where they are shot
	if (part->staintexnum >= 0 && cl_decals.integer) { // BLOODLY
		VectorCopy(part->velpart, v);
		VectorNormalize(v);
		staintex = part->staintexnum;
		R_DecalSystem_SplatEntities(part->org, v, 1-part->staincolor[0]*(1.0f/255.0f), 1-part->staincolor[1]*(1.0f/255.0f), 1-part->staincolor[2]*(1.0f/255.0f), part->stainalpha*(1.0f/255.0f), particletexture[staintex].s1, particletexture[staintex].t1, particletexture[staintex].s2, particletexture[staintex].t2, part->stainsize);
	}

	// blood creates a splash at spawn, not just at impact, this makes monsters bloody where they are shot
	if (part->ptypeindex == pt_blood && cl_decals.integer /*d:1*/) {  // BLOODLY Baker: No PAIRED_PARTICLE_REMOVE_MASK_127 to avoid double blood
		VectorCopy(part->velpart, v);
		VectorNormalize(v);
		staintex = tex_blooddecal[rand()&7];
		R_DecalSystem_SplatEntities(part->org, v, part->color[0]*(1.0f/255.0f), part->color[1]*(1.0f/255.0f), part->color[2]*(1.0f/255.0f), part->alpha*(1.0f/255.0f), particletexture[staintex].s1, particletexture[staintex].t1, particletexture[staintex].s2, particletexture[staintex].t2, part->size * 2);
	}
}

void CL_SpawnDecalParticleForSurface(int hitent, const vec3_t org, const vec3_t normal, int color1, int color2, int texnum, float size, float alpha)
{
	int l1, l2;
	entity_render_t *ent = &cl.entities[hitent].render;
	unsigned char color[3];
	if (!cl_decals.integer)
		return;
	if (!ent->allowdecals) // BLOODLY
		return;

	l2 = (int)lhrandom(0.5, 256.5);
	l1 = 256 - l2;
	color[0] = ((((color1 >> 16) & 0xFF) * l1 + ((color2 >> 16) & 0xFF) * l2) >> 8) & 0xFF;
	color[1] = ((((color1 >>  8) & 0xFF) * l1 + ((color2 >>  8) & 0xFF) * l2) >> 8) & 0xFF;
	color[2] = ((((color1 >>  0) & 0xFF) * l1 + ((color2 >>  0) & 0xFF) * l2) >> 8) & 0xFF;

	if (vid.sRGB3D)
		R_DecalSystem_SplatEntities(org, normal, Image_LinearFloatFromsRGB(color[0]), Image_LinearFloatFromsRGB(color[1]), Image_LinearFloatFromsRGB(color[2]), alpha*(1.0f/255.0f), particletexture[texnum].s1, particletexture[texnum].t1, particletexture[texnum].s2, particletexture[texnum].t2, size);
	else
		R_DecalSystem_SplatEntities(org, normal, color[0]*(1.0f/255.0f), color[1]*(1.0f/255.0f), color[2]*(1.0f/255.0f), alpha*(1.0f/255.0f), particletexture[texnum].s1, particletexture[texnum].t1, particletexture[texnum].s2, particletexture[texnum].t2, size);
}

void CL_SpawnDecalParticleForPoint(const vec3_t org, float maxdist, float size, float alpha, int texnum, int color1, int color2)
{
	int i;
	vec_t bestfrac;
	vec3_t bestorg;
	vec3_t bestnormal;
	vec3_t org2;
	int besthitent = 0, hitent;
	trace_t trace;
	bestfrac = 10;
	// Baker: do 32 random org2 vectors
	// Keep the shortest distance one.
	for (i = 0;i < 32;i++)
	{
		VectorRandom(org2);
		VectorMA(org, maxdist, org2, org2);
		trace = CL_TraceLine(org, org2, MOVE_NOMONSTERS_1, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_SKY, 0, 0, collision_extendmovelength.value, true, false, &hitent, false, true);
		// take the closest trace result that doesn't end up hitting a NOMARKS
		// surface (sky for example)
		if (bestfrac > trace.fraction && !(trace.hitq3surfaceflags & Q3SURFACEFLAG_NOMARKS))
		{
			bestfrac = trace.fraction;
			besthitent = hitent;
			VectorCopy(trace.endpos, bestorg);
			VectorCopy(trace.plane.normal, bestnormal);
		}
	}
	if (bestfrac < 1)
		CL_SpawnDecalParticleForSurface(besthitent, bestorg, bestnormal, color1, color2, texnum, size, alpha);
}

// generates a cubemap name with prefix flags based on info flags (for now only `!`)
static char *LightCubemapNumToName(char *vabuf, size_t vasize, int lightcubemapnum, int flags)
{
	if (lightcubemapnum <= 0)
		return NULL;
	// `!` is prepended if the cubemap must be nearest-filtered
	if (Have_Flag (flags, PARTICLEEFFECT_FORCENEAREST_4))
		return va(vabuf, vasize, "!cubemaps/%d", lightcubemapnum); // Baker: Notice the "!" EXCLAIM
	return va(vabuf, vasize, "cubemaps/%d", lightcubemapnum);
}

static void CL_Sparks(const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, float sparkcount);
static void CL_Smoke(const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, float smokecount);
static void CL_NewParticlesFromEffectinfo(int effectnameindex, float pcount, const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, entity_t *ent, int palettecolor, qbool spawndlight, qbool spawnparticles, float tintmins[4], float tintmaxs[4], float fade, qbool wanttrail);


RELATED_ (CL_ParticleEffect_Fallback)
#include "cl_particles_fallback_svc.c.h"

// this is also called on point effects with spawndlight = true and
// spawnparticles = true
RELATED_ (VM_CL_pointparticles CL_ParticleEffect CL_ParticleBox CL_NewParticlesFromEffectinfo CL_NewParticle)
static void CL_NewParticlesFromEffectinfo(int effectnameindex, float pcount, const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, entity_t *ent, int palettecolor, qbool spawndlight, qbool spawnparticles, float tintmins[4], float tintmaxs[4], float fade, qbool wanttrail)
{
	qbool found = false;
	char vabuf[1024];

#if 1 // April 14 2025 - Hemebond particle when paused fix.
	if (cl.time <= cl.oldtime)
		return;
#endif

	if (effectnameindex < 1 || effectnameindex >= MAX_PARTICLEEFFECTNAME_4096 || !particleeffectname[effectnameindex][0]) {
		Con_DPrintLinef ("Unknown effect number %d received from server", effectnameindex);
		return; // no such effect
	}

	if (!cl_particles_quake.integer /*d:0*/ && particleeffectinfo[0].effectnameindex) {
		// Baker: This is the norm

		//int supercontents;
		//vec_t traillen;	// Baker: a float
		vec_t trailstep; // Baker a float
		//qbool underwater;
		float avgtint[4], tint[4], tintlerp;
		// note this runs multiple effects with the same name, each one spawns only one kind of particle, so some effects need more than one
		vec3_t center;
		VectorLerp(originmins, 0.5, originmaxs, center);
		int supercontents = CL_PointSuperContents(center);
		qbool underwater = (supercontents & (SUPERCONTENTS_WATER | SUPERCONTENTS_SLIME)) != 0;

		vec3_t traildir;
		VectorSubtract(originmaxs, originmins, traildir);
		vec_t traillen = VectorLength(traildir); // Baker: a float
		VectorNormalize(traildir);
		if (tintmins) {
			Vector4Lerp(tintmins, 0.5, tintmaxs, avgtint); // Baker: VM_CL_pointparticles never does this, this is SVC_PARTICLE dark_red_73
		}
		else {
			Vector4Set(avgtint, 1, 1, 1, 1);
		}

		particleeffectinfo_t *info;
		int effectinfoindex;
		int tex, staintex;

		vec3_t trailpos;
		vec3_t rvec;
		vec3_t angles;
		vec3_t velocity;
		vec3_t forward;
		vec3_t right;
		vec3_t up;
		qbool immediatebloodstain;
		particle_t *part;

		for (effectinfoindex = 0, info = particleeffectinfo;
			effectinfoindex < MAX_PARTICLEEFFECTINFO_8192 && info->effectnameindex;
			effectinfoindex ++, info ++) {
			//if ((info->effectnameindex == effectnameindex) && (info->flags & PARTICLEEFFECT_DEFINED)) {
			if (info->effectnameindex != effectnameindex || !Have_Flag (info->einfo_flags, PARTICLEEFFECT_DEFINED))
				continue;

			qbool definedastrail = info->trailspacing > 0;

			qbool drawastrail = wanttrail;
			if (cl_particles_forcetraileffects.integer /*d:0*/)
				drawastrail = drawastrail || definedastrail;

			found = true;
			if (Have_Flag (info->einfo_flags, PARTICLEEFFECT_UNDERWATER_1) && !underwater)
				continue;
			if (Have_Flag (info->einfo_flags, PARTICLEEFFECT_NOTUNDERWATER_2) && underwater)
				continue;

			// spawn a dlight if requested
			if (info->lightradiusstart > 0 && spawndlight) {
				matrix4x4_t tempmatrix;
				if (drawastrail)
					Matrix4x4_CreateTranslate(&tempmatrix, originmaxs[0], originmaxs[1], originmaxs[2]);
				else
					Matrix4x4_CreateTranslate(&tempmatrix, center[0], center[1], center[2]);
				//if (info->lighttime > 0 || info->lightradiusfade > 0) { // Baker: hemebond fix. May 23 2025
				if (info->lighttime > 0 && info->lightradiusfade > 0) { // Baker: hemebond idea reverted, causes issues :(
					// light flash (explosion, etc)
					// called when effect starts
					CL_AllocLightFlash(NULL, &tempmatrix, info->lightradiusstart, info->lightcolor[0]*avgtint[0]*avgtint[3], info->lightcolor[1]*avgtint[1]*avgtint[3], info->lightcolor[2]*avgtint[2]*avgtint[3], info->lightradiusfade, info->lighttime, LightCubemapNumToName(vabuf, sizeof(vabuf), info->lightcubemapnum, info->einfo_flags), -1, info->lightshadow, info->lightcorona[0], info->lightcorona[1], 0, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
				}
				else if (r_refdef.scene.numlights < MAX_DLIGHTS_256) {
					// glowing entity
					// called by CL_LinkNetworkEntity
					Matrix4x4_Scale(&tempmatrix, info->lightradiusstart, 1);
					rvec[0] = info->lightcolor[0]*avgtint[0]*avgtint[3];
					rvec[1] = info->lightcolor[1]*avgtint[1]*avgtint[3];
					rvec[2] = info->lightcolor[2]*avgtint[2]*avgtint[3];
					R_RTLight_Update(&r_refdef.scene.templights[r_refdef.scene.numlights], false, &tempmatrix, rvec, -1, LightCubemapNumToName(vabuf, sizeof(vabuf), info->lightcubemapnum, info->einfo_flags), info->lightshadow, info->lightcorona[0], info->lightcorona[1], 0, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
					r_refdef.scene.lights[r_refdef.scene.numlights] = &r_refdef.scene.templights[r_refdef.scene.numlights];r_refdef.scene.numlights++;
				}
			}

			if (!spawnparticles)
				continue;

			// spawn particles - BAKER: TRAIL PARTICLES HERE.  April 20 2025
			tex = info->tex[0];
			if (info->tex[1] > info->tex[0]) {
				tex = (int)lhrandom(info->tex[0], info->tex[1]);
				tex = min(tex, info->tex[1] - 1);
			}
			if (info->staintex[0] < 0)
				staintex = info->staintex[0];
			else {
				staintex = (int)lhrandom(info->staintex[0], info->staintex[1]);
				staintex = min(staintex, info->staintex[1] - 1);
			}
// DECAL
			if (info->particletype == pt_decal) {
				VectorMAM(0.5f, velocitymins, 0.5f, velocitymaxs, velocity); // VELA2
				AnglesFromVectors(angles, velocity, NULL, false);
				AngleVectors(angles, forward, right, up);
				VectorMAMAMAM(1.0f, center, info->relativeoriginoffset[0], forward, info->relativeoriginoffset[1], right, info->relativeoriginoffset[2], up, trailpos);

				CL_SpawnDecalParticleForPoint(trailpos, info->originjitter[0], lhrandom(info->size[0], info->size[1]), lhrandom(info->alpha[0], info->alpha[1])*avgtint[3], tex, info->color[0], info->color[1]);
			}
// BEAM
			else if (info->orientation == PORIENT_HBEAM) {
				if (!drawastrail)
					continue;

				AnglesFromVectors(angles, traildir, NULL, false);
				AngleVectors(angles, forward, right, up);
				VectorMAMAM(info->relativeoriginoffset[0], forward, info->relativeoriginoffset[1], right, info->relativeoriginoffset[2], up, trailpos);

				CL_NewParticle /*HBEAM*/ (center, info->particletype, info->color[0], info->color[1], tex, lhrandom(info->size[0], info->size[1]), info->size[2], lhrandom(info->alpha[0], info->alpha[1]), info->alpha[2], 0, 0, originmins[0] + trailpos[0], originmins[1] + trailpos[1], originmins[2] + trailpos[2], originmaxs[0], originmaxs[1], originmaxs[2], 0, 0, 0, 0, false, lhrandom(info->time[0], info->time[1]), info->stretchfactor, info->blendmode, info->orientation, info->staincolor[0], info->staincolor[1], staintex, lhrandom(info->stainalpha[0], info->stainalpha[1]), lhrandom(info->stainsize[0], info->stainsize[1]), 0, 0, tintmins ? avgtint : NULL);
			}
			else {
				// Baker: NOT HBEAM and not DECAL
				float cnt;
				if (!cl_particles.integer /*d:1*/)
					continue;
				switch (info->particletype)
				{
				case pt_smoke:
					if (!cl_particles_smoke.integer)
						continue;
					break;
				case pt_spark: if (!cl_particles_sparks.integer) continue;break;
				case pt_bubble: if (!cl_particles_bubbles.integer) continue;break;
				case pt_blood: if (!cl_particles_blood.integer) continue;break;
				case pt_rain: if (!cl_particles_rain.integer) continue;break;
				case pt_snow: if (!cl_particles_snow.integer) continue;break;
				default: break;
				}

				cnt = info->countabsolute;
				cnt += (pcount * info->countmultiplier) * cl_particles_quality.value;
				// if drawastrail is not set, we will
				// use the regular cnt-based random
				// particle spawning at the center; so
				// do NOT apply trailspacing then!
				if (drawastrail && definedastrail)
					cnt += (traillen / info->trailspacing) * cl_particles_quality.value;
				cnt *= fade;
				if (cnt == 0)
					continue;  // nothing to draw
				info->particleaccumulator += cnt;

				if (drawastrail || definedastrail)
					immediatebloodstain = false;
				else
					immediatebloodstain =
						((cl_decals_newsystem_immediatebloodstain.integer /*d:2*/ >= 1)
						    && (info->particletype == pt_blood))
						||
						((cl_decals_newsystem_immediatebloodstain.integer >= 2) && staintex);

				if (drawastrail) {
					VectorCopy(originmins, trailpos);
					trailstep = traillen / cnt;
				}
				else { // !drawastrail
					VectorCopy(center, trailpos); // VELA2 - PTSPARK HERE
					trailstep = 0;
				}

				if (trailstep == 0) {
					VectorMAM(0.5f, velocitymins, 0.5f, velocitymaxs, velocity);  // VELA2 - PTSPARK HERE
					AnglesFromVectors(angles, velocity, NULL, false);
				}
				else // !trailstep ..
					AnglesFromVectors(angles, traildir, NULL, false);

				AngleVectors(angles, forward, right, up);
				VectorMAMAMAM(1.0f, trailpos, info->relativeoriginoffset[0], forward, info->relativeoriginoffset[1], right, info->relativeoriginoffset[2], up, trailpos);
				VectorMAMAM(info->relativevelocityoffset[0], forward, info->relativevelocityoffset[1], right, info->relativevelocityoffset[2], up, velocity);
				// Baker: Number of particles to spawn ...
				// countabsolute + requestedcount * countmultiplier * quality
				info->particleaccumulator = bound(0, info->particleaccumulator, 16384);
				for (;info->particleaccumulator >= 1;info->particleaccumulator--) {
					if (info->tex[1] > info->tex[0]) {
						tex = (int)lhrandom(info->tex[0], info->tex[1]);
						tex = min(tex, info->tex[1] - 1);
					}
					if (!(drawastrail || definedastrail)) {
						trailpos[0] = lhrandom(originmins[0], originmaxs[0]);
						trailpos[1] = lhrandom(originmins[1], originmaxs[1]);
						trailpos[2] = lhrandom(originmins[2], originmaxs[2]);
					}
					if (tintmins) { // Baker: SVC_PARTICLE dark_red_73, pointparticles is always NULL
						tintlerp = lhrandom(0, 1);
						Vector4Lerp(tintmins, tintlerp, tintmaxs, tint); // Baker: tint calculated
					}

					VectorRandom(rvec); // Baker: Multipled by originjitter for CL_NewParticle origin
					RELATED_ (baselineparticleeffectinfo)
					part = CL_NewParticle /*CHUMBO*/ (
						center,
						info->particletype,
						info->color[0], info->color[1],
						tex,											// Tex
						lhrandom(info->size[0], info->size[1]),			// Size
						info->size[2],									// Size increase
						lhrandom(info->alpha[0], info->alpha[1]),
						info->alpha[2],
						info->gravity,
						info->bounce,
						trailpos[0] + info->originoffset[0] + info->originjitter[0] * rvec[0], // px
						trailpos[1] + info->originoffset[1] + info->originjitter[1] * rvec[1], // py
						trailpos[2] + info->originoffset[2] + info->originjitter[2] * rvec[2], // pz

						// Velocity
						lhrandom(velocitymins[0], velocitymaxs[0]) * info->velocitymultiplier + info->velocityoffset[0] + info->velocityjitter[0] * rvec[0] + velocity[0],
						lhrandom(velocitymins[1], velocitymaxs[1]) * info->velocitymultiplier + info->velocityoffset[1] + info->velocityjitter[1] * rvec[1] + velocity[1],
						lhrandom(velocitymins[2], velocitymaxs[2]) * info->velocitymultiplier + info->velocityoffset[2] + info->velocityjitter[2] * rvec[2] + velocity[2],
						info->airfriction,
						info->liquidfriction,
						0, // origin jitter
						0, // velocity jitter
						info->countabsolute <= 0,
						lhrandom(info->time[0], info->time[1]), // lifetime
						info->stretchfactor,
						info->blendmode,
						info->orientation,
						info->staincolor[0],
						info->staincolor[1],
						staintex,
						lhrandom(info->stainalpha[0], info->stainalpha[1]),
						lhrandom(info->stainsize[0], info->stainsize[1]),
						lhrandom(info->rotate[0], info->rotate[1]), // "angle"
						lhrandom(info->rotate[2], info->rotate[3]), // "spin"
						tintmins ? tint : NULL
					); // VELA2 PT_SPARK
					//if (part && Have_Flag (info->einfo_flags, PARTICLEEFFECT_TEX2_ACTIVE_8) ) {
					if (part && info->b.tex[0] >= 0) {
						// Baker: Synchronized random.

						if (info->b.tex[1] > info->b.tex[0]) {
							int tex1_rnd_index = tex - info->tex[0]; // Index chosen above (0,1,2 ...)
							//tex = (int)lhrandom(info->b.tex[0], info->b.tex[1]);
							tex = info->b.tex[0] + tex1_rnd_index;
							tex = min(tex, info->b.tex[1] - 1);
						} else {
							tex = info->b.tex[0];
						}

						particle_t *part2 __ATTRIBUTE_USED__ = CL_NewParticle /*CHUMBO2*/ (
							part->sortorigin,								// (A) center,
							part->ptypeindex | PAIRED_PARTICLE_FLAG_128,	// (S) info->particletype

							info->b.color[0], info->b.color[1],				// (B 0)
							tex,											// (B 1) Reused variable

							// Baker: If texture2
							// Need to default the 7 properties in info-> definition time
							// When does it default?  At the end of the read for any not read
							// Make it happen
							lhrandom(info->b.size[0], info->b.size[1]),		// (B 2) Size
							info->b.size[2],								// (B 2) Size increase
							lhrandom(info->b.alpha[0], info->b.alpha[1]),	// (B 3)
							info->b.alpha[2],								// (B 3)
							info->gravity,									// (S)
							info->bounce,									// (S)
							part->org[0],									// (A) px
							part->org[1],									// (A) py
							part->org[2],									// (A) pz

							// Velocity
							part->velpart[0],								// (A) pvx
							part->velpart[1],								// (A) pvy
							part->velpart[2],								// (A) pvy
							info->airfriction,								// (S)
							info->liquidfriction,							// (S)
							0, // origin jitter								// (S)
							0, // velocity jitter							// (S)
							info->countabsolute <= 0,						// (S) qual reduction
							lhrandom(info->b.time[0], info->b.time[1]),		// (B 4) lifetime
							info->stretchfactor,							// (S) stretch
							info->b.blendmode,								// (B 5) blend
							info->b.orientation,							// (B 6) orient

							// NO STAIN
							(unsigned int)-1 /*unsigned int staincolor0*/,	// (NONE) info->staincolor[0],
							(unsigned int)-1 /*unsigned int staincolor1*/,	// (NONE) info->staincolor[1],
							-1 /*int staintex*/,							// (NONE) no stain texture
							1.0f, 											// (NONE) float stainalpha
							1.0f, 											// (NONE) float stainsize
							part->pangle,									// (A) "angle"
							part->pspin,									// (A) "spin"
							tintmins ? tint : NULL							// (S) Use same value
						); // VELA2 PT_SPARK

					} // PARTICLEEFFECT_TEX2_ACTIVE_8
					if (part && immediatebloodstain) {
						immediatebloodstain = false;
						CL_ImmediateBloodStain(part);
					}
					if (trailstep)
						VectorMA(trailpos, trailstep, traildir, trailpos);
				} // for particle accumulator
			} // NOT HBEAM and not DECAL

		} // for each effectinfo
	} // !particles quake if block

	// Baker: This is where not found in effectinfo
	if (!found)
		CL_ParticleEffect_Fallback(effectnameindex, pcount, originmins, originmaxs, velocitymins, velocitymaxs, ent, palettecolor, spawndlight, spawnparticles, wanttrail);
}

#include "cl_particles_misc.c.h"

static void R_DrawParticle_TransparentCallback (const entity_render_t *ent,
 const rtlight_t *rtlight, int numsurfaces, int *surfacelist)
{
	vec3_t vecorg, vecvel, baseright, baseup;
	int surfacelistindex;
	int batchstart, batchcount;
	const particle_t *p;
	pblend_t blendmode;
	rtexture_t *texture;
	float *v3f, *t2f, *c4f;
	particletexture_t *tex;
	float up2[3], v[3], right[3], up[3], fog, ifog, size, len, lenfactor, alpha;
//	float ambient[3], diffuse[3], diffusenormal[3];
	float palpha, spintime, spinrad, spincos, spinsin, spinm1, spinm2, spinm3, spinm4;
	vec4_t colormultiplier;
	float minparticledist_start, minparticledist_end;
	qbool dofade;

	RSurf_ActiveModelEntity(r_refdef.scene.worldentity, false, false, false);

	Vector4Set(colormultiplier, r_refdef.view.colorscale * (1.0 / 256.0f), r_refdef.view.colorscale * (1.0 / 256.0f), r_refdef.view.colorscale * (1.0 / 256.0f), cl_particles_alpha.value * (1.0 / 256.0f));

	r_refdef.stats[r_stat_particles] += numsurfaces;
//	R_Mesh_ResetTextureState();
	GL_DepthMask(false);
	GL_DepthRange(0, 1);
	GL_PolygonOffset(0, 0);
	GL_DepthTest(true);
	GL_CullFace(GL_NONE);

	spintime = r_refdef.scene.time;

	minparticledist_start = DotProduct(r_refdef.view.origin, r_refdef.view.forward) + r_drawparticles_nearclip_min.value;
	minparticledist_end = DotProduct(r_refdef.view.origin, r_refdef.view.forward) + r_drawparticles_nearclip_max.value;
	dofade = (minparticledist_start < minparticledist_end);

	// first generate all the vertices at once
	for (
		surfacelistindex = 0, v3f = particle_vertex3f, t2f = particle_texcoord2f, c4f = particle_color4f;
		surfacelistindex < numsurfaces; // Baker: numsurfaces is function arg
		surfacelistindex++, v3f += 3*4, t2f += 2*4, c4f += 4*4) {


		p = cl.particles + surfacelist[surfacelistindex];

		blendmode = (pblend_t)p->pblendmode;
		palpha = p->alpha;
		//if (dofade && p->porientation != PARTICLE_VBEAM && p->porientation != PARTICLE_HBEAM)
		if (dofade && isin2 (p->porientation, PORIENT_VBEAM, PORIENT_HBEAM) == false)
			palpha *= min(1, (DotProduct(p->org, r_refdef.view.forward)  - minparticledist_start) / (minparticledist_end - minparticledist_start));
		alpha = palpha * colormultiplier[3];
		// ensure alpha multiplier saturates properly
		if (alpha > 1.0f)
			alpha = 1.0f;

		// Q: Where does the snow physics occur?
		RELATED_ (R_DrawParticles called by R_RenderScene)
		switch (blendmode) {
		case PBLEND_INVALID:
		case PBLEND_INVMOD:
			// additive and modulate can just fade out in fog (this is correct)
			if (r_refdef.fogenabled)
				alpha *= RSurf_FogVertex(p->org);
			// collapse alpha into color for these blends (so that the particlefont does not need alpha on most textures)
			alpha *= 1.0f / 256.0f;
			c4f[0] = p->color[0] * alpha;
			c4f[1] = p->color[1] * alpha;
			c4f[2] = p->color[2] * alpha;
			c4f[3] = 0;
			break;
		case PBLEND_ADD:
			// additive and modulate can just fade out in fog (this is correct)
			if (r_refdef.fogenabled)
				alpha *= RSurf_FogVertex(p->org);
			// collapse alpha into color for these blends (so that the particlefont does not need alpha on most textures)
			c4f[0] = p->color[0] * colormultiplier[0] * alpha;
			c4f[1] = p->color[1] * colormultiplier[1] * alpha;
			c4f[2] = p->color[2] * colormultiplier[2] * alpha;
			c4f[3] = 0;
			break;
		case PBLEND_ALPHA_0:
			c4f[0] = p->color[0] * colormultiplier[0];
			c4f[1] = p->color[1] * colormultiplier[1];
			c4f[2] = p->color[2] * colormultiplier[2];
			c4f[3] = alpha;
			// note: lighting is not cheap!
			if (particletype[p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127].lighting) {
				// Baker: This can never happen because .lighting is false for all particles
				// I did enabled it to try it and it works.
				float a[3], c[3], dir[3];
				vecorg[0] = p->org[0];
				vecorg[1] = p->org[1];
				vecorg[2] = p->org[2];
				R_CompleteLightPoint(a, c, dir, vecorg, LP_LIGHTMAP | LP_RTWORLD | LP_DYNLIGHT, r_refdef.scene.lightmapintensity, r_refdef.scene.ambientintensity);
				c4f[0] = p->color[0] * colormultiplier[0] * (a[0] + 0.25f * c[0]);
				c4f[1] = p->color[1] * colormultiplier[1] * (a[1] + 0.25f * c[1]);
				c4f[2] = p->color[2] * colormultiplier[2] * (a[2] + 0.25f * c[2]);
			}
			// mix in the fog color
			if (r_refdef.fogenabled) {
				fog = RSurf_FogVertex(p->org);
				ifog = 1 - fog;
				c4f[0] = c4f[0] * fog + r_refdef.fogcolor[0] * ifog;
				c4f[1] = c4f[1] * fog + r_refdef.fogcolor[1] * ifog;
				c4f[2] = c4f[2] * fog + r_refdef.fogcolor[2] * ifog;
			}
			// for premultiplied alpha we have to apply the alpha to the color (after fog of course)
			VectorScale(c4f, alpha, c4f);
			break;
		}
		// copy the color into the other three vertices
		Vector4Copy(c4f, c4f + 4);
		Vector4Copy(c4f, c4f + 8);
		Vector4Copy(c4f, c4f + 12);

		size = p->size * cl_particles_size.value /*d:1*/;
		// SPRITELY - August 2 2025
		tex = &particletexture[p->ptexnum];
		switch(p->porientation)
		{
//		case PARTICLE_INVALID:
		case PORIENT_BILLBOARD_0:
			if (p->pangle + p->pspin) {
				spinrad = (p->pangle + p->pspin * (spintime - p->delayedspawn)) * (float)(M_PI / 180.0f);
				spinsin = sin(spinrad) * size;
				spincos = cos(spinrad) * size;
				spinm1 = -p->stretch * spincos;
				spinm2 = -spinsin;
				spinm3 = spinsin;
				spinm4 = -p->stretch * spincos;
				VectorMAM(spinm1, r_refdef.view.left, spinm2, r_refdef.view.up, right);
				VectorMAM(spinm3, r_refdef.view.left, spinm4, r_refdef.view.up, up);
			}
			else
			{
				VectorScale(r_refdef.view.left, -size * p->stretch, right);
				VectorScale(r_refdef.view.up, size, up);
			}

			v3f[ 0] = p->org[0] - right[0] - up[0];
			v3f[ 1] = p->org[1] - right[1] - up[1];
			v3f[ 2] = p->org[2] - right[2] - up[2];
			v3f[ 3] = p->org[0] - right[0] + up[0];
			v3f[ 4] = p->org[1] - right[1] + up[1];
			v3f[ 5] = p->org[2] - right[2] + up[2];
			v3f[ 6] = p->org[0] + right[0] + up[0];
			v3f[ 7] = p->org[1] + right[1] + up[1];
			v3f[ 8] = p->org[2] + right[2] + up[2];
			v3f[ 9] = p->org[0] + right[0] - up[0];
			v3f[10] = p->org[1] + right[1] - up[1];
			v3f[11] = p->org[2] + right[2] - up[2];
			t2f[0] = tex->s1;t2f[1] = tex->t2;
			t2f[2] = tex->s1;t2f[3] = tex->t1;
			t2f[4] = tex->s2;t2f[5] = tex->t1;
			t2f[6] = tex->s2;t2f[7] = tex->t2;
			break;
		case PORIENT_UPRIGHT:
			if (p->pangle + p->pspin) {
				spinrad = (p->pangle + p->pspin * (spintime - p->delayedspawn)) * (float)(M_PI / 180.0f);
				spinsin = sin(spinrad) * size;
				spincos = cos(spinrad) * size;
				spinm1 = -p->stretch * spincos;
				spinm2 = -spinsin;
				spinm3 = spinsin;
				spinm4 = -p->stretch * spincos;
				VectorMAM(spinm1, r_refdef.view.left, spinm2, r_refdef.view.up, right);
				VectorMAM(spinm3, r_refdef.view.left, spinm4, r_refdef.view.up, up);
			}
			else
			{
				VectorScale(r_refdef.view.left, -size * p->stretch, right);
				VectorScale(r_refdef.view.up, size, up);
			}

			v3f[ 0] = p->org[0] - right[0] - up[0];
			v3f[ 1] = p->org[1] - right[1] - up[1];
			v3f[ 2] = p->org[2] - right[2] - up[2];
			v3f[ 3] = p->org[0] - right[0] + up[0];
			v3f[ 4] = p->org[1] - right[1] + up[1];
			v3f[ 5] = p->org[2] - right[2] + up[2];
			v3f[ 6] = p->org[0] + right[0] + up[0];
			v3f[ 7] = p->org[1] + right[1] + up[1];
			v3f[ 8] = p->org[2] + right[2] + up[2];
			v3f[ 9] = p->org[0] + right[0] - up[0];
			v3f[10] = p->org[1] + right[1] - up[1];
			v3f[11] = p->org[2] + right[2] - up[2];
#if 0
			t2f[0] = tex->s1;t2f[1] = tex->t2; // LT
			t2f[2] = tex->s1;t2f[3] = tex->t1; // RT
			t2f[4] = tex->s2;t2f[5] = tex->t1; // RB
			t2f[6] = tex->s2;t2f[7] = tex->t2; // LB
#endif
#if 0 // 180
			t2f[0] = tex->s2;t2f[1] = tex->t2; // LT
			t2f[2] = tex->s2;t2f[3] = tex->t1; // RT
			t2f[4] = tex->s1;t2f[5] = tex->t1; // RB
			t2f[6] = tex->s1;t2f[7] = tex->t2; // LB
#endif
			t2f[0] = tex->s1;t2f[1] = tex->t1; // LT
			t2f[2] = tex->s1;t2f[3] = tex->t2; // RT
			t2f[4] = tex->s2;t2f[5] = tex->t2; // RB
			t2f[6] = tex->s2;t2f[7] = tex->t1; // LB

			break;
		case PORIENT_ORIENTED_DOUBLESIDED: // Baker: decals
			vecvel[0] = p->velpart[0];
			vecvel[1] = p->velpart[1];
			vecvel[2] = p->velpart[2];
			VectorVectors(vecvel, baseright, baseup);
			if (p->pangle + p->pspin) {
				spinrad = (p->pangle + p->pspin * (spintime - p->delayedspawn)) * (float)(M_PI / 180.0f);
				spinsin = sin(spinrad) * size;
				spincos = cos(spinrad) * size;
				spinm1 = p->stretch * spincos;
				spinm2 = -spinsin;
				spinm3 = spinsin;
				spinm4 = p->stretch * spincos;
				VectorMAM(spinm1, baseright, spinm2, baseup, right);
				VectorMAM(spinm3, baseright, spinm4, baseup, up);
			}
			else
			{
				VectorScale(baseright, size * p->stretch, right);
				VectorScale(baseup, size, up);
			}
			v3f[ 0] = p->org[0] - right[0] - up[0];
			v3f[ 1] = p->org[1] - right[1] - up[1];
			v3f[ 2] = p->org[2] - right[2] - up[2];
			v3f[ 3] = p->org[0] - right[0] + up[0];
			v3f[ 4] = p->org[1] - right[1] + up[1];
			v3f[ 5] = p->org[2] - right[2] + up[2];
			v3f[ 6] = p->org[0] + right[0] + up[0];
			v3f[ 7] = p->org[1] + right[1] + up[1];
			v3f[ 8] = p->org[2] + right[2] + up[2];
			v3f[ 9] = p->org[0] + right[0] - up[0];
			v3f[10] = p->org[1] + right[1] - up[1];
			v3f[11] = p->org[2] + right[2] - up[2];
			t2f[0] = tex->s1;t2f[1] = tex->t2;
			t2f[2] = tex->s1;t2f[3] = tex->t1;
			t2f[4] = tex->s2;t2f[5] = tex->t1;
			t2f[6] = tex->s2;t2f[7] = tex->t2;
			break;
		case PORIENT_SPARK:
			len = VectorLength(p->velpart);
			VectorNormalize2(p->velpart, up);
			lenfactor = p->stretch * 0.04 * len;
			if (lenfactor < size * 0.5)
				lenfactor = size * 0.5;
			VectorMA(p->org, -lenfactor, up, v);
			VectorMA(p->org,  lenfactor, up, up2);
			R_CalcBeam_Vertex3f(v3f, v, up2, size);
			t2f[0] = tex->s1;t2f[1] = tex->t2;
			t2f[2] = tex->s1;t2f[3] = tex->t1;
			t2f[4] = tex->s2;t2f[5] = tex->t1;
			t2f[6] = tex->s2;t2f[7] = tex->t2;
			break;
		case PORIENT_VBEAM:
			R_CalcBeam_Vertex3f(v3f, p->org, p->velpart, size);
			VectorSubtract(p->velpart, p->org, up);
			VectorNormalize(up);
			v[0] = DotProduct(p->org, up) * (1.0f / 64.0f) * p->stretch;
			v[1] = DotProduct(p->velpart, up) * (1.0f / 64.0f) * p->stretch;
			t2f[0] = tex->s2;t2f[1] = v[0];
			t2f[2] = tex->s1;t2f[3] = v[0];
			t2f[4] = tex->s1;t2f[5] = v[1];
			t2f[6] = tex->s2;t2f[7] = v[1];
			break;
		case PORIENT_HBEAM:
			R_CalcBeam_Vertex3f(v3f, p->org, p->velpart, size);
			VectorSubtract(p->velpart, p->org, up);
			VectorNormalize(up);
			v[0] = DotProduct(p->org, up) * (1.0f / 64.0f) * p->stretch;
			v[1] = DotProduct(p->velpart, up) * (1.0f / 64.0f) * p->stretch;
			t2f[0] = v[0];t2f[1] = tex->t1;
			t2f[2] = v[0];t2f[3] = tex->t2;
			t2f[4] = v[1];t2f[5] = tex->t2;
			t2f[6] = v[1];t2f[7] = tex->t1;
			break;
		}
		if (r_showparticleedges.integer /*d:0*/) {
			R_DebugLine(v3f, v3f + 3);
			R_DebugLine(v3f + 3, v3f + 6);
			R_DebugLine(v3f + 6, v3f + 9);
			R_DebugLine(v3f + 9, v3f);
		}
	}

	// now render batches of particles based on blendmode and texture
	blendmode = PBLEND_INVALID;
	texture = NULL;
	batchstart = 0;
	batchcount = 0;
	R_Mesh_PrepareVertices_Generic_Arrays (
		numsurfaces * 4,
		particle_vertex3f,
		particle_color4f,
		particle_texcoord2f
	);
	for (surfacelistindex = 0;surfacelistindex < numsurfaces;) {
		p = cl.particles + surfacelist[surfacelistindex];

		if (texture != particletexture[p->ptexnum].texture)
		{
			texture = particletexture[p->ptexnum].texture;
			R_SetupShader_Generic(texture, false, false, false);
		}

		if (p->pblendmode == PBLEND_INVMOD) {
			// inverse modulate blend - group these
			GL_BlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
			// iterate until we find a change in settings
			batchstart = surfacelistindex ++;
			for (;surfacelistindex < numsurfaces;surfacelistindex++) {
				p = cl.particles + surfacelist[surfacelistindex];
				if (p->pblendmode != PBLEND_INVMOD || texture != particletexture[p->ptexnum].texture)
					break;
			}
		}
		else
		{
			// additive or alpha blend - group these
			// (we can group these because we premultiplied the texture alpha)
			GL_BlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			// iterate until we find a change in settings
			batchstart = surfacelistindex++;
			for (;surfacelistindex < numsurfaces;surfacelistindex++)
			{
				p = cl.particles + surfacelist[surfacelistindex];
				if (p->pblendmode == PBLEND_INVMOD || texture != particletexture[p->ptexnum].texture)
					break;
			}
		}

		// Baker:
		batchcount = surfacelistindex - batchstart;
		R_Mesh_Draw(			// Baker:
			batchstart * 4,		// firstvertex
			batchcount * 4,		// numvertices
			batchstart * 2,		// firsttriangle
			batchcount * 2,		// numtriangles
			NULL,				// element3i
			NULL,				// element3i_indexbuffer
			0,					// element3i_bufferoffset
			particle_elements,	// element3s (array of unsigned shorts)
			NULL,				// element3s_indexbuffer
			0					// element3s_bufferoffset
		);
	}
}

#include "cl_particles_r_drawparticles_physics.c.h"

RELATED_ (CL_Particles_ParseEffectInfo)

void CL_Particles_Init (void)
{
	Cmd_AddCommand (CF_CLIENT, "cl_effectslevel", CL_EffectsLevel_f, "Set effects level (1 - 6) [Zircon]");
	Cmd_AddCommand (CF_CLIENT, "pointfile", CL_ReadPointFile_f, "display point file produced by qbsp when a leak was detected in the map (a line leading through the leak hole, to an entity inside the level)");
	Cmd_AddCommand (CF_CLIENT, "cl_particles_reloadeffects", CL_Particles_LoadEffectInfo_f, "reloads effectinfo.txt and maps/levelname_effectinfo.txt (where levelname is the current map) if parameter is given, loads from custom file (no levelname_effectinfo are loaded in this case)");
	Cmd_AddCommand (CF_CLIENT, "effectinfo_dump", CL_EffectInfo_Dump_f, "dumps text of effect, loading text fresh from file effectinfo.txt  [Zircon]");
	Cmd_AddCommand (CF_CLIENT, "effectinfo_list", CL_EffectInfo_List_f, "displays effectinfo names loaded in client memory even without a map [Zircon]"); // Particles loaded in Render_InitOnce
	Cmd_AddCommand (CF_CLIENT, "effectinfo_particleeffectnum", CL_EffectInfo_particleeffectnum, "particleeffectnum for effectname [Zircon]"); // Particles loaded in Render_InitOnce
	Cmd_AddCommand (CF_CLIENT, "particles_split", CL_Particles_Split_f, "Particle font to particles/images [Zircon]"); // Particles loaded in Render_InitOnce
	Cmd_AddCommand (CF_CLIENT, "particles_count", CL_Particles_Count_f, "Particle count [Zircon]"); // Particles loaded in Render_InitOnce
	Cmd_AddCommand (CF_CLIENT, "image_cols_rows_split", CL_Image_Cols_Rows_Split_f, "Split image imagepath/imagename/000.png - divides image into specified rows and columns .. [Zircon]"); // Particles loaded in Render_InitOnce
	Cmd_AddCommand (CF_CLIENT, "particles_join", CL_Particles_Join_f, "Particle images in particles/images to particles/particlefont.tga [Zircon]"); // Particles loaded in Render_InitOnce

	Cvar_RegisterVariable (&cl_particles);
	Cvar_RegisterVariable (&cl_particles_quality);
	Cvar_RegisterVariable (&cl_particles_alpha);
	Cvar_RegisterVariable (&cl_particles_size);
	Cvar_RegisterVariable (&cl_particles_quake);
	Cvar_RegisterVariable (&cl_particles_blood);
	Cvar_RegisterVariable (&cl_particles_blood_alpha);
	Cvar_RegisterVariable (&cl_particles_blood_decal_alpha);
	Cvar_RegisterVariable (&cl_particles_blood_decal_scalemin);
	Cvar_RegisterVariable (&cl_particles_blood_decal_scalemax);
	Cvar_RegisterVariable (&cl_particles_blood_bloodhack);
	Cvar_RegisterVariable (&cl_particles_explosions_sparks);
	Cvar_RegisterVariable (&cl_particles_explosions_shell);
	Cvar_RegisterVariable (&cl_particles_bulletimpacts);
	Cvar_RegisterVariable (&cl_particles_rain);
	Cvar_RegisterVariable (&cl_particles_snow);
	Cvar_RegisterVariable (&cl_particles_smoke);
	Cvar_RegisterVariable (&cl_particles_smoke_alpha);
	Cvar_RegisterVariable (&cl_particles_smoke_alphafade);
	Cvar_RegisterVariable (&cl_particles_sparks);
	Cvar_RegisterVariable (&cl_particles_bubbles);
	Cvar_RegisterVariable (&cl_particles_visculling);
	Cvar_RegisterVariable (&cl_particles_collisions);
	Cvar_RegisterVariable (&cl_particles_forcetraileffects);
	Cvar_RegisterVariable (&cl_particles_stain_mix_with_particle_color); // STAIN-EON
	Cvar_RegisterVariable (&cl_decals);
	Cvar_RegisterVariable (&cl_decals_time);
	Cvar_RegisterVariable (&cl_decals_fadetime);
	Cvar_RegisterVariable (&cl_decals_newsystem_intensitymultiplier);
	Cvar_RegisterVariable (&cl_decals_newsystem_immediatebloodstain);
	Cvar_RegisterVariable (&cl_decals_newsystem_bloodsmears);
	Cvar_RegisterVariable (&cl_decals_models);
	Cvar_RegisterVariable (&cl_decals_bias);
	Cvar_RegisterVariable (&cl_decals_max);
}
