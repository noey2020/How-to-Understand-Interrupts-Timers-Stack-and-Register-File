#include "stm32l1xx.h"

volatile uint32_t msTicks = 0;        /* Variable to store millisecond ticks */

void SysTick_Handler(void)
{   /* SysTick interrupt Handler. */
    msTicks++;                        /* See startup file startup_LPC17xx.s for SysTick vector */
}

int main(void){
    uint32_t returnCode;

    returnCode = SysTick_Config(SystemCoreClock / 1000);    /* Configure SysTick to generate an interrupt every millisecond */

    if(returnCode != 0){                                    /* Check return code for errors */
    // Error Handling
    }
    while(1);
}
