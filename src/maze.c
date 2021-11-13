#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define NO_PARENT -1

static size_t arcsPerMaze(maze *maze) {
  return (2 * (maze->width - 1)) * (2 * (maze->height - 1));
}

int initMaze(maze *maze) {
  struct timeval seedTime;
  gettimeofday(&seedTime, NULL);
  srand(seedTime.tv_usec);

  // Init disjoint set
  maze->disjointSet = malloc(sizeof *maze->disjointSet * maze->height);
  if (!maze->disjointSet) {
    fprintf(stderr, "Error: Cannot allocate memory for row pointers\n");
    return 0;
  }

  for (size_t y = 0; y < maze->height; y++) {
    maze->disjointSet[y] = malloc(sizeof **maze->disjointSet * maze->width);
    if (!maze->disjointSet[y]) {
      fprintf(stderr, "Error: Cannot allocate memory for row %zu.\n", y);
      return 0;
    }

    for (size_t x = 0; x < maze->width; x++) {
      maze->disjointSet[y][x] = NO_PARENT;
    }
  }

  return 1;
}

static int find(maze *maze, vect2 ptr) {
  int atRoot = 0, parentNode;
  while (!atRoot) {
    parentNode = maze->disjointSet[ptr.y][ptr.x];
    atRoot = parentNode == NO_PARENT;

    if (!atRoot) {
      size_t x = parentNode % maze->width, y = parentNode / maze->width;
      struct vect2 newPtr = {x, y};
      ptr = newPtr;
    }
  }

  return parentNode;
}

int unionFind(maze *maze, vect2 newL, vect2 newR) {
  int l = find(maze, newL), r = find(maze, newR);
  if (l == r) {
    return 0;
  } else {
    // Union the sets l and r
    // Sets all members of l to point to the parent of r
    vect2 ptr = newL;
    int atRoot = 0, parentNode;
    while (!atRoot) {
      parentNode = maze->disjointSet[ptr.y][ptr.x];
      maze->disjointSet[ptr.y][ptr.x] = l;
      atRoot = parentNode == NO_PARENT;

      if (!atRoot) {
        size_t x = parentNode % maze->width, y = parentNode / maze->width;
        struct vect2 newPtr = {x, y};
        ptr = newPtr;
      }
    }

    return 1;
  }
}

void kruskals(maze *maze) {
  size_t found = 0, max = (maze->width * maze->height) - 1;
  while (found < max) {
    int added = 0;
    while (!added) {
      long x = abs(random()) % maze->width,
           y = abs(random()) % maze->height;
      vect2 node = {x, y};

      added = unionFind(maze, node);
    }
  }
}

void freeMaze(maze *maze) {
  if (maze->disjointSet) {
    for (size_t y = 0; y < maze->height; y++) {
      if (maze->disjointSet[y]) {
        free(maze->disjointSet[y]);
      }
    }
  }
}
