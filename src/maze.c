#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define NO_PARENT -1
#define WHITE 0xFFFFFF

int initMaze(maze *maze, int width, int height) {    
    struct timeval seedTime;
    gettimeofday(&seedTime, NULL);
    srand(seedTime.tv_usec);
    
    maze->width = width;
    maze->height = height;
    
    // Init disjoint set
    maze->disjointSet = malloc(sizeof * maze->disjointSet * maze->height);
    if (!maze->disjointSet) {
        fprintf(stderr, "Error: Cannot allocate memory for disjoint set row pointers\n");
        return 0;
    }
    
    for (size_t y = 0; y < maze->height; y++) {
        maze->disjointSet[y] = malloc(sizeof **maze->disjointSet * maze->width);
        if (!maze->disjointSet[y]) {
            fprintf(stderr, "Error: Cannot allocate memory for disjoint set row %zu.\n", y);
            return 0;
        }
        
        for (size_t x = 0; x < maze->width; x++) {
            maze->disjointSet[y][x] = NO_PARENT;
        }
    }
    
    // Init the image
    printf("output image size: %zu, %zu... ",
           maze->width * 2 + 1,
           maze->height * 2 + 1);
    maze->image = malloc(sizeof * maze->image * (maze->height * 2 + 1));
    if (!maze->image) {
        fprintf(stderr, "Error: Cannot allocate memory for image row pointers\n");
        return 0;
    }
    
    for (size_t y = 0; y < maze->height * 2 + 1; y++) {
        maze->image[y] = malloc(sizeof **maze->image * (maze->width * 2 + 1));
        if (!maze->image[y]) {
            fprintf(stderr, "Error: Cannot allocate memory for image row %zu.\n", y);
            return 0;
        }
        
        for (size_t x = 0; x < maze->width; x++) {
            maze->image[y][x] = 0x000000;
        }
    }
    
    maze->image[1][0] = WHITE;
    maze->image[1][maze->width * 2] = WHITE;
    
    return 1;
}

static int find(maze *maze, vect2 ptrIn) {
    vect2 ptr = ptrIn;
    int atRoot = 0;
    long last = ptr.x + ptr.y * maze->width,
         parentNode;
    while (!atRoot) {
        parentNode = maze->disjointSet[ptr.y][ptr.x];
        atRoot = parentNode == NO_PARENT;
        
        if (!atRoot) {
            long x = parentNode % maze->width, 
                 y = parentNode / maze->width;
            struct vect2 newPtr = {x, y};
            ptr = newPtr;
            last = parentNode;
        }
        
        atRoot |= parentNode == last;
    }
    
    atRoot = 0;
    ptr = ptrIn;
    long x, y;
    while (!atRoot) {
        parentNode = maze->disjointSet[ptr.y][ptr.x];
        atRoot = parentNode == NO_PARENT;
        
        x = parentNode % maze->width;
        y = parentNode / maze->width;
        maze->disjointSet[ptr.y][ptr.x] = last;
             
        if (!atRoot) {
            struct vect2 newPtr = {x, y};
            ptr = newPtr;
        }
        
        atRoot |= parentNode == last;
    }
    
    return last;
}

static int unionFind(maze *maze, vect2 newL, vect2 newR) {
    int l = find(maze, newL),
        r = find(maze, newR);
    if (l != r) {
        long ry = r / maze->width,
             rx = r % maze->width;
        maze->disjointSet[ry][rx] = r;
        
        long ly = l / maze->width,
             lx = l % maze->width;
        maze->disjointSet[ly][lx] = r;
        return 1;
    } else {
        return 0;
    }
}

void kruskals(maze *maze) {
    size_t found = 0;
    size_t max = (maze->width * maze->height) - 1;
    while (found < max) {
        int added = 0;
        while (!added) {
            long x = abs(rand()) % maze->width,
                 y = abs(rand()) % maze->height;
            vect2 nodeR, imageMiddle, nodeL = {x, y};
            imageMiddle = nodeR = nodeL;
            
            imageMiddle.x *= 2;
            imageMiddle.x++;
            
            imageMiddle.y *= 2;
            imageMiddle.y++;
            
            int validPoint = 1;
            int direction = abs(rand()) % 4;            
            switch(direction) {
                case 0:
                    if (nodeR.x + 1 < maze->width) {
                        nodeR.x++;
                        imageMiddle.x++;
                        break;
                    }
                case 1:
                    if (nodeR.x - 1 >= 0) {
                        nodeR.x--;
                        imageMiddle.x--;
                        break;
                    }
                case 2:                    
                    if (nodeR.y + 1 < maze->height) {
                        nodeR.y++;
                        imageMiddle.y++;
                        break;
                    }
                case 3:
                    if (nodeR.y - 1 >= 0) {
                        nodeR.y--;
                        imageMiddle.y--;
                        break;
                    }
                default:
                    validPoint = 0;
                    break;
            }
            
            if (validPoint) {
                added = unionFind(maze, nodeL, nodeR);
                
                if (added) {
                    maze->image[nodeL.y * 2 + 1][nodeL.x * 2 + 1] = WHITE;
                    maze->image[nodeR.y * 2 + 1][nodeR.x * 2 + 1] = WHITE;
                    maze->image[imageMiddle.y][imageMiddle.x] = WHITE;
                    found++;
                }
            }
        }
    }
    
    for (size_t y = 0; y < maze->height * 2 + 1; y++) {
        for (size_t x = 0; x < maze->width * 2 + 1; x++) {
            char c = '#';
            if (maze->image[y][x] == WHITE) {
                c = ' ';
            }
            printf("%c%c", c, c);
        }
        printf("\n");
    }
}

void freeMaze(maze *maze) {
    if (maze->disjointSet) {
        for (size_t y = 0; y < maze->height; y++) {
            if (maze->disjointSet[y]) {
                free(maze->disjointSet[y]);
            }
        }
        free(maze->disjointSet);
    }
    
    if (maze->image) {
        for (size_t y = 0; y < maze->height; y++) {
            if (maze->image[y]) {
                free(maze->image[y]);
            }
        }
        free(maze->image);
    }
}
