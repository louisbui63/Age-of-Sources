#include "asset_manager.h"

#include "util.h"

HashMap ASSET_STORE;

int init_asset_manager() {
  ASSET_STORE = hash_map_create(hash_str, not_strcmp);
  return 0;
}
