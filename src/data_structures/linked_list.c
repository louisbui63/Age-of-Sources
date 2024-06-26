#include "linked_list.h"
#include "../errors.h"
#include <stdlib.h>

LinkedList linked_list_create() {
  LinkedList t = {0};
  return t;
}
int linked_list_insert(LinkedList *l, void *o, int p) {
  if (!p) {
    void *oh = l->head;
    void *lkaddr = malloc(sizeof(LinkedListLink));
    if (!lkaddr)
      return OUT_OF_MEMORY;
    l->head = lkaddr;
    LinkedListLink lk = {o, oh};
    *l->head = lk;
    return SUCCESS;
  }
  if (!l->head) {
    return INDEX_OUT_OF_RANGE;
  }
  int cp = 0;
  LinkedListLink *cur = l->head;
  while (cp < p - 1) {
    if (cur->next)
      cur = cur->next;
    else
      return INDEX_OUT_OF_RANGE;
    cp++;
  }
  void *next = cur->next;
  void *lkaddr = malloc(sizeof(LinkedListLink));
  if (!lkaddr)
    return OUT_OF_MEMORY;
  cur->next = lkaddr;
  LinkedListLink lk = {o, next};
  *cur->next = lk;
  return SUCCESS;
}

int linked_list_remove_callback(LinkedList *l, int p,
                                void (*callback)(void *)) {
  if (!l->head)
    return INDEX_OUT_OF_RANGE;
  if (!p) {
    void *head = 0;
    if (l->head) {
      head = l->head->next;
      callback(l->head->data);
    }
    free(l->head);
    l->head = head;
    return SUCCESS;
  }
  int cp = 0;
  LinkedListLink *cur = l->head;
  while (cp < p - 1) {
    if (cur->next)
      cur = cur->next;
    else
      return INDEX_OUT_OF_RANGE;
    cp++;
  }
  void *next = 0;
  if (cur->next) {
    next = cur->next->next;
    callback(cur->next->data);
  }
  free(cur->next);
  cur->next = next;
  return SUCCESS;
}
int linked_list_remove(LinkedList *l, int p) {
  return linked_list_remove_callback(l, p, free);
}

void linked_list_free(LinkedList *l) {
  LinkedListLink *cur = l->head;
  while (cur) {
    LinkedListLink *nc = cur->next;
    free(cur->data);
    free(cur);
    cur = nc;
  }
}
void linked_list_free_callback(LinkedList *l, void (*callback)(void *)) {
  LinkedListLink *cur = l->head;
  while (cur) {
    LinkedListLink *nc = cur->next;
    callback(cur->data);
    free(cur);
    cur = nc;
  }
}

void *linked_list_get(LinkedList *l, int p) {
  // returns a null pointer if p is out of range
  if (!l->head) {
    return 0;
  }
  LinkedListLink *cur = l->head;
  int cp = 0;
  while (cp < p) {
    if (cur->next)
      cur = cur->next;
    else
      return 0;
    cp++;
  }
  return cur->data;
}
