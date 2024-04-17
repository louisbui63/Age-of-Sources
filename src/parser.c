#include <stdio.h>
#include <string.h>

#include "data_structures/asset_manager.h"
#include "parser.h"
#include "units/units.h"
#include "util.h"

#define parser_get_uint16_t(field)                                             \
  {                                                                            \
    c1 = fgetc(f);                                                             \
    c2 = fgetc(f);                                                             \
    while ((c1 != '/') && (c2 != '*')) {                                       \
      c1 = c2;                                                                 \
      c2 = fgetc(f);                                                           \
    }                                                                          \
    fgetc(f);                                                                  \
    tempuint16_t = 0;                                                          \
    for (c = fgetc(f); c != '*'; c = fgetc(f)) {                               \
      overf = tempuint16_t;                                                    \
      tempuint16_t = tempuint16_t * 10 + (c - '0');                            \
      if (overf > tempuint16_t) {                                              \
        HANDLE_ERROR(1, "overflow in a parsed uint16_t field", abort())        \
      }                                                                        \
    }                                                                          \
    fgetc(f);                                                                  \
    br->field = tempuint16_t;                                                  \
  }

#define parser_get_int(field)                                                  \
  {                                                                            \
    c1 = fgetc(f);                                                             \
    c2 = fgetc(f);                                                             \
    while ((c1 != '/') && (c2 != '*')) {                                       \
      c1 = c2;                                                                 \
      c2 = fgetc(f);                                                           \
    }                                                                          \
    fgetc(f);                                                                  \
    tempint = 0;                                                               \
    for (c = fgetc(f); c != '*'; c = fgetc(f)) {                               \
      tempint = tempint * 10 + (c - '0');                                      \
    }                                                                          \
    fgetc(f);                                                                  \
    br->field = tempint;                                                       \
  }

unit *parse(char *path, SDL_Renderer *renderer, SDL_Window *window) {
  FILE *f = fopen(path, 'r');
  char c;
  char c1;
  char c2;
  uint16_t tempuint16_t;
  uint64_t overf;
  int tempint;
  int i;
  unit *br = malloc(sizeof(unit));
  SDL_Rect *rec = calloc(1, sizeof(SDL_Rect));
  Sprite *spr = malloc(sizeof(Sprite));
  spr->rect = rec;
  br->sprite = spr;

  // parsing of the Name
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') && (c2 != '*')) {
    c1 = c2;
    c2 = fgetc(f);
  }
  char *temp = calloc(
      1024, sizeof(char)); // this line defines the maximum length of the name
  for (c = fgetc(f); c != '*'; c = fgetc(f)) {
    if (i == 32) {
      HANDLE_ERROR(1, "Name field too long", abort());
    }
    temp[i++] = c;
  }
  char *nme = malloc(i + 1);
  strcpy(nme, temp);
  br->Name = nme;
  fgetc(f); // this is to clean the '/' in '*/'

  // parsing of the HP
  parser_get_uint16_t(HP);

  // parsing of the BDam
  parser_get_uint16_t(BDam);

  // parsing of the PDam
  parser_get_uint16_t(PDam);

  // parsing of the SDam
  parser_get_uint16_t(SDam);

  // parsing of the BDef
  parser_get_uint16_t(BDef);

  // parsing of the PDef
  parser_get_uint16_t(PDef);

  // parsing of the SDef
  parser_get_uint16_t(SDef);

  // parsing of the Rg
  parser_get_uint16_t(Rg);

  // parsing of the Sp
  parser_get_uint16_t(Sp);

  // parsing of the Sprite's rect's w
  parser_get_int(sprite->rect->w);

  // parsing of the256 Sprite's rect's h
  parser_get_int(sprite->rect->h);

  // parsing of the Sprite's rect's texture
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') && (c2 != '*')) {
    c1 = c2;
    c2 = fgetc(f);
  }
  for (c = fgetc(f); c != '*'; c = fgetc(f)) {
    if (i == 256) {
      HANDLE_ERROR(1, "Path field too long", abort());
    }
    temp[i++] = c;
  }
  temp[i] = '\0';
  br->sprite->texture = get_texture(temp, renderer, window);
  fgetc(f); // this is to clean the '/' in '*/'

  // parsing of the unit's description
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') && (c2 != '*')) {
    c1 = c2;
    c2 = fgetc(f);
  }
  for (c = fgetc(f); c != '*'; c = fgetc(f)) {
    if (i == 1024) {
      HANDLE_ERROR(1, "Descr field too long", abort());
    }
    temp[i++] = c;
  }
  temp[i] = '\0';
  char *descr = malloc(i + 1);
  strcpy(descr, temp);
  br->Descr = descr;
  fgetc(f); // this is to clean the '/' in '*/'

  free(temp);
  return br;
}
