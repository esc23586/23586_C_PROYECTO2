/*
 * eepromsave.c
 *
 * Created: 16/05/2026 17:11:59
 *  Author: grett
 */ 
#define F_CPU 16000000UL
#include "eepromsave.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "../ServoMemory/Servomemory.h"// la otra librería que ayuda con las globales. 
//basicamente acceso aaaa estop : servoAng[].. etc


//================ BANDERA =================//

volatile uint8_t saveFlag = 0;
// La ""bandera"" está en cero hasta que se presióna PB5, entonces la rutina de interrupción la cambia a 1

//================ INIT =================//

void EEPROM_Init(void)
{
	//mini declaración de entrada
	
	//=== Leds de estado
	DDRB |= (1 << DDB0);
	DDRD |= (1 << DDD7);

	PORTB &= ~(1 << PORTB0);
	PORTD &= ~(1 << PORTD7);
	
	// PB5 entrada
	DDRB &= ~(1 << DDB5);
	
	// Pull-up
	PORTB |= (1 << PORTB5);
	// Pin Change Interrupt
	PCICR |= (1 << PCIE0);

	// PB5
	PCMSK0 |= (1 << PCINT5);
}

//================ ISR =================//
//rutina de interrúpción para el botonazo. 
ISR(PCINT0_vect)
{
	
	if (!(PINB & (1 << PINB5)))
	{
		_delay_ms(20);

		if (!(PINB & (1 << PINB5)))
		{
			saveFlag = 1;
		}
	}
}

//================ SAVE =================//
void EEPROM_SaveServos(void)
{
	
	//================ ENCENDER LEDS =================//
//leds de estado
	PORTB |= (1 << PORTB0);
	PORTD |= (1 << PORTD7);
	
	// las cocalidades de la 24 a 29 se utilizarán para la modificación manual de los potenciometros y su posterior guardado en eeprom. 
	//Esto solo guarda lo de 1 combinación de posiciones. Seria la combinación nuemero 5 que aparece en el menú de UART. 
	eeprom_update_byte((uint8_t*)24, servoAng[0]);
	eeprom_update_byte((uint8_t*)25, servoAng[1]);

	eeprom_update_byte((uint8_t*)26, servoAng[2]);
	eeprom_update_byte((uint8_t*)27, servoAng[3]);

	eeprom_update_byte((uint8_t*)28, servoAng[4]);
	eeprom_update_byte((uint8_t*)29, servoAng[5]);
	//luego de guardar
	//================ APAGAR LEDS =================//

	PORTB &= ~(1 << PORTB0);
	PORTD &= ~(1 << PORTD7);
}