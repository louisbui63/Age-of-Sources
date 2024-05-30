#include "ecs.hh"
#include "../errors.hh"
#include "bitflag.hh"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

World::World() { last_component = 0; }
World::~World() {
  for (uint i = 0; i < this->components.size(); i++) {
    if (this->components[i].component) {
      uint id = this->components[i].type_id;
      (this->component_free[id])(this->components[i].component);
    }
  }
}

template <typename T>
Error World::register_component_callback(std::function<void(void *)> callback) {
  int size = sizeof(T);
  if (this->last_component >= sizeof(Bitflag) * 8) {
    return OUT_OF_COMPONENTS;
  }
  this->component_sizes.push_back(size);
  this->component_free.push_back(callback);
  // potential memory leak, not sure how c++ manages this kind of thing
  this->entity_map.emplace(1 << this->last_component,
                           new std::vector<uint64_t>);
  this->last_component++;

  for (uint i = 0; i < this->entities.size(); i++) {
    this->entities[i].components.push_back(UINT64_MAX);
  }

  return SUCCESS;
}
template <typename T> Error World::register_component() {
  this->register_component_callback<T>(free);
}

void World::register_system_requirement(Bitflag b) {
  std::vector<uint64_t> g = {};
  for (uint i = 0; i < this->entities.size(); i++) {
    Bitflag nb = b;
    for (uint j = 0; j < this->last_component; j++) {
      if (this->entities[i].components[j] != UINT64_MAX) {
        ComponentWrapper c = this->components[this->entities[i].components[j]];
        if (bitflag_get(nb, c.type_id)) {
          nb = bitflag_set(nb, c.type_id, 0);
        }
      }
    }
    if (!nb) {
      g.push_back(this->entities[i].id);
    };
  }

  this->entity_map.emplace(b, g);
}
Entity *World::spawn_entity() {
  std::vector<uint64_t> cmps(this->last_component);
  for (uint i = 0; i < this->last_component; i++)
    cmps.push_back(UINT64_MAX);

  if (this->entity_sparsity.size()) {
    uint64_t loc = this->entity_sparsity.back();
    this->entity_sparsity.pop_back();
    this->entities[loc] = {
        loc,
        cmps,
    };
    return &this->entities[loc];
  }

  Entity e = {
      this->entities.size(),
      cmps,
  };
  this->entities.push_back(e);
  return &this->entities.back();
}

void World::ecs_add_component(Entity *e, int cid, void *c) {
  uint64_t emp;
  uint8_t nw = 1;
  if (this->component_sparsity.size()) {
    emp = this->component_sparsity.back();
    this->component_sparsity.pop_back();
    nw = 0;
  } else
    emp = this->components.size();

  // translation ends here

  ComponentWrapper cw = {emp, cid, c};
  e->components[cid] = emp;

  if (nw)
    this->components.push_back(cw);
  else
    this->components[emp] = cw;

  this->component2entity.emplace(cw.id, e->id);

  for (auto &[key, val] : this->entity_map) {
    if (bitflag_get(*(Bitflag *)key, cid)) {
      Bitflag expected = bitflag_set(*(Bitflag *)key, cid, 0);
      char failed = 0;
      uint j = 0;
      while (expected && !failed) {
        if (expected & 1 && e->components[j] == UINT64_MAX) {
          failed = 1;
          break;
        }
        expected >>= 1;
        j++;
      }
      if (!failed) {
        val.push_back(e->id);
      }
    }
  }
}

void World::despawn_entity(Entity *e) {
  uint64_t id = e->id;

  for (uint i = 0; i < e->components.size(); i++) {
    if (e->components[i] != UINT64_MAX) {
      uint64_t cid = e->components[i];
      this->component_free[this->components[cid].type_id](
          this->components[cid].component);
      this->components[cid].component = 0;
      this->component_sparsity.push_back(this->components[cid].id);
    }
  }

  // remove the entity from the entity map
  for (auto &[key, val] : this->entity_map) {
    for (uint i = 0; i < val.size(); i++) {
      if (val[i] == e->id) {
        val[i] = val.back();
        val.pop_back();
      }
    }
  }

  this->entity_sparsity.push_back(id);

  // that looks like something that would cause a bug down the line so I keep it
  // there vec_free(e->components); e->components = 0;
}

Entity *World::get_entity(EntityRef ref) { return &this->entities[ref]; }

std::vector<EntityRef> World::world_query(Bitflag b) {
  return this->entity_map[b];
}

void *entity_get_component(World *w, Entity *e, int type) {
  if (e->components[type] == UINT64_MAX)
    return 0;
  return w->components[e->components[type]].component;
}

void World::despawn_from_component(Bitflag b) {
  std::vector<EntityRef> err = this->world_query(b);
  while (err.size()) {
    // not sure this works either
    Entity *e = get_entity(err[0]);
    this->despawn_entity(e);
  }
}
