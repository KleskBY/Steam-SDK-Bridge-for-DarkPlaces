// cl_particles_misc.c.h

void CL_ParticleTrail(int effectnameindex, float pcount, const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, entity_t *ent, int palettecolor, qbool spawndlight, qbool spawnparticles, float tintmins[4], float tintmaxs[4], float fade)
{
#if 1 // April 14 2025 - Hemebond particle when paused fix.
	if (cl.time <= cl.oldtime)
		return;
#endif
	CL_NewParticlesFromEffectinfo(effectnameindex, pcount, originmins, originmaxs, velocitymins, velocitymaxs, ent, palettecolor, spawndlight, spawnparticles, tintmins, tintmaxs, fade, true);
}

RELATED_ (VM_CL_pointparticles CL_ParticleEffect CL_ParticleBox CL_NewParticlesFromEffectinfo CL_NewParticle)
void CL_ParticleBox(int effectnameindex, float pcount, const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, entity_t *ent, int palettecolor, qbool spawndlight, qbool spawnparticles, float tintmins[4], float tintmaxs[4], float fade)
{
	CL_NewParticlesFromEffectinfo(effectnameindex, pcount, originmins, originmaxs, velocitymins, velocitymaxs, ent, palettecolor, spawndlight, spawnparticles, tintmins, tintmaxs, fade, false);
}

// note: this one ONLY does boxes!
CALLERS_ (VM_CL_pointparticles)
void CL_ParticleEffect(int effectnameindex, float pcount, const vec3_t originmins, const vec3_t originmaxs, const vec3_t velocitymins, const vec3_t velocitymaxs, entity_t *ent, int palettecolor)
{
	CL_ParticleBox(effectnameindex, pcount, originmins, originmaxs, velocitymins, 
		velocitymaxs, ent, palettecolor, /*spawnDlight?*/ true, /*spawnparticles*/ true, 
		/*Baker: tint*/ NULL, NULL, /*fade?*/ 1);
}


cvar_t r_drawparticles = {CF_CLIENT, "r_drawparticles", "1", "enables drawing of particles"};
static cvar_t r_drawparticles_drawdistance = {CF_CLIENT | CF_ARCHIVE, "r_drawparticles_drawdistance", "2000", "particles further than drawdistance*size will not be drawn"};
static cvar_t r_drawparticles_nearclip_min = {CF_CLIENT | CF_ARCHIVE, "r_drawparticles_nearclip_min", "4", "particles closer than drawnearclip_min will not be drawn"};
static cvar_t r_drawparticles_nearclip_max = {CF_CLIENT | CF_ARCHIVE, "r_drawparticles_nearclip_max", "4", "particles closer than drawnearclip_min will be faded"};
cvar_t r_drawdecals = {CF_CLIENT, "r_drawdecals", "1", "enables drawing of decals"};
static cvar_t r_drawdecals_drawdistance = {CF_CLIENT | CF_ARCHIVE, "r_drawdecals_drawdistance", "500", "decals further than drawdistance*size will not be drawn"};

#define PARTICLETEXTURESIZE_64 64
#define PARTICLEFONTSIZE_512 (PARTICLETEXTURESIZE_64*8)

static unsigned char shadebubble(float dx, float dy, vec3_t light)
{
	float dz, f, dot;
	vec3_t normal;
	dz = 1 - (dx*dx+dy*dy);
	if (dz > 0) // it does hit the sphere
	{
		f = 0;
		// back side
		normal[0] = dx;normal[1] = dy;normal[2] = dz;
		VectorNormalize(normal);
		dot = DotProduct(normal, light);
		if (dot > 0.5) // interior reflection
			f += ((dot *  2) - 1);
		else if (dot < -0.5) // exterior reflection
			f += ((dot * -2) - 1);
		// front side
		normal[0] = dx;normal[1] = dy;normal[2] = -dz;
		VectorNormalize(normal);
		dot = DotProduct(normal, light);
		if (dot > 0.5) // interior reflection
			f += ((dot *  2) - 1);
		else if (dot < -0.5) // exterior reflection
			f += ((dot * -2) - 1);
		f *= 128;
		f += 16; // just to give it a haze so you can see the outline
		f = bound(0, f, 255);
		return (unsigned char) f;
	}
	else
		return 0;
}

int particlefontwidth, particlefontheight, particlefontcellwidth, particlefontcellheight, particlefontrows, particlefontcols;
static void CL_Particle_PixelCoordsForTexnum(int texnum, int *basex, int *basey, int *width, int *height)
{
	*basex = (texnum % particlefontcols) * particlefontcellwidth;
	*basey = ((texnum / particlefontcols) % particlefontrows) * particlefontcellheight;
	*width = particlefontcellwidth;
	*height = particlefontcellheight;
}

static void setuptex(int texnum, unsigned char *data, unsigned char *particletexturedata)
{
	int basex, basey, w, h, y;
	CL_Particle_PixelCoordsForTexnum(texnum, &basex, &basey, &w, &h);
	if (w != PARTICLETEXTURESIZE_64 || h != PARTICLETEXTURESIZE_64)
		Sys_Error ("invalid particle texture size for autogenerating");
	for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
		memcpy(particletexturedata + ((basey + y) * PARTICLEFONTSIZE_512 + basex) * 4, data + y * PARTICLETEXTURESIZE_64 * 4, PARTICLETEXTURESIZE_64 * 4);
}

static void particletextureblotch(unsigned char *data, float radius, float red, float green, float blue, float alpha)
{
	int x, y;
	float cx, cy, dx, dy, f, iradius;
	unsigned char *d;
	cx = (lhrandom(radius + 1, PARTICLETEXTURESIZE_64 - 2 - radius) + lhrandom(radius + 1, PARTICLETEXTURESIZE_64 - 2 - radius)) * 0.5f;
	cy = (lhrandom(radius + 1, PARTICLETEXTURESIZE_64 - 2 - radius) + lhrandom(radius + 1, PARTICLETEXTURESIZE_64 - 2 - radius)) * 0.5f;
	iradius = 1.0f / radius;
	alpha *= (1.0f / 255.0f);
	for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
	{
		for (x = 0;x < PARTICLETEXTURESIZE_64;x++)
		{
			dx = (x - cx);
			dy = (y - cy);
			f = (1.0f - sqrt(dx * dx + dy * dy) * iradius) * alpha;
			if (f > 0)
			{
				if (f > 1)
					f = 1;
				d = data + (y * PARTICLETEXTURESIZE_64 + x) * 4;
				d[0] += (int)(f * (blue  - d[0]));
				d[1] += (int)(f * (green - d[1]));
				d[2] += (int)(f * (red   - d[2]));
			}
		}
	}
}

#if 0
static void particletextureclamp(unsigned char *data, int minr, int ming, int minb, int maxr, int maxg, int maxb)
{
	int i;
	for (i = 0;i < PARTICLETEXTURESIZE_64*PARTICLETEXTURESIZE_64;i++, data += 4)
	{
		data[0] = bound(minb, data[0], maxb);
		data[1] = bound(ming, data[1], maxg);
		data[2] = bound(minr, data[2], maxr);
	}
}
#endif

static void particletextureinvert(unsigned char *data)
{
	int i;
	for (i = 0;i < PARTICLETEXTURESIZE_64*PARTICLETEXTURESIZE_64;i++, data += 4)
	{
		data[0] = 255 - data[0];
		data[1] = 255 - data[1];
		data[2] = 255 - data[2];
	}
}

// Those loops are in a separate function to work around an optimization bug in Mac OS X's GCC
static void R_InitBloodTextures (unsigned char *particletexturedata)
{
	int i, j, k, m;
	size_t datasize = PARTICLETEXTURESIZE_64*PARTICLETEXTURESIZE_64*4;
	unsigned char *data = (unsigned char *)Mem_Alloc(tempmempool, datasize);

	// blood particles
	for (i = 0;i < 8;i++)
	{
		memset(data, 255, datasize);
		for (k = 0;k < 24;k++)
			particletextureblotch(data, PARTICLETEXTURESIZE_64/16, 96, 0, 0, 160);
		//particletextureclamp(data, 32, 32, 32, 255, 255, 255);
		particletextureinvert(data);
		setuptex(tex_bloodparticle[i], data, particletexturedata);
	}

	// blood decals
	for (i = 0;i < 8;i++)
	{
		memset(data, 255, datasize);
		m = 8;
		for (j = 1;j < 10;j++)
			for (k = min(j, m - 1);k < m;k++)
				particletextureblotch(data, (float)j*PARTICLETEXTURESIZE_64/64.0f, 96, 0, 0, 320 - j * 8);
		//particletextureclamp(data, 32, 32, 32, 255, 255, 255);
		particletextureinvert(data);
		setuptex(tex_blooddecal[i], data, particletexturedata);
	}

	Mem_Free(data);
}

//uncomment this to make engine save out particle font to a tga file when run
//#define DUMPPARTICLEFONT

static void R_InitParticleTexture (void)
{
	int x, y, d, i, k, m;
	int basex, basey, w, h;
	float dx, dy, f, s1, t1, s2, t2;
	vec3_t light;
	char *buf;
	fs_offset_t filesize;
	char texturename[MAX_QPATH_128];
	skinframe_t *sf;

	// a note: decals need to modulate (multiply) the background color to
	// properly darken it (stain), and they need to be able to alpha fade,
	// this is a very difficult challenge because it means fading to white
	// (no change to background) rather than black (darkening everything
	// behind the whole decal polygon), and to accomplish this the texture is
	// inverted (dark red blood on white background becomes brilliant cyan
	// and white on black background) so we can alpha fade it to black, then
	// we invert it again during the blendfunc to make it work...

#ifndef DUMPPARTICLEFONT
	//decalskinframe = R_SkinFrame_LoadExternal("particles/particlefont.tga", TEXF_ALPHA | TEXF_FORCELINEAR | TEXF_RGBMULTIPLYBYALPHA, false, false);
	decalskinframe = R_SkinFrame_LoadExternal("particles/particlefont.tga", TEXF_ALPHA | TEXF_FORCELINEAR | TEXF_RGBMULTIPLYBYALPHA, false, false);
	if (decalskinframe)
	{
		particlefonttexture = decalskinframe->base;
		// TODO maybe allow custom grid size?
		particlefontwidth = image_width;
		particlefontheight = image_height;
		particlefontcellwidth = image_width / 8;
		particlefontcellheight = image_height / 8;
		particlefontcols = 8;
		particlefontrows = 8;
	}
	else
#endif
	{
		unsigned char *particletexturedata = (unsigned char *)Mem_Alloc(tempmempool, PARTICLEFONTSIZE_512*PARTICLEFONTSIZE_512*4);
		size_t datasize = PARTICLETEXTURESIZE_64*PARTICLETEXTURESIZE_64*4;
		unsigned char *data = (unsigned char *)Mem_Alloc(tempmempool, datasize);
		unsigned char *noise1 = (unsigned char *)Mem_Alloc(tempmempool, PARTICLETEXTURESIZE_64*2*PARTICLETEXTURESIZE_64*2);
		unsigned char *noise2 = (unsigned char *)Mem_Alloc(tempmempool, PARTICLETEXTURESIZE_64*2*PARTICLETEXTURESIZE_64*2);

		particlefontwidth = particlefontheight = PARTICLEFONTSIZE_512;
		particlefontcellwidth = particlefontcellheight = PARTICLETEXTURESIZE_64;
		particlefontcols = 8;
		particlefontrows = 8;

		memset(particletexturedata, 255, PARTICLEFONTSIZE_512*PARTICLEFONTSIZE_512*4);

		// smoke
		for (i = 0;i < 8;i++)
		{
			memset(data, 255, datasize);
			do
			{
				fractalnoise(noise1, PARTICLETEXTURESIZE_64*2, PARTICLETEXTURESIZE_64/8);
				fractalnoise(noise2, PARTICLETEXTURESIZE_64*2, PARTICLETEXTURESIZE_64/4);
				m = 0;
				for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
				{
					dy = (y - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
					for (x = 0;x < PARTICLETEXTURESIZE_64;x++)
					{
						dx = (x - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
						d = (noise2[y*PARTICLETEXTURESIZE_64*2+x] - 128) * 3 + 192;
						if (d > 0)
							d = (int)(d * (1-(dx*dx+dy*dy)));
						d = (d * noise1[y*PARTICLETEXTURESIZE_64*2+x]) >> 7;
						d = bound(0, d, 255);
						data[(y*PARTICLETEXTURESIZE_64+x)*4+3] = (unsigned char) d;
						if (m < d)
							m = d;
					}
				}
			}
			while (m < 224);
			setuptex(tex_smoke[i], data, particletexturedata);
		}

		// rain splash
		memset(data, 255, datasize);
		for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
		{
			dy = (y - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
			for (x = 0;x < PARTICLETEXTURESIZE_64;x++)
			{
				dx = (x - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
				f = 255.0f * (1.0 - 4.0f * fabs(10.0f - sqrt(dx*dx+dy*dy)));
				data[(y*PARTICLETEXTURESIZE_64+x)*4+3] = (int) (bound(0.0f, f, 255.0f));
			}
		}
		setuptex(tex_rainsplash, data, particletexturedata);

		// normal particle
		memset(data, 255, datasize);
		for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
		{
			dy = (y - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
			for (x = 0;x < PARTICLETEXTURESIZE_64;x++)
			{
				dx = (x - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
				d = (int)(256 * (1 - (dx*dx+dy*dy)));
				d = bound(0, d, 255);
				data[(y*PARTICLETEXTURESIZE_64+x)*4+3] = (unsigned char) d;
			}
		}
		setuptex(tex_particle, data, particletexturedata);

		// rain
		memset(data, 255, datasize);
		light[0] = 1;light[1] = 1;light[2] = 1;
		VectorNormalize(light);
		for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
		{
			dy = (y - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
			// stretch upper half of bubble by +50% and shrink lower half by -50%
			// (this gives an elongated teardrop shape)
			if (dy > 0.5f)
				dy = (dy - 0.5f) * 2.0f;
			else
				dy = (dy - 0.5f) / 1.5f;
			for (x = 0;x < PARTICLETEXTURESIZE_64;x++)
			{
				dx = (x - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
				// shrink bubble width to half
				dx *= 2.0f;
				data[(y*PARTICLETEXTURESIZE_64+x)*4+3] = shadebubble(dx, dy, light);
			}
		}
		setuptex(tex_raindrop, data, particletexturedata);

		// bubble
		memset(data, 255, datasize);
		light[0] = 1;light[1] = 1;light[2] = 1;
		VectorNormalize(light);
		for (y = 0;y < PARTICLETEXTURESIZE_64;y++)
		{
			dy = (y - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
			for (x = 0;x < PARTICLETEXTURESIZE_64;x++)
			{
				dx = (x - 0.5f*PARTICLETEXTURESIZE_64) / (PARTICLETEXTURESIZE_64*0.5f-1);
				data[(y*PARTICLETEXTURESIZE_64+x)*4+3] = shadebubble(dx, dy, light);
			}
		}
		setuptex(tex_bubble, data, particletexturedata);

		// Blood particles and blood decals
		R_InitBloodTextures (particletexturedata);

		// bullet decals
		for (i = 0;i < 8;i++)
		{
			memset(data, 255, datasize);
			for (k = 0;k < 12;k++)
				particletextureblotch(data, PARTICLETEXTURESIZE_64/16, 0, 0, 0, 128);
			for (k = 0;k < 3;k++)
				particletextureblotch(data, PARTICLETEXTURESIZE_64/2, 0, 0, 0, 160);
			//particletextureclamp(data, 64, 64, 64, 255, 255, 255);
			particletextureinvert(data);
			setuptex(tex_bulletdecal[i], data, particletexturedata);
		}

#ifdef DUMPPARTICLEFONT
		Image_WriteTGABGRA ("particles/particlefont.tga", PARTICLEFONTSIZE_512, PARTICLEFONTSIZE_512, particletexturedata);
#endif

		decalskinframe = R_SkinFrame_LoadInternalBGRA("particlefont", TEXF_ALPHA | TEXF_FORCELINEAR | TEXF_RGBMULTIPLYBYALPHA, particletexturedata, PARTICLEFONTSIZE_512, PARTICLEFONTSIZE_512, 0, 0, 0, q_tx_convertsrgb_false, q_is_sky_load_false);
		particlefonttexture = decalskinframe->base;

		Mem_Free(particletexturedata);
		Mem_Free(data);
		Mem_Free(noise1);
		Mem_Free(noise2);
	}
	for (i = 0;i < MAX_PARTICLETEXTURES_256;i++)
	{
		CL_Particle_PixelCoordsForTexnum(i, &basex, &basey, &w, &h);
		particletexture[i].texture = particlefonttexture;
		particletexture[i].s1 = (basex + 1) / (float)particlefontwidth;
		particletexture[i].t1 = (basey + 1) / (float)particlefontheight;
		particletexture[i].s2 = (basex + w - 1) / (float)particlefontwidth;
		particletexture[i].t2 = (basey + h - 1) / (float)particlefontheight;
	}

#ifndef DUMPPARTICLEFONT
	particletexture[tex_beam].texture = loadtextureimage(particletexturepool, "particles/nexbeam.tga", false, TEXF_ALPHA | TEXF_FORCELINEAR | TEXF_RGBMULTIPLYBYALPHA, true, vid.sRGB3D);
	if (!particletexture[tex_beam].texture)
#endif
	{
		unsigned char noise3[64][64], data2[64][16][4];
		// nexbeam
		fractalnoise(&noise3[0][0], 64, 4);
		m = 0;
		for (y = 0;y < 64;y++)
		{
			dy = (y - 0.5f*64) / (64*0.5f-1);
			for (x = 0;x < 16;x++)
			{
				dx = (x - 0.5f*16) / (16*0.5f-2);
				d = (int)((1 - sqrt(fabs(dx))) * noise3[y][x]);
				data2[y][x][0] = data2[y][x][1] = data2[y][x][2] = (unsigned char) bound(0, d, 255);
				data2[y][x][3] = 255;
			}
		}

#ifdef DUMPPARTICLEFONT
		Image_WriteTGABGRA ("particles/nexbeam.tga", 64, 64, &data2[0][0][0]);
#endif
		particletexture[tex_beam].texture = R_LoadTexture2D(particletexturepool, "nexbeam", 16, 64, &data2[0][0][0], TEXTYPE_BGRA, TEXF_ALPHA | TEXF_FORCELINEAR | TEXF_RGBMULTIPLYBYALPHA, q_tx_miplevel_neg1, q_tx_palette_NULL);
	}
	particletexture[tex_beam].s1 = 0;
	particletexture[tex_beam].t1 = 0;
	particletexture[tex_beam].s2 = 1;
	particletexture[tex_beam].t2 = 1;

	// now load an texcoord/texture override file
	buf = (char *) FS_LoadFile("particles/particlefont.txt", tempmempool, fs_quiet_FALSE, &filesize);
	if (buf)
	{
		const char *bufptr;
		bufptr = buf;
		for(;;)
		{
			if (!COM_ParseToken_Simple(&bufptr, true, false, true))
				break;
			if (String_Match(com_token, "\n"))
				continue; // empty line
			i = atoi(com_token);

			texturename[0] = 0;
			s1 = 0;
			t1 = 0;
			s2 = 1;
			t2 = 1;

			if (COM_ParseToken_Simple(&bufptr, true, false, true) && strcmp(com_token, "\n"))
			{
				c_strlcpy(texturename, com_token);
				s1 = atof(com_token);
				if (COM_ParseToken_Simple(&bufptr, true, false, true) && strcmp(com_token, "\n"))
				{
					texturename[0] = 0;
					t1 = atof(com_token);
					if (COM_ParseToken_Simple(&bufptr, true, false, true) && strcmp(com_token, "\n"))
					{
						s2 = atof(com_token);
						if (COM_ParseToken_Simple(&bufptr, true, false, true) && strcmp(com_token, "\n"))
						{
							t2 = atof(com_token);
							c_strlcpy(texturename, "particles/particlefont.tga");
							if (COM_ParseToken_Simple(&bufptr, true, false, true) && strcmp(com_token, "\n"))
								c_strlcpy(texturename, com_token);
						}
					}
				}
				else
					s1 = 0;
			}
			if (!texturename[0])
			{
				Con_Printf ("particles/particlefont.txt: syntax should be texnum x1 y1 x2 y2 texturename or texnum x1 y1 x2 y2 or texnum texturename\n");
				continue;
			}
			if (i < 0 || i >= MAX_PARTICLETEXTURES_256)
			{
				Con_PrintLinef ("particles/particlefont.txt: texnum %d outside valid range (0 to %d)", i, MAX_PARTICLETEXTURES_256);
				continue;
			}
			sf = R_SkinFrame_LoadExternal(texturename, TEXF_ALPHA | TEXF_FORCELINEAR | TEXF_RGBMULTIPLYBYALPHA, true, true); // note: this loads as sRGB if sRGB is active!
			particletexture[i].texture = sf->base;
			particletexture[i].s1 = s1;
			particletexture[i].t1 = t1;
			particletexture[i].s2 = s2;
			particletexture[i].t2 = t2;
		}
		Mem_Free(buf);
	}
}

static void r_part_start(void)
{
	int i;
	// generate particlepalette for convenience from the main one
	for (i = 0;i < 256;i++)
		particlepalette[i] = palette_rgb[i][0] * 65536 + palette_rgb[i][1] * 256 + palette_rgb[i][2];
	particletexturepool = R_AllocTexturePool();
	R_InitParticleTexture ();
	CL_Particles_LoadEffectInfo(NULL);
}

static void r_part_shutdown(void)
{
	R_FreeTexturePool(&particletexturepool);
}

static void r_part_newmap(void)
{
	if (decalskinframe)
		R_SkinFrame_MarkUsed(decalskinframe);
	CL_Particles_LoadEffectInfo(NULL);
}

unsigned short particle_elements[MESHQUEUE_TRANSPARENT_BATCHSIZE*6];
float particle_vertex3f[MESHQUEUE_TRANSPARENT_BATCHSIZE*12], particle_texcoord2f[MESHQUEUE_TRANSPARENT_BATCHSIZE*8], particle_color4f[MESHQUEUE_TRANSPARENT_BATCHSIZE*16];

CALLERS_ (Render_InitOnce)
void R_Particles_InitOnce (void)
{
	int i;
	for (i = 0;i < MESHQUEUE_TRANSPARENT_BATCHSIZE;i++)
	{
		particle_elements[i*6+0] = i*4+0;
		particle_elements[i*6+1] = i*4+1;
		particle_elements[i*6+2] = i*4+2;
		particle_elements[i*6+3] = i*4+0;
		particle_elements[i*6+4] = i*4+2;
		particle_elements[i*6+5] = i*4+3;
	}

	Cvar_RegisterVariable(&r_drawparticles);
	Cvar_RegisterVariable(&r_drawparticles_drawdistance);
	Cvar_RegisterVariable(&r_drawparticles_nearclip_min);
	Cvar_RegisterVariable(&r_drawparticles_nearclip_max);
	Cvar_RegisterVariable(&r_drawdecals);
	Cvar_RegisterVariable(&r_drawdecals_drawdistance);
	R_RegisterModule("R_Particles", r_part_start, r_part_shutdown, r_part_newmap, NULL, NULL);
}
