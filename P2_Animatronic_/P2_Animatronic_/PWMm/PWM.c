/*
 * PWM.c
 *
 * Created: 28/04/2026 12:47:30
 *  Author: grett
 
 Es el pwm para la parte de: I2:Pupila izquierda & D2:Pupila derecha.

 */ 

#include "PWM.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t servo0A_ticks = 75; // PD6
volatile uint16_t servo0B_ticks = 75; // PD5

volatile uint16_t counter0 = 0;


void PWM0_Init(void)
{
    // PD6 y PD5 salida
    DDRD |= (1 << DDD6) | (1 << DDD5);

    // CTC mode
    TCCR0A = (1 << WGM01);

    // prescaler 8
    TCCR0B = (1 << CS01);

    /*
    16MHz / 8 = 2MHz

    tick = 0.5us

    OCR0A = 39

    40 ticks = 20us
    */

    OCR0A = 39;

    // habilitar interrupción compare match
    TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
    counter0++;

    // inicio frame
    if (counter0 == 1)
    {
        PORTD |= (1 << PORTD6);
        PORTD |= (1 << PORTD5);
    }

    // apagar servo PD6
    if (counter0 == servo0A_ticks)
    {
        PORTD &= ~(1 << PORTD6);
    }

    // apagar servo PD5
    if (counter0 == servo0B_ticks)
    {
        PORTD &= ~(1 << PORTD5);
    }

    // reinicio frame 20ms
    if (counter0 >= 1000)
    {
        counter0 = 0;
    }
}

void PWM0_SetDuty1(uint16_t ticks)
{
    servo0A_ticks = ticks;
}

void PWM0_SetDuty2(uint16_t ticks)
{
    servo0B_ticks = ticks;
}