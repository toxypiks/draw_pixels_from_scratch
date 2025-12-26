#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "draw_pixel.h"

void swap_int(int *one, int *two)
{
    int tmp = *one;
    *one = *two;
    *two = tmp;
}

void draw_line(PixelBuf *pb, int start_x, int start_y, int end_x, int end_y, uint32_t color)
{
    // using y = k*x +c

    int dx = end_x - start_x;
    int dy = end_y - start_y;

    if (dx != 0) {
        int c = start_y - dy*start_x/dx;

        if (start_x > end_x) {swap_int(&start_x, &end_x);}
        for (int x = start_x; x <= end_x; ++x) {
            if(0 <= x && x < (int)pb->width) {
                int y = dy*x/dx + c;
                if (0 <= y && y < (int)pb->height) {
                    pb->pixels[y*pb->width + x] = color;
                }
            }
        }
    } else {
        // vertical line
        int x = start_x;
        if (x < 0 && x < (int)pb->width) {
            if (start_y > end_y) {swap_int(&start_y, &end_y);}
            for (int y = start_y; y < end_y; ++y) {
                if (0 <= y && y < (int)pb->height) {
                    pb->pixels[y*pb->width + x] = color;
                }
            }
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
    fill_pixels(my_pb, 0xFF202020);
    draw_line(my_pb, 0, 0, 800, 600, 0xFF00FF00);

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
