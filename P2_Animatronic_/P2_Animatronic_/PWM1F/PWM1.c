/*
 * PWM1.c
 *
 * Created: 28/04/2026 12:39:28
 *  Author: grett
 */ 

#include "PWM1.h"
#include <avr/io.h>



void PWM1_Init(void)
{
	// PB1 (OC1A) y PB2 (OC1B) como salida
	DDRB |= (1 << PB1) | (1 << PB2);

	// Fast PWM, TOP = ICR1
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);//activa salida PWM en OC1A
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);// modo Fast PWM con ICR1 y cs11 prescaler = 8

	// 50 Hz aproximadamente da 20ms
	ICR1 = 39999;

	// Centro inicial Para ambos
	OCR1A = 3000;
	OCR1B = 3000;
}
void PWM1_SetDuty(uint16_t duty)
{
	OCR1A = duty;//mov, servo
}

void PWM2_SetDuty(uint16_t duty)
{
	OCR1B = duty;
}