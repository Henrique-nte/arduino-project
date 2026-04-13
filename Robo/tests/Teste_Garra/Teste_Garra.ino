#include "garra.h"
#include "config/pinos.h"

#include "Arduino.h"

void setup() {
  garra_init(PINO_SERVO);
}

void loop() {
  abrirGarra();
  delay(2000);

  fecharGarra();
  delay(2000);
}