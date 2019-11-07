/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-23
 * Last update: 2019-11-07
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Analog-to-digital conversion with displaying result on LCD and 
 *    transmitting via UART.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Read ADC result and transmit via UART.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // LCD display
    lcd_init(LCD_DISP_ON);
    lcd_puts("LCD testing");

    /* ADC
     * TODO: Configure ADC reference, clock source, enable ADC module, 
     *       and enable conversion complete interrupt */
     ADMUX |= _BV(REFS0);
     ADMUX &= ~_BV(REFS1);
     ADMUX &= ~_BV(MUX0);
     ADMUX &= ~_BV(MUX1);
     ADMUX &= ~_BV(MUX2);
     ADMUX &= ~_BV(MUX3);
     ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
     ADCSRA |= _BV(ADIE);
     ADCSRA |= _BV(ADEN);
    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);
    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put string to ringbuffer for transmitting via UART.
    uart_puts("UART testing\r\n");

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Start ADC conversion.
 */
ISR(TIMER1_OVF_vect)
{
    // TODO: Start ADC conversion
    ADCSRA |= _BV(ADSC);
}

/**
 *  Brief: ADC complete interrupt routine. Update LCD and UART 
 *         transmiter.
 */
ISR(ADC_vect)
{
   uint16_t value = 0;
   char uart_string[30];

	// Read 10-bit ACD value
   value = ADC;

    // TODO: Update LCD and UART transmiter
    itoa(value, uart_string , 10);
    lcd_clrscr();
    lcd_puts(uart_string);
    uart_puts(uart_string);
    if (value>=500 && value<=750){
        lcd_clrscr();
        lcd_puts("Select button");
        uart_puts("\033[32m");  
        uart_puts(" Select button\r\n");
    }
    else if (value>= 300 && value <=500){
        lcd_clrscr();
        lcd_puts("Left button");
        uart_puts("\033[33m");  
        uart_puts(" Left button\r\n");
    }
    else if (value >= 150 && value <= 300){
        lcd_clrscr();
        lcd_puts("Down button");
        uart_puts("\033[34m");  
        uart_puts(" Down button\r\n");
    }
    else if (value >= 50 && value <= 150){
        lcd_clrscr();
        lcd_puts("Up button");
        uart_puts("\033[35m");  
        uart_puts(" Up button\r\n");
    }
    else if (value <= 50){
        lcd_clrscr();
        lcd_puts("Right button");
        uart_puts("\033[36m");  
        uart_puts(" Right button\r\n");
    }
    else {
        lcd_clrscr();
        lcd_puts("No button");
        uart_puts("\033[0m");    
        uart_puts(" No button\r\n");
    }
}