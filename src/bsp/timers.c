#include <stdint.h>
#include "hal_data.h"
#include "core_cm23.h"
#include "timers.h"


static uint32_t tick = 0;


void bsp_timers_init(void) {
    SysTick_Config(SystemCoreClock / 1000); //1000 is the number of ticks per second
    NVIC_SetPriority(SysTick_IRQn, 0); // Set User configured Priority for Systick Interrupt, 0 is the interrupt priority
}


void SysTick_Handler(void) {
    tick++;
}


uint32_t bsp_timers_get_millis(void) {
    return tick;
}
