//
// Created by enzoc on 24/03/2026.
//

#include "Fantome.h"
#include "map.h"
Fantome::Fantome(const Vecteur2D basePosition, const Vecteur2D baseVelocity, uint16_t couleurBase, ComportementFantome comportement)  {
    position = basePosition;
    velocity = baseVelocity;
    couleur = couleurBase;
    this->comportement =comportement;
    
}

Vecteur2D Fantome::getPos() {
    return this->position;
}
void Fantome::updatePos(Vecteur2D pos) {
    this->position = pos;
}
void Fantome::draw(Vecteur2D pastPos) {
    
    
    drawEntity(pastPos, this->position, this->couleur);

    Vecteur2D pixel1;
    Vecteur2D pixel2;
    Vecteur2D pixel3;
    //Serial.println("velocité");
    //Serial.println(this->velocity.y);
    if(this->velocity.x != 0) {
         pixel1 = {pastPos.x  - velocity.x,pastPos.y};
         pixel2 = {pastPos.x  - velocity.x,pastPos.y +1};
         pixel3 = {pastPos.x  - velocity.x,pastPos.y -1};
    }else if(this->velocity.y != 0) {
         pixel1 = {pastPos.x  ,pastPos.y - velocity.y};
         pixel2 = {pastPos.x -1,pastPos.y  - velocity.y };
         pixel3 = {pastPos.x +1 ,pastPos.y  - velocity.y};
    } 
    //Serial.println("Vecteur pixel1");
    // Serial.println(pixel1.y);
    // Serial.println(pixel1.x);
    redrawObjetsAt(pixel1,pixel2,pixel3);
}
void Fantome::changeVelocityWithDirection(Direction direction) {
    this->velocity = getVecteurFrom(direction);
}
void Fantome::randomizeFantomeVelocity(Direction directionPossibles[4], int tailleTableauAssigne) {
    uint8_t choixDirection =  (uint8_t)random(tailleTableauAssigne);;

    changeVelocityWithDirection(directionPossibles[choixDirection]);
}

void Fantome::addChemin(int tailleChemin) {
    for (int i = 0 ; i < tailleChemin ; i++) {
        chemin[i] = cheminFantome[i];
    }
}
void Fantome::updatePoursuit(Vecteur2D pacmanPos) {
    Vecteur2D pastPos = this->position;
    if (this->indexChemin == -1 ) {
        int tailleC = recherche(this->position,pacmanPos);
        this->tailleChemin = tailleC;
        addChemin(tailleC);
        indexChemin = 0;
    }
    if (this->indexChemin == tailleChemin) {
       int tailleC = recherche(this->position,pacmanPos);
        this->tailleChemin = tailleC;
        addChemin(tailleC);

        indexChemin = 0;
    }
    Vecteur2D newPosition = chemin[this->indexChemin];
    this->velocity = newPosition - pastPos;
    this->position = newPosition;
    indexChemin ++;
}
void Fantome::updateAleatoire(Vecteur2D pacmanPos) {
    int nombreDirections = nombreDirectionsPossible(this->position,this->directions) ;
    if (nombreDirections > 2) {
        randomizeFantomeVelocity(this->directions,nombreDirections);
    } else if(estMurPresent(this->velocity,this->position+ (this->velocity*2))) {
        if (nombreDirections > 1) {
                nombreDirections = removeFromDirection(this->directions,directionFromVelocity(-this->position),nombreDirections);
        }

        randomizeFantomeVelocity(this->directions,nombreDirections);
    }
    this->position = (this->position + this->velocity);
}

void Fantome::updateOptimiste(Vecteur2D pacmanPos) {
     int nombreDirections = nombreDirectionsPossible(this->position,this->directions) ;
    if (nombreDirections > 2) {
        if(nombreDirections > 1) {
                nombreDirections = removeFromDirection(this->directions,lastDirection,nombreDirections);
        }
        Direction bestDir = closestToTarget(this->position,pacmanPos, this->directions,nombreDirections);
       // printDirection(bestDir);
        changeVelocityWithDirection(bestDir);
        this->lastDirection= bestDir;
    }else if(estMurPresent(this->velocity,this->position+ (this->velocity*2))) {
        Direction bestDir = closestToTarget(this->position,pacmanPos, this->directions,nombreDirections);
        //printDirection(bestDir);
        changeVelocityWithDirection(bestDir);
        this->lastDirection = bestDir;

        
    }
     this->position = (this->position + this->velocity);
}

void Fantome::update(Vecteur2D pacmanPos){
    Vecteur2D previousPos = this->position;

    switch (this->comportement) {
        case POURSUIT:
            updatePoursuit(pacmanPos);
            break;
        case OPTIMISTE:
            updateOptimiste(pacmanPos);
            break;
        case ALEATOIRE:
            updateAleatoire(pacmanPos);
            break;
    }
    draw(previousPos);
    
}



