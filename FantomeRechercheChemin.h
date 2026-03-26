#ifndef FANTOMERECHERCHECHEMIN_H
#define FANTOMERECHERCHECHEMIN_H
#include "Vecteur2D.h"
#define VISITE_TAILLE_MAX 100
extern Vecteur2D cheminFantome[VISITE_TAILLE_MAX];
int recherche(Vecteur2D positionFantome, Vecteur2D cible);
#endif