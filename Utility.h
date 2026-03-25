#ifndef UTILITY_H
#define UTILITY_H
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Vecteur2D.h"
#include "Direction.h"
#include "ObjetGrille.h"
Vecteur2D getVecteurFrom(Direction direction);
Direction closestToTarget(Vecteur2D position, Vecteur2D target, Direction directionPossibles[], int taille) ;
int nombreDirectionsPossible(Vecteur2D position,Direction directionPossibles[]);
void printDirection(Direction direction) ;
Direction directionFromVelocity(Vecteur2D velocity) ;
int removeFromDirection(Direction directions[],Direction direction, int tailleDirection) ;
// Recherche binaire du tableau d'objet pour trouver en temps logarithmique l'objet de la grille à la position demandée.
// Si l'objet n'est pas trouvé un -1 est retourné. 
int rechercheBinaireObjets(Vecteur2D target, ObjetGrille objects[]);
#endif