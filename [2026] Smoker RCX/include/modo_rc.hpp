//===============================================================================================//
//======================================// MODO RC //============================================//
//===============================================================================================//

#ifndef MODO_RC_HPP
#define MODO_RC_HPP

#pragma region INCLUDES

#include "defines.hpp"
#include "move.hpp"
#include <XboxSeriesXControllerESP32_asukiaaa.hpp> // Substituída a biblioteca PS4

#pragma endregion

int zonaMorta = 20;                // Zona desconsiderada do controlo (já mapeada para 0-255)
volatile int r2 = 0;               // Valor do gatilho direito da Xbox (RT)
volatile int l2 = 0;               // Valor do gatilho esquerdo da Xbox (LT)
volatile int direcao = 0;          // Valor do direcional esquerdo da Xbox em X (LHori)
volatile bool velLimitada = false; // Switch definido pra limitar a velocidade do motor
int limiteVelocidade = 255;        // Limite de velocidade do motor

bool ativarMira = false;
bool primeiroComando = true;    // Flag para abrir o servo no primeiro movimento
bool estadoServoFechado = true; // Controla se o robô está fechado (95º) ou aberto (0º)

// Variáveis para desligar o servo sem travar o robô
unsigned long tempoMovimentoServo = 0;
bool servoPrecisaDesligar = false;

float coefAtenuacao = 1.8; // Atenuacao da velocidade em curvas (deve ser FLOAT)
float coefReverse = 0.9;   // Coeficiente para balancear a re (deve ser FLOAT)
float limiteCurva = 120.0; // Velocidade limite em curvas puras (deve ser FLOAT)  120

const char *SMOKER = "d8:3b:da:13:62:d8";

// Instância global do controlo da Xbox passando o MAC Address desejado
XboxSeriesXControllerESP32_asukiaaa::Core xbox;

#pragma region Setup RC

void setupRC()
{
    xbox.begin(); // Inicia o Bluetooth para a Xbox

    while (!xbox.isConnected())
    {
        xbox.onLoop(); // CRÍTICO: Necessário para processar a conexão BLE

        vTaskDelay(pdMS_TO_TICKS(5)); // Pausa mínima para o FreeRTOS respirar
    }
}

#pragma endregion

#pragma region Loop RC

void loopRC()
{

    while (true)
    {
        xbox.onLoop(); // CRÍTICO: Deve ser chamado a cada iteração para ler os inputs

        if (xbox.isConnected())
        {
            int velocidadeEsquerda = 0; // Velocidade do motor esquerdo
            int velocidadeDireita = 0;  // Velocidade do motor direito

            // Lemos as entradas analógicas e mapeamos para a escala antiga do PS4 para não quebrar o código
            r2 = map(xbox.xboxNotif.trigRT, 0, 1023, 0, 255);            // Gatilho Direito (RT)
            l2 = map(xbox.xboxNotif.trigLT, 0, 1023, 0, 255);            // Gatilho Esquerdo (LT)
            direcao = map(xbox.xboxNotif.joyLHori, 0, 65535, -128, 127); // Eixo X do Analógico Esquerdo

            // Atalhos para os botões da Xbox
            bool btnA = xbox.xboxNotif.btnA;            // Equivalente ao Cross (X)
            bool btnX = xbox.xboxNotif.btnX;            // Equivalente ao Square (Quadrado)
            bool btnY = xbox.xboxNotif.btnY;            // Equivalente ao Triangle (Triângulo)
            bool btnLB = xbox.xboxNotif.btnLB;          // Equivalente ao L1
            bool btnRB = xbox.xboxNotif.btnRB;          // Equivalente ao R1
            bool btnUp = xbox.xboxNotif.btnDirUp;       // Seta Cima
            bool btnDown = xbox.xboxNotif.btnDirDown;   // Seta Baixo
            bool btnLeft = xbox.xboxNotif.btnDirLeft;   // Seta Esquerda
            bool btnRight = xbox.xboxNotif.btnDirRight; // Seta Direita

            // ======================= || FLAG PRIMEIRO COMANDO || =======================
            if (primeiroComando)
            {
                // Se detetar QUALQUER botão de macro, gatilho, curva ou frentão
                if (btnA || btnLB || btnRB || btnLeft || btnRight || btnUp || r2 > zonaMorta || l2 > zonaMorta || abs(direcao) > zonaMorta)
                {
                    // Religa o PWM caso estivesse desligado
                    if (!servo1.attached())
                        servo1.attach(SERVOMOTOR_PIN_1, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
                    if (!servo2.attached())
                        servo2.attach(SERVOMOTOR_PIN_2, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);

                    servo1.write(SERVO1_DESARMADO);
                    servo2.write(SERVO2_DESARMADO);
                    estadoServoFechado = false;
                    primeiroComando = false;

                    // Inicia a contagem para desligar o servo após a queda
                    tempoMovimentoServo = millis();
                    servoPrecisaDesligar = true;
                }
            }

            // ======================= || COMANDOS COM BOTÕES (MACROS) || ===================

            if (btnA)
            { // A = Frentão
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(50));
                continue;
            }

            else if (btnLB)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
                moverMotores(-255, 255);
                vTaskDelay(pdMS_TO_TICKS(120));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(300));
                moverMotores(0, 0);
                vTaskDelay(pdMS_TO_TICKS(10));
                moverMotores(255, -255);
                vTaskDelay(pdMS_TO_TICKS(160));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(200));
            }

            else if (btnRB)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
                moverMotores(255, -255);
                vTaskDelay(pdMS_TO_TICKS(120));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(300));
                moverMotores(0, 0);
                vTaskDelay(pdMS_TO_TICKS(10));
                moverMotores(-255, 255);
                vTaskDelay(pdMS_TO_TICKS(160));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(200));
            }

            else if (btnLeft)
            {
                moverMotores(-255, 255);
                vTaskDelay(pdMS_TO_TICKS(70));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(220));
                moverMotores(255, -255);
                vTaskDelay(pdMS_TO_TICKS(200));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(200));
                moverMotores(0, 0);
                vTaskDelay(pdMS_TO_TICKS(15));
            }
            else if (btnRight)
            {
                moverMotores(255, -255);
                vTaskDelay(pdMS_TO_TICKS(70));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(220));
                moverMotores(-255, 255);
                vTaskDelay(pdMS_TO_TICKS(200));
                moverMotores(255, 255);
                vTaskDelay(pdMS_TO_TICKS(200));
                moverMotores(0, 0);
                vTaskDelay(pdMS_TO_TICKS(15));
            }

            // ======================= || CONTROLE DO SERVO MANUAL || =======================
            else if (btnDown)
            {
                // Religa o PWM sempre que formos dar um comando manual
                if (!servo1.attached())
                    servo1.attach(SERVOMOTOR_PIN_1, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
                if (!servo2.attached())
                    servo2.attach(SERVOMOTOR_PIN_2, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);

                if (!estadoServoFechado)
                {
                    // ARMA OS SERVOS (Sobe para posição inicial)
                    servo1.write(SERVO1_ARMADO);
                    servo2.write(SERVO2_ARMADO);
                    estadoServoFechado = true;    // Marca que o robô está fechado/pronto
                    primeiroComando = true;       // REATIVA a função de cair no 1º movimento
                    servoPrecisaDesligar = false; // Cancela desligamentos, pois armado ele deve manter a força
                }
                else
                {
                    // DESARMA OS SERVOS (Cai)
                    servo1.write(SERVO1_DESARMADO);
                    servo2.write(SERVO2_DESARMADO);
                    estadoServoFechado = false; // Marca que o robô já abriu
                    primeiroComando = false;    // Desativa a flag (pois já caíram)

                    // Inicia a contagem para desligar o servo após a queda
                    tempoMovimentoServo = millis();
                    servoPrecisaDesligar = true;
                }
                moverMotores(0, 0);             // Mantém o robô parado ao alternar
                vTaskDelay(pdMS_TO_TICKS(300)); // Debounce essencial para não "piscar" os servos
                continue;
            }

            else if (btnUp)
            {
                moverMotores(0, 0);
                vTaskDelay(pdMS_TO_TICKS(25));
                moverMotores(255, -255);
                vTaskDelay(pdMS_TO_TICKS(140));
                moverMotores(0, 0);
                vTaskDelay(pdMS_TO_TICKS(10));
                continue;
            }
            else if (btnX && !ativarMira)
            {
                ativarMira = true;
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            else if (btnX)
            {
                ativarMira = false;
                vTaskDelay(pdMS_TO_TICKS(200));
            }

            // ======================= || LIMITADOR DE VELOCIDADE || =======================

            // Limita a velocidade a 127
            else if (btnY && !velLimitada)
            {
                velLimitada = true;
                limiteVelocidade = 160;         // Define o limite reduzido
                vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
            }

            // Permite a velocidade de 255
            else if (btnY)
            {
                velLimitada = false;
                limiteVelocidade = 255;         // Restaura o limite maximo
                vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
            }

            // ======================= || MOVIMENTAÇÃO LIVRE || ==========================

            else
            {
                //===================== || CURVAS PURAS || =============================

                if (r2 <= zonaMorta && l2 <= zonaMorta)
                {
                    velocidadeEsquerda = 0;
                    velocidadeDireita = 0;

                    if (direcao < -zonaMorta)
                    { // Curva pra esquerda parado
                        int velCurva = map(abs(direcao), zonaMorta, 128, 0, limiteCurva);
                        velocidadeEsquerda = -velCurva;
                        velocidadeDireita = velCurva;
                    }
                    else if (direcao > zonaMorta)
                    { // Curva pra direita parado
                        int velCurva = map(direcao, zonaMorta, 127, 0, limiteCurva);
                        velocidadeEsquerda = velCurva;
                        velocidadeDireita = -velCurva;
                    }
                }

                //===================== || GATILHOS PUROS || =============================

                else
                { // Pelo menos um gatilho pressionado (RT ou LT)

                    if (r2 > zonaMorta) // Gatilho direito (frente)
                    {

                        int velocidade = map(r2, zonaMorta, 255, 0, limiteVelocidade);
                        velocidadeEsquerda = velocidade;
                        velocidadeDireita = velocidade;
                    }
                    else
                    { // Gatilho esquerdo (re)
                        int velocidade = map(l2, zonaMorta, 255, 0, limiteVelocidade);
                        velocidadeEsquerda = -velocidade;
                        velocidadeDireita = -int(coefReverse * velocidade);
                    }

                    //===================== || GATILHOS + CURVA || =============================

                    // Aplica a curva sobre a velocidade existente

                    if (direcao < -zonaMorta)
                    { // Curva pra esquerda em movimento
                        // Reduz a velocidade da roda interna (esquerda para frente, direita para re)
                        if (r2 > zonaMorta)
                            velocidadeEsquerda *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                        else
                            velocidadeDireita *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                    }
                    else if (direcao > zonaMorta)
                    { // Curva pra direita em movimento
                        // Reduz a velocidade da roda interna (direita para frente, esquerda para re)
                        if (r2 > zonaMorta)
                            velocidadeDireita *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                        else
                            velocidadeEsquerda *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                    }
                }
            }

            // ======================= || DESLIGAMENTO AUTOMÁTICO DOS SERVOS || =======================
            // Se passaram 400ms desde que mandámos desarmar, cortamos o sinal para pararem de forçar
            if (servoPrecisaDesligar && (millis() - tempoMovimentoServo > 400))
            {
                servo1.detach();
                servo2.detach();
                servoPrecisaDesligar = false;
            }

            moverMotores(velocidadeEsquerda, velocidadeDireita);
        }

        else
        {
            moverMotores(0, 0); // Para os motores caso a conexão seja perdida
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

#pragma endregion

#endif