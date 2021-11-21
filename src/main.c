#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "maze.h"
#include "images.h"

static void printHelp() {
    printf("Mazegenerator usage: ./mazegen [WIDTH] [HEIGHT]\n");
    printf("Exmaple: ./mazegen 50 50 \n\n");
    printf("Made by Danny Piper, git repo: https://github.com/djpiper28/ANSI-C-Maze-Generator.git\n");
}

int main(int argc, char **argv) {
    if (argc != 3) {
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
    printf("%zu ms\n", start.tv_sec - end.tv_sec);
    
    
    gettimeofday(&start, NULL);
    printf("Solving... \n");
    
    kruskals(&mz);    
    
    gettimeofday(&end, NULL);
    
    for (size_t y = 0; y < mz.height * 2 + 1; y++) {
        for (size_t x = 0; x < mz.width * 2 + 1; x++) {
            char c = '#';
            if (mz.image[y][x] == 0xFFFFFF) {
                c = ' ';
            }
            printf("%c%c", c, c);
        }
        printf("\n");
    }
    
    printf("%zu ms\n", start.tv_sec - end.tv_sec);
        
    freeMaze(&mz);
    
}
