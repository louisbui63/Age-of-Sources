#include "linked_list.h"

#include "../src/data_structures/linked_list.h"
#include "../src/errors.h"
#include "../src/util.h"

#include <stdlib.h>

int test_linked_list() {
  // thes tests might not be sufficient
  LinkedList ll = linked_list_create();
  ASSERT(linked_list_get(&ll, 0) == 0);
  for (int i = 0; i < 32; i++) {
    int *u = malloc(sizeof(int));
    *u = i + 1;
    int status = linked_list_insert(&ll, u, i);
    ASSERT(!status);
  }
  int *u = malloc(sizeof(int));
  *u = 0;
  ASSERT(!linked_list_insert(&ll, u, 0));
  for (int i = 0; i < 33; i++) {
    int *u = linked_list_get(&ll, i);
    ASSERT(u);
    ASSERT(*u == i);
  }
  ASSERT(!linked_list_remove(&ll, 0));
  for (int i = 0; i < 32; i++) {
    ASSERT(*(int *)linked_list_get(&ll, i) == i + 1);
  }
  linked_list_free(&ll);
  return SUCCESS;
}
