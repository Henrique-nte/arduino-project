#include <Arduino.h>
#include "sensoresLinha.h"

int sEE, sEI, sDI, sDE;

void sensores_init(int eExt, int eInt, int dInt, int dExt) {
  sEE = eExt;
  sEI = eInt;
  sDI = dInt;
  sDE = dExt;

  pinMode(sEE, INPUT_PULLUP);
  pinMode(sEI, INPUT_PULLUP);
  pinMode(sDI, INPUT_PULLUP);
  pinMode(sDE, INPUT_PULLUP);
}

bool ler(int pino) {
  return digitalRead(pino) == LOW;
}

void lerSensores(bool &eExt, bool &eInt, bool &dInt, bool &dExt) {
  eExt = ler(sEE);
  eInt = ler(sEI);
  dInt = ler(sDI);
  dExt = ler(sDE);
}