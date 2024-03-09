#include "input.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

Inputs* inputs_new(){
  Inputs* in = malloc(sizeof(Inputs));
  in->keys = malloc(sizeof(int) * SDL_NUM_SCANCODES);
  for(int i=0;i<SDL_NUM_SCANCODES;i++) in->keys[i] = 0;
  in->mouse = 0;

  return in;
}

void inputs_free(Inputs* in){
  free(in->keys);
  free(in);
}