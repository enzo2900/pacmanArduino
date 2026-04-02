#ifndef OBJETGRILLE_H
#define OBJETGRILLE_H
struct ObjetGrille {
  Vecteur2D position;
  // Représentation de l'id de l'objet :
// - Partie entière (jusqu'à 99)
// - Première décimale : valeur initiale de l'ID, utilisée pour restaurer l'état au début d'une nouvelle partie.
// - Deuxième décimale : valeur modifiée pendant la partie.
  uint8_t id;
};

#endif