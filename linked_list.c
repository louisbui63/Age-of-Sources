#include "linked_list.h"

LinkedList linked_list_create() {
  LinkedList t = {0};
  return t;
}
int linked_list_insert(LinkedList *l, void *o, int p) {
  //  0 -> SUCCESS
  // -1 -> MEMORY ALLOCATION ERROR
  // -2 -> OUT OF BOUND ERROR
  if (!l->head) {
    if (!p) {
      void *lkaddr = malloc(sizeof(LinkedListLink));
      if (!lkaddr)
        return -1;
      l->head = lkaddr;
      LinkedListLink lk = {o, 0};
      *l->head = lk;
      return 0;
    }
    return -2;
  }
  int cp = 0;
  LinkedListLink *cur = l->head;
  while (cp < p) {
    if (cur->next)
      cur = cur->next;
    else
      return -2;
    cp++;
  }
  void *next = cur->next;
  void *lkaddr = malloc(sizeof(LinkedListLink));
  if (!lkaddr)
    return -1;
  cur->next = lkaddr;
  LinkedListLink lk = {o, next};
  *cur->next = lk;
  return 0;
}
int linked_list_remove(LinkedList *l, int p) {
  //  0 -> SUCCESS
  // -1 -> MEMORY ALLOCATION ERROR
  // -2 -> OUT OF BOUND ERROR
  if (!l->head)
    return -2;
  if (p == 0) {
    void *head = l->head->next;
    free(l->head);
    l->head = head;
    return 0;
  }
  int cp = 0;
  LinkedListLink *cur = l->head;
  while (cp < p - 1) {
    if (cur->next)
      cur = cur->next;
    else
      return -2;
    cp++;
  }
  void *next = cur->next->next;
  free(cur->next);
  cur->next = next;
  return 0;
}

void linked_list_free(LinkedList *l) {
  LinkedListLink *cur = l->head;
  while (cur) {
    LinkedListLink *nc = cur->next;
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
  if (!l->head)
    return 0;
  LinkedListLink *cur = l->head;
  int cp = 0;
  while (cp < p)
    if (cur->next)
      cur = cur->next;
    else
      return 0;
  return cur->data;
}
