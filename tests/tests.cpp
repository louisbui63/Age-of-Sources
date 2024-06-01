#include <stdio.h>

#include "ecs.hpp"
// #include "input.hh"
// #include "parser.hh"
// #include "pathfinding.hh"
#include "vec2.hpp"

int RUNNING = 1;
char IS_FULLSCREEN = 0;
// HashMap GRID_FUNCTION_MAP;

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
  TEST(test_vec2, "vec2");
  // TEST(test_input, "input");
  // TEST(test_pathfinding, "pathfinding");
  TEST(test_ecs, "ecs");
  // TEST(test_parser, "parser");
  return 0;
}
