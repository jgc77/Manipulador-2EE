# Manipulador RRR - Arduino

Este projeto implementa um manipulador robótico com 3 graus de liberdade, composto por três juntas rotacionais (RRR), controlado via Arduino. O sistema utiliza motores de passo acionados por drivers e é estruturado com uma arquitetura modular, separando o controle de hardware e os cálculos de cinemática.

## 🎓 Finalidade

Este trabalho foi desenvolvido como parte do **Projeto 2EE de Robótica** do curso de **Engenharia de Controle e Automação** da **Universidade de Pernambuco (UPE)**.

## 🛠️ Itens do Projeto

- Manipulador robótico com 3 juntas rotacionais
- Controle de motores de passo com biblioteca AccelStepper
- Interface serial para envio de comandos
- Seletor para leitura digital de estado
- Estrutura modular com bibliotecas separadas:
  - `ControlMotor`: encapsula o controle dos motores
  - `Cinematic`: realiza cálculos de cinemática direta/inversa

## 🔧 Requisitos

- Placa Arduino (ex: UNO)
- 3 motores de passo
- 3 drivers (A4988, DRV8825 ou similar)
- Fonte de alimentação adequada
- Alavanca (chave de dois estados)
- Biblioteca AccelStepper

## 🚀 Como Usar
