#pragma once
#include <SDL2/SDL_render.h>
#include "../data_structures/linked_list.h"
#include "sprite.h"

typedef struct {
  //List of pixels ID based on RGBA values (subject to change)
  LinkedList pixels;
  //Height of the map in pixels
  int height;
  //Length of the map in pixels
  int length;
} Maptable;


//Uses a picture to create a table for future operations (resize and map generation) 
Maptable create_table_from_picture(char* path);

//Generates the map by associating an ID/RGBA to a tile
Sprite generate_map(Maptable table, Sprite tileset); 