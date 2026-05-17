/*
 * eepromsave.h
 *
 * Created: 16/05/2026 17:12:11
 *  Author: grett
 */ 


#ifndef EEPROMSAVE_H_
#define EEPROMSAVE_H_


#include <avr/io.h>
#include <stdint.h>

//================ BANDERAS ================//

extern volatile uint8_t saveFlag;

//================ FUNCIONES ================//

void EEPROM_Init(void);
void EEPROM_SaveServos(void);
#endif /* EEPROMSAVE_H_ */