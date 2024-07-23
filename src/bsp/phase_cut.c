#include <stdint.h>
#include "phase_cut.h"
#include "hal_data.h"
#include "timers.h"

static void start_phase(void *arg);
static void stop_phase(void *arg);

static uint16_t phase_cut_percentage = 0;
static uint16_t pins[BSP_PHASE_CUT_PHASE_NUM] = { BSP_PIN_PWM1, BSP_PIN_PWM2, BSP_PIN_PWM3 };
static uint16_t heater_power_half_us[101] = {
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
        2106, 1716, 1212, 100        // max power 0
        };

void bsp_phase_cut_set_percentage(uint8_t percentage) {
    if (percentage > 100) {
        phase_cut_percentage = 100;
    } else {
        phase_cut_percentage = percentage;
    }
}

void bsp_phase_cut_prime(bsp_phase_cut_phase_t phase) {
    /*
    static uint8_t blinks[3] = {0,0,0};
    if (blinks[phase]) {
        start_phase((void*) (uintptr_t) phase);
    } else {
        stop_phase((void*) (uintptr_t) phase);
    }
    blinks[phase] = !blinks[phase];
    return;
    */

    // Full steam ahead
    if (phase_cut_percentage == 100) {
        bsp_timers_set_callback(phase, 0, NULL, NULL);
        bsp_timers_set_callback(phase + 3, 0, NULL, NULL);
        start_phase((void*) (uintptr_t) phase);
    }
    // A non-null percentage but still regulated
    else if (phase_cut_percentage > 0) {
        uint16_t period_us = heater_power_half_us[phase_cut_percentage] / 2;
        bsp_timers_set_callback(phase, period_us, start_phase,
                                (void*) (uintptr_t) phase);
        bsp_timers_set_callback(phase + 3, period_us + 200, stop_phase,
                                (void*) (uintptr_t) phase);
        stop_phase((void*) (uintptr_t) phase);
    }
    // Phase cut completely off
    else {
        bsp_timers_set_callback(phase, 0, NULL, NULL);
        bsp_timers_set_callback(phase + 3, 0, NULL, NULL);
        stop_phase((void*) (uintptr_t) phase);
    }
}

static void start_phase(void *arg) {
    bsp_phase_cut_phase_t phase = (bsp_phase_cut_phase_t) (uintptr_t) arg;
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, pins[phase], BSP_IO_LEVEL_HIGH);
}

static void stop_phase(void *arg) {
    bsp_phase_cut_phase_t phase = (bsp_phase_cut_phase_t) (uintptr_t) arg;
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, pins[phase], BSP_IO_LEVEL_LOW);
}
