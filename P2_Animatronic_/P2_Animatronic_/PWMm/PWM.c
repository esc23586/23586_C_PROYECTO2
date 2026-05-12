/*
 * PWM.c
 *
 * Created: 28/04/2026 12:47:30
 *  Author: grett
 */ 
#include "PWM.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t servo1A_ticks = 15; // PD6
volatile uint8_t servo1B_ticks = 15; // PD5

volatile uint8_t counter0 = 0;

void PWM0_Init(void)
{
	// Configurar PD6 y PD5 como salida
	DDRD |= (1 << DDD6)| (1 << DDD5);

	// Timer0 en modo normal
	TCCR0A = 0;
	TCCR0B = (1 << CS01) | (1 << CS00); // prescaler 64

	TIMSK0 = (1 << TOIE0); // habilitar overflow

	//sei();
}

ISR(TIMER0_OVF_vect)
{
	counter0++;

	// Inicio del pulso
	if (counter0 == 1)
	{
		PORTD |= (1 << PORTD6) | (1 << PORTD5);
	}

	// Apagar servo 1 (PD6)
	if (counter0 == servo1A_ticks)
	{
		PORTD &= ~(1 << PORTD6);
	}

	// Apagar servo 2 (PD5)
	if (counter0 == servo1B_ticks)
	{
		PORTD &= ~(1 << PORTD5);
	}

	// Frame ~20ms
	if (counter0 >= 156)
	{
		counter0 = 0;
	}
}

void PWM0_SetDuty1(uint8_t ticks)
{
	servo1A_ticks = ticks;
}

void PWM0_SetDuty2(uint8_t ticks)
{
	servo1B_ticks = ticks;
}