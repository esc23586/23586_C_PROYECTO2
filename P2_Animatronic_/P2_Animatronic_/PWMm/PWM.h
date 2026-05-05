/*
 * PWM.h
 *
 * Created: 28/04/2026 12:47:42
 *  Author: grett
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>


void PWM0_Init(void);
void PWM0_SetDuty1(uint8_t ticks);
void PWM0_SetDuty2(uint8_t ticks);


#endif /* PWM_H_ */