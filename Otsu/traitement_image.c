#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

// Appliquer un seuil à l'image
void appliquer_seuil(Image *img, int seuil) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            Pixel *p = &img->pixels[i * img->width + j];
            unsigned char moyenne = (p->r + p->g + p->b) / 3;
            if (moyenne < seuil) {
                p->r = p->g = p->b = 0;   // Pixel noir
            } else {
                p->r = p->g = p->b = 255; // Pixel blanc
            }
        }
    }
}

// Méthode d'Otsu : détermine le seuil optimal pour l'image
int otsu_threshold(const Image *img) {
    int histogram[256] = {0};

    // Calcul du histogramme
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            Pixel *p = &img->pixels[i * img->width + j];
            unsigned char moyenne = (p->r + p->g + p->b) / 3;
            histogram[moyenne]++;
        }
    }

    int total = img->width * img->height;
    int sumB = 0, sum1 = 0;
    for (int i = 0; i < 256; i++) {
        sum1 += i * histogram[i];
    }

    int maxVar = 0;
    int threshold = 0;

    for (int i = 0; i < 256; i++) {
        sumB += histogram[i];
        if (sumB == 0) continue;
        
        int sumF = total - sumB;
        if (sumF == 0) break;

        float meanB = (float)sumB / total;
        float meanF = (float)sumF / total;

        int between = sumB * (total - sumB) * (meanB - meanF) * (meanB - meanF);
        
        if (between > maxVar) {
            maxVar = between;
            threshold = i;
        }
    }

    return threshold;
}

void libere_image(Image *img) {
    if (img) {
        free(img->pixels);
        free(img);
    }
}