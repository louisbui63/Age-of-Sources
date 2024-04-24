#include <stdlib.h>

#include "parser.h"

#include "../src/parser.h"
#include "../src/util.h"

int test_parser() {
  Unit *unit = parse("./tests/testunit.h", NULL, NULL);

  ASSERT(unit->b_dam == 100);
  ASSERT(unit->b_def == 20);
  ASSERT(!strcmp(unit->name, "testname"))
  ASSERT(!strcmp(unit->descr, "test descr"));

  return SUCCESS;
}
