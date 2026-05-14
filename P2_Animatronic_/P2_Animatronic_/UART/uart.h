/*
 * uart.h
 *
 * Created: 28/04/2026 14:25:53
 *  Author: grett
 */ 


#ifndef UART_H_
#define UART_H_


//========================== UART.h ==========================//
#include <stdint.h>

//================ Variables Globales ================//

extern volatile uint8_t modo;
extern volatile uint8_t commandReady;

extern char buffer[20];
extern char out[10];

//================ Inicialización UART ================//

void UART_Init(void);

//================ Comunicación UART ================//

void writeChar(char c);
void writeString(char *string);

//================ Menú y comandos ================//

void printMenu(void);
void processCommand(void);

//================ Conversión Servo ================//

uint16_t ServoToOCR(uint8_t ang);
uint8_t ServoToTicks(uint8_t ang);



#endif /* UART_H_ */