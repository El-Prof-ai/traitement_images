#include <stdio.h>
#include <stdlib.h>
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

// Calculer la somme de deux images
void somme_images(const Image *img1, const Image *img2, Image *result) {
    if (img1->width != img2->width || img1->height != img2->height) {
        fprintf(stderr, "Les dimensions des images doivent être identiques.\n");
        return;
    }

    result->width = img1->width;
    result->height = img1->height;
    result->pixels = malloc(result->width * result->height * sizeof(Pixel));

    for (int i = 0; i < img1->width * img1->height; i++) {
        result->pixels[i].r = (img1->pixels[i].r + img2->pixels[i].r > 255) ? 255 : img1->pixels[i].r + img2->pixels[i].r;
        result->pixels[i].g = (img1->pixels[i].g + img2->pixels[i].g > 255) ? 255 : img1->pixels[i].g + img2->pixels[i].g;
        result->pixels[i].b = (img1->pixels[i].b + img2->pixels[i].b > 255) ? 255 : img1->pixels[i].b + img2->pixels[i].b;
    }
}

// Libérer la mémoire allouée pour une image
void libere_image(Image *img) {
    free(img->pixels);
    free(img);
}
