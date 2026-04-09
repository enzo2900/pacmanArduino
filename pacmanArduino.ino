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
#include "TimerFour.h"
#include "Direction.h"
#include "Fantome.h"
#define NOMBRE_FANTOMES 3
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
  Fantome({26,30}, {0, -1},matrix.Color333(2, 7, 0),POURSUIT),
   Fantome({33,30}, {0, 1},matrix.Color333(6, 0, 0),OPTIMISTE),
   Fantome({29,30}, {0, 1},matrix.Color333(6, 6, 0),ALEATOIRE),
};

void setup() {
  
  Serial.begin(9600);
  Serial.println("Setup");
  Serial2.begin(9600);
  while (Serial2.available()) Serial2.read();
  Timer3.initialize(150000);
  Timer4.initialize(150000);
  //Timer3.attachInterrupt(pacManMouv,1000000);
  Serial.println(objects[0].position.x);
  
  matrix.begin();
  
  lancerPartie();
}
void lancerPartie() {
  matrix.fillScreen(matrix.Color333(0,0,0));
  delay(1000);
  partiePerdu = false;
  partieGagne = false;
  pacmanPos = {2,2};
  resetObjects();
  
  nombreBillesARecuperer = getNombreBilles();
  drawMap(map1);
  Vecteur2D fantome1PastPos = fantomes[0].position.copy();
  Vecteur2D fantome2PastPos = fantomes[1].position.copy();
  Vecteur2D fantome3PastPos = fantomes[2].position.copy();
  fantomes[0].updatePos({26,29});
  fantomes[1].updatePos({29,29});
  fantomes[2].updatePos({33,29});
  for(Fantome& fantome : fantomes) {
      fantome.indexChemin = -1;
  }

  drawPacman(pacmanPos.x, pacmanPos.y, pacmanPos.x, pacmanPos.y);
  fantomes[0].draw(fantome1PastPos);
  fantomes[1].draw(fantome2PastPos);
  fantomes[2].draw(fantome3PastPos);
  delay(300);
  drawObjets();

  delay(1000);
  Timer3.attachInterrupt(pacManMouv,200000);
  Timer4.attachInterrupt(fantomeLoop,400000);
  score = 0;
  afficherTexteScore();
  afficherScore(score);
}

void gauche() {
  if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(GAUCHE);
  directionMemorise = GAUCHE;
}

void droite() {
    if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(DROITE);
  directionMemorise = DROITE;
}

void haut() {
    if(partiePerdu || partieGagne){
    lancerPartie();
    return;
  }
  pacmanVelocityOptimist = getVecteurFrom(HAUT);
  directionMemorise = HAUT;
}

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
    //zdelay(500);
    int distance = pacmanPos.distanceRectangulaire(fantome.position);
    //Serial.println("Distance");
    //Serial.println(distance);
    // DIstance -3 pour prendre en compte la largeur ou longueur de pacman et du fantome 
    if(distance -3 < 0) {
      return true;
    }
  }
  return false;
}

void verifierPacmanToucheObjet() {
  int indexObjet = rechercheBinaireObjets(pacmanPos,objects,getTailleTableauObjets());
  if(indexObjet != -1) {
    ObjetGrille objet = objects[indexObjet];

    recupererObjet(objet);
    
    // Mise a jour du type d'objet affiché
    objet.id = getSavedIdObject(objet) *10;
    objects[indexObjet] = objet;
  }
}
void partieGagn() {
  partieGagne = true;
  Timer3.detachInterrupt();
  Timer4.detachInterrupt();
  drawEcranVictoire();
}

void recupererObjet(ObjetGrille objet) {
  switch(objet.id %10) {
    // Bille
    case ID_BILLE: 
      //Serial.println("Touche Bille");
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
void partiePerd() {
  partiePerdu = true;
  Timer3.detachInterrupt();
  Timer4.detachInterrupt();
  drawEcranDefaite();
}
void pacmanMeurt() {
  // TODO la partie est perdu ou perd une vie
  partiePerd();
}  
void inputHandling() {
  if (Serial2.available() > 0) {
    char command = Serial2.read();

    //delay(5);
    // Serial.println(command);
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
        //Serial.println(command);
    }
  }
}
void fantomeLoop() {
  if(partiePerdu || partieGagne) return;
  //delay(200);
  //pacManMouv();
  updateFantomes();
  if(pacmanToucheFantome()) {
    pacmanMeurt();
  }
}
void loop() {
  
  inputHandling();
  
}
