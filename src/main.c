#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

typedef struct PixelBuf {
    size_t width;
    size_t height;
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
    for (int delta_y = 0; delta_y < (int) rect_h; ++delta_y) {
        int y = pos_y + delta_y;
        if (0 <= y && y < pixels->height) {
            for (int delta_x = 0; delta_x < (int) rect_w; ++delta_x) {
                int x = pos_x + delta_x;
                if (0 <= x && x < (int) pixels->width) {
                    pixels->pixels[y*pixels->width + x] = color;
                }
            }
        }
    }
}

typedef int Errno;

#define return_defer(value) do {result = (value); goto defer; } while (0)

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

int main(void)
{
    printf("Hello from C\n");

    PixelBuf *my_pb = create_pixelbuf(800, 600);
    if(!my_pb) {
        fprintf(stderr, "Failed to allocate PixelBuf\n");
        return 1;
    }

    fill_pixels(my_pb, 0xFF202020);
    fill_rectangle(my_pb, 0, 0, 100, 100, 0xFFFF0000);

    char *file_path = "output.ppm";
    Errno err = save_to_ppm_file(my_pb, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        free_pixelbuf(my_pb);
        return 1;
    }
    free_pixelbuf(my_pb);
    return 0;
}
