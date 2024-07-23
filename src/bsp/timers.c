#include <stdint.h>
#include "hal_data.h"
#include "core_cm23.h"
#include "timers.h"
#include "adc.h"
#include "services/timestamp.h"

#define NUM_SAMPLES 500

static volatile uint8_t prime_adc_interrupt = 0;
static timestamp_t tick = 0;
static volatile size_t voltage_samples_index = 0;
static volatile uint16_t voltage_samples[NUM_SAMPLES] = { 0 };

static volatile uint16_t timers_counters[TIMERS_MAX_TIMERS] = { 0 };
static void (*timers_callbacks[TIMERS_MAX_TIMERS])(void*) = {NULL };
static volatile void *timers_args[TIMERS_MAX_TIMERS] = { NULL };

void bsp_timers_init(void) {
    SysTick_Config(SystemCoreClock / 1000); //1000 is the number of ticks per second
    NVIC_SetPriority(SysTick_IRQn, 0); // Set User configured Priority for Systick Interrupt, 0 is the interrupt priority

    R_AGT_Open(&timer_100us_ctrl, &timer_100us_cfg);
    R_AGT_Start(&timer_100us_ctrl);


     //R_BSP_IrqDisable(AGT0_INT_IRQn);
}

void SysTick_Handler(void) {
    tick++;
}

timestamp_t bsp_timers_get_millis(void) {
    return tick;
}

void bsp_timers_prime_adc_interrupt(void) {
    prime_adc_interrupt = 1;
}

void bsp_timers_set_callback(uint16_t timer_index, uint16_t counter,
                             void (*callback)(void *arg), void *arg) {
    timers_counters[timer_index] = 0;
    timers_callbacks[timer_index] = callback;
    timers_args[timer_index] = arg;
    timers_counters[timer_index] = counter;
}

void timer_100us_callback(timer_callback_args_t *p_args) {
    (void) p_args;

    /*
     if (prime_adc_interrupt) {
     prime_adc_interrupt = 0;
     } else {
     R_BSP_IrqClearPending(ADC0_WINDOW_A_IRQn);
     R_BSP_IrqEnable(ADC0_WINDOW_A_IRQn);
     }
     */

    (void) R_ADC_ScanStart(&g_adc0_ctrl);

    static uint16_t divider = 0;
    divider++;
    if (divider >= 2) {
        voltage_samples[voltage_samples_index] = bsp_adc_get(BSP_ADC_T);
        voltage_samples_index = (voltage_samples_index + 1) % NUM_SAMPLES;

        const uint16_t ticks = 100;
        for (uint16_t i = 0; i < TIMERS_MAX_TIMERS; i++) {
            if (timers_counters[i] > ticks) {
                timers_counters[i] -= ticks;
            } else if (timers_counters[i] > 0) {
                timers_counters[i] = 0;
                if (timers_callbacks[i] != NULL) {
                    timers_callbacks[i]((void*) timers_args[i]);
                }
            }
        }
        divider = 0;
    }
}
