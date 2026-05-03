/*!
 * @file pacmanArduino.ino
 * @brief Jeu de Pacman sur matrice 64x64
 * @author Cluzel Enzo, Faussurier Mateo
 */
#include <avr/pgmspace.h>
#include "map.h"
#include "Utility.h"
#include "Vecteur2D.h"
#include "TimerThree.h"
#include "TimerFour.h"
#include "Direction.h"
#include "Fantome.h"
// Nombre de fantomes affichés
#define NOMBRE_FANTOMES 3
// id des billes, une bille est représentée par cet id avec 
#define ID_BILLE 2
#define ID_POUVOIR 3

int nombreBillesARecuperer;
int score = 0;
Vecteur2D pacmanPos = {2,2};
Vecteur2D pacmanVelocity = {-1,0};
Vecteur2D pacmanVelocityOptimist = {0,0};
Direction directionMemorise = NONE;

volatile bool partiePerdu = false;
volatile bool partieGagne = false;
Fantome fantomes[NOMBRE_FANTOMES] = {
  Fantome({26,30}, {0, -1},matrix.Color333(3, 3, 6),POURSUIT,260),
   Fantome({33,30}, {0, 1},matrix.Color333(6, 0, 0),POURSUIT,200),
   Fantome({29,30}, {0, 1},matrix.Color333(5, 2, 2),ALEATOIRE,100),
};

void setup() {
  
  Serial.begin(9600);
  Serial.println("Setup");
  Serial2.begin(9600);
  while (Serial2.available()) Serial2.read();
  Timer3.initialize(150000);
  Timer4.initialize(150000);
  Serial.println(objects[0].position.x);
  
  matrix.begin();
  
  lancerPartie();
}

/**
 * Le lancement d'une parti est dinifi par la mise en place des éléments sur
 * la matrice, puis le lancement des déplacements des fantômes.
 */
void lancerPartie() {
  matrix.fillScreen(matrix.Color333(0,0,0));
  delay(1000);
  partiePerdu = false;
  partieGagne = false;
  pacmanPos = {2,2};
  resetObjects();
  
  fantomes[0].updatePos({26,29});
  fantomes[1].updatePos({29,29});
  fantomes[2].updatePos({33,29});
  for(Fantome& fantome : fantomes) {
      fantome.indexChemin = -1;
  }
  Vecteur2D fantome1PastPos = fantomes[0].position.copy();
  Vecteur2D fantome2PastPos = fantomes[1].position.copy();
  Vecteur2D fantome3PastPos = fantomes[2].position.copy();
  drawPacman(pacmanPos.x, pacmanPos.y, pacmanPos.x, pacmanPos.y);
  fantomes[0].draw(fantome1PastPos);
  fantomes[1].draw(fantome2PastPos);
  fantomes[2].draw(fantome3PastPos);
  nombreBillesARecuperer = getNombreBilles();
  drawMap(map1);
  delay(300);
  drawObjets();

  delay(1000);
  Timer3.attachInterrupt(pacManMouv,200000);
  Timer4.attachInterrupt(fantomeLoop,300000);
  score = 0;
  afficherTexteScore();
  afficherScore(score);
}

/**
 * Défini la direction de pacman à gauche.
 */
void gauche() {
  if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(GAUCHE);
  directionMemorise = GAUCHE;
}

/**
 * Défini la direction de pacman à droite.
 */
void droite() {
    if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(DROITE);
  directionMemorise = DROITE;
}

/**
 * Défini la direction de pacman en haut.
 */
void haut() {
    if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(HAUT);
  directionMemorise = HAUT;
}

/**
 * Défini la direction de pacman en bas.
 */
void bas() {
    if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(BAS);
  directionMemorise = BAS;
}

// Met a jour la position des fantomes
// Peut mettre a jour la velocity des fantomes si il touche un mur
void updateFantomes() {
  for(Fantome& fantome : fantomes) {
    fantome.update(pacmanPos);
  }
}

//Doit être appelé seulement après la mise a jour du mouvement de pacman et des fantomes
bool pacmanToucheFantome() {

  for(int i = 0 ; i < NOMBRE_FANTOMES ; i ++) {
    Fantome fantome = fantomes[i];
    int distance = pacmanPos.distanceRectangulaire(fantome.position);
    // DIstance -3 pour prendre en compte la largeur ou longueur de pacman et du fantome 
    if(distance -3 < 0) {
      return true;
    }
  }
  return false;
}

/**
 * Effectue une recherche sur les 8 pixels extérieurs de pacman pour savoir si un objet s'y trouve.
 */
void verifierPacmanToucheObjet() {
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x,pacmanPos.y-1},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x-1,pacmanPos.y},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x+1,pacmanPos.y},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x,pacmanPos.y+1},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x+1,pacmanPos.y-1},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x-1,pacmanPos.y-1},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x+1,pacmanPos.y+1},objects,getTailleTableauObjets()));
  recupererObjetPacman( rechercheBinaireObjets({pacmanPos.x-1,pacmanPos.y+1},objects,getTailleTableauObjets()));
}

/**
 * Récupère une bille si elle remplit les conditions pour.
 */
void recupererObjetPacman(int indexObjet) {
  if(indexObjet != -1) {
    ObjetGrille objet = objects[indexObjet];

    recupererObjet(objet);
    
    // Mise a jour du type d'objet affiché
    objet.id = getSavedIdObject(objet) *10;
    objects[indexObjet] = objet;
  }
}

/**
 * Lance le protocole de partie gagnée.
 */
void partieGagn() {
  partieGagne = true;
  Timer3.detachInterrupt();
  Timer4.detachInterrupt();
  drawEcranVictoire();
}

/**
 * Récupère une bille.
 * @param objet La bille récupérée par pacman.
 */
void recupererObjet(ObjetGrille objet) {
  switch(objet.id %10) {
    // Bille
    case ID_BILLE:
      score += 50;
      afficherScore(score);
      nombreBillesARecuperer -=1;
      if(nombreBillesARecuperer == 0) {
        partieGagn();
      }
      
      break;
    // Pouvoirs
    case ID_POUVOIR :
       break;
    default :
      break;
  }

}

/**
 * Déplace pacman sur la carte.
 */
void pacManMouv(){
  if(directionMemorise != NONE && !estMurPresent(pacmanVelocityOptimist,pacmanPos + pacmanVelocityOptimist *2)) {
    pacmanVelocity = pacmanVelocityOptimist;
    Vecteur2D copy = pacmanPos.copy();
    int lastXPos = copy.x;
    int lastYPos = copy.y;
    pacmanPos = pacmanPos+ pacmanVelocity;
    drawPacman(lastXPos,lastYPos,pacmanPos);
    directionMemorise = NONE;
  } else if (!estMurPresent(pacmanVelocity,pacmanPos + pacmanVelocity *2)) {
    Vecteur2D copy = pacmanPos.copy();
    int lastXPos = copy.x;
    int lastYPos = copy.y;
    pacmanPos = pacmanPos+ pacmanVelocity;
    drawPacman(lastXPos,lastYPos,pacmanPos);
  }
  
  verifierPacmanToucheObjet();
}

/**
 * Lance le protocole de partie perdue.
 */
void partiePerd() {
  partiePerdu = true;
  Timer3.detachInterrupt();
  Timer4.detachInterrupt();
  drawEcranDefaite();
}

/**
 * Lance le protocole lorsque pacman meurt.
 */
void pacmanMeurt() {
  partiePerd();
}

/**
 * Récupère les inputs de déplacement de pacman.
 */
void inputHandling() {
  if (Serial2.available() > 0) {
    char command = Serial2.read();
    switch (command) {
      case 'g' :
        gauche();
        break;
      case 'd':
        droite();
        break;
      case 'h':
        haut();
        break;
      case 'b':
        bas();
        break;
      default:
        break;
    }
  }
}

/**
 * Déplacement continu des fantômes.
 */
void fantomeLoop() {
  if(partiePerdu || partieGagne) return;
  if(pacmanToucheFantome()) {
    pacmanMeurt();
  }
}

void loop() {
  inputHandling();
  if(partiePerdu || partieGagne) return;
  updateFantomes();
}
