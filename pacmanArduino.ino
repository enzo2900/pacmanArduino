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

int pacmanPixelX = 2;
int pacmanPixelY = 2;
int velocityX = -1;
int velocityY = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  Serial.println();
  matrix.begin();
  drawMap(map1);
  delay(100);
  drawPacman(pacmanPixelX, pacmanPixelY, pacmanPixelX, pacmanPixelY);
}


void pacManMouv(){
  if(!estMurPresent(velocityX, velocityY,pacmanPixelX + velocityX *2,pacmanPixelY + velocityY *2)) {
    int lastXPos = pacmanPixelX;
    int lastYPos = pacmanPixelY;
    pacmanPixelX += velocityX;
    pacmanPixelY += velocityY;
    drawPacman(lastXPos,lastYPos,pacmanPixelX,pacmanPixelY);
  }
  
}

void loop() {

  delay(100);
  pacManMouv();
}
