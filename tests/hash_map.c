#include "hash_map.h"

#include "../hash_map.h"
#include "../util.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int test_hash_map() {
  HashMap hm = hash_map_create(hash_str, not_strcmp);

  for (int i = 0; i < 32; i++) {
    char *k = malloc(8);
    strcpy(k, "0wu");
    uint64_t *v = malloc(sizeof(uint64_t));
    *v = i;
    *k += i;
    ASSERT(!hash_map_insert(&hm, k, v));
  }
  for (int i = 0; i < 32; i++) {
    char k[] = "0wu";
    *k += i;
    int *w = hash_map_get(&hm, k);
    ASSERT(w);
    ASSERT(*w == i);
  }
  for (int i = 0; i < 31; i++) {
    char k[] = "0wu";
    *k += i;
    ASSERT(!hash_map_delete(&hm, k));
  }
  int *w = hash_map_get(&hm, "Owu" /*<- this isn't a typo*/);
  ASSERT(w);
  ASSERT(*w = 31);
  hash_map_free(&hm);

  return 0;
}
