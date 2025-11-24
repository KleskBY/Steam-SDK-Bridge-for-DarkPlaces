// cl_particles_baseline.c.h


RELATED_ (PARTICLEEFFECT_NOTUNDERWATER_2)

particleeffectinfo_t baselineparticleeffectinfo =
{
	0, //int effectnameindex; // which effect this belongs to
	// PARTICLEEFFECT_* bits
	0, //int flags;
	// blood effects may spawn very few particles, so proper fraction-overflow
	// handling is very important, this variable keeps track of the fraction
	0.0, //double particleaccumulator;
	// the math is: countabsolute + requestedcount * countmultiplier * quality
	// absolute number of particles to spawn, often used for decals
	// (unaffected by quality and requestedcount)
	0.0f, //float countabsolute;
	// multiplier for the number of particles CL_ParticleEffect was told to
	// spawn, most effects do not really have a count and hence use 1, so
	// this is often the actual count to spawn, not merely a multiplier
	0.0f, //float countmultiplier;
	// if > 0 this causes the particle to spawn in an evenly spaced line from
	// originmins to originmaxs (causing them to describe a trail, not a box)
	0.0f, //float trailspacing;
	// type of particle to spawn (defines some aspects of behavior)
	pt_alphastatic, //ptype_e particletype;
	// blending mode used on this particle type
	PBLEND_ALPHA_0, //pblend_t blendmode;
	// orientation of this particle type (BILLBOARD, SPARK, BEAM, etc)
	PORIENT_BILLBOARD_0, //porientation_t orientation;
	// range of colors to choose from in hex RRGGBB (like HTML color tags),
	// randomly interpolated at spawn
	{0xFFFFFF, 0xFFFFFF}, //unsigned int color[2];
	// a random texture is chosen in this range (note the second value is one
	// past the last choosable, so for example 8,16 chooses any from 8 up and
	// including 15)
	// if start and end of the range are the same, no randomization is done
	{63, 63 /* tex_particle */}, //int tex[2];
	// range of size values randomly chosen when spawning, plus size increase over time
	{1, 1, 0.0f}, //float size[3];
	// range of alpha values randomly chosen when spawning, plus alpha fade
	{0.0f, 256.0f, 256.0f}, //float alpha[3];
	// how long the particle should live (note it is also removed if alpha drops to 0)
	{16777216.0f, 16777216.0f}, //float time[2]; // Baker: Default particle time
	{
		PBLEND_ALPHA_0, //pblend_t blendmode;
		// orientation of this particle type (BILLBOARD, SPARK, BEAM, etc)
		PORIENT_BILLBOARD_0, //porientation_t orientation;
		// range of colors to choose from in hex RRGGBB (like HTML color tags),
		// randomly interpolated at spawn
		{0xFFFFFF, 0xFFFFFF}, //unsigned int color[2];
		// a random texture is chosen in this range (note the second value is one
		// past the last choosable, so for example 8,16 chooses any from 8 up and
		// including 15)
		// if start and end of the range are the same, no randomization is done
		{-1, -1 /* tex_particle */}, //int tex[2];
		// range of size values randomly chosen when spawning, plus size increase over time
		{1, 1, 0.0f}, //float size[3];
		// range of alpha values randomly chosen when spawning, plus alpha fade
		{0.0f, 256.0f, 256.0f}, //float alpha[3];
		// how long the particle should live (note it is also removed if alpha drops to 0)
		{16777216.0f, 16777216.0f}, //float time[2]; // Baker: Default particle time
	},
	// how much gravity affects this particle (negative makes it fly up!)
	0.0f, //float gravity;
	// how much bounce the particle has when it hits a surface
	// if negative the particle is removed on impact
	0.0f, //float bounce;
	// if in air this friction is applied
	// if negative the particle accelerates
	0.0f, //float airfriction;
	// if in liquid (water/slime/lava) this friction is applied
	// if negative the particle accelerates
	0.0f, //float liquidfriction;
	// these offsets are added to the values given to particleeffect(), and
	// then an ellipsoid-shaped jitter is added as defined by these
	// (they are the 3 radii)
	1.0f, //float stretchfactor;
	// stretch velocity factor (used for sparks)
	{0.0f, 0.0f, 0.0f}, //float originoffset[3];
	{0.0f, 0.0f, 0.0f}, //float relativeoriginoffset[3];
	{0.0f, 0.0f, 0.0f}, //float velocityoffset[3];
	{0.0f, 0.0f, 0.0f}, //float relativevelocityoffset[3];
	{0.0f, 0.0f, 0.0f}, //float originjitter[3];
	{0.0f, 0.0f, 0.0f}, //float velocityjitter[3];
	0.0f, //float velocitymultiplier;
	// an effect can also spawn a dlight
	0.0f, //float lightradiusstart;
	0.0f, //float lightradiusfade;
	16777216.0f, //float lighttime;
	{1.0f, 1.0f, 1.0f}, //float lightcolor[3];
	true, //qbool lightshadow;
	0, //int lightcubemapnum;
	{1.0f, 0.25f}, //float lightcorona[2];
	{(unsigned int)-1, (unsigned int)-1}, //unsigned int staincolor[2]; // note: 0x808080 = neutral (particle's own color), these are modding factors for the particle's original color!
	{-1, -1}, //int staintex[2];
	{1.0f, 1.0f}, //float stainalpha[2];
	{2.0f, 2.0f}, //float stainsize[2];
	// other parameters
	{0.0f, 360.0f, 0.0f, 0.0f}, //float rotate[4]; // min/max base angle, min/max rotation over time
};

