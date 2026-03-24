#ifndef UTILITY_H
#define UTILITY_H
#include <Arduino.h>
#include <avr/pgmspace.h>
// Est ce qu'un mur est présent au pixel demandée par rapport à la direction du regard.
bool estMurPresent(int directionX,int directionY, int pixelPosX, int pixelPosY) ;
enum Direction {
    DROITE,
    GAUCHE,
    HAUT,
    BAS
};
struct Vecteur2D 
{
  int8_t x;
  int8_t y;

  // Distance rectangulaire entre deux vecteurs
  int distance(Vecteur2D autre) {
      int8_t xDirection = autre.x - x;
      int8_t yDirection = autre.y - y;
      Vecteur2D direction =  {xDirection,  yDirection};
      // Retourne la distance max par rapport à x ou y;
      return max(abs(direction.x),abs(direction.y));
  }
};


int nombreDirectionPossible(Vecteur2D position,Direction directionPossibles[]);
Direction directionFromVelocity(Vecteur2D velocity) ;
int removeFromDirection(Direction directions[],Direction direction, int tailleDirection) ;
#endif