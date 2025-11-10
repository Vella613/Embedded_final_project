

#include "registers.h"
#include <string.h>

volatile bool button_was_pressed = false;
volatile bool newline_rcvd = false;
volatile uint8_t data[100];  
volatile uint16_t data_idx = 0;

void resetBuffer(void) {
    newline_rcvd = false;
    memset(data, 0, sizeof(data)); 
    data_idx = 0;
}

void ADC_Init(void) {
   
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    
    GPIOA->MODER |= GPIO_MODER_MODER0;

    //Config ADC
    ADC1->CFGR1 &= ~ADC_CFGR1_RES; 
    ADC1->CHSELR = ADC_CHSELR_CHSEL0; 
    ADC1->CFGR1 |= ADC_CFGR1_CONT; 

    // Calibrate the ADC
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    //ADC conversion
    ADC1->CR |= ADC_CR_ADSTART;
}

void init_user_button(void) {
    //clock for GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    //PC13 input
    GPIOC->MODER &= ~(GPIO_MODER_MODER13);
}

void init_exti(void) {

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

    EXTI->IMR |= EXTI_IMR_MR13;

    // Set rising edge trigger
    EXTI->RTSR |= EXTI_RTSR_TR13;

    // Enable interrupt
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void EXTI4_15_IRQHandler(void) {
    // Check the interrupt
    if (EXTI->PR & EXTI_PR_PR13) {

        EXTI->PR |= EXTI_PR_PR13;

        // Set the flag
        button_was_pressed = true;
    }
}

void USART2_IRQHandler(void) {
    if (USART2->ISR & USART_ISR_RXNE) { 
        // checking if the bit for the Receive Data Register Not Empty (RXNE) 
        // in the Interrupt Status Register (ISR) of the USART2 module is set.
        // If  set, then a new character has been received 
        // and is available in the RDR  - Receive Data Register of the USART2 module.

        uint8_t saving_read_symbol = USART2->RDR;   

        // die Daten werden Durch COM3 erhalten und ueber den USART bekommen
        //  dann werden diese in einem Register RDR gespiechert und das was in der Received Data Register gespeichert wurde wird in saving_read_symbol gespcheirt

        // checking if the data index doesnt exceed boundaries -i.e. the data size..

        if (data_idx >= sizeof(data)) {
            return;
        }

        // here saving  the symbol in the array accordingly with its index
        data[data_idx] = saving_read_symbol;

        // increasing the index  to iterate the array
        data_idx++;
        if (saving_read_symbol == '\n') {
            // in case of new line, the newline_rcvd will be set as true.
            //  the purpose is to get into the switch case in weiterer Folge
            newline_rcvd = true;
        }
    }
}
