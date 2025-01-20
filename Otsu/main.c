#include <stdio.h>
#include <stdlib.h>
#include "traitement_image.h"

int main() {
    // Charger l'image
    Image *img1 = charger_image("img/image1.bmp");
    if (!img1) {
        fprintf(stderr, "Erreur lors du chargement de image1.bmp\n");
        return 1;
    }

    // Calculer le seuil optimal avec la méthode d'Otsu
    int seuil = otsu_threshold(img1);

    printf("Seuil optimal déterminé par la méthode d'Otsu : %d\n", seuil);

    // Appliquer le seuil
    appliquer_seuil(img1, seuil);

    // Sauvegarder l'image modifiée
    if (sauvegarder_image("result/resultat.bmp", img1) != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image résultat\n");
        libere_image(img1);
        return 1;
    }

    printf("L'image après traitement a été sauvegardée avec succès dans result/resultat.bmp.\n");

    // Libérer la mémoire
    libere_image(img1);

    return 0;
}
