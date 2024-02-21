#include "hash_map.h"
#include "linked_list.h"

#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET_BASIS 0xcbf29ce484222325

uint64_t hash_str(void *str) {
  // plynomial rolling hash
  // adapted from TN++ SWERC training
  char *s = (char *)str;
  const uint64_t MOD = 1e9 + 7;
  const uint64_t p = 53;
  uint64_t h = 0, pl = 1;
  for (int i = 0; s[i]; ++i) {
    h = (p * h + s[i]) % MOD;
    pl = (pl * p) % MOD;
  }
  return h;
}

uint64_t hash_u64(void *u) {
  uint64_t w = *(uint64_t *)u;
  // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
  uint64_t out = FNV_OFFSET_BASIS;
  for (int i = 0; i < 8; i++) {
    uint8_t byte = (uint8_t)w;
    w >>= 8;
    out *= FNV_PRIME;
    out ^= byte;
  }
  return out;
}

HashMap hash_map_create(uint64_t (*h)(void *), char (*c)(void *, void *)) {
  HashMap hm = {malloc(sizeof(HashMapEntry) * HASHMAP_DEFAULT_LENGTH), h, c,
                HASHMAP_DEFAULT_LENGTH, 0};
  for (int i = 0; i < HASHMAP_DEFAULT_LENGTH; i++)
    hm.bucket[i] = linked_list_create();
  return hm;
}

int grow(HashMap *h) {
  int ns = h->length * 2;
  LinkedList *nb = malloc(sizeof(LinkedList) * ns);
  for (int i = 0; i < h->length; i++) {
    LinkedListLink *cur = h->bucket[1].head;
    while (cur) {
      HashMapEntry *e = cur->data;
      int err = linked_list_insert(&nb[e->hash % ns], e, 0);
      if (err) {
        return err;
      }

      void *nc = cur->next;
      free(cur);
      cur = nc;
    }
  }
  h->length *= 2;
  h->bucket = nb;
  return 0;
}
int shrink(HashMap *h) {
  int ns = h->length / 2;
  LinkedList *nb = malloc(sizeof(LinkedList) * ns);
  for (int i = 0; i < h->length; i++) {
    LinkedListLink *cur = h->bucket[1].head;
    while (cur) {
      HashMapEntry *e = cur->data;
      int err = linked_list_insert(&nb[e->hash % ns], e, 0);
      if (err) {
        return err;
      }

      void *nc = cur->next;
      free(cur);
      cur = nc;
    }
  }
  h->length /= 2;
  h->bucket = nb;
  return 0;
}

int hash_map_insert(HashMap *h, void *k, void *v) {
  hash_map_delete(h, k);
  int hash = h->hash_function(k);
  int bplace = hash % h->length;

  HashMapEntry *et = malloc(sizeof(HashMapEntry));
  HashMapEntry temp = {k, v, hash};
  *et = temp;
  int err = linked_list_insert(&h->bucket[bplace], et, 0);
  if (err)
    return err;
  h->size++;

  if ((float)h->length / (float)h->size > HASHMAP_OCCUP_MAX) {
    return grow(h);
  }
  return 0;
}

void *hash_map_get(HashMap *h, void *k) {
  LinkedList u = h->bucket[h->hash_function(k) % h->length];
  LinkedListLink *cur = u.head;
  while (cur)
    if (h->comp_function(k, ((HashMapEntry *)(cur->data))->key))
      return ((HashMapEntry *)(cur->data))->value;
  return 0;
}

int hash_map_delete(HashMap *h, void *k) {
  return hash_map_delete_callback(h, k, free);
}

int hash_map_delete_callback(HashMap *h, void *k, void (*callback)(void *)) {
  char deleted = 0;
  LinkedList u = h->bucket[h->hash_function(k) % h->length];
  if (h->comp_function(k, ((HashMapEntry *)(u.head->data))->key)) {
    void *next = u.head->next;
    free(u.head);
    h->bucket[h->hash_function(k) % h->length].head = next;
    deleted = 1;
  }
  LinkedListLink *prev = u.head;
  LinkedListLink *cur = u.head->next;

  while (cur && !deleted)
    if (h->comp_function(k, ((HashMapEntry *)(cur->data))->key)) {
      void *next = cur->next;
      callback(cur);
      prev->next = next;
      deleted = 1;
    }

  if (deleted && (float)h->length / (float)h->size < HASHMAP_OCCUP_MIN) {
    return shrink(h);
  }

  return 0;
}

void hash_map_free_callback(HashMap *h, void (*callback)(void *)) {
  for (int i = 0; i < h->length; i++)
    linked_list_free_callback(&h->bucket[i], callback);
  free(h->bucket);
}
void hash_map_free(HashMap *h) { return hash_map_free_callback(h, free); }
void hash_map_free_void(void *h) { return hash_map_free(h); }
