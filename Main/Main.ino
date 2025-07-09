#include "ControlMotor.h"
#include "Cinematic.h"

String inputString = "";
bool newData = false;

const Dim dim = {5.7, 6.0, 7.3}; // Dimensões do manipulador

bool modoAtual = -1; // -1 = indefinido, 0 = direta, 1 = inversa

void setup() {
  Serial.begin(9600);
  inicializarMotores();

  Serial.println("=========================================");
  Serial.println(" Manipulador Robótico RRR Antropomórfico");
  Serial.println("=========================================");
  Serial.println("Selecione na chave seletora:");
  Serial.println(" [1] Cinemática Inversa");
  Serial.println(" [2] Cinemática Direta");
  Serial.println("Ou digite 'calib' para entrar no modo calibração");
  Serial.println();
}

void loop() {
  verificarSeletor();
  bool seletor = obterEstadoSeletor(); // true = direta (1), false = inversa (0)

  // Atualiza e imprime o modo selecionado se houver mudança
  if (seletor != modoAtual) {
    modoAtual = seletor;
    Serial.println();
    if (modoAtual == 0) {
      Serial.println(">> Modo selecionado: Cinemática Direta");
      Serial.println("Digite os ângulos da base, ombro e cotovelo (ex: 90 -45 30)");
      Serial.println("O sistema irá calcular e exibir a posição final do atuador.");
    } else {
      Serial.println(">> Modo selecionado: Cinemática Inversa");
      Serial.println("Digite a posição desejada do atuador no formato X Y Z (ex: 10 5 8)");
    }
    Serial.println();
  }

  // Leitura da serial
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      newData = true;
    } else {
      inputString += inChar;
    }
  }

  if (newData) {
    processaComando(inputString, modoAtual);
    inputString = "";
    newData = false;
  }

  atualizarMotores();
}

void processaComando(String input, bool seletorDireto) {
  input.trim();

  if (input.equalsIgnoreCase("calib")) {
    ativarCalibracao();
    Serial.println(">> Modo calibração ativado. Digite 'exit' para desativar.");
    return;
  }

  if (emModoCalibracao() && input.equalsIgnoreCase("exit")) {
    desativarCalibracao();
    Serial.println(">> Modo calibração desativado.");
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

    if (seletorDireto == 0) {
      // Cinemática Direta: entrada = ângulos
      setAlvosGraus(v1, v2, v3);

      Serial.println();
      Serial.print(">> Cinemática Direta <<"); Serial.println();
      Serial.print(">> Movendo para: Base = ");
      Serial.print(v1); Serial.print("°, Ombro = ");
      Serial.print(v2); Serial.print("°, Cotovelo = ");
      Serial.print(v3); Serial.println("°");

      // Calcula posição resultante do atuador
      Ang ang = {v1, v2, v3};
      Pos pos = cin_dir(dim, ang);
      
      Serial.print(">> Posição estimada do atuador (X Y Z): ");
      Serial.print(pos.x, 2); Serial.print(" ");
      Serial.print(pos.y, 2); Serial.print(" ");
      Serial.print(pos.z, 2); Serial.println();
      Serial.println();

    } else {
      // Cinemática Inversa: entrada = posição (x y z)
      Pos alvo = {v1, v2, v3};
      Ang ang = cin_inv(dim, alvo);

      if (isnan(ang.t1) || isnan(ang.t2) || isnan(ang.t3)) {
        Serial.println(">> ERRO: Posição fora do alcance do braço.");
        return;
      }

      setAlvosGraus(ang.t1, ang.t2, ang.t3);

      Serial.println();
      Serial.print(">> Cinemática Inversa <<"); Serial.println();
      Serial.print(">> Posição desejada (X Y Z): ");
      Serial.print(v1); Serial.print(" ");
      Serial.print(v2); Serial.print(" ");
      Serial.print(v3); Serial.println();

      Serial.print(">> Movendo para ângulos: Base = ");
      Serial.print(ang.t1); Serial.print("°, Ombro = ");
      Serial.print(ang.t2); Serial.print("°, Cotovelo = ");
      Serial.print(ang.t3); Serial.println("°");
      Serial.println();
    }

  } else {
    Serial.println(">> Formato inválido. Use três valores separados por espaço.");
  }
}
