/*
 * PWM.c
 *
 * Created: 28/04/2026 12:47:30
 *  Author: grett
 */ 
#include "PWM.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t pwm_counter = 0;
volatile uint8_t pwm_duty = 0;

void PWM3_Init(void)
{
	// Pin de salida (ej: PB4)
	DDRB |= (1 << PB4);

	// Timer0 en modo normal
	TCCR0A = 0;
	TCCR0B = (1 << CS01); // prescaler 8

	// Habilitar interrupción por overflow
	TIMSK0 |= (1 << TOIE0);

	sei(); // habilitar interrupciones globales
}

void PWM3_SetDuty(uint8_t duty)
{
	pwm_duty = duty;
}

// ISR del Timer0
ISR(TIMER0_OVF_vect) //cada vez que pasa de 255 a 0
{
	pwm_counter++;

	if (pwm_counter == 0)
	{
		PORTB |= (1 << PB4); // ON
	}

	if (pwm_counter == pwm_duty) //cuando llega al valor cae
	{
		PORTB &= ~(1 << PB4); // OFF
	}
}