#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "traitement_image.h"

// Charger une image BMP
Image *charger_image(const char *fichier) {
    FILE *fp = fopen(fichier, "rb");
    if (!fp) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Lire les en-têtes BMP
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fp);

    // Récupérer les dimensions de l'image
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];

    // Allouer l'image
    Image *img = malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    img->pixels = malloc(width * height * sizeof(Pixel));

    // Lire les pixels (format BGR dans BMP)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel *p = &img->pixels[i * width + j];
            fread(&p->b, sizeof(unsigned char), 1, fp);
            fread(&p->g, sizeof(unsigned char), 1, fp);
            fread(&p->r, sizeof(unsigned char), 1, fp);
        }
    }

    fclose(fp);
    return img;
}

// Sauvegarder une image BMP
int sauvegarder_image(const char *fichier, const Image *img) {
    FILE *fp = fopen(fichier, "wb");
    if (!fp) {
        perror("Erreur lors de la création du fichier");
        return -1;
    }

    // Créer un en-tête BMP
    unsigned char header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    int fileSize = 54 + img->width * img->height * 3;
    *(int *)&header[2] = fileSize;
    header[10] = 54;
    header[14] = 40;
    *(int *)&header[18] = img->width;
    *(int *)&header[22] = img->height;
    header[26] = 1;
    header[28] = 24;

    fwrite(header, sizeof(unsigned char), 54, fp);

    // Écrire les pixels (format BGR dans BMP)
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            Pixel *p = &img->pixels[i * img->width + j];
            fwrite(&p->b, sizeof(unsigned char), 1, fp);
            fwrite(&p->g, sizeof(unsigned char), 1, fp);
            fwrite(&p->r, sizeof(unsigned char), 1, fp);
        }
    }

    fclose(fp);
    return 0;
}

// Appliquer le filtre passe-haut Prewitt
void filtre_passe_haut_prewitt(Image *img) {
    int width = img->width;
    int height = img->height;
    Pixel *src = img->pixels;
    Pixel *dest = malloc(width * height * sizeof(Pixel));
    memcpy(dest, src, width * height * sizeof(Pixel));

    // Matrice de convolution Prewitt
    int Gx[3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };
    int Gy[3][3] = {
        {-1, -1, -1},
        { 0,  0,  0},
        { 1,  1,  1}
    };

    // Appliquer le filtre
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sumR_x = 0, sumG_x = 0, sumB_x = 0;
            int sumR_y = 0, sumG_y = 0, sumB_y = 0;

            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    Pixel *p = &src[(i + m) * width + (j + n)];
                    sumR_x += p->r * Gx[m + 1][n + 1];
                    sumG_x += p->g * Gx[m + 1][n + 1];
                    sumB_x += p->b * Gx[m + 1][n + 1];
                    sumR_y += p->r * Gy[m + 1][n + 1];
                    sumG_y += p->g * Gy[m + 1][n + 1];
                    sumB_y += p->b * Gy[m + 1][n + 1];
                }
            }

            int newR = sqrt(sumR_x * sumR_x + sumR_y * sumR_y);
            int newG = sqrt(sumG_x * sumG_x + sumG_y * sumG_y);
            int newB = sqrt(sumB_x * sumB_x + sumB_y * sumB_y);

            dest[i * width + j].r = (newR > 255) ? 255 : newR;
            dest[i * width + j].g = (newG > 255) ? 255 : newG;
            dest[i * width + j].b = (newB > 255) ? 255 : newB;
        }
    }

    // Copier les résultats dans l'image originale
    memcpy(src, dest, width * height * sizeof(Pixel));
    free(dest);
}

// Appliquer le filtre passe-haut de Roberts
void filtre_passe_haut_rober(Image *img) {
    int width = img->width;
    int height = img->height;
    Pixel *src = img->pixels;
    Pixel *dest = malloc(width * height * sizeof(Pixel));
    memcpy(dest, src, width * height * sizeof(Pixel));

    // Matrice de convolution Roberts
    int Gx[2][2] = {
        { 1,  0},
        { 0, -1}
    };
    int Gy[2][2] = {
        { 0,  1},
        {-1,  0}
    };

    // Appliquer le filtre
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            int sumR_x = 0, sumG_x = 0, sumB_x = 0;
            int sumR_y = 0, sumG_y = 0, sumB_y = 0;

            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    Pixel *p = &src[(i + m) * width + (j + n)];
                    sumR_x += p->r * Gx[m][n];
                    sumG_x += p->g * Gx[m][n];
                    sumB_x += p->b * Gx[m][n];
                    sumR_y += p->r * Gy[m][n];
                    sumG_y += p->g * Gy[m][n];
                    sumB_y += p->b * Gy[m][n];
                }
            }

            int newR = sqrt(sumR_x * sumR_x + sumR_y * sumR_y);
            int newG = sqrt(sumG_x * sumG_x + sumG_y * sumG_y);
            int newB = sqrt(sumB_x * sumB_x + sumB_y * sumB_y);

            dest[i * width + j].r = (newR > 255) ? 255 : newR;
            dest[i * width + j].g = (newG > 255) ? 255 : newG;
            dest[i * width + j].b = (newB > 255) ? 255 : newB;
        }
    }

    // Copier les résultats dans l'image originale
    memcpy(src, dest, width * height * sizeof(Pixel));
    free(dest);
}

// Appliquer le filtre passe-haut de Laplacien
void filtre_passe_haut_laplacien(Image *img) {
    int width = img->width;
    int height = img->height;
    Pixel *src = img->pixels;
    Pixel *dest = malloc(width * height * sizeof(Pixel));
    memcpy(dest, src, width * height * sizeof(Pixel));

    // Matrice de convolution Laplacien
    int kernel[3][3] = {
        { 0,  1,  0},
        { 1, -4,  1},
        { 0,  1,  0}
    };

    // Appliquer le filtre
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sumR = 0, sumG = 0, sumB = 0;

            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    Pixel *p = &src[(i + m) * width + (j + n)];
                    sumR += p->r * kernel[m + 1][n + 1];
                    sumG += p->g * kernel[m + 1][n + 1];
                    sumB += p->b * kernel[m + 1][n + 1];
                }
            }

            int newR = sumR < 0 ? 0 : sumR > 255 ? 255 : sumR;
            int newG = sumG < 0 ? 0 : sumG > 255 ? 255 : sumG;
            int newB = sumB < 0 ? 0 : sumB > 255 ? 255 : sumB;

            dest[i * width + j].r = newR;
            dest[i * width + j].g = newG;
            dest[i * width + j].b = newB;
        }
    }

    // Copier les résultats dans l'image originale
    memcpy(src, dest, width * height * sizeof(Pixel));
    free(dest);
}

// Libérer la mémoire allouée pour une image
void libere_image(Image *img) {
    free(img->pixels);
    free(img);
}
