/**
 * Regroupement des fonctions relatives aux déplacements des objets.
 */
#include "Utility.h"
#include "map.h"


Vecteur2D getPositionFrom(Vecteur2D position,Direction direction) {

    return position + getVecteurFrom(direction) *2;
}

/**
 * Affiche la direction choisi en console pour avoir un suivi des inputs en
 * développement.
 */
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

/**
 * Récupère la valeur du vecteur correspondant à une direction enum.
 * @param direction L'enum de la direction.
 */
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

/**
 * Evalu les directions se rapprochant le plus de pacman.
 * @param position Position du fantôme.
 * @param target Position de pacman.
 * @param directionPossibles La liste des directions possibles.
 * @param taille Nombre de direction possible.
 */
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

/**
 *  Récupère le nombre de direction possible à partir d'une position et des
 * murs autour de cette position.
 * @param position Position à partir de laquelle on veut connaître les
 *                 directions empreintable.
 * @param directionPossibles Tableau où on ajoute les directions possibles.
 */
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
        directionPossibles[indexDirection] = GAUCHE;
        indexDirection++;
    }

    // Bas toujours = indexDirection-1
    if (!estMurPresent(getVecteurFrom(BAS),position + (getVecteurFrom(BAS)*2))) {
        directionPossibles[indexDirection] = BAS;
        indexDirection++;
    }

    return indexDirection;

}

/**
 * Réinitialise les directions possibles.
 * @param directions La liste des direction.
 * @param direction La direction à concervé.
 * @param tailleDirection Nombre de direction initialement.
 */
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

/**
 * Déduis une direction à partir d'une vélocité.
 * @param velocity La vitesse à partir de laquelle on déduit la direction.
 */
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

/**
 * Dessine une bille à une position.
 * @param pixel la position du pixel représentant la bille à redessiner.
 */
int redrawObjetAt(Vecteur2D pixel) {
    int index1 = rechercheBinaireObjets(pixel,objects,getTailleTableauObjets());
    if(index1 != -1) {
        drawObjet(objects[index1]);
    }
}

/**
 * Redessine les objets présent sur les trois pixel derrière un fantôme si
 * nécessaire.
 * @param pixel1, pixel2, pixel3 Les pixels devant être redessinés.
 */
int redrawObjetsAt(Vecteur2D pixel1, Vecteur2D pixel2, Vecteur2D pixel3) {
    redrawObjetAt(pixel1);
    redrawObjetAt(pixel2);
    redrawObjetAt(pixel3);
}


// Recherche binaire du tableau d'objet pour trouver en temps logarithmique l'objet de la grille à la position demandée.
// Si l'objet n'est pas trouvé un -1 est retourné. 
int rechercheBinaireObjets(Vecteur2D target,const ObjetGrille objects[], int tailleTableau) {
  int fin = tailleTableau-1;
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