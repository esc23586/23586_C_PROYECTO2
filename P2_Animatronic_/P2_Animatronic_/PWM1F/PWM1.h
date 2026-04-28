/*
 * PWM1.h
 *
 * Created: 28/04/2026 12:42:15
 *  Author: grett
 */ 


#ifndef PWM1_H_
#define PWM1_H_


#include <stdint.h>

// Inicializa el PWM usando Timer1 en PB1 (OC1A)
void PWM1_Init(void);

// Ajusta el duty cycle del PWM
void PWM1_SetDuty(uint16_t duty); //Cambia el ancho del pulso, basicamente mueve el servo, revisar***

// Ajusta el duty cycle para el segundo servo (OC1B)
void PWM2_SetDuty(uint16_t duty);


#endif /* PWM1_H_ */