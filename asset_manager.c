#include "asset_manager.h"

HashMap ASSET_STORE;

char not_strcmp(void *a, void *b) {
  return !(char)strcmp((char *)a, (char *)b);
}

int init_asset_manager() {
  ASSET_STORE = hash_map_create(hash_str, not_strcmp);
  return 0;
}
