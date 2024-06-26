#include <string.h>

#include "../audio/audio.h"
#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "../players.h"
#include "../selection.h"
#include "button.h"

extern Running RUNNING;
extern char IS_FULLSCREEN;

Clickable *spawn_main_quit(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_main_quit(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_main_option(World *w, SDL_Renderer *renderer,
                             SDL_Window *window);

void event_main_option(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_main_start(World *w, SDL_Renderer *renderer,
                            SDL_Window *window);

void event_main_start(World *w, SDL_Renderer *renderer, SDL_Window *window);

void spawn_optionmain_menu(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

Clickable *spawn_optionmain_back(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window);

void event_optionmain_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

Background *spawn_option_background(World *w, SDL_Renderer *renderer,
                                    SDL_Window *window);

Clickable *spawn_option_fullscreen(World *w, SDL_Renderer *renderer,
                                   SDL_Window *window);

void event_option_fullscreen(World *w, SDL_Renderer *renderer,
                             SDL_Window *window);

Clickable *spawn_option_music(World *w, SDL_Renderer *renderer,
                              SDL_Window *window);

void event_option_music(World *w, SDL_Renderer *renderer, SDL_Window *window);

void event_gamemenu_resume(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_gamemenu_option(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_gamemenu_quit(World *w, SDL_Renderer *renderer, SDL_Window *window);

void spawn_game_ui(World *w, SDL_Renderer *renderer, SDL_Window *window);

Background *spawn_game_background(World *w, SDL_Renderer *renderer,
                                  SDL_Window *window);

void spawn_game_ressources(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

char *get_water(World *w, Entity *e);

char *get_clay(World *w, Entity *e);

void spawn_gameoption_menu(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_gameoption_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_gameoption_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

Background *spawn_gamemenu_background(World *w, SDL_Renderer *renderer,
                                      SDL_Window *window);

Clickable *spawn_gamemenu_resume(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window);

Clickable *spawn_gamemenu_quit(World *w, SDL_Renderer *renderer,
                               SDL_Window *window);

Clickable *spawn_gamemenu_option(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window);

Clickable *spawn_gameoption_back(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window);
void spawn_option_sound(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_sound_p1(World *w, SDL_Renderer *renderer, SDL_Window *window);

void event_sound_p1(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_sound_p16(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_sound_p16(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_sound_m1(World *w, SDL_Renderer *renderer, SDL_Window *window);

void event_sound_m1(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_sound_m16(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_sound_m16(World *w, SDL_Renderer *renderer, SDL_Window *window);

ActualisedText *spawn_sound_level(World *w, SDL_Renderer *renderer,
                                  SDL_Window *window);

Background *spawn_sound_levelbg(World *w, SDL_Renderer *renderer,
                                SDL_Window *window);

Clickable *spawn_sound_test(World *w, SDL_Renderer *renderer,
                            SDL_Window *window);

void event_sound_test(__attribute__((unused)) World *w,
                      __attribute__((unused)) SDL_Renderer *renderer,
                      __attribute__((unused)) SDL_Window *window);

void spawn_victory(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_end_exit(World *w, SDL_Renderer *renderer, SDL_Window *window);

void event_end_exit(World *w, SDL_Renderer *renderer, SDL_Window *window);

Background *spawn_end_background(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window);

ActualisedText *spawn_victory_text(World *w, SDL_Renderer *renderer,
                                   SDL_Window *window);

char *str_victory_text(World *w, Entity *e);

void spawn_defeat(World *w, SDL_Renderer *renderer, SDL_Window *window);

ActualisedText *spawn_defeat_text(World *w,

                                  SDL_Renderer *renderer, SDL_Window *window);
char *str_defeat_text(World *w, Entity *e);

//
//
//

Clickable *spawn_button(World *w, SDL_Renderer *renderer, SDL_Window *window,
                        void (*event)(World *w, SDL_Renderer *renderer,
                                      SDL_Window *window),
                        char *t, int xp, int yp) {
  Clickable *click = malloc(sizeof(Clickable));
  click->sprite = malloc(sizeof(Sprite));
  click->rect = malloc(sizeof(SDL_Rect));
  click->sprite->rect = malloc(sizeof(SDL_Rect));
  click->text = malloc(sizeof(Text));
  click->text->color = malloc(sizeof(SDL_Color));
  click->is_clicked = 0;
  click->click_event = event;
  *click->text->color = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  click->text->str = malloc(strlen(t) + 1);
  strcpy(click->text->str, t);
  click->text->padding = 5;
  click->rect->x = xp;
  click->rect->y = yp;
  click->rect->w = 100;
  click->rect->h = 30;
  click->sprite->rect->x = 0;
  click->sprite->rect->y = 0;
  click->sprite->rect->w = 100;
  click->sprite->rect->h = 30;
  click->sprite->texture =
      get_texture("./asset/sprites/button.bmp", renderer, window);
  KeyEvent *key_event = malloc(sizeof(KeyEvent));
  *key_event = clickable_event;
  spawn_clickable(w, click, key_event);
  return click;
}

Clickable *spawn_button_mute(World *w, SDL_Renderer *renderer,
                             SDL_Window *window,
                             void (*event)(World *w, SDL_Renderer *renderer,
                                           SDL_Window *window),
                             char *t, int xp, int yp) {
  Clickable *click = malloc(sizeof(Clickable));
  click->sprite = malloc(sizeof(Sprite));
  click->rect = malloc(sizeof(SDL_Rect));
  click->sprite->rect = malloc(sizeof(SDL_Rect));
  click->text = malloc(sizeof(Text));
  click->text->color = malloc(sizeof(SDL_Color));
  click->is_clicked = 0;
  click->click_event = event;
  *click->text->color = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  click->text->str = malloc(strlen(t) + 1);
  strcpy(click->text->str, t);
  click->text->padding = 5;
  click->rect->x = xp;
  click->rect->y = yp;
  click->rect->w = 100;
  click->rect->h = 30;
  click->sprite->rect->x = 0;
  click->sprite->rect->y = 0;
  click->sprite->rect->w = 100;
  click->sprite->rect->h = 30;
  click->sprite->texture =
      get_texture("./asset/sprites/button.bmp", renderer, window);
  KeyEvent *key_event = malloc(sizeof(KeyEvent));
  *key_event = clickable_event_mute;
  spawn_clickable(w, click, key_event);
  return click;
}

void spawn_main_menu(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  spawn_main_quit(w, renderer, window);
  spawn_main_option(w, renderer, window);
  spawn_main_start(w, renderer, window);
  RUNNING = MAIN;
}

Clickable *spawn_main_quit(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  return spawn_button(w, renderer, window, event_main_quit, "Exit",
                      (WIN_W - 100) / 2, 256);
}

void event_main_quit(__attribute__((unused)) World *w,
                     __attribute__((unused)) SDL_Renderer *renderer,
                     __attribute__((unused)) SDL_Window *window) {
  RUNNING = 0;
}

Clickable *spawn_main_option(World *w, SDL_Renderer *renderer,
                             SDL_Window *window) {
  return spawn_button(w, renderer, window, event_main_option, "Options",
                      (WIN_W - 100) / 2, 192);
}

void event_main_option(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  despawn_from_component(w, COMPF_CLICKABLE);
  // despawn_from_component(w, COMP_HOVERABLE);
  spawn_optionmain_menu(w, renderer, window);
}

Clickable *spawn_main_start(World *w, SDL_Renderer *renderer,
                            SDL_Window *window) {
  return spawn_button(w, renderer, window, event_main_start, "Start",
                      (WIN_W - 100) / 2, 128);
}

void event_main_start(World *w, __attribute__((unused)) SDL_Renderer *renderer,
                      __attribute__((unused)) SDL_Window *window) {
  RUNNING = IN_GAME;
  despawn_from_component(w, COMPF_CLICKABLE);
  spawn_game_ui(w, renderer, window);
  // Entity *e = spawn_entity(w);
}

void spawn_optionmain_menu(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  spawn_optionmain_back(w, renderer, window);
  spawn_option_background(w, renderer, window);
  spawn_option_fullscreen(w, renderer, window);
  spawn_option_sound(w, renderer, window);
  spawn_option_music(w, renderer, window);
  RUNNING = OPTIONMAIN;
}

Clickable *spawn_optionmain_back(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window) {
  return spawn_button(w, renderer, window, event_optionmain_back, "Back",
                      (WIN_W - 100) / 2, (WIN_W - 100) / 2);
}

void event_optionmain_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_ACTUALISEDTEXT);
  spawn_main_menu(w, renderer, window);
}

Background *spawn_option_background(World *w, SDL_Renderer *renderer,
                                    SDL_Window *window) {
  Background *b = malloc(sizeof(Background));
  b->sprite = malloc(sizeof(Sprite));
  b->sprite->rect = malloc(sizeof(SDL_Rect));
  b->sprite->rect->x = 0;
  b->sprite->rect->y = 0;
  b->sprite->rect->w = 600;
  b->sprite->rect->h = 250;
  b->sprite->texture =
      get_texture("./asset/sprites/optionbg.bmp", renderer, window);
  b->rect = malloc(sizeof(SDL_Rect));
  b->rect->w = 500;
  b->rect->h = 200;
  b->rect->x = (WIN_W - b->rect->w) / 2;
  b->rect->y = (WIN_H - b->rect->h) / 2 - WIN_H / 8;
  Entity *e = spawn_entity(w);
  ecs_add_component(w, e, COMP_BACKGROUND, b);
  return b;
}

Clickable *spawn_option_fullscreen(World *w, SDL_Renderer *renderer,
                                   SDL_Window *window) {
  return spawn_button(w, renderer, window, event_option_fullscreen,
                      "Fullscreen", (WIN_W - 500) / 2 + (500 - 300) / 2,
                      (WIN_H - 200) / 2 - WIN_H / 8 + 2 * (210 - 3 * 30) / 3 +
                          45);
}

void event_option_fullscreen(__attribute__((unused)) World *w,
                             __attribute__((unused)) SDL_Renderer *renderer,
                             SDL_Window *window) {
  SDL_DisplayMode dm;
  SDL_GetWindowDisplayMode(window, &dm);
  if (IS_FULLSCREEN) {
    SDL_SetWindowFullscreen(window, 0);
    SDL_SetWindowSize(window, WIN_W, WIN_H);
  } else {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  IS_FULLSCREEN = !IS_FULLSCREEN;
}

Clickable *spawn_option_music(World *w, SDL_Renderer *renderer,
                              SDL_Window *window) {
  return spawn_button(w, renderer, window, event_option_music, "Music",
                      (WIN_W - 500) / 2 + (500 - 300) / 2 + 2 * 100,
                      (WIN_H - 200) / 2 - WIN_H / 8 + 2 * (210 - 3 * 30) / 3 +
                          45);
}

void event_option_music(__attribute__((unused)) World *w,
                        __attribute__((unused)) SDL_Renderer *renderer,
                        __attribute__((unused)) SDL_Window *window) {
  toggle_music();
}

void event_game_menu(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  RUNNING = IN_GAMEMENU;
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_HOVERABLE);
  despawn_from_component(w, COMPF_ACTUALISEDTEXT);
  // This function is unused because of the poor quality of the result.
  // spawn_gamemenu_background(w, renderer, window);
  spawn_gamemenu_resume(w, renderer, window);
  spawn_gamemenu_quit(w, renderer, window);
  spawn_gamemenu_option(w, renderer, window);
}

Background *spawn_gamemenu_background(World *w, SDL_Renderer *renderer,
                                      SDL_Window *window) {
  Entity *e = spawn_entity(w);
  Background *bg = malloc(sizeof(Background));
  bg->rect = malloc(sizeof(SDL_Rect));
  *(bg->rect) = (SDL_Rect){.w = 150, .h = 250, .x = 0, .y = 0};
  bg->sprite = malloc(sizeof(Sprite));
  bg->sprite->rect = malloc(sizeof(SDL_Rect));
  *(bg->sprite->rect) = (SDL_Rect){.w = 120, .h = 250, .x = 231, .y = 46};
  bg->sprite->texture = get_texture(
      "./asset/sprites/ingameoptionbackground.bmp", renderer, window);
  ecs_add_component(w, e, COMP_BACKGROUND, bg);
  return bg;
}

Clickable *spawn_gamemenu_resume(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window) {
  return spawn_button(w, renderer, window, event_gamemenu_resume, "Resume",
                      (WIN_W - 100) / 2, 128);
}

void event_gamemenu_resume(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_HOVERABLE);
  spawn_game_ui(w, renderer, window);
  RUNNING = IN_GAME;
}

Clickable *spawn_gamemenu_quit(World *w, SDL_Renderer *renderer,
                               SDL_Window *window) {
  return spawn_button(w, renderer, window, event_gamemenu_quit, "Quit",
                      (WIN_W - 100) / 2, (WIN_W - 100) / 2);
}

void event_gamemenu_quit(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_HOVERABLE);
  spawn_main_menu(w, renderer, window);
}

Clickable *spawn_gamemenu_option(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window) {
  return spawn_button(w, renderer, window, event_gamemenu_option, "Options",
                      (WIN_W - 100) / 2, 192);
}

void event_gamemenu_option(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_BACKGROUND);
  spawn_gameoption_menu(w, renderer, window);
}

void spawn_gameoption_menu(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  RUNNING = IN_GAMEOPTION;
  spawn_option_background(w, renderer, window);
  spawn_option_fullscreen(w, renderer, window);
  spawn_gameoption_back(w, renderer, window);
  spawn_option_sound(w, renderer, window);
  spawn_option_music(w, renderer, window);
}

Clickable *spawn_gameoption_back(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window) {
  return spawn_button(w, renderer, window, event_gameoption_back, "Back",
                      (WIN_W - 100) / 2, (WIN_W - 100) / 2);
}

void event_gameoption_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_ACTUALISEDTEXT);
  event_game_menu(w, renderer, window);
}

void spawn_game_ui(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  spawn_game_background(w, renderer, window);
  spawn_game_ressources(w, renderer, window);
}

Background *spawn_game_background(World *w, SDL_Renderer *renderer,
                                  SDL_Window *window) {
  Background *back = malloc(sizeof(Background));
  back->rect = malloc(sizeof(SDL_Rect));
  *(back->rect) = (SDL_Rect){.x = 0, .y = 270, .h = 90, .w = 640};
  back->sprite = malloc(sizeof(Sprite));
  back->sprite->rect = malloc(sizeof(SDL_Rect));
  (back->sprite->rect) = NULL;
  back->sprite->texture =
      get_texture("asset/sprites/ingamebackground.bmp", renderer, window);
  Entity *e = spawn_entity(w);
  ecs_add_component(w, e, COMP_BACKGROUND, back);
  return back;
}

void spawn_game_ressources(World *w,
                           __attribute__((unused)) SDL_Renderer *renderer,
                           __attribute__((unused)) SDL_Window *window) {
  ActualisedText *wa = malloc(sizeof(ActualisedText));
  wa->color = malloc(sizeof(SDL_Color));
  *(wa->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  wa->rect = malloc(sizeof(SDL_Rect));
  *(wa->rect) = (SDL_Rect){.x = 15, .y = 285, .h = 17, .w = 100};
  wa->get_text = get_water;
  Entity *ew = spawn_entity(w);
  ecs_add_component(w, ew, COMP_ACTUALISEDTEXT, wa);

  ActualisedText *cl = malloc(sizeof(ActualisedText));
  cl->color = malloc(sizeof(SDL_Color));
  *(cl->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  cl->rect = malloc(sizeof(SDL_Rect));
  *(cl->rect) = (SDL_Rect){.x = 15, .y = 285 + 24, .h = 16, .w = 100};
  cl->get_text = get_clay;
  Entity *ec = spawn_entity(w);
  ecs_add_component(w, ec, COMP_ACTUALISEDTEXT, cl);
}

char *get_water(World *w, __attribute__((unused)) Entity *e) {
  Bitflag flag = COMPF_PLAYERMANAGER;
  VEC(EntityRef) ps = world_query(w, &flag);
  PlayerManager *pm0 =
      entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
  PlayerManager *pm1 =
      entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
  if (pm0->id == 1) {
    PlayerManager *tmp = pm0;
    pm0 = pm1;
    pm1 = tmp;
  }
  int wi = pm0->water;
  char *tmp = malloc(12);
  tmp[11] = 0;
  tmp[10] = ' ';
  tmp[9] = 0x83;
  tmp[8] = 0x81;
  tmp[7] = 0xef;
  tmp[6] = ' ';
  tmp[5] = '0' + wi % 10;
  tmp[4] = ' ';
  tmp[3] = ' ';
  tmp[2] = ' ';
  tmp[1] = ' ';
  tmp[0] = ' ';
  int i = 4;
  wi = wi / 10;
  while ((i >= 0) && (wi)) {
    tmp[i--] = '0' + wi % 10;
    wi = wi / 10;
  }
  return tmp;
}

char *get_clay(World *w, __attribute__((unused)) Entity *e) {
  Bitflag flag = COMPF_PLAYERMANAGER;
  VEC(EntityRef) ps = world_query(w, &flag);
  PlayerManager *pm0 =
      entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
  PlayerManager *pm1 =
      entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
  if (pm0->id == 1) {
    PlayerManager *tmp = pm0;
    pm0 = pm1;
    pm1 = tmp;
  } // ef92b7
  int wi = pm0->clay;
  char *tmp = malloc(12);
  tmp[11] = 0;
  tmp[10] = ' ';
  tmp[9] = 0xb7;
  tmp[8] = 0x92;
  tmp[7] = 0xef;
  tmp[6] = ' ';
  tmp[5] = '0' + wi % 10;
  tmp[4] = ' ';
  tmp[3] = ' ';
  tmp[2] = ' ';
  tmp[1] = ' ';
  tmp[0] = ' ';
  int i = 4;
  wi = wi / 10;
  while ((i >= 0) && (wi)) {
    tmp[i--] = '0' + wi % 10;
    wi = wi / 10;
  }
  return tmp;
}

void key_event_escape(World *w, SDL_Renderer *rdr, Entity *entity, Inputs *in,
                      KeyState keystate) {
  if ((keystate == KEY_RELEASED) && (inputs_is_key_in(in, SDLK_ESCAPE))) {
    Window *wind = entity_get_component(w, entity, COMP_WINDOW);

    switch (RUNNING) {
    case OPTIONMAIN:
      event_optionmain_back(w, rdr, wind->w);
      break;

    case IN_GAME: {
      Bitflag flag = COMPF_SELECTOR;
      Selector *s = entity_get_component(
          w, get_entity(w, world_query(w, &flag)[0]), COMP_SELECTOR);
      if (s->type != Building)
        event_game_menu(w, rdr, wind->w);
      else
        unselect(s);
    } break;

    case IN_GAMEMENU:
      event_gamemenu_resume(w, rdr, wind->w);
      break;

    case IN_GAMEOPTION:
      event_gameoption_back(w, rdr, wind->w);
      break;

    case VICTORY:
      event_end_exit(w, rdr, wind->w);
      break;

    case DEFEAT:
      event_end_exit(w, rdr, wind->w);
      break;

    default:
      break;
    }
  }
}

int soundxpos = (WIN_W - 500) / 2 + (500 - 300) / 2;
int soundypos = (WIN_H - 200) / 2 - WIN_H / 8 + (210 - 3 * 30) / 3;

void spawn_option_sound(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  spawn_sound_p1(w, renderer, window);
  spawn_sound_p16(w, renderer, window);
  spawn_sound_m1(w, renderer, window);
  spawn_sound_m16(w, renderer, window);
  spawn_sound_level(w, renderer, window);
  spawn_sound_levelbg(w, renderer, window);
  spawn_sound_test(w, renderer, window);
}

Clickable *spawn_sound_p1(World *w, SDL_Renderer *renderer,
                          SDL_Window *window) {
  return spawn_button(w, renderer, window, event_sound_p1, "+ 1",
                      soundxpos + 200, soundypos);
}

void event_sound_p1(__attribute__((unused)) World *w,
                    __attribute__((unused)) SDL_Renderer *renderer,
                    __attribute__((unused)) SDL_Window *window) {
  uint8_t v = get_volume();
  set_volume(min(v + 1, 128));
}

Clickable *spawn_sound_p16(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  return spawn_button(w, renderer, window, event_sound_p16, "+ 16",
                      soundxpos + 200, soundypos + 30);
}

void event_sound_p16(__attribute__((unused)) World *w,
                     __attribute__((unused)) SDL_Renderer *renderer,
                     __attribute__((unused)) SDL_Window *window) {
  uint8_t v = get_volume();
  set_volume(min(v + 16, 128));
}

Clickable *spawn_sound_m1(World *w, SDL_Renderer *renderer,
                          SDL_Window *window) {
  return spawn_button(w, renderer, window, event_sound_m1, "- 1", soundxpos,
                      soundypos);
}

void event_sound_m1(__attribute__((unused)) World *w,
                    __attribute__((unused)) SDL_Renderer *renderer,
                    __attribute__((unused)) SDL_Window *window) {
  uint8_t v = get_volume();
  set_volume(max(v - 1, 0));
}

Clickable *spawn_sound_m16(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  return spawn_button(w, renderer, window, event_sound_m16, "- 16", soundxpos,
                      soundypos + 30);
}

void event_sound_m16(__attribute__((unused)) World *w,
                     __attribute__((unused)) SDL_Renderer *renderer,
                     __attribute__((unused)) SDL_Window *window) {
  uint8_t v = get_volume();
  set_volume(max(v - 16, 0));
}

ActualisedText *spawn_sound_level(World *w,
                                  __attribute__((unused))
                                  SDL_Renderer *renderer,
                                  __attribute__((unused)) SDL_Window *window) {
  Entity *e = spawn_entity(w);
  ActualisedText *t = malloc(sizeof(ActualisedText));
  t->color = malloc(sizeof(SDL_Color));
  *(t->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  t->rect = malloc(sizeof(SDL_Rect));
  *(t->rect) =
      (SDL_Rect){.x = soundxpos + 100, .y = soundypos + 5, .h = 20, .w = 100};
  t->get_text = str_sound_level;
  ecs_add_component(w, e, COMP_ACTUALISEDTEXT, t);
  return t;
}

char *str_sound_level(__attribute__((unused)) World *w,
                      __attribute__((unused)) Entity *e) {
  uint8_t v = get_volume();
  char *t = malloc(8);
  strcpy(t, "   /128");
  *(t + 2) = '0' + v % 10;
  v /= 10;

  if ((v / 10) != 0) {
    t[0] = v / 10 + '0';
    t[1] = v % 10 + '0';
  } else if (v != 0) {
    t[1] = v + '0';
  }
  return t;
}

Background *spawn_sound_levelbg(World *w, SDL_Renderer *renderer,
                                SDL_Window *window) {
  Background *bg = malloc(sizeof(Background));
  bg->rect = malloc(sizeof(SDL_Rect));
  *(bg->rect) =
      (SDL_Rect){.x = soundxpos + 100, .y = soundypos, .h = 30, .w = 100};
  bg->sprite = malloc(sizeof(Sprite));
  bg->sprite->rect = malloc(sizeof(SDL_Rect));
  *(bg->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 30, .w = 100};
  bg->sprite->texture =
      get_texture("./asset/sprites/button.bmp", renderer, window);
  Entity *e = spawn_entity(w);
  ecs_add_component(w, e, COMP_BACKGROUND, bg);
  return bg;
}

Clickable *spawn_sound_test(World *w, SDL_Renderer *renderer,
                            SDL_Window *window) {
  return spawn_button_mute(w, renderer, window, event_sound_test, "Test son",
                           soundxpos + 100, soundypos + 30);
}

void event_sound_test(__attribute__((unused)) World *w,
                      __attribute__((unused)) SDL_Renderer *renderer,
                      __attribute__((unused)) SDL_Window *window) {
  play_audio("./asset/sfx/yahoo.wav", 0);
}

void render_vec(World *w, VEC(Clickable *) vec) {
  int n = vec_len(vec);
  int bx = 0;
  int by = 0;
  for (int i = 0; i < n; i++) {
    KeyEvent *key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    vec[i]->rect->x = bx + (i % 3) * 32;
    vec[i]->rect->y = by + (i / 3) * 32;
    spawn_clickable(w, vec[i], key_event);
  }
  vec_free(vec);
}

void spawn_victory(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  RUNNING = VICTORY;
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_HOVERABLE);
  despawn_from_component(w, COMPF_ACTUALISEDTEXT);
  spawn_end_exit(w, renderer, window);
  spawn_end_background(w, renderer, window);
  spawn_victory_text(w, renderer, window);
}

Clickable *spawn_end_exit(World *w, SDL_Renderer *renderer,
                          SDL_Window *window) {
  return spawn_button(w, renderer, window, event_end_exit, "Exit",
                      (WIN_W - 100) / 2, (WIN_W - 100) / 2);
}

void event_end_exit(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  despawn_from_component(w, COMPF_ACTUALISEDTEXT);
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_CLICKABLE);
  spawn_main_menu(w, renderer, window);
}

Background *spawn_end_background(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window) {
  return spawn_option_background(w, renderer, window);
}

ActualisedText *spawn_victory_text(__attribute__((unused)) World *w,
                                   __attribute__((unused))
                                   SDL_Renderer *renderer,
                                   __attribute__((unused)) SDL_Window *window) {
  Entity *e = spawn_entity(w);
  ActualisedText *t = malloc(sizeof(ActualisedText));
  t->color = malloc(sizeof(SDL_Color));
  *(t->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  t->rect = malloc(sizeof(SDL_Rect));
  int padding = 20;
  *(t->rect) = (SDL_Rect){.x = (WIN_W - 500) / 2 + padding,
                          .y = (WIN_H - 200) / 2 - WIN_H / 8 + padding,
                          .h = 200 - 2 * padding,
                          .w = 500 - 2 * padding};
  t->get_text = str_victory_text;
  ecs_add_component(w, e, COMP_ACTUALISEDTEXT, t);
  return t;
}

char *str_victory_text(__attribute__((unused)) World *w,
                       __attribute__((unused)) Entity *e) {
  int n = 20;
  if (!(rand() % n)) {
    play_audio("./asset/sfx/yahoo.wav", 0);
  }

  char *v = "Victory";
  char *t = malloc(strlen(v) + 1);
  strcpy(t, v);
  return t;
}

void spawn_defeat(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  RUNNING = DEFEAT;
  despawn_from_component(w, COMPF_BACKGROUND);
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_HOVERABLE);
  despawn_from_component(w, COMPF_ACTUALISEDTEXT);
  spawn_end_exit(w, renderer, window);
  spawn_end_background(w, renderer, window);
  spawn_defeat_text(w, renderer, window);
}

ActualisedText *spawn_defeat_text(__attribute__((unused)) World *w,
                                  __attribute__((unused))
                                  SDL_Renderer *renderer,
                                  __attribute__((unused)) SDL_Window *window) {
  Entity *e = spawn_entity(w);
  ActualisedText *t = malloc(sizeof(ActualisedText));
  t->color = malloc(sizeof(SDL_Color));
  *(t->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
  t->rect = malloc(sizeof(SDL_Rect));
  int padding = 20;
  *(t->rect) = (SDL_Rect){.x = (WIN_W - 500) / 2 + padding,
                          .y = (WIN_H - 200) / 2 - WIN_H / 8 + padding,
                          .h = 200 - 2 * padding,
                          .w = 500 - 2 * padding};
  t->get_text = str_defeat_text;
  ecs_add_component(w, e, COMP_ACTUALISEDTEXT, t);
  return t;
}

char *str_defeat_text(__attribute__((unused)) World *w,
                      __attribute__((unused)) Entity *e) {
  char *v = "Defeat";
  char *t = malloc(strlen(v) + 1);
  strcpy(t, v);
  return t;
}
