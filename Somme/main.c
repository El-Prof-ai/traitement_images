#include <stdio.h>
#include <stdlib.h>
#include "traitement_image.h"

int main() {
    // Charger les deux images
    Image *img1 = charger_image("img/image1.bmp");
    if (!img1) {
        fprintf(stderr, "Erreur lors du chargement de image1.bmp\n");
        return 1;
    }

    Image *img2 = charger_image("img/image2.bmp");
    if (!img2) {
        fprintf(stderr, "Erreur lors du chargement de image2.bmp\n");
        free(img1->pixels);
        free(img1);
        return 1;
    }

    // Créer une image pour stocker le résultat
    Image *result = malloc(sizeof(Image));

    // Calculer la somme des deux images
    somme_images(img1, img2, result);

    // Sauvegarder l'image résultat
    if (sauvegarder_image("result/resultat.bmp", result) != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image résultat\n");
        libere_image(img1);
        libere_image(img2);
        libere_image(result);
        return 1;
    }

    printf("La somme des deux images a été sauvegardée avec succès dans result/resultat.bmp.\n");

    // Libérer la mémoire
    libere_image(img1);
    libere_image(img2);
    libere_image(result);

    return 0;
}
