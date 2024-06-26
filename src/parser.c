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
    while ((c1 != '/') || (c2 != '*')) {                                       \
      c1 = c2;                                                                 \
      c2 = fgetc(f);                                                           \
    }                                                                          \
    tempuint16 = 0;                                                            \
    for (c = fgetc(f); c != '*'; c = fgetc(f)) {                               \
      overf = tempuint16;                                                      \
      tempuint16 = tempuint16 * 10 + (c - '0');                                \
      if (overf > tempuint16) {                                                \
        HANDLE_ERROR(1, "overflow in a parsed uint16_t field", abort())        \
      }                                                                        \
    }                                                                          \
    fgetc(f);                                                                  \
    br->field = tempuint16;                                                    \
  }

#define parser_get_int(field)                                                  \
  {                                                                            \
    c1 = fgetc(f);                                                             \
    c2 = fgetc(f);                                                             \
    while ((c1 != '/') || (c2 != '*')) {                                       \
      c1 = c2;                                                                 \
      c2 = fgetc(f);                                                           \
    }                                                                          \
    tempint = 0;                                                               \
    for (c = fgetc(f); c != '*'; c = fgetc(f)) {                               \
      tempint = tempint * 10 + (c - '0');                                      \
    }                                                                          \
    fgetc(f);                                                                  \
    br->field = tempint;                                                       \
  }

UnitT *parse(char *path, SDL_Renderer *renderer, SDL_Window *window) {
  // printf("$%s\n", path);
  FILE *f = fopen(path, "r");
  char c;
  char c1;
  char c2;
  uint16_t tempuint16;
  uint64_t overf;
  int tempint = 0;
  int i;
  UnitT *br = malloc(sizeof(Unit));
  SDL_Rect *rec = calloc(1, sizeof(SDL_Rect));
  Sprite *spr = malloc(sizeof(Sprite));
  spr->rect = rec;
  br->sprite = spr;

  // parsing of the Name
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') || (c2 != '*')) {
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
  // printf("%s\n", temp);
  br->name = nme;
  fgetc(f); // this is to clean the '/' in '*/'

  // parsing of the hp
  parser_get_uint16_t(hp);

  // parsing of the b_dam
  parser_get_uint16_t(b_dam);

  // parsing of the p_dam
  parser_get_uint16_t(p_dam);

  // parsing of the s_dam
  parser_get_uint16_t(s_dam);

  // parsing of the b_def
  parser_get_uint16_t(b_def);

  // parsing of the p_def
  parser_get_uint16_t(p_def);

  // parsing of the s_def
  parser_get_uint16_t(s_def);

  // parsing of the rg
  parser_get_uint16_t(rg);

  // parsing of the sp
  parser_get_uint16_t(sp);

  // parsing of the Sprite's rect's w
  parser_get_int(sprite->rect->w);

  // parsing of the256 Sprite's rect's h
  parser_get_int(sprite->rect->h);

  // parsing of the Sprite's rect's texture
#ifndef IS_TEST
  // Lors des tests il n'y a pas de renderer et de window donc on ne peut pas
  // charger la texture
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') || (c2 != '*')) {
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
  br->path_to_sprite = malloc(strlen(temp) + 1);
  strcpy(br->path_to_sprite, temp);
  // printf("%s\n", temp);
  fgetc(f); // this is to clean the '/' in '*/'
#endif

  // parsing of the unit's description
  i = 0;
  c1 = fgetc(f);
  c2 = fgetc(f);
  while ((c1 != '/') || (c2 != '*')) {
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
  // printf("%s\n", temp);
  br->descr = descr;
  fgetc(f); // this is to clean the '/' in '*/'

  free(temp);
  fclose(f);
  return br;
}
