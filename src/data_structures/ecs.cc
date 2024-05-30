#include "ecs.hh"
#include "../errors.hh"
#include "bitflag.hh"
#include <cstdint>
#include <cstdlib>
#include <cstring>

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

// translation ends here

void register_system_requirement(World *w, Bitflag b) {
  uint64_t *g = vec_new(uint64_t);
  for (uint i = 0; i < vec_len(w->entities); i++) {
    Bitflag nb = b;
    for (uint j = 0; j < w->last_component; j++) {
      if (w->entities[i].components[j] != UINT64_MAX) {
        ComponentWrapper c = w->components[w->entities[i].components[j]];
        if (bitflag_get(nb, c.type_id)) {
          nb = bitflag_set(nb, c.type_id, 0);
        }
      }
    }
    if (!nb) {
      vec_push(g, w->entities[i].id);
    };
  }
  Bitflag *u = malloc(sizeof(Bitflag));
  memcpy(u, &b, sizeof(Bitflag));
  void **v = malloc(sizeof(uintptr_t));
  *v = g;
  hash_map_insert_callback(&w->entity_map, u, v, hash_map_entry_free_vecptr);
}

Entity *spawn_entity(World *w) {
  VEC(uint64_t) cmps = vec_new(uint64_t);
  uint64_t x = UINT64_MAX;
  for (uint i = 0; i < w->last_component; i++) {
    vec_push(cmps, x);
  }

  if (vec_len(w->entity_sparsity)) {
    uint64_t loc = vec_last(w->entity_sparsity);
    vec_pop(w->entity_sparsity);
    w->entities[loc] = (Entity){
        loc,
        cmps,
    };
    return &w->entities[loc];
  }

  Entity e = {
      vec_len(w->entities),
      cmps,
  };
  vec_push(w->entities, e);
  return &w->entities[vec_len(w->entities) - 1];
}

void ecs_add_component(World *w, Entity *e, int cid, void *c) {
  uint64_t emp;
  uint8_t new = 1;
  if (vec_len(w->component_sparsity)) {
    emp = vec_last(w->component_sparsity);
    vec_pop(w->component_sparsity);
    new = 0;
  } else
    emp = vec_len(w->components);

  ComponentWrapper cw = {emp, cid, c};
  e->components[cid] = emp;

  if (new)
    vec_push(w->components, cw);
  else
    w->components[emp] = cw;

  uint64_t *cwid = malloc(sizeof(uint64_t)), *eid = malloc(sizeof(uint64_t));
  *cwid = cw.id;
  *eid = e->id;
  hash_map_insert(&w->component2entity, cwid, eid);

  HashMap *h = &w->entity_map;

  for (uint i = 0; i < h->length; i++) {
    LinkedListLink *cur = h->bucket[i].head;

    Bitflag *to_add = vec_new(Bitflag);
    while (cur) {
      HashMapEntry *he = cur->data;
      if (bitflag_get(*(Bitflag *)he->key, cid)) {
        Bitflag expected = bitflag_set(*(Bitflag *)he->key, cid, 0);
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
          vec_push(to_add, *(Bitflag *)he->key);
        }
      }
      void *nc = cur->next;
      cur = nc;
    }
    for (uint i = 0; i < vec_len(to_add); i++) {
      uint64_t **es = hash_map_get(&w->entity_map, &to_add[i]);
      vec_push(*es, e->id);
    }
    vec_free(to_add);
  }
}

void despawn_entity(World *w, Entity *e) {
  uint64_t id = e->id;

  for (uint i = 0; i < vec_len(e->components); i++) {
    if (e->components[i] != UINT64_MAX) {
      uint64_t cid = e->components[i];
      w->component_free[w->components[cid].type_id](
          w->components[cid].component);
      w->components[cid].component = 0;
      vec_push(w->component_sparsity, w->components[cid].id);
    }
  }

  // remove the entity from the entity map
  for (uint i = 0; i < w->entity_map.length; i++) {
    LinkedListLink *cur = w->entity_map.bucket[i].head;
    while (cur) {
      LinkedListLink *next = cur->next;
      uint64_t *uwu = *(uint64_t **)(((HashMapEntry *)cur->data)->value);
      for (uint i = 0; i < vec_len(uwu); i++) {
        if (uwu[i] == e->id) {
          vec_swap(uwu, i, vec_len(uwu) - 1);
          vec_pop(uwu);
        }
      }
      cur = next;
    }
  }

  vec_push(w->entity_sparsity, id);
  vec_free(e->components);
  e->components = 0;
}

Entity *get_entity(World *w, EntityRef ref) { return &w->entities[ref]; }

VEC(EntityRef) world_query(World *w, Bitflag *b) {
  return *(VEC(EntityRef) *)hash_map_get(&w->entity_map, b);
}

VEC(EntityRef) * world_query_mut(World *w, Bitflag *b) {
  return (VEC(EntityRef) *)hash_map_get(&w->entity_map, b);
}

void *entity_get_component(World *w, Entity *e, int type) {
  if (e->components[type] == UINT64_MAX)
    return 0;
  return w->components[e->components[type]].component;
}

void despawn_from_component(World *w, Bitflag b) {
  EntityRef **err = world_query_mut(w, &b);
  while (vec_len(*err) > 0) {
    Entity *e = get_entity(w, *err[0]);
    despawn_entity(w, e);
  }
}

EntityRef get_next_entity_ref(World *w) {
  if (vec_len(w->entity_sparsity)) {
    return w->entity_sparsity[vec_len(w->entity_sparsity) - 1];
  }
  return vec_len(w->entities) - 1;
}
