
#ifndef MAP_H
#define MAP_H
#include <avr/pgmspace.h>
#include <DFRobot_RGBMatrix.h>

#include <Arduino.h>
#include "Vecteur2D.h"
#define WIDTH 64
#define _HIGH 64
#define TAILLE_CASE 3

extern const DFRobot_RGBMatrix matrix;
extern const uint8_t map1[_HIGH][WIDTH] PROGMEM;
extern const  uint16_t couleurs[];

// Est ce qu'un mur est présent au pixel demandée par rapport à la direction du regard.
bool estMurPresent(int directionX,int directionY, int pixelPosX, int pixelPosY) ;
bool estMurPresent(int directionX,int directionY, Vecteur2D pos) ;
bool estMurPresent(Vecteur2D direction, Vecteur2D pos) ;

void drawEntity(Vecteur2D lastPos, Vecteur2D newPos, uint16_t couleur) ;
void drawEntity(int lastX, int lastY, int xPos, int yPos, uint16_t couleur);
void drawMap(uint8_t map[WIDTH][_HIGH]);
void drawPacman(int lastX, int lastY, Vecteur2D newPos) ;
void drawPacman(int lastX, int lastY, int xPos, int yPos);
#endif