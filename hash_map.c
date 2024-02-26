#include "hash_map.h"
#include "linked_list.h"
#include "util.h"

#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET_BASIS 0xcbf29ce484222325

// insert calls delete which might shrink just before growing again. We
// shouldn't do that

void hash_map_entry_free(void *u) {
  HashMapEntry *w = u;
  free(w->key);
  free(w->value);
  free(u);
}

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
  uint64_t ns = h->length * 2;
  LinkedList *nb = malloc(sizeof(LinkedList) * ns);
  for (uint i = 0; i < ns; i++) {
    // todo : replace this with a more efficient init routine
    LinkedList tmp = {0};
    nb[i] = tmp;
  }
  for (uint i = 0; i < h->length; i++) {
    LinkedListLink *cur = h->bucket[i].head;
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
  free(h->bucket);
  h->length *= 2;
  h->bucket = nb;
  return 0;
}
int shrink(HashMap *h) {
  uint64_t ns = h->length / 2;
  if (ns < HASHMAP_DEFAULT_LENGTH)
    return 0;
  LinkedList *nb = malloc(sizeof(LinkedList) * ns);
  for (uint i = 0; i < ns; i++) {
    // todo : replace this with a more efficient init routine
    LinkedList tmp = {0};
    nb[i] = tmp;
  }
  for (uint i = 0; i < h->length; i++) {
    LinkedListLink *cur = h->bucket[i].head;
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
  free(h->bucket);
  h->length /= 2;
  h->bucket = nb;
  return 0;
}

int hash_map_insert_callback(HashMap *h, void *k, void *v,
                             void (*callback)(void *)) {
  hash_map_delete_callback(h, k, callback);
  uint64_t hash = h->hash_function(k);
  uint64_t bplace = hash % h->length;

  HashMapEntry *et = malloc(sizeof(HashMapEntry));
  HashMapEntry temp = {k, v, hash};
  *et = temp;
  int err = linked_list_insert(&h->bucket[bplace], et, 0);
  if (err)
    return err;
  h->size++;

  if ((float)h->size / (float)h->length > HASHMAP_OCCUP_MAX) {
    return grow(h);
  }
  return 0;
}

void *hash_map_get(HashMap *h, void *k) {
  // can be used to check if the key exists (if not, it returns the null
  // pointer)
  LinkedList u = h->bucket[h->hash_function(k) % (uint64_t)h->length];
  LinkedListLink *cur = u.head;
  while (cur) {
    if (h->comp_function(k, ((HashMapEntry *)(cur->data))->key))
      return ((HashMapEntry *)(cur->data))->value;
    cur = cur->next;
  }
  return 0;
}

int hash_map_insert(HashMap *h, void *k, void *v) {
  return hash_map_insert_callback(h, k, v, hash_map_entry_free);
}

int hash_map_delete(HashMap *h, void *k) {
  return hash_map_delete_callback(h, k, hash_map_entry_free);
}

int hash_map_delete_callback(HashMap *h, void *k, void (*callback)(void *)) {
  char deleted = 0;
  uint64_t td = h->hash_function(k) % (uint64_t)h->length;
  LinkedList u = h->bucket[td];
  if (!u.head) {
    return 0;
  }
  if (h->comp_function(k, ((HashMapEntry *)(u.head->data))->key)) {
    void *next = u.head->next;
    callback(u.head->data);
    free(u.head);
    h->bucket[h->hash_function(k) % (uint64_t)h->length].head = next;
    deleted = 1;
  } else {
    LinkedListLink *prev = u.head;
    LinkedListLink *cur = prev->next;

    while (cur && !deleted)
      if (h->comp_function(k, ((HashMapEntry *)(cur->data))->key)) {
        void *next = cur->next;
        callback(cur->data);
        free(cur);
        prev->next = next;
        deleted = 1;
      }
  }

  if (deleted)
    h->size--;
  if (deleted && (float)h->size / (float)h->length < HASHMAP_OCCUP_MIN) {
    return shrink(h);
  }

  return 0;
}

void hash_map_free_callback(HashMap *h, void (*callback)(void *)) {
  for (uint i = 0; i < h->length; i++)
    linked_list_free_callback(&h->bucket[i], callback);
  free(h->bucket);
}
void hash_map_free(HashMap *h) {
  hash_map_free_callback(h, hash_map_entry_free);
}
void hash_map_free_void(void *h) { hash_map_free(h); }
