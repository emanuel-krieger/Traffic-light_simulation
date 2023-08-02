#include <string.h>
#include <stdint.h>
#include <stdlib.h>

using namespace std;
//Zeit bis zur tastergesteuerten Schaltung auf grün
unsigned int zeitHauptmin = 5000;  
//Zeit bis zur automatischen Umschaltung der Hauptstraße auf rot
unsigned int zeitHauptmax = 12000;  
//Zeit bis zur tastergesteuerten Umschaltung der Nebenstraße auf rot
unsigned int zeitNebenmin = 3000;
//Zeit bis zur automatischen Umschaltung der Nebenstraße auf rot
unsigned int zeitNebenmax = 8000;
//zeit Umschalten von gelb zu Rot Hauptstraße
int milli_laengereschaltung = 1500;
//zeit Umschalten von gelb zu Rot Nebenstraße
int milli_normale_schaltung = 1000;
//frühzeitig schalten, verzögerung
int schaltverzoegerung = 500;
//Zeit zwischen den Farben: rot und gelb 1 Sekunde, dann grün
int milli_aus = 40;
//Zeit in der alle Lampen aus sind, zwischen der Schaltung
int A_LED[6] = {A0, A1, A2, A3, A4, A5};
int index = 0;
//Array für die Steuerung der Lampen und ein int für den index(1-5) dafür
long startzeit = 0; //zur Zeitzmessung
long laufzeit = 0;  //zur ausgabe / Debug
long durchlaeufe = 0; //durchläufe zählen bis zur Schaltung
//long durchlaeufe2 = 0; //durchläufe zählen bis zur Schaltung
bool serialfordebug = 0;
bool taster_12 = 0; //Simulation des Autos, das an der Ampel steht
bool taster_13 = 0; //Simulation des Autos, das an der Ampel steht
bool Haupt_rot = 0; //Zustand der Ampel
bool Neben_rot = 0; //Zustand der Ampel
bool ausgeben = false;  //zweifache ausgabe vermeiden

void setup() {
Serial.begin (9600);
  for (int i = 0; i <= 5; i++) {
    pinMode (A_LED[i], OUTPUT);
    //LEDs als OUTPUT definieren
  }
  pinMode (13, OUTPUT);
  pinMode (12, OUTPUT);
  //pins als output definieren, um als Taster zu verwenden
  digitalWrite(A0, HIGH);
  digitalWrite(A3, HIGH);
  delay(schaltverzoegerung);
  Serial.println("setup");
  Neben_gruenschalten();
}

void loop() {
  durchlaeufe++;
  /*
  if (durchlaeufe >= 32,766)
    durchlaeufe = 0;
    */
  laufzeit = millis();
  if (digitalRead(12) == HIGH){
    taster_12 = true;
    ausgeben = true;
  }
  if (taster_12 && ausgeben){
    Serial.println("taster_12");
    ausgeben = false;
  }
  if (digitalRead(13) == HIGH){
    taster_13 = true;
    ausgeben = true;
  }
  if (taster_13 && ausgeben){
    Serial.println("taster_13");
    ausgeben = false;
  }
  schaltlogik();
  serialfordebug = false;
}

void schaltlogik(){
  bool schonwahr = false;
  if ((millis() - startzeit) >= zeitHauptmin && Haupt_rot == false && taster_12) {
    Serial.println("A");
    debug();
    delay(schaltverzoegerung);
    Haupt_rotschalten();
    schonwahr = true;
    ausgeben = true;
    durchlaeufe = 0;
  }
  else {
    if ((millis() - startzeit) >= zeitHauptmax && Haupt_rot == false && !schonwahr){
      Serial.println("B");
      debug();
      Haupt_rotschalten();
      schonwahr = true;
      ausgeben = true;
      durchlaeufe = 0;
    }
    else {
      if ((millis() - startzeit) >= zeitNebenmin && Neben_rot == false && taster_13 && !schonwahr) {
        Serial.println("C");
        debug();
        delay(schaltverzoegerung);
        Neben_rotschalten();
        ausgeben = true;
        schonwahr = true;
        durchlaeufe = 0;
      }
      else {
        
        if ((millis() - startzeit) >= zeitNebenmax && Neben_rot == false && !schonwahr) {
          Serial.println("D");
          debug();
          Neben_rotschalten();
          schonwahr = true;
          ausgeben = true;
          durchlaeufe = 0;
        }
      }
    }
  }
}

void debug(){
  //if ((laufzeit - startzeit) >=5){
  Serial.print("laufzeit: ");
    Serial.println(laufzeit);
  Serial.print("startzeit: ");
    Serial.println(startzeit);
  //}
  Serial.print("durchlaeufe: ");
  Serial.println(durchlaeufe/* + 32,766 * durchlaeufe2*/);
}

void Haupt_rotschalten(){
  //  auf rot schalten
  digitalWrite (A5, LOW);
  delay(milli_aus);
  digitalWrite (A4, HIGH);
  delay(milli_laengereschaltung);
  digitalWrite (A4, LOW);
  delay(milli_aus);
  digitalWrite (A3, HIGH);
  Haupt_rot = true;
  delay(schaltverzoegerung);
  Neben_gruenschalten();
}

void Neben_rotschalten(){
  //  auf rot schalten
  digitalWrite (A2, LOW);
  delay(milli_aus);
  digitalWrite (A1, HIGH);
  delay(milli_normale_schaltung);
  digitalWrite (A1, LOW);
  delay(milli_aus);
  digitalWrite (A0, HIGH);
  Neben_rot = true;
  delay(schaltverzoegerung);
  Haupt_gruenschalten();
}

void Neben_gruenschalten() {
  //  auf grün schalten
  Serial.print("1");
  digitalWrite (A1, HIGH);
  delay(milli_normale_schaltung);
  Serial.print("2");
  digitalWrite (A0, LOW);
  digitalWrite (A1, LOW);
  delay(milli_aus);
  Serial.println("3");
  digitalWrite (A2, HIGH);
  Neben_rot = false;
  taster_12 = false;
  startzeit = millis();
}

void Haupt_gruenschalten() {
  //  auf grün schalten
  Serial.print("1");
  digitalWrite (A3, HIGH);
  digitalWrite (A4, HIGH);
  delay(milli_normale_schaltung);
  Serial.print("2");
  digitalWrite (A3, LOW);
  digitalWrite (A4, LOW);
  delay(milli_aus);
  Serial.println("3");
  digitalWrite (A5, HIGH);
  Haupt_rot = false;
  taster_13 = false;
  startzeit = millis();
}