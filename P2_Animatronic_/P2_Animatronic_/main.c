/*
 * P2_Animatronic.c
 *
 * Created: 28/04/2026
 * Author: Grettel Escobedo 23586
 
 * Description del Proyecto: 
 
 Para la realización de este proyecto se tomará como base la propuesta llamada Rostro animatrónico.
 *Consiste en la fabricación de un animatrónico que pueda duplicar el funcionamiento de los ojos humanos. 
 * Este movimiento ocular está dividido en dos ejes principales,que permiten moverse de derecha a izquierda, arriba y abajo. 
 *Esto será realizado con ayuda de tres diferentes ejes impresos en PLA. 
 
 Modificación: 
 *Sin embargo, se cambiará los servomotores que están destinados a las cejas,
 * por unos encargados de realizar un movimiento igual al de un parpado móvil.
 
 Librerías/MODULOS: 
 ADC: Lectura de los 6 potenciometros
 PWM: Tranformación de lectura en un pulso cuadrado y a servo
	- se trabajará con pwm manual por ahora debido a la cantidad de servomotores
	
 UART: Comunicación a la temrinal y de esta a los servomotores.
 EEPROM:
 ADAFRUT:
 
 */

/****************************************/
// Encabezado (Libraries)

#include <avr/io.h>

/****************************************/
// Function prototypes
/****************************************/
// Main Function
int main(void)
{
	/* Replace with your application code */
	while (1)
	{
	}
}
/****************************************/
// NON-Interrupt subroutines
/****************************************/
// Interrupt routines