#include "ultrassonico.h"
#include "config/pinos.h"

#include "Arduino.h"

void setup() {
  ultra_init(PINO_TRIG, PINO_ECHO);
}

void loop() {
  float distancia = lerDistancia();
  delay(300);
}