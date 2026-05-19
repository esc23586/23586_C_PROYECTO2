/*
 * Servomemory.c
 *
 * Created: 16/05/2026 21:02:09
 *  Author: grett
 descripción: 
 Debido a que se debe tomar dos acciones diferentes de eeprom, se decidió dividir en dos acciones principales. 
 La parte de guardar con boton PB5 Está en la librería EEPROMSave. 
 Por otro lado en esta, ServoMemory, será una segunda carpeta que se ejecutará en apoyo a la librería UART.
  Pues se ejecutará cuando se desee reproducir y modificar el valor desde UART 
  
 
 */ 


#include "Servomemory.h"

#include <avr/io.h>
#include <avr/eeprom.h>

// PWM
#include "../PWMm/PWM.h"
#include "../PWM1F/PWM1.h"
#include "../PWM2F/PWM2.h"

// Utils
#include "../ServoUtils/ServoUtils.h"
#include "../UART/UART.h"
#include <stdlib.h>

//==============GLOBALES=================//


volatile uint8_t servoAng[6] =
{
	90,
	90,
	90,
	90,
	90,
	90
};// se cambiaron las variables temporales

void ServoMemory_PrintEEPROM(void)// esta es la funcón para el menú cuando sale en la opción tres de eeprom del menú
{
	char out[10];

	writeString("\r\n===== EEPROM =====\r\n");

	writeString("I1 -> ");
	itoa(servoAng[0], out, 10);
	writeString(out);
	writeString("\r\n");

	writeString("D1 -> ");
	itoa(servoAng[1], out, 10);
	writeString(out);
	writeString("\r\n");

	writeString("I2 -> ");
	itoa(servoAng[2], out, 10);
	writeString(out);
	writeString("\r\n");

	writeString("D2 -> ");
	itoa(servoAng[3], out, 10);
	writeString(out);
	writeString("\r\n");

	writeString("M1 -> ");
	itoa(servoAng[4], out, 10);
	writeString(out);
	writeString("\r\n");

	writeString("M2 -> ");
	itoa(servoAng[5], out, 10);
	writeString(out);
	writeString("\r\n");
}

void ServoMemory_ApplyPWM(void)// funcion solo para el pwm
{
	// Ojos base
	PWM1_SetDuty(ServoToOCR(servoAng[0]));
	PWM1_SetDuty2(ServoToOCR(servoAng[1]));

	// Pupilas
	PWM0_SetDuty1(ServoToTicks(servoAng[2]));
	PWM0_SetDuty2(ServoToTicks(servoAng[3]));

	// Parpados
	PWM2_SetDuty1(ServoToTicks(servoAng[4]));
	PWM2_SetDuty2(ServoToTicks(servoAng[5]));
}

void ServoMemory_LoadAndApply(uint8_t pose)
{
	uint8_t base;

	base = (pose - 1) * 6;

	//================ LEER EEPROM =================//

	servoAng[0] = eeprom_read_byte((uint8_t*)(base + 0));
	servoAng[1] = eeprom_read_byte((uint8_t*)(base + 1));

	servoAng[2] = eeprom_read_byte((uint8_t*)(base + 2));
	servoAng[3] = eeprom_read_byte((uint8_t*)(base + 3));

	servoAng[4] = eeprom_read_byte((uint8_t*)(base + 4));
	servoAng[5] = eeprom_read_byte((uint8_t*)(base + 5));

	//================ APLICAR PWM =================//

	ServoMemory_ApplyPWM();
}

//================(Verión anterior para la lectura de eeprom)  LEER EEPROM  y se aplicaba =================//
/*
	void ServoMemory_LoadAndApply(void)
	{
		//LEER EEPROM:

		servoAng[0] = eeprom_read_byte((uint8_t*)24);
		servoAng[1] = eeprom_read_byte((uint8_t*)25);

		servoAng[2] = eeprom_read_byte((uint8_t*)26);
		servoAng[3] = eeprom_read_byte((uint8_t*)27);

		servoAng[4] = eeprom_read_byte((uint8_t*)28);
		servoAng[5] = eeprom_read_byte((uint8_t*)29);

		//APLICAR:

		ServoMemory_ApplyPWM();
	}
	*/
