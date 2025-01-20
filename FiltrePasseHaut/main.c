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

    // Choisir le filtre à appliquer
    int choix;
    printf("Choisissez le filtre à appliquer :\n");
    printf("1. Prewitt\n");
    printf("2. Roberts\n");
    printf("3. Laplacien\n");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            filtre_passe_haut_prewitt(img1);
            break;
        case 2:
            filtre_passe_haut_rober(img1);
            break;
        case 3:
            filtre_passe_haut_laplacien(img1);
            break;
        default:
            fprintf(stderr, "Choix invalide\n");
            libere_image(img1);
            return 1;
    }

    // Sauvegarder l'image modifiée
    if (sauvegarder_image("result/resultat.bmp", img1) != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image résultat\n");
        libere_image(img1);
        return 1;
    }

    printf("L'image après filtre a été sauvegardée avec succès dans result/resultat.bmp.\n");

    // Libérer la mémoire
    libere_image(img1);

    return 0;
}
