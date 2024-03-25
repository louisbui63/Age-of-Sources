//! @file map.h

#pragma once

//! used to store a map as a matrix of `ìnt` (each value designates a specific type of tile, ex: water, plain...)
typedef int** Map;

//! returns a new `Map` initialized at 0 with size `w*h`
Map map_create(int w,int h);

//! frees a `Map` created with `map_create`
void map_free(Map m);

//! returns the width of a `Map`
#define map_width(m) (m)[-2]

//! returns the height of a `Map`
#define map_height(m) (m)[-1]