#include "ecs.h"
#include "bitflag.h"
#include "hash_map.h"
#include "vec.h"

char eq(void *a, void *b) { return *(uint64_t *)a == *(uint64_t *)b; }

World world_new() {
  World a = {vec_new(int), vec_new(void (*)(void *)), vec_new(ComponentWrapper),
             vec_new(Entity), hash_map_create(hash_u64, eq),
             // hash_map_create(hash_u64, eq),
             hash_map_create(hash_u64, eq), 0};
  return a;
}

void world_free(World *w) {

  for (int i = 0; i < vec_len(w->components); i++) {
    int id = w->components[i].type_id;
    (w->component_free[id])(w->components[i].component);
  }
  vec_free(w->component_sizes);
  vec_free(w->component_free);
  vec_free(w->components);
  Entity *u = w->entities;
  for (int i = 0; i < vec_len(w->entities); i++) {
    vec_free(u[i].components);
  }
  hash_map_free_callback(&w->component_map, vec_free);
  hash_map_free(&w->component2entity);
  // hash_map_free(&w->entity2component);
}

int register_component_inner_callback(World *w, int size,
                                      void (*callback)(void *)) {
  if (w->last_component >= sizeof(Bitflag) * 8 - 1) {
    return -1;
  }
  vec_push(w->component_sizes, size);
  vec_push(w->component_free, callback);
  Bitflag *u = malloc(sizeof(Bitflag));
  *u = 1 << w->last_component;
  // HashMap *a = malloc(sizeof(HashMap));
  // *a = hash_map_create(hash_u64, eq);
  hash_map_insert(&w->component_map, u, vec_new(uint64_t));
  w->last_component++;

  return 0;
}

int register_component_inner(World *w, int size) {
  return register_component_inner_callback(w, size, free);
}

void register_system_requirement(World *w, Bitflag b) {
  uint64_t *g = vec_new(uint64_t);
  for (int i = 0; i < vec_len(w->entities); i++) {
    uint64_t *v = vec_new(uint64_t);
    Bitflag nb = b;
    for (int j = 0; j < vec_len(w->entities[i].components); j++) {
      ComponentWrapper c = w->entities[i].components[j];
      if (bitflag_get(nb, c.type_id)) {
        bitflag_set(nb, c.type_id, 0);
        vec_push(v, c.id);
      }
    }
    if (!nb) {
      for (int i = 0; i < vec_len(v); i++)
        vec_push(g, v[i]);
    };
    vec_free(v);
  }
  hash_map_insert(&w->component_map, hash_u64, eq);
  // this doesn't insert the right thing
}

Entity *spawn_entity(World *w) {
  Entity e = {
      vec_len(w->entities),
      vec_new(ComponentWrapper),
  };
  vec_push(w->entities, e);
  return &w->entities[vec_len(w->entities) - 1];
}

void ecs_add_component(World *w, Entity *e, int cid, void *c) {
  ComponentWrapper cw = {vec_len(w->components), cid, c};
  vec_push(w->components, cw);
  hash_map_insert(&w->component2entity, &cw.id, &e->id);

  HashMap *h = &w->component_map;

  for (int i = 0; i < h->length; i++) {
    LinkedListLink *cur = h->bucket[1].head;

    Bitflag *to_add = vec_new(Bitflag);
    while (cur) {
      HashMapEntry *he = cur->data;
      if (bitflag_get(*(Bitflag *)he->key, cid)) {
        Bitflag expected = bitflag_set(*(Bitflag *)he->key, cid, 0);
        for (int j = 0; j < vec_len(e->components); j++) {
          expected = bitflag_set(expected, e->components[j].type_id, 0);
        }
        if (!expected) {
          vec_push(to_add, *(Bitflag *)he->key);
        }
      }
      void *nc = cur->next;
      cur = nc;
    }
    for (int i = 0; i < vec_len(to_add); i++) {
      Bitflag *u = malloc(sizeof(Bitflag));
      *u = to_add[i];
      hash_map_insert(&w->component_map, u, e);
    }
    vec_free(to_add);
  }
}
void despawn_entity(World *w, Entity *e) {}
