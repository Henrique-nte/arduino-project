#include "../config/pinos.h"
#include "../src/motores/motores.h"
#include "../src/sensoresLinha/sensoresLinha.h"
#include "../src/ultrassonico/ultrassonico.h"
#include "../src/garra/garra.h"
#include "Arduino.h"

enum Movimento {
  FRENTE,
  ESQ,
  DIR,
  PARADO,
  BUSCAR_LINHA  // Estado novo para quando perde a linha
};

// ============ CONSTANTES ============
const int DELAY_CICLO_MS = 100;           // Delay entre leituras (ajuste para velocidade)
const int CICLOS_BUSCA_MAX = 20;          // Máximo ciclos girando antes de desistir
int ciclosBuscaAtual = 0;
bool emBuscaLinhaAgora = false;

// ============ SETUP ============
void setup() {
  
  // Inicializa módulos com CONSTANTES do pinos.h (não hardcoded!)
  motores_init(MOTOR_ESQ_AVANCA, MOTOR_ESQ_RECUA, MOTOR_DIR_AVANCA, MOTOR_DIR_RECUA);
  sensores_init(PINO_ESQ_EXT, PINO_ESQ_INT, PINO_DIR_INT, PINO_DIR_EXT);
  ultra_init(PINO_TRIG, PINO_ECHO);
  garra_init(PINO_SERVO);
  
  pararMotores();
}

/**
 * Calcula próximo movimento baseado em leitura dos sensores de linha
 * 
 * LÓGICA DE PRIORIDADE:
 *   1. Se ambos sensores INTERNOS veem linha (preto) → FRENTE
 *   2. Se sensor EXTERNO ESQUERDO vê linha → ESQ (linha saiu à esquerda)
 *   3. Se sensor EXTERNO DIREITO vê linha → DIR (linha saiu à direita)
 *   4. Se NENHUM sensor vê a linha → BUSCAR_LINHA (gira para encontrar)
 */
Movimento calcular(bool eExt, bool eInt, bool dInt, bool dExt) {
  // Condição nominal: ambos sensores internos no preto
  if (eInt && dInt) {
    emBuscaLinhaAgora = false;  // Sai do estado de busca
    ciclosBuscaAtual = 0;
    return FRENTE;
  }
  
  // Linha desviou para esquerda
  if (eExt) {
    emBuscaLinhaAgora = false;
    return ESQ;
  }
  
  // Linha desviou para direita
  if (dExt) {
    emBuscaLinhaAgora = false;
    return DIR;
  }
  
  // Nenhum sensor vê a linha!
  return BUSCAR_LINHA;
}

/**
 * Executa o movimento decidido pela máquina de estados
 */
void executarMovimento(Movimento m) {
  switch (m) {
    case FRENTE:
      andarFrente();
      break;
      
    case ESQ:
      curvaEsquerda();
      break;
      
    case DIR:
      curvaDireita();
      break;
      
    case BUSCAR_LINHA:
      // Gira lentamente para encontrar linha novamente
      if (!emBuscaLinhaAgora) {
        emBuscaLinhaAgora = true;
        ciclosBuscaAtual = 0;
      }
      
      // Limita tempo de busca para evitar loop infinito (fail-safe)
      if (ciclosBuscaAtual < CICLOS_BUSCA_MAX) {
        virarDireita();  // Gira para direita buscando
        ciclosBuscaAtual++;
      } else {
        // Timeout de busca alcançado: segurança
        pararMotores();
        emBuscaLinhaAgora = false;
        ciclosBuscaAtual = 0;
      }
      break;
      
    case PARADO:
    default:
      pararMotores();
      break;
  }
}

void loop() {
  // 1. LER SENSORES
  bool eExt, eInt, dInt, dExt;
  lerSensores(eExt, eInt, dInt, dExt);
  
  // 2. CALCULAR MOVIMENTO
  Movimento m = calcular(eExt, eInt, dInt, dExt);
  
  // 3. EXECUTAR
  executarMovimento(m);
  
  // 4. DELAY PARA PRÓXIMO CICLO
  delay(DELAY_CICLO_MS);
}