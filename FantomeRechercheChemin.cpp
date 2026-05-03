/**
 * On cherche un chemin avec différent algorithme afin d'attraper pacman.
 */

#include "FantomeRechercheChemin.h"
#include "map.h"
#include "Vecteur2D.h"
#include "Utility.h"

Vecteur2D cheminFantome[VISITE_TAILLE_MAX];
Vecteur2D caseVoisins[4] ;
bool estUnMurA(Vecteur2D position,Direction direction) {
    int x = position.x;
    int y = position.y;

    return estMurPresent(getVecteurFrom(direction).x,getVecteurFrom(direction).y,x,y);
}

/**
 * Récupère l'information concernant les cases autour du fantôme afin d'obtenir
 * les directions possibles.
 * @param caseActuelle La case où se situ le fantôme.
 */
int caseVoisinDisponible(Vecteur2D caseActuelle) {
    int nombresVoisins = 0;
    if (!estUnMurA(caseActuelle + getVecteurFrom(HAUT) *2,HAUT) ) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(HAUT);
        nombresVoisins++;
    }
    if (!estUnMurA(caseActuelle + getVecteurFrom(BAS) *2,BAS) ) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(BAS);
        nombresVoisins++;
    }
    if (!estUnMurA(caseActuelle + getVecteurFrom(DROITE) *2,DROITE)) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(DROITE);
        nombresVoisins++;
    }
    if (!estUnMurA(caseActuelle + getVecteurFrom(GAUCHE) *2,GAUCHE)) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(GAUCHE);
        nombresVoisins++;

    }

    return nombresVoisins;
}

/**
 * Supprime un chemin s'il est de longueur supérieur à un trouver.
 * Si l'index est supérieur à la taille actuelle -1 est renvoyé.
 * @param tab le tableau de la carte.
 * @param index le chemin actuel.
 * @param tailleTab la chemin trouvé.
 */
int removeTo(Vecteur2D tab[],int index, int tailleTab) {
    if (index > tailleTab || index < 0) {
        return -1;
    }
    for (int i = index; i < tailleTab-1; i++) {
        if(i+1 > tailleTab-1) {
            continue;
        }
        tab[i] = tab[i+1];
    }
    return tailleTab-1;

}

/**
 * Supprime un chemin s'il est de longueur supérieur à un trouver.
 * Si l'index est supérieur à la taille actuelle -1 est renvoyé.
 * @param tab le tableau de la carte.
 * @param index le chemin actuel.
 * @param tailleTab la chemin trouvé.
 */
int removetToInt(int tab[],int index, int tailleTab) {
   if (index > tailleTab || index < 0) {
        return -1;
    }
    for (int i = index; i < tailleTab-1; i++) {
        if(i+1 > tailleTab-1) {
            continue;
        }
        tab[i] = tab[i+1];
    }
    return tailleTab-1;
}

/**
 * Renvoie l'index de la position la plus proche de la cible
 * @param positions les positions possibles.
 * @param cible position de la cible.
 * @param taille le nomobre de position possibles.
 */
int closestPosition(const Vecteur2D positions[], Vecteur2D cible, int taille) {
    int bestDistance= 999;
    int indexClosest;
    for (int i = 0; i < taille; i++) {
        Vecteur2D pos = positions[i];
        if (bestDistance > pos.distance(cible)) {
            bestDistance = pos.distance(cible);
            indexClosest = i;
        }
    }
    return indexClosest;
}

/**
 * Renvoie true si le vecteur2D à vérifier est présent dans le tableau des
 * positions, sinon false.
 * @param positions la liste des positions.
 * @param toCheck la position à vérifier.
 * @param taillePositions le nombre de position à vérifier.
 */
bool isVisited(const Vecteur2D positions[], Vecteur2D toCheck, int taillePositions) {
    for (int i = 0; i < taillePositions; i++) {
        Vecteur2D pos = positions[i];
        if (toCheck.x == pos.x && toCheck.y == pos.y) {
            return true;
        }
    }
    return false;
}

/**
 * Inverse le chemin.
 * @param tailleChemin Le chemin à inverser.
 */
void renverserChemin(int tailleChemin) {
    int begin = 0;
    int end = tailleChemin-1;
    while (begin < end-1) {
        Vecteur2D first = cheminFantome[begin];
        Vecteur2D second = cheminFantome[end];
        cheminFantome[begin] = second;
        cheminFantome[end] = first;
        begin++;
        end--;
    }


}
/**
 * Algorithme de recherche de chemin vers pacman.
 * @param positionFantome La position du fantôme.
 * @param cible La position de pacman.
 */
int recherche(Vecteur2D positionFantome, Vecteur2D cible) {
    int tailleToVisit = 0;
    int tailleVisited = 0;
    int tailleIndexParents = 0;

    Vecteur2D toVisit[VISITE_TAILLE_MAX];
    Vecteur2D visited[VISITE_TAILLE_MAX];
    int16_t toVisitIndexParents[VISITE_TAILLE_MAX];
    int16_t indexParents[VISITE_TAILLE_MAX];
    toVisit[tailleToVisit] = positionFantome;
    toVisitIndexParents[tailleToVisit] = -1;
    visited[tailleVisited] = positionFantome;
    indexParents[tailleVisited] = -1;
    tailleToVisit++;
    tailleVisited++;
    tailleIndexParents ++;
    while (tailleVisited < VISITE_TAILLE_MAX-1 && tailleToVisit != 0) {
        int bestIndex =closestPosition(toVisit,cible,tailleToVisit);
        if (toVisit[bestIndex].x == cible.x && toVisit[bestIndex].y == cible.y) {
            break;
        }
        int taille = caseVoisinDisponible(toVisit[bestIndex]);
        tailleToVisit = removeTo(toVisit,bestIndex,tailleToVisit);
        int indexParent = toVisitIndexParents[bestIndex];
        removetToInt(toVisitIndexParents,bestIndex,tailleToVisit+1);
        for (int i = 0; i < taille; i++) {
            if (tailleVisited > VISITE_TAILLE_MAX-1) {
                break;
            }
            if (!isVisited(visited, caseVoisins[i], tailleVisited)) {
                visited[tailleVisited] = caseVoisins[i];
                indexParents[tailleVisited] = indexParent ;
                toVisitIndexParents[tailleToVisit] = tailleVisited;
                tailleIndexParents ++;
                tailleVisited++;
                toVisit[tailleToVisit] = caseVoisins[i];
                tailleToVisit++;
            }
        }

    }

    int indexBestPos = closestPosition(visited,cible,tailleVisited);
    int tailleChemin = 0;
    int currentParent = indexParents[indexBestPos];
    cheminFantome[tailleChemin] = visited[indexBestPos];
    tailleChemin++;
    int iterations = 0;
    while (currentParent != -1) {
        iterations ++;
        cheminFantome[tailleChemin] = visited[currentParent];
        currentParent = indexParents[currentParent];
        tailleChemin++;
        if(iterations > 200) {
            break;
        }

    }
    renverserChemin(tailleChemin);
    return tailleChemin;
}
