
#ifndef SET_REGISTER_H
#define SET_REGISTER_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f0xx.h"  


extern volatile bool button_was_pressed;
extern volatile bool newline_rcvd;
extern volatile uint8_t data[];
extern volatile uint16_t data_idx;


void ADC_Init(void);
void init_user_button(void);
void init_exti(void);
void EXTI4_15_IRQHandler(void);
void USART2_IRQHandler(void);
void resetBuffer(void);

#endif // SET_REGISTER_H
