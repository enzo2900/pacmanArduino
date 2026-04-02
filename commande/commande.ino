/**
* Récupère les commandes du joueurs et les transmets à l'arduino gérant le
* fonctionnement du jeu.
*/

int pinBtnGauche = 2;
int pinBtnDroit = 3;
int pinBtnHaut = 18;
int pinBtnBas = 19;
volatile unsigned long dela = 10000L;
volatile unsigned long lastTime;
void setup() {
  // Communication
  lastTime = millis();
  Serial2.begin(9600);
  Serial.begin(9600);
  pinMode(pinBtnGauche,INPUT_PULLUP);
  pinMode(pinBtnDroit,INPUT_PULLUP);
  pinMode(pinBtnHaut,INPUT_PULLUP);
  pinMode(pinBtnBas,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinBtnGauche), gauche, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnDroit), droite, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnHaut), haut, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBtnBas), bas, FALLING);

}

bool delayBouton() {
  long current = millis();
  if(current > lastTime +dela) {
    lastTime = current;
    return true;
  }
  return false;
}
void gauche() {
  if(!delayBouton) return; 
  Serial2.println('g');
   Serial.println('g');

}

void droite() {
  if(!delayBouton) return; 
  Serial2.println('d');
  Serial.println('d');

}

void haut() {
  if(!delayBouton) return; 
  Serial2.println('h');
  Serial.println('h');

}

void bas() {
  if(!delayBouton) return; 
  Serial2.println('b');
  Serial.println('b');
}

void loop() {
  //delay(1000);
  //Serial2.println('g');
}