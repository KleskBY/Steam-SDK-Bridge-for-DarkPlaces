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
// world.h

#ifndef WORLD_H
#define WORLD_H

#include "qtypes.h"
#include "collision.h"

#define MOVE_NORMAL_0						0
#define MOVE_NOMONSTERS_1					1		// Solid BSP (4) only
#define MOVE_MISSILE_2						2
#define MOVE_WORLDONLY_3					3
#define MOVE_HITMODEL_4						4

// FTE has ...
//#define	MOVE_HITMODEL	(1<<2) // 4
//#define MOVE_RESERVED	(1<<3) // 8			//so we are less likly to get into tricky situations when we want to steal annother future DP extension.
//#define MOVE_TRIGGERS	(1<<4) // 16			//triggers must be marked with FINDABLE_NONSOLID	(an alternative to solid-corpse)
//#define MOVE_EVERYTHING	(1<<5) // 32			//can return triggers and non-solid items if they're marked with FINDABLE_NONSOLID (works even if the items are not properly linked)
//#define MOVE_LAGGED		(1<<6) // 64			//trace touches current last-known-state, instead of actual ents (just affects players for now)
//#define MOVE_ENTCHAIN	(1<<7) // 128			//chain of impacted ents, otherwise result shows only world
//#define MOVE_OTHERONLY	(1<<8) // 256			//test the trace against a single entity, ignoring non-solid/owner/etc flags (but respecting contents).
//#define MOVE_IGNOREHULL	(1u<<31) 	//used on tracelines etc to simplify the code a little

RELATED_ (RENDER_STEP RENDER_SOLID_NOT_BAKER_256 RENDER_FL_INTERACTIVE_512)
#define	MOVE_NETWORK_ENTITIES_BAKER_4096	4096	// Zircon CSQC collide against monsters and other network entities

#define AREA_GRID 128
#define AREA_GRIDNODES (AREA_GRID * AREA_GRID)

typedef struct link_s
{
	llist_t list;
	int entitynumber;
} link_t;

typedef struct world_physics_s
{
	// for ODE physics engine
	qbool ode; // if true then ode is activated
	void *ode_world;
	void *ode_space;
	void *ode_contactgroup;
	// number of constraint solver iterations to use (for dWorldQuickStep)
	int ode_iterations;
	// actual step (server frametime / ode_iterations)
	vec_t ode_step;
	// time we need to simulate, for constantstep
	vec_t ode_time;
	// stats
	int ode_numobjects; // total objects cound
	int ode_activeovjects; // active objects count
	// max velocity for a 1-unit radius object at current step to prevent
	// missed collisions
	vec_t ode_movelimit;
}
world_physics_t;

struct prvm_prog_s;

typedef struct world_s
{
	// convenient fields
	char filename[MAX_QPATH_128];
	vec3_t mins;
	vec3_t maxs;
	struct prvm_prog_s *prog;

	int areagrid_stats_calls;
	int areagrid_stats_nodechecks;
	int areagrid_stats_entitychecks;

	link_t areagrid[AREA_GRIDNODES];
	link_t areagrid_outside;
	vec3_t areagrid_bias;
	vec3_t areagrid_scale;
	vec3_t areagrid_mins;
	vec3_t areagrid_maxs;
	vec3_t areagrid_size;
	int areagrid_marknumber;

	// if the QC uses a physics engine, the data for it is here
	world_physics_t physics;
}
world_t;

struct prvm_edict_s;

// cyclic doubly-linked list functions
void World_ClearLink(link_t *l);
void World_RemoveLink(link_t *l);
void World_InsertLinkBefore(link_t *l, link_t *before, int entitynumber);

void World_InitOnce(void);
void World_Shutdown(void);

/// called after the world model has been loaded, before linking any entities
void World_SetSize(world_t *world, const char *filename, const vec3_t mins, const vec3_t maxs, struct prvm_prog_s *prog);
/// unlinks all entities (used before reallocation of edicts)
void World_UnlinkAll(world_t *world);

void World_PrintAreaStats(world_t *world, const char *worldname);

/// call before removing an entity, and before trying to move one,
/// so it doesn't clip against itself
void World_UnlinkEdict(struct prvm_edict_s *ent);

/// Needs to be called any time an entity changes origin, mins, maxs
void World_LinkEdict(world_t *world, struct prvm_edict_s *ent, const vec3_t mins, const vec3_t maxs, qbool link_solid_not);

/// \returns list of entities touching a box
int World_EntitiesInBox(world_t *world, const vec3_t mins, const vec3_t maxs, int maxlist, struct prvm_edict_s **list);

void World_Start(world_t *world);
void World_End(world_t *world);

// update physics
// this is called by SV_Physics
void World_Physics_Frame(world_t *world, double frametime, double gravity);

// change physics properties of entity
struct prvm_edict_s;
struct edict_odefunc_s;
void World_Physics_ApplyCmd(struct prvm_edict_s *ed, struct edict_odefunc_s *f);

// remove physics data from entity
// this is called by entity removal
void World_Physics_RemoveFromEntity(world_t *world, struct prvm_edict_s *ed);
void World_Physics_RemoveJointFromEntity(world_t *world, struct prvm_edict_s *ed);

#endif

