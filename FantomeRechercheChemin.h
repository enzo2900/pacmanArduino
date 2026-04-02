#ifndef FANTOMERECHERCHECHEMIN_H
#define FANTOMERECHERCHECHEMIN_H
#include "Vecteur2D.h"
#define VISITE_TAILLE_MAX 25
// Chemin mise à jour lorsque la fonction recherche de ce même fichier est appelé.
// Ce chemin est entre deux positions et est limité par rapport à la taille fixe.
extern Vecteur2D cheminFantome[VISITE_TAILLE_MAX];

// Fonction qui recherche un chemin vers la cible.
// Le chemin est optimisé mais est limité par rapport à la taille fixe du nombre de case qui peuvent être visités
// Le chemin commence à la position du fantome et finis à la position la plus proche de la cible.
// La fonction retourne la taille du chemin
int recherche(Vecteur2D positionFantome, Vecteur2D cible);
#endif