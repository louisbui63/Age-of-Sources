#include <stdio.h>

#include "ecs.h"
#include "hash_map.h"
#include "input.h"
#include "linked_list.h"
#include "pqueue.h"
#include "vec.h"

#define TEST(fn, name)                                                         \
  {                                                                            \
    printf("testing %s : \n", name);                                           \
    if ((fn)())                                                                \
      fprintf(stderr, "test %s \033[38;5;31mFAILED\033[0m\n", name);           \
    else                                                                       \
      printf("test %s \033[38;5;32mSUCCESS\033[0m\n\n", name);                 \
  }

int main() {
  // note that test failure will probably leak memory. This isn't really a
  // problem since the tests don't serve any real purpose other than making sure
  // they don't fail, and there is nothing happening after them except other
  // tests.
  TEST(test_vec, "vec");
  TEST(test_linked_list, "linked_list");
  TEST(test_hash_map, "hash_map");
  TEST(test_ecs, "ecs");
  TEST(test_input, "input");
  TEST(test_pqueue, "pqueue");
  return 0;
}
