#include "motores.h"
#include "sensoresLinha.h"
#include "config/pinos.h"

#include "Arduino.h"

// ==========================================
// ENUM DE MOVIMENTO
// ==========================================
enum Movimento {
  FRENTE,
  CURVA_ESQUERDA,
  CURVA_DIREITA,
  VIRAR_ESQUERDA,
  VIRAR_DIREITA,
  PARADO
};

// ==========================================
// LÓGICA DOS 4 SENSORES
// ==========================================
Movimento calcularMovimento(bool eExt, bool eInt, bool dInt, bool dExt) {

  // Linha central (ideal)
  if (eInt && dInt) {
    return FRENTE;
  }

  // Muito para esquerda
  if ((eExt || eInt) && !dInt && !dExt) {
    if (eExt && eInt) return VIRAR_ESQUERDA;
    return CURVA_ESQUERDA;
  }

  // Muito para direita
  if ((dExt || dInt) && !eInt && !eExt) {
    if (dExt && dInt) return VIRAR_DIREITA;
    return CURVA_DIREITA;
  }

  // Correções leves
  if (eInt) return CURVA_ESQUERDA;
  if (dInt) return CURVA_DIREITA;

  // Perdeu a linha
  return PARADO;
}

// ==========================================
// EXECUÇÃO DO MOVIMENTO
// ==========================================
void executarMovimento(Movimento mov) {
  switch (mov) {
    case FRENTE:
      andarFrente();
      break;

    case CURVA_ESQUERDA:
      curvaEsquerda();
      break;

    case CURVA_DIREITA:
      curvaDireita();
      break;

    case VIRAR_ESQUERDA:
      virarEsquerda();
      break;

    case VIRAR_DIREITA:
      virarDireita();
      break;

    case PARADO:
    default:
      pararMotores();
      break;
  }
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(9600);

  motores_init(
    MOTOR_ESQ_AVANCA,
    MOTOR_ESQ_RECUA,
    MOTOR_DIR_AVANCA,
    MOTOR_DIR_RECUA
  );

  sensores_init(
    PINO_ESQ_EXT,
    PINO_ESQ_INT,
    PINO_DIR_INT,
    PINO_DIR_EXT
  );

  Serial.println("==================================");
  Serial.println(" TESTE: LINHA + MOTORES");
  Serial.println("==================================");

  pararMotores();
  delay(1000);
}

// ==========================================
// LOOP
// ==========================================
void loop() {
  bool eExt, eInt, dInt, dExt;

  lerSensores(eExt, eInt, dInt, dExt);

  Movimento mov = calcularMovimento(eExt, eInt, dInt, dExt);

  executarMovimento(mov);

  // DEBUG VISUAL (MUITO IMPORTANTE)
  Serial.print("Sensores: ");

  Serial.print(eExt ? "[■]" : "[ ]");
  Serial.print(eInt ? "[■]" : "[ ]");
  Serial.print(" | ");
  Serial.print(dInt ? "[■]" : "[ ]");
  Serial.print(dExt ? "[■]" : "[ ]");

  Serial.print("  ->  ");

  switch (mov) {
    case FRENTE: Serial.println("FRENTE"); break;
    case CURVA_ESQUERDA: Serial.println("CURVA ESQ"); break;
    case CURVA_DIREITA: Serial.println("CURVA DIR"); break;
    case VIRAR_ESQUERDA: Serial.println("VIRA ESQ"); break;
    case VIRAR_DIREITA: Serial.println("VIRA DIR"); break;
    case PARADO: Serial.println("PARADO"); break;
  }

  delay(20);
}