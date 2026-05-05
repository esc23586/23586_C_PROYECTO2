/*
 * PWM.c
 *
 * Created: 28/04/2026 12:47:30
 *  Author: grett
 */ 
#include "PWM.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t servo1_ticks = 15; // PD6
volatile uint8_t servo2_ticks = 15; // PD5

volatile uint8_t counter = 0;

void PWM0_Init(void)
{
	// Configurar PD6 y PD5 como salida
	DDRD |= (1 << PD6) | (1 << PD5);

	// Timer0 en modo normal
	TCCR0A = 0;
	TCCR0B = (1 << CS01) | (1 << CS00); // prescaler 64

	TIMSK0 = (1 << TOIE0); // habilitar overflow

	sei();
}

ISR(TIMER0_OVF_vect)
{
	counter++;

	// Inicio del pulso
	if (counter == 1)
	{
		PORTD |= (1 << PD6) | (1 << PD5);
	}

	// Apagar servo 1 (PD6)
	if (counter == servo1_ticks)
	{
		PORTD &= ~(1 << PD6);
	}

	// Apagar servo 2 (PD5)
	if (counter == servo2_ticks)
	{
		PORTD &= ~(1 << PD5);
	}

	// Frame ~20ms
	if (counter >= 156)
	{
		counter = 0;
	}
}

void PWM0_SetDuty1(uint8_t ticks)
{
	servo1_ticks = ticks;
}

void PWM0_SetDuty2(uint8_t ticks)
{
	servo2_ticks = ticks;
}