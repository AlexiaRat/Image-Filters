#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
#define MAX_IMAGES 20
#define MAX_FILTERS 50
#define MAX_PATH 100

typedef struct {
    int ***image;
    int N, M;
} image_t;

typedef struct {
    float **filter;
    int size;
} filter_t;

int main() {
    // TODO(alexia_maria.rat) Task 7
    image_t *imgs = (image_t *)malloc(MAX_IMAGES * sizeof(image_t));
    filter_t *filters = (filter_t *)malloc(MAX_FILTERS * sizeof(filter_t));
    int nr_img = 0, nr_fil = 0;
    char command[3];

    scanf("%s", command);
    int index = 0;
    while (strcmp(command, "e") != 0) {
        if (strcmp(command, "l") == 0) {
            char path[MAX_PATH];
            int N = 0, M = 0;
            scanf("%d %d %s", &N, &M, path);
            int ***image = (int ***)malloc(N * sizeof(int **));
            for (int i = 0; i < N; i++) {
                image[i] = (int **)malloc(M * sizeof(int *));
                for (int j = 0; j < M; j++) {
                    image[i][j] = (int *)malloc(3 * sizeof(int));
                }
            }
            read_from_bmp(image, N, M, path);
            imgs[nr_img].image = image;
            imgs[nr_img].N = N;
            imgs[nr_img].M = M;
            nr_img++;
        }
        if (strcmp(command, "s") == 0) {
            char save_path[MAX_PATH];
            scanf("%d %s", &index, save_path);
            write_to_bmp(imgs[index].image, imgs[index].N, imgs[index].M, save_path);
        }
        if (strcmp(command, "ah") == 0) {
            scanf("%d", &index);
            int ***flipped_image = flip_horizontal(imgs[index].image, imgs[index].N, imgs[index].M);
            imgs[index].image = flipped_image;
        }
        if (strcmp(command, "ar") == 0) {
            scanf("%d", &index);
            int*** rotated_image = rotate_left(imgs[index].image, imgs[index].N, imgs[index].M);
            imgs[index].image = rotated_image;
            int x = imgs[index].N;
            imgs[index].N = imgs[index].M;
            imgs[index].M = x;
        }
        if (strcmp(command, "ac") == 0) {
            int x = 0, y = 0, w = 0, h = 0;
            scanf("%d %d %d %d %d", &index, &x, &y, &w, &h);
            imgs[index].image = crop(imgs[index].image, imgs[index].N, imgs[index].M, x, y, h, w);
            imgs[index].N = h;
            imgs[index].M = w;
        }
        if (strcmp(command, "ae") == 0) {
            int rows = 0, cols = 0, R = 0, G = 0, B = 0;
            scanf("%d %d %d %d %d %d", &index, &rows, &cols, &R, &G, &B);
            int ***extended_image = extend(imgs[index].image, imgs[index].N, imgs[index].M, rows, cols, R, G, B);
            imgs[index].image = extended_image;
            imgs[index].N = imgs[index].N + 2 * rows;
            imgs[index].M = imgs[index].M + 2 * cols;
        }
        if (strcmp(command, "ap") == 0) {
            int id = 0, is = 0, x = 0, y = 0;
            int Nd = 0, Ns = 0, Md = 0, Ms = 0;
            scanf("%d %d %d %d", &id, &is, &x, &y);
            Nd = imgs[id].N;
            Md = imgs[id].M;
            Ns = imgs[is].N;
            Ms = imgs[is].M;
            imgs[id].image = paste(imgs[id].image, Nd, Md, imgs[is].image, Ns, Ms, x, y);
        }
        if (strcmp(command, "cf") == 0) {
            int size = 0;
            scanf("%d", &size);
            float **filter = (float **)malloc(size * sizeof(float *));
            for (int i = 0; i < size; i++) {
                filter[i] = (float *)malloc(size * sizeof(float));
            }
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    scanf("%f", &filter[i][j]);
                }
            }
            filters[nr_fil].filter = filter;
            filters[nr_fil].size = size;
            nr_fil++;
        }
        if (strcmp(command, "af") == 0) {
            int ind_img = 0, ind_filter = 0;
            scanf("%d %d", &ind_img, &ind_filter);
            float **filter = filters[ind_filter].filter;
            int filter_size = filters[ind_filter].size;
            int*** f_image = apply_filter(imgs[ind_img].image, imgs[ind_img].N, imgs[ind_img].M, filter, filter_size);
            imgs[ind_img].image = f_image;
        }
        if (strcmp(command, "df") == 0) {
            int index_filter = 0;
            scanf("%d", &index_filter);
            float **filter = filters[index_filter].filter;
            int filter_size = filters[index_filter].size;
            if (filter != NULL) {
                for (int i = 0; i < filter_size; i++) {
                    free(filter[i]);
                }
                free(filter);
            }
            for (int i = index_filter; i < nr_fil - 1; i++) {
                filters[i] = filters[i + 1];
            }
            nr_fil--;
        }
        if (strcmp(command, "di") == 0) {
            int index_img = 0;
            scanf("%d", &index_img);
            if (imgs[index_img].image != NULL) {
                for (int i = 0; i < imgs[index_img].N; i++) {
                    for (int j = 0; j < imgs[index_img].M; j++) {
                        free(imgs[index_img].image[i][j]);
                    }
                    free(imgs[index_img].image[i]);
                }
                free(imgs[index_img].image);
            }
            for (int i = index_img; i < nr_img - 1; i++) {
                imgs[i] = imgs[i + 1];
            }
            nr_img--;
        }

        scanf("%s", command);
    }

    if (imgs != NULL) {
        for (int k = 0; k < nr_img; k++) {
            int ***image = imgs[k].image;
            for (int i = 0; i < imgs[k].N; i++) {
                for (int j = 0; j < imgs[k].M; j++) {
                    free(image[i][j]);
                }
                free(image[i]);
            }
            free(image);
        }
        free(imgs);
    }

    if (filters != NULL) {
        for (int k = 0; k < nr_fil; k++) {
            float **filter = filters[k].filter;
            {
                for (int i = 0; i < filters[k].size; i++) {
                    free(filter[i]);
                }
                free(filter);
            }
        }
        free(filters);
    }

    return 0;
}
