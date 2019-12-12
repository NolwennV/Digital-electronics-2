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
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/
#define SERVO_MOTOR_SIGNAL PB2

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
	/* Configure PWM pin (OC1B, PB2) as output*/
 	GPIO_config_output(&DDRB, SERVO_MOTOR_SIGNAL);
    /* Timer1
     * Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_256);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);
	/*Set PWM phase correct mode*/
	TCCR1A |= _BV(WGM10) | _BV(WGM11);
	TCCR1B &= ~_BV(WGM12);
	TCCR1B |= _BV(WGM13);
	/*Set non inverting mode for PWM */
	TCCR1A &= ~_BV(COM1B0);
	TCCR1A |= _BV(COM1B1);
	/* Define TOP value to obtain f = 50Hz */
	OCR1A = 625;
	/* Define duty cycle */
	OCR1B = 50; 	//2ms pulse, right
	


    // Enables interrupts by setting the global interrupt mask
    sei();


    // Infinite loop
    for (;;) {
        
    }

    // Will never reach this
    return (0);
}

