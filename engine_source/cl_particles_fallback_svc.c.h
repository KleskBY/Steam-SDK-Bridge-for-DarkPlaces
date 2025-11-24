// cl_particles_fallback_svc.c.h

static void CL_ParticleEffect_Fallback(int effectnameindex, float count, const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, entity_t *ent, int palettecolor, qbool spawndlight, qbool spawnparticles, qbool wanttrail)
{
	vec3_t center;
	matrix4x4_t lightmatrix;
	particle_t *part;

	VectorLerp(originmins, 0.5, originmaxs, center);
	Matrix4x4_CreateTranslate(&lightmatrix, center[0], center[1], center[2]);
	if (effectnameindex == EFFECT_SVC_PARTICLE) {
		if (cl_particles.integer /*d:1*/) {
			// bloodhack checks if this effect's color matches regular or lightning blood and if so spawns a blood effect instead
			if (count == 1024)
				CL_NewParticlesFromEffectinfo(EFFECT_TE_EXPLOSION, 1, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			else if (cl_particles_blood_bloodhack.integer /*d:1*/ && !cl_particles_quake.integer /*d:0*/ 
				// && (palettecolor == 73 || palettecolor == 225))
				&& (isin3 (palettecolor, 73, 75, 225)))
				CL_NewParticlesFromEffectinfo(EFFECT_TE_BLOOD, count / 2.0f, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			else
			{
				// Baker: August 6 2025 - tex_particle is usually 63 it seems
				count *= cl_particles_quality.value;
				for (;count > 0;count--)
				{
					int k = particlepalette[(palettecolor & ~7) + (rand()&7)];
					CL_NewParticle(center, pt_alphastatic, k, k, tex_particle, 1.5, 0, 255, 0, 0.15, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 0, 0, 8, 3, true, lhrandom(0.1, 0.4), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				}
			}
		}
	}
	else if (effectnameindex == EFFECT_TE_WIZSPIKE)
		CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 30*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 20, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
	else if (effectnameindex == EFFECT_TE_KNIGHTSPIKE)
		CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 20*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 226, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
	else if (effectnameindex == EFFECT_TE_SPIKE)
	{
		if (cl_particles_bulletimpacts.integer)
		{
			if (cl_particles_quake.integer)
			{
				if (cl_particles_smoke.integer)
					CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 10*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			}
			else
			{
				CL_Smoke(originmins, originmaxs, velocitymins, velocitymaxs, 4*count);
				CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, 15*count);
				CL_NewParticle(center, pt_static, 0x808080,0x808080, tex_particle, 3, 0, 256, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		// bullet hole
		R_Stain(center, 16, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 3, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
	}
	else if (effectnameindex == EFFECT_TE_SPIKEQUAD)
	{
		if (cl_particles_bulletimpacts.integer)
		{
			if (cl_particles_quake.integer)
			{
				if (cl_particles_smoke.integer)
					CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 10*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			}
			else
			{
				CL_Smoke(originmins, originmaxs, velocitymins, velocitymaxs, 4*count);
				CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, 15*count);
				CL_NewParticle(center, pt_static, 0x808080,0x808080, tex_particle, 3, 0, 256, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		// bullet hole
		R_Stain(center, 16, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 3, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
		CL_AllocLightFlash(NULL, &lightmatrix, 100, 0.15f, 0.15f, 1.5f, 500, 0.2, NULL, -1, true, 1, 0.25, 1, 0, 0, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_SUPERSPIKE)
	{
		if (cl_particles_bulletimpacts.integer)
		{
			if (cl_particles_quake.integer)
			{
				if (cl_particles_smoke.integer)
					CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 20*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			}
			else
			{
				CL_Smoke(originmins, originmaxs, velocitymins, velocitymaxs, 8*count);
				CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, 30*count);
				CL_NewParticle(center, pt_static, 0x808080,0x808080, tex_particle, 3, 0, 256, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		// bullet hole
		R_Stain(center, 16, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 3, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
	}
	else if (effectnameindex == EFFECT_TE_SUPERSPIKEQUAD)
	{
		if (cl_particles_bulletimpacts.integer)
		{
			if (cl_particles_quake.integer)
			{
				if (cl_particles_smoke.integer)
					CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 20*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			}
			else
			{
				CL_Smoke(originmins, originmaxs, velocitymins, velocitymaxs, 8*count);
				CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, 30*count);
				CL_NewParticle(center, pt_static, 0x808080,0x808080, tex_particle, 3, 0, 256, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		// bullet hole
		R_Stain(center, 16, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 3, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
		CL_AllocLightFlash(NULL, &lightmatrix, 100, 0.15f, 0.15f, 1.5f, 500, 0.2, NULL, -1, true, 1, 0.25, 1, 0, 0, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_BLOOD)
	{
		if (!cl_particles_blood.integer)
			return;
		if (cl_particles_quake.integer)
			CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 2*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 73, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
		else
		{
			static double bloodaccumulator = 0;
			qbool immediatebloodstain = (cl_decals_newsystem_immediatebloodstain.integer >= 1);
			//CL_NewParticle(center, pt_alphastatic, 0x4f0000,0x7f0000, tex_particle, 2.5, 0, 256, 256, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 1, 4, 0, 0, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, NULL);
			bloodaccumulator += count * 0.333 * cl_particles_quality.value;
			for (;bloodaccumulator > 0;bloodaccumulator--) {
				part = CL_NewParticle(center, pt_blood, 0xFFFFFF, 0xFFFFFF, tex_bloodparticle[rand()&7], 8, 0, cl_particles_blood_alpha.value * 768, cl_particles_blood_alpha.value * 384, 1, -1, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 1, 4, 0, 64, true, 0, 1, PBLEND_INVMOD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				if (immediatebloodstain && part) {
					immediatebloodstain = false;
					CL_ImmediateBloodStain(part);
				}
			} // for bloodaccumulator
		}
	} // End TE_BLOOD
	else if (effectnameindex == EFFECT_TE_SPARK)
		CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, count);
	else if (effectnameindex == EFFECT_TE_PLASMABURN)
	{
		// plasma scorch mark
		R_Stain(center, 40, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 6, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
		CL_AllocLightFlash(NULL, &lightmatrix, 200, 1, 1, 1, 1000, 0.2, NULL, -1, true, 1, 0.25, 1, 0, 0, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_GUNSHOT)
	{
		if (cl_particles_bulletimpacts.integer)
		{
			if (cl_particles_quake.integer)
				CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 20*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			else
			{
				CL_Smoke(originmins, originmaxs, velocitymins, velocitymaxs, 4*count);
				CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, 20*count);
				CL_NewParticle(center, pt_static, 0x808080,0x808080, tex_particle, 3, 0, 256, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		// bullet hole
		R_Stain(center, 16, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 3, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
	}
	else if (effectnameindex == EFFECT_TE_GUNSHOTQUAD)
	{
		if (cl_particles_bulletimpacts.integer)
		{
			if (cl_particles_quake.integer)
				CL_NewParticlesFromEffectinfo(EFFECT_SVC_PARTICLE, 20*count, originmins, originmaxs, velocitymins, velocitymaxs, NULL, 0, spawndlight, spawnparticles, NULL, NULL, 1, wanttrail);
			else
			{
				CL_Smoke(originmins, originmaxs, velocitymins, velocitymaxs, 4*count);
				CL_Sparks(originmins, originmaxs, velocitymins, velocitymaxs, 20*count);
				CL_NewParticle(center, pt_static, 0x808080,0x808080, tex_particle, 3, 0, 256, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		// bullet hole
		R_Stain(center, 16, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 3, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
		CL_AllocLightFlash(NULL, &lightmatrix, 100, 0.15f, 0.15f, 1.5f, 500, 0.2, NULL, -1, true, 1, 0.25, 1, 0, 0, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_EXPLOSION)
	{
		CL_ParticleExplosion(center);
		CL_AllocLightFlash(NULL, &lightmatrix, 350, 4.0f, 2.0f, 0.50f, 700, 0.5, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_EXPLOSIONQUAD)
	{
		CL_ParticleExplosion(center);
		CL_AllocLightFlash(NULL, &lightmatrix, 350, 2.5f, 2.0f, 4.0f, 700, 0.5, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_TAREXPLOSION)
	{
		if (cl_particles_quake.integer)
		{
			int i;
			for (i = 0;i < 1024 * cl_particles_quality.value;i++)
			{
				if (i & 1)
					CL_NewParticle(center, pt_alphastatic, particlepalette[66], particlepalette[71], tex_particle, 1.5f, 0, 255, 0, 0, 0, center[0], center[1], center[2], 0, 0, 0, -4, -4, 16, 256, true, (rand() & 1) ? 1.4 : 1.0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				else
					CL_NewParticle(center, pt_alphastatic, particlepalette[150], particlepalette[155], tex_particle, 1.5f, 0, 255, 0, 0, 0, center[0], center[1], center[2], 0, 0, lhrandom(-256, 256), 0, 0, 16, 0, true, (rand() & 1) ? 1.4 : 1.0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
		else
			CL_ParticleExplosion(center);
		CL_AllocLightFlash(NULL, &lightmatrix, 600, 1.6f, 0.8f, 2.0f, 1200, 0.5, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_SMALLFLASH)
		CL_AllocLightFlash(NULL, &lightmatrix, 200, 2, 2, 2, 1000, 0.2, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	else if (effectnameindex == EFFECT_TE_FLAMEJET)
	{
		count *= cl_particles_quality.value;
		while (count-- > 0)
			CL_NewParticle(center, pt_smoke, 0x6f0f00, 0xe3974f, tex_particle, 4, 0, lhrandom(64, 128), 384, -1, 1.1, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 1, 4, 0, 128, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
	}
	else if (effectnameindex == EFFECT_TE_LAVASPLASH)
	{
		float i, j, inc, vel;
		vec3_t dir, org;

		inc = 8 / cl_particles_quality.value;
		for (i = -128;i < 128;i += inc)
		{
			for (j = -128;j < 128;j += inc)
			{
				dir[0] = j + lhrandom(0, inc);
				dir[1] = i + lhrandom(0, inc);
				dir[2] = 256;
				org[0] = center[0] + dir[0];
				org[1] = center[1] + dir[1];
				org[2] = center[2] + lhrandom(0, 64);
				vel = lhrandom(50, 120) / VectorLength(dir); // normalize and scale
				CL_NewParticle(center, pt_alphastatic, particlepalette[224], particlepalette[231], tex_particle, 1.5f, 0, 255, 0, 0.05, 0, org[0], org[1], org[2], dir[0] * vel, dir[1] * vel, dir[2] * vel, 0, 0, 0, 0, true, lhrandom(2, 2.62), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
	}
	else if (effectnameindex == EFFECT_TE_TELEPORT)
	{
		float i, j, k, inc, vel;
		vec3_t dir;

		if (cl_particles_quake.integer)
			inc = 4 / cl_particles_quality.value;
		else
			inc = 8 / cl_particles_quality.value;
		for (i = -16;i < 16;i += inc)
		{
			for (j = -16;j < 16;j += inc)
			{
				for (k = -24;k < 32;k += inc)
				{
					VectorSet(dir, i*8, j*8, k*8);
					VectorNormalize(dir);
					vel = lhrandom(50, 113);
					if (cl_particles_quake.integer)
						CL_NewParticle(center, pt_alphastatic, particlepalette[7], particlepalette[14], tex_particle, 1.5f, 0, 255, 0, 0, 0, center[0] + i + lhrandom(0, inc), center[1] + j + lhrandom(0, inc), center[2] + k + lhrandom(0, inc), dir[0] * vel, dir[1] * vel, dir[2] * vel, 0, 0, 0, 0, true, lhrandom(0.2, 0.34), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					else
						CL_NewParticle(center, pt_alphastatic, particlepalette[7], particlepalette[14], tex_particle, 1.5f, 0, inc * lhrandom(37, 63), inc * 187, 0, 0, center[0] + i + lhrandom(0, inc), center[1] + j + lhrandom(0, inc), center[2] + k + lhrandom(0, inc), dir[0] * vel, dir[1] * vel, dir[2] * vel, 0, 0, 0, 0, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				}
			}
		}
		if (!cl_particles_quake.integer)
			CL_NewParticle(center, pt_static, 0xffffff, 0xffffff, tex_particle, 30, 0, 256, 512, 0, 0, center[0], center[1], center[2], 0, 0, 0, 0, 0, 0, 0, false, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		CL_AllocLightFlash(NULL, &lightmatrix, 200, 2.0f, 2.0f, 2.0f, 400, 99.0f, NULL, -1, true, 1, 0.25, 1, 0, 0, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_TEI_G3)
		CL_NewParticle(center, pt_beam, 0xFFFFFF, 0xFFFFFF, tex_beam, 8, 0, 256, 256, 0, 0, originmins[0], originmins[1], originmins[2], originmaxs[0], originmaxs[1], originmaxs[2], 0, 0, 0, 0, false, 0, 1, PBLEND_ADD, PORIENT_HBEAM, -1, -1, -1, 1, 1, 0, 0, NULL);
	else if (effectnameindex == EFFECT_TE_TEI_SMOKE)
	{
		if (cl_particles_smoke.integer)
		{
			count *= 0.25f * cl_particles_quality.value;
			while (count-- > 0)
				CL_NewParticle(center, pt_smoke, 0x202020, 0x404040, tex_smoke[rand()&7], 5, 0, 255, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 0, 0, 1.5f, 6.0f, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		}
	}
	else if (effectnameindex == EFFECT_TE_TEI_BIGEXPLOSION)
	{
		CL_ParticleExplosion(center);
		CL_AllocLightFlash(NULL, &lightmatrix, 500, 2.5f, 2.0f, 1.0f, 500, 9999, NULL, -1, true, 1, 0.25, 0.5, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_TE_TEI_PLASMAHIT)
	{
		float f;
		R_Stain(center, 40, 40, 40, 40, 64, 88, 88, 88, 64);
		CL_SpawnDecalParticleForPoint(center, 6, 8, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);
		if (cl_particles_smoke.integer)
			for (f = 0;f < count;f += 4.0f / cl_particles_quality.value)
				CL_NewParticle(center, pt_smoke, 0x202020, 0x404040, tex_smoke[rand()&7], 5, 0, 255, 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 0, 0, 20, 155, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		if (cl_particles_sparks.integer)
			for (f = 0;f < count;f += 1.0f / cl_particles_quality.value)
				CL_NewParticle(center, pt_spark, 0x2030FF, 0x80C0FF, tex_particle, 2.0f, 0, lhrandom(64, 255), 512, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 0, 0, 0, 465, true, 0, 1, PBLEND_ADD, PORIENT_SPARK, -1, -1, -1, 1, 1, 0, 0, NULL);
		CL_AllocLightFlash(NULL, &lightmatrix, 500, 0.6f, 1.2f, 2.0f, 2000, 9999, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_EF_FLAME)
	{
		if (!spawnparticles)
			count = 0;
		count *= 300 * cl_particles_quality.value;
		while (count-- > 0)
			CL_NewParticle(center, pt_smoke, 0x6f0f00, 0xe3974f, tex_particle, 4, 0, lhrandom(64, 128), 384, -1, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 1, 4, 16, 128, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		CL_AllocLightFlash(NULL, &lightmatrix, 200, 2.0f, 1.5f, 0.5f, 0, 0, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (effectnameindex == EFFECT_EF_STARDUST)
	{
		if (!spawnparticles)
			count = 0;
		count *= 200 * cl_particles_quality.value;
		while (count-- > 0)
			CL_NewParticle(center, pt_static, 0x903010, 0xFFD030, tex_particle, 4, 0, lhrandom(64, 128), 128, 1, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 0.2, 0.8, 16, 128, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		CL_AllocLightFlash(NULL, &lightmatrix, 200, 1.0f, 0.7f, 0.3f, 0, 0, NULL, -1, true, 1, 0.25, 0.25, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
	}
	else if (!strncmp(particleeffectname[effectnameindex], "TR_", 3))
	{
		vec3_t dir, pos;
		float len, dec, qd;
		int smoke, blood, bubbles, r, color, spawnedcount;

		if (spawndlight && r_refdef.scene.numlights < MAX_DLIGHTS_256)
		{
			vec4_t light;
			Vector4Set(light, 0, 0, 0, 0);

			if (effectnameindex == EFFECT_TR_ROCKET)
				Vector4Set(light, 3.0f, 1.5f, 0.5f, 200);
			else if (effectnameindex == EFFECT_TR_VORESPIKE)
			{
				if (gamemode == GAME_PRYDON && !cl_particles_quake.integer)
					Vector4Set(light, 0.3f, 0.6f, 1.2f, 100);
				else
					Vector4Set(light, 1.2f, 0.5f, 1.0f, 200);
			}
			else if (effectnameindex == EFFECT_TR_NEXUIZPLASMA)
				Vector4Set(light, 0.75f, 1.5f, 3.0f, 200);

			if (light[3])
			{
				matrix4x4_t traillightmatrix;
				Matrix4x4_CreateFromQuakeEntity(&traillightmatrix, originmaxs[0], originmaxs[1], originmaxs[2], 0, 0, 0, light[3]);
				R_RTLight_Update(&r_refdef.scene.templights[r_refdef.scene.numlights], false, &traillightmatrix, light, -1, NULL, true, 1, 0.25, 0, 1, 1, LIGHTFLAG_NORMALMODE | LIGHTFLAG_REALTIMEMODE);
				r_refdef.scene.lights[r_refdef.scene.numlights] = &r_refdef.scene.templights[r_refdef.scene.numlights];r_refdef.scene.numlights++;
			}
		}

		if (!spawnparticles)
			return;

		if (originmaxs[0] == originmins[0] && originmaxs[1] == originmins[1] && originmaxs[2] == originmins[2])
			return;

		VectorSubtract(originmaxs, originmins, dir);
		len = VectorNormalizeLength(dir);

		if (ent)
		{
			dec = -ent->persistent.trail_time;
			ent->persistent.trail_time += len;
			if (ent->persistent.trail_time < 0.01f)
				return;

			// if we skip out, leave it reset
			ent->persistent.trail_time = 0.0f;
		}
		else
			dec = 0;

		// advance into this frame to reach the first puff location
		VectorMA(originmins, dec, dir, pos);
		len -= dec;

		smoke = cl_particles.integer && cl_particles_smoke.integer;
		blood = cl_particles.integer && cl_particles_blood.integer;
		bubbles = cl_particles.integer && cl_particles_bubbles.integer && !cl_particles_quake.integer && (CL_PointSuperContents(pos) & (SUPERCONTENTS_WATER | SUPERCONTENTS_SLIME));
		qd = 1.0f / cl_particles_quality.value;
		spawnedcount = 0;

		while (len >= 0 && ++spawnedcount <= 16384)
		{
			dec = 3;
			if (blood)
			{
				if (effectnameindex == EFFECT_TR_BLOOD)
				{
					if (cl_particles_quake.integer)
					{
						color = particlepalette[67 + (rand()&3)];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0.25, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 3, 0, true, 2, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					else
					{
						dec = 16;
						CL_NewParticle(center, pt_blood, 0xFFFFFF, 0xFFFFFF, tex_bloodparticle[rand()&7], 8, 0, qd * cl_particles_blood_alpha.value * 768.0f, qd * cl_particles_blood_alpha.value * 384.0f, 1, -1, pos[0], pos[1], pos[2], lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 1, 4, 0, 64, true, 0, 1, PBLEND_INVMOD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
				}
				else if (effectnameindex == EFFECT_TR_SLIGHTBLOOD)
				{
					if (cl_particles_quake.integer)
					{
						dec = 6;
						color = particlepalette[67 + (rand()&3)];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0.25, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 3, 0, true, 2, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					else
					{
						dec = 32;
						CL_NewParticle(center, pt_blood, 0xFFFFFF, 0xFFFFFF, tex_bloodparticle[rand()&7], 8, 0, qd * cl_particles_blood_alpha.value * 768.0f, qd * cl_particles_blood_alpha.value * 384.0f, 1, -1, pos[0], pos[1], pos[2], lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 1, 4, 0, 64, true, 0, 1, PBLEND_INVMOD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
				}
			}
			if (smoke)
			{
				if (effectnameindex == EFFECT_TR_ROCKET)
				{
					if (cl_particles_quake.integer)
					{
						r = rand()&3;
						color = particlepalette[ramp3[r]];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, -0.10, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 3, 0, true, 0.1372549*(6-r), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					else
					{
						CL_NewParticle(center, pt_smoke, 0x303030, 0x606060, tex_smoke[rand()&7], 3, 0, cl_particles_smoke_alpha.value*62, cl_particles_smoke_alphafade.value*62, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
						CL_NewParticle(center, pt_static, 0x801010, 0xFFA020, tex_smoke[rand()&7], 3, 0, cl_particles_smoke_alpha.value*288, cl_particles_smoke_alphafade.value*1400, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 20, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
				}
				else if (effectnameindex == EFFECT_TR_GRENADE)
				{
					if (cl_particles_quake.integer)
					{
						r = 2 + (rand()%4);
						color = particlepalette[ramp3[r]];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, -0.15, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 3, 0, true, 0.1372549*(6-r), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					else
					{
						CL_NewParticle(center, pt_smoke, 0x303030, 0x606060, tex_smoke[rand()&7], 3, 0, cl_particles_smoke_alpha.value*50, cl_particles_smoke_alphafade.value*75, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
				}
				else if (effectnameindex == EFFECT_TR_WIZSPIKE)
				{
					if (cl_particles_quake.integer)
					{
						dec = 6;
						color = particlepalette[52 + (rand()&7)];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, pos[0], pos[1], pos[2], 30*dir[1], 30*-dir[0], 0, 0, 0, 0, 0, true, 0.5, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, pos[0], pos[1], pos[2], 30*-dir[1], 30*dir[0], 0, 0, 0, 0, 0, true, 0.5, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					//else if (gamemode == GAME_GOODVSBAD2)
					//{
					//	dec = 6;
					//	CL_NewParticle(center, pt_static, 0x00002E, 0x000030, tex_particle, 6, 0, 128, 384, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					//}
					else
					{
						color = particlepalette[20 + (rand()&7)];
						CL_NewParticle(center, pt_static, color, color, tex_particle, 2, 0, 64, 192, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
				}
				else if (effectnameindex == EFFECT_TR_KNIGHTSPIKE)
				{
					if (cl_particles_quake.integer)
					{
						dec = 6;
						color = particlepalette[230 + (rand()&7)];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, pos[0], pos[1], pos[2], 30*dir[1], 30*-dir[0], 0, 0, 0, 0, 0, true, 0.5, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, pos[0], pos[1], pos[2], 30*-dir[1], 30*dir[0], 0, 0, 0, 0, 0, true, 0.5, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					else
					{
						color = particlepalette[226 + (rand()&7)];
						CL_NewParticle(center, pt_static, color, color, tex_particle, 2, 0, 64, 192, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
				}
				else if (effectnameindex == EFFECT_TR_VORESPIKE)
				{
					if (cl_particles_quake.integer)
					{
						color = particlepalette[152 + (rand()&3)];
						CL_NewParticle(center, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 8, 0, true, 0.3, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					//else if (gamemode == GAME_GOODVSBAD2)
					//{
					//	dec = 6;
					//	CL_NewParticle(center, pt_alphastatic, particlepalette[0 + (rand()&255)], particlepalette[0 + (rand()&255)], tex_particle, 6, 0, 255, 384, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					//}
					else if (gamemode == GAME_PRYDON)
					{
						dec = 6;
						CL_NewParticle(center, pt_static, 0x103040, 0x204050, tex_particle, 6, 0, 64, 192, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
					}
					else
						CL_NewParticle(center, pt_static, 0x502030, 0x502030, tex_particle, 3, 0, 64, 192, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				}
				else if (effectnameindex == EFFECT_TR_NEHAHRASMOKE)
				{
					dec = 7;
					CL_NewParticle(center, pt_alphastatic, 0x303030, 0x606060, tex_smoke[rand()&7], 7, 0, 64, 320, 0, 0, pos[0], pos[1], pos[2], 0, 0, lhrandom(4, 12), 0, 0, 0, 4, false, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				}
				else if (effectnameindex == EFFECT_TR_NEXUIZPLASMA)
				{
					dec = 4;
					CL_NewParticle(center, pt_static, 0x283880, 0x283880, tex_particle, 4, 0, 255, 1024, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 16, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				}
				else if (effectnameindex == EFFECT_TR_GLOWTRAIL)
					CL_NewParticle(center, pt_alphastatic, particlepalette[palettecolor], particlepalette[palettecolor], tex_particle, 5, 0, 128, 320, 0, 0, pos[0], pos[1], pos[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
			if (bubbles)
			{
				if (effectnameindex == EFFECT_TR_ROCKET)
					CL_NewParticle(center, pt_bubble, 0x404040, 0x808080, tex_bubble, 2, 0, lhrandom(128, 512), 512, -0.25, 1.5, pos[0], pos[1], pos[2], 0, 0, 0, 0.0625, 0.25, 0, 16, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
				else if (effectnameindex == EFFECT_TR_GRENADE)
					CL_NewParticle(center, pt_bubble, 0x404040, 0x808080, tex_bubble, 2, 0, lhrandom(128, 512), 512, -0.25, 1.5, pos[0], pos[1], pos[2], 0, 0, 0, 0.0625, 0.25, 0, 16, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
			// advance to next time and position
			dec *= qd;
			len -= dec;
			VectorMA (pos, dec, dir, pos);
		}
		if (ent)
			ent->persistent.trail_time = len;
	}
	else
		Con_DPrintLinef ("CL_ParticleEffect_Fallback: no fallback found for effect %s", particleeffectname[effectnameindex]);
}


/*
===============
CL_ParseParticleEffect

Parse an effect out of the server message
===============
*/
void CL_ParseParticleEffect (void)
{
	vec3_t org, dir;
	int i, count, msgcount, color;

	MSG_ReadVector(&cl_message, org, cls.protocol);
	for (i=0 ; i<3 ; i++)
		dir[i] = MSG_ReadChar(&cl_message) * (1.0 / 16.0);
	msgcount = MSG_ReadByte(&cl_message);
	color = MSG_ReadByte(&cl_message);

	if (msgcount == 255)
		count = 1024;
	else
		count = msgcount;

	CL_ParticleEffect(EFFECT_SVC_PARTICLE, count, org, org, dir, dir, NULL, color);
}

/*
===============
CL_ParticleExplosion

===============
*/
void CL_ParticleExplosion (const vec3_t org)
{
	int i;
	trace_t trace;

	R_Stain(org, 96, 40, 40, 40, 64, 88, 88, 88, 64);
	CL_SpawnDecalParticleForPoint(org, 40, 48, 255, tex_bulletdecal[rand()&7], 0xFFFFFF, 0xFFFFFF);

	if (cl_particles_quake.integer)
	{
		for (i = 0;i < 1024;i++)
		{
			int color;
			int r = rand()&3;

			if (i & 1)
			{
				color = particlepalette[ramp1[r]];
				CL_NewParticle(org, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, org[0], org[1], org[2], 0, 0, 0, -4, -4, 16, 256, true, 0.1006 * (8 - r), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
			else
			{
				color = particlepalette[ramp2[r]];
				CL_NewParticle(org, pt_alphastatic, color, color, tex_particle, 1.5f, 0, 255, 0, 0, 0, org[0], org[1], org[2], 0, 0, 0, 1, 1, 16, 256, true, 0.0669 * (8 - r), 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			}
		}
	}
	else
	{
		i = CL_PointSuperContents(org);
		if (i & (SUPERCONTENTS_SLIME | SUPERCONTENTS_WATER))
		{
			if (cl_particles.integer && cl_particles_bubbles.integer)
				for (i = 0;i < 128 * cl_particles_quality.value;i++)
					CL_NewParticle(org, pt_bubble, 0x404040, 0x808080, tex_bubble, 2, 0, lhrandom(128, 255), 128, -0.125, 1.5, org[0], org[1], org[2], 0, 0, 0, 0.0625, 0.25, 16, 96, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		}
		else
		{
			if (cl_particles.integer && cl_particles_sparks.integer && cl_particles_explosions_sparks.integer)
			{
				for (i = 0;i < 512 * cl_particles_quality.value;i++)
				{
					int k = 0;
					vec3_t v, v2;
					do
					{
						VectorRandom(v2);
						VectorMA(org, 128, v2, v);
						trace = CL_TraceLine(org, v, MOVE_NOMONSTERS_1, NULL, SUPERCONTENTS_SOLID, 0, 0, collision_extendmovelength.value, true, false, NULL, false, false);
					}
					while (k++ < 16 && trace.fraction < 0.1f);
					VectorSubtract(trace.endpos, org, v2);
					VectorScale(v2, 2.0f, v2);
					CL_NewParticle(org, pt_spark, 0x903010, 0xFFD030, tex_particle, 1.0f, 0,
						lhrandom(0, 255), 512, 0, 0, org[0], org[1], org[2], v2[0], v2[1], v2[2],
						0, 0, 0, 0, true, 0, 1, PBLEND_ADD, PORIENT_SPARK, -1, -1, -1, 1, 1, 0, 0, NULL);
				}
			}
		}
	}

	if (cl_particles_explosions_shell.integer)
		R_NewExplosion(org);
}

/*
===============
CL_ParticleExplosion2

===============
*/
void CL_ParticleExplosion2 (const vec3_t org, int colorStart, int colorLength)
{
	int i, k;
	if (!cl_particles.integer) return;

	for (i = 0;i < 512 * cl_particles_quality.value;i++)
	{
		k = particlepalette[colorStart + (i % colorLength)];
		if (cl_particles_quake.integer)
			CL_NewParticle(org, pt_alphastatic, k, k, tex_particle, 1, 0, 255, 0, 0, 0, org[0], org[1], org[2], 0, 0, 0, -4, -4, 16, 256, true, 0.3, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL); // Baker #58 CL_NewQuakeParticle
		else
			CL_NewParticle(org, pt_alphastatic, k, k, tex_particle, lhrandom(0.5, 1.5), 0, 255, 512, 0, 0, org[0], org[1], org[2], 0, 0, 0, lhrandom(1.5, 3), lhrandom(1.5, 3), 8, 192, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
	}
}

static void CL_Sparks(const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, float sparkcount)
{
	vec3_t center;
	VectorMAM(0.5f, originmins, 0.5f, originmaxs, center);
	if (cl_particles_sparks.integer)
	{
		sparkcount *= cl_particles_quality.value;
		while(sparkcount-- > 0)
			CL_NewParticle(center, pt_spark, particlepalette[0x68], particlepalette[0x6f], tex_particle, 0.5f, 0, lhrandom(64, 255), 512, 1, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]) + cl.movevars_gravity * 0.1f, 0, 0, 0, 64, true, 0, 1, PBLEND_ADD, PORIENT_SPARK, -1, -1, -1, 1, 1, 0, 0, NULL);
	}
}

static void CL_Smoke(const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, float smokecount)
{
	vec3_t center;
	VectorMAM(0.5f, originmins, 0.5f, originmaxs, center);
	if (cl_particles_smoke.integer)
	{
		smokecount *= cl_particles_quality.value;
		while(smokecount-- > 0)
			CL_NewParticle(center, pt_smoke, 0x101010, 0x101010, tex_smoke[rand()&7], 2, 2, 255, 256, 0, 0, lhrandom(originmins[0], originmaxs[0]), lhrandom(originmins[1], originmaxs[1]), lhrandom(originmins[2], originmaxs[2]), lhrandom(velocitymins[0], velocitymaxs[0]), lhrandom(velocitymins[1], velocitymaxs[1]), lhrandom(velocitymins[2], velocitymaxs[2]), 0, 0, 0, smokecount > 0 ? 16 : 0, true, 0, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
	}
}

void CL_ParticleCube (const vec3_t mins, const vec3_t maxs, const vec3_t dir, int count, int colorbase, vec_t gravity, vec_t randomvel)
{
	vec3_t center;
	int k;
	if (!cl_particles.integer) return;
	VectorMAM(0.5f, mins, 0.5f, maxs, center);

	count = (int)(count * cl_particles_quality.value);
	while (count--)
	{
		k = particlepalette[colorbase + (rand()&3)];
		CL_NewParticle(center, pt_alphastatic, k, k, tex_particle, 2, 0, 255, 128, gravity, 0, lhrandom(mins[0], maxs[0]), lhrandom(mins[1], maxs[1]), lhrandom(mins[2], maxs[2]), dir[0], dir[1], dir[2], 0, 0, 0, randomvel, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
	}
}

void CL_ParticleRain (const vec3_t mins, const vec3_t maxs, const vec3_t dir, int count, int colorbase, int type)
{
	int k;
	float minz, maxz, lifetime = 30;
	vec3_t org;
	if (!cl_particles.integer) return;
	if (dir[2] < 0) // falling
	{
		minz = maxs[2] + dir[2] * 0.1;
		maxz = maxs[2];
		if (cl.worldmodel)
			lifetime = (maxz - cl.worldmodel->normalmins[2]) / max(1, -dir[2]);
	}
	else // rising??
	{
		minz = mins[2];
		maxz = maxs[2] + dir[2] * 0.1;
		if (cl.worldmodel)
			lifetime = (cl.worldmodel->normalmaxs[2] - minz) / max(1, dir[2]);
	}

	count = (int)(count * cl_particles_quality.value);

	switch(type)
	{
	case 0:
		if (!cl_particles_rain.integer) break;

		count *= 4; // ick, this should be in the mod or maps?

		while(count--)
		{
			k = particlepalette[colorbase + (rand()&3)];
			VectorSet(org, lhrandom(mins[0], maxs[0]), lhrandom(mins[1], maxs[1]), lhrandom(minz, maxz));
			// Baker: GAME_GOODVSBAD2 has particle size 20
			//if (gamemode == GAME_GOODVSBAD2)
			//	CL_NewParticle(org, pt_rain, k, k, tex_particle, 20, 0, lhrandom(32, 64), 0, 0, -1, org[0], org[1], org[2], dir[0], dir[1], dir[2], 0, 0, 0, 0, true, lifetime, 1, PBLEND_ADD, PORIENT_SPARK, -1, -1, -1, 1, 1, 0, 0, NULL);
			//else
				CL_NewParticle(org, pt_rain, k, k, tex_particle, 0.5, 0, lhrandom(32, 64), 0, 0, -1, org[0], org[1], org[2], dir[0], dir[1], dir[2], 0, 0, 0, 0, true, lifetime, 1, PBLEND_ADD, PORIENT_SPARK, -1, -1, -1, 1, 1, 0, 0, NULL);
		}
		break;
	case 1:
		if (!cl_particles_snow.integer) break;
		// Baker: GAME_GOODVSBAD2, a 2003 long dead multiplayer only mod for DarkPlaces
		// has particle size 20 here instead of 1

		while(count--)
		{
			k = particlepalette[colorbase + (rand()&3)];
			VectorSet(org, lhrandom(mins[0], maxs[0]), lhrandom(mins[1], maxs[1]), lhrandom(minz, maxz));
			//if (gamemode == GAME_GOODVSBAD2)
			//	CL_NewParticle(org, pt_snow, k, k, tex_particle, 20, 0, lhrandom(64, 128), 0, 0, -1, org[0], org[1], org[2], dir[0], dir[1], dir[2], 0, 0, 0, 0, true, lifetime, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
			//else
				CL_NewParticle(org, pt_snow, k, k, tex_particle, 1, 0, lhrandom(64, 128), 0, 0, -1, org[0], org[1], org[2], dir[0], dir[1], dir[2], 0, 0, 0, 0, true, lifetime, 1, PBLEND_ADD, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		}
		break;
	default:
		Con_PrintLinef ("CL_ParticleRain: unknown type %d (0 = rain, 1 = snow)", type);
	}
}

/*
===============
CL_EntityParticles
===============
*/
CALLERS_ (CL_UpdateNetworkEntityTrail for EF_BRIGHTFIELD_1) // EXCLUSIVE
void CL_EntityParticles (const entity_t *ent)
{
	int i, j;
	vec_t pitch, yaw, dist = 64, beamlength = 16;
	vec3_t org, v;
	static vec3_t avelocities[NUMVERTEXNORMALS];
	if (!cl_particles.integer) return;
	if (cl.time <= cl.oldtime) return; // don't spawn new entity particles while paused

	Matrix4x4_OriginFromMatrix(&ent->render.matrix, org);

	if (!avelocities[0][0])
		for (i = 0;i < NUMVERTEXNORMALS;i++)
			for (j = 0;j < 3;j++)
				avelocities[i][j] = lhrandom(0, 2.55);

	for (i = 0;i < NUMVERTEXNORMALS;i++)
	{
		yaw = cl.time * avelocities[i][0];
		pitch = cl.time * avelocities[i][1];
		v[0] = org[0] + m_bytenormals[i][0] * dist + (cos(pitch)*cos(yaw)) * beamlength;
		v[1] = org[1] + m_bytenormals[i][1] * dist + (cos(pitch)*sin(yaw)) * beamlength;
		v[2] = org[2] + m_bytenormals[i][2] * dist + (-sin(pitch)) * beamlength;
		CL_NewParticle(org, pt_entityparticle, particlepalette[0x6f], particlepalette[0x6f], tex_particle, 1, 0, 255, 0, 0, 0, v[0], v[1], v[2], 0, 0, 0, 0, 0, 0, 0, true, 0, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
	}
}


void CL_ReadPointFile_f(cmd_state_t *cmd)
{
	double org[3], leakorg[3];
	vec3_t vecorg;
	int r, c, s;
	char *pointfile = NULL, *pointfilepos, *t, tchar;
	char name[MAX_QPATH_128];

	if (!cl.worldmodel)
		return;

	dpsnprintf(name, sizeof(name), "%s.pts", cl.worldnamenoextension);
	pointfile = (char *)FS_LoadFile(name, tempmempool, fs_quiet_true, fs_size_ptr_null);
	if (!pointfile)
	{
		Con_PrintLinef ("Could not open %s", name);
		return;
	}

	Con_Printf ("Reading %s...\n", name);
	VectorClear(leakorg);
	c = 0;
	s = 0;
	pointfilepos = pointfile;
	while (*pointfilepos)
	{
		while (*pointfilepos == '\n' || *pointfilepos == '\r')
			pointfilepos++;
		if (!*pointfilepos)
			break;
		t = pointfilepos;
		while (*t && *t != '\n' && *t != '\r')
			t++;
		tchar = *t;
		*t = 0;
#if _MSC_VER >= 1400
#define sscanf sscanf_s
#endif
		r = sscanf (pointfilepos,"%lf %lf %lf", &org[0], &org[1], &org[2]);
		VectorCopy(org, vecorg);
		*t = tchar;
		pointfilepos = t;
		if (r != 3)
			break;
		if (c == 0)
			VectorCopy(org, leakorg);
		c++;

		if (cl.num_particles < cl.max_particles - 3)
		{
			s++;
			CL_NewParticle(vecorg, pt_alphastatic, particlepalette[(-c)&15], particlepalette[(-c)&15], tex_particle, 2, 0, 255, 0, 0, 0, org[0], org[1], org[2], 0, 0, 0, 0, 0, 0, 0, true, 1<<30, 1, PBLEND_ALPHA_0, PORIENT_BILLBOARD_0, -1, -1, -1, 1, 1, 0, 0, NULL);
		}
	}
	Mem_Free(pointfile);
	VectorCopy(leakorg, vecorg);
	Con_PrintLinef ("%d points read (%d particles spawned)\nLeak at %f %f %f", c, s, leakorg[0], leakorg[1], leakorg[2]);

	if (c == 0)
	{
		return;
	}

	CL_NewParticle(vecorg, pt_beam, 0xFF0000, 0xFF0000, tex_beam, 64, 0, 255, 0, 0, 0, org[0] - 4096, org[1], org[2], org[0] + 4096, org[1], org[2], 0, 0, 0, 0, false, 1<<30, 1, PBLEND_ADD, PORIENT_HBEAM, -1, -1, -1, 1, 1, 0, 0, NULL);
	CL_NewParticle(vecorg, pt_beam, 0x00FF00, 0x00FF00, tex_beam, 64, 0, 255, 0, 0, 0, org[0], org[1] - 4096, org[2], org[0], org[1] + 4096, org[2], 0, 0, 0, 0, false, 1<<30, 1, PBLEND_ADD, PORIENT_HBEAM, -1, -1, -1, 1, 1, 0, 0, NULL);
	CL_NewParticle(vecorg, pt_beam, 0x0000FF, 0x0000FF, tex_beam, 64, 0, 255, 0, 0, 0, org[0], org[1], org[2] - 4096, org[0], org[1], org[2] + 4096, 0, 0, 0, 0, false, 1<<30, 1, PBLEND_ADD, PORIENT_HBEAM, -1, -1, -1, 1, 1, 0, 0, NULL);
}
