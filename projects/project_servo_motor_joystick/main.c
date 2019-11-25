/*
 * ---------------------------------------------------------------------
 * Author:      
 * Created:     2018-11-25
 * Last update:
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *   
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "uart.h"

/* Typedef -----------------------------------------------------------*/
typedef enum {
    IDLE = 1,
    X_CONVERSION,
    Y_CONVERSION,
} state_t;
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
state_t current_state = IDLE;
/* Function prototypes -----------------------------------------------*/
void fsm_ADC(void);

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Read ADC result and transmit via UART.
 *  Input:  None
 *  Return: None
 */
int main(void)
{

    /* ADC
     * Configure ADC reference, clock source, enable ADC module, 
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
    uart_puts("---Input from joystick---\r\n");

    // Infinite loop
    for (;;) {
        
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. SUpdtae state of the FSM.
 */
ISR(TIMER1_OVF_vect)
{
    fsm_ADC();
}

/**
 *  Brief: ADC complete interrupt routine. Update UART 
 *         transmiter.
 */
ISR(ADC_vect)
{
   uint16_t value = 0;
   char uart_string[30];

   // Read 10-bit ACD value
   value = ADC;

    //Update UART transmiter
    itoa(value, uart_string , 10);
    uart_puts(uart_string); 
  
}
/* Functions ---------------------------------------------------------*/
/* Switch ADC conversion between X and Y */
void fsm_ADC(void)
{
    switch (current_state) {
    case IDLE:
	uart_puts("\r\n");
        current_state = X_CONVERSION;
        break;

    
    case X_CONVERSION:
        //Select channel 0 for X conversion
	ADMUX &= ~_BV(MUX0);
	ADMUX &= ~_BV(MUX1);
	ADMUX &= ~_BV(MUX2);
	ADMUX &= ~_BV(MUX3);
	
	uart_puts("X = ");
	//Start ADC conversion
	ADCSRA |= _BV(ADSC);
        current_state = Y_CONVERSION;
        break;

    
    case Y_CONVERSION:
        //Select channel 1 for Y conversion
	ADMUX |= _BV(MUX0);
	ADMUX &= ~_BV(MUX1);
	ADMUX &= ~_BV(MUX2);
	ADMUX &= ~_BV(MUX3);
	uart_puts("Y = ");
	//Start ADC conversion
	ADCSRA |= _BV(ADSC);
        current_state = IDLE;
        break;

    default:
        current_state = IDLE;
    }
}
