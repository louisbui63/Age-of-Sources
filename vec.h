// redimensionable array, based on the related TP, with quality of life
// improvements

#pragma once

#define VEC_INIT_CAPACITY 16

typedef void *Vec;

#define vec_new(type) (vec_new_inner(sizeof(type)))
void *vec_new_inner(int);

void vec_free(void *vec);

#define vec_push(vec, obj) (vec_push_inner(((void **)(&vec)), (void *)&(obj)))
void vec_push_inner(void **vec, void *obj);

int vec_len(void *vec);
