 //code_manette_finale
 
 //#include <VirtualWire_Config.h>
#include <VirtualWire.h>



// Pin potentiometre
const char potDirectionPin = A3;
const char potVitessePin = A4;
const char potlumieresPin = A5;

// Pin LED Vitesse
const int pinrouge1 = 2;
const int pinrouge2 = 4;
const int pinrouge3 = 5;
const int pinjaune1 = 6;
const int pinjaune2 = 7;
const int pinvert = 8;

// Etat initial LED Vitesse

 int staterouge1 = HIGH;
 int staterouge2 = HIGH;
 int staterouge3 = HIGH;
 int statejaune1 = HIGH;
 int statejaune2 = HIGH;
 int statevert = HIGH;




// Etat initial Message

//Message Bouton
int Message = 0;
int DernierMessage = Message;

//Message Direction
int Message2 = 0;
int DernierMessage2 = Message2;

//Message Vitesse
int Message3 = 0;
int DernierMessage3 = Message3;

//Message Son
int Message4 = 0;
int DernierMessage4 = Message4;




// Boutons

// pin bouttons

const int buttonPin1 = 9;
const int buttonPin2 = 10;
const int buttonPin3 = 11;
const int buttonPin4 = 12;
const int buttonPin5 = 13;
const char buttonPin6 = A0;
const char buttonPin7 = A1;
const char buttonPin8 = A2;


// Etat initial bouttons

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;
int buttonState8 = 0;




// fonction initialisant l'émetteur radio
void rd_send_setup(int speed_val,int pin){
 
  vw_setup(speed_val);
  vw_set_tx_pin(pin);
}


// fonction émettant le message (soit un code entre 11 et 45)
void rd_send_msg(int code){

// passe le code en chaîne de caractères
  char message[20];
  sprintf(message, "%i", code); 
  const char *msg = message;

  // envoi le message
  if(vw_send((uint8_t *)msg, strlen(msg))){

  }
}


void setup() {
 
  
  rd_send_setup(2000,3); // initialisation de l'émetteur radio




//Entrée bouttons

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  pinMode(buttonPin7, INPUT);
  pinMode(buttonPin8, INPUT);

//Sorties LED

  pinMode (pinrouge1, OUTPUT);
  pinMode (pinrouge2, OUTPUT);
  pinMode (pinrouge3, OUTPUT);
  pinMode (pinjaune1, OUTPUT);
  pinMode (pinjaune2, OUTPUT);
  pinMode (pinvert, OUTPUT);


//A l'allumage de la manette toutes les leds s'allument pendant une seconde.


  digitalWrite(pinrouge1, LOW);
  digitalWrite(pinrouge2, LOW);
  digitalWrite(pinrouge3, LOW);
  digitalWrite(pinjaune1, LOW);
  digitalWrite(pinjaune2, LOW);
  digitalWrite(pinvert, LOW);

  delay(1000);

  digitalWrite(pinrouge1, HIGH);
  digitalWrite(pinrouge2, HIGH);
  digitalWrite(pinrouge3, HIGH);
  digitalWrite(pinjaune1, HIGH);
  digitalWrite(pinjaune2, HIGH);
  digitalWrite(pinvert, HIGH);






}

void loop() {

//Valeurs Potentiometres
  
int potDirectionValue = analogRead(potDirectionPin)* (9.0 / 1023.0); // diminue la sensibilité du potentiomètre et enregistre sa valeur
int potVitesseValue = analogRead(potVitessePin)* (15.0 / 1023.0);     // diminue la sensibilité du potentiomètre et enregistre sa valeur
int potlumieresValue = analogRead(potlumieresPin)* (6.0 / 1023.0);             // diminue la sensibilité du potentiomètre et enregistre sa valeur


// Message Direction

if ( potDirectionValue <=2.0) { //potentiomètre à gauche
  Message2 = 21;

}

if ( potDirectionValue == 3.0 or potDirectionValue == 4.0) { //potentiomètre au centre
  Message2 = 22;
}

if (  potDirectionValue >=5.0) { //potentiomètre à droite
  Message2 = 23;

}
if (Message2 != 0 and Message2 != DernierMessage2) { // Permet d'envoyer un mesage seulement s'il y a un changement d'état du potentiometre.

// On envoie Deux fois pour plus de sureté.
  rd_send_msg(Message2);
  rd_send_msg(Message2);


}

DernierMessage2 = Message2; // on enregistre le dernier message pour détecter un future changement

// Message vitesse et allumage des leds correspondantes. (Si l'état de la led est LOW alors elle s'allume.)


if ( potVitesseValue <=4.0) { // Marche arrière rapide
  Message3 = 31;

  staterouge1 = LOW;
 staterouge2 = LOW;
 staterouge3 = LOW;
 statejaune1 = HIGH;
 statejaune2 = HIGH;
 statevert = HIGH;

}

if ( potVitesseValue <=5.0 and potVitesseValue >4.0) { // Marche arrière
  Message3 = 32;
  
  staterouge1 = HIGH;
 staterouge2 = LOW;
 staterouge3 = LOW;
 statejaune1 = HIGH;
 statejaune2 = HIGH;
 statevert = HIGH;
}

if ( potVitesseValue <=7.0 and potVitesseValue >5.0) { //stop
  Message3 = 33;
  
  staterouge1 = HIGH;
 staterouge2 = HIGH;
 staterouge3 = LOW;
 statejaune1 = HIGH;
 statejaune2 = HIGH;
 statevert = HIGH;

}

if ( potVitesseValue <=8.0 and potVitesseValue >7.0) { // Marche avant
  Message3 = 34;

  staterouge1 = HIGH;
 staterouge2 = HIGH;
 staterouge3 = HIGH;
 statejaune1 = LOW;
 statejaune2 = HIGH;
 statevert = HIGH;

}

if ( potVitesseValue <=9.0 and potVitesseValue >8.0) {// Marche avant rapide
  Message3 = 35;

  staterouge1 = HIGH;
 staterouge2 = HIGH;
 staterouge3 = HIGH;
 statejaune1 = LOW;
 statejaune2 = LOW;
 statevert = HIGH;
}

if ( potVitesseValue >9.0) { // Marche avant maximale
  Message3 = 36;

  staterouge1 = HIGH;
 staterouge2 = HIGH;
 staterouge3 = HIGH;
 statejaune1 = LOW;
 statejaune2 = LOW;
 statevert = LOW;
}
if (Message3 != 0 and Message3 != DernierMessage3) { // Permet d'envoyer un mesage seulement s'il y a un changement d'état du potentiometre.

// On envoie Deux fois pour plus de sureté.
  rd_send_msg(Message3);
  rd_send_msg(Message3);

  


}

DernierMessage3 = Message3;// on enregistre le dernier message pour détecter un future changement


// Allumage des LED en foncion de la vitesse

digitalWrite(pinrouge1, staterouge1);
digitalWrite(pinrouge2, staterouge2);
digitalWrite(pinrouge3, staterouge3);
digitalWrite(pinjaune1, statejaune1);
digitalWrite(pinjaune2, statejaune2);
digitalWrite(pinvert, statevert);





// Message selectionnant les signaux lumineux du ruban de LEDS.
if ( potlumieresValue <=1.0) {
  Message4 = 41;

}

if ( potlumieresValue <=2.0 and potlumieresValue >1.0) {
  Message4 = 42;
}

if ( potlumieresValue <=3.0 and potlumieresValue >2.0) {
  Message4 = 43;

}

if ( potlumieresValue <=4.0 and potlumieresValue >3.0) {
  Message4 = 44;

}

if ( potlumieresValue >4.0) {
  Message4 = 45;
}
if (Message4 != 0 and Message4 != DernierMessage4) { // Permet d'envoyer un mesage seulement s'il y a un changement d'état du potentiometre.

// On envoie Deux fois pour plus de sureté.
  rd_send_msg(Message4);
  rd_send_msg(Message4);



}

DernierMessage4 = Message4;// on enregistre le dernier message pour détecter un future changement


// Lecture de l'état des boutton

buttonState1 = digitalRead(buttonPin1);
buttonState2 = digitalRead(buttonPin2);
buttonState3 = digitalRead(buttonPin3);
buttonState4 = digitalRead(buttonPin4);
buttonState5 = digitalRead(buttonPin5);
buttonState6 = digitalRead(buttonPin6);
buttonState7 = digitalRead(buttonPin7);
buttonState8 = digitalRead(buttonPin8);




//Message boutton (activation/désactivation de la sirène, du ruban, des phares, ou commande des futures modules pouvant être brancher sur la voiture.)

if (buttonState1 == HIGH) { //modules
  Message = 11;

}

if (buttonState2 == HIGH) { //modules
  Message = 12;

}
if (buttonState3 == HIGH) { //modules
  Message = 13;

}
if (buttonState4 == HIGH) { //modules
  Message = 14;

}
if (buttonState5 == HIGH) { //phares
  Message = 15;

}
if (buttonState6 == HIGH) { //sirène
  Message = 16;

}
if (buttonState7 == HIGH) { //ruban
  Message = 17;

}
if (buttonState8 == HIGH) { //modules
  Message = 18;
}



if (Message != 0 and Message != DernierMessage) { 
  
// Le message provenant des bouttons n'est envoyé qu'une fois car le nombre d'envoie de ces messages est un paramètre entrant dans le fonctionnement de la voiture.
  rd_send_msg(Message);

}

DernierMessage = Message; // on enregistre le dernier message pour détecter un future changement
Message = 0;  // on réinitialise Message pour permettre le renvoie du même message sans devoire appuyer sur un autre bouton



// Lorsque qu'on appuie sur un boutton, par exemple le 1, Message prend la valeur 11 avant la comparaison avec DernierMessage, et cela tant que le boutton 1 reste enfoncé.

// Lorqu'on appuis sur le boutton 1, le message 11 est envoyé une fois (car initialement DernierMessage = 0).

// Ensuite DernierMessage prend la valeur 11 et empèche le renvoie du message tant que le boutton 1 reste enfoncé.

// il faut relacher le boutton pour que Message prennent la valeur 0 et rentre cette valeur dans la variable DernierMessage.

// Comme DernierMessage égale à nouveau 0, il est possible de réappuyer sur le boutton 1 pour envoyer une deuxième fois le message 11. 



delay(10);

}
