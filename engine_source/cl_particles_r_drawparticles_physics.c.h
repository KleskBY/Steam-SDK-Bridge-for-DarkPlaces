// cl_particles_r_drawparticles_physics.c.h

CALLERS_ (/*Exclusive*/ R_RenderScene)
void R_DrawParticles (void)
{
	int a;
	int drawparticles = r_drawparticles.integer;
	float minparticledist_start;
	
	float gravity, /*my_frametime,*/ f, dist, oldorg[3], decaldir[3];
	float drawdist2;
	int hitent;
	trace_t trace;
	qbool update;
#if 0 // Baker May 31 2024 - From DarkPlaces Beta
	float pt_explode_frame_interval, pt_explode2_frame_interval;
	int color;
#endif

	// Baker: 
	const float my_frametime = bound(0, cl.time - cl.particles_updatetime1, 1); // Baker: Added const
	cl.particles_updatetime1 = bound(cl.time - 1, cl.particles_updatetime1 + my_frametime, cl.time + 1);
	// Baker: cl.particles_updatetime1 is not referenced below ...

	// LadyHavoc: early out conditions
	if (!cl.num_particles)
		return;

#if 0 // Baker May 31 2024 - From DarkPlaces Beta
	// Handling of the colour ramp for pt_explode and pt_explode2
	pt_explode_frame_interval = my_frametime * 10;
	pt_explode2_frame_interval = my_frametime * 15;
#endif
	minparticledist_start = DotProduct(r_refdef.view.origin, r_refdef.view.forward) + r_drawparticles_nearclip_min.value /*d:4*/;
	gravity = my_frametime * cl.movevars_gravity;
	update = my_frametime > 0;
	drawdist2 = r_drawparticles_drawdistance.value /*d:2000*/ * r_refdef.view.quality;
	drawdist2 = drawdist2*drawdist2;

	particle_t *p;
	int particle_idx;
	
	for (particle_idx = 0, p = cl.particles; particle_idx < cl.num_particles; particle_idx ++, p ++) {
		if (!p->ptypeindex) { // Baker: No need to check PAIRED_PARTICLE_REMOVE_MASK_127
			if (cl.free_particle > particle_idx)
				cl.free_particle = particle_idx;
			continue;
		}

		if (update) { // Baker: my_frametime > 0
			if (p->delayedspawn > cl.time)
				continue;

			p->size += p->sizeincrease * my_frametime;
			p->alpha -= p->alphafade * my_frametime;

			if (p->alpha <= 0 || p->die <= cl.time)
				goto killparticle;

			//if (p->porientation != PORIENT_VBEAM && p->porientation != PORIENT_HBEAM && my_frametime > 0)
			if (isin2 (p->porientation, PORIENT_VBEAM, PORIENT_HBEAM) == false && my_frametime > 0) {
// Baker: NOT BEAM - This is the norm
				if (p->liquidfriction && cl_particles_collisions.integer /*d:1*/ && 
					(CL_PointSuperContents(p->org) & SUPERCONTENTS_LIQUIDSMASK)) {
					if ((p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127) == pt_blood)
						p->size += my_frametime * 8;
					else
						p->velpart[2] -= p->gravity * gravity;
					f = 1.0f - min(p->liquidfriction * my_frametime, 1);
					VectorScale(p->velpart, f, p->velpart);
				}
				else {
					p->velpart[2] -= p->gravity * gravity;
					if (p->airfriction)
					{
						f = 1.0f - min(p->airfriction * my_frametime, 1); // Baker: VELA2 this is what gravity happens?
						VectorScale(p->velpart, f, p->velpart);
					}
				} // if ! liquidfriction

				VectorCopy(p->org, oldorg);
				VectorMA(p->org, my_frametime, p->velpart, p->org);
//				if (p->bounce && cl.time >= p->delayedcollisions)
				if (p->bounce && cl_particles_collisions.integer /*d:1*/ && VectorLength(p->velpart)) {
					trace = CL_TraceLine(oldorg, p->org, MOVE_NORMAL_0, NULL, SUPERCONTENTS_SOLID | 
						(((p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127) == pt_rain || 
							(p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127) == pt_snow) ? 
								SUPERCONTENTS_LIQUIDSMASK : 0), 0, 0, collision_extendmovelength.value, true, false, &hitent, false, false);
					// if the trace started in or hit something of SUPERCONTENTS_NODROP
					// or if the trace hit something flagged as NOIMPACT
					// then remove the particle
					if (trace.hitq3surfaceflags & Q3SURFACEFLAG_NOIMPACT || ((trace.startsupercontents | trace.hitsupercontents) & SUPERCONTENTS_NODROP) || (trace.startsupercontents & SUPERCONTENTS_SOLID))
						goto killparticle;
					VectorCopy(trace.endpos, p->org);
					// react if the particle hit something
					if (trace.fraction < 1) {
						VectorCopy(trace.endpos, p->org);

						if (p->staintexnum >= 0) {
							// blood - splash on solid
							if (!(trace.hitq3surfaceflags & Q3SURFACEFLAG_NOMARKS))
							{
								R_Stain(p->org, 16,
									p->staincolor[0], p->staincolor[1], p->staincolor[2], (int)(p->stainalpha * p->stainsize * (1.0f / 160.0f)),
									p->staincolor[0], p->staincolor[1], p->staincolor[2], (int)(p->stainalpha * p->stainsize * (1.0f / 160.0f)));
								if (cl_decals.integer)
								{
									// create a decal for the blood splat
									a = 0xFFFFFF ^ (p->staincolor[0]*65536+p->staincolor[1]*256+p->staincolor[2]);
									if (cl_decals_newsystem_bloodsmears.integer /*d:1*/)
									{
										VectorCopy(p->velpart, decaldir);
										VectorNormalize(decaldir);
									}
									else
										VectorCopy(trace.plane.normal, decaldir);
									CL_SpawnDecalParticleForSurface(hitent, p->org, decaldir, a, a, p->staintexnum, p->stainsize, p->stainalpha); // staincolor needs to be inverted for decals!
								}
							}
						}

						if ((p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127) == pt_blood) {
							// blood - splash on solid
							if (trace.hitq3surfaceflags & Q3SURFACEFLAG_NOMARKS)
								goto killparticle;
							if (p->staintexnum == -1) { // staintex < -1 means no stains at all
								R_Stain(p->org, 16, 64, 16, 16, (int)(p->alpha * p->size * (1.0f / 80.0f)), 64, 32, 32, (int)(p->alpha * p->size * (1.0f / 80.0f)));
								if (cl_decals.integer) {
									// create a decal for the blood splat
									if (cl_decals_newsystem_bloodsmears.integer /*d:1*/) {
										VectorCopy(p->velpart, decaldir);
										VectorNormalize(decaldir);
									}
									else
										VectorCopy(trace.plane.normal, decaldir);
									CL_SpawnDecalParticleForSurface(hitent, p->org, decaldir, p->color[0] * 65536 + p->color[1] * 256 + p->color[2], p->color[0] * 65536 + p->color[1] * 256 + p->color[2], tex_blooddecal[rand()&7], p->size * lhrandom(cl_particles_blood_decal_scalemin.value, cl_particles_blood_decal_scalemax.value), cl_particles_blood_decal_alpha.value * 768);
								} // if decals
							} // if staintex
							goto killparticle; // END PT_BLOOD
						} else if (p->bounce < 0) {
							// bounce -1 means remove on impact
							goto killparticle;
						}
						else {
							// anything else - bounce off solid
							dist = DotProduct(p->velpart, trace.plane.normal) * -p->bounce;
							VectorMA(p->velpart, dist, trace.plane.normal, p->velpart);
						}
					}
				}// if bounce

				if (VectorLength2(p->velpart) < 0.03) {
					if (p->porientation == PORIENT_SPARK) // sparks are virtually invisible if very slow, so rather let them go off
						goto killparticle;
					VectorClear(p->velpart);
				} // if veclen2
			} // if not beam

			if ((p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127) != pt_static) {
// Baker: NOT PT_STATIC
				switch ((p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127)) {
				case pt_entityparticle:
					// particle that removes itself after one rendered frame
					if (p->time2)
						goto killparticle;
					else
						p->time2 = 1;
					break;
				case pt_blood:
					a = CL_PointSuperContents(p->org);
					if (a & (SUPERCONTENTS_SOLID | SUPERCONTENTS_LAVA | SUPERCONTENTS_NODROP))
						goto killparticle;
					break;
				case pt_bubble:
					a = CL_PointSuperContents(p->org);
					if (!(a & (SUPERCONTENTS_WATER | SUPERCONTENTS_SLIME)))
						goto killparticle;
					break;
				case pt_rain:
					a = CL_PointSuperContents(p->org);
					if (a & (SUPERCONTENTS_SOLID | SUPERCONTENTS_LIQUIDSMASK))
						goto killparticle;
					break;
				case pt_snow:
#if 1 // Baker: Paired particle snow
					
					if (particle_idx && Have_Flag (p->ptypeindex, PAIRED_PARTICLE_FLAG_128)) {// && p->typeindex == pt_snow) {
						particle_t *pbef = cl.particles + (particle_idx - 1);
						p->time2		= pbef->time2;
						p->velpart[0]	= pbef->velpart[0];
						p->velpart[1]	= pbef->velpart[1];
					} else
#endif
					if (cl.time > p->time2) {
						// snow flutter
						p->time2 = cl.time + (rand() & 3) * 0.1;
						p->velpart[0] = p->velpart[0] * 0.9f + lhrandom(-32, 32);
						p->velpart[1] = p->velpart[0] * 0.9f + lhrandom(-32, 32);
					}
					a = CL_PointSuperContents(p->org);
					if (a & (SUPERCONTENTS_SOLID | SUPERCONTENTS_LIQUIDSMASK))
						goto killparticle;
					break;

				default:
					break;
				} // sw
			} // if NOT EQ pt_static
		} // IF UPDATE
		else if (p->delayedspawn > cl.time)
			continue;
		if (!drawparticles)
			continue;
		// don't render particles too close to the view (they chew fillrate)
		// also don't render particles behind the view (useless)
		// further checks to cull to the frustum would be too slow here
		switch((p->ptypeindex & PAIRED_PARTICLE_REMOVE_MASK_127)) {
		case pt_beam:
			// beams have no culling
			R_MeshQueue_AddTransparent(TRANSPARENTSORT_DISTANCE, p->sortorigin, R_DrawParticle_TransparentCallback, NULL, particle_idx, NULL);
			break;
		default:
			if (cl_particles_visculling.integer /*d:0*/) {
				// Baker: This is NOT the norm ... no vis culling on particles with default settings
				if (!r_refdef.viewcache.world_novis) {
					if (r_refdef.scene.worldmodel && r_refdef.scene.worldmodel->brush.PointInLeaf)
					{
						mleaf_t *leaf = r_refdef.scene.worldmodel->brush.PointInLeaf(r_refdef.scene.worldmodel, p->org);
						if (leaf)
							if (!CHECKPVSBIT(r_refdef.viewcache.world_pvsbits, leaf->clusterindex))
								continue;
					}
				}
			} // Baker: Vis culling

			// anything else just has to be in front of the viewer and visible at this distance
			if (!r_refdef.view.useperspective || (DotProduct(p->org, r_refdef.view.forward) >= minparticledist_start && VectorDistance2(p->org, r_refdef.view.origin) < drawdist2 * (p->size * p->size)))
				R_MeshQueue_AddTransparent(TRANSPARENTSORT_DISTANCE, p->sortorigin, R_DrawParticle_TransparentCallback, NULL, particle_idx, NULL);
			break;
		} // SW

		continue;
killparticle:
		p->ptypeindex = 0;
		if (cl.free_particle > particle_idx)
			cl.free_particle = particle_idx;
	} // for

	// reduce cl.num_particles if possible
	while (cl.num_particles > 0 && cl.particles[cl.num_particles - 1].ptypeindex == 0)
		cl.num_particles--;

	if (cl.num_particles == cl.max_particles && cl.max_particles < MAX_PARTICLES_1048576) {
		particle_t *oldparticles = cl.particles;
		cl.max_particles = min(cl.max_particles * 2, MAX_PARTICLES_1048576);
		cl.particles = (particle_t *) Mem_Alloc(cls.levelmempool, cl.max_particles * sizeof(particle_t));
		memcpy(cl.particles, oldparticles, cl.num_particles * sizeof(particle_t));
		Mem_Free(oldparticles);
	}
}

