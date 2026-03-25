#include "utility.h"
#include "map.h"

Vecteur2D getPositionFrom(Vecteur2D position,Direction direction) {

    switch (direction) {
        case HAUT:
            return {position.x+2,position.y};
        case BAS:
            return {position.x -2,position.y};
        case DROITE:
            return {position.x,position.x +2};
        case GAUCHE:
            return {position.x,position.y-2};

    }
}
Vecteur2D getVecteurFrom(Direction direction) {
     switch (direction) {
        case HAUT:
            return {1,0};
        case BAS:
            return {-1,0};
        case DROITE:
            return {0,1};
        case GAUCHE:
            return {0,-1};
    }
}
Direction closestToTarget(Vecteur2D position, Vecteur2D target, Direction directionPossibles[], int taille) {
    Direction bestDirection = HAUT;
    int bestDistance = 999;
    for (int i = 0; i < taille; i++) {
        int dist = getPositionFrom(position,directionPossibles[i]).distance(target);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestDirection = directionPossibles[i];
        }
    }
    return bestDirection;
}
int nombreDirectionPossible(Vecteur2D position,Direction directionPossibles[]);

int nombreDirectionPossible(Vecteur2D position,Direction directionPossibles[]) {
    int indexDirection = 0;
    // Haut toujours = 0
    if (!estMurPresent(1,0,position.x +2,position.y)) {
        directionPossibles[indexDirection] = HAUT;
        indexDirection++;
  }
    // Droite ou gauche toujours entre 0 et indexDirection-1
    if (!estMurPresent(0,1,position.x ,position.y + 2)) {
        directionPossibles[indexDirection] = DROITE;
        indexDirection++;
    }
    if (!estMurPresent(0,-1,position.x,position.y- 2)) {
        directionPossibles[indexDirection] = GAUCHE;
        indexDirection++;
    }

    // Bas toujours = indexDirection-1
    if (!estMurPresent(-1,0,position.x -2,position.y)) {
        directionPossibles[indexDirection] = BAS;
        indexDirection++;
    }

    return indexDirection;

}

int removeFromDirection(Direction directions[],Direction direction, int tailleDirection) {
    int nouvelleTaille = 0;
    for (int i = 0; i < tailleDirection; i++) {
        if (directions[i] != direction) {
            directions[nouvelleTaille] = directions[i];
            nouvelleTaille++;
        }
    }
    return nouvelleTaille;
}
Direction directionFromVelocity(Vecteur2D velocity) {
    if (velocity.x == 1 && velocity.y == 0) {
        return HAUT;
    } else if (velocity.x == -1 && velocity.y == 0) {
        return BAS;
    } else if (velocity.x == 0 && velocity.y == 1) {
        return DROITE;
    } else if (velocity.x == 0 && velocity.y == -1) {
        return GAUCHE;
    }
    return HAUT;
}
