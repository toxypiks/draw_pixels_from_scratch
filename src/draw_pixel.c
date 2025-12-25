#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "draw_pixel.h"

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

void free_pixelbuf(PixelBuf *pb)
{
    if (!pb) return;
    free(pb->pixels);
    free(pb);
}

void fill_pixels(PixelBuf *pixels, uint32_t color)
{
    for (size_t i = 0; i < pixels->width*pixels->height; ++i) {
        pixels->pixels[i] = color;
    }
}

void fill_rectangle(PixelBuf *pixels, int pos_x, int pos_y, size_t rect_w, size_t rect_h, uint32_t color)
{
    // Clipping of coordinates
    int start_x = pos_x < 0 ? 0 : pos_x;
    int end_x   = (pos_x + (int)rect_w > (int)pixels->width) ? (int)pixels->width : pos_x + (int)rect_w;

    int start_y = pos_y < 0 ? 0 : pos_y;
    int end_y   = (pos_y + (int)rect_h > (int)pixels->height) ? (int)pixels->height : pos_y + (int)rect_h;

    for (int y = start_y; y < end_y; ++y) {
        int row_offset = y * pixels->width;
        for (int x = start_x; x < end_x; ++x) {
            pixels->pixels[row_offset + x] = color;
        }
    }
}
