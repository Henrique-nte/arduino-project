#include "ultrassonico.h"
#include "config/pinos.h"

#include "Arduino.h"

void setup() {
  Serial.begin(9600);

  ultra_init(PINO_TRIG, PINO_ECHO);

  Serial.println("==================================");
  Serial.println(" TESTE DO ULTRASSONICO");
  Serial.println("==================================");
  Serial.println("Aproxime e afaste objetos do sensor.");
  Serial.println("");
}

void loop() {
  float distancia = lerDistancia();

  if (distancia > 0) {
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");
  } else {
    Serial.println("Falha na leitura");
  }

  delay(300);
}