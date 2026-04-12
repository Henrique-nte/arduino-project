#include "motores.h"
#include "config/pinos.h"

#include "Arduino.h"

void aguardarComMensagem(const char *msg, unsigned long tempoMs) {
  Serial.println(msg);
  delay(tempoMs);
}

void setup() {
  Serial.begin(9600);

  motores_init(MOTOR_ESQ_AVANCA, MOTOR_ESQ_RECUA, MOTOR_DIR_AVANCA, MOTOR_DIR_RECUA);

  Serial.println("==================================");
  Serial.println(" TESTE DOS MOTORES INICIADO");
  Serial.println("==================================");

  pararMotores();
  delay(1000);
}

void loop() {
  aguardarComMensagem("Andando para frente", 100);
  andarFrente();
  delay(2000);
  pararMotores();
  delay(1000);

  aguardarComMensagem("Andando para tras", 100);
  andarRe();
  delay(2000);
  pararMotores();
  delay(1000);

  aguardarComMensagem("Curva leve para a esquerda", 100);
  curvaEsquerda();
  delay(2000);
  pararMotores();
  delay(1000);

  aguardarComMensagem("Curva leve para a direita", 100);
  curvaDireita();
  delay(2000);
  pararMotores();
  delay(1000);

  aguardarComMensagem("Virando para a esquerda", 100);
  virarEsquerda();
  delay(2000);
  pararMotores();
  delay(1000);

  aguardarComMensagem("Virando para a direita", 100);
  virarDireita();
  delay(2000);
  pararMotores();
  delay(2000);
}