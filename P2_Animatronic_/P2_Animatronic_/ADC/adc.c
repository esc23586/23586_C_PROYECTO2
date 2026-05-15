/*
 * adc.c
 *
 * Created: 28/04/2026 11:40:01
 *  Author: grett
 */ 
#include "adc.h"
#include <avr/io.h>

void ADC_Init(void)
{
	// Referencia AVcc (5V) para mi nano
	ADMUX = (1 << REFS0);

	// Habilitar ADC + prescaler 128 (~125 kHz)
	ADCSRA = (1 << ADEN) |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read(uint8_t channel)
{
	// Limitar canal (0–7)
	channel &= 0x07;

	// Mantener referencia y ajustar canal
	ADMUX = (ADMUX & 0xF8) | channel;

	// Iniciar conversión
	ADCSRA |= (1 << ADSC);

	// Esperar fin
	while (ADCSRA & (1 << ADSC));

	return ADC;
}