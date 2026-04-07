/**
* Récupère les commandes du joueurs et les transmets à l'arduino gérant le
* fonctionnement du jeu.
*/

int pinBtnGauche = 2;
int pinBtnDroit = 3;
int pinBtnHaut = 18;
int pinBtnBas = 19;

void setup() {
  // Communication
  Serial2.begin(9600);
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
  Serial2.println('g');
}

void droite() {
  Serial2.println('d');
}

void haut() {
  Serial2.println('h');

}

void bas() {
  Serial2.println('b');
}

void loop() {
}