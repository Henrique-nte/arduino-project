#include <Servo.h>

// ==========================================
// 1. CONFIGURAÇÃO DOS PINOS
// ==========================================
const int pinoSensorLinhaEsq = 2;
const int pinoSensorLinhaDir = 3;

// Pinos do Ultrassônico
const int pinoTrig = 8;
const int pinoEcho = 9;

// Pinos dos Motores Traseiros (Usando Ponte H L298N)
// "Avanca" e "Recua" definem o sentido de giro da roda
const int motorEsqAvanca = 4;
const int motorEsqRecua = 5;
const int motorDirAvanca = 6;
const int motorDirRecua = 7;

// Pino do Servo (Garra)
const int pinoServo = 10;

// ==========================================
// 2. VARIÁVEIS E OBJETOS
// ==========================================
Servo garra;
const int DISTANCIA_OBJETO_CM = 5; // Distância para ativar a garra
long tempoMarcador = 0;            // Variável para controlar o tempo sem usar delay()

// Definição dos Estados possíveis do nosso robô
enum EstadoRobo {
  SEGUIR_LINHA,
  PARAR_FRENTE_OBJETO,
  FECHAR_GARRA,
  RETOMAR_TRAJETO
};

// Variável que guarda o que o robô está fazendo AGORA
EstadoRobo estadoAtual = SEGUIR_LINHA; 

// ==========================================
// 3. SETUP (Configuração Inicial)
// ==========================================
void setup() {
  Serial.begin(9600);
  
  pinMode(pinoSensorLinhaEsq, INPUT_PULLUP);
  pinMode(pinoSensorLinhaDir, INPUT_PULLUP);
  
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  
  // Configurando os pinos dos motores traseiros como saída
  pinMode(motorEsqAvanca, OUTPUT);
  pinMode(motorEsqRecua, OUTPUT);
  pinMode(motorDirAvanca, OUTPUT);
  pinMode(motorDirRecua, OUTPUT);
  
  garra.attach(pinoServo);
  garra.write(0); // Garra aberta inicialmente (0 graus)
  
  Serial.println("Robô Iniciado! Estado: SEGUIR_LINHA");
}

// ==========================================
// 4. FUNÇÕES AUXILIARES
// ==========================================
float lerDistancia() {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  
  long duracao = pulseIn(pinoEcho, HIGH);
  float distancia = duracao * 0.034 / 2;
  return distancia;
}

void pararMotores() {
  digitalWrite(motorEsqAvanca, LOW);
  digitalWrite(motorEsqRecua, LOW);
  digitalWrite(motorDirAvanca, LOW);
  digitalWrite(motorDirRecua, LOW);
}

void andarFrente() {
  // Para ir para frente, ativamos o pino de avanço de ambas as rodas traseiras
  digitalWrite(motorEsqAvanca, HIGH);
  digitalWrite(motorEsqRecua, LOW);
  digitalWrite(motorDirAvanca, HIGH);
  digitalWrite(motorDirRecua, LOW);
}

// ==========================================
// 5. LOOP PRINCIPAL (A Máquina de Estados)
// ==========================================
void loop() {
  // A cada ciclo do loop, o robô verifica em qual estado ele está
  switch (estadoAtual) {

    // ----------------------------------------------------
    case SEGUIR_LINHA:
      {
        float distancia = lerDistancia();
        
        // Se viu um objeto muito perto, muda de estado!
        if (distancia > 0 && distancia <= DISTANCIA_OBJETO_CM) {
          Serial.println("Objeto detectado! Mudando para: PARAR_FRENTE_OBJETO");
          pararMotores();
          tempoMarcador = millis(); // Marca o momento exato que parou
          estadoAtual = PARAR_FRENTE_OBJETO;
          break; // Sai do switch
        }

        // Se não tem objeto, continua a lógica normal de seguir linha
        bool esqNaLinha = (digitalRead(pinoSensorLinhaEsq) == LOW);
        bool dirNaLinha = (digitalRead(pinoSensorLinhaDir) == LOW);

        if (esqNaLinha && dirNaLinha) {
          andarFrente(); // Ambos na linha, vai reto
        } else if (!esqNaLinha && dirNaLinha) {
          // Virar para a direita: Motor esquerdo avança, motor direito para (ou recua)
          digitalWrite(motorEsqAvanca, HIGH);
          digitalWrite(motorEsqRecua, LOW);
          digitalWrite(motorDirAvanca, LOW);
          digitalWrite(motorDirRecua, LOW);
        } else if (esqNaLinha && !dirNaLinha) {
          // Virar para a esquerda: Motor direito avança, motor esquerdo para (ou recua)
          digitalWrite(motorEsqAvanca, LOW);
          digitalWrite(motorEsqRecua, LOW);
          digitalWrite(motorDirAvanca, HIGH);
          digitalWrite(motorDirRecua, LOW);
        } else {
          pararMotores(); // Perdeu a linha, para por segurança
        }
      }
      break;

    // ----------------------------------------------------
    case PARAR_FRENTE_OBJETO:
      // Espera 1 segundo para estabilizar o robô
      if (millis() - tempoMarcador >= 1000) {
        Serial.println("Estabilizou. Mudando para: FECHAR_GARRA");
        tempoMarcador = millis();
        estadoAtual = FECHAR_GARRA;
      }
      break;

    // ----------------------------------------------------
    case FECHAR_GARRA:
      garra.write(90); // Move o servo para 90 graus (fechando a garra)
      
      // Espera 1,5 segundos para a garra terminar de fechar
      if (millis() - tempoMarcador >= 1500) {
        Serial.println("Garra fechada. Mudando para: RETOMAR_TRAJETO");
        estadoAtual = RETOMAR_TRAJETO;
      }
      break;

    // ----------------------------------------------------
    case RETOMAR_TRAJETO:
      // Aqui a garra já está fechada segurando o objeto.
      // O robô volta a procurar a linha e andar.
      Serial.println("Retomando trajeto com o objeto...");
      estadoAtual = SEGUIR_LINHA;
      break;
  }
  
  // Um pequeno delay apenas para não sobrecarregar o simulador
  delay(10); 
}
