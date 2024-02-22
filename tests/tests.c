#include <stdio.h>

#include "linked_list.h"
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
  TEST(test_vec, "vec");
  TEST(test_linked_list, "linked_list");
  return 0;
}
