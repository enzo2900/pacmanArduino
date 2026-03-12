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
#include <avr/pgmspace.h>
#include "map.h"
#include "Vecteur2D.h"
int pinBtnGauche = 2;
int pinBtnDroit = 3;
int pinBtnHaut = 18;
int pinBtnBas = 19;

Vecteur2D pacmanPos = {2,2};
Vecteur2D pacmanVelocity = {-1,0};

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  Timer3.initialize(150000);
  Timer3.attachInterrupt(pacManMouv,1000000);
  
  matrix.begin();
  drawMap(map1);
  delay(100);
  drawPacman(pacmanVelocity.x, pacmanVelocity.y, pacmanPos.x, pacmanPos.y);

  // gestion des boutons
  pinMode(pinBtnGauche,INPUT_PULLUP);
  pinMode(pinBtnDroit,INPUT_PULLUP);
  pinMode(pinBtnHaut,INPUT_PULLUP);
  pinMode(pinBtnBas,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinBtnGauche), gauche, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnDroit), droite, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnHaut), haut, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnBas), bas, FALLING);

}

void gauche() {
  velocityX = 0;
  velocityY = -1;
}

void droite() {
  velocityX = 0;
  velocityY = 1;
}

void haut() {
  velocityX = 1;
  velocityY = 0;
}

void bas() {
  velocityX = -1;
  velocityY = 0;
}


// Met a jour la position de pacman par rapport à sa position
void pacManMouv(){
  if(!estMurPresent(pacmanVelocity,pacmanPos + pacmanVelocity *2)) {
    int lastXPos = pacmanPos.x;
    int lastYPos = pacmanPos.x;
    pacmanPos = pacmanPos+ pacmanVelocity;
    drawPacman(lastXPos,lastYPos,pacmanPos);
  }
  
}

void loop() {

}
