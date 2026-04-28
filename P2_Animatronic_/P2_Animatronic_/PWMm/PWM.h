/*
 * PWM.h
 *
 * Created: 28/04/2026 12:47:42
 *  Author: grett
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

// Inicializa PWM por software
void PWM3_Init(void);

// Cambia duty cycle (0–255)
void PWM3_SetDuty(uint8_t duty);



#endif /* PWM_H_ */