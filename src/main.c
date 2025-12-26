#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "draw_pixel.h"

void fill_circle_checkboard(PixelBuf *pb, int cols, int rows, uint32_t color)
{
    int cell_width = pb->width/cols;
    int cell_height = pb->height/rows;

    fill_pixels(pb, 0xFF202020);

    for(int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int radius = cell_width;
            if (cell_height < radius) {
                radius = cell_height;
            }
            fill_circle(pb, x*cell_width + cell_width/2, y*cell_height + cell_height/2, radius/2, color);
        }
    }
}

int main(void)
{
    printf("Hello from C\n");

    PixelBuf *my_pb = create_pixelbuf(800, 600);
    if(!my_pb) {
        fprintf(stderr, "Failed to allocate PixelBuf\n");
        return 1;
    }

    //call your drawing function you wanna test here
    fill_circle_checkboard(my_pb, 8, 8, 0xFF00FF00);

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
