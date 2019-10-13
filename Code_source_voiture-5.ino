#include <VirtualWire.h> // On inclus la librairie VirtualWire
//#include <VirtualWire_Config.h>

#include <Adafruit_NeoPixel.h>        //Librairie pour controler les rubans de led


// variable stockant le dernier message radio reçu
int message;


// variable modifiées par les messages envoyés par radio
int vitesse = 33;  // vitesse des moteurs
int volant = 22;   // direction de la voiture
int lumieres = 45;  // Sélection des signaux lumineux du ruban de LEDS

int boutton1 =0; 
int boutton2 =0;
int boutton3 =0;
int boutton4 =0;
int boutton5 =0; // sert à la commande des phares 
int boutton6 =0; // état de la sirène de police   ( 0 = éteint; 1 = allumé)
int boutton7 =0; // état du ruban de leds         ( 0 = éteint; 1 = allumé)
int boutton8 =0;



//pin moteurs
int motorPin1 = 22; //avant moteur 1
int motorPin2 = 24; //arrière moteur 1
int enablePin1 = 3; //vitesse moteur 1  (PWM)
int motorPin3 = 28; //avant moteur 2
int motorPin4 = 26; //arrière moteur 2
int enablePin2 = 4; //vitesse moteur 2  (PWM)



// variable d'état des phares
int allume = 0;  // ( 0 = éteint; 1 = allumé)


//Pin des phares
const int ledPin1 = 7;
const int ledPin2 = 8;
const int ledPin3 = 48;
const int ledPin4 = 44;



// pin buzzer
const int speakerPin = 10;

unsigned long previousMillis = millis();       // sera utilisé pour enregistrer le temps passé entre le démarage du programme et la dernière note jouée par le Buzzer.

//variable intervenant dans le fonctionnement non bloquant du buzzer
int note = 0;
int der_note = 0;


//"unsigned int" pour avoir que des valeurs positives et ainsi avoir + de marge de valeur
unsigned int tension;   //tension en A1 ou A2 ou A3 ou A4 selon la fonction détection_mudule appelée dans le void loop


int brancher1 = 0; //variable d'état du pin A1 (= 0 si la prise est libre, = 1 si un module est branché sur la prise)
int brancher2 = 0; //variable d'état du pin A2
int brancher3 = 0; //variable d'état du pin A3
int brancher4 = 0; //variable d'état du pin A4



/*---------------------------------------------------------------- Définition du ruban ---------------------------------------------------------------------------*/
#define PIXEL_PIN 6    //Pin où est branché le ruban (fil du milieu) ici 6
#define PIXEL_COUNT 15   //Définit le nombre de led sur le ruban (15 ici)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);   //Paramètre l'objet strip qui correspond à toutes les leds du ruban
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------- Définition des couleurs ----------------------------------------------------------------------*/
#define NB_COLOR 8
int RED[3] = {255, 0, 0};    //Couleur Rouge
int GREEN[3] = {0, 255, 0};    //Couleur Verte
int CYAN[3] = {0, 255, 255};    //Couleur Cyan
int YELLOW[3] = {255, 125, 0};    //Couleur Jaune
int ORANGE[3] = {255, 40, 0};    //Couleur Orange
int PURPLE[3] = {255, 0 , 255};    //Couleur Violette
int ROSE[3] = {255, 0, 100};    //Couleur Rose
int BLUE[3] = {0, 0, 255};    //Couleur Bleu
int WHITE[3] = {255, 255, 255};    //Couleur Blanche
int OFF[3] = {0, 0, 0};    //Éteint
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned long previousMillis2 = millis();        // sera utilisé pour enregistrer le temps passé entre le démarage du programme et les derniers signaux lumineux du ruban de LEDS

//variable intervenant dans le fonctionnement non bloquant du ruban
int Led = 0;
int der_Led = 0;







 
void setup()
{
  // déclaration sortie pin moteurs
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(enablePin1, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    pinMode(enablePin2, OUTPUT);

    
 // mise en fonctionnment du récepteur radio
    
    vw_set_rx_pin(2); // déclaration du pin 2 comme pin de reception radio
    vw_setup(2000); // initialisation de la librairie VirtualWire à 2000 bauds
    vw_rx_start();  // Activation de la partie réception de la librairie VirtualWire




// Déclaration pin phares en sortie
     pinMode(ledPin1, OUTPUT);      
     pinMode(ledPin2, OUTPUT);
     pinMode(ledPin3, OUTPUT);
     pinMode(ledPin4, OUTPUT);


// Déclaration pin buzzer en sortie
     pinMode(speakerPin, OUTPUT); 



     // initialisation ruban led
     strip.begin();   //Lance la connection
  strip.show();    //Initialise toutes les leds à "off"
  strip.setBrightness(100); // Determine la luminosité des leds



// Allumage des phares pendant une second lorsque la voiture est mise en marche
allume = phares(allume);
delay(1000);
allume = phares(allume);




  
}





 
void loop()
{



//Réception du message radio

  
    uint8_t msgRecu[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
    uint8_t longueurMsg = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
 
    if (vw_wait_rx_max(200)) // Si un message est reçu dans les 200ms qui viennent
    {
        if (vw_get_message(msgRecu, &longueurMsg)) // On copie le message, qu'il soit corrompu ou non
        {


                                                                                                      


            
            for (byte i = 0; i < longueurMsg; i++) // Si il n'est pas corrompu on l'enregistre
                                                                                                          
                message = (msgRecu[0] - '0')*10 +(msgRecu[1] - '0'); // on passe le message qui est une chaîne de caractères en entier entre 11 et 45.
                                                                                                       
                                                                                                      


            
            if (message >= 41 and message <= 45){ // Les codes entre 41 et 45 sont assignés à la sélection des signaux lumineux du ruban de LEDS
              lumieres = message;
            }
            if (message >= 31 and message <= 36){ // Les codes entre 31 et 36 sont assignés à la vitesse des moteurs
              vitesse = message;
            }
            if (message >= 21 and message <= 23){ // Les codes entre 21 et 23 sont assignés à la direction de la voiture
              volant = message;
            }

// Les codes entre 11 et 14 seront utilisées pour faire tourner les servomoteur des future modules


            // fait tourner le servomoteur 1 à gauche
            if (message == 11){
              if (boutton1 == 0) {
                boutton1 = 1;
              }
              else {
                boutton1 = 0;
              }
            }

            // fait tourner le servomoteur 2 à gauche
            if (message == 12){
              if (boutton2 == 0) {
                boutton2 = 1;
              }
              else {
                boutton2 = 0;
            }
            }

            // fait tourner le servomoteur 2 à droite
            if (message == 13){
              if (boutton3 == 0) {
                boutton3 = 1;
              }
              else {
                boutton3 = 0;
              }
            }

            // fait tourner le servomoteur 1 à droite
            if (message == 14){
              if (boutton4 == 0) {
                boutton4 = 1;
              }
              else {
                boutton4 = 0;
            }

            }
            
            if (message == 15){ // Le code 15 est assigné à l'allumage et l'éteignage des phares
                
                allume = phares(allume);
                
                

                
            }
            }
            
            if (message == 16){ // Le code 16 est assigné à l'allumage et l'éteignage de la sirène de police
              if (boutton6 == 0) {
                boutton6 = 1;
              }
              else  {
                boutton6 = 0;
            }
       
            }


             




              
            if (message == 17){ // Le code 17 est assigné à l'allumage et l'éteignage du ruban de LEDS
              if (boutton7 == 0) {
                boutton7 = 1;
              }
              else {
                boutton7 = 0;
            }
            }

            

            
            
            if (message == 18){ // permet d'allumer ou d'éteindre la LED du future module avec projecteur.
              if (boutton8 == 0) {
                boutton8 = 1;
              }
              else {
                boutton8 = 0;
            }
            }
            
            message = 0; // Réinitialise le message radio en attendant un nouveau message


            
        }    
    
             if (boutton6 == 1){ //active la sirène si le bouton 6 est pressé une fois puis la désactive s'il est à nouveau pressé.
              
              der_note = sirene (previousMillis, note); // on enregistre la denière note jouer
              
              if (not (der_note == note)){ // si der_note est différent de note alors cela signifie que l'on vient de jouer une note et donc que previousMillis doit être mis à jour.
                previousMillis = millis();
                note = der_note;           // on enregistre la denière note jouer dans note pour les comparaisons suivantes
              }
              
              
             }

             if (boutton7 == 1){ //active le ruban de LEDS si le bouton 7 est pressé.
              der_Led = Ruban_LEDS ( lumieres,previousMillis2, Led );
              
              
              if (not (der_Led == Led)){ // si der_Led est différent de Led, alors cela signifie que l'on vient d'émettre un signal lumineux (ou une partie)  avec le ruban. Il faut donc mettre à jour previousMillis2.
                previousMillis2 = millis();
                Led = der_Led;           // on enregistre le dernier signal émit dans Led pour les comparaisons suivantes
              }

              
             }
             
             else {
                  allOff(); // Permet de désactiver le ruban si le bouton 7 est à nouveau pressé
             }



    moteurs(vitesse,  volant); // appelle la fonction commandant les moteurs selon la vitesse et la direction transmise par radio
    

    // détection des modules
      brancher1 =  detection_modules (A1, brancher1); // détecte la connection ou la déconnection d'un module sur le Pin A1 et l'enregistre dans branché1 (branché1 correspond à l'état du pin A1)
      brancher2 =  detection_modules (A2, brancher2); // détecte la connection ou la déconnection d'un module sur le Pin A2 et l'enregistre dans branché1 (branché2 correspond à l'état du pin A2)
      brancher3 =  detection_modules (A3, brancher3); // détecte la connection ou la déconnection d'un module sur le Pin A3 et l'enregistre dans branché1 (branché3 correspond à l'état du pin A3)
      brancher4 =  detection_modules (A4, brancher4); // détecte la connection ou la déconnection d'un module sur le Pin A4 et l'enregistre dans branché1 (branché4 correspond à l'état du pin A4)

    delay(10);
      
}











/*-------------------------------------------------------------------- Fonctions ----------------------------------------------------------------------*/







/*-------------------------------------------------------------------- fonction controllant les moteurs----------------------------------------------------------------------*/

void moteurs (int vitesse, int volant){
  if (vitesse >= 34 ) // avant
      {

        // Marche avant pour le moteur gauche
        digitalWrite(motorPin1, HIGH); 
        digitalWrite(motorPin2, LOW);

        // Marche avant pour le moteur droite
        digitalWrite(motorPin3, HIGH); 
        digitalWrite(motorPin4, LOW);

        

      }
      else if (vitesse <= 32) // arrière
      {

        // Marche arrière pour le moteur gauche
        digitalWrite(motorPin1, LOW); 
        digitalWrite(motorPin2, HIGH);

        // Marche arrière pour le moteur droite
        digitalWrite(motorPin3, LOW); 
        digitalWrite(motorPin4, HIGH);

        
      }
      
      else // Stop (freinage)
      {

        // freinage du le moteur gauche
        digitalWrite(motorPin1, HIGH); 
        digitalWrite(motorPin2, HIGH);

        // freinage du le moteur droite
        digitalWrite(motorPin3, HIGH); 
        digitalWrite(motorPin4, HIGH);
     
      }
 
      //
      // Vitesse du mouvement
      //
      int allure1; //Vitesse du moteur gauche
      int allure2; //Vitesse du moteur droite

      //Les deux moteurs on des vitesses légèrements différentes pour compenser leur différence de comportement. (Permet de corriger la trajectiore de la voiture)

      if (vitesse == 31){
        allure1 = 160;
        allure2 = 140;
      }
      
      if (vitesse == 32 ){
        allure1 = 75;
        allure2 = 75;
}

      if (vitesse == 33){
        allure1 = 0;
        allure2 = 0;
      }
      if ( vitesse == 34){
        allure1 = 65;
        allure2 = 75;
      }
      if (vitesse == 35){
        allure1 = 160;
        allure2 = 140;
      }
      if (vitesse == 36){
        allure1 = 190;
        allure2 = 180;
        
      }


      //direction
      if (volant == 21){ //gauche
        analogWrite(enablePin1, allure1*0.5); // on diminue la vitesse du moteur gauche
        analogWrite(enablePin2, allure2);

      }
      
      if (volant == 23){ //droite
        analogWrite(enablePin1, allure1);
        analogWrite(enablePin2, allure2*0.5);// on diminue la vitesse du moteur droite
      }

      if (volant == 22) { //tout droit
        analogWrite(enablePin1, allure1);
        analogWrite(enablePin2, allure2);
      }

      

}




/*-------------------------------------------------------------------- Fonctions commandant les phares ----------------------------------------------------------------------*/


int phares (int allume) {

  // si les phares sont éteints alors ils s'allument
  if ( allume == 0){
    digitalWrite(ledPin1,HIGH);
      digitalWrite(ledPin2,HIGH);
      digitalWrite(ledPin3,HIGH);
      digitalWrite(ledPin4,HIGH);
      allume = 1;
      
      
      
      
   // si les phares sont allumer alors ils s'éteignent
  }
  else {
    digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPin3,LOW);
      digitalWrite(ledPin4,LOW);
      allume = 0;
      
      
      
  }
  return allume; // renvoie l'état des phares
  
}


/*-------------------------------------------------------------------- Fonctions fasant jouer une sirène de police au Buzzer sans bloquer le programme----------------------------------------------------------------------*/

int sirene (unsigned long previousMillis,int note){

// previousMillis est le temps passé entre le démarage du programme et la dernière note jouée


  unsigned long currentMillis = millis(); // on enregistre le temps passé depuis l'activation du programme
  int Temps_notes = 600;// le temps d'une note doit être de 600 millisecondes

// (currentMillis - previousMillis) correspond au temps passé depuis la dernière note joué


// si le temps depuis la dernière note est supérieur à 600 alors on joue la note suivante
  if (currentMillis - previousMillis > Temps_notes) { 
    
  
//  Si la dernière note était A alors on joue D
  if (note == 0) {
    play('D', 4);
    
    note = 1;
    
  }
  
//  Si la dernière note était D alors on joue A  
  else{
    
    play('A', 4);
    note = 0;
    
  
  }
  
  }
  return note; // on renvoie la dernière note jouer avec 0 ou 1



}


/*-------------------------------------------------------------------- Fonctions commandant le Buzzer ----------------------------------------------------------------------*/

void play(char note, int beats)  {
  int numNotes = 14;  // il y a 15 notes possible (si on compte le silence)
  
  

  // Permet le lien entre note et fréquence
  char notes[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', ' '};
  int frequencies[] = {131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 0};

  
  
  int currentFrequency = 0;    //variable enregistrant la note jouée
  int beatLength = 150;   //tempo

  //regarde toutes les notes enregistrée
  for (int i = 0; i < numNotes; i++)  
  {
    if (notes[i] == note)             //associe la fréquence à la note demandée.
    {
      currentFrequency = frequencies[i];   
    }
  }

  //joue la note à la fréquence "currentFrequency" pendant la durée "beats * beatLength" par le pin "speakerPin".
  tone(speakerPin, currentFrequency, beats * beatLength);
                    

}




/*-------------------------------------------------------------------- Fonctions détectant le branchement d'un module sur la voiture ----------------------------------------------------------------------*/


int detection_modules (char pin, int brancher){

  //regarde la tension au pin demandé
  tension = analogRead(pin);
  
  
  
  
// si la voiture capte un module alors elle émet un cours signal sonore pour le signaler.
  if (tension >= 290 && tension <= 300)  {   //Reconnaît le PROJECTEUR
    if (brancher == 0)  {
      play('C', 1);
      delay(50);
      play('G', 4);
      delay(50);
      
      brancher = 1;
    }
  }
  if (tension >= 810 && tension <= 820)  {   //Reconnaît la GRUE
    if (brancher == 0)  {
      play('c', 8);
      delay(50);
      
      brancher = 1;
    }
  }
  if (tension >= 720 && tension <= 730)  {   //Reconnaît le COCKPIT
    
    if (brancher == 0)  {
      play('b', 6);
      delay(50);
      play('b', 6);
      delay(50);
      play('b', 6);
      
      delay(100);
      play('B', 7);
      delay(50);
      
      brancher = 1;
      
    }
  }
  
  if (     not(tension >= 290 && tension <= 300) and not(tension >= 810 && tension <= 820) and not(tension >= 720 && tension <= 730)   )  { // si la voiture ne détecte pas une tension référencée pour un module, alors cela signifie qu'il n'y a pas de module branché sur le pin.
    brancher = 0;
  }
  
  return brancher; // renvoie 1 s'il y a un module de branché au pin, 0 sinon.

  

}





/*-------------------------------------------------  Fonctions pour le ruban de leds:-----------------------------------------------------*/


/*------------------------------------------------- Fonction allumant TOUTES les leds de la couleur choisie -----------------------------------------------------*/
void allLeds(int COLOR[])  {
  for (int i = 0 ; i < PIXEL_COUNT ; i++)  {
    strip.setPixelColor(i, COLOR[0], COLOR[1], COLOR[2]);
  }
  strip.show();
  //  delay(2000);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------- Fonction chenillard ----------------------------------------------------------------------*/
void chenillard(int temps, int color[3])  {
  for(int i = 0 ; i < 15 ; i++)  {
    strip.setPixelColor(i, color[0], color[1], color[2]);
    strip.show();
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    delay(temps);
  }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void random_chenillard(int temps)  {
  int color1 = random(0, 255);
  int color2 = random(0, 255);
  int color3 = random(0, 255);
  
  for(int i = 0 ; i < 15 ; i++)  {
    strip.setPixelColor(i, color1, color2, color3);
    strip.show();
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    delay(temps);
  }
}




/*------------------------------------------------- Fonction allumant chacunes  des leds d'une couleur différente -----------------------------------------------*/
void diffLeds()  {
  for (int i = 0 ; i < PIXEL_COUNT ; i++)  {
    strip.setPixelColor(i, random(0, 255), random(0, 255), random(0, 255)); //j+2 et j+3 mis au hasard --> donner un coté aléatoire
  }
  strip.show();
  //  delay(1000);   //valeur du delay() à revoir grâce aux tests qu'on fera
}
/*---------------------------------------------------Fonction allumant toutes les leds en blanc mais avec un luminosité aléatoire------------------------------------------------------------------------------------------------------------*/

void allrandomLeds()  {
  int color = random(0, 255);
  for (int i = 0 ; i < PIXEL_COUNT ; i++)  {
    
    strip.setPixelColor(i, color, color, color); //j+2 et j+3 mis au hasard --> donner un coté aléatoire
  }
  strip.show();
  //  delay(1000);   //valeur du delay() à revoir grâce aux tests qu'on fera
}


/*-------------------------------------------- Fonction allumant toutes les leds progressivement avec des couleurs différentes ----------------------------------*/
void diff_lumi_progressive(int temps)  {
  for(int lumi = 0 ; lumi < 150 ; lumi++)  {
     for(int i = 0 ; i < 15 ; i++)  {
       strip.setPixelColor(i, random(0, 255), random(0, 255), random(0, 255));   //COLOR[0], COLOR[1], COLOR[2]);
    }
    strip.setBrightness(lumi); 
    strip.show();
    delay(temps);
  }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------- Fonction allumant toutes les leds progressivement avec des couleurs différentes ----------------------------------*/
void all_lumi_progressive(int temps, int COLOR[])  {   
  for(int lumi = 0 ; lumi < 150 ; lumi++)  {
     for(int i = 0 ; i < 15 ; i++)  {
       strip.setPixelColor(i, COLOR[0], COLOR[1], COLOR[2]);
    }
    strip.setBrightness(lumi); 
    strip.show();
    delay(temps);
  }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------- Fonction qui allume le ruban moitié couleur, moitié de l'autre --------------------------------------------------*/
void allBicolor(int COLOR1[], int COLOR2[])  {
  for (int i = 0 ; i < PIXEL_COUNT / 2 ; i++)  {
    strip.setPixelColor(i, COLOR1[0], COLOR1[1], COLOR1[2]);
  }

  for (int i = PIXEL_COUNT / 2 ; i < PIXEL_COUNT ; i++)  {
    strip.setPixelColor(i, COLOR2[0], COLOR2[1], COLOR2[2]);
  }
  strip.show();
  //  delay(1000);
}
/*-------------------------------------------------Fonction qui allume les leds paires d'une couleur et les impaires d'une autre couleur --------------------------------------------------------------------------------------------------------------*/
void allBicolor2(int COLOR1[], int COLOR2[])  {
  for (int i = 0 ; i < PIXEL_COUNT / 2 ; i++)  {
    int j = i*2;
    strip.setPixelColor(j, COLOR1[0], COLOR1[1], COLOR1[2]);
  }

  for (int i = 0 ; i < PIXEL_COUNT / 2 ; i++)  {
    int k = (i*2)+1;
    strip.setPixelColor(k, COLOR2[0], COLOR2[1], COLOR2[2]);
  }
  strip.show();
  //  delay(1000);
}


/*--------------------------------------------------------------- Fonction éteignant toutes les leds ------------------------------------------------------------*/
void allOff() {
  strip.clear();
  strip.show();
}
/*---------------------------------------------------------------- Fonction permettant lechoix des signaux lumineux du ruban de LEDS-----------------------------------------------------------------------------------------------*/

int Ruban_LEDS ( int lumieres,unsigned long previousMillis2, int Led ){


//lumieres correspond au choix du signal lumineux sélectionner sur la télécommande (code entre 41 et 45))
  
  unsigned long currentMillis2 = millis();

// previousMillis2 est le temps passé entre le démarage du programme et le dernier sigal du ruban

// (currentMillis2 - previousMillis2) correspond au temps passé depuis le dernier sigal du ruban

  
  if (lumieres == 41)  { //Schéma : moitié rouge, moitié orange

    if (currentMillis2 - previousMillis2 > 600) { //Le signal change toute les 600 millisecondes.
      if (Led == 0){
        allBicolor(RED, ORANGE); 
        Led = 1;
        
      
      }
      else{
        
        allBicolor( ORANGE, RED);
        Led = 0;
      }
    

      
    }
   

    }
    

    if (lumieres == 42)  {//Schéma : bicolor bleu et blanc  ==> Police

      if (currentMillis2 - previousMillis2 > 600) { //Le signal change toute les 600 millisecondes.
      if (Led == 0){
        allBicolor2(BLUE, WHITE); 
        Led = 1;
       
      
      }
      else{
        
        allBicolor2( WHITE, BLUE);
        Led = 0;
      }
    

    
    }

    }
    

    if (lumieres == 43)  {   //Schéma : Blanc de luminosité aléatoire

      if (currentMillis2 - previousMillis2 > 600) { //Le signal change toute les 600 millisecondes.
      if (Led == 0){
        allrandomLeds();  //1
        Led = 1;
      
      }
      else{
        allrandomLeds();
        Led = 0;
      }
    

      
    }

    
      
    }

    if (lumieres == 44)  {    //Schéma : couleurs différentes et aléatoires
      if (currentMillis2 - previousMillis2 > 600) { //Le signal change toute les 600 millisecondes.
      if (Led == 0){
        diffLeds();  //1
        Led = 1;
      
      }
      else{
        diffLeds();
        Led = 0;
      }
    }
      
      
    }

// le code 45 permet de garder constant le dernier signal émit avec le code 44
    
return Led;

}
