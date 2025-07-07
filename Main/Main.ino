#include "ControlMotor.h"

String inputString = "";
bool newData = false;

void setup() {
  Serial.begin(9600);
  inicializarMotores(); // Inicializa os motores

  Serial.println("Digite os ângulos (graus) para base, ombro e cotovelo (ex: 90 -45 30):");
}

void loop() {
  verificarSeletor(); // Lê seletor físico ( muda modo de operação)

  bool seletor = obterEstadoSeletor(); // Obtém estado atual do seletor

  // Leitura da serial (entrada do usuário)
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      newData = true;
    } else {
      inputString += inChar;
    }
  }

  // Processa comando quando uma linha completa for recebida
  if (newData) {
    processaComando(inputString);
    inputString = "";
    newData = false;
  }

  atualizarMotores(); // Atualiza posição dos motores gradualmente
}

void processaComando(String input) {
  input.trim();

  // Ativa modo de calibração
  if (input.equalsIgnoreCase("calib")) {
    ativarCalibracao();
    Serial.println("Modo calibração ativado. Digite 'exit' para desativar.");
    return;
  }

  // Sai do modo de calibração
  if (emModoCalibracao() && input.equalsIgnoreCase("exit")) {
    desativarCalibracao();
    Serial.println("Modo calibração desativado.");
    return;
  }

  parseInput(input); // Interpreta entrada com 3 ângulos
}

void parseInput(String input) {
  int space1 = input.indexOf(' ');
  int space2 = input.indexOf(' ', space1 + 1);

  // Se formato válido, extrai e define ângulos
  if (space1 > 0 && space2 > space1) {
    float ang1 = input.substring(0, space1).toFloat();
    float ang2 = input.substring(space1 + 1, space2).toFloat();
    float ang3 = input.substring(space2 + 1).toFloat();

    setAlvosGraus(ang1, ang2, ang3); // Define metas dos motores

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
