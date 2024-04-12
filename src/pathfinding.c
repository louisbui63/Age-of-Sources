#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "data_structures/pqueue.h"
#include "pathfinding.h"
#include "util.h"
#include "data_structures/vec.h"

void path_free(Path p){
  for (uint i = 0; i < vec_len(p); i++)
    free(p[i]);
  vec_free(p);
}

typedef struct {
  int ind;
  int from;
} Entry;

PQueue pqueue_push_wrapper_inner(PQueue p, int ind, int from, double w) {
  Entry *e = malloc(sizeof(Entry));
  *e = (Entry){.ind = ind, .from = from};
  pqueue_push(p, e, w);
  return p;
}

#define pqueue_push_wrapper(p, ind, from, w) (p = pqueue_push_wrapper_inner(p, ind, from, w))

int pqe_get_ind(PQueueEntry *pqe) { return ((Entry *)pqe->value)->ind; }
int pqe_get_from(PQueueEntry *pqe) { return ((Entry *)pqe->value)->from; }

double dist(TilePosition *src, TilePosition *dest) {
  return sqrt((src->x - dest->x) * (src->x - dest->x) +
              (src->y - dest->y) * (src->y - dest->y));
}

double pathfind_astar_heuristic(UnitTypes u, TilePosition *src,
                                TilePosition *dest) {
  double m = INFINITY;
  m = 0;
  for (int i = 0; i < TILE_NUMBER; i++)
    //m = m > units_get_tile_speed(u, i) ? units_get_tile_speed(u, i) : m;
    m += units_get_tile_speed(u, i);
  m /= TILE_NUMBER;
  return dist(src, dest) / m;
  // return (abs(src->x - dest->x) + abs(src->y - dest->y))/m;
}

// converts map coordinates into flatmap coordinates
#define flatmap_index(m, i, j) (1 + (i) % ((int*)(m))[0] + (j) * ((int*)(m))[0])

// converts a TilePosition into flatmap coordinates
#define flatmap_tile_index(m, t) (1 + ((t)->x) % ((int*)(m))[0] + ((t)->y) * ((int*)(m))[0])

// gets the horizontal component of a flatmap index
#define flatmap_index_x(m, i) (((i)-1) % ((int*)(m))[0])

// gets the vertical component of a flatmap index
#define flatmap_index_y(m, i) (((int)(i)-1) / ((int*)(m))[0])

// gets the value at given map coordinates
#define flatmap_get(m, i, j) ((m)[flatmap_index(m, i, j)])

// gets the value at given TilePosition
#define flatmap_tile_get(m, t) ((m)[flatmap_tile_index(m, t)])

// sets the value at given map coordinates to v
#define flatmap_set(m, i, j, v) ((m)[flatmap_index(m, i, j)] = (v))

VEC(int*) int_vec_push_inner(VEC(int *) v, int i) {
  int *i_ = malloc(sizeof(int));
  *i_ = i;
  vec_push(v, i_);
  return v;
}

#define int_vec_push(v,i) (v = int_vec_push_inner(v, i))

VEC(int *) flatmap_get_neighbours(int *map, int ind, int height) {
  int x = flatmap_index_x(map, ind);
  int y = flatmap_index_y(map, ind);
  VEC(int *) neigh = vec_new(int *);
  if (x != 0)
    int_vec_push(neigh, flatmap_index(map, x - 1, y));
  if (y != 0)
    int_vec_push(neigh, flatmap_index(map, x, y - 1));
  if (y != height - 1)
    int_vec_push(neigh, flatmap_index(map, x, y + 1));
  if (x != map[0] - 1)
    int_vec_push(neigh, flatmap_index(map, x + 1, y));
  return neigh;
}

VEC(int *) flatmap_get_diag(int *map, int ind, int height) {
  int x = flatmap_index_x(map, ind);
  int y = flatmap_index_y(map, ind);
  VEC(int *) neigh = vec_new(int *);
  if (x != 0) {
    if (y != 0)
      int_vec_push(neigh, flatmap_index(map, x - 1, y - 1));
    if (y != height - 1)
      int_vec_push(neigh, flatmap_index(map, x - 1, y + 1));
  }
  if (x != map[0] - 1) {
    if (y != 0)
      int_vec_push(neigh, flatmap_index(map, x + 1, y - 1));
    if (y != height - 1)
      int_vec_push(neigh, flatmap_index(map, x + 1, y + 1));
  }
  return neigh;
}

Path pathfind_astar(Map m, UnitTypes u, TilePosition *src, TilePosition *dest) {
  PQueue open = pqueue_new();
  int len = map_width(m) * map_height(m) + 1;
  // a flattened matrix containing the flat map index of the tile from which the
  // current tile was explored
  int *from = malloc(sizeof(int) * len);
  from[0] = map_width(m);
  // a flattened matrix containing the current distance from the source to each
  // point
  double *dist = malloc(sizeof(double) * len);
  ((int*)dist)[0] = map_width(m);
  for (int i = 1; i < len; i++) {
    from[i] = -1;
    dist[i] = INFINITY;
  }
  pqueue_push_wrapper(open, flatmap_tile_index(dist, src), -1,
                      pathfind_astar_heuristic(u, src, dest));

  while (pqueue_len(open)) {
    PQueueEntry *pqe = pqueue_pop(open);
    int curr = pqe_get_ind(pqe);
    TilePosition currpos = {.x = flatmap_index_x(dist, curr),
                            .y = flatmap_index_y(dist, curr)};

    // printf("{x=%d,y=%d}\n",currpos.x,currpos.y); // DEBUG
    dist[curr] = pqe->weight - pathfind_astar_heuristic(u, &currpos, dest);
    from[curr] = pqe_get_from(pqe);
    free(pqe->value);
    free(pqe);
    // if the destination has been reached
    if (curr == flatmap_tile_index(dist, dest)) {
      Path p = vec_new(TilePosition *);
      TilePosition *t;
      // reconstruction of the path
      while (curr != flatmap_tile_index(dist, src)) {
        t = malloc(sizeof(TilePosition));
        *t = (TilePosition){.x = flatmap_index_x(dist, curr),
                            .y = flatmap_index_y(dist, curr)};
        vec_push(p, t);
        curr = from[curr];
      }
      t = malloc(sizeof(TilePosition));
      *t = (TilePosition){.x = flatmap_index_x(dist, curr),
                          .y = flatmap_index_y(dist, curr)};
      vec_push(p, t);
      vec_reverse(p);

      // frees everything
      pqueue_free_callback(open, free);
      free(dist);
      free(from);
      return p;
    }
    // printf("  neighbour time\n"); // DEBUG
    // checks the neighbours of the current tile
    TileTypes ttype =
        m[flatmap_index_x(dist, curr)][flatmap_index_y(dist, curr)];
    double spd = units_get_tile_speed(u, ttype);
    VEC(int *) neigh = flatmap_get_neighbours(from, curr, map_height(m));
    for (uint i = 0; i < vec_len(neigh); i++) {
      int j = *neigh[i];
      
      if (isinf(dist[j])) {
        TilePosition tp = {.x = flatmap_index_x(dist, j),
                           .y = flatmap_index_y(dist, j)};
        TileTypes nttype = m[tp.x][tp.y];
        double nspd = units_get_tile_speed(u, nttype);
        // printf("  %lf -> %lf + %lf\n",nspd,1 / (2 * spd) + 1 / (2 * nspd), pathfind_astar_heuristic(u, &tp, dest)); // DEBUG
        pqueue_push_wrapper(open, j, curr,
                            1 / (2 * spd) + 1 / (2 * nspd) +
                                pathfind_astar_heuristic(u, &tp, dest));
      }
      free(neigh[i]);
    }
    vec_free(neigh);
    neigh = flatmap_get_diag(from, curr, map_height(m));
    for (uint i = 0; i < vec_len(neigh); i++) {
      int j = *neigh[i];
      if (isinf(dist[j])) {
        TilePosition tp = {.x = flatmap_index_x(dist, j),
                           .y = flatmap_index_y(dist, j)};
        TileTypes nttype = m[tp.x][tp.y];
        double nspd = units_get_tile_speed(u, nttype);
        // printf("  %lf -> %lf + %lf\n",nspd,sqrt(2) * (1 / (2 * spd) + 1 / (2 * nspd)), pathfind_astar_heuristic(u, &tp, dest)); // DEBUG
        pqueue_push_wrapper(open, j, curr,
                            sqrt(2) * (1 / (2 * spd) + 1 / (2 * nspd)) +
                                pathfind_astar_heuristic(u, &tp, dest));
      }
      free(neigh[i]);
    }
    vec_free(neigh);
  }
  pqueue_free_callback(open, free);
  free(dist);
  free(from);
  return NULL;
}
