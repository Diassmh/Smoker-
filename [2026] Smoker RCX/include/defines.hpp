//===============================================================================================//
//=====================================//INCLUDES E DEFINES//====================================//
//===============================================================================================//

#pragma region INCLUDES

#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>
#include <ESP32Servo.h>

#pragma endregion

//=====================================//Objetos e Handles//=====================================//

//=====================================//Objetos e Handles//=====================================//

#pragma region OBJETOS E HANDLES

Servo servo1;
Servo servo2;

#pragma endregion

//===============================================================================================//
//=====================================//PINOS E VARIAVEIS//=====================================//
//===============================================================================================//

//==========================================//Motores//==========================================//

#pragma region PINOS MOTORES

// Remapeados exclusivamente para o ESP32-C3 Super Mini
#define Motor_Direito_Positivo 0
#define Motor_Direito_Negativo 1
#define Motor_Esquerdo_Positivo 3
#define Motor_Esquerdo_Negativo 4

#pragma endregion

//========================================//Servomotor//========================================//

#pragma region PINOS SERVO

#define SERVOMOTOR_PIN_1 20
#define SERVOMOTOR_PIN_2 21

// Ângulo de inicialização (quando o robô é ligado)
#define SERVO_ANGULO_INICIAL 90

// Ângulos quando o robô está DESARMADO (Fechado pelo controle)
#define SERVO1_DESARMADO 0
#define SERVO2_DESARMADO 180

// Ângulos quando o robô está ARMADO (Aberto pelo controle) - Ajuste conforme a sua mecânica
#define SERVO1_ARMADO 180
#define SERVO2_ARMADO 0

#define SERVO_FREQ_HZ 50
#define SERVO_RESOLUTION 16
#define SERVO_MIN_PULSE_US 544
#define SERVO_MAX_PULSE_US 2400

#pragma endregion

//========================================//Comunicacao//========================================//

#pragma region COMUNICACAO

#pragma endregion

//======================================//Enums//======================================//

#pragma region ENUMS

enum ServoMotor
{
  Fechado,
  Aberto
};

#pragma endregion

//===================================//Variaveis utilizadas//====================================//

#pragma region VARIAVEIS

ServoMotor servo = Fechado;

#pragma endregion

#endif