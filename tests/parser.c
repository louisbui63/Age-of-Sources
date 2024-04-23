#include <stdlib.h>

#include "parser.h"

#include "../src/parser.h"
#include "../src/util.h"

int test_parser(SDL_Renderer *renderer, SDL_Window *window) {
  Unit *unit = parse("./testunit.h", renderer, window);

  ASSERT(unit->b_dam == 100);
  ASSERT(unit->b_def == 20);
  ASSERT(!strcmp(unit->name, "testname"))
  ASSERT(!strcmp(unit->descr, "test descr"));
}
