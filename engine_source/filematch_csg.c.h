// filematch_csg.c.h - Created SEPT 28 2024!

void bounds_expand (const vec3_t v, vec3_t loz, vec3_t hiz)
{
	if (v[0] > hiz[0])  hiz[0] = v[0];
	if (v[1] > hiz[1])  hiz[1] = v[1];
	if (v[2] > hiz[2])  hiz[2] = v[2];

	if (v[0] < loz[0])  loz[0] = v[0];
	if (v[1] < loz[1])  loz[1] = v[1];
	if (v[2] < loz[2])  loz[2] = v[2];
}

CALLERS_ (VM_map_entity_get_mins_maxs)
void brmins_maxs_init (vec3_t brmins, vec3_t brmaxs)
{
	vec3_t _brmins = { 999999,  999999,  999999};
	vec3_t _brmaxs = {-999999, -999999, -999999};
	VectorCopyDestSrc (brmins, _brmins);
	VectorCopyDestSrc (brmaxs, _brmaxs);
}


#include "filematch_csg_lifecycle.c.h"			// Allocation / free
#include "filematch_csg_parse_and_write.c.h"	// Parse and write

#include "filematch_csg_p_brushrow.c.h"
#include "filematch_csg_p_brush.c.h"
#include "filematch_csg_p_keys.c.h"
#include "filematch_csg_p_entity.c.h"
#include "filematch_csg_p_entitylist.c.h"		// entitylist_ functions

RELATED_ (VM_map_load)
#include "filematch_csg_p_vm_functions.h"		// map_load + such 







// LEFT TOP RIGHT BOTTOM LRTB CEIL FLOOR
// CEIL FLOOR
// -1 if neither.
// RELATED_ ()


#if 0
void entitylist_metric_world_brushes_with_walls (entitylist_t *plist)
{
	int lightrod_num_or_neg1 = entitylist_find_key_value (plist, "classname", "func_lightrod");

	plist->func_lightrod_entnum = lightrod_num_or_neg1;

	entityx_t	*p_ent_world = &plist->entity[ENTITY_WORLD_0];
	entity_metric_entity_brushes_and_brushrows (p_ent_world, plist);

	// QUADRANTS
	for (int brush_idx = 0; brush_idx < p_ent_world->brushlist.count; brush_idx ++) {
		brush_s *p_brush = &p_ent_world->brushlist.brush[brush_idx];
		if (p_brush->is_a_patch || !p_brush->brushrowlist.count)
			continue; // PATCH OR SOMETHING WITH NO ROWS, SKIP

		// Now determine quadrants of this brush and update world (plist quadrants)
		// WEST
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_west_0]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			// The west is the least east = lowest Y maxs.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmaxs[0] < p_best->bxmaxs[0]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_west_0] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// EAST
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_east_1]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			// The east is the least west = highest X mins.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmins[0] > p_best->bxmins[0]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_east_1] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// SOUTH
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_south_2]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			// The south is the least north = lowest Y maxs.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmaxs[1] < p_best->bxmaxs[1]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_south_2] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// NORTH
		while (1) {
			// The NORTH is the exclusive least south.  That means the mins[1] is the highest -- will beat them all.
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_north_3]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			// The north is the least south = highest Y mins.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmins[1] > p_best->bxmins[1]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_north_3] = PLUS1(brush_idx);
				break;
			}
			break;
		}

		// FLOOR
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_floor_4]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);
				break;
			}
			// The floor is the least high = lowest Z maxs.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmaxs[2] < p_best->bxmaxs[2]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_floor_4] = PLUS1(brush_idx);

				break;
			}

			break;
		}

		// CEILING
		while (1) {
			int best_brush_idx = UNPLUS1(plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5]); // 0 ==> -1
			if (best_brush_idx == not_found_neg1) {
				// Automatic win by default
				plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			// The ceiling is the most high = highest Z mins.
			brush_s *p_best = &p_ent_world->brushlist.brush[best_brush_idx];
			if (p_brush->bxmins[2] > p_best->bxmins[2]) { // MORE
				// Automatic win
				plist->quadrants_world_brushnums_plus1[quadrant_ceiling_5] = PLUS1(brush_idx);
				break;
			}
			break;
		}
#ifdef _DEBUG
		int j = 5; // Check p_brush
#endif
	} // each brush idx

	//int j = 5; // Check plist
}
#endif





#if 0
CALLERS_ (NONE June 2 2025) // This could mean it is obsoleted by entitylist_translate_brushes
int entity_translate_brushes_num_moved (entityx_t *d_ent, vec3_t vadd)
{
	int num_moved = 0;
	for (int bx = 0; bx < d_ent->brushlist.count; bx ++) {
		brush_s *d_brush = &d_ent->brushlist.brush[bx];
		qbool was_processed = brush_or_patch_tranlate_was_processed (d_brush, vadd);
		if (was_processed)
			num_moved ++;
	} // brush
	return num_moved;
}
#endif



