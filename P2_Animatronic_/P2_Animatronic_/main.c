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
//#include "PWM1F/PWM1.h"//timer 1 
#include "Servo/servo.h"//Timer 1

/****************************************/
// Function prototypes

void intUART(void);
void writeChar(char c);
void writeString(char *string);
//void writeNumber(uint8_t num);
//void displayASCII(uint8_t value);//Cmabiar
void printMenu(void);

/****************************************/
// Main Function
//globales ;
volatile uint8_t modo = 0;//MODO Predeterminado
//Aqui puede ir la parte de la cara y sus 
char buffer[20];        // buffer de entrada
char out[10];           // buffer para imprimir números
volatile uint8_t idx = 0;


// Main Function
//***********************************  MAIN   **************************************//
int main(void)
{
	cli();
	//Entradas- y salidas declaración
	DDRB |= (1<<DDB5) | (1<<DDB4);
	PORTB &= ~((1<<PORTB5) | (1<<PORTB4));
	
	//inicializaciónes 
	intUART();//Comunicación UART
	/*
	PWM1_Init(); // Servo 1 y 2 (Timer1)
	PWM3_Init();// Led manual con TImer 0
	*/
	
	Servo_Init();// CONTROL DE servomotores Timer 1
	ADC_Init();//ADC
	
	
	
	sei();// BUENAS CONSTUMBRES

	printMenu();
	while (1)
	{
		//Realizar la comparación en el Main
				if (modo == 0)
				{
					if (buffer[0] == '1')
					{
						modo = 1;
						writeString("Modo OJOS\r\n");
						writeString("I1,D1,I2,D2 + angulo | x salir\r\n");
					}
					else if (buffer[0] == '2')
					{
						modo = 2;
						writeString("Modo PARPADOS\r\n");
						writeString("M1,M2 + angulo | x salir\r\n");
					}
					else if (buffer[0] == '3')
					{
						writeString("EEPROM pendiente\r\n");
					}
					else
					{
						writeString("Opcion invalida\r\n");
					}

					printMenu();
				}

				// ================= OJOS =================
				else if (modo == 1)
				{
					if (strncmp(buffer, "I1:", 3) == 0)
					{
						uint8_t ang = atoi(&buffer[3]);
						Servo_Set(0, ang);

						writeString("I1 -> ");
						itoa(ang, out, 10);
						writeString(out);
						writeString("\r\n");
					}
					else if (strncmp(buffer, "D1:", 3) == 0)
					{
						uint8_t ang = atoi(&buffer[3]);
						Servo_Set(1, ang);

						writeString("D1 -> ");
						itoa(ang, out, 10);
						writeString(out);
						writeString("\r\n");
					}
					else if (strncmp(buffer, "I2:", 3) == 0)
					{
						uint8_t ang = atoi(&buffer[3]);
						Servo_Set(2, ang);

						writeString("I2 -> ");
						itoa(ang, out, 10);
						writeString(out);
						writeString("\r\n");
					}
					else if (strncmp(buffer, "D2:", 3) == 0)
					{
						uint8_t ang = atoi(&buffer[3]);
						Servo_Set(3, ang);

						writeString("D2 -> ");
						itoa(ang, out, 10);
						writeString(out);
						writeString("\r\n");
					}
					else if (buffer[0] == 'x')
					{
						modo = 0;
						printMenu();
					}
					else
					{
						writeString("Comando invalido\r\n");
					}
				}

				// ================= PARPADOS =================
				else if (modo == 2)
				{
					if (strncmp(buffer, "M1:", 3) == 0)
					{
						uint8_t ang = atoi(&buffer[3]);
						Servo_Set(4, ang);

						writeString("M1 -> ");
						itoa(ang, out, 10);
						writeString(out);
						writeString("\r\n");
					}
					else if (strncmp(buffer, "M2:", 3) == 0)
					{
						uint8_t ang = atoi(&buffer[3]);
						Servo_Set(5, ang);

						writeString("M2 -> ");
						itoa(ang, out, 10);
						writeString(out);
						writeString("\r\n");
					}
					else if (buffer[0] == 'x')
					{
						modo = 0;
						printMenu();
					}
					else
					{
						writeString("Comando invalido\r\n");
					}
				}

				idx = 0;
			}
			else
			{
				if (idx < sizeof(buffer)-1)
				{
					buffer[idx++] = c;
				}
			}
		}
		// las subrutinas de la rutina
		
		//la interrupción debe ser unicamente para levantar banderas. 

/*****************************************************************************/
// NON-Interrupt subroutines

//para el menu
void printMenu(void)
{
	writeString("=========== Menu Animatrónico=============\r\n");
	writeString("1: Mover Ojos \r\n");
	writeString("2: Mover Parpados\r\n");
	writeString("3: Mostrar Ultima posición guradada EEPROM\r\n");
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
/*
			ISR(USART_RX_vect)
			{
				char c = UDR0;
				writeChar(c);// eco


				// ENTER: Si se le da enter al final entoncecs continua y procesa mi comando
				if (c == '\r' || c == '\n')
				{
					buffer[idx] = '\0';// cerrar string
					writeString("\r\n");
					
					// =======================
					// MODO 0 ? MENÚ
					// =======================
					if (modo == 0)
					{
						if (buffer[0] == '1')
						{
							
							modo = 1;
							writeString("Modo OJOS\r\n");
							writeString("Use: I1:[angulo], D1:[angulo], I2:[angulo], D2:[angulo]\r\n");
							writeString("Con x puede salir\r\n");
							
						}
						else if (buffer[0] == '2')
						{
							modo = 2;
							writeString("Modo PARPADOS\r\n");
							writeString("Use: M1:[angulo], M2:[angulo]\r\n");
							writeString("Con x puede salir\r\n");
						}
						else if (buffer[0] == '3')
						{
							writeString("Leyendo EEPROM...\r\n");
						}
						else
						{
							writeString("Opcion invalida\r\n");
						}
						
						printMenu();
					}

					// =======================
					// MODO 1 ? OJOS
					// =======================
					else if (modo == 1)
					{
						if (strncmp(buffer, "I1:", 3) == 0)
						{
							int ang = atoi(&buffer[3]);
							Servo_Set(0, ang);

							writeString("I1 -");
							itoa(ang, out, 10);
							writeString(out);
							writeString("\r\n");
							
						}
						else if (strncmp(buffer, "D1:", 3) == 0)
						{
							int ang = atoi(&buffer[3]);
							Servo_Set(1, ang);
							writeString("Ojo der base -> ");

							itoa(ang, out, 10);
							writeString(out);
							writeString("\r\n");
						}
						else if (strncmp(buffer, "I2:", 3) == 0)
						{
							int ang = atoi(&buffer[3]);
							Servo_Set(2, ang);
							writeString("Pupila iz -> ");
							itoa(ang, out, 10);
							writeString(out);
							writeString("\r\n");
						}
						else if (strncmp(buffer, "D2:", 3) == 0)
						{
							uint8_t ang = atoi(&buffer[3]);
							Servo_Set(3, ang);

							writeString("D2 -> ");
							itoa(ang, out, 10);
							writeString(out);
							writeString("\r\n");
							
						}
						else if (buffer[0] == 'x') // salir
						{
							modo = 0;
							printMenu();
						}
						else
						{
							writeString("Comando invalido\r\n");
						}
					}

					// =======================
					// MODO 2 ? PARPADOS
					// =======================
					else if (modo == 2)
					{
						if (strncmp(buffer, "M1:", 3) == 0)
						{
							uint8_t ang = atoi(&buffer[3]);
							Servo_Set(4, ang);

							writeString("M1 -> ");
							itoa(ang, out, 10);
							writeString(out);
							writeString("\r\n");
						}
						else if (strncmp(buffer, "M2:", 3) == 0)
						{
							uint8_t ang = atoi(&buffer[3]);
							Servo_Set(5, ang);

							writeString("M2 -> ");
							itoa(ang, out, 10);
							writeString(out);
							writeString("\r\n");
						}
						else if (buffer[0] == 'x')
						{
							modo = 0;
							printMenu();
						}
						else
						{
							writeString("Comando invalido\r\n");
						}
					}

					// reiniciar buffer
					idx = 0;
				}
				else
					{
						// guardar en buffer
						if (idx < sizeof(buffer)-1)
						{
							buffer[idx++] = c;
						}
					}
				}
				
			
*/
			
//Continuación del código
ISR(USART_RX_vect)
{
	char c = UDR0;
	writeChar(c);

	// ENTER
	if (c == '\r' || c == '\n')
	{
		buffer[idx] = '\0';
		writeString("\r\n");

		// ================= MENU =================
		
			

			
		

		// ================= PARPADOS =================
		
			

			
		}
	
	
}
