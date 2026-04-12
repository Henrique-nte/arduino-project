#include <Arduino.h>
#include <Servo.h>
#include "garra.h"

//Sujeito a mudanças depois dos testes
const ANGULO_GARRA_ABERTA = 0;
const ANGULO_GARRA_FECHADA = 90;

Servo servo;

void garra_init(int pino) {
  servo.attach(pino);
  servo.write(0);
}

void abrirGarra() {
  servo.write(ANGULO_GARRA_ABERTA);
}

void fecharGarra() {
  servo.write(ANGULO_GARRA_FECHADA);
}