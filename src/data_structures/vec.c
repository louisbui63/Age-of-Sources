#include "vec.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void *vec_new_inner(int size) {
  void *vec = malloc(size * VEC_INIT_CAPACITY + 3 * sizeof(uint));
  ((uint *)vec)[0] = size;
  ((uint *)vec)[1] = VEC_INIT_CAPACITY;
  ((uint *)vec)[2] = 0;

  return (uint8_t *)vec + 3 * sizeof(uint);
}

inline void vec_free(void *vec) { free((uint8_t *)vec - 3 * sizeof(uint)); }

void *vec_push_inner(void *vec, void *obj) {
  void *v = vec;
  uint size = ((uint *)v)[-3];
  uint cap = ((uint *)v)[-2];
  uint len = ((uint *)v)[-1];

  if (cap == len) {
    vec = (uint8_t *)realloc((uint8_t *)vec - 3 * sizeof(uint),
                             size * cap * 2 + 3 * sizeof(uint)) +
          3 * sizeof(int);
    v = vec;
    ((uint *)v)[-2] *= 2;
  }

  memcpy((uint8_t *)v + size * len, obj, size);
  ((uint *)v)[-1]++;

  return v;
}

inline void vec_pop(void *vec) { ((uint *)vec)[-1]--; }
inline uint vec_len(void *vec) { return ((uint *)vec)[-1]; }
static inline uint vec_data_size(void *vec) { return ((uint *)vec)[-3]; }

inline void vec_clear(VEC(void) vec) { ((uint *)vec)[-1] = 0; }

char u64_gt(void *a, void *b) { return *(uint64_t *)a > *(uint64_t *)b; }

void vec_sort(void *v, char (*gt)(void *, void *)) {
  int sz = vec_data_size(v);
  if (vec_len(v) == 0 || vec_len(v) == 1)
    return;
  if (vec_len(v) == 2) {
    if (gt(v, (uint8_t *)v + sz)) {
      void *u = malloc(sz);
      memcpy(u, v, sz);
      memcpy(v, (uint8_t *)v + sz, sz);
      memcpy((uint8_t *)v + sz, u, sz);
      free(u);
    }
    return;
  }
  void *left = vec_new_inner(sz);
  void *right = vec_new_inner(sz);
  for (uint i = 0; i < vec_len(v); i++) {
    if (i % 2 == 0)
      left = vec_push_inner(left, (uint8_t *)v + i * sz);
    else
      right = vec_push_inner(right, (uint8_t *)v + i * sz);
  }
  vec_sort(left, gt);
  vec_sort(right, gt);

  uint i = 0, j = 0, k = 0;
  while (i < vec_len(left) && j < vec_len(right)) {
    if (j < vec_len(right) &&
        (i >= vec_len(left) ||
         gt((uint8_t *)left + i * sz, (uint8_t *)right + j * sz))) {
      memcpy((uint8_t *)v + k * sz, (uint8_t *)right + j * sz, sz);
      j++;
    } else {
      memcpy((uint8_t *)v + k * sz, (uint8_t *)left + i * sz, sz);
      i++;
    }
    k++;
  }
  vec_free(left);
  vec_free(right);
}

void vec_swap(void *v, int a, int b) {
  if (a == b)
    return;
  uint sz = vec_data_size(v);
  void *u = malloc(sz);
  memcpy(u, (uint8_t *)v + a * sz, sz);
  memcpy((uint8_t *)v + a * sz, (uint8_t *)v + b * sz, sz);
  memcpy((uint8_t *)v + b * sz, u, sz);
  free(u);
}

void vec_remove(void *vec, int a) {
  uint size = ((uint *)vec)[-3];
  void *temp = malloc((vec_len(vec) - (a + 1)) * size);
  memcpy(temp, (uint8_t *)vec + (a + 1) * size,
         (vec_len(vec) - (a + 1)) * size);
  memcpy((uint8_t *)vec + a * size, temp, (vec_len(vec) - (a + 1)) * size);
  vec_pop(vec);
  free(temp);
}

void *vec_copy(void *vec) {
  uint size = ((uint *)vec)[-3];
  uint cap = ((uint *)vec)[-2];
  uint len = ((uint *)vec)[-1];
  void *veccp = malloc(cap * size + 3 * sizeof(uint));
  memcpy((uint8_t *)veccp, (uint8_t *)vec - 3 * sizeof(uint),
         len * size + 3 * sizeof(uint));
  veccp = (uint8_t *)veccp + 3 * sizeof(uint);
  return veccp;
}

void vec_reverse(void *vec) {
  uint size = ((uint *)vec)[-3];
  uint len = ((uint *)vec)[-1];
  void *cpy = vec_copy(vec);
  for (uint i = 0; i < vec_len(vec); i++) {
    memcpy((uint8_t *)vec + i * size, (uint8_t *)cpy + (len - 1 - i) * size,
           size);
  }
  vec_free(cpy);
}
