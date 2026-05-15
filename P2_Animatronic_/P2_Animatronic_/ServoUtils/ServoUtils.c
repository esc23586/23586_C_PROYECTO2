/*
 * ServoUtils.c
 *
 * Created: 12/05/2026 13:06:53
 *  Author: grett
 */ 

/*
Se separó estas funocnes de la carpeta Uart, esto debido a que el main 
también necesita realizar la conversión para el movimiento de los servomotores. 

Por ello se opto que fuera fuera una librería paarte para que ambas partes la pueden utilizar cuando la necesiten. 
*/
#define F_CPU 16000000UL
#include <avr/io.h>
/*
Hay un movimiento poco refinado en los servomotores, por ello se tratará de arreglar cambiando el valor de 2000 
*/
uint16_t ServoToOCR(uint8_t ang)
{
	return 2000 + ((uint32_t)ang * 2000) / 180;
}

/*
uint8_t ServoToTicks(uint8_t ang)
{
	return 8 + ((uint16_t)ang * 8) / 180;
}
*/

uint16_t ServoToTicks(uint8_t ang)
{
	return 50 + ((uint32_t)ang * 50) / 180;
}

uint8_t ADCToAngle(uint16_t adc)
{
	return (adc * 180UL) / 1023;
}