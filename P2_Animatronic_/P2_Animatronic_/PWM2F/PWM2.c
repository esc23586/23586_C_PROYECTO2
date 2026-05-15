/*
 * PWM2.c
 *
 * Created: 5/05/2026 13:09:51
 *  Author: grett
 
 PWM para los pines asociados al timer 2--pd3, pb11, M1:Párpado 1 &  M2:Párpado 2
 */ 

#include "PWM2.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t servo2A_ticks = 75; // PD3
volatile uint16_t servo2B_ticks = 75; // PB3

volatile uint16_t counter2 = 0;

void PWM2_Init(void)
{
    // PD3 y PB3 salida
    DDRD |= (1 << DDD3);
    DDRB |= (1 << DDB3);

    // CTC mode
    TCCR2A = (1 << WGM21);

    // prescaler 8
    TCCR2B = (1 << CS21);

    /*
    16MHz / 8 = 2MHz

    2MHz = 0.5us por tick

    OCR2A = 39

    40 ticks = 20us
    */

    OCR2A = 39;

    // habilitar compare match
    TIMSK2 = (1 << OCIE2A);
}


ISR(TIMER2_COMPA_vect)
{
    counter2++;

    // inicio frame
    if (counter2 == 1)
    {
        PORTD |= (1 << PORTD3);
        PORTB |= (1 << PORTB3);
    }

    // apagar servo A
    if (counter2 == servo2A_ticks)
    {
        PORTD &= ~(1 << PORTD3);
    }

    // apagar servo B
    if (counter2 == servo2B_ticks)
    {
        PORTB &= ~(1 << PORTB3);
    }

    // 20ms frame
    if (counter2 >= 1000)
    {
        counter2 = 0;
    }
}

void PWM2_SetDuty1(uint16_t ticks)
{
    servo2A_ticks = ticks;
}

void PWM2_SetDuty2(uint16_t ticks)
{
    servo2B_ticks = ticks;
}

