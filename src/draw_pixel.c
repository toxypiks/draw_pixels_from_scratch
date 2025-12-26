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

void fill_checkboard(PixelBuf *pb, int cols, int rows, uint32_t color)
{
    int cell_width = pb->width/cols;
    int cell_height = pb->height/rows;

    uint32_t rect_color;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if((x + y)%2 == 0) {
                rect_color = color;
            } else {
                rect_color = 0xFF000000;
            }
            fill_rectangle(pb, x*cell_width, y*cell_height, cell_width, cell_height, rect_color);
        }
    }
}

void fill_circle(PixelBuf *pb, int pos_x, int pos_y, int radius, uint32_t color)
{
    // Clipping coordinates
    int x1 = pos_x < 0 ? 0 : pos_x - radius;
    int y1 = pos_y < 0 ? 0 : pos_y - radius;

    int x2 = (pos_x + radius) > pb->width ? pb->width - 1: pos_x + radius;
    int y2 = (pos_y + radius) > pb->height ? pb->height - 1 : pos_y + radius;

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            int dx = x - pos_x;
            int dy = y - pos_y;
            if ((dx*dx + dy*dy) <= radius*radius) {
                pb->pixels[y*pb->width + x] = color;
            }
        }
    }
}

float lerpf(float a, float b, float t)
{
    return a + (b - a)*t;
}


void fill_circle_checkboard(PixelBuf *pb, int cols, int rows, uint32_t color)
{
    if (!pb || cols <= 0 || rows <= 0) return;

    size_t cell_width = pb->width/cols;
    size_t cell_height = pb->height/rows;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            float u = (float)x/(float)cols; // 0..1
            float v = (float)y/(float)rows; // 0..1
            float t = (u + v)/2; // 0..1

            // center of cell
            int cx = x * cell_width + cell_width / 2;
            int cy = y * cell_height + cell_height / 2;

            // radius uses smallest part of cell/2
            int radius = cell_width < cell_height ? cell_width / 2 : cell_height / 2;

            // make sure radius not greater then cell
            if (cx - radius < 0) radius = cx;
            if (cy - radius < 0) radius = cy;
            if (cx + radius >= pb->width)  radius = pb->width - 1 - cx;
            if (cy + radius >= pb->height) radius = pb->height - 1 - cy;

            fill_circle(pb, cx, cy, (int)lerpf(radius/2, radius, t), color);
        }
    }
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
