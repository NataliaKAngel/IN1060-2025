#include <RH_ASK.h>   // RadioHead bibliotek for RF-mottak
#include <SPI.h>      // Kreves av RH_ASK

// Oppretter RF-driver med 2000 bps. Mottak på pin 9, ingen TX (siste verdi = -1)
RH_ASK driver(2000, 9, -1);

const int piezoPin = 3;   // Piezo-høyttaler koblet til digital pin 3

void setup() {
  Serial.begin(9600);               // Åpner seriell monitor for feilsøking
  pinMode(piezoPin, OUTPUT);        // Setter piezo som utgang

  if (!driver.init()) {
    Serial.println("RF-driver initialisering feilet!");  // Hvis RF-modul ikke starter korrekt
  } else {
    Serial.println("RF-driver OK. Klar til å motta signaler.");
  }
}

void loop() {
  uint8_t buf[10];                  // Buffer for innkommende meldinger
  uint8_t buflen = sizeof(buf);     // Lengde på bufferet

  // Sjekker om en melding har blitt mottatt
  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0';             // Avslutter meldingen som en C-streng
    Serial.print("Mottatt: ");      
    Serial.println((char*)buf);     // Skriver ut meldingen

    // Hvis vi mottar "1", skru på piezoen (tone)
    if (strcmp((char*)buf, "1") == 0) {
      tone(piezoPin, 1000);         // Spill 1000 Hz tone
    } 
    // Hvis vi mottar "0", skru den av
    else {
      noTone(piezoPin);             // Stopp lyd
    }
  }
}
