/*
 * servo.h
 *
 * Created: 28/04/2026 13:12:39
 *  Author: grett
 */ 

/**
 * Con brief  se mueve un servo a un ángulo específico
 *angulo 0–180 grados
 */
#ifndef SERVO_H_
#define SERVO_H_

#include <avr/io.h>
#include <stdint.h>

#define NUM_SERVOS 6

// Inicializar sistema de servos
void Servo_Init(void);

// Mover un servo específico (0–5)
void Servo_Set(uint8_t id, uint8_t angulo);


#endif /* SERVO_H_ */