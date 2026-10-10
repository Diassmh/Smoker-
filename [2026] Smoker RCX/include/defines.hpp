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
#define Motor_Direito_Positivo 4
#define Motor_Direito_Negativo 3
#define Motor_Esquerdo_Positivo 1
#define Motor_Esquerdo_Negativo 0

//========================================//Servomotor//========================================//

#pragma region PINOS SERVO

#define SERVOMOTOR_PIN_1 7
#define SERVOMOTOR_PIN_2 20

#define SERVO1_DESARMADO 180
#define SERVO2_DESARMADO 0

// Ângulos quando o robô está ARMADO (Aberto/Ativado pelo controle)
// Volta para a posição inicial de 180º
#define SERVO1_ARMADO 90 // 100
#define SERVO2_ARMADO 90 // 100

#define SERVO_FREQ_HZ 50
#define SERVO_RESOLUTION 16
#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500

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