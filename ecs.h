#pragma once

#include "bitflag.h"
#include "hash_map.h"

typedef struct {
  uint64_t id;
  int type_id;
  void *component;
} ComponentWrapper;

typedef struct {
  uint64_t id;
  ComponentWrapper *components;
} Entity;

typedef struct {
  int *component_sizes;
  void (**component_free)(void *);
  ComponentWrapper *components;
  Entity *entities;
  HashMap /*<Bitflag, vec<uint64_t>*>*/ entity_map;
  HashMap /*<uint64_t, uint64_t>*/ component2entity;
  // HashMap /*<uint64_t, uint64_t>*/ entity2component;
  int last_component;
} World;

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
