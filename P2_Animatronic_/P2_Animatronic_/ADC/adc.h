/*
 * adc.h
 *
 * Created: 28/04/2026 11:40:16
 *  Author: grett
 */ 
 //declaración de funciones

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

// Inicializa el ADC
void ADC_Init(void);

// Lee canal (0–7)
uint16_t ADC_Read(uint8_t channel);

#endif
