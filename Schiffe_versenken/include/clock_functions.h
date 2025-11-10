#ifndef CLOCK_FUNCTIONS_H
#define CLOCK_FUNCTIONS_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f0xx.h"  

void TIM2_Init(void);
void TIM2_IRQHandler(void);
void GPIO_Init(void);
void SystemClock_Config(void);



#endif 
