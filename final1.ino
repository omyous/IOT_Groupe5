#include <LiquidCrystal.h>

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd2(12, 11, 5, 4, 3, 2);
float stepADC = 0.0048828125;
// adresse du Buzzer (PIN) 
int BUZZER_PIN = 4;
// adresse du BP1 (bouton poussoir 1) 
int Bp1 = 2;
// variable de l'etat de l'appui sur le BP1
int appui_bp1 = 0;
// variable pour son
float demi_periode = (2.27e-3) * 1000000;
//temps du son en milliseconde
float temps = 500;
//variable de calcul du temps de fonctionnement du buzzer
float duree = 0;
float frequenc = 120;
// adresse du Vibreur (PIN) 
int vibreur = 13;
// seuil de température fixer
int baselineTemp = 40;
//initialisation de variables
int celsius = 0;
String demande = "";
// adresse du capteur de gaz (PIN) 
int detecteur = A1;
// seuil de gaz dans l'air fixer
int niveau_senseur = 250;
// adresse du led rouge (PIN) 
int pin_led_rouge = 7;
bool gaz = false;
// adresse du lcd (PIN) 
const int temperaturePin = A0;

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882812);
}



void setup() {
// Initialisation des composants  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(Bp1, INPUT);
  digitalWrite(Bp1, HIGH);
  pinMode(detecteur, INPUT);
  pinMode(pin_led_rouge, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(vibreur, OUTPUT);
  
 // taille du serial
  Serial.begin(9600);
  
//initialisation de l'écran LCD et affichage du nom de l'application 
  lcd2.begin(20, 2);
  lcd2.setCursor(0,0);
  lcd2.print("test");
  
  
}



void loop() {
  float voltage, degreesC, degreesF;
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  
  lcd2.setCursor(0,0);
  lcd2.print("Temp:  ");
  lcd2.print(degreesC);
  lcd2.print("C");

  
  
 delay(200);
delay(1000);
  
// récupération de la valeur du détécteur
  int valeur_detecteur = analogRead(detecteur);
// Si la présence de gaz est supérieur au seuil fixer (niveau_senceur) on fait clignoter le led rouge et affiche un message sur l'écran LCD
  if (valeur_detecteur >= niveau_senseur) {
    digitalWrite(pin_led_rouge, HIGH);
    lcd2.setCursor(0,1);
    lcd2.print("Presence de gaz!!!   ");
    lcd2.noDisplay();
  	delay(200);
  	lcd2.display();
    gaz = true;
    
  } 
  //sinon éteindre la led rouge et effacer le message sur l'écran LED
  else if (valeur_detecteur < niveau_senseur && gaz) {
    digitalWrite(pin_led_rouge, LOW);
    lcd2.setCursor(0,1);
    lcd2.print("                 ");
    gaz = false;
  }
// Récupération et calcul de la température en C° 
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);

// si la température est inférieure au seuil fixer à 40 le vibreur ne s'active pas sinon il s'active pour calmer le chien 
  if (celsius < baselineTemp) {
    digitalWrite(vibreur, LOW);
  } else {
    digitalWrite(vibreur, HIGH);
  }

 // Récupération de l'état du bouton
  appui_bp1 = digitalRead(Bp1);
// si le bouton est appuyer on allume le buzzer le temps de la 
  if (appui_bp1 == LOW) {
    duree = 0;
    float frequenc;
    while (duree < temps * 1000) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(demi_periode);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(demi_periode);
      duree = duree + 2 * demi_periode;
    }
  }
// commande sur le moniteur série
// vérification qu'une information à été prise en entrée
  if (Serial.available() > 0) {
    
    demande = Serial.readString();
    Serial.print("I received:");
    Serial.println(demande);
 // Action "Temp" affiche la température du chien 

    if (demande == "Temp") {
      lcd2.setCursor(0,1);
      lcd2.print(celsius);
      lcd2.setCursor(2,1);
      lcd2.print("C.           ");
    } 
// Action "Loc" envoie la localisation du chien 

    else if (demande == "Loc") {
      lcd2.setCursor(0,1);
      lcd2.print("Maison            ");
    } 
// Action "Calm" Active le vibreur pour calmer le chien 
    
    else if (demande == "Calm") {
      lcd2.setCursor(0,1);
      digitalWrite(vibreur, HIGH);
      delayMicroseconds(demi_periode);
      digitalWrite(vibreur, LOW);
      lcd2.print("Nous calmons votre chien");
      
    }
  }
}