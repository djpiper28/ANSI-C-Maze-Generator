#include "maze.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define NO_PARENT -1

static size_t arcsPerMaze(maze *maze){
    return (2 * (maze->width - 1)) * (2 * (maze->height - 1));
}

int initMaze(maze* maze)
{
    struct timeval seedTime;
    gettimeofday(&seedTime, NULL);
    srand(seedTime.tv_usec);
    
    // Init disjoint set
    maze->disjointSet = malloc(sizeof * maze->disjointSet * maze->height);
    if (!maze->disjointSet)
    {
        fprintf(stderr, "Error: Cannot allocate memory for row pointers\n");
        return 0;
    }
    
    for (size_t y = 0; y < maze->height; y++)
    {
        maze->disjointSet[y] = malloc(sizeof * *maze->disjointSet * maze->width);
        if (!maze->disjointSet[y])
        {
            fprintf(stderr, "Error: Cannot allocate memory for row %zu.\n", y);
            return 0;
        }
        
        for (size_t x = 0; x < maze->width; x++)
        {
            maze->disjointSet[y][x] = NO_PARENT;
        }
    }
    
    return 1;
}

int unionFind(maze* maze, vect2 new)
{
    return 1;
}

void kruskals(maze* maze)
{
    size_t found = 0,
           max = (maze->width * maze->height) - 1;
    while (found < max)
    {
        int added = 0;
        while (!added)
        {
            long x = abse(random()) % maze->width,
                 y = abs(random()) % maze->height;
            vect2 node = {x, y};
        }
    }
}

void freeMaze(maze* maze)
{
    if (maze->disjointSet)
    {        
        for (size_t y = 0; y < maze->height; y++)
        {
            if (maze->disjointSet[y]){
                free(maze->disjointSet[y]);
            }
        }
    }
}
