#include <stdio.h>

int main(void)
{
    printf("Hello from excercises\n");

    void print_chessboard(int width, int height)
    {
        for(int y = 0; y < height; ++y) {
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
        for(int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                printf("%d ", x + y);
            }
            printf("\n");
        }
    }

    return 0;
}
