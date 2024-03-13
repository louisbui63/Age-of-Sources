//! @file vec.h
//! file This file defines a redimensionnable array, hereafter reffered to as
//! `vec`. Relevent informations about the content of the `vec` are stored just
//! before the pointer that the user manipulates
#pragma once

#include "util.h"

//! A macro that extends to a pointer to `x`, to differentiate vectors from
//! arbitrary pointers
#define VEC(x) x *

//! The length of a `vec` at creation
#define VEC_INIT_CAPACITY 16

//! Creates a new `vec` for type `type`
#define vec_new(type) (vec_new_inner(sizeof(type)))
VEC(void) vec_new_inner(int);

//! frees a `vec`. This should always be used instead of `free(vec)`
void vec_free(VEC(void) vec);

//! adds a copy of `obj` at the end of `vec`
#define vec_push(vec, obj)                                                     \
  vec = (vec_push_inner(((void *)(vec)), (void *)&(obj)))
//! adds a copy of what `obj` points to at the end of `vec`. returns a
//! potentially new pointer to the vec
VEC(void) vec_push_inner(VEC(void) vec, void *obj);

//! removes the last element of the `vec`. Doesn't return it for optimisation
//! purposes
void vec_pop(VEC(void) vec);

//! returns the length of `vec`. This is a `O(1)` operation.
uint vec_len(VEC(void) vec);

//! sorts `vec` in place, using `gt` as a way to compare elements. `gt`'s
//! parameters are pointers to the actually compared data, and `gt` returns true
//! iff `a>=b`.
//! `vec_sort` uses merge sort and is consequentially in `O(n*log(n))`
void vec_sort(VEC(void) vec, char (*gt)(void *a, void *b));

//! swaps the elements at index `a` and `b` in `vec`
void vec_swap(VEC(void) vec, int a, int b);

//! `a` and `b` are assumed to be `uint64_t`. returns true iff
//! `&(uint64_t*)a>=&(uint64_t*)b`. Used for `vec_sort`
char u64_gt(void *a, void *b);

//! removes element at index `a` in `vec`
void vec_remove(void *vec, int a);

//! copies `vec` and returns the copy
VEC(void) vec_copy(VEC(void) vec);
