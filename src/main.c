#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "maze.h"
#include "images.h"

static void printHelp() {
    printf("Mazegenerator usage: ./mazegen [WIDTH] [HEIGHT] [OUTPUT FILE NAME]\n");
    printf("Exmaple: ./mazegen 50 50 \n\n");
    printf("Made by Danny Piper, git repo: https://github.com/djpiper28/ANSI-C-Maze-Generator.git\n");
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printHelp();
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp("help", argv[i]) == 0
            || strcmp("--help", argv[i]) == 0
            || strcmp("-h", argv[i]) == 0) {
            printHelp();
            return 1;
        }
    }
    
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    const char *name = argv[3];
    
    if (width <= 0 || height <= 0) {
        printHelp();
        printf("Width and height must be numbers greater than 0\n");
        return 2;
    }
    
    // Generate maze
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    maze mz;
    printf("Initialising a maze of size %dx%d... \n", width, height);
    
    initMaze(&mz, width, height);
    
    gettimeofday(&end, NULL);
    printf("%ld ms\n", end.tv_usec - start.tv_usec);
    
    
    gettimeofday(&start, NULL);
    printf("Solving... \n");
    
    kruskals(&mz);
    
    gettimeofday(&end, NULL);
    printf("%ld s\n", end.tv_sec - start.tv_sec);
    
    gettimeofday(&start, NULL);
    printf("Saving maze... ");
    
    mazeToImage(&mz, name);
    
    gettimeofday(&end, NULL);
    printf("%ld ms\n", end.tv_usec - start.tv_usec);
    freeMaze(&mz);
}
