#include <stdlib.h>

#include "map.h"

Map map_create(int w,int h) {
    Map m = malloc(sizeof(int*)*w + 2*sizeof(int));
    m[0] = w;
    m[1] = h;
    m += 2*sizeof(int);
    for(int i=0;i<w;i++) m[i] = calloc(h,sizeof(int));
    return m;
}

void map_free(Map m) {
    for(int i=0;i<map_width(m);i++) free(m[i]);
    free(m-2*sizeof(int));
}