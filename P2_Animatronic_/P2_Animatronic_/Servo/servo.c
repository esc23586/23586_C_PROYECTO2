/*
 * servo.c
 *
 * Created: 28/04/2026 13:13:03
 *  Author: grett
 */ 
#include "servo.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// ================= VARIABLES INTERNAS =================

volatile uint16_t servo_ticks[NUM_SERVOS];
volatile uint8_t servo_index = 0;

static uint8_t servo_pins[NUM_SERVOS] = {
	(1<<PORTD2), // I1
	(1<<PORTD3), // D1
	(1<<PORTD4), // I2
	(1<<PORTD5), // D2
	(1<<PORTD6), // M1
	(1<<PORTD7)  // M2
};

// ================= FUNCIONES PRIVADAS =================

// convertir ángulo a ticks
static uint16_t Servo_Map(uint8_t ang)
{
	return 2000 + ((uint32_t)ang * 2000) / 180;
}

// ================= FUNCIONES PÚBLICAS =================

void Servo_Init(void)
{
	// Pines como salida
	DDRD |= (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);

	// Timer1 CTC
	TCCR1A = 0;
	TCCR1B = (1<<WGM12) | (1<<CS11); // prescaler 8

	OCR1A = 3000;
	TIMSK1 |= (1<<OCIE1A);
	// Revisar el cambio
	for (uint8_t i = 0; i < NUM_SERVOS; i++)
	{
		servo_ticks[i] = 3000; // ~1.5 ms (posición neutra)
	}
	//
}

void Servo_Set(uint8_t id, uint8_t ang)
{
	if (id < NUM_SERVOS)
	{
		servo_ticks[id] = Servo_Map(ang);
	}
}

// ================= ISR =================

ISR(TIMER1_COMPA_vect)
{
	static uint8_t estado = 0;

	if (estado == 0)
	{
		PORTD |= servo_pins[servo_index];
		OCR1A = servo_ticks[servo_index];
		estado = 1;
	}
	else
	{
		PORTD &= ~servo_pins[servo_index];

		servo_index++;

		if (servo_index >= NUM_SERVOS)
		{
			servo_index = 0;
			OCR1A = 40000; // completar ciclo
		}
		else
		{
			OCR1A = 2000;
		}

		estado = 0;
	}
}