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

//adicionado por JOAO ----------------

void virar_90g_Direita() {
    digitalWrite(motorEsqAvanca, HIGH);
    digitalWrite(motorEsqRecua, LOW);
    digitalWrite(motorDirAvanca, LOW);
    digitalWrite(motorDirRecua, HIGH);
    delay(500); // Ajuste esse valor para calibrar o tempo de giro
    pararMotores();
}

void virarEsquerda() {
  digitalWrite(motorEsqAvanca, LOW);
  digitalWrite(motorEsqRecua, LOW);
  digitalWrite(motorDirAvanca, HIGH);
  digitalWrite(motorDirRecua, LOW);
}

void andar_1cm_re(){
    digitalWrite(motorEsqAvanca, LOW);
    digitalWrite(motorEsqRecua, HIGH);
    digitalWrite(motorDirAvanca, LOW);
    digitalWrite(motorDirRecua, HIGH);
    delay(500); // Ajuste esse valor para calibrar a distância de 1 cm
    pararMotores();
}

void dar_re_90g_direita(){
    digitalWrite(motorEsqAvanca, LOW);
    digitalWrite(motorEsqRecua, HIGH);
    digitalWrite(motorDirAvanca, LOW);
    digitalWrite(motorDirRecua, LOW);
    delay(500); // Ajuste esse valor para calibrar o tempo de giro
    pararMotores();
}

void andar_1cm_frente() {
    andarFrente();
    delay(500); // Ajuste esse valor para calibrar a distância de 1 cm
    pararMotores();
}

// ==========================================
// 5. LOOP PRINCIPAL (A Máquina de Estados)
// ==========================================
void loop() {
  // A cada ciclo do loop, o robô verifica em qual estado ele está
  switch (estadoAtual) {

    // ----------------------------------------------------
    case FECHAR_GARRA:
      garra.write(90); // Move o servo para 90 graus (fechando a garra)
      // Espera 1,5 segundos para a garra terminar de fechar
      if (millis() - tempoMarcador >= 1500) {
        Serial.println("Garra fechada. Mudando para: RETOMAR_TRAJETO");
        virar_90g_Direita();
        andar_1cm_frente();
        estadoAtual = RETOMAR_TRAJETO;
      }
      break;

    // ----------------------------------------------------
    case RETOMAR_TRAJETO:
      Serial.println("Retomando trajeto com o objeto...");
      garra.write(0); // Abre a garra para soltar o objeto
        delay(1000); // Espera a garra abrir
      andar_re(); // Dá ré para se afastar do local
        delay(500); // Ajuste esse valor para calibrar a distância de ré
        pararMotores();
      dar_re_90g_direita(); // Vira para a direita para se alinhar com a linha
        delay(500); // Ajuste esse valor para calibrar o tempo de giro
        pararMotores();
      estadoAtual = SEGUIR_LINHA;
      break;
  }
  // Um pequeno delay apenas para não sobrecarregar o simulador
  delay(10); 
}
