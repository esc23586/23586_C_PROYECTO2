/*
 * uart.c
 *
 * Created: 28/04/2026 14:25:30
 *  Author: grett
 */ 
//========================== UART.c ==========================//

#define F_CPU 16000000UL

#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <string.h>

// PWM
#include "../PWMm/PWM.h"
#include "../PWM1F/PWM1.h"
#include "../PWM2F/PWM2.h"
//meterme a una carpeta desde otra carpeta ..

//================ Variables Globales ================//

volatile uint8_t modo = 0;
volatile uint8_t commandReady = 0;

volatile uint8_t idx = 0;

char buffer[20];
char out[10];

//================ Inicialización UART ================//

void UART_Init(void)
{
	// RX entrada
	DDRD &= ~(1 << DDD0);

	// TX salida
	DDRD |= (1 << DDD1);

	UCSR0A = 0;

	// RX + TX + interrupción RX
	UCSR0B = (1 << RXCIE0) |
	(1 << RXEN0)  |
	(1 << TXEN0);

	// 8 bits
	UCSR0C = (1 << UCSZ01) |
	(1 << UCSZ00);

	// Baudrate 9600
	UBRR0 = 103;
}

//================ UART Básico ================//

void writeChar(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));

	UDR0 = c;
}

void writeString(char *string)
{
	uint8_t i = 0;

	while (string[i] != '\0')
	{
		writeChar(string[i]);
		i++;
	}
}

//================ Menú ================//

void printMenu(void)
{
	writeString("\r\n");
	writeString("======= MENU ANIMATRONICO =======\r\n");
	writeString("1 -> Control de Ojos UART\r\n");
	writeString("2 -> Control de Parpados UART\r\n");
	writeString("3 -> Leer EEPROM\r\n");
	writeString("4 -> Control Manual ADC\r\n");
	writeString("=================================\r\n");
	writeString("> ");
}

//================ Conversión Servos ================//
/*

uint16_t ServoToOCR(uint8_t ang)
{
	return 2000 + ((uint32_t)ang * 2000) / 180;
}

uint8_t ServoToTicks(uint8_t ang)
{
	return 8 + ((uint16_t)ang * 8) / 180;
}
*/

//================ Procesamiento Comandos ================//

void processCommand(void)
{
	//================ MODO 0 ================//

	if (modo == 0)
	{
		if (strcmp(buffer, "1") == 0)
		{
			modo = 1;

			writeString("\r\nModo OJOS\r\n");
			writeString("I1:angulo\r\n");
			writeString("D1:angulo\r\n");
			writeString("I2:angulo\r\n");
			writeString("D2:angulo\r\n");
			writeString("x para salir\r\n");
		}

		else if (strcmp(buffer, "2") == 0)
		{
			modo = 2;

			writeString("\r\nModo PARPADOS\r\n");
			writeString("M1:angulo\r\n");
			writeString("M2:angulo\r\n");
			writeString("x para salir\r\n");
		}

		else if (strcmp(buffer, "3") == 0)
		{
			writeString("Leyendo EEPROM...\r\n");
		}
		
		else if (strcmp(buffer, "4") == 0)
		{
			modo = 3;

			writeString("Modo ADC Manual\r\n");
			writeString("Potenciometros activos\r\n");
			writeString("x; para salir\r\n");
		}
		
		
		else
		{
			writeString("Opcion invalida\r\n");
			printMenu();
		}
	}

	//================ MODO OJOS ================//

	else if (modo == 1)
	{
		if (strncmp(buffer, "I1:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			PWM1_SetDuty(ServoToOCR(ang));

			writeString("I1 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "D1:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			PWM1_SetDuty2(ServoToOCR(ang));

			writeString("D1 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "I2:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			PWM0_SetDuty1(ServoToTicks(ang));

			writeString("I2 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "D2:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			PWM0_SetDuty2(ServoToTicks(ang));

			writeString("D2 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strcmp(buffer, "x") == 0)
		{
			modo = 0;

			printMenu();
		}

		else
		{
			writeString("Comando invalido\r\n");
		}
	}

	//================ MODO PARPADOS ================//

	else if (modo == 2)
	{
		if (strncmp(buffer, "M1:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			PWM2_SetDuty1(ServoToTicks(ang));

			writeString("M1 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "M2:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			PWM2_SetDuty2(ServoToTicks(ang));

			writeString("M2 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strcmp(buffer, "x") == 0)
		{
			modo = 0;

			printMenu();
		}

		else
		{
			writeString("Comando invalido\r\n");
		}
	}
	
	else if (modo == 3)
	{
		if (strcmp(buffer, "x") == 0)
		{
			modo = 0;

			writeString("Saliendo modo ADC\r\n");

			printMenu();
		}
	}
}

//================ ISR UART ===================================================================================//

ISR(USART_RX_vect)
{
	char c = UDR0;

	writeChar(c);

	// Delimitador
	if (c == ';')
	{
		buffer[idx] = '\0';

		idx = 0;

		commandReady = 1;
	}

	else
	{
		if (idx < sizeof(buffer) - 1)
		{
			buffer[idx++] = c;
		}
	}
}