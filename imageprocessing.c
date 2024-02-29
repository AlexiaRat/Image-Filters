#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#define MAX_COLOR 255

// TODO(alexia_maria.rat) Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    int ***flipped_image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        flipped_image[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            flipped_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                flipped_image[i][j][k] = image[i][M - 1 - j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return flipped_image;
}

// TODO(alexia_maria_rat) Task 2
int ***rotate_left(int ***image, int N, int M) {
    int ***rotated_image = (int ***)malloc(M * sizeof(int **));
    for (int i = 0; i < M; i++) {
        rotated_image[i] = (int **)malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++) {
            rotated_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                rotated_image[M - 1 - j][i][k] = image[i][j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return rotated_image;
}

// TODO(alexia_maria.rat) Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    if (x < 0 || y < 0 || h <= 0 || w <= 0 || x + w > M || y + h > N) {
        printf("Invalid crop dimensions\n");
        return NULL;
    }
    int ***cropped_image = (int ***)malloc(h * sizeof(int **));
    for (int i = 0; i < h; i++) {
        cropped_image[i] = (int **)malloc(w * sizeof(int *));
        for (int j = 0; j < w; j++) {
            cropped_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < 3; k++) {
                cropped_image[i][j][k] = image[y + i][x + j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return cropped_image;
}

// TODO(alexia_maria.rat) Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int new_N = N + 2 * rows;
    int new_M = M + 2 * cols;
    int ***extended_image = (int ***)malloc(new_N * sizeof(int **));
    for (int i = 0; i < new_N; i++) {
        extended_image[i] = (int **)malloc(new_M * sizeof(int *));
        for (int j = 0; j < new_M; j++) {
            extended_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < new_N; i++) {
        for (int j = 0; j < new_M; j++) {
            extended_image[i][j][0] = new_R;
            extended_image[i][j][1] = new_G;
            extended_image[i][j][2] = new_B;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                extended_image[i + rows][j + cols][k] = image[i][j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return extended_image;
}

// TODO(alexia_maria.rat) Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src, int N_src, int M_src, int x, int y) {
    for (int i = 0; i < N_src; i++) {
        for (int j = 0; j < M_src; j++) {
            if (x + j < M_dst && y + i < N_dst) {
                for (int k = 0; k < 3; k++) {
                    image_dst[y + i][x + j][k] = image_src[i][j][k];
                }
            }
        }
    }

    return image_dst;
}

// TODO(alexia_maria.rat) Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***filtered_image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        filtered_image[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            filtered_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                filtered_image[i][j][k] = 0;
                float sum = 0;
                for (int fi = 0; fi < filter_size; fi++) {
                    for (int fj = 0; fj < filter_size; fj++) {
                        int ni = i + fi - filter_size / 2;
                        int nj = j + fj - filter_size / 2;
                        if (ni >= 0 && ni < N && nj >= 0 && nj < M) {
                            sum += (float)image[ni][nj][k] * filter[fi][fj];
                        }
                        filtered_image[i][j][k] = (int)sum;
                    }
                }
                if (filtered_image[i][j][k] < 0)
                    filtered_image[i][j][k] = 0;
                if (filtered_image[i][j][k] > MAX_COLOR)
                    filtered_image[i][j][k] = MAX_COLOR;
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return filtered_image;
}
