// filematch_csg_lifecycle.c.h

//
// brushrowlist -- entry free, list free, list add
//

static void brushrow_free(brushrow_s *p_brushrow)
{
	Mem_FreeNull_ (p_brushrow->pbrtexture);
}


void brushrowlistfreecontents (brushrowlist_t *list)
{
	for (int j = 0;j < list->count; j++) {
		brushrow_free (&list->brushrow[j]);
	}
	list->count = list->maxsize = 0;
	Mem_FreeNull_ (list->brushrow);
}

// ADD .... brush_s *p_brushrow = brushrowlist_add(&p_brush->brushrowlist);
brushrow_s *brushrowlist_add (brushrowlist_t *list)
{
	if (list->count >= list->maxsize) {
#ifdef _DEBUG
		size_t sizeof_brushrow_s= sizeof(brushrow_s);
		size_t sizeof_alloc = list->maxsize * sizeof(brushrow_s);
#endif
		list->maxsize += 16; // Baker: 4096 is way too big for this.  Page array, right?
		list->brushrow = (brushrow_s *)Mem_Realloc(zonemempool,
			list->brushrow, list->maxsize * sizeof(brushrow_s) );
	}

	brushrow_s *p_brushrow = &list->brushrow[list->count ++]; // Incremented count
	return p_brushrow;
}

//
// brushlist -- entry free, list free, list add
//

static void brush_free(brush_s *p_brush)
{
//typedef struct _brush_st {
//	brushrowlist_t	brushrowlist;
//} brush_s;

	brushrowlistfreecontents	(&p_brush->brushrowlist);

	// As patch
	Mem_FreeNull_ (p_brush->texture);
	patchrowlistfreecontents	(&p_brush->patchrowlist);


}

void brushlistfreecontents(brushlist_t *list)
{
	for (int j = 0;j < list->count; j++) {
		brush_free (&list->brush[j]);
	}
	list->count = list->maxsize = 0;
	Mem_FreeNull_ (list->brush);
}

RELATED_ (stringlistappend)
// ADD .... brush_s *p_brush = brushlist_add(&p_ent->brushlist);
brush_s *brushlist_add (brushlist_t *list)
{
	if (list->count >= list->maxsize) {
		list->maxsize += 32; // Baker: 4096 is really wasteful
		list->brush = (brush_s *)Mem_Realloc(zonemempool,
			list->brush, list->maxsize * sizeof(brush_s) );
	}

	brush_s *p_brush = &list->brush[list->count ++]; // Incremented count
	return p_brush;
}

//
// patchrowlist -- entry free, list free, list add
//

static void patchrow_free(patchrow_s *p_patchrow)
{
	// We do nothing!
}


void patchrowlistfreecontents (patchrowlist_t *list)
{
	for (int j = 0;j < list->count; j++) {
		patchrow_free (&list->patchrow[j]);
	}
	list->count = list->maxsize = 0;
	Mem_FreeNull_ (list->patchrow);
}

// ADD .... patch_s *p_patchrow = patchrowlist_add(&p_patch->patchrowlist);
patchrow_s *patchrowlist_add (patchrowlist_t *list)
{
	if (list->count >= list->maxsize) {
		list->maxsize += 16; // Baker: 4096 is stupidly wasteful
		list->patchrow = (patchrow_s *)Mem_Realloc(zonemempool,
			list->patchrow, list->maxsize * sizeof(patchrow_s) );
	}

	patchrow_s *p_patchrow = &list->patchrow[list->count ++]; // Incremented count
	return p_patchrow;
}

//
// entitylist -- entry free, list free, list add
//

static void entity_free(entityx_t *p_ent)
{
	stringlistfreecontents	(&p_ent->pairslist);
	brushlistfreecontents	(&p_ent->brushlist);
}

RELATED_ (stringlistappend)

void entitylistfreecontents(entitylist_t *list)
{
	for (int j = 0; j < list->count; j++) {
		entity_free (&list->entity[j]);
	}
	list->count = list->maxsize = 0;
	Mem_FreeNull_ (list->entity);
}

// ADD .... entityx_t *p_ent = entitylist_add(&myentities_list);
entityx_t *entitylist_add (entitylist_t *list)
{
	if (list->count >= list->maxsize) {
		list->maxsize += 1024; // Baker: 4096 is rather large.
		list->entity = (entityx_t *)Mem_Realloc(zonemempool,
			list->entity, list->maxsize * sizeof(entityx_t) );
	}

	entityx_t *pe = &list->entity[list->count ++]; // Incremented count
	return pe;
}

qbool entitylist_collide_down_ignoring_entnum (vec3_t collide, entitylist_t *plist, int start_idx, int beyond_idx, vec3_t org, int ignoreme)
{
	int num_collides = 0;
	VectorCopyDestSrc (collide, org);
	
	for (int ex = start_idx; ex < beyond_idx; ex ++) {
		if (ex == ignoreme)
			continue;

		const entityx_t *e = &plist->entity[ex];

		if (!e->brushlist.count)
			continue; // We don't collide against things without brushes.

		// Collide against brushes.	
		for (int bx = 0; bx < e->brushlist.count; bx ++) {
			const brush_s *a = &e->brushlist.brush[bx];

			if (!in_range (a->bxmins[0], org[0],  a->bxmaxs[0])) continue; // No X collide
			if (!in_range (a->bxmins[1], org[1],  a->bxmaxs[1])) continue; // No Y collide
			
			// Now .. z max must be lower
			if (a->bxmaxs[2] > org[2]) continue; // It's above us.

			// HIT - But is it best?  We want the largest one. (Highest thing below us)
			if (num_collides > 0 && a->bxmaxs[2] <= collide[2]) continue; // We aren't the highest

			collide[2] = a->bxmaxs[2]; // Best collision
			num_collides ++;
		} // brush		
	} // entities

	return (num_collides > 0);
}

// Baker: May 31 2025 - FIX_THIS_SHIT
entityx_t *entitylist_add_at_1_shiftup (entitylist_t *list)
{
	if (list->count >= list->maxsize) {
		list->maxsize += 1024; // Baker: 4096 is rather large
		list->entity = (entityx_t *)Mem_Realloc(zonemempool,
			list->entity, list->maxsize * sizeof(entityx_t) );
	}

//	entityx_t *pe = &list->entity[list->count ++]; // Incremented count
//	return pe;

	int newidx = list->count; // live: 480

	// 0
	// 1
	// 99
	// 100 <--- us (newidx)
	// we move 1 to 99 to 2 100 ...
	//Dec 22 2024 unused ... entityx_t *pstructfake = entitylist_add(list);


	// Baker: This is a page array.  These aren't pointers!
	size_t startidx = 1;
	size_t move_num_entries = /*beyond*/ newidx /*100*/ - startidx /*1*/ ; // Result 99 is size of move

	entityx_t *pstart1 = &list->entity[1];
	entityx_t *p2 = &list->entity[2];
	//entityx_t *p479 = &list->entity[newidx - 1];
	//entityx_t *p480 = &list->entity[newidx];
	size_t struct_size = sizeof(*pstart1); // 1 to 479 get moved to 2 thru 480.  This 479 entries.

	//Dec 22 2024 unused ... entityx_t *pdest  = &list->entity[2];
	size_t movesize = struct_size * move_num_entries;
	memmove (p2, pstart1, movesize);
	entityx_t *pe  = &list->entity[1];
	memset (pe, 0, sizeof(*pe));

	list->count ++;
	return pe;
}

//
//
//

RELATED_ (VM_map_entity_add)
int entitylist_add_entity (entitylist_t *plist)
{
	int entnum = plist->count;
	/*entityx_t *xt = unused Dec 22 2024*/ entitylist_add (plist);
	return entnum;
}
