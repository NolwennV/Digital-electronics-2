/*
 * ---------------------------------------------------------------------
 * Author:      Nolwenn Violette, Freddie Wilson Jennings
 * Created:     2018-11-25
 * Last update:	2019-12-18
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *	Control of two servomotors with one joystick, with 4 different
 *	operating modes. 
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer.h"
#include "gpio.h"
#include "uart.h"

/* Typedef -----------------------------------------------------------*/
typedef enum {
    IDLE = 1,
    X_CONVERSION,
    Y_CONVERSION,
	PWM_SIGNAL
} state_t;

/* Define ------------------------------------------------------------*/
#define SERVO_MOTOR_SIGNAL_X PB2
#define SERVO_MOTOR_SIGNAL_Y PB1
#define KEY_BUTTON PC2
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
state_t current_state = IDLE;
int is_x = 1;
int mode = 0;
int debounce = 0;

/* Function prototypes -----------------------------------------------*/
void fsm_ADC(void);

/* Functions ---------------------------------------------------------*/
/**
 * Brief:  Main program. Send joystick output to ADC,then generate two
 * 	   PWM signals on timer 1 according to ADC value, for servo-
 *	   motors control.
 * Input:  None
 * Return: None
 */
int main(void)
{
	/* Configure PWM pin (OC1A, PB1) as output*/
 	GPIO_config_output(&DDRB, SERVO_MOTOR_SIGNAL_Y);
     	/* Configure PWM pin (OC1A, PB2) as output*/
 	GPIO_config_output(&DDRB, SERVO_MOTOR_SIGNAL_X);
    	/* Timer1
     	* Configure Timer1 clock source and enable overflow 
    	* interrupt */
    	TIM_config_prescaler(TIM1, TIM_PRESC_256);
    	TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);
	/*Set PWM phase correct mode*/
	TCCR1A &= ~_BV(WGM10);
    	TCCR1A |= _BV(WGM11);
	TCCR1B &= ~_BV(WGM12);
	TCCR1B |= _BV(WGM13);
	/*Set non inverting mode for PWM */
	TCCR1A &= ~_BV(COM1B0);
	TCCR1A |= _BV(COM1B1);
    	TCCR1A &= ~_BV(COM1A0);
	TCCR1A |= _BV(COM1A1);
	/* Define TOP value to obtain f = 50Hz */
	ICR1 = 625;
	/* Define duty cycle */
   	OCR1A = 45; //0, middle 
	OCR1B = 45; 	//0, middle 
 
	/*Set input pin for joystick key button*/
    	 GPIO_config_input_pullup(&PORTC, KEY_BUTTON);
    	/* ADC
     	* Configure ADC reference, clock source, enable ADC module, 
    	* and enable conversion complete interrupt */
     	ADMUX |= _BV(REFS0);
     	ADMUX &= ~_BV(REFS1);
     	ADMUX &= ~_BV(MUX0);
     	ADMUX &= ~_BV(MUX1);
     	ADMUX &= ~_BV(MUX2);
     	ADMUX &= ~_BV(MUX3);
     	ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
     	ADCSRA |= _BV(ADIE);
     	ADCSRA |= _BV(ADEN);
 
 	/* UART: asynchronous, 8-bit data, no parity, 1-bit stop,
	 * for debugging purposes */
    	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    	
	// Enables interrupts by setting the global interrupt mask
    	sei();


    	// Infinite loop
    	for (;;) { 
    	}

    	// Will never reach this
    	return (0);
}

/**
 * Brief: Timer0 overflow interrupt routine. Updtae state of the FSM.
 */
ISR(TIMER1_OVF_vect)
{
    fsm_ADC();
}

/**
 * Brief: ADC complete interrupt routine. Set duty cycle value of the
 * 	  PWM signal(s) according to its value and the operating mode. 
 */
ISR(ADC_vect)
{
    uint16_t value = 0;
    char uart_string[30];
   // Read 10-bit ACD value
   value = ADC;
   switch (mode){
    case 0:
        if (is_x){
            OCR1B = value /17 + 15;
        }
        else{
            OCR1A = value /17 + 15;
        }
        //Update UART transmiter
    itoa(mode, uart_string , 10);
    uart_puts(uart_string); 
        break;
    case 1:
        if (is_x == 1){
            OCR1B = value /17 + 15;
            OCR1A = 45;
        }
        //Update UART transmiter
    itoa(mode, uart_string , 10);
    uart_puts(uart_string); 
        break;
    case 2:
        if (is_x == 0){
            OCR1A = value /17 + 15;
            OCR1B = 45;
        }
        //Update UART transmiter
    itoa(mode, uart_string , 10);
    uart_puts(uart_string); 
        break;
    default:
        break;
   }
  
  
}
/* Functions ---------------------------------------------------------*/
/**
 * Brief : Finite State Machine. Switch ADC conversion between X and Y
 *         channels.
 * Input:  None
 * Return: None
 */
void fsm_ADC(void)
{
    switch (current_state) {
    case IDLE:
	if(GPIO_read(&PINC, KEY_BUTTON) == 0){
	    //Count how long the push button signal has been low
            debounce++; 
	    //The signal is considered stable and the button pressed
            if(debounce>2){
                debounce = 0; 
            	mode++;
	    }
            if(mode > 3){
                mode = 0;
            }
	}
        current_state = X_CONVERSION;
        break;
    
    case X_CONVERSION:
        //Select channel 0 for X conversion
	ADMUX &= ~_BV(MUX0);
	ADMUX &= ~_BV(MUX1);
	ADMUX &= ~_BV(MUX2);
	ADMUX &= ~_BV(MUX3);

	is_x = 1;
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

	is_x = 0;
	//Start ADC conversion
	ADCSRA |= _BV(ADSC);
        current_state = IDLE;
        break;

    default:
        current_state = IDLE;
    }
}

