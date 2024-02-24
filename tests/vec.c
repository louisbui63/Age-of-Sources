#include "vec.h"

#include "../util.h"
#include "../vec.h"

int test_vec() {
  int *tv = vec_new(int);
  ASSERT(tv);
  ASSERT(vec_len(tv) == 0);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY; i++) {
    vec_push(tv, i);
    ASSERT(tv);
  }
  ASSERT(vec_len(tv) == 2 * VEC_INIT_CAPACITY);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY; i++) {
    ASSERT(tv[i] == i);
  }
  vec_free(tv);

  return 0;
}
