#include "garra.h"
#include "config/pinos.h"

#include "Arduino.h"

void setup() {
  Serial.begin(9600);

  garra_init(PINO_SERVO);

  Serial.println("==================================");
  Serial.println(" TESTE DA GARRA");
  Serial.println("==================================");
  Serial.println("Abrindo e fechando a garra em ciclo.");
  Serial.println("");
}

void loop() {
  Serial.println("Abrindo garra");
  abrirGarra();
  delay(2000);

  Serial.println("Fechando garra");
  fecharGarra();
  delay(2000);
}