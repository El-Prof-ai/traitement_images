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

    // Demander à l'utilisateur la valeur de modification de la luminosité
    int valeur;
    printf("Entrez une valeur pour modifier la luminosité (-255 à 255) : ");
    scanf("%d", &valeur);

    // Modifier la luminosité
    modifier_luminosite(img1, valeur);

    // Sauvegarder l'image modifiée
    if (sauvegarder_image("result/resultat.bmp", img1) != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image résultat\n");
        libere_image(img1);
        return 1;
    }

    printf("L'image modifiée a été sauvegardée avec succès dans result/resultat.bmp.\n");

    // Libérer la mémoire
    libere_image(img1);

    return 0;
}
