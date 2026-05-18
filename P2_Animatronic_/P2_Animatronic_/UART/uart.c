/*
 * uart.c
 *
 * Created: 28/04/2026 14:25:30
 *  Author: grett
 */ 
//==========================librería ==========================//

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

//PARTE DE CONEXIÓN CON EEPROM 
#include "../ServoMemory/ServoMemory.h"//libreria de apoyo para lectura de memoria de eeporm (opción 3)
#include "../PosesEditor/PosesEditor.h"// conezión con la librería de apoyo para la edicion de poses (opcíon5)

//================ Variables Globales ================//

volatile uint8_t modo = 0;
volatile uint8_t commandReady = 0;

volatile uint8_t idx = 0;

char buffer[20];
char out[10];

//modificación para que funcione los angulos 
volatile uint8_t servoSeleccionado = 0;
//Basicamente cuando sea 0- selección de servos y cuando es dif de 0 se qeuda esperando el angulo

/****************************************************************************************************/
//Funciones descritas


//============ Inicialización UART =============//

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

//========== UART funcoines básicas de comunicación ==========//

void writeChar(char c)//para caracters
{
	while (!(UCSR0A & (1 << UDRE0)));

	UDR0 = c;
}

void writeString(char *string)//para mandar mis cadenas de texto 
{
	uint8_t i = 0;

	while (string[i] != '\0')
	{
		writeChar(string[i]);
		i++;
	}
}

//================ Menú =================//

void printMenu(void)
{
	writeString("\r\n");
	writeString("======= MENU ANIMATRONICO =======\r\n");
	writeString("1 > Control de Ojos UART\r\n");
	writeString("2 > Control de Parpados UART\r\n");
	writeString("3 > Reproducir Posiciones EEPROM\r\n");
	writeString("4 > Control Manual ADC\r\n");
	writeString("5 > Editar Posiciones EEPROM\r\n");
	writeString("=================================\r\n");
	
	writeString("Presiona el número de la opción y añade al final ; \r\n");
	writeString("Ejemplo: 1; \r\n");
	writeString("> ");
	
	/* menú opciones
	1 -> Control Ojos UART
	2 -> Control Parpados UART
	3 -> Reproducir Posiciones EEPROM
	4 -> Control Manual ADC
	5 -> Editar Posiciones EEPROM
	*/
}

//================ Conversión Servos ================//
//Esta parte se movió a servo Utilitis. pues se necestiaba en el main también.
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

//================ Procesamiento Comandos: Casos del menú ========================//

void processCommand(void)
{
	//================ MODO 0 ================//

	if (modo == 0)
	{
		if (strcmp(buffer, "1") == 0)
		{
			modo = 1;
/*
			writeString("\r\nModo OJOS\r\n");
			writeString("1 -> Servo I1\r\n");
			writeString("2 -> Servo D1\r\n");
			writeString("3 -> Servo I2\r\n");
			writeString("4 -> Servo D2\r\n");
*/
			writeString("\r\nModo OJOS\r\n");
			writeString("I1:angulo;\r\n");
			writeString("D1:angulo;\r\n");
			writeString("I2:angulo;\r\n");
			writeString("D2:angulo;\r\n");


			writeString("x; para salir\r\n");
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
			/*
			writeString("Leyendo EEPROM...\r\n");
			
			writeString("Cargando posiciones EEPROM...\r\n");//nuevo con la adición a eeprom 
			ServoMemory_LoadAndApply();//mueve físicamente el animatrónico.
			ServoMemory_PrintEEPROM();//muestra por UART los ángulos cargados, en listita
			
			writeString("Posiciones aplicadas\r\n");// basicmaente interacción con la interfás del susuario. 
			*/
			//En esta versión del código se hará un a rpueba para sacar los valores que se guardaron al presiona pb5
		
		
			modo = 4;

			writeString("\r\n===== REPRODUCIR POSICIONES =====\r\n");

			writeString("1 -> Posicion 1\r\n");
			writeString("2 -> Posicion 2\r\n");
			writeString("3 -> Posicion 3\r\n");
			writeString("4 -> Posicion 4\r\n");
			writeString("5 -> Posicion 5\r\n");

			writeString("x -> salir\r\n");
		}
		
		//================ ADC MANUAL ================//
		
		else if (strcmp(buffer, "4") == 0)
		{
			modo = 3;

			writeString("Modo ADC Manual\r\n");
			writeString("Potenciometros activos\r\n");
			writeString("x; para salir\r\n");
		}
		
		//================ EDITAR EEPROM ================// mi opción 5

		else if (strcmp(buffer, "5") == 0)
		{
			modo = 5;

			writeString("\r\n===== EDITAR POSICIONES =====\r\n");

			writeString("1 -> Editar Posicion 1\r\n");
			writeString("2 -> Editar Posicion 2\r\n");
			writeString("3 -> Editar Posicion 3\r\n");
			writeString("4 -> Editar Posicion 4\r\n");
			writeString("5 -> Editar Posicion 5\r\n");

			writeString("x -> salir\r\n");
		}
		
		else
		{
			writeString("Opcion invalida\r\n");
			printMenu();
		}
	}

/////////////////////////////////////////---Logica para navegar en cada una de las opciones dichas anteirormente
//para la opcion 1 


	//================ MODO OJOS ================//
/*
	else if (modo == 1)
	{//NO HAY SERVO SELECCIONADO

		if (servoSeleccionado == 0)
		{
			if (strcmp(buffer, "1") == 0)// en caso se quiera editar el angulo de base izquierdo
			{
				servoSeleccionado = 1;

				writeString("\r\nServo I1 seleccionado\r\n");
				writeString("Ingrese angulo 0-180;\r\n");
			}

			else if (strcmp(buffer, "2") == 0)//en caso se quiera editar el angulo de base derecho
			{
				servoSeleccionado = 2;

				writeString("\r\nServo D1 seleccionado\r\n");
				writeString("Ingrese angulo 0-180;\r\n");
			}

			else if (strcmp(buffer, "3") == 0)//en caso se quiera editar el angulo de ojo izquierda
			{
				servoSeleccionado = 3;

				writeString("\r\nServo I2 seleccionado\r\n");
				writeString("Ingrese angulo 0-180;\r\n");
			}

			else if (strcmp(buffer, "4") == 0)////en caso se quiera editar el angulo de ojo derecho
			{
				servoSeleccionado = 4;

				writeString("\r\nServo D2 seleccionado\r\n");
				writeString("Ingrese angulo 0-180;\r\n");
			}

			else if (strcmp(buffer, "x") == 0)
			{
				modo = 0;

				printMenu();
			}

			else
			{
				writeString("Servo invalido\r\n");
			}
		}

		//YA hay un servo seleccionado (un motor)

		else
		{
			uint8_t ang = atoi(buffer);

			if (ang > 180)
			{
				ang = 180;
			}

			switch (servoSeleccionado)
			{
				case 1:
				PWM1_SetDuty(ServoToOCR(ang));
				writeString("I1 -> ");
				break;

				case 2:
				PWM1_SetDuty2(ServoToOCR(ang));
				writeString("D1 -> ");
				break;

				case 3:
				PWM0_SetDuty1(ServoToTicks(ang));
				writeString("I2 -> ");
				break;

				case 4:
				PWM0_SetDuty2(ServoToTicks(ang));
				writeString("D2 -> ");
				break;
			}

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");

			// Reset de selección por si quiere seleccionar otro dentro del mismo grupo de motores (ojos y bases)
			servoSeleccionado = 0;

			writeString("\r\nSeleccione otro servo:\r\n");
			writeString("1 -> I1\r\n");
			writeString("2 -> D1\r\n");
			writeString("3 -> I2\r\n");
			writeString("4 -> D2\r\n");
			writeString("x -> salir\r\n");
		
		
		}
	}
*/	
	else if (modo == 1)
	{
		if (strncmp(buffer, "I1:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			if (ang > 180)
			{
				ang = 180;
			}

			PWM1_SetDuty(ServoToOCR(ang));

			writeString("I1 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "D1:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			if (ang > 180)
			{
				ang = 180;
			}

			PWM1_SetDuty2(ServoToOCR(ang));

			writeString("D1 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "I2:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			if (ang > 180)
			{
				ang = 180;
			}

			PWM0_SetDuty1(ServoToTicks(ang));

			writeString("I2 -> ");

			itoa(ang, out, 10);

			writeString(out);
			writeString("\r\n");
		}

		else if (strncmp(buffer, "D2:", 3) == 0)
		{
			uint8_t ang = atoi(&buffer[3]);

			if (ang > 180)
			{
				ang = 180;
			}

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
//================ MODO PARPADOS =================================// MI OPCIÓN 2

		else if (modo == 2)
		{
			if (strncmp(buffer, "M1:", 3) == 0)
			{
			
				uint8_t ang = atoi(&buffer[3]);

				if (ang > 180)
				{
					ang = 180;
				}

				PWM2_SetDuty1(ServoToTicks(ang));

				writeString("M1 -> ");

				itoa(ang, out, 10);

				writeString(out);
				writeString("\r\n");
			}

			else if (strncmp(buffer, "M2:", 3) == 0)
			{
			
				uint8_t ang = atoi(&buffer[3]);

				if (ang > 180)
				{
					ang = 180;
				}

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
//  MODO PARA LA LECTURA adc la opción 4 de mi menú

		else if (modo == 3)
		{
			if (strcmp(buffer, "x") == 0)
			{
				modo = 0;

				writeString("Saliendo modo ADC\r\n");

				printMenu();
			}
		}
//================ REPRODUCIR POSICIONES ================// Opción 3(lectura de eeprom )  pero modo 4

		else if (modo == 4)
		{
			if (strcmp(buffer, "1") == 0)
			{
				writeString("Reproduciendo Posicion 1\r\n");

				// Pose_Load(1);

				ServoMemory_LoadAndApply(1);

				ServoMemory_PrintEEPROM();

				

				writeString("Posicion aplicada\r\n");
			}

			else if (strcmp(buffer, "2") == 0)
			{
				writeString("Reproduciendo Posicion 2\r\n");

				// Pose_Load(2);
				ServoMemory_LoadAndApply(2);

				ServoMemory_PrintEEPROM();
				

				writeString("Posicion aplicada\r\n");
			}

			else if (strcmp(buffer, "3") == 0)
			{
				writeString("Reproduciendo Posicion 3\r\n");

				// Pose_Load(3);
				ServoMemory_LoadAndApply(3);

				ServoMemory_PrintEEPROM();
				
				writeString("Posicion aplicada\r\n");
			}

			else if (strcmp(buffer, "4") == 0)
			{
				writeString("Reproduciendo Posicion 4\r\n");

				// Pose_Load(4);
				ServoMemory_LoadAndApply(4);

				ServoMemory_PrintEEPROM();
				

				writeString("Posicion aplicada\r\n");
			}

			else if (strcmp(buffer, "5") == 0)
			{
				writeString("Reproduciendo Posicion 5\r\n");

				// Pose_Load(5);
				ServoMemory_LoadAndApply(5);

				ServoMemory_PrintEEPROM();
				

				writeString("Posicion aplicada\r\n");
			}

			else if (strcmp(buffer, "x") == 0)
			{
				modo = 0;

				printMenu();
			}

			else
			{
				writeString("Opcion invalida\r\n");
			}
		}
//===================================== EDITAR POSICIONES // opción numero 5 del menú

		else if (modo == 5)
		{
			if (strcmp(buffer, "1") == 0)
			{
				writeString("Editando Posicion 1\r\n");

				// Pose_Edit(1);
				Pose_Save(
				1,
				servoAng[0],
				servoAng[1],
				servoAng[2],
				servoAng[3],
				servoAng[4],
				servoAng[5]
				);
			}

			else if (strcmp(buffer, "2") == 0)
			{
				writeString("Editando Posicion 2\r\n");

				// Pose_Edit(2);
				Pose_Save(
				2,
				servoAng[0],
				servoAng[1],
				servoAng[2],
				servoAng[3],
				servoAng[4],
				servoAng[5]
				);
			}

			else if (strcmp(buffer, "3") == 0)
			{
				writeString("Editando Posicion 3\r\n");

				// Pose_Edit(3);
				Pose_Save(
				3,
				servoAng[0],
				servoAng[1],
				servoAng[2],
				servoAng[3],
				servoAng[4],
				servoAng[5]
				);
			}

			else if (strcmp(buffer, "4") == 0)
			{
				writeString("Editando Posicion 4\r\n");

				// Pose_Edit(4);
				Pose_Save(
				4,
				servoAng[0],
				servoAng[1],
				servoAng[2],
				servoAng[3],
				servoAng[4],
				servoAng[5]
				);
			}

			else if (strcmp(buffer, "5") == 0)
			{
				writeString("Editando Posicion 5\r\n");

				// Pose_Edit(5);
				Pose_Save(
				5,
				servoAng[0],
				servoAng[1],
				servoAng[2],
				servoAng[3],
				servoAng[4],
				servoAng[5]
				);
			}

			else if (strcmp(buffer, "x") == 0)
			{
				modo = 0;

				printMenu();
			}

			else
			{
				writeString("Opcion invalida\r\n");
			}
		}
	}
//****************************************************************************************************************//
//Rutina de interrupción 

//Rutina de interrupción para manejar el menú principal para comunicación rx 
//======================== ISR UART ==========================//

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