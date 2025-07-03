#include <AccelStepper.h>

// Definição dos motores
AccelStepper stepper1(1, 4, 5);  // Base
AccelStepper stepper2(1, 6, 7);  // Ombro
AccelStepper stepper3(1, 8, 9);  // Cotovelo

const int pinoSeletor = 2;
int estadoAnteriorSeletor = HIGH;

const int enable1 = 10;
const int enable2 = 11;
const int enable3 = 12;

bool modoCalibracao = false;

// Fator de conversão grau → passo
const float passoPorGrau = 2048.0 / 360.0;

// Alvos (em passos)
long target1 = 0;
long target2 = 0;
long target3 = 0;

String inputString = "";
bool newData = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinoSeletor, INPUT_PULLUP);

  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  pinMode(enable3, OUTPUT);

  digitalWrite(enable1, LOW);
  digitalWrite(enable2, LOW);
  digitalWrite(enable3, LOW);

  stepper1.setMaxSpeed(100);
  stepper1.setAcceleration(50);

  stepper2.setMaxSpeed(100);
  stepper2.setAcceleration(50);

  stepper3.setMaxSpeed(100);
  stepper3.setAcceleration(50);

  Serial.println("Digite os ângulos (graus) para base, ombro e cotovelo (ex: 90 -45 30):");
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
    float ang1 = input.substring(0, space1).toFloat();
    float ang2 = input.substring(space1 + 1, space2).toFloat();
    float ang3 = input.substring(space2 + 1).toFloat();

    target1 = grausParaPassos(limitarBase(ang1));
    target2 = grausParaPassos(limitarOmbro(ang2));
    target3 = grausParaPassos(limitarCotovelo(ang3));

    Serial.print("Movendo para: Base = ");
    Serial.print(ang1);
    Serial.print("° | Ombro = ");
    Serial.print(ang2);
    Serial.print("° | Cotovelo = ");
    Serial.print(ang3);
    Serial.println("°");
  } else {
    Serial.println("Formato inválido. Use: grau1 grau2 grau3 (ex: 90 -45 30)");
  }
}

// Limites em graus
float limitarBase(float ang) {
  return constrain(ang, -180, 180);
}

float limitarOmbro(float ang) {
  return constrain(ang, -90, 90);
}

float limitarCotovelo(float ang) {
  return constrain(ang, -90, 90);
}

// Conversão grau → passo
long grausParaPassos(float grau) {
  return round(grau * passoPorGrau);
}
