#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helper.h"
#include <string.h>

typedef struct PixelBuf {
    size_t width;
    size_t height;
    // color : RGBA 8*4 = 32
    uint32_t *pixels;
} PixelBuf;

PixelBuf *create_pixelbuf(size_t width, size_t height)
{
    PixelBuf *pb = malloc(sizeof(PixelBuf));
    if (!pb) return NULL;

    pb->width = width;
    pb->height = height;
    pb->pixels = malloc(width*height*sizeof(uint32_t));
    if (!pb->pixels) {
        free(pb);
        return NULL;
    }
    return pb;
}

Errno save_to_ppm_file(PixelBuf *pb, const char *file_path)
{
    int result = 0;
    FILE *f = NULL;

    {
        f = fopen(file_path, "wb");
        if (f == NULL) { return_defer(errno); }

        fprintf(f, "P6\n%zu %zu\n 255\n", pb->width, pb->height);
        if (ferror(f)) { return_defer(errno); }

        for(size_t i = 0; i < pb->width*pb->height; ++i) {
            uint32_t pixel = pb->pixels[i];
            uint8_t bytes[3] = {
                (pixel>>(8*0))&0xFF,
                (pixel>>(8*1))&0xFF,
                (pixel>>(8*2))&0xFF,
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            if (ferror(f)) { return_defer(errno); }
        }
    }

defer:
    if (f) fclose(f);
    return result;
}

void free_pixelbuf(PixelBuf *pb)
{
    if (!pb) return;
    free(pb->pixels);
    free(pb);
}

void fill_pixels(PixelBuf *pb, uint32_t color)
{
    for (int y = 0; y < pb->height; ++y) {
        for (int x = 0; x < pb->width; ++x) {
            pb->pixels[y * pb->width + x] = color;
        }
    }
}

void draw_horizontal_line(PixelBuf *pb, uint32_t color)
{
    int y = pb->height /2;
    for (int x = 0; x < pb->width; ++x) {
        pb->pixels[y * pb->width + x] = color;
    }
}

// Raycast functions

typedef struct RaycastMap {
    int width;
    int height;
    int **cells;
} RaycastMap;

RaycastMap *create_raycastmap(int width, int height)
{
    RaycastMap *rm = malloc(sizeof(RaycastMap));
    if (!rm) return NULL;

    rm->width = width;
    rm->height = height;
    rm->cells = malloc(height*sizeof(int*));
    if (!rm->cells) {
        free(rm);
        return NULL;
    }
    for (int y = 0; y < height; ++y) {
        rm->cells[y] = malloc(width*sizeof(int));
    }
    return rm;
}

void fill_raycast_map_with_walls(RaycastMap *map) {
    for(int y = 0; y < map->height; ++y) {
        for (int x = 0; x < map->width; ++x) {
            if (x == 0 || y == 0 || x == map->width - 1 || y == map->height -1) {
                map->cells[y][x] = 1;
            }
            else {
                map->cells[y][x] = 0;
            }
        }
    }
}

void fill_pixels_with_map(PixelBuf *pb, RaycastMap *map) {
    for (int y = 0; y < pb->height; ++y) {
        for (int x = 0; x < pb->width; ++x) {

            int map_x = x * map->width  / pb->width;
            int map_y = y * map->height / pb->height;

            int cell = map->cells[map_y][map_x];
            uint32_t color = (cell == 1) ? 0xFFFFFFFF: 0xFF000000;
            pb->pixels[y * pb->width + x] = color;
        }
    }
}


int main()
{
    RaycastMap *map = create_raycastmap(10, 10);
    fill_raycast_map_with_walls(map);

    PixelBuf *my_pb = create_pixelbuf(800, 600);
    if(!my_pb) {
        fprintf(stderr, "Failed to allocate PixelBuf\n");
        return 1;
    }

    fill_pixels_with_map(my_pb, map);

    //call your drawing function you wanna test here
    //fill_pixels(my_pb, 0xFF202020);
    //draw_horizontal_line(my_pb, 0xFF00FF00);

    char *file_path = "graphics.ppm";
    Errno err = save_to_ppm_file(my_pb, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        free_pixelbuf(my_pb);
        return 1;
    }
    free_pixelbuf(my_pb);
    return 0;
}
