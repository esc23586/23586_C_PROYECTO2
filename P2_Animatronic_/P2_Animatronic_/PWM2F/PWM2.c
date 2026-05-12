/*
 * PWM2.c
 *
 * Created: 5/05/2026 13:09:51
 *  Author: grett
 */ 
#include "PWM2.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t servo2A_ticks = 15; // PD3
volatile uint8_t servo2B_ticks = 15; // PB3

volatile uint8_t counter2 = 0;

void PWM2_Init(void)
{
	// Configurar PD3 y PB3 como salida
	
	DDRD |= (1 << DDD3);
	DDRB |= (1 << PORTB3);
	
	

	// Timer2 normal mode
	TCCR2A = 0;
	TCCR2B = (1 << CS22); // prescaler 64

	TIMSK2 = (1 << TOIE2); // habilitar overflow

	//sei();
}

ISR(TIMER2_OVF_vect)
{
	counter2++;

	// Inicio del pulso (ambos en alto)
	if (counter2 == 1)
	{
		PORTD |= (1 << PORTD3);
		PORTB |= (1 << PORTB3);
	}

	// Apagar servo 1 (PD3)
	if (counter2 == servo2A_ticks)
	{
		PORTD &= ~(1 << PORTD3);
	}

	// Apagar servo 2 (PB3)
	if (counter2 == servo2B_ticks)
	{
		PORTB &= ~(1 << PORTB3);
	}

	// Reinicio del frame (~20ms)
	if (counter2 >= 156)
	{
		counter2 = 0;
	}
}

void PWM2_SetDuty1(uint8_t ticks)
{
	servo2A_ticks = ticks;
}

void PWM2_SetDuty2(uint8_t ticks)
{
	servo2B_ticks = ticks;
}