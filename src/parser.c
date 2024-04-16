#include <stdio.h>
#include <string.h>

#include "parser.h"

body *parse(char *path) {
  FILE *f = fopen(path, 'r');
  char c;
  char c1;
  char c2;
  int i;
  body *br = malloc(sizeof(body));

  // parsing of the name
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') && (c2 != '*')) {
    c1 = c2;
    c2 = fgetc(f);
  }
  fgetc(f); // this fgetc is to avoid the space after '//'.
  /*a*/
  char *temp = malloc(32);
  for (c = fgetc(f); c != '*'; c = fgetc(f)) {
    temp[i++] = c;
  }
  char *nme = malloc(strlen(temp));
  strcpy(nme, temp);
  br->name = nme;
  free(temp);
}
