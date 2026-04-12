#include <Arduino.h>
#include "ultrassonico.h"

int t, e;
int TIME_OUT = 30000; //  de 30 ms

void ultra_init(int trig, int echo) {
  t = trig;
  e = echo;

  pinMode(t, OUTPUT);
  pinMode(e, INPUT);
}

float lerDistancia() {
  digitalWrite(t, LOW);
  delayMicroseconds(2);
  digitalWrite(t, HIGH);
  delayMicroseconds(10);
  digitalWrite(t, LOW);

  long duracao = pulseIn(e, HIGH, TIME_OUT);
  if (duracao == 0) return -1; // sem leitura válida

  return duracao * 0.034 / 2.0;
}