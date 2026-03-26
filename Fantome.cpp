//
// Created by enzoc on 24/03/2026.
//

#include "Fantome.h"
#include "map.h"
Fantome::Fantome(const Vecteur2D basePosition, const Vecteur2D baseVelocity, uint16_t couleurBase)  {
    position = basePosition;
    velocity = baseVelocity;
    couleur = couleurBase;
    
}

Vecteur2D Fantome::getPos() {
    return this->position;
}
void Fantome::draw(Vecteur2D pastPos) {
    drawEntity(pastPos, this->position, this->couleur);
}
void Fantome::changeVelocityWithDirection(Direction direction) {
    this->velocity = getVecteurFrom(direction);
}
void Fantome::randomizeFantomeVelocity(Direction directionPossibles[4], int tailleTableauAssigne) {
    uint8_t choixDirection =  (uint8_t)random(tailleTableauAssigne);;

    changeVelocityWithDirection(directionPossibles[choixDirection]);
}


void Fantome::update(Vecteur2D pacmanPos){
    Vecteur2D previousPos = this->position;
    int nombreDirections = nombreDirectionsPossible(this->position,this->directions) ;
    Serial.println("Direction");
    for(int i = 0 ; i < nombreDirections ; i++) {
         printDirection(this->directions[i]);
    }
   
    if (nombreDirections > 2) {
        // Serial.println("Intersection");
        bool chasePacman = true;
        if (chasePacman) {
            //Serial.println("Chase");
            //Serial.println(pacmanPos.x);
            if(nombreDirections > 1) {
                nombreDirections = removeFromDirection(this->directions,lastDirection,nombreDirections);
            }
            Direction bestDir = closestToTarget(this->position,pacmanPos, this->directions,nombreDirections);
            printDirection(bestDir);
            changeVelocityWithDirection(bestDir);
            this->lastDirection= bestDir;
        } else {
            randomizeFantomeVelocity(this->directions,nombreDirections);

        }
    } else if(estMurPresent(this->velocity,this->position+ (this->velocity*2))) {
        delay(2000);
        Serial.println("Va vers mur");
        // If chase pacman
        bool chasePacman = true;
        if (chasePacman) {
           // Serial.println("Chase");
            Direction bestDir = closestToTarget(this->position,pacmanPos, this->directions,nombreDirections);
            printDirection(bestDir);
            changeVelocityWithDirection(bestDir);
            this->lastDirection = bestDir;
        } else {
            Serial.println("Randomize");
            // Suppression de la direction derriere selon la velocité
            if (nombreDirections > 1) {
                nombreDirections = removeFromDirection(this->directions,directionFromVelocity(-this->position),nombreDirections);
            }

            randomizeFantomeVelocity(this->directions,nombreDirections);
        }
    }
   
    this->position = (this->position + this->velocity).copy();
    draw(previousPos);
}



