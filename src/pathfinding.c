#include <math.h>
#include <stdlib.h>

#include "pathfinding.h"
#include "data_structures/pqueue.h"

double pathfind_astar_heuristic(UnitTypes u, TilePosition *src,
                                TilePosition *dest) {
  double m = INFINITY;
  for (int i = 0; i < TILE_NUMBER; i++)
    m = m > units_get_tile_speed(u, i) ? units_get_tile_speed(u, i) : m;
  return m * (abs(src->x - dest->x) + abs(src->y - dest->y));
}

// converts map coordinates into flatmap coordinates
#define flatmap_index(m,i,j) (1 + (i)%(m)[0] + (j)*(m)[0])
// converts a TilePosition into flatmap coordinates
#define flatmap_tile_index(m,t) (1 + ((t)->x)%(m)[0] + ((t)->y)*(m)[0])
// gets the horizontal component of a flatmap index
#define flatmap_index_x(m,i) ((i)%(m)[0])
// gets the vertical component of a flatmap index
#define flatmap_index_y(m,i) (((int)(i))/(m)[0])
// gets the value at given map coordinates
#define flatmap_get(m,i,j) ((m)[flatmap_index(m,i,j)])
// gets the value at given TilePosition
#define flatmap_tile_get(m,t) ((m)[flatmap_tile_index(m,t)])
// sets the value at given map coordinates to v
#define flatmap_set(m,i,j,v) ((m)[flatmap_index(m,i,j)] = (v))

Path pathfind_astar(Map m,__attribute__((unused)) UnitTypes u,__attribute__((unused)) TilePosition *src,__attribute__((unused)) TilePosition *dest) {
  PQueue open = pqueue_new();
  int len = map_width(m)*map_height(m)+1;
  // contains for each tile it's own coordinates (tiles[i] = i for i>0), used to put them in the queue
  int* tiles = malloc(sizeof(int)*len);
  tiles[0] = map_width(m);
  // a flattened matrix containing the flat map index of the tile from which the current tile was explored
  int* from = malloc(sizeof(int)*len);
  from[0] = map_width(m);
  // a flattened matrix containing the current distance from the source to each point
  int* dist = malloc(sizeof(int)*len);
  dist[0] = map_width(m);
  for(int i=1;i<len;i++){
    tiles[i] = i;
    from[i] = -1;
    dist[i] = INFINITY;
  }
  pqueue_push(open,&flatmap_tile_get(tiles,src),0.); // potential threat

  while(pqueue_len(open)) {
    PQueueEntry* pqe = pqueue_pop(open);
    int curr = *(int*)pqe->value;
    if(curr == flatmap_tile_index(tiles,dest)){
      Path p = vec_new(TilePosition*);
      TilePosition* t;
      while(curr != flatmap_tile_index(tiles,src)){
        t = malloc(sizeof(TilePosition));
        *t = (TilePosition){.x = flatmap_index_x(tiles,curr), .y = flatmap_index_x(tiles,curr)};
        vec_push(p,t);
        curr = from[curr];
      }
      t = malloc(sizeof(TilePosition));
      *t = (TilePosition){.x = flatmap_index_x(tiles,curr), .y = flatmap_index_x(tiles,curr)};
      vec_reverse(p);
      return p;
    }
  }

  return NULL;
}
