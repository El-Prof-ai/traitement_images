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
void modifier_luminosite(Image *img, int valeur);
void libere_image(Image *img);

#endif
