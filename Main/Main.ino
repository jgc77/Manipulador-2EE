#include "ControlMotor.h"
#include "Cinematic.h"  // Cinemática direta/inversa

String inputString = "";
bool newData = false;

// Define dimensões fixas do robô (em cm, por exemplo)
const Dim dim = {6.8, 6.5, 5.0};

void setup() {
  Serial.begin(9600);
  inicializarMotores(); // Inicializa os motores

  Serial.println("Digite:");
  Serial.println("Modo direto:    90 -45 30");
  Serial.println("Modo inverso:   10 5 8");
}

void loop() {
  verificarSeletor(); // Lê seletor físico
  bool seletor = obterEstadoSeletor(); // true = modo direto, false = modo inverso

  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      newData = true;
    } else {
      inputString += inChar;
    }
  }

  if (newData) {
    processaComando(inputString, seletor);
    inputString = "";
    newData = false;
  }

  atualizarMotores(); // Move motores de forma gradual
}

void processaComando(String input, bool seletorDireto) {
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

  parseInput(input, seletorDireto);
}

void parseInput(String input, bool seletorDireto) {
  int space1 = input.indexOf(' ');
  int space2 = input.indexOf(' ', space1 + 1);

  if (space1 > 0 && space2 > space1) {
    float v1 = input.substring(0, space1).toFloat();
    float v2 = input.substring(space1 + 1, space2).toFloat();
    float v3 = input.substring(space2 + 1).toFloat();

    if (seletorDireto) {
      // Modo Direto: usuário passou ângulos
      setAlvosGraus(v1, v2, v3);

      Serial.print("Movendo para: Base = ");
      Serial.print(v1);
      Serial.print("° | Ombro = ");
      Serial.print(v2);
      Serial.print("° | Cotovelo = ");
      Serial.print(v3);
      Serial.println("°");

    } else {
      // Modo Inverso: usuário passou posição (x, y, z)
      Pos alvo = {v1, v2, v3};
      Ang ang = cin_inv(dim, alvo);

      if (isnan(ang.t1) || isnan(ang.t2) || isnan(ang.t3)) {
        Serial.println("Posição fora do alcance do braço.");
        return;
      }

      setAlvosGraus(ang.t1, ang.t2, ang.t3);

      Serial.print("Alvo XYZ = ");
      Serial.print(v1); Serial.print(", ");
      Serial.print(v2); Serial.print(", ");
      Serial.println(v3);

      Serial.print("Movendo para: Base = ");
      Serial.print(ang.t1);
      Serial.print("° | Ombro = ");
      Serial.print(ang.t2);
      Serial.print("° | Cotovelo = ");
      Serial.print(ang.t3);
      Serial.println("°");
    }

  } else {
    Serial.println("Formato inválido. Use: v1 v2 v3 (ex: 90 -45 30 ou 10 5 8)");
  }
}
