//! @file asset_manager.h
#pragma once

#include "../errors.h"
#include <stdint.h>

//! Plays sound `path`
Error play_audio(char *path, char is_music);

//! Sets the audio volume. Volume must be between 0 and 128. Anything higher
//! than that is clamped.
void set_volume(uint8_t volume);
