#include "audio.h"
#include "../data_structures/asset_manager.h"
#include <SDL2/SDL_mixer.h>

void set_volume(uint8_t volume) { Mix_Volume(-1, volume); }

uint8_t get_volume() { return Mix_Volume(-1, -1); }

Error play_audio(char *path, char is_music) {
  void *l = get_audio(path, is_music);
  if (!l)
    return ASSET_NOT_FOUND;
  if (Mix_PlayChannel(-1, l, 0) == -1)
    return COULD_NOT_MIX_SOUND;
  return SUCCESS;
}
