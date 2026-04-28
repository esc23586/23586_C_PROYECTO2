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
#include "ADC/adc.h" //para importar mi adc

/****************************************/
// Function prototypes

void intUART(void);
void writeChar(char c);
void writeString(char *string);
void writeNumber(uint8_t num);
void displayASCII(uint8_t value);//Cmabiar
void printMenu(void);
/****************************************/
// Main Function
volatile uint8_t modo = 0;//MODO Predeterminado
//Aqui puede ir la parte de la cara y sus 


//parte del Buffer
char buffer[10];

//***********************************  MAIN   **************************************//
int main(void)
{
	cli();
	//Entradas- y salidas declaración
	// LED en PB5 como salida --- va indicando el modo.
	DDRB |= (1<<DDB5);
	PORTB &= ~(1<<PORTB5); // inicialmente apagado
	
	//inicializaciónes 
	intUART();//Comunicación UART
	ADC_Init();//ADC
	//pwm too
	
	
	sei();// BUENAS CONSTUMBRES

	//imprimir el menu nuevamente 
	printMenu();/
	
	
	while (1)
	{
	}
}
/*****************************************************************************/
// NON-Interrupt subroutines

//para el menu
void printMenu(void)
{
	writeString("=========== Menu Animatrónico=============\r\n");
	writeString("1: Mover Ojos \r\n");
	writeString("2: Mover Parpados\r\n");
	writeString("==============================\r\n");
	writeString("\r\n");
	writeString("\r\n");
	PORTB &= ~(1<<PORTB5); // apagar LED
	//Apagar todos los demás leds de modo too
	
}

//************ Modulo  para Comunicación ***************//
// Inicialización UART
void intUART(void)
{
	// RX (PD0) entrada
	DDRD &= ~(1<<DDD0);

	// TX (PD1) salida
	DDRD |= (1<<DDD1);

	UCSR0A = 0;

	
	// Habilitar RX, TX e interrupción RX
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	//si solo habilito la de tx, solo se manda, si habilito el rx me permite recibir el mensaje


	// 8 bits de datos
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

	// Baudrate 9600 parap 16MHz
	UBRR0 = 103;
}


// *********** Modulo de escritura en TERMINAL  **********//
// Enviar un carácter
void writeChar(char c)
{
	while (!(UCSR0A & (1<<UDRE0))); // Esperar buffer vacío
	UDR0 = c;
}

// Enviar string

void writeString(char *string)
{
	for (uint8_t i = 0; string[i] != '\0'; i++)
	{
		writeChar(string[i]);
	}
}





/**********************************************************************/
// Interrupt routines