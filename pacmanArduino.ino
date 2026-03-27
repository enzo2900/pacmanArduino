/*!
 * @file testRGBMatrix.ino
 * @brief Run the routine to test the RGB LED Matrix Panel
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie]](jie.tang@dfrobot.com)
 * @version  V1.0.1
 * @date  2022-03-23
 * @url https://github.com/DFRobot/DFRobot_RGBMatrix
 */
#include <avr/pgmspace.h>// Hardware-specific library
#include "map.h"
#include "Utility.h"
#include "Vecteur2D.h"
#include "TimerThree.h"
#include "Direction.h"
#include "Fantome.h"
#define NOMBRE_FANTOMES 1
#define ID_BILLE 2
#define ID_POUVOIR 3

int pinBtnGauche = 2;
int pinBtnDroit = 3;
int pinBtnHaut = 18;
int pinBtnBas = 19;

Vecteur2D pacmanPos = {2,2};
Vecteur2D pacmanVelocity = {-1,0};
Vecteur2D pacmanVelocityOptimist = {0,0};
Direction directionMemorise = NONE;

bool partiePerdu = false;
Fantome fantomes[NOMBRE_FANTOMES] = {
  Fantome({29,25}, {0, -1},matrix.Color333(2, 7, 0),POURSUIT)
  // {29,28, 0, 1,matrix.Color333(6, 0, 0)},
  // {29,31, 0, 1,matrix.Color333(6, 6, 0)}
};

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  Timer3.initialize(150000);
  Timer3.attachInterrupt(pacManMouv,1000000);
  
  matrix.begin();
  drawMap(map1);
  delay(100);
  drawPacman(pacmanPos.x, pacmanPos.y, pacmanPos.x, pacmanPos.y);
  Serial.println(fantomes[0].position.x);
  for(Fantome fantome : fantomes) {
    Serial.println("Salut");
    fantome.draw(fantome.position);
    Serial.println(fantome.getPos().x);
    //Serial.println(fantome.position.x);
  }
  // gestion des boutons
  pinMode(pinBtnGauche,INPUT_PULLUP);
  pinMode(pinBtnDroit,INPUT_PULLUP);
  pinMode(pinBtnHaut,INPUT_PULLUP);
  pinMode(pinBtnBas,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinBtnGauche), gauche, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnDroit), droite, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnHaut), haut, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnBas), bas, FALLING);

  drawObjets();
}

void gauche() {
  pacmanVelocityOptimist = getVecteurFrom(GAUCHE);
  directionMemorise = GAUCHE;
}

void droite() {
  pacmanVelocityOptimist = getVecteurFrom(DROITE);
  directionMemorise = DROITE;
}

void haut() {
  pacmanVelocityOptimist = getVecteurFrom(HAUT);
  directionMemorise = HAUT;
}

void bas() {
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
    int distance = pacmanPos.distance(fantome.position);

    // DIstance -2 pour prendre en compte la largeur ou longueur de pacman et du fantome 
    if(distance -2 < 0) {
      return true;
    }
  }
  return false;
}

void verifierPacmanToucheObjet() {
  int indexObjet = rechercheBinaireObjets(pacmanPos,objects);
  if(indexObjet != -1) {
    ObjetGrille objet = objects[indexObjet];

    recupererObjet(objet);
    
    // Mise a jour du type d'objet affiché
    objet.id = 0;
    objects[indexObjet] = objet;
  }
}

void recupererObjet(ObjetGrille objet) {
  switch(objet.id) {
    // Bille
    case ID_BILLE: 
      Serial.println("Touche Bille");
      break;
    // Pouvoirs
    case ID_POUVOIR :
       break;
    default :
      break;
  }

}
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
void pacmanMeurt() {
  // TODO la partie est perdu ou perd une vie
  partiePerdu = true;
}
void loop() {

  delay(1000);
  //pacManMouv();
  updateFantomes();
  if(pacmanToucheFantome()) {
    pacmanMeurt();
  }
}
