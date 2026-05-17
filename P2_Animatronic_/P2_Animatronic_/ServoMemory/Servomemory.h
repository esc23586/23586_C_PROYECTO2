/*
 * Servomemory.h
 *
 * Created: 16/05/2026 21:02:21
 *  Author: grett
 */ 


#ifndef SERVOMEMORY_H_
#define SERVOMEMORY_H_

#include <stdint.h>

//================ ESTADO GLOBAL SERVOS =================//

extern volatile uint8_t servoAng[6];

//================ FUNCIONES =================//

void ServoMemory_LoadAndApply(void);

void ServoMemory_ApplyPWM(void);
//FUNCIÓN para mi print
void ServoMemory_PrintEEPROM(void);

#endif /* SERVOMEMORY_H_ */