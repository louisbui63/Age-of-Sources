//! @file linked_list.h
#pragma once

//! A link of `LinkedList`
typedef struct _Lk {
  //! Pointer to this link's data. Figuring out which type it is is up to the
  //! user.
  void *data;
  //! Pointer to the next link in the list. `NULL` if last.
  struct _Lk *next;
  // next == NULL means it is the last element
} LinkedListLink;

//! A singly linked list
typedef struct {
  //! Pointer to the the first link of the list. `NULL` if empty.
  LinkedListLink *head;
} LinkedList;

//! Creates a `LinkedList`
LinkedList linked_list_create();
//! Add `e` as an element of `l` at index `i`
//! Returns `0` on success, `-1` on allocation error and `-2` if i is out of
//! range
int linked_list_insert(LinkedList *l, void *e, int i);
//! Same as `linked_list_remove_callback`, with `free` as the `callback`
int linked_list_remove(LinkedList *l, int i);
//! Removes element at index `i` in `l`, running `callback` on its `data` as a
//! way to free it
int linked_list_remove_callback(LinkedList *l, int i, void (*callback)(void *));
//! Same as `linked_list_free`, with `free` as the `callback`
void linked_list_free(LinkedList *);
//! Frees `l`, calling `callback` on the `data` fields of each link as a way to
//! free them
void linked_list_free_callback(LinkedList *l, void (*callback)(void *));
//! Returns the `data` field of the `i`th element of `l`
void *linked_list_get(LinkedList *l, int i);
