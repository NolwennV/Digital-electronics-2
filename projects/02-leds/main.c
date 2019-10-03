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
    DDRB |= _BV(LED_PIN);           /* DDRB = DDRB or (0000 0001) */
    DDRB |= _BV(LED_PIN2);          /* DDRB = DDRB or (0010 0000) */
    DDRD &= ~_BV(PUSH_BUTTON);      /* DDRD = DDRD or (0000 0010) */

    /* Turn LED off */
    PORTB &= ~_BV(LED_PIN);         /* PORTB = PORTB and (0000 0001) */
    PORTB &= ~_BV(LED_PIN2);        /* PORTB = PORTB and (0010 0000) */
    PORTD |= _BV(PUSH_BUTTON);

    /* Infinite loop */
    for (;;)
    {
        //blink();
        if(bit_is_clear(PIND,2)){
          blink();
        }

    }

    return (0);
}
/* Functions ---------------------------------------------------------*/
/**
  * Brief:  Blink a LED with delay function.
  * Input:  None
  * Return: None
  */
 void blink (){
   /* Invert LED and delay */
        PORTB ^= _BV(LED_PIN);      /* PORTB = PORTB xor (0010 0000) */
        _delay_ms(BLINK_DELAY);
        PORTB ^= _BV(LED_PIN);     /* Wait for several milisecs */
        PORTB ^= _BV(LED_PIN2);      
        _delay_ms(BLINK_DELAY);
        PORTB ^= _BV(LED_PIN2);
 }