//! @file ecs.h
#pragma once

#include "../util.h"
#include "bitflag.h"
#include "hash_map.h"
#include <stdint.h>

//! Note that this reference is only valid until the number of entities
//! decreases
typedef uint64_t EntityRef;

//! Used to store the component, its type and its id
typedef struct {
  //! The component id
  uint64_t id;
  //! The id refering to the component type
  int type_id;
  //! A pointer to the component itself
  void *component;
} ComponentWrapper;

//! The entity structure for the ECS
typedef struct {
  //! The entity's id
  uint64_t id;

  // this should probably be replaced by a simple array that acts as a primitive
  // hashmap
  //! A vector of `ComponentWrapper` containing the entity's components
  VEC(ComponentWrapper) components;
} Entity;

//! The world structure used to store the different parts of the ECS
typedef struct {
  //! A vector containing all the sizes corresponding to each of the components'
  //! types
  VEC(uint) component_sizes;
  //! A vector of functions used to free each of the compontents (one function
  //! per type)
  void(VEC() * component_free)(void *);
  //! A vector of `ComponentWrapper` containing all the components
  VEC(ComponentWrapper) components;
  //! A vector of `Entity` containing all the entities
  VEC(Entity) entities;
  //! A `HashMap` with `Bitflag` as keys and `VEC(uint64_t)` as values, the map
  //! is used to easily access the list of entities corresponding to the system
  //! represented by the `Bitflag` key
  HashMap /*<Bitflag, vec<uint64_t>*>*/ entity_map;
  //! A `HashMap` with `uint64_t` as keys and `uint64_t` as values, the keys are
  //! components'ids and the values are entities'ids. It establishes for each
  //! component the list of the entities currently linked to it
  HashMap /*<uint64_t, uint64_t>*/ component2entity;
  // HashMap /*<uint64_t, uint64_t>*/ entity2component;
  //! Indicates the id the next component to be added should take
  uint last_component;
} World;

//! Returns a normalized boolean (0 or 1) indicating if the two arguments are
//! equal when both interpreted as `uint64_t`
char eq_u64(void *a, void *b);

//! Returns a new initialized `World` structure
World world_new();

//! Frees a `World` structure created using `world_new`
void world_free(World *);

//! `register_component(World*, type)` where type is the type of the component.
//! Registers a new component that uses `free` as a way to free it
#define register_component(w, tp)                                              \
  register_component_inner_callback((w), sizeof(tp), free)

//! `register_component(World*, type, void (*callback)(void *))` where type is
//! the type of the component. Registers a new component using a callback
//! function to free it
#define register_component_callback(w, tp, callback)                           \
  register_component_inner_callback((w), sizeof(tp), (callback))

//! Registers a new component using a callback function to free it, the size of
//! the component's type needs to be passed instead of the type itself
int register_component_inner_callback(World *w, int size,
                                      void (*callback)(void *));

//! Updates the entity_map of the world to take into account the system
//! represented by the `Bitflag` argument
void register_system_requirement(World *w, Bitflag b);

//! Spawns an `Entity` into the world and returns a pointer to it
Entity *spawn_entity(World *w);
//! Links a component to an `Entity`. The component itself need to live as long
//! as the world does (beware of scopes)
void ecs_add_component(World *w, Entity *e, int cid, void *c);
//! Despawns an `Entity`
void despawn_entity(World *w, Entity *e);
//! Returns an `Entity` pointer corresponding to the passed reference
Entity *get_entity(World *w, EntityRef ref);
//! Returns a vector of `EntityRef` referencing entities corresponding to the
//! system described by the `Bitflag` argument. The system needs to be
//! registered using `register_system_requirement` before using this function
VEC(EntityRef) world_query(World *w, Bitflag *b);
//! Returns a pointer to a vector of `EntityRef` referencing entities
//! corresponding to the system described by the `Bitflag` argument. The system
//! needs to be registered using `register_system_requirement` before using this
//! function
VEC(EntityRef) * world_query_mut(World *w, Bitflag *b);
//! Returns a pointer to the component of type `type` linked to the `Entity`, if
//! no component of this type is linked the the `Entity` the NULL pointer is
//! returned
void *entity_get_component(Entity *e, int type);
