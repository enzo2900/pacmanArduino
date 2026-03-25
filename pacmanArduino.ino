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

Vecteur2D pacmanPos = {2,2};
Vecteur2D pacmanVelocity = {-1,0};

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  Serial.println();
  matrix.begin();
  drawMap(map1);
  delay(100);
  drawPacman(pacmanVelocity.x, pacmanVelocity.y, pacmanPos.x, pacmanPos.y);
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

  delay(100);
  pacManMouv();
}
