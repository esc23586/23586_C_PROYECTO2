/*
 * PWM2.c
 *
 * Created: 5/05/2026 13:09:51
 *  Author: grett
 */ 
#include "PWM2.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t servo1_ticks = 15; // PD3
volatile uint8_t servo2_ticks = 15; // PB3

volatile uint8_t counter = 0;

void PWM2_Init(void)
{
	// Configurar PD3 y PB3 como salida
	DDRD |= (1 << PD3);
	DDRB |= (1 << PB3);

	// Timer2 normal mode
	TCCR2A = 0;
	TCCR2B = (1 << CS22); // prescaler 64

	TIMSK2 = (1 << TOIE2); // habilitar overflow

	sei();
}

ISR(TIMER2_OVF_vect)
{
	counter++;

	// Inicio del pulso (ambos en alto)
	if (counter == 1)
	{
		PORTD |= (1 << PD3);
		PORTB |= (1 << PB3);
	}

	// Apagar servo 1 (PD3)
	if (counter == servo1_ticks)
	{
		PORTD &= ~(1 << PD3);
	}

	// Apagar servo 2 (PB3)
	if (counter == servo2_ticks)
	{
		PORTB &= ~(1 << PB3);
	}

	// Reinicio del frame (~20ms)
	if (counter >= 156)
	{
		counter = 0;
	}
}

void PWM2_SetDuty1(uint8_t ticks)
{
	servo1_ticks = ticks;
}

void PWM2_SetDuty2(uint8_t ticks)
{
	servo2_ticks = ticks;
}