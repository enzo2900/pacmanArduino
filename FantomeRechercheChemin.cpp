//
// Created by enzoc on 26/03/2026.
//

#include "FantomeRechercheChemin.h"

#include "map.h"
#include "Vecteur2D.h"
#include "Utility.h"
struct Chemin {
    int tailleChemin;
    Vecteur2D chemin[VISITE_TAILLE_MAX];

};
Vecteur2D cheminFantome[VISITE_TAILLE_MAX];
Vecteur2D caseVoisins[4] ;
Direction directionPossibles[4];
bool estUnMurA(Vecteur2D position,Direction direction) {
    int x = position.x;
    int y = position.y;

    return estMurPresent(getVecteurFrom(direction).x,getVecteurFrom(direction).y,x,y);
}
int caseVoisinDisponible(Vecteur2D caseActuelle) {
    int nombresVoisins = 0;
    if (!estUnMurA(caseActuelle + getVecteurFrom(HAUT) *2,HAUT) ) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(HAUT);
        directionPossibles[nombresVoisins] = HAUT;
        nombresVoisins++;
    }
    if (!estUnMurA(caseActuelle + getVecteurFrom(BAS) *2,BAS) ) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(BAS);
        directionPossibles[nombresVoisins] = BAS;
        nombresVoisins++;
    }
    if (!estUnMurA(caseActuelle + getVecteurFrom(DROITE) *2,DROITE)) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(DROITE);
        directionPossibles[nombresVoisins] = DROITE;
        nombresVoisins++;
    }
    if (!estUnMurA(caseActuelle + getVecteurFrom(GAUCHE) *2,GAUCHE)) {
        caseVoisins[nombresVoisins] = caseActuelle + getVecteurFrom(GAUCHE);
        directionPossibles[nombresVoisins] = GAUCHE;
        nombresVoisins++;

    }

    return nombresVoisins;
}
int addTo(Vecteur2D tab[], int tailleTab, Vecteur2D tab2[], int tailleTab2) {
    for (int i = 0; i < tailleTab2; i++) {
        int index = i + tailleTab;
        if (index >= VISITE_TAILLE_MAX) {
            return -1;
        }
        tab[index] = tab2[i];
    }
    return tailleTab + tailleTab2;
}

// Enleve l'élément à l'index
// Si l'index est supérieur à la taille actuelle -1 est renvoyé
int removeTo(Vecteur2D tab[],int index, int tailleTab) {
    if (index > tailleTab) {
        return -1;
    }
    for (int i = index; i < tailleTab-1; i++) {
        tab[i] = tab[i+1];
    }
    return tailleTab-1;

}
int removetToInt(int tab[],int index, int tailleTab) {
    if (index > tailleTab) {
        return -1;
    }
    for (int i = index; i < tailleTab-1; i++) {
        tab[i] = tab[i+1];
    }
    return tailleTab-1;
}
int closestPosition(Vecteur2D positions[], Vecteur2D cible, int taille) {
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

bool isVisited(Vecteur2D positions[], Vecteur2D toCheck, int taillePositions) {
    for (int i = 0; i < taillePositions; i++) {
        Vecteur2D pos = positions[i];
        if (toCheck.x == pos.x && toCheck.y == pos.y) {
            return true;
        }
    }
    return false;
}
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

int recherche(Vecteur2D positionFantome, Vecteur2D cible) {
    int tailleToVisit = 0;
    int tailleVisited = 0;
    int tailleIndexParents = 0;

    Vecteur2D toVisit[VISITE_TAILLE_MAX];
    Vecteur2D visited[VISITE_TAILLE_MAX];
    Direction directions[VISITE_TAILLE_MAX];
    int toVisitIndexParents[VISITE_TAILLE_MAX];
    int indexParents[VISITE_TAILLE_MAX];
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
        /*int taille = caseVoisinDisponible(toVisit[0]);
        tailleToVisit = removeTo(toVisit,0,tailleToVisit);
        int indexParent = toVisitIndexParents[0];
        removetToInt(toVisitIndexParents,0,tailleToVisit+1);*/
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
    while (currentParent != -1) {
        cheminFantome[tailleChemin] = visited[currentParent];
        currentParent = indexParents[currentParent];
        tailleChemin++;

    }
    renverserChemin(tailleChemin);
    return tailleChemin;
}
