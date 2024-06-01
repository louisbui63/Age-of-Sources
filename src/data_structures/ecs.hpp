//! @file ecs.hpp
#pragma once
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

#include "../util.hpp"
#include "bitflag.hpp"

//! A representation of an `Entity`. this note was left in its implementation :
//! "Note that this reference is only valid until the number of entities
//! decreases", but I'm pretty sure it's not true
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
  std::vector<uint64_t> components;
} Entity;

//! The world structure used to store the different parts of the ECS
struct World {
  //! A vector containing all the sizes corresponding to each of the components'
  //! types
  std::vector<uint> component_sizes;

  //! A vector of functions used to free each of the compontents (one
  //! function per type)
  std::vector<std::function<void(void *)>> component_free;

  //! A vector of `ComponentWrapper` containing all the components
  std::vector<ComponentWrapper> components;

  //! A vector of `Entity` containing all the entities
  std::vector<Entity> entities;

  //! A `HashMap` with `Bitflag` as keys and `VEC(uint64_t)` as values, the map
  //! is used to easily access the list of entities corresponding to the system
  //! represented by the `Bitflag` key
  std::unordered_map<Bitflag, std::vector<uint64_t>> entity_map;

  //! A `HashMap` with `uint64_t` as keys and `uint64_t` as values, the keys are
  //! components'ids and the values are entities'ids. It establishes for each
  //! component the list of the entities currently linked to it
  std::unordered_map<uint64_t, uint64_t> component2entity;

  //! Indicates the id the next component to be added should take
  uint last_component;

  //! Stores the available spaces in `components` that entity deletion created
  std::vector<uint64_t> component_sparsity;

  //! Stores the available spaces in `entities` that entity deletion created
  std::vector<uint64_t> entity_sparsity;

  //! Returns a new initialized `World` structure
  World();
  //! Frees a `World` structure created using `world_new`
  ~World();
  //! Registers a new component using a callback function to free it, the size
  //! of the component's type needs to be passed instead of the type itself
  template <typename T>
  Error register_component_callback(std::function<void(void *)> callback) {
    int size = sizeof(T);
    if (this->last_component >= sizeof(Bitflag) * 8) {
      return OUT_OF_COMPONENTS;
    }
    this->component_sizes.push_back(size);
    this->component_free.push_back(callback);
    // potential memory leak, not sure how c++ manages this kind of thing
    std::vector<uint64_t> u;
    this->entity_map.emplace(1 << this->last_component, u);
    this->last_component++;

    for (uint i = 0; i < this->entities.size(); i++) {
      this->entities[i].components.push_back(UINT64_MAX);
    }

    return SUCCESS;
  }
  //! Registers a new component using free function to free it, the size
  //! of the component's type needs to be passed instead of the type itself
  template <typename T> Error register_component() {
    return this->register_component_callback<T>(
        [](void *a) { delete reinterpret_cast<T *>(a); });
  }

  //! Updates the entity_map of the world to take into account the system
  //! represented by the `Bitflag` argument. Please not that single-component
  //! requirements SHOULD NOT be registered. This is considered undefined
  //! behavior, as well as registering the same requirements more than once.
  void register_system_requirement(Bitflag b);

  //! Spawns an `Entity` into the world and returns a pointer to it
  Entity *spawn_entity();

  //! Links a component to an `Entity`. The component itself need to live as
  //! long as the world does (beware of scopes)
  void add_component(Entity *e, int cid, void *c);

  //! Despawns an `Entity`
  void despawn_entity(Entity *e);

  //! Returns an `Entity` pointer corresponding to the passed reference
  Entity *get_entity(EntityRef ref);

  //! Returns a vector of `EntityRef` referencing entities corresponding to the
  //! system described by the `Bitflag` argument. If you want to modify the
  //! `World` based on the return value of this function, use `world_query_mut`
  //! instead. The system needs to be registered using
  //! `register_system_requirement` before using this function
  std::vector<EntityRef> query(Bitflag b);

  //! Despawns every `Entity` with this `Bitflag`
  void despawn_from_component(Bitflag b);

  //! Returns a pointer to the component of type `type` linked to the `Entity`,
  //! if no component of this type is linked the the `Entity` the NULL pointer
  //! is returned
  template <typename T> T get_component(Entity *e, int type) {
    if (e->components[type] == UINT64_MAX)
      return 0;
    return reinterpret_cast<T>(this->components[e->components[type]].component);
  }
};

//! Expands to a parallel query on the elements of `erefs`. `erefs` is expected
//! to be the return value of `world_query`, and must be a glvalue. Commands are
//! executed with the understanding that they can access the element they work
//! on with `ei`.
//! Note that spawning the threads is a significant overhead. For trivial cases,
//! using the sequential method can be faster. If unsure, use `TIME` to
//! benchmark both usecases. Note that Valgrind will detect some "possibly lost
//! memory". This is intended behavior, see
//! `https://gcc.gnu.org/bugzilla/show_bug.cgi?id=36298`
#define parallelize_query(erefs, commands)                                     \
  {                                                                            \
    _Pragma("omp parallel") {                                                  \
      _Pragma("omp for") {                                                     \
        for (uint i = 0; i < erefs.size(); i++) {                              \
          EntityRef ei = erefs[i];                                             \
          commands;                                                            \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }