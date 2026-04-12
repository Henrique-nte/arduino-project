#include <Arduino.h>
#include "ultrassonico.h"

int t, e;

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

  long duracao = pulseIn(e, HIGH, 30000);
  if (duracao == 0) return -1;

  return duracao * 0.034 / 2.0;
}