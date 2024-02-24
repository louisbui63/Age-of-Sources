// redimensionable array, based on the related TP, with quality of life
// improvements

#pragma once

#include "util.h"
#define VEC_INIT_CAPACITY 16

typedef void *Vec;

#define vec_new(type) (vec_new_inner(sizeof(type)))
void *vec_new_inner(int);

void vec_free(void *vec);

#define vec_push(vec, obj)                                                     \
  vec = (vec_push_inner(((void *)(vec)), (void *)&(obj)))
void *vec_push_inner(void *vec, void *obj);

// currently this doesn't return anything and doesn't return anything
void vec_pop(void *vec);

uint vec_len(void *vec);

void vec_sort(void *vec, char (*gt)(void *, void *));

void vec_swap(void *v, int a, int b);

char u64_gt(void *a, void *b);
