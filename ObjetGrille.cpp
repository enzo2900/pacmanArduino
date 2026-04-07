#include "ObjetGrille.h"

// Permet d'obtenir l'id sauvegardé de l'objet.
// L'id sauvegardé permet de relancer la partie avec l'id original des objets.
uint8_t getSavedIdObject(ObjetGrille objet) {
    return (objet.id /10);
}