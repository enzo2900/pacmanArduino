/**
 * Un fantôme se déplace sur la matrice sans pouvoir traverser les murs.
 * Si un fantôme entre en contact avec pacman le joueur perdra la partie.
 */

#include "Fantome.h"
#include "map.h"
/**
 * Un Fantome est défini par une position et une vitesse qui mettra à jour sa
 * position en fonction de sa direction. Il s'affichera avec une couleur
 * spécifique et cherchera à toucher pacman avec une stratégie particulière
 * spécifiée par son comportement.
 * @param basePosition Position initial, là où il apparaîtra au début d'une partie.
 * @param baseVelocity La vitesse de déplacement du fantôme.
 * @param couleurBase  La couleur du fantôme.
 * @param comportement L'algorithme de déplacement du fantôme.
 */
Fantome::Fantome(const Vecteur2D basePosition, const Vecteur2D baseVelocity, uint16_t couleurBase, ComportementFantome comportement,long baseTimeBeforeMovement)  {
    position = basePosition;
    velocity = baseVelocity;
    couleur = couleurBase;
    timeBeforeMovement=  baseTimeBeforeMovement;
    currentTimeForMovement = 0;
    this->comportement =comportement;
}

/**
 * Récupère la prosition d'un fantôme.
 */
Vecteur2D Fantome::getPos() {
    return this->position;
}

/**
 *Met à jour la position du fantôme.
 */
void Fantome::updatePos(Vecteur2D pos) {
    this->position = pos;
}

/**
 * Dessine le fantôme à sa nouvelle position. Efface le fantôme présent sur la
 * position précédente.
 */
void Fantome::draw(Vecteur2D pastPos) {
    drawEntity(pastPos, this->position, this->couleur);

    Vecteur2D pixel1;
    Vecteur2D pixel2;
    Vecteur2D pixel3;
    if(this->velocity.x != 0) {
         pixel1 = {pastPos.x  - velocity.x,pastPos.y};
         pixel2 = {pastPos.x  - velocity.x,pastPos.y +1};
         pixel3 = {pastPos.x  - velocity.x,pastPos.y -1};
    }else if(this->velocity.y != 0) {
         pixel1 = {pastPos.x  ,pastPos.y - velocity.y};
         pixel2 = {pastPos.x -1,pastPos.y  - velocity.y };
         pixel3 = {pastPos.x +1 ,pastPos.y  - velocity.y};
    } 
    redrawObjetsAt(pixel1,pixel2,pixel3);
}

/**
 * Change la vitesse en fonction de la direction spécifié. La vitesse est un
 * vecteur corespondant à la vitesse sur l'axe x et y.
 */
void Fantome::changeVelocityWithDirection(Direction direction) {
    this->velocity = getVecteurFrom(direction);
}

/**
 * Permet d'obtenir une vitesse aléatoire pour un fantome.
 */
void Fantome::randomizeFantomeVelocity(Direction directionPossibles[4], int tailleTableauAssigne) {
    uint8_t choixDirection =  (uint8_t)random(tailleTableauAssigne);;

    changeVelocityWithDirection(directionPossibles[choixDirection]);
}

/*
 * Ajoute un chemin possible du fantôme vers le joueur.
 */
void Fantome::addChemin(int tailleChemin) {
    for (int i = 0 ; i < tailleChemin ; i++) {
        chemin[i] = cheminFantome[i];
    }
}

/*
 * Met à jour la poursuite d'un fantôme. Cela permet de redéfinir sa position
 * par rapport à celle du joueur.
 */
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

/**
 * Met à jour une direction aléatoire à partir d'une intersection.
 */
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

/**
 * Met à jour la décision de la direction pour l'algorithme optimiste.
 */
void Fantome::updateOptimiste(Vecteur2D pacmanPos) {
     int nombreDirections = nombreDirectionsPossible(this->position,this->directions) ;
    if (nombreDirections > 2) {
        if(nombreDirections > 1) {
                nombreDirections = removeFromDirection(this->directions,lastDirection,nombreDirections);
        }
        Direction bestDir = closestToTarget(this->position,pacmanPos, this->directions,nombreDirections);
        changeVelocityWithDirection(bestDir);
        this->lastDirection= bestDir;
    }else if(estMurPresent(this->velocity,this->position+ (this->velocity*2))) {
        Direction bestDir = closestToTarget(this->position,pacmanPos, this->directions,nombreDirections);
        changeVelocityWithDirection(bestDir);
        this->lastDirection = bestDir;

        
    }
     this->position = (this->position + this->velocity);
}

/**
 * Définis la nouvelle position de pacman pour un fantôme.
 */
void Fantome::update(Vecteur2D pacmanPos){
    if(millis() < currentTimeForMovement + timeBeforeMovement) {
        Serial.println(millis());
        return;
    } else {
        currentTimeForMovement = millis();
    }
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



