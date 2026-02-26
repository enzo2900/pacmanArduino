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
 
#include <DFRobot_RGBMatrix.h> // Hardware-specific library

#define OE   	9
#define LAT 	10
#define CLK 	11
#define A   	A0
#define B   	A1
#define C   	A2
#define D   	A3
#define E   	A4
#define WIDTH 64
#define _HIGH	64
#define TAILLE_CASE 3
DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);

// 1 = Mur, 2 = bille, 3 = rien
uint16_t couleurs[] = {matrix.Color333(7, 0, 0),matrix.Color333(0, 7, 0),matrix.Color333(0, 0, 7)};
//uint16_t tabLog[WIDTH * _HIGH] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} 

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  uint16_t level[WIDTH * _HIGH] ;
  uint16_t arr[WIDTH];
  arr[1] = 1;
  level[1] = 1;
    // Printing array name
  Serial.println(level[1]);
  
  Serial.println("pointeur");
  for(int i = 0 ; i < WIDTH ; i++) {
    level[i] = 0;
  }
  //Serial.println((level)[1]);
  Serial.println("Pointeur set");
  
  Serial.println();
  matrix.begin();
  drawMap(level);
  Serial.println(matrix.width());

}

void drawMap(uint16_t* map) {
  Serial.println("Dessine la map");
  for(uint16_t i = 0; i < WIDTH; i++) {
    //for(uint16_t j = 0; i < _HIGH; i++) {
      //Serial.println()
      //Serial.println(map[i]);
      //uint16_t couleur = couleurs[map[i*(j*WIDTH)]]; 
      matrix.drawPixel(i, 1, matrix.Color333(7, 0, 0));
    //}
  }
}

void loop() {
  
}

