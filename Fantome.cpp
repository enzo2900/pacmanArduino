//
// Created by enzoc on 24/03/2026.
//

#include "Fantome.h"
#include "map.h"

Fantome::Fantome(const Vecteur2D basePosition, const Vecteur2D baseVelocity, uint16_t couleur) : position(basePosition), velocity(baseVelocity), couleur(couleur) {
}

Vecteur2D Fantome::getPos() {
    return this->position;
}
void Fantome::draw() {
    drawEntity(position, velocity, couleur);
}
void Fantome::changeVelocityWithDirection(Direction direction) {
        switch(direction) {
            case HAUT:
                this->velocity = {1,0};
                break;
            case BAS:
                this->velocity = {-1,0};
                break;
            case DROITE:
                this->velocity = {0,1};
                break;
            case GAUCHE:
                this->velocity = {0,-1};
                break;
            default :
                break;
        }
}
void Fantome::randomizeFantomeVelocity(Direction directionPossibles[4], int tailleTableauAssigne) {
    uint8_t choixDirection =  (uint8_t)random(tailleTableauAssigne);;

    return changeVelocityWithDirection(directionPossibles[choixDirection]);
}


void Fantome::update(Vecteur2D pacmanPos){
    Vecteur2D previousPos = position;
    int tailleDirections = nombreDirectionPossible(this->position,directions) ;
    if (tailleDirections > 2) {
        bool chasePacman = true;
        if (chasePacman) {
            Direction bestDir = closestToTarget(this->position,pacmanPos, directions,tailleDirections);
            changeVelocityWithDirection(bestDir);
        } else {
            randomizeFantomeVelocity(directions,tailleDirections);

        }
        this->position = this->position + this->velocity;

    } else if(estMurPresent(this->position.x, this->position.y,this->position.x + this->velocity.x,
        this->position.y + this->velocity.y)) {
        // Suppression de la direction derriere selon la velocité
        // If chase pacman
        bool chasePacman = true;
        if (chasePacman) {
            Direction bestDir = closestToTarget(this->position,pacmanPos, directions,tailleDirections);
            changeVelocityWithDirection(bestDir);
        } else {
            if (tailleDirections != 1) {
                tailleDirections = removeFromDirection(directions,directionFromVelocity(-this->position),tailleDirections);
            }

            randomizeFantomeVelocity(directions,tailleDirections);
        }
    }
    this->position = this->position + this->velocity;
    draw();
}



