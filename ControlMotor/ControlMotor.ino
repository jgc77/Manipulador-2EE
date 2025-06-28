#include <AccelStepper.h>

// Definição dos motores
AccelStepper stepper1(1, 4, 5);  // Junta 1 - Base
AccelStepper stepper2(1, 6, 7);  // Junta 2 - Ombro
AccelStepper stepper3(1, 8, 9);  // Junta 3 - Cotovelo

const int pinoSeletor = 2;      // Pino da Seletor
int estadoAnteriorSeletor = HIGH;  // Estado anterior da Seletor

// Alvos de posição
long target1 = 0;
long target2 = 0;
long target3 = 0;

String inputString = "";
bool newData = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinoSeletor, INPUT_PULLUP);  // Usa resistor pull-up interno

  // Configuração dos motores
  stepper1.setMaxSpeed(400);
  stepper1.setAcceleration(200);

  stepper2.setMaxSpeed(400);
  stepper2.setAcceleration(200);

  stepper3.setMaxSpeed(400);
  stepper3.setAcceleration(200);

  Serial.println("Digite os ângulos alvo para base, ombro e cotovelo separados por espaço (ex: 200 -150 100):");
}

void loop() {
  // Leitura da Seletor
  int estadoAtualSeletor = digitalRead(pinoSeletor);
  
  // Só imprime se houve mudança de estado
  if (estadoAtualSeletor != estadoAnteriorSeletor) {
    if (estadoAtualSeletor == HIGH) {
      Serial.println("Seletor 0");
    } else {
      Serial.println("Seletor 1");
    }
    estadoAnteriorSeletor = estadoAtualSeletor;
  }

  // Leitura Serial
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      newData = true;
    } else {
      inputString += inChar;
    }
  }

  if (newData) {
    parseInput(inputString);
    inputString = "";
    newData = false;
  }

  // Movimentação
  stepper1.moveTo(target1);
  stepper2.moveTo(target2);
  stepper3.moveTo(target3);

  stepper1.run();
  stepper2.run();
  stepper3.run();
}

// Interpreta a string recebida e extrai os valores
void parseInput(String input) {
  input.trim(); // Remove espaços extras

  int space1 = input.indexOf(' ');
  int space2 = input.indexOf(' ', space1 + 1);

  if (space1 > 0 && space2 > space1) {
    long val1 = input.substring(0, space1).toInt();
    long val2 = input.substring(space1 + 1, space2).toInt();
    long val3 = input.substring(space2 + 1).toInt();

    target1 = val1;
    target2 = val2;
    target3 = val3;

    Serial.print("Movendo para: Base = ");
    Serial.print(target1);
    Serial.print(" | Ombro = ");
    Serial.print(target2);
    Serial.print(" | Cotovelo = ");
    Serial.println(target3);
  } else {
    Serial.println("Formato inválido. Use: valor1 valor2 valor3 (ex: 200 -150 100)");
  }
}
