#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Canvas {
    size_t width;
    size_t height;
    uint32_t *pixels;
} Canvas;

Canvas *create_canvas(size_t width, size_t height)
{
    Canvas *c = malloc(sizeof(Canvas));
    if (!c) return NULL;

    c->width = width;
    c->height = height;
    c->pixels = malloc(width*height*sizeof(uint32_t));
    if (!c->pixels) {
        free(c);
        return NULL;
    }
    return c;
}

void fill_canvas(Canvas *c, uint32_t color)
{
    for (size_t i = 0; i < c->width*c->height; ++i) {
        c->pixels[i] = color;
    }
}

int save_canvas_to_ppm_file(Canvas *c, const char *file_path)
{
    int result = 0;
    FILE *f = fopen(file_path, "wb");

    if (f == NULL) {
        result = -1;
        goto defer;
    }

    fprintf(f, "P6\n%zu %zu\n 255\n", c->width, c->height);
    if (ferror(f)) {
        result = -1;
        goto defer;
    }

    for(size_t i = 0; i < c->width*c->height; ++i) {
        uint32_t pixel = c->pixels[i];
        uint8_t bytes[3] = {
            (pixel>>(8*0))&0xFF,
            (pixel>>(8*1))&0xFF,
            (pixel>>(8*2))&0xFF,
        };
        fwrite(bytes, sizeof(bytes), 1, f);
        if (ferror(f)) {
            result = -1;
            goto defer;
        }
    }

defer:
    if (f) fclose(f);
    return result;
}

int main(void)
{
    printf("Hello from C\n");

    Canvas *my_canvi = create_canvas(800, 600);
    fill_canvas(my_canvi, 0xFF00FF00);
    save_canvas_to_ppm_file(my_canvi, "output.ppm");

    return 0;
}
