/*
 * PWM1.c
 *
 * Created: 28/04/2026 12:39:28
 *  Author: grett
 */ 

#include "PWM1.h"
#include <avr/io.h>

/*
En este caso este pwm se eligió para el movimiento de ambas bases de los ojos. 
I1:Ojo izquierdo base
D1:Ojo derecho base
*/

void PWM1_Init(void)
{
	// PB1 (OC1A) y PB2 (OC1B) como salida
	DDRB |= (1 << PORTB1) | (1 << PORTB2);

	// Fast PWM, TOP = ICR1
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);//activa salida PWM en OC1A
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);// modo Fast PWM con ICR1 y cs11 prescaler = 8

	// 50 Hz aproximadamente da 20ms
	ICR1 = 39999;

	// Centro inicial Para ambos
	/*
	Nota
	Basicamente inician en 45 graados. Puedo variar esto según convenga una vez ensamblado. 
	Para que quede en el grado necesario para que se vea como mi 0. 
	
	3000 es 45 y 4000 es 180, 2000 es 0 
	*/
	
	OCR1A = 3000;
	OCR1B = 3000;
}
void PWM1_SetDuty(uint16_t duty)
{
	OCR1A = duty;//mov, servo
}


void PWM1_SetDuty2(uint16_t duty)
{
	OCR1B = duty;
}