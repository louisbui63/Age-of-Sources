#include "vec.h"

#include <stdlib.h>
#include <string.h>

void *vec_new_inner(int size) {
  void *vec = malloc(size * VEC_INIT_CAPACITY + 3 * sizeof(int));
  ((int *)vec)[0] = size;
  ((int *)vec)[1] = VEC_INIT_CAPACITY;
  ((int *)vec)[2] = 0;

  return vec + 3 * sizeof(int);
}

inline void vec_free(void *vec) { free(vec - 3 * sizeof(int)); }

void vec_push_inner(void **vec, void *obj) {
  void *v = *vec;
  int size = ((int *)v)[-3];
  int cap = ((int *)v)[-2];
  int len = ((int *)v)[-1];

  if (cap == len) {
    *vec = realloc(vec - 3 * sizeof(int), size * cap * 2) + 3 * sizeof(int);
    v = *vec;
    ((int *)v)[-2] *= 2;
  }

  memcpy(v + size * (len + 1), obj, size);
  ((int *)v)[-3]++;
}

int vec_len(void *vec) { return ((int *)vec)[-1]; }
