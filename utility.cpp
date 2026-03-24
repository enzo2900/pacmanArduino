#include "utility.h"
#include "map1.h"



int nombreDirectionPossible(Vecteur2D position,Direction directionPossibles[]);
// Est ce qu'un mur est présent au pixel demandée par rapport à la direction du regard.
bool estMurPresent(int directionX,int directionY, int pixelPosX, int pixelPosY) {
  // Regarde vers l'avant ou derriere
  byte pixel1 = 0;
  byte pixel2 = 0;
  byte pixel3 = 0;
  if (directionX == 1 || directionX == -1) {
    pixel1 = pgm_read_byte(&(map1[pixelPosX][pixelPosY-1]));
    pixel2 = pgm_read_byte(&(map1[pixelPosX][pixelPosY]));
    pixel3 = pgm_read_byte(&(map1[pixelPosX ][pixelPosY+1]));
    // Regarde vers la droite ou gauche
  } else if(directionY == 1 || directionY == -1){
    pixel1 = pgm_read_byte(&(map1[pixelPosX-1][pixelPosY]));
    pixel2 = pgm_read_byte(&(map1[pixelPosX][pixelPosY]));
    pixel3 = pgm_read_byte(&(map1[pixelPosX+1][pixelPosY]));
  }

  return pixel1 == 1 || pixel2 == 1 || pixel3 == 1;
}

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
