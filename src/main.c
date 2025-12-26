#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "draw_pixel.h"

int main(void)
{
    printf("Hello from C\n");

    PixelBuf *my_pb = create_pixelbuf(800, 600);
    if(!my_pb) {
        fprintf(stderr, "Failed to allocate PixelBuf\n");
        return 1;
    }

    //fill_pixels(my_pb, 0xFF202020);
    //fill_rectangle(my_pb, my_pb->width/2 - 50, my_pb->height/2 -50, 100, 100, 0xFFFF0000);

    //fill_rectangle(my_pb, my_pb->width/2 - 100, my_pb->height/2 -100, 100, 100, 0xFF00FF00);

    fill_circle(my_pb, my_pb->width/2, my_pb->height/2, 100, 0xFF00FF00);
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
