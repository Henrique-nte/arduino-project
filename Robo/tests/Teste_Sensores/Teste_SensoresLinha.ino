#include "sensoresLinha.h"
#include "config/pinos.h"

#include "Arduino.h"

void setup() {
  sensores_init(
    PINO_ESQ_EXT,
    PINO_ESQ_INT,
    PINO_DIR_INT,
    PINO_DIR_EXT
  );
}

void loop() {
  bool eExt, eInt, dInt, dExt;
  lerSensores(eExt, eInt, dInt, dExt);

  delay(200);
}