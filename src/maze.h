#pragma once
#include <stdlib.h>

typedef struct vect2 {
    long x, y;
} vect2;

typedef struct maze {
    long width, height;
    long **disjointSet;
    int **image;
} maze;

int initMaze(maze *, int width, int height);
void kruskals(maze *);
void freeMaze(maze *);
