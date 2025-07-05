#include "ControlMotor.h"

String inputString = "";
bool newData = false;

void setup() {
  Serial.begin(9600);
  inicializarMotores();

  Serial.println("Digite os ângulos (graus) para base, ombro e cotovelo (ex: 90 -45 30):");
}

void loop() {
  verificarSeletor();

  // Exemplo de uso do seletor
  bool seletor = obterEstadoSeletor();

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

  atualizarMotores();
}

void processaComando(String input) {
  input.trim();

  if (input.equalsIgnoreCase("calib")) {
    ativarCalibracao();
    Serial.println("Modo calibração ativado. Digite 'exit' para desativar.");
    return;
  }

  if (emModoCalibracao() && input.equalsIgnoreCase("exit")) {
    desativarCalibracao();
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

    setAlvosGraus(ang1, ang2, ang3);

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