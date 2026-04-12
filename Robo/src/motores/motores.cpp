#include <Arduino.h>
#include "motores.h"

int mEA, mER, mDA, mDR;

void motores_init(int eA, int eR, int dA, int dR) {
  mEA = eA;
  mER = eR;
  mDA = dA;
  mDR = dR;

  pinMode(mEA, OUTPUT);
  pinMode(mER, OUTPUT);
  pinMode(mDA, OUTPUT);
  pinMode(mDR, OUTPUT);
}

void pararMotores() {
  digitalWrite(mEA, LOW);
  digitalWrite(mER, LOW);
  digitalWrite(mDA, LOW);
  digitalWrite(mDR, LOW);
}

void andarFrente() {
  digitalWrite(mEA, HIGH);
  digitalWrite(mER, LOW);
  digitalWrite(mDA, HIGH);
  digitalWrite(mDR, LOW);
}

void andarRe() {
  digitalWrite(mEA, LOW);
  digitalWrite(mER, HIGH);
  digitalWrite(mDA, LOW);
  digitalWrite(mDR, HIGH);
}

void curvaEsquerda() {
  digitalWrite(mEA, LOW);
  digitalWrite(mDA, HIGH);
}

void curvaDireita() {
  digitalWrite(mEA, HIGH);
  digitalWrite(mDA, LOW);
}

void virarEsquerda() {
  digitalWrite(mEA, LOW);
  digitalWrite(mER, HIGH);
  digitalWrite(mDA, HIGH);
  digitalWrite(mDR, LOW);
}

void virarDireita() {
  digitalWrite(mEA, HIGH);
  digitalWrite(mER, LOW);
  digitalWrite(mDA, LOW);
  digitalWrite(mDR, HIGH);
}