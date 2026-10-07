/*/=========================================================================================================
===========================================================================================================
=====================================     //    SMOKER RC   //    =============================================
==========================================================================================================
========================================================================================================*/

#pragma region Includes

#include "defines.hpp"
#include "modo_rc.hpp"
#include "move.hpp"

// ======================================|| CONFIG ||=======================================
#pragma region Config

void config()
{
    // --- MOTORES ---
    pinMode(Motor_Esquerdo_Positivo, OUTPUT);
    pinMode(Motor_Esquerdo_Negativo, OUTPUT);
    pinMode(Motor_Direito_Positivo, OUTPUT);
    pinMode(Motor_Direito_Negativo, OUTPUT);

    // --- SERVO 1 ---
    servo1.setPeriodHertz(SERVO_FREQ_HZ);
    servo1.attach(SERVOMOTOR_PIN_1, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
    servo1.write(SERVO_ANGULO_INICIAL); // Inicia em 90º ao ligar

    // --- SERVO 2 ---
    servo2.setPeriodHertz(SERVO_FREQ_HZ);
    servo2.attach(SERVOMOTOR_PIN_2, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
    servo2.write(SERVO_ANGULO_INICIAL); // Inicia em 90º ao ligar
}

#pragma endregion

// ==========================================================
// SETUP PRINCIPAL
// ==========================================================

#pragma region Setup MAIN

void setup()
{
    Serial.begin(115200);
    config();

    // Inicia a configuração do Bluetooth BLE do comando Xbox imediatamente
    setupRC();
}

#pragma endregion

// ==========================================================
// LOOP PRINCIPAL
// ==========================================================

#pragma region Loop MAIN

void loop()
{
    // Mantém o robô focado apenas na leitura do comando
    loopRC();
}
#pragma endregion