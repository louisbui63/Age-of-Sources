//! @file hash_map.h
#pragma once

#include <stdint.h>

#include "../util.h"
#include "linked_list.h"
#include "vec.h"

//! The initial length of the internal array of a `HashMap`
#define HASHMAP_DEFAULT_LENGTH 32

//! The occupation ratio of a `HashMap` over which it grows
#define HASHMAP_OCCUP_MAX 0.7
//! The occupation ratio of a `HashMap` below which it shrinks
#define HASHMAP_OCCUP_MIN 0.3

//! A polynomial rolling hash for strings
uint64_t hash_str(void *);

//! A FNV hash function for 64 bit integers
uint64_t hash_u64(void *);

//! A FNV hash function for 8 bit integers
uint64_t hash_u8(void *);

//! An entry in a `HashMap`, i.e. a key-value pair
typedef struct {
  void *key;
  void *value;
  //! The hash of `value`
  uint64_t hash;
} HashMapEntry;

//! A hash map
typedef struct {
  //! The vector that stores the entries
  VEC(LinkedList) bucket;

  //! The function used for hashing the values stored in the `HashMap`
  uint64_t (*hash_function)(void *);
  //! The function used to compare values in the `HashMap`
  char (*comp_function)(void *, void *);

  //! Length of the bucket
  uint length;
  //! Numberb of elements in the hashmap
  uint size;
} HashMap;

//! Creates and returns a new `HashMap` that uses `hash` as the hash function
//! and `cmp` as the comparison function
HashMap hash_map_create(uint64_t (*hash)(void *), char (*cmp)(void *, void *));
//! Frees `h`, calling `callback` on each entry to free it
void hash_map_free_callback(HashMap *h, void (*callback)(void *));
//! Same as `hash_map_free_callback` but uses `hash_map_entry_free` as callback
void hash_map_free(HashMap *h);
//! Same as `hash_map_free`, deprecated
void hash_map_free_void(void *h);
//! Inserts the key-value pair `k`,`v` in `h`, deleting any previous entry of
//! key `k` with `callback`
int hash_map_insert_callback(HashMap *h, void *k, void *v,
                             void (*callback)(void *));
//! Same as `hash_map_insert_callback` but uses `hash_map_entry_free` as
//! callback
int hash_map_insert(HashMap *h, void *k, void *v);
//! Deletes the entry with key `k` using `callback`
int hash_map_delete_callback(HashMap *h, void *k, void (*callback)(void *));
//! Same as hash_map_delete_callback but uses `hash_map_entry_free` as callback
int hash_map_delete(HashMap *h, void *k);
//! Returns the value associated with key `k`, or a null pointer if there is no
//! such pair
void *hash_map_get(HashMap *h, void *k);

//! A `hash_map_free_callback` callback that dosn't free the values
void hash_map_entry_free_keys(void *u);
