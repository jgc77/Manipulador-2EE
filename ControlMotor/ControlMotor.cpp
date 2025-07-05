#include "motor.h"

const float passoPorGrau = 2048.0 / 360.0;

AccelStepper stepper1(1, 4, 5);
AccelStepper stepper2(1, 6, 7);
AccelStepper stepper3(1, 8, 9);

const int enable1 = 10;
const int enable2 = 11;
const int enable3 = 12;

const int pinoSeletor = 2;
int estadoAnteriorSeletor = HIGH;

long target1 = 0;
long target2 = 0;
long target3 = 0;

bool modoCalibracao = false;
bool estadoSeletor = true;


void inicializarMotores() {
  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  pinMode(enable3, OUTPUT);

  digitalWrite(enable1, LOW);
  digitalWrite(enable2, LOW);
  digitalWrite(enable3, LOW);

  stepper1.setMaxSpeed(90);
  stepper1.setAcceleration(50);

  stepper2.setMaxSpeed(90);
  stepper2.setAcceleration(50);

  stepper3.setMaxSpeed(90);
  stepper3.setAcceleration(50);

  pinMode(pinoSeletor, INPUT_PULLUP);
}

void atualizarMotores() {
  stepper1.moveTo(target1);
  stepper2.moveTo(target2);
  stepper3.moveTo(target3);

  stepper1.run();
  stepper2.run();
  stepper3.run();
}

void verificarSeletor() {
  int estadoAtual = digitalRead(pinoSeletor);
  if (estadoAtual != estadoAnteriorSeletor) {
    Serial.println(estadoAtual == HIGH ? "Seletor 0" : "Seletor 1");
    estadoAnteriorSeletor = estadoAtual;
  }
  estadoSeletor = (estadoAtual == HIGH);  // salva o estado atual
}

bool obterEstadoSeletor() {
  return estadoSeletor;
}


void setAlvosGraus(float base, float ombro, float cotovelo) {
  target1 = grausParaPassos(limitarBase(base));
  target2 = grausParaPassos(limitarOmbro(ombro));
  target3 = grausParaPassos(limitarCotovelo(cotovelo));
}

void ativarCalibracao() {
  modoCalibracao = true;
  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);
}

void desativarCalibracao() {
  modoCalibracao = false;
  digitalWrite(enable1, LOW);
  digitalWrite(enable2, LOW);
  digitalWrite(enable3, LOW);
}

bool emModoCalibracao() {
  return modoCalibracao;
}

// Funções auxiliares
float limitarBase(float ang) {
  return constrain(ang, -180, 180);
}

float limitarOmbro(float ang) {
  return constrain(ang, -90, 90);
}

float limitarCotovelo(float ang) {
  return constrain(ang, -90, 90);
}

long grausParaPassos(float grau) {
  return round(grau * passoPorGrau);
}
