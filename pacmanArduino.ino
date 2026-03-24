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
#include "TimerThree.h"
#include "Utility.h"
int pinBtnGauche = 2;
int pinBtnDroit = 3;
int pinBtnHaut = 18;
int pinBtnBas = 19;

Vecteur2D pacmanPos = {2,2};
Vecteur2D pacmanVelocity = {-1,0};
Vecteur2D pacmanVelocityOptimist = {0,0};
Direction directionMemorise = NONE;

Fantome fantomes[NOMBRE_FANTOMES] = {
  {29,25, 0, 1,matrix.Color333(2, 7, 0)}
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

  for(int i = 0 ; i < NOMBRE_FANTOMES; i ++) {
    Fantome fantome = fantomes[i];
    drawFantome(fantome.posX,fantome.posY,fantome);
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

// Est ce qu'un mur est présent au pixel demandée par rapport à la direction du regard.
bool estMurPresent(int directionX,int directionY, int pixelPosX, int pixelPosY) {
  // Regarde vers l'avant ou derriere
  byte pixel1 = 0;
  byte pixel2 = 0;
  byte pixel3 = 0;
  if (directionX == 1 || directionX == -1) {
    pixel1 = pgm_read_byte(&(map1[pixelPosX][pixelPosY-1]));
    pixel2 = pgm_read_byte(&(map1[pixelPosX][pixelPosY]));
    pixel3 = pgm_read_byte(&(map1[pixelPosX ][pixelPosY+1]));
    // Regarde vers la droite ou gauche
  } else if(directionY == 1 || directionY == -1){
    pixel1 = pgm_read_byte(&(map1[pixelPosX-1][pixelPosY]));
    pixel2 = pgm_read_byte(&(map1[pixelPosX][pixelPosY]));
    pixel3 = pgm_read_byte(&(map1[pixelPosX+1][pixelPosY]));
  }

  return pixel1 == 1 || pixel2 == 1 || pixel3 == 1;
}

// Dessine pacman sur la grille
// Enleve les pixels de la derniere position de pacman
void drawPacman(int lastX, int lastY, int xPos, int yPos) {
  matrix.drawRect(lastX - 1, lastY - 1, 3, 3, couleurs[0]);
  matrix.drawRect(xPos - 1, yPos - 1, 3, 3, matrix.Color333(7, 7, 7));
}

// Dessine un fantome sur la grille
// Enleve les pixels de la derniere position du fantome
void drawFantome(int lastX, int lastY, Fantome fantome) {
  matrix.drawRect(lastX - 1, lastY - 1, 3, 3, couleurs[0]);
  matrix.drawRect(fantome.posX - 1, fantome.posY - 1, 3, 3, fantome.couleur);
}


// Dessine la map sur la grille
void drawMap(uint8_t map[WIDTH][_HIGH]) {
  Serial.println("Dessine la map");
  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < _HIGH; j++) {
      // Récupére l'adresse mémoire de la valeur du tableau à i, j puis lis la valeur dans la mémoire flash
      byte mapValue = pgm_read_byte(&(map1[i][j]));
      uint16_t couleur = couleurs[mapValue];
      matrix.drawPixel(i, j, couleur);
    }
  }
}
// Met a jour la position des fantomes
// Peut mettre a jour la velocity des fantomes si il touche un mur
void updateFantomes() {
  for(int i = 0 ; i < NOMBRE_FANTOMES ; i ++) {
    Fantome fantome = fantomes[i];
    int lastXPos = fantome.posX;
    int lastYPos = fantome.posY;
    
    int velocityXF = fantome.velocityX;
    int velocityYF = fantome.velocityY;
    
    int tailleDirections = nombreDirectionPossible({lastXPos,lastYPos},directions) ;
    if (tailleDirections > 2) {
        fantome = randomizeFantomeVelocity(fantome,directions,tailleDirections);
        fantome.posX = lastXPos + fantome.velocityX;
        fantome.posY = lastYPos + fantome.velocityY;
    } else if(!estMurPresent(velocityXF, velocityYF,fantome.posX + velocityXF *2,fantome.posY + velocityYF *2)) {
        fantome.posX = lastXPos + fantome.velocityX;
        fantome.posY = lastYPos + fantome.velocityY;
    } else {
        fantome = randomizeFantomeVelocity(fantome,directions,tailleDirections);
    }

    // Mise a jour du fantome dans le tableau
    fantomes[i] = fantome;
  }
}

int nombreDirectionPossible(Vecteur2D position,Direction directionPossibles[]) {
    int indexDirection = 0;
    // Haut toujours = 0
    if (!estMurPresent(1,0,position.x +2,position.y)) {
        directionPossibles[indexDirection] = HAUT;
        indexDirection++;
  }
    // Droite ou gauche toujours entre 0 et indexDirection-1
    if (!estMurPresent(0,1,position.x ,position.y + 2)) {
        directionPossibles[indexDirection] = DROITE;
        indexDirection++;
    }
    if (!estMurPresent(0,-1,position.x,position.y- 2)) {
        directionPossibles[indexDirection] = GAUCHE;
        indexDirection++;
    }

    // Bas toujours = indexDirection-1
    if (!estMurPresent(-1,0,position.x -2,position.y)) {
        directionPossibles[indexDirection] = BAS;
        indexDirection++;
    }

    return indexDirection;

}
// Donne une nouvelle vélocity à un fantome de manière aléatoire
Fantome randomizeFantomeVelocity(Fantome fantome,Direction directionPossibles[4],int tailleTableauAssigne) {
    uint8_t choixDirection =  (uint8_t)random(tailleTableauAssigne);
    switch(directionPossibles[choixDirection]) {
        case HAUT:
            fantome.velocityX = 1;

            fantome.velocityY = 0;
            break;
        case BAS:
            fantome.velocityX = -1;
            printf("Nouveau velocity %d\n" , fantome.velocityX);
            //Serial.println("Nouveau velocity " + fantome.velocityX);
            fantome.velocityY = 0;
            break;
        case DROITE:
            fantome.velocityX = 0;
            fantome.velocityY = 1;
            break;
        case GAUCHE:
            fantome.velocityX =0;
            fantome.velocityY = -1;
            break;
        default :
            break;
    }
    return fantome;
}
 
//Doit être appelé seulement après la mise a jour du mouvement de pacman et des fantomes
bool pacmanToucheFantome() {

  for(int i = 0 ; i < NOMBRE_FANTOMES ; i ++) {
    Fantome fantome = fantomes[i];
    Vecteur2D positionFantome = {fantome.posX,fantome.posY};
    Vecteur2D positionPacman = {pacmanPixelX, pacmanPixelY};

    int distance = positionPacman.distance(positionFantome);

    // DIstance -2 pour prendre en compte la largeur ou longueur de pacman et du fantome 
    if(distance -2 < 0) {
      return true;
    }
  }
  return false;
}
// Met a jour la position de pacman par rapport à sa position
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
  
}
void pacmanMeurt() {
  // TODO la partie est perdu ou perd une vie
  partiePerdu = true;
}
void loop() {

  delay(400);
  pacManMouv();
  updateFantomes();
  if(pacmanToucheFantome()) {
    pacmanMeurt();
  }
}
