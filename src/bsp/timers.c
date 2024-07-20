#include <stdint.h>
#include "hal_data.h"
#include "core_cm23.h"
#include "timers.h"
#include "adc.h"
#include "phase_cut.h"
#include "services/timestamp.h"



static timestamp_t tick = 0;

volatile uint16_t timers_counters[TIMERS_MAX_TIMERS] = { 0 };
void (*timers_callbacks[TIMERS_MAX_TIMERS])(void *) = { NULL };
volatile void *timers_args[TIMERS_MAX_TIMERS] = { NULL };

void bsp_timers_init(void) {
    SysTick_Config(SystemCoreClock / 1000); //1000 is the number of ticks per second
    NVIC_SetPriority(SysTick_IRQn, 0); // Set User configured Priority for Systick Interrupt, 0 is the interrupt priority

    R_GPT_Open(&g_timer4_ctrl, &g_timer4_cfg);
    R_GPT_Start(&g_timer4_ctrl);
}

void SysTick_Handler(void) {
    tick++;
}

timestamp_t bsp_timers_get_millis(void) {
    return tick;
}

void bsp_timers_set_callback(uint16_t timer_index, uint16_t counter, void (*callback)(void* arg), void*arg) {
    timers_counters[timer_index] = 0;
    timers_callbacks[timer_index] = callback;
    timers_args[timer_index] = arg;
    timers_counters[timer_index] = counter;
}

// now called by the adc irq
void timer_100us_callback(timer_callback_args_t *p_args) {
    (void) p_args;

    const uint16_t ticks = 100;
    for (uint16_t i = 0; i < TIMERS_MAX_TIMERS; i++) {
        if (timers_counters[i] > ticks) {
            timers_counters[i] -= ticks;
        } else if (timers_counters[i] > 0){
            timers_counters[i] = 0;
            if (timers_callbacks[i] != NULL) {
                timers_callbacks[i]((void*)timers_args[i]);
            }
        }
    }

}
