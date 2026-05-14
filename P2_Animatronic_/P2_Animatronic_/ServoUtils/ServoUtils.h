/*
 * ServoUtils.h
 *
 * Created: 12/05/2026 13:07:06
 *  Author: grett
 */ 


#ifndef SERVOUTILS_H_
#define SERVOUTILS_H_

#include <stdint.h>

uint16_t ServoToOCR(uint8_t ang);

uint8_t ServoToTicks(uint8_t ang);

uint8_t ADCToAngle(uint16_t adc);


#endif /* SERVOUTILS_H_ */