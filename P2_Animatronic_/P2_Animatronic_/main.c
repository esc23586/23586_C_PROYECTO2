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
		 
		 UART: Comunicación a la temrinal y de esta a los servomotores./Se podrá elegir esos 4 modos igualmente y un 5 para seleccionar servomotor y su grado.
		 EEPROM: Tendrá 4 estados o posiciones que puede grabar y reproducir desde la EEPROM.
		 ADAFRUT: Control de los ervomotores desde la página
 
Descripción de Modos: Se puede seleccionar entre los modos: manual, EEPROM y Adafruit
 
 */
//===========================================================================================================================================/


// Encabezado (Libraries)
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

//nuevos
#include <stdlib.h>   // para itoa
#include <string.h>
#include "ADC/adc.h" //para importar mi adc

// Para los pwm
#include "PWMm/PWM.h" //la del timer cero
#include "PWM1F/PWM1.h"// la que tiene al timer1
#include "PWM2F/PWM2.h"// la que ocupa el timer 2

//other essential
#include "UART/UART.h" //Menu logic, and comparisons :3
#include "ServoUtils/ServoUtils.h"// the functions to convert number in angles

//para guardar en eprom
#include "EEPROMSave/eepromsave.h" // se encarga solo de gaurdar
#include "ServoMemory/Servomemory.h"// se encarga de reproducir desde eprom 

/****************************************/



// Main Function
//***********************************  MAIN   **************************************//
int main(void)
{
	cli();
	//Entradas- y salidas declaración pb0 Y pd7 - LEDS DE ESTADO
	/*
	DDRB |= (1<<DDB0);
	PORTB &= ~(1<<PORTB0);
	DDRD |= (1 << DDD7);
	PORTD &= ~(1<<PORTD7)
	*/
	
	//inicializaciónes 
	UART_Init();//Comunicación UART
	PWM0_Init();//Control de Timer 0
	PWM1_Init();//Control de Timer 1
	PWM2_Init();//Control de Timer 2
	ADC_Init();//ADC
	EEPROM_Init();// guardar en eprom con boton pb5
	
	sei();// BUENAS CONSTUMBRES

	printMenu();
	
	//================ VARIABLES ADC =================//

	uint16_t adc0;
	uint16_t adc1;
	uint16_t adc2;
	uint16_t adc3;
	uint16_t adc4;
	uint16_t adc5;

	
	//***************************************************//
	
	//loop 
	while (1)
	{
		//============EEPROM GUARDADO ==========//
		/*
		if(saveFlag)
		{
			saveFlag = 0;
			// pequeño detalle, en teoria no es necesario pues ya es global y en teoria se puede leer directamente.
			// pero son detalles :3
			EEPROM_SaveServos(
			servoAng[0],
			servoAng[1],
			servoAng[2],
			servoAng[3],
			servoAng[4],
			servoAng[5]
			);
		}
		*/
		if(saveFlag)
		{
			saveFlag = 0;//apago la flag para que solo guarde una vez y sale

			EEPROM_SaveServos();
		}
		
		//================ UART =================//

		if (commandReady)
		{
			commandReady = 0;

			processCommand();
		}

		//================ CONTROL ADC =================//

		if (modo == 3)
		{
			//========= LEER ADC =========//

			adc0 = ADC_Read(2); // PC2
			adc1 = ADC_Read(3); // PC3
			adc2 = ADC_Read(4); // PC4
			adc3 = ADC_Read(5); // PC5
			adc4 = ADC_Read(6); // A6
			adc5 = ADC_Read(7); // A7

			//========= ADC para ANGULO =========// esto es lo de reproducción desde servo memory
			
			servoAng[0] = ADCToAngle(adc0);
			servoAng[1] = ADCToAngle(adc1);

			servoAng[2] = ADCToAngle(adc2);
			servoAng[3] = ADCToAngle(adc3);

			servoAng[4] = ADCToAngle(adc4);
			servoAng[5] = ADCToAngle(adc5);

			//========= aplicar el pwm =========// esto tambien va de la mano con la parte de eeprom

			ServoMemory_ApplyPWM();
		}
	}
}
		// las subrutinas de la rutina
		
		//la interrupción debe ser unicamente para levantar banderas. 


/**********************************************************************/
// Interrupt routines
			
//Lógica de rutina de interrupción: lo que se planea ver en la terminal y su interación:
	/**********************************
	*Primero: 
			pregunta el modo general
			1 ? modo ojos
			2 ? modo párpados
	*Si 1 fue presionado:
			CON 1-- los de labase del ojo
					I1:90 
					D1:45
			CON 2----Los de la pupila
					I2:30
					D2:120
					x ? salir
			
	*Si 2 fue preionado:
			M1:90
			M2:40
			x ? salir
	*/
	/********************************/
	

