#pragma once

#include <stdint.h>

#include "linked_list.h"

#define HASHMAP_DEFAULT_LENGTH 32

#define HASHMAP_OCCUP_MAX 0.7
#define HASHMAP_OCCUP_MIN 0.3

uint64_t hash_str(void *);

uint64_t hash_u64(void *);

typedef struct {
  void *key;
  void *value;
  uint64_t hash;
} HashMapEntry;

typedef struct {
  LinkedList *bucket;

  uint64_t (*hash_function)(void *);
  char (*comp_function)(void *, void *);

  int length; // length of the bucket
  int size;   // nb of elements in the hashmap
} HashMap;

HashMap hash_map_create(uint64_t (*)(void *), char (*)(void *, void *));
void hash_map_free(HashMap *);
void hash_map_free_void(void *);
void hash_map_free_callback(HashMap *, void (*callback)(void *));
int hash_map_insert(HashMap *, void *, void *);
int hash_map_delete(HashMap *, void *);
int hash_map_delete_callback(HashMap *, void *, void (*callback)(void *));
void *hash_map_get(HashMap *, void *);
