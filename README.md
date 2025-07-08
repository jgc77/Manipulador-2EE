# Manipulador RRR - Arduino

Este repositório contém o projeto de um **manipulador robótico com 3 graus de liberdade (RRR) Antropomórfico**, controlado por motores de passo e programado em Arduino. O sistema executa movimentos baseados em cálculos de **cinemática direta e inversa**, e pode ser operado via interface serial. A arquitetura do projeto é modular, com bibliotecas dedicadas ao controle dos motores e aos cálculos de posição.

## 🎓 Finalidade

Este trabalho foi desenvolvido como parte do **Projeto 2EE de Robótica** do curso de **Engenharia de Controle e Automação** da **Universidade de Pernambuco (UPE)**.

## 📁 Estrutura do Repositório

- **/Main**: Código principal que executa o controle do manipulador e interpreta os comandos recebidos via serial.
- **/ControlMotor**: Biblioteca para controle dos motores de passo com base na biblioteca AccelStepper.
- **/Cinematic**: Biblioteca responsável pelos cálculos de cinemática direta e inversa do braço robótico.
- **/Docs**: Documentação Técnica.
  
## 🔧 Componentes e Requisitos

- **Placa Arduino** (ex: Uno, Mega ou similar)
- **3 motores de passo**
- **3 drivers** (A4988, DRV8825 ou compatíveis)
- **Fonte de alimentação** adequada para os motores
- **Chave seletora** (leitura digital para definir o modo de operação)
- **Jumpers, protoboard ou shield**
- **Biblioteca AccelStepper** instalada na IDE do Arduino
> **[https://github.com/jgc77/Manipulador-2EE/blob/main/Docs/Diagrama%20El%C3%A9trico.pdf]**

## ⚙️ Modos de Operação

- **Cinemática Direta**  
  Entrada via serial dos **ângulos das juntas** (base, ombro e cotovelo).  
  O sistema calcula a posição final do atuador e movo os motores para a posição.

- **Cinemática Inversa**  
  Entrada via serial da **posição desejada do atuador (X, Y, Z)**.  
  O sistema calcula os ângulos das juntas e move os motores para a posição.

- **Seletor de Modo**  
  Utiliza uma chave de 2 estados (digital HIGH/LOW) para alternar entre os modos direto e inverso.

- **Calibração**  
  Digitando a palavra "calib" no serial, o sistema entra em moto calibração manual, onde podemos movimentar o braço para os angulos 0,0,0 e digitar "exit" para       salvar a calibração.

## 💻 Interface Serial

A comunicação com o manipulador é feita por meio do monitor serial da IDE Arduino ou outro terminal serial compatível. Os comandos seguem os seguintes formatos:

### 📤 Cinemática Direta
```
>> Modo selecionado: Cinemática Direta
Digite os ângulos da base, ombro e cotovelo (ex: 90 -45 30)
```

### 📤 Cinemática Inversa
```
>> Modo selecionado: Cinemática Inversa
Digite a posição desejada do atuador no formato X Y Z (ex: 10 5 8)
```

## 📌 Portas Utilizadas
- **D02**: Chave Gangorra
- **D04**: STP - Drive 1
- **D05**: DIR - Drive 1  
- **D06**: STP - Drive 2
- **D07**: DIR - Drive 2
- **D08**: STP - Drive 3 
- **D09**: DIR - Drive 3
- **D10**: ENABLE - Drive 1 
- **D11**: ENABLE - Drive 2 
- **D12**: ENABLE - Drive 3

## 🗂️ Instruções de Instalação

1. Copie as pastas **ControlMotor** e **Cinematic** para a pasta `libraries` do Arduino.
2. Abra o sketch principal em **/Main** e carregue para a placa Arduino.
3. Edite o código conforme necessário, ajustando os parâmetros do motor (passos por volta, velocidade, aceleração) de acordo com o seu hardware.
