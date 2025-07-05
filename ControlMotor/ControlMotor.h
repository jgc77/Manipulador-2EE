#ifndef CONTROLMOTOR_H
#define CONTROLMOTOR_H

#include <AccelStepper.h>

// Inicialização dos motores
extern AccelStepper stepper1;
extern AccelStepper stepper2;
extern AccelStepper stepper3;

extern const int enable1;
extern const int enable2;
extern const int enable3;

void inicializarMotores();
void atualizarMotores();
void setAlvosGraus(float base, float ombro, float cotovelo);
void ativarCalibracao();
void desativarCalibracao();
bool emModoCalibracao();

void verificarSeletor();
bool obterEstadoSeletor(); 

#endif