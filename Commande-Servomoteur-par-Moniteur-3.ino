
// Ce programme permet d'activé la rotation d'un servomoteur à rotation continue grace au moniteur. 

// En remplacant la commande par moniteur par une comande radio, il devient possible de faire tourner le servo moteur vers la gauche ou vers la droite selon sur quel bouton de la télécommande on appuie.

 
 #include  <Servo.h>

 Servo monServo;

 char PWM_SERVO = 9; // pin de commande du servomoteur.

//initialisation de la vitesse du servomoteur.
 int vitesseDuServo = 95;

// variable enregistrant le code donné par le moniteur. 
int state = 0;


 void setup ()
{//initialisation du pin et de la vitesse du moteur.
   monServo.attach(PWM_SERVO);
   monServo.write(vitesseDuServo);

   //initialisation du moniteur
   Serial.begin(9600);
}
void loop () 
{

if (Serial.available() > 0) // si quelque chose est entrée dans le moniteur. (à remplacer par l'arriver d'un message radio.)
    {
      // Lecture de l'entier passé au port série
      state = Serial.parseInt();  //( à remplacer par le message de la radio)
      if (state ==1){             // si le code entrée est 1 alors le servomoteur tourne brièvement dans un sens.
        vitesseDuServo=180;
        monServo.write(vitesseDuServo);
        delay(100);
        
      }
      if (state ==2){             // si le code entrée est 2 alors le servomoteur tourne brièvement dans l'autre un sens.
        vitesseDuServo=-180;
        monServo.write(vitesseDuServo);
        delay(100);
        

      }
  monServo.write(95);             // stop le servomoteur
  state = 0;                      // réinitialise le message
  }

}
