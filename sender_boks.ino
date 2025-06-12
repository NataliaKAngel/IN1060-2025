#include <RH_ASK.h>      // RadioHead-bibliotek for ASK (Amplitude Shift Keying) over RF
#include <SPI.h>         // Kreves av RadioHead, selv om SPI ikke brukes direkte

const int ledPin = 2;        // LED som viser at bryteren er aktiv
const int bryterPin = 3;     // Bryter som styrer om signal skal sendes

// Oppretter RF-sender med 2000 bps, RX pin = 5 (ubrukt), TX pin = 4
RH_ASK rf_driver(2000, 5, 4);

void setup() {
  pinMode(ledPin, OUTPUT);                         // Setter LED som utgang
  pinMode(bryterPin, INPUT_PULLUP);                // Bruker intern pullup: pinnen er HIGH når bryteren er av

  Serial.begin(9600);                              // Starter seriell kommunikasjon for feilsøking
  if (!rf_driver.init()) {
    Serial.println("RF-sender initiering feilet!");  // Skriv feilmelding hvis RF ikke starter
  } else {
    Serial.println("RF-sender OK. Klar til å sende.");
  }
}

void loop() {
  // Sjekk om bryteren er på (dvs. koblet til GND -> LOW)
  if (digitalRead(bryterPin) == LOW) {
    digitalWrite(ledPin, HIGH);     // Slå på LED for visuell bekreftelse
    const char *melding = "1";      // RF-melding som indikerer "på"
    rf_driver.send((uint8_t *)melding, strlen(melding));  // Send meldingen
    rf_driver.waitPacketSent();     // Vent til meldingen er ferdig sendt
    Serial.println("Sendt: 1");     // Logg
  } else {
    digitalWrite(ledPin, LOW);      // Slå av LED
    const char *melding = "0";      // RF-melding som indikerer "av"
    rf_driver.send((uint8_t *)melding, strlen(melding));
    rf_driver.waitPacketSent();
    Serial.println("Sendt: 0");
  }

  delay(200);  // Liten forsinkelse for å unngå overflødig spamming av signal
}
