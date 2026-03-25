#ifndef UTILITY_H
#define UTILITY_H
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Vecteur2D.h"
#include "Direction.h"
Direction closestToTarget(Vecteur2D position, Vecteur2D target, Direction directionPossibles[], int taille) ;
int nombreDirectionsPossible(Vecteur2D position,Direction directionPossibles[]);
Direction directionFromVelocity(Vecteur2D velocity) ;
int removeFromDirection(Direction directions[],Direction direction, int tailleDirection) ;
#endif