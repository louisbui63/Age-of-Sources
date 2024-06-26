#include "vec.h"

#include "../src/data_structures/vec.h"
#include "../src/util.h"

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
  int *tvcp = vec_copy(tv);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY; i++) {
    ASSERT(tv[i] == tvcp[i]);
  }
  vec_remove(tvcp, 0);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY - 1; i++) {
    ASSERT(tv[i + 1] == tvcp[i]);
  }
  vec_free(tvcp);
  tvcp = vec_copy(tv);
  vec_remove(tvcp, vec_len(tvcp) - 1);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY - 1; i++) {
    ASSERT(tv[i] == tvcp[i]);
  }
  vec_free(tvcp);
  tvcp = vec_copy(tv);
  vec_remove(tvcp, (vec_len(tvcp) - 1) / 2);
  uint delta = 0;
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY - 1; i++) {
    if (i == ((vec_len(tvcp) - 1) / 2)) {
      delta++;
    }
    ASSERT(tv[i + delta] == tvcp[i]);
  }
  vec_free(tv);
  vec_free(tvcp);

  tv = vec_new(int);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY; i++)
    vec_push(tv, i);
  vec_reverse(tv);
  for (int i = 0; i < 2 * VEC_INIT_CAPACITY; i++)
    ASSERT(tv[i] == 2 * VEC_INIT_CAPACITY - 1 - i);
  vec_free(tv);

  return SUCCESS;
}
