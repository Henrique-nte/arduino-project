#include "../src/motores/motores.h"
#include "../src/sensoresLinha/sensoresLinha.h"
#include "../src/ultrassonico/ultrassonico.h"
#include "../src/garra/garra.h"
#include "../config/pinos.h"


enum Movimento {
  FRENTE,
  ESQ,
  DIR,
  PARADO
};

void setup() {
  Serial.begin(9600);

  motores_init(4,5,6,7); // (int eA, int eR, int dA, int dR)
  sensores_init(2,3,11,12); // (int eExt, int eInt, int dInt, int dExt)
  ultra_init(8,9); // int trig, int echo
  garra_init(10); // int pino
}

Movimento calcular(bool eExt, bool eInt, bool dInt, bool dExt) {
  if (eInt && dInt) return FRENTE;
  if (eExt) return ESQ;
  if (dExt) return DIR;
  return PARADO;
}

void loop() {
  bool eExt, eInt, dInt, dExt;
  lerSensores(eExt, eInt, dInt, dExt); // (bool &eExt, bool &eInt, bool &dInt, bool &dExt)

  Movimento m = calcular(eExt, eInt, dInt, dExt);

  switch (m) {
    case FRENTE: andarFrente(); break;
    case ESQ: curvaEsquerda(); break;
    case DIR: curvaDireita(); break;
    case PARADO: pararMotores(); break;
  }
}