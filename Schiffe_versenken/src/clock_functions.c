#include <stm32f0xx.h>
#include "epl_clock.h"
#include "epl_usart.h"
#include "registers.h"



void TIM2_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Timer 2 Clock activating

    TIM2->PSC = 47999; // Prescaler set to  47999 for 1 kHz timer frequence when System-Clock is 48 MHz)
    TIM2->ARR = 1999; //setting  Auto-Reload-Register to 1999  for the interval of 2 sec
    TIM2->DIER |= TIM_DIER_UIE; // activating Update Interrupt 
    TIM2->CR1 |= TIM_CR1_URS; // seting the Update Request Source URS bit in the Control Register 1 CR1 of the Timer 2 TIM2 peripheral. 
                                // only an overflow event will trigger an update interrupt.

    NVIC_EnableIRQ(TIM2_IRQn); // Timer 2 Interrupt im NVIC einschalten
}

// Timer 2 Interrupt-Handler
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) // Checking if Update Interrupt Flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF; // removing Update Interrupt Flag 

    }
}

// GPIO Configuration
void GPIO_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // activating GPIOA Clock 
}

void SystemClock_Config(void)
{
    // System Clock Configuration
    // HSI -> SYSCLK (48MHz)
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    RCC->CFGR |= RCC_CFGR_SW_HSI;
    while(!(RCC->CFGR & RCC_CFGR_SWS_HSI));

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // AHB prescaler = 1
    RCC->CFGR |= RCC_CFGR_PPRE_DIV1; // APB prescaler = 1

    RCC->CR &= ~RCC_CR_PLLON; // Disable PLL
    while(RCC->CR & RCC_CR_PLLRDY);

    RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLMUL12; // PLL x 12

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}