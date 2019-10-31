/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-16
 * Last update: 2019-10-25
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Decimal counter with data output on LCD display.
 * 
 * Note:
 *    Modified version of Peter Fleury's LCD library with R/W pin 
 *    connected to GND. Newline symbol "\n" is not implemented, use
 *    lcd_gotoxy() function instead.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>             // itoa() function
#include "timer.h"
#include "lcd.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
uint16_t  counter = 0;
uint8_t  x_bar = 0;

/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on LCD display.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    /* LCD display
     * TODO: See Peter Fleury's online manual for LCD library 
     * http://homepage.hispeed.ch/peterfleury/avr-software.html
     * Initialize display and test different types of cursor */
    lcd_init(LCD_DISP_ON);

    // Display string without auto linefeed
    /* lcd_puts("LCD TEST");
    lcd_gotoxy(x,y);
    lcd_putc('A');
    lcd_home();
    lcd_putc('A'); */

    // TODO: Display variable value in decimal, binary, and hexadecimal
    
    
    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    TIM_config_prescaler(TIM0, TIM_PRESC_8);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);
    /* TODO: Design at least two user characters and store them in 
     *       the display memory */
    lcd_command(1<<LCD_CGRAM);
    uint8_t  lcd_user_symbols [8*6] = {0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111,
                                       0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
                                       0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
                                       0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
                                       0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
                                       0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110};
    
        for(int i = 0; i < 48;i++){
            lcd_data(lcd_user_symbols[i]);
        }
    
    /* lcd_clrscr();
    lcd_putc(0x00);
    lcd_putc(0x01);
    lcd_putc(0x02); */

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    for (;;) {
        
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Increment counter value.
 */
ISR(TIMER1_OVF_vect)
{
    counter++;
    x_bar++;
    if (counter > 16){
        counter = 0;
        x_bar = 0;
    }
}
ISR(TIMER0_OVF_vect)
{
    /* uint8_t x,y = 0;
    char lcd_string [9];

    lcd_home();

    itoa(counter, lcd_string , 10);
    lcd_puts("Counter:");
    x = 10;
    y = 0;
    lcd_gotoxy(x,y);
    lcd_puts(lcd_string);
    itoa(counter, lcd_string , 16);
    x = 0;
    y = 1;
    lcd_gotoxy(x,y);
    lcd_puts("0x");
    lcd_puts(lcd_string);
    itoa(counter, lcd_string , 2);
    x = 6;
    y = 1;
    lcd_gotoxy(x,y);
    lcd_puts("0b");
    lcd_puts(lcd_string); */ 
    uint8_t x,y = 0;
    char lcd_string [9];
    if (counter == 0){
        lcd_clrscr();
    }
    lcd_home();
    itoa(counter, lcd_string , 10);
    lcd_puts("Counter:");
    x = 10;
    y = 0;
    lcd_gotoxy(x,y);
    lcd_puts(lcd_string);
    x = 0; y = 1;
    lcd_gotoxy(x,y);
    lcd_gotoxy(x_bar,y);
    lcd_putc(0x01); 
}