#include <stdint.h>
#include "hal_data.h"
#include "core_cm23.h"
#include "timers.h"
#include "adc.h"
#include "services/timestamp.h"

#define NUM_SAMPLES 500

static timestamp_t tick = 0;
static volatile size_t voltage_samples_index = 0;
static volatile uint16_t voltage_samples[NUM_SAMPLES] = { 0 };

uint16_t heater_power[100] = {
        19000,     //0 - off
        17788, 17284, 16894, 16564, 16272, 16007, 15761, 15531, 15315,
        15108,     //10
        14911, 14721, 14538, 14361, 14190, 14022, 13859, 13700, 13544,
        13392,     //20
        13242, 13095, 12950, 12807, 12667, 12528, 12391, 12255, 12121,
        11989,     //30
        11857, 11727, 11598, 11470, 11343, 11216, 11091, 10966, 10842,
        10718,     //40
        10595, 10472, 10349, 10227, 10106, 9984, 9863, 9742, 9621,
        9500,      //50
        9379, 9258, 9137, 9016, 8894, 8773, 8651, 8528, 8405,
        8282,      //60
        8158, 8034, 7909, 7784, 7657, 7530, 7402, 7273, 7143, 7011, 6879, 6745,
        6609, 6472, 6333, 6193, 6050, 5905, 5758, 5608, 5456, 5300, 5141, 4978,
        4810, 4639, 4462, 4279, 4089, 3892, 3685, 3469, 3239, 2993, 2728, 2436,
        2106, 1716, 1212, 100,         // max power 0
        };

void bsp_timers_init(void) {

    SysTick_Config(SystemCoreClock / 1000); //1000 is the number of ticks per second
    NVIC_SetPriority(SysTick_IRQn, 0); // Set User configured Priority for Systick Interrupt, 0 is the interrupt priority

    R_AGT_Open(&timer_100us_ctrl, &timer_100us_cfg);
    R_AGT_Start(&timer_100us_ctrl);
}

void SysTick_Handler(void) {
    tick++;
}

timestamp_t bsp_timers_get_millis(void) {
    return tick;
}

void timer_100us_callback(timer_callback_args_t *p_args) {
    (void) p_args;
    R_BSP_IrqEnable(ADC0_WINDOW_A_IRQn);
    voltage_samples[voltage_samples_index] = bsp_adc_get(BSP_ADC_R1);
    voltage_samples_index = (voltage_samples_index + 1) % NUM_SAMPLES;
}
