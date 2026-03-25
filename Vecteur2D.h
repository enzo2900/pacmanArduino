#ifndef VECTEUR2D_H
#define VECTEUR2D_H
#include <Arduino.h>
#include <avr/pgmspace.h>
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
  Vecteur2D operator+(const Vecteur2D & velocity) const {
        return {x + velocity.x, y + velocity.y};
    }
    Vecteur2D operator*(int valeur  ) const {
        return {valeur * x, valeur * y};
    }

    Vecteur2D operator-() {
        return {-x,-y};
    }
};
#endif