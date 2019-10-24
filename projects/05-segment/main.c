/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-11
 * Last update: 2019-10-16
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Decimal counter with data display on 7-segment display.
 * TODO: Create and test functions to operate a 7-segment display using
 *       shift registers.
 * NOTE: Store segment.c and segment.h files in library/src and 
 *       library/inc folders.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include "gpio.h"
#include "timer.h"
#include "segment.h"
#include <avr/interrupt.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_D1      PB5
#define BTN_S1      PC1     // PCINT 9
#define BTN_S2      PC2     // PCINT 10
#define BTN_S3      PC3     // PCINT 11

static uint16_t cpt = 0;
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on 7-segment display.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    /* D1 led */
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write(&PORTB, LED_D1, PIN_HIGH);

    /* Pin Change Interrupts 11:9 */
     PCMSK1 |= _BV(BTN_S1) | _BV(BTN_S2) | _BV(BTN_S3);
     PCICR |= _BV(PCIE1);


    /* 7-segment display interface */
    GPIO_config_output(&DDRD, SEGMENT_LATCH);
    GPIO_write(&PORTD, SEGMENT_LATCH, PIN_LOW);

    GPIO_config_output(&DDRB, SEGMENT_DATA);
    GPIO_write(&PORTB, SEGMENT_DATA, PIN_LOW);

    GPIO_config_output(&DDRD, SEGMENT_CLK);
    GPIO_write(&PORTD, SEGMENT_CLK, PIN_LOW);

    /* Set Timer0 */
    TIM_config_prescaler(TIM0, TIM_PRESC_8);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

    /* Set Timer1 */
    TIM_config_prescaler(TIM1, TIM_PRESC_8);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // Enable interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    for (;;) {
        // TODO: Use function to display digit 1 at position 0
        /* SEG_putc(8, 0);
        SEG_putc(9, 1);
        SEG_putc(6, 2);
        SEG_putc(7, 3); */  //infinite loop, so the 4 digits are visible at the same time without blinking
        
    
    }
    return (0);
}

/**
 *  Brief: Pin Change Interrupt 11:9 routine. Toggle a LED.
 */
ISR(PCINT1_vect)
{
    /* cpt++;
    if(cpt > 99){
        cpt = 0;
    } */
}
/**
 *  Brief: Timer/Counter0 overflow interrupt routine. Display counter.
 */
ISR(TIMER0_OVF_vect)
{
    SEG_putc(((cpt % 1000) % 100 ) % 10, 0);
    SEG_putc(((cpt % 1000) % 100 ) / 10, 1);
    SEG_putc((cpt % 1000) / 100, 2);
    SEG_putc(cpt / 1000, 3);
}

/**
 *  Brief: Timer/Counter2 overflow interrupt routine. Increments counter.
 */
ISR(TIMER1_OVF_vect)
{
    cpt++;
    if(cpt > 9999){
        cpt = 0;
    }
}