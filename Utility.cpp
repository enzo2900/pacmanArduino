#include "Utility.h"
#include "map.h"


Vecteur2D getPositionFrom(Vecteur2D position,Direction direction) {

    return position + getVecteurFrom(direction) *2;
}
void printDirection(Direction direction) {
    switch(direction) {
        case HAUT :
            Serial.println("HAUT");
            break;
        case BAS : 
            Serial.println("BAS");
            break;
        case DROITE : 
            Serial.println("DROITE");
            break;
        case GAUCHE : 
            Serial.println("GAUCHE");
            break;
        default:
            Serial.println("Direction inconnue");
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
        default:
            return {0,0};
    }
}
Direction closestToTarget(Vecteur2D position, Vecteur2D target, Direction directionPossibles[], int taille) {
    Direction bestDirection = HAUT;
    int bestDistance = 999;
    for (int i = 0; i < taille; i++) {
        int dist = getPositionFrom(position,directionPossibles[i]).distance(target);
        //Serial.println("Direction Numéro :"+i);
        //Serial.println(dist);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestDirection = directionPossibles[i];
        }
    }
    return bestDirection;
}
int nombreDirectionsPossible(Vecteur2D position,Direction directionPossibles[]) {
    int indexDirection = 0;
    // Haut toujours = 0
    if (!estMurPresent(getVecteurFrom(HAUT),position + (getVecteurFrom(HAUT)*2))) {
        directionPossibles[indexDirection] = HAUT;
        indexDirection++;
    }
    // Droite ou gauche toujours entre 0 et indexDirection-1
    if (!estMurPresent(getVecteurFrom(DROITE),position + (getVecteurFrom(DROITE)*2))) {
        directionPossibles[indexDirection] = DROITE;
        indexDirection++;
    }
    if (!estMurPresent(getVecteurFrom(GAUCHE),position + (getVecteurFrom(GAUCHE)*2))) {
        //Serial.println("Gauche intersection");
        delay(50);
        directionPossibles[indexDirection] = GAUCHE;
        indexDirection++;
    } else {
        //Serial.println("Pas gauche intersection");
    }

    // Bas toujours = indexDirection-1
    if (!estMurPresent(getVecteurFrom(BAS),position + (getVecteurFrom(BAS)*2))) {
        directionPossibles[indexDirection] = BAS;
        //Serial.println("Bas inter");
        
        indexDirection++;
    } else {
        //Serial.println("Pas bas intersection");
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
int redrawObjetAt(Vecteur2D pixel) {
    int index1 = rechercheBinaireObjets(pixel,objects);
    Serial.println("Vecteur2D");
    Serial.println(pixel.x);
    Serial.println(pixel.y);
    Serial.println("Index");
    Serial.println(index1);
    if(index1 != -1) {
        drawObjet(objects[index1]);
    }
}
int redrawObjetsAt(Vecteur2D pixel1, Vecteur2D pixel2, Vecteur2D pixel3) {
    redrawObjetAt(pixel1);
    redrawObjetAt(pixel2);
    redrawObjetAt(pixel3);
}



// Recherche binaire du tableau d'objet pour trouver en temps logarithmique l'objet de la grille à la position demandée.
// Si l'objet n'est pas trouvé un -1 est retourné. 
int rechercheBinaireObjets(Vecteur2D target, ObjetGrille objects[]) {
  int fin = sizeof(objects)/sizeof(*objects)-1;
  int debut = 0;
  while(debut <= fin) {
    int millieu = debut +(fin - debut)/2;
    Vecteur2D millieuPos = objects[millieu].position;
    if(millieuPos.x < target.x) {
      debut = millieu+1;
    }  else if(millieuPos.x > target.x) {
      fin = millieu -1;
    } else {
        if(millieuPos.y > target.y) {
          fin = millieu -1;
        }  else if(millieuPos.y < target.y) {
          debut = millieu +1;
        } else {
          return millieu;
        }
    }

  }

  return -1;
}