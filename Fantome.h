//
// Created by enzoc on 24/03/2026.
//

#ifndef FANTOME_H
#define FANTOME_H
#include "Utility.h"
#include "Vecteur2D.h"
#include "Direction.h"

class Fantome {

public:
    Fantome(Vecteur2D basePosition, Vecteur2D baseVelocity,uint16_t couleur);
    void update(Vecteur2D pacmanPos);

    Vecteur2D getPos();
    void draw() ;
    void changeVelocityWithDirection(Direction direction);
    // Donne une nouvelle vélocity à un fantome de manière aléatoire
    void randomizeFantomeVelocity(Direction directionPossibles[4],int tailleTableauAssigne);
    Vecteur2D position{};
    Vecteur2D velocity{};
     uint16_t couleur;
private:
    
    
    Direction directions[4]{};
   

};
#endif 