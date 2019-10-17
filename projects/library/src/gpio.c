#include "gpio.h"

#define DDR(x) (*(&x -1))

void GPIO_config_output (volatile uint8_t *reg_name, uint8_t pin_num){

    *reg_name |= _BV(pin_num);           
}

void GPIO_config_input_nopull (volatile uint8_t *reg_name, uint8_t pin_num){

    *reg_name &= ~_BV(pin_num);
    DDR(*reg_name) &= ~_BV(pin_num);

}

void GPIO_config_input_pullup (volatile uint8_t *reg_name, uint8_t pin_num){
    
    *reg_name |= _BV(pin_num);
    DDR(*reg_name) &= ~_BV(pin_num);
} 

void GPIO_write (volatile uint8_t *reg_name, uint8_t pin_num, uint8_t pin_val){
    
     if (pin_val == PIN_LOW)
        *reg_name &=~ _BV(pin_num);
    else
        *reg_name |= _BV(pin_num);
}

void GPIO_toggle (volatile uint8_t *reg_name, uint8_t pin_num){
    
    *reg_name ^= _BV(pin_num);
}

uint8_t GPIO_read (volatile uint8_t *reg_name, uint8_t pin_num){
     /* if(bit_is_clear(*reg_name, pin_num)
        return 0;
    else
        return 1; */
    
    return ((*reg_name & _BV(pin_num)) >> pin_num ); //correction
}