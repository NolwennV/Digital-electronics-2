/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-09-27
 * Last update: 2019-07-16
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Blink a LED with delay function.
 * TODO: Verify functionality of LED blinker.
 * NOTE: Macro _BV converts a bit number into a byte value (see AVR Libc
 * Reference Manual).
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN      PB0
#define LED_PIN2     PB5
#define PUSH_BUTTON  PD2
#define BLINK_DELAY  250

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/
void blink();

/* Functions ---------------------------------------------------------*/
/**
  * Brief:  Main program. Blink a LED if the push button is pressed
  * Input:  None
  * Return: None
  */
int main(void)
{
    /* Set output pin */
    GPIO_config_output(&DDRB, LED_PIN);
    GPIO_config_output(&DDRB, LED_PIN2);
    GPIO_config_input_pullup(&PORTD, PUSH_BUTTON); 
       
    /* Turn LED off */
    GPIO_write(&PORTB, LED_PIN,0);         
    GPIO_write(&PORTB, LED_PIN,0);

    /* Infinite loop */
    for (;;)
    {
        if(GPIO_read(&PIND, PUSH_BUTTON) == 0){
          GPIO_toggle(&PORTB, LED_PIN);
          _delay_ms(BLINK_DELAY);
          GPIO_toggle(&PORTB, LED_PIN);
          GPIO_toggle(&PORTB, LED_PIN2);
          _delay_ms(BLINK_DELAY);
          GPIO_toggle(&PORTB, LED_PIN2);
        }
    }

    return (0);
}
