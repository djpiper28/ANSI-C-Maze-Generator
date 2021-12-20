#include <png.h>
#include "images.h"

void mazeToImage(maze *maze, char *name) {    
    int width = maze->width * 2 + 1;
    int height = maze->height * 2 + 1;
    
    FILE *fp = fopen(name, "wb");
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;
    
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s for writing\n", name);
        goto finalise;
    }
    
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "Could not allocate write struct\n");
        goto finalise;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Could not allocate info struct\n");
        goto finalise;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        finalise:;
        fprintf(stderr, "Error during png creation - exiting.\n");
        
        if (fp != NULL) fclose(fp);
        if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        if (row != NULL) free(row);
        
        return;
    }
    
    png_init_io(png_ptr, fp);
    
    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr,
                 info_ptr,
                 width,
                 height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    
    png_text title_text;
    title_text.compression = PNG_TEXT_COMPRESSION_NONE;
    title_text.key = "Maze";
    title_text.text = "Maze";
    png_set_text(png_ptr, info_ptr, &title_text, 1);
    png_write_info(png_ptr, info_ptr);
    
    // Write image data
    for (int y = 0 ; y < height; y++) {
        // Yummy hack
        png_write_row(png_ptr, (unsigned char *) maze->image[y]);
    }
    
    // End write
    png_write_end(png_ptr, NULL);    
    png_write_info(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    free(row);
    fclose(fp);
}
