#ifndef DRAW_PIXEL_H_
#define DRAW_PIXEL_H_

typedef struct PixelBuf {
    size_t width;
    size_t height;
    uint32_t *pixels;
} PixelBuf;

PixelBuf *create_pixelbuf(size_t width, size_t height);
void free_pixelbuf(PixelBuf *pb);
void fill_pixels(PixelBuf *pixels, uint32_t color);
void fill_rectangle(PixelBuf *pixels, int pos_x, int pos_y, size_t rect_w, size_t rect_h, uint32_t color);

#endif // DRAW_PIXEL_H_
