/*
 * PWM2.h
 *
 * Created: 5/05/2026 13:10:05
 *  Author: grett
 */ 


#ifndef PWM2_H_
#define PWM2_H_

#include <stdint.h>

void PWM2_Init(void);
void PWM2_SetDuty1(uint8_t ticks);
void PWM2_SetDuty2(uint8_t ticks);

#endif /* PWM2_H_ */