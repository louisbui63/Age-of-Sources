#include <stdlib.h>

#include "parser.h"

#include "../src/parser.h"
#include "../src/util.h"

int test_parser() {
  UnitT *unit = parse("./tests/testunit.h", NULL, NULL);

  ASSERT(unit->b_dam == 100);
  ASSERT(unit->b_def == 20);
  ASSERT(!strcmp(unit->name, "testname"))
  ASSERT(!strcmp(unit->descr, "test descr"));

  free(unit->descr);
  free(unit->name);
  sprite_component_free(unit->sprite);
  free(unit);
  return SUCCESS;
}
