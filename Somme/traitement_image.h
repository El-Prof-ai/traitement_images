#ifndef TRAITEMENT_IMAGE_H
#define TRAITEMENT_IMAGE_H

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int width, height;
    Pixel *pixels;
} Image;

// Prototypes des fonctions
Image *charger_image(const char *fichier);
int sauvegarder_image(const char *fichier, const Image *img);
void somme_images(const Image *img1, const Image *img2, Image *result);
void libere_image(Image *img);

#endif
