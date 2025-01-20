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

    // Demander à l'utilisateur le seuil
    int seuil;
    printf("Entrez un seuil (valeur entre 0 et 255) : ");
    scanf("%d", &seuil);

    // Appliquer le seuillage
    seuillage_image(img1, seuil);

    // Sauvegarder l'image modifiée
    if (sauvegarder_image("result/resultat.bmp", img1) != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image résultat\n");
        libere_image(img1);
        return 1;
    }

    printf("L'image après seuillage a été sauvegardée avec succès dans result/resultat.bmp.\n");

    // Libérer la mémoire
    libere_image(img1);

    return 0;
}
