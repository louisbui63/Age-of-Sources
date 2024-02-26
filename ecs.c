#include "ecs.h"
#include "bitflag.h"
#include "hash_map.h"
#include "vec.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char eq_u64(void *a, void *b) { return *(uint64_t *)a == *(uint64_t *)b; }

void hash_map_entry_free_vecptr(void *u) {
  HashMapEntry *w = u;
  free(w->key);
  vec_free(*(void **)w->value);
  free(w->value);
  free(u);
}

World world_new() {
  World a = {vec_new(int), vec_new(void (*)(void *)), vec_new(ComponentWrapper),
             vec_new(Entity), hash_map_create(hash_u64, eq_u64),
             // hash_map_create(hash_u64, eq),
             hash_map_create(hash_u64, eq_u64), 0};
  return a;
}

void world_free(World *w) {
  for (uint i = 0; i < vec_len(w->components); i++) {
    uint id = w->components[i].type_id;
    (w->component_free[id])(w->components[i].component);
  }
  vec_free(w->component_sizes);
  vec_free(w->component_free);
  vec_free(w->components);
  Entity *u = w->entities;
  for (uint i = 0; i < vec_len(w->entities); i++) {
    vec_free(u[i].components);
  }
  vec_free(w->entities);
  hash_map_free_callback(&w->entity_map, hash_map_entry_free_vecptr);
  hash_map_free(&w->component2entity);
  // hash_map_free(&w->entity2component);
}

int register_component_inner_callback(World *w, int size,
                                      void (*callback)(void *)) {
  if (w->last_component >= sizeof(Bitflag)) {
    return -1;
  }
  vec_push(w->component_sizes, size);
  vec_push(w->component_free, callback);
  Bitflag *u = malloc(sizeof(Bitflag));
  *u = 1 << w->last_component;
  // HashMap *a = malloc(sizeof(HashMap));
  // *a = hash_map_create(hash_u64, eq);
  void **v = malloc(sizeof(uintptr_t));
  *v = vec_new(uint64_t);
  hash_map_insert_callback(&w->entity_map, u, v, hash_map_entry_free_vecptr);
  w->last_component++;

  return 0;
}

int register_component_inner(World *w, int size) {
  return register_component_inner_callback(w, size, free);
}

void register_system_requirement(World *w, Bitflag b) {
  uint64_t *g = vec_new(uint64_t);
  for (uint i = 0; i < vec_len(w->entities); i++) {
    Bitflag nb = b;
    for (uint j = 0; j < vec_len(w->entities[i].components); j++) {
      ComponentWrapper c = w->entities[i].components[j];
      if (bitflag_get(nb, c.type_id)) {
        nb = bitflag_set(nb, c.type_id, 0);
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
  vec_push(e->components, cw);

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
        for (uint j = 0; j < vec_len(e->components); j++) {
          expected = bitflag_set(expected, e->components[j].type_id, 0);
        }
        if (!expected) {
          vec_push(to_add, *(Bitflag *)he->key);
        }
      }
      void *nc = cur->next;
      cur = nc;
    }
    for (uint i = 0; i < vec_len(to_add); i++) {
      // Bitflag *u = malloc(sizeof(Bitflag));
      // *u = to_add[i];
      uint64_t **es = hash_map_get(&w->entity_map, &to_add[i]);
      vec_push(*es, e->id);
      // free(u);
    }
    vec_free(to_add);
  }
}

void despawn_entity(World *w, Entity *e) {
  uint64_t id = e->id;

  // free associated components
  HashMap /*<uint64_t,int>*/ ccor = hash_map_create(hash_u64, eq_u64);

  for (uint i = 0; i < vec_len(e->components); i++) {
    uint64_t ecid = e->components[i].id;
    int *y = hash_map_get(&ccor, &ecid);
    if (y) {
#ifndef NDEBUG
      if (*y == -1)
        fprintf(stderr, "unreachable\n");
#endif
      ecid = *y;
    }
    uint64_t *c = malloc(sizeof(uint64_t));
    *c = ecid;
    int *a = malloc(sizeof(int));
    *a = -1;
    hash_map_insert(&ccor, c, a);
    if (ecid != vec_len(w->components) - 1) {
      vec_swap(w->components, ecid, vec_len(w->components) - 1);
      w->components[ecid].id = ecid;
      uint64_t *b = malloc(sizeof(uint64_t));
      *b = vec_len(w->components) - 1;
      int *c = malloc(sizeof(int));
      *c = ecid;
      hash_map_insert(&ccor, b, c);
    }
    (w->component_free[w->components[vec_len(w->components) - 1].type_id])(
        w->components[vec_len(w->components) - 1].component);
    vec_pop(w->components);
  }

  for (uint i = 0; i < vec_len(w->entities); i++) {
    if (w->entities[i].id == e->id)
      continue;
    for (uint j = 0; j < vec_len(w->entities[i].components); j++) {
      uint64_t ecid = w->entities[i].components[j].id;
      int *y = hash_map_get(&ccor, &ecid);
      if (y) {
#ifndef NDEBUG
        if (*y == -1)
          fprintf(stderr, "unreachable\n");
#endif
        ecid = *y;
      }
      w->entities[i].components[j].id = ecid;
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
        if (uwu[i] == vec_len(w->entities) - 1) {
          uwu[i] = e->id;
        }
      }
      cur = next;
    }
  }

  // update component2entity
  for (uint i = 0; i < w->component2entity.length; i++) {
    LinkedListLink *cur = w->component2entity.bucket[i].head;
    LinkedListLink *prev = w->component2entity.bucket[i].head;
    char beg = 1;
    while (cur) {
      LinkedListLink *next = cur->next;
      int64_t ecid = *(uint64_t *)((HashMapEntry *)cur->data)->key;
      int *y = hash_map_get(&ccor, &ecid);
      if (y) {
        ecid = *y;
      }
      if (ecid == -1) {
        if (beg) {
          w->component2entity.bucket[i].head = next;
          beg = 2;
        } else
          prev->next = next;
        HashMapEntry *entry = cur->data;
        free(entry->key);
        free(entry->value);
        free(entry);
        free(cur);
      } else if ((uint64_t)ecid !=
                 *(uint64_t *)((HashMapEntry *)cur->data)->key) {
        if (beg)
          w->component2entity.bucket[i].head = next;
        else
          prev->next = next;
        *(uint64_t *)((HashMapEntry *)cur->data)->key = ecid;
        uint64_t h = hash_u64(&ecid);
        ((HashMapEntry *)cur->data)->hash = h;

        cur->next =
            w->component2entity.bucket[h % w->component2entity.length].head;
        w->component2entity.bucket[h % w->component2entity.length].head = cur;
      }
      if (beg)
        beg--;
      prev = cur;
      cur = next;
    }
  }

  hash_map_free(&ccor);

  // UintPair out = {vec_len(w->entities) - 1, id};

  vec_free(e->components);

  // free the entity
  if (id != vec_len(w->entities) - 1) {
    vec_swap(w->entities, id, vec_len(w->entities) - 1);
    w->entities[id].id = id;
  }
  vec_pop(w->entities);

  // return out;
}
Entity *get_entity(World *w, EntityRef ref) { return &w->entities[ref]; }
EntityRef *world_query(World *w, Bitflag *b) {
  return *(EntityRef **)hash_map_get(&w->entity_map, b);
}
EntityRef **world_query_mut(World *w, Bitflag *b) {
  return (EntityRef **)hash_map_get(&w->entity_map, b);
}
