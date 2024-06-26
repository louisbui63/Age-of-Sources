//! @file audio.h
#pragma once

#include "../errors.h"
#include <stdint.h>

//! Plays sound `path`
Error play_audio(char *path, char is_music);

//! Sets the audio volume. Volume must be between 0 and 128. Anything higher
//! than that is clamped.
void set_volume(uint8_t volume);

//! Returns the current volume of the audio.
uint8_t get_volume();

//! Starts and stops the music.
void toggle_music();
