#pragma once

#include "bitflag.h"
#include "hash_map.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>

//! Note that this reference is only valid until the number of entities
//! decreases
typedef uint64_t EntityRef;

typedef struct {
  uint64_t id;
  int type_id;
  void *component;
} ComponentWrapper;

typedef struct {
  uint64_t id;

  // this should probably be replaced by a simple array that acts as a primitive
  // hashmap
  ComponentWrapper *components;
} Entity;

typedef struct {
  VEC(uint) component_sizes;
  void(VEC() * component_free)(void *);
  VEC(ComponentWrapper) components;
  VEC(Entity) entities;
  HashMap /*<Bitflag, vec<uint64_t>*>*/ entity_map;
  HashMap /*<uint64_t, uint64_t>*/ component2entity;
  // HashMap /*<uint64_t, uint64_t>*/ entity2component;
  uint last_component;
} World;

char eq_u64(void *a, void *b);

World world_new();

void world_free(World *);

#define register_component(w, tp)                                              \
  register_component_inner_callback((w), sizeof(tp), free)
#define register_component_callback(w, tp, callback)                           \
  register_component_inner_callback((w), sizeof(tp), (callback))
int register_component_inner_callback(World *w, int size,
                                      void (*callback)(void *));

void register_system_requirement(World *w, Bitflag b);
Entity *spawn_entity(World *w);
void ecs_add_component(World *w, Entity *e, int cid, void *c);
void despawn_entity(World *w, Entity *e);
Entity *get_entity(World *w, EntityRef ref);
VEC(EntityRef) world_query(World *w, Bitflag *b);
VEC(EntityRef) * world_query_mut(World *w, Bitflag *b);
void *entity_get_component(Entity *e, int type);
