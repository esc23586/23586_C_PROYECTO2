/*
 * ServoUtils.c
 *
 * Created: 12/05/2026 13:06:53
 *  Author: grett
 */ 

/*
Se separó estas funocnes de la carpeta Uart, esto debido a que el main 
también necesita realizar la conversión para el movimiento de los servomotores. 
en general:Convierten ángulo de servo (0–180) a los valores que necesitan looos timers/PWM.

Por ello se opto que fuera fuera una librería paarte para que ambas partes la pueden utilizar cuando la necesiten. 
*/
#define F_CPU 16000000UL
#include <avr/io.h>

uint16_t ServoToOCR(uint8_t ang)
{
	return 2000 + ((uint32_t)ang * 2000) / 180;
	//Esta función convierte el ángulo a un valor para un registro OCR.
}


uint16_t ServoToTicks(uint8_t ang)
{
	return 50 + ((uint32_t)ang * 50) / 180;
	//misma idea pero aplicable para pwm0 y pwm2. (como timer uno es de 16 se hizo una para cada uno)
}

uint8_t ADCToAngle(uint16_t adc)
{
	return (adc * 180UL) / 1023;
}