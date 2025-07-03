#include <AccelStepper.h>

// Definição dos motores
AccelStepper stepper1(1, 4, 5);  // Junta 1 - Base
AccelStepper stepper2(1, 6, 7);  // Junta 2 - Ombro
AccelStepper stepper3(1, 8, 9);  // Junta 3 - Cotovelo

const int pinoSeletor = 2;  // Pino da Seletor
int estadoAnteriorSeletor = HIGH;

const int enable1 = 10;
const int enable2 = 11;
const int enable3 = 12;

bool modoCalibracao = false;

long target1 = 0;
long target2 = 0;
long target3 = 0;

String inputString = "";
bool newData = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinoSeletor, INPUT_PULLUP);

  // Pinos de enable dos drivers
  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  pinMode(enable3, OUTPUT);

  // Inicialmente, desativa os drivers (nível alto no enable barrado)
  digitalWrite(enable1, LOW);
  digitalWrite(enable2, LOW);
  digitalWrite(enable3, LOW);

  // Configuração dos motores
  stepper1.setMaxSpeed(100);
  stepper1.setAcceleration(50);

  stepper2.setMaxSpeed(100);
  stepper2.setAcceleration(50);

  stepper3.setMaxSpeed(100);
  stepper3.setAcceleration(50);

  Serial.println("Digite os ângulos alvo para base, ombro e cotovelo separados por espaço (ex: 200 -150 100):");
}

void loop() {
  int estadoAtualSeletor = digitalRead(pinoSeletor);
  if (estadoAtualSeletor != estadoAnteriorSeletor) {
    Serial.println(estadoAtualSeletor == HIGH ? "Seletor 0" : "Seletor 1");
    estadoAnteriorSeletor = estadoAtualSeletor;
  }

  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      newData = true;
    } else {
      inputString += inChar;
    }
  }

  if (newData) {
    processaComando(inputString);
    inputString = "";
    newData = false;
  }

  // Movimentação normal
  stepper1.moveTo(target1);
  stepper2.moveTo(target2);
  stepper3.moveTo(target3);

  stepper1.run();
  stepper2.run();
  stepper3.run();
}

void processaComando(String input) {
  input.trim();

  if (input.equalsIgnoreCase("calib")) {
    modoCalibracao = true;
    digitalWrite(enable1, HIGH);
    digitalWrite(enable2, HIGH);
    digitalWrite(enable3, HIGH);
    Serial.println("Modo calibração ativado. Digite 'ok' para desativar.");
    return;
  }

  if (modoCalibracao && input.equalsIgnoreCase("ok")) {
    modoCalibracao = false;
    digitalWrite(enable1, LOW);
    digitalWrite(enable2, LOW);
    digitalWrite(enable3, LOW);
    Serial.println("Modo calibração desativado.");
    return;
  }

  parseInput(input);
}

void parseInput(String input) {
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
