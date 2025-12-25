#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "draw_pixel.h"

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
    fill_rectangle(my_pb, my_pb->width/2 - 50, my_pb->height/2 -50, 100, 100, 0xFFFF0000);

    fill_rectangle(my_pb, my_pb->width/2 - 100, my_pb->height/2 -100, 100, 100, 0xFF00FF00);

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
