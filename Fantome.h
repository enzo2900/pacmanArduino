/**
 * Interface de Fantome.cpp
 */

#ifndef FANTOME_H
#define FANTOME_H
#include "Utility.h"
#include "Vecteur2D.h"
#include "Direction.h"
#include "FantomeRechercheChemin.h"
enum ComportementFantome {
    ALEATOIRE,
    POURSUIT,
    OPTIMISTE,
};
class Fantome {
    
public:
    Fantome(Vecteur2D basePosition, Vecteur2D baseVelocity,uint16_t couleur,ComportementFantome comportement,long timeBeforeMovement);
    // Met à jour la position du fantome par rapport à son comportement.
    void update(Vecteur2D pacmanPos);

    Vecteur2D getPos();
    void updatePos(Vecteur2D pos);
    // Dessine le fantome sur la matrice.
    void draw(Vecteur2D pastPos) ;
    // Change la vélocité du fantome par rapport à la direction donnée
    void changeVelocityWithDirection(Direction direction);
    // Donne une nouvelle vélocity au fantome de manière aléatoire
    void randomizeFantomeVelocity(Direction directionPossibles[4],int tailleTableauAssigne);
    Vecteur2D position{};
    Vecteur2D velocity{};
     uint16_t couleur;
     int indexChemin = -1;
private:
    
    // Le fantome suit un chemin optimisé vers pacman
    void updatePoursuit(Vecteur2D pacmanPos);

    // Le fantome se déplace aléatoirement
    void updateAleatoire(Vecteur2D pacmanPos);

    // Le fantome se déplace de manière optimiste vers pacman et peut faire des allers retours
    void updateOptimiste(Vecteur2D pacmanPos);

    // Ajoute toutes les positions au chemin interne du fantome jusqu'a la fin du chemin
    void addChemin(int tailleChemin);
    unsigned long timeBeforeMovement;
    unsigned long currentTimeForMovement;
    Direction lastDirection = NONE;
    Direction directions[4]{};
    ComportementFantome comportement;
    Vecteur2D chemin[VISITE_TAILLE_MAX];
    int tailleChemin ;
   
   

};
#endif 