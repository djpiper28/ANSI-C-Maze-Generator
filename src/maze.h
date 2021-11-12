#pragma once
#include <stdlib.h>

typedef struct vect2 {
    long x, y;
} vect2;

typedef struct maze {
    long width, height;
    size_t arc_pointer;
    vect2 **disjointSet;
} maze;

int initMaze(maze *);
void generateArcs(maze *);
void kruskals(maze *);
int unionFind(maze *, vect2 /*new arc*/);
void freeMaze(maze *);
