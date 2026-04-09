#include <Servo.h>

// ==========================================
// 1. CONFIGURAÇÃO DOS PINOS
// ==========================================
const int pinoSensorLinhaEsq = 2;
const int pinoSensorLinhaDir = 3;

const int pinoTrig = 8;
const int pinoEcho = 9;

const int motorEsqAvanca = 4;
const int motorEsqRecua = 5;
const int motorDirAvanca = 6;
const int motorDirRecua = 7;

const int pinoServo = 10;

// ==========================================
// 2. VARIÁVEIS E ESTADOS
// ==========================================
Servo garra;
const int DISTANCIA_OBJETO_CM = 5; 
long tempoMarcador = 0;            

enum EstadoRobo {
  SEGUIR_LINHA,
  PARAR_FRENTE_OBJETO,
  FECHAR_GARRA,
  RETOMAR_TRAJETO
};

enum Movimento {
  FRENTE,
  DIREITA,
  ESQUERDA,
  PARADO
};

EstadoRobo estadoAtual = SEGUIR_LINHA;
Movimento movimentoAtual = PARADO;

// ==========================================
// 3. SETUP
// ==========================================
void setup() {
  Serial.begin(9600);
  
  pinMode(pinoSensorLinhaEsq, INPUT_PULLUP);
  pinMode(pinoSensorLinhaDir, INPUT_PULLUP);
  
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  
  pinMode(motorEsqAvanca, OUTPUT);
  pinMode(motorEsqRecua, OUTPUT);
  pinMode(motorDirAvanca, OUTPUT);
  pinMode(motorDirRecua, OUTPUT);
  
  garra.attach(pinoServo);
  garra.write(0); // Inicia aberta
  
  Serial.println("Robô Iniciado! Estado: SEGUIR_LINHA");
}

// ==========================================
// 4. FUNÇÕES DE MOVIMENTAÇÃO
// ==========================================

void pararMotores() {
  digitalWrite(motorEsqAvanca, LOW);
  digitalWrite(motorEsqRecua, LOW);
  digitalWrite(motorDirAvanca, LOW);
  digitalWrite(motorDirRecua, LOW);
}

void andarFrente() {
  digitalWrite(motorEsqAvanca, HIGH);
  digitalWrite(motorEsqRecua, LOW);
  digitalWrite(motorDirAvanca, HIGH);
  digitalWrite(motorDirRecua, LOW);
}

void andar_re() {
  digitalWrite(motorEsqAvanca, LOW);
  digitalWrite(motorEsqRecua, HIGH);
  digitalWrite(motorDirAvanca, LOW);
  digitalWrite(motorDirRecua, HIGH);
}

void virar_90g_Direita() {
    digitalWrite(motorEsqAvanca, HIGH);
    digitalWrite(motorEsqRecua, LOW);
    digitalWrite(motorDirAvanca, LOW);
    digitalWrite(motorDirRecua, HIGH);
    delay(500); 
    pararMotores();
}

void dar_re_90g_direita(){
    digitalWrite(motorEsqAvanca, LOW);
    digitalWrite(motorEsqRecua, HIGH);
    digitalWrite(motorDirAvanca, LOW);
    digitalWrite(motorDirRecua, LOW);
    delay(500); 
    pararMotores();
}

void andar_1cm_frente() {
    andarFrente();
    delay(500); 
    pararMotores();
}

// Lógica para seguir linha (simples, para testes)
Movimento calcularMovimento(bool esqNaLinha, bool dirNaLinha) {
  if (esqNaLinha && dirNaLinha) return FRENTE;
  if (!esqNaLinha && dirNaLinha) return DIREITA;
  if (esqNaLinha && !dirNaLinha) return ESQUERDA;
  return PARADO;
}

void executarMovimento(Movimento mov) {
  switch (mov) {
    case FRENTE: andarFrente(); break;
    case DIREITA:
      digitalWrite(motorEsqAvanca, HIGH);
      digitalWrite(motorEsqRecua, LOW);
      digitalWrite(motorDirAvanca, LOW);
      digitalWrite(motorDirRecua, LOW);
      break;
    case ESQUERDA:
      digitalWrite(motorEsqAvanca, LOW);
      digitalWrite(motorEsqRecua, LOW);
      digitalWrite(motorDirAvanca, HIGH);
      digitalWrite(motorDirRecua, LOW);
      break;
    case PARADO: pararMotores(); break;
  }
}

float lerDistancia() {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  long duracao = pulseIn(pinoEcho, HIGH);
  return duracao * 0.034 / 2;
}

// ==========================================
// 5. LOOP PRINCIPAL
// ==========================================
void loop() {
  switch (estadoAtual) {

    case SEGUIR_LINHA:
      {
        float distancia = lerDistancia();
        if (distancia > 0 && distancia <= DISTANCIA_OBJETO_CM) {
          Serial.println("Objeto detectado! Mudando para: PARAR_FRENTE_OBJETO");
          pararMotores();
          tempoMarcador = millis();
          estadoAtual = PARAR_FRENTE_OBJETO;
          break;
        }

        bool esqNaLinha = (digitalRead(pinoSensorLinhaEsq) == LOW);
        bool dirNaLinha = (digitalRead(pinoSensorLinhaDir) == LOW);
        movimentoAtual = calcularMovimento(esqNaLinha, dirNaLinha);
        executarMovimento(movimentoAtual);
      }
      break;

    case PARAR_FRENTE_OBJETO:
      if (millis() - tempoMarcador >= 1000) {
        tempoMarcador = millis();
        estadoAtual = FECHAR_GARRA;
      }
      break;

    case FECHAR_GARRA:
      garra.write(90); // Fecha
      if (millis() - tempoMarcador >= 1500) {
        Serial.println("Garra fechada. Fazendo manobra...");
        virar_90g_Direita();
        andar_1cm_frente();
        estadoAtual = RETOMAR_TRAJETO;
      }
      break;

    case RETOMAR_TRAJETO:
      Serial.println("Soltando objeto e voltando...");
      garra.write(0); // Abre
      delay(1000); 
      
      andar_re();
      delay(500);
      pararMotores();
      
      dar_re_90g_direita();
      delay(500);
      pararMotores();
      
      estadoAtual = SEGUIR_LINHA;
      break;
  }
  delay(10);
}
