#include <stdio.h>

void print_chessboard(int width, int height)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y) % 2 == 0) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void print_sum_grid(int width, int height)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            printf("%d ", x + y);
        }
        printf("\n");
    }
}

void print_rect(int width, int height)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("*");
        }
        printf("\n");
    }
}

void print_diagonal(int width, int height)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y == x) {
                printf("X");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void fill_array(int width, int height)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            printf("%d ", y*width + x);
        }
        printf("\n");
    }
}

void fill_cross(int width, int height)
{
    int mid_x = width/2;
    int mid_y = height/2;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y == mid_y || x == mid_x) {
                printf("X");
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void fill_diagonal_cross(int width, int height)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y == x || x + y == width -1) {
                printf("X");
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void fill_frame(int width, int height)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width -1 || y == height -1) {
                printf("X");
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int main(void)
{
    fill_frame(7 ,7);

    return 0;
}
