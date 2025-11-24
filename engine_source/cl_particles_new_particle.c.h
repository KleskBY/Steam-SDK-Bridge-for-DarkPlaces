// cl_particles_new_particle.c.h

/* SUPERBOSS*/
particle_t *CL_NewParticle(const vec3_t sortorigin, unsigned short ptypeindex,
   int pcolor1, int pcolor2,
   int ptex, float psize, float psizeincrease, float palpha, float palphafade,
   float pgravity, float pbounce, float px, float py, float pz,
   float pvx, float pvy, float pvz, float pairfriction,
   float pliquidfriction, float originjitter, float velocityjitter,
   qbool pqualityreduction, float lifetime, float stretch, pblend_t blendmode,
   porientation_t orientation, int staincolor1, int staincolor2,
   int staintex, float stainalpha, float stainsize, float angle, float spin, float tint[4]
)
{
	int l1, l2, r, g, b;
	particle_t *part;
	vec3_t v;
	if (!cl_particles.integer)
		return NULL;

#if 1 // April 14 2025 - Hemebond particle when paused fix.
	if (cl.time <= cl.oldtime)
		return NULL;
#endif

//#ifdef _DEBUG // Total hackage
//	if (ptex == 128) {
//		CL_NewParticle (
//			sortorigin,
//			ptypeindex,
//			0x710193,
//
//			0x710193,
//			ptex + 1,		// Baker: Next texture always.
//			psize,
//			psizeincrease, palpha, palphafade,
//			pgravity, pbounce, px, py, pz,
//			pvx, pvy, pvz, pairfriction,
//			pliquidfriction, originjitter, velocityjitter,
//			pqualityreduction, lifetime, stretch,
//			blendmode,		// Baker: pblend_t PBLEND_ADD
//			orientation,
//			-1,				// Baker: staincolor1
//			-1,				// Baker: staincolor2
//			-1,				// Baker: staintex,
//			stainalpha, stainsize,
//			angle, spin, tint /*[4]*/
//		);
//	}
//#endif

	for (;cl.free_particle < cl.max_particles && cl.particles[cl.free_particle].ptypeindex; cl.free_particle ++);
	if (cl.free_particle >= cl.max_particles /*MAX_PARTICLES_1048576*/)
		return NULL;
	if (!lifetime)
		lifetime = palpha / min(1, palphafade);
	part = &cl.particles[cl.free_particle++];
	if (cl.num_particles < cl.free_particle)
		cl.num_particles = cl.free_particle;
	memset(part, 0, sizeof(*part));
	VectorCopy(sortorigin, part->sortorigin);
	part->ptypeindex = ptypeindex; // Baker: pt_static, pt_smoke, ...
	part->pblendmode = blendmode;
	if (isin2 (orientation, PORIENT_HBEAM, PORIENT_VBEAM))
	{
		particletexture_t *tex = &particletexture[ptex];
		if (tex->t1 == 0 && tex->t2 == 1) // full height of texture?
			part->porientation = PORIENT_VBEAM;
		else
			part->porientation = PORIENT_HBEAM;
	}
	else
		part->porientation = orientation;// VELA2 orient PARTICLE_SPARK
	l2 = (int)lhrandom(0.5, 256.5);
	l1 = 256 - l2;
	part->color[0] = ((((pcolor1 >> 16) & 0xFF) * l1 + ((pcolor2 >> 16) & 0xFF) * l2) >> 8) & 0xFF;
	part->color[1] = ((((pcolor1 >>  8) & 0xFF) * l1 + ((pcolor2 >>  8) & 0xFF) * l2) >> 8) & 0xFF;
	part->color[2] = ((((pcolor1 >>  0) & 0xFF) * l1 + ((pcolor2 >>  0) & 0xFF) * l2) >> 8) & 0xFF;
	if (vid.sRGB3D)
	{
		part->color[0] = (unsigned char)floor(Image_LinearFloatFromsRGB(part->color[0]) * 255.0f + 0.5f);
		part->color[1] = (unsigned char)floor(Image_LinearFloatFromsRGB(part->color[1]) * 255.0f + 0.5f);
		part->color[2] = (unsigned char)floor(Image_LinearFloatFromsRGB(part->color[2]) * 255.0f + 0.5f);
	}
	part->alpha = palpha;
	part->alphafade = palphafade;
	part->staintexnum = staintex;
#if 1 // Baker: July 31 2025 - Allow particles to not mix based on the main color.  STAIN-EON
	if (!cl_particles_stain_mix_with_particle_color.integer && staincolor1 >= 0 && staincolor2 >= 0) { // STAIN-EON
		l2 = (int)lhrandom(0.5, 256.5);
		l1 = 256 - l2;
		if (blendmode == PBLEND_INVMOD)
		{
			r = ((((staincolor1 >> 16) & 0xFF) * l1 + ((staincolor2 >> 16) & 0xFF) * l2) * 255) / 0x8000; // staincolor 0x808080 keeps color invariant
			g = ((((staincolor1 >>  8) & 0xFF) * l1 + ((staincolor2 >>  8) & 0xFF) * l2) * 255) / 0x8000;
			b = ((((staincolor1 >>  0) & 0xFF) * l1 + ((staincolor2 >>  0) & 0xFF) * l2) * 255) / 0x8000;
		}
		else
		{
			r = ((((staincolor1 >> 16) & 0xFF) * l1 + ((staincolor2 >> 16) & 0xFF) * l2) * 255) / 0x8000; // staincolor 0x808080 keeps color invariant
			g = ((((staincolor1 >>  8) & 0xFF) * l1 + ((staincolor2 >>  8) & 0xFF) * l2) * 255) / 0x8000;
			b = ((((staincolor1 >>  0) & 0xFF) * l1 + ((staincolor2 >>  0) & 0xFF) * l2) * 255) / 0x8000;
		}
		if (r > 0xFF) r = 0xFF;
		if (g > 0xFF) g = 0xFF;
		if (b > 0xFF) b = 0xFF;
	}
#endif // 1 - DarkPlaces default stain mixing with particle color
	else
	if (staincolor1 >= 0 && staincolor2 >= 0)
	{
		l2 = (int)lhrandom(0.5, 256.5);
		l1 = 256 - l2;
		if (blendmode == PBLEND_INVMOD)
		{
			r = ((((staincolor1 >> 16) & 0xFF) * l1 + ((staincolor2 >> 16) & 0xFF) * l2) * (255 - part->color[0])) / 0x8000; // staincolor 0x808080 keeps color invariant
			g = ((((staincolor1 >>  8) & 0xFF) * l1 + ((staincolor2 >>  8) & 0xFF) * l2) * (255 - part->color[1])) / 0x8000;
			b = ((((staincolor1 >>  0) & 0xFF) * l1 + ((staincolor2 >>  0) & 0xFF) * l2) * (255 - part->color[2])) / 0x8000;
		}
		else
		{
			r = ((((staincolor1 >> 16) & 0xFF) * l1 + ((staincolor2 >> 16) & 0xFF) * l2) * part->color[0]) / 0x8000; // staincolor 0x808080 keeps color invariant
			g = ((((staincolor1 >>  8) & 0xFF) * l1 + ((staincolor2 >>  8) & 0xFF) * l2) * part->color[1]) / 0x8000;
			b = ((((staincolor1 >>  0) & 0xFF) * l1 + ((staincolor2 >>  0) & 0xFF) * l2) * part->color[2]) / 0x8000;
		}
		if (r > 0xFF) r = 0xFF;
		if (g > 0xFF) g = 0xFF;
		if (b > 0xFF) b = 0xFF;
	}
	else
	{
		r = part->color[0]; // -1 is shorthand for stain = particle color
		g = part->color[1];
		b = part->color[2];
	}
	part->staincolor[0] = r;
	part->staincolor[1] = g;
	part->staincolor[2] = b;
	part->stainalpha = palpha * stainalpha;
	part->stainsize = psize * stainsize;
	if (tint)
	{
		if (blendmode != PBLEND_INVMOD) // invmod is immune to tinting
		{
			part->color[0] *= tint[0];
			part->color[1] *= tint[1];
			part->color[2] *= tint[2];
		}
		part->alpha *= tint[3];
		part->alphafade *= tint[3];
		part->stainalpha *= tint[3];
	}
	part->ptexnum = ptex; /* FLAGGED ASSIGN TEX2_ACTION */
	part->size = psize;
	part->sizeincrease = psizeincrease;
	part->gravity = pgravity;
	part->bounce = pbounce;
	part->stretch = stretch;
	VectorRandom(v); // Baker: Only for origin jitter and velocity jitter
	part->org[0] = px + originjitter * v[0];
	part->org[1] = py + originjitter * v[1];
	part->org[2] = pz + originjitter * v[2];
	part->velpart[0] = pvx + velocityjitter * v[0]; // VELA2 SPARK VELOCITY (SMOKE COMES HERE TOO)
	part->velpart[1] = pvy + velocityjitter * v[1];
	part->velpart[2] = pvz + velocityjitter * v[2];
	part->time2 = 0;
	part->airfriction = pairfriction;
	part->liquidfriction = pliquidfriction;
	part->die = cl.time + lifetime;
	part->delayedspawn = cl.time;
//	part->delayedcollisions = 0;
	part->qualityreduction = pqualityreduction;
	part->pangle = angle;
	part->pspin = spin;
	// if it is rain or snow, trace ahead and shut off collisions until an actual collision event needs to occur to improve performance
	if (part->ptypeindex == pt_rain) { // Baker: No PAIRED_PARTICLE_REMOVE_MASK_127 to avoid 2x rain
		int i;
		particle_t *part2;
		vec3_t endvec;
		trace_t trace;
		// turn raindrop into simple spark and create delayedspawn splash effect
		part->ptypeindex = pt_spark;
		part->bounce = 0;
		VectorMA(part->org, lifetime, part->velpart, endvec);

		// Baker: Here is the rain splash pre-calc
		trace = CL_TraceLine(part->org, endvec, MOVE_NOMONSTERS_1, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_LIQUIDSMASK, 0, 0, collision_extendmovelength.value, true, false, NULL, false, false);

		part->die = cl.time + lifetime * trace.fraction;

#if 0 // def _DEBUG
		// Baker: Are we acculumating useless particles?
		if (part->die >= 16777216.0f) {
			int j = 5;
			goto baker_early_out;
		}
#endif

		part2 = CL_NewParticle(endvec, pt_raindecal, pcolor1, pcolor2, tex_rainsplash,
			part->size, part->size * 20, part->alpha, part->alpha / 0.4, 0, 0,
			trace.endpos[0] + trace.plane.normal[0], trace.endpos[1] + trace.plane.normal[1], trace.endpos[2] +
			trace.plane.normal[2], trace.plane.normal[0], trace.plane.normal[1], trace.plane.normal[2],
			0, 0, 0, 0, pqualityreduction, 0, 1, PBLEND_ADD, PORIENT_ORIENTED_DOUBLESIDED, -1, -1, -1, 1, 1,
			0, 0, NULL);
		if (part2) {
			part2->delayedspawn = part->die;
			part2->die += part->die - cl.time;
			for (i = rand() & 7;i < 10;i++)
			{
				part2 = CL_NewParticle(endvec, pt_spark, pcolor1, pcolor2, tex_particle, 0.25f, 0, part->alpha * 2, part->alpha * 4, 1, 0.1, trace.endpos[0] + trace.plane.normal[0], trace.endpos[1] + trace.plane.normal[1], trace.endpos[2] + trace.plane.normal[2], trace.plane.normal[0] * 16, trace.plane.normal[1] * 16, trace.plane.normal[2] * 16 + cl.movevars_gravity * 0.04, 0, 0, 0, 32, pqualityreduction, 0, 1, PBLEND_ADD, PORIENT_SPARK, -1, -1, -1, 1, 1, 0, 0, NULL);
				if (part2)
				{
					part2->delayedspawn = part->die;
					part2->die += part->die - cl.time;
				}
			}
		}
	}
baker_early_out:
#if 0
	else if (part->bounce != 0 && part->gravity == 0 && part->typeindex != pt_snow)
	{
		float lifetime = part->alpha / (part->alphafade ? part->alphafade : 1);
		vec3_t endvec;
		trace_t trace;
		VectorMA(part->org, lifetime, part->vel, endvec);
		trace = CL_TraceLine(part->org, endvec, MOVE_NOMONSTERS_1, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY, true, false, NULL, false);
		part->delayedcollisions = cl.time + lifetime * trace.fraction - 0.1;
	}
#endif

	return part;
}
