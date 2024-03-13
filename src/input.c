#include <stdlib.h>

#include "components.h"
#include "data_structures/vec.h"
#include "input.h"

Inputs *inputs_new() {
  Inputs *in = malloc(sizeof(Inputs));
  in->keys = malloc(sizeof(int) * SDL_NUM_SCANCODES);
  for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    in->keys[i] = 0;
  in->key_nb = 0;
  in->mouse = 0;

  return in;
}

void inputs_free(Inputs *in) {
  free(in->keys);
  free(in);
}

void inputs_update_key_in_from_scancode(Inputs *inputs, SDL_Scancode scancode,
                                        bool new_val) {
  bool norm_val = !!new_val;
  if (inputs->keys[scancode] != norm_val)
    inputs->key_nb += norm_val ? 1 : -1;
  inputs->keys[scancode] = norm_val;
}

void inputs_run_callbacks(World *w, Inputs *in, KeyState ks) {
  if ((in->key_nb > 0) || in->mouse) {
    Bitflag b = COMPF_KEY_EVENT;
    EntityRef *entities = world_query(w, &b);
    for (uint i = 0; i < vec_len(entities); i++) {
      Entity *e = get_entity(w, entities[i]);
      for (uint j = 0; j < vec_len(e->components); j++) {
        if (e->components[j].type_id == COMP_KEY_EVENT) {
          (*(KeyEvent *)e->components[j].component)(e, in, ks);
        }
      }
    }
  }
}
