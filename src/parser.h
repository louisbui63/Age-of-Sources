//! @file parser.h
#pragma once

#include "units/units.h"

//! This function create and UnitT by parsing the file at `path`, the file must
//! follow the model of `unit_template.h`
UnitT *parse(char *path, SDL_Renderer *renderer, SDL_Window *window);
