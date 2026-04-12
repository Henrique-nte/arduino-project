#include "sensoresLinha.h"
#include "config/pinos.h"

#include "Arduino.h"

void setup() {
  Serial.begin(9600);

  sensores_init(
    PINO_ESQ_EXT,
    PINO_ESQ_INT,
    PINO_DIR_INT,
    PINO_DIR_EXT
  );

  Serial.println("==================================");
  Serial.println(" TESTE DOS SENSORES DE LINHA");
  Serial.println("==================================");

  Serial.println("Formato:");
  Serial.println("[E_EXT] [E_INT] | [D_INT] [D_EXT]");
  Serial.println("");
}

void loop() {
  bool eExt, eInt, dInt, dExt;
  lerSensores(eExt, eInt, dInt, dExt);

  Serial.print("Sensores: ");

  Serial.print(eExt ? "[■]" : "[ ]");
  Serial.print(eInt ? "[■]" : "[ ]");
  Serial.print(" | ");
  Serial.print(dInt ? "[■]" : "[ ]");
  Serial.print(dExt ? "[■]" : "[ ]");

  Serial.println();

  delay(200);
}