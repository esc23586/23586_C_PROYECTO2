/*
 * PosesEDITOR.c
 *
 * Created: 17/05/2026 17:00:17
 *  Author: grett
 */ 
#include "PosesEditor.h"

#include <avr/eeprom.h>

//================ DIRECCION BASE =================//

uint8_t Pose_GetBaseAddress(uint8_t pose)
{
	return (pose - 1) * 6;
}

//================ GUARDAR POSE =================//

void Pose_Save(
uint8_t pose,
uint8_t s1,
uint8_t s2,
uint8_t s3,
uint8_t s4,
uint8_t s5,
uint8_t s6
)
{
	uint8_t base;

	base = Pose_GetBaseAddress(pose);

	//================ GUARDAR EEPROM =================//

	eeprom_update_byte((uint8_t*)(base + 0), s1);
	eeprom_update_byte((uint8_t*)(base + 1), s2);

	eeprom_update_byte((uint8_t*)(base + 2), s3);
	eeprom_update_byte((uint8_t*)(base + 3), s4);

	eeprom_update_byte((uint8_t*)(base + 4), s5);
	eeprom_update_byte((uint8_t*)(base + 5), s6);
}