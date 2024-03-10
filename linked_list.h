#pragma once

typedef struct _Lk {
  void *data;
  struct _Lk *next;
  // next == NULL means it is the last element
} LinkedListLink;

typedef struct {
  LinkedListLink *head;
} LinkedList;

LinkedList linked_list_create();
int linked_list_insert(LinkedList *, void *, int);
int linked_list_remove(LinkedList *, int);
int linked_list_remove_callback(LinkedList *, int, void (*)(void *));
void linked_list_free(LinkedList *);
void linked_list_free_callback(LinkedList *, void (*)(void *));
void *linked_list_get(LinkedList *, int);
