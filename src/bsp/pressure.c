#include <stdint.h>
#include "hal_data.h"
#include "services/timestamp.h"
#include "timers.h"
#include "adc.h"
#include "pressure.h"


#define NUM_SAMPLES 10


static volatile timestamp_t timestamp = 0;
static size_t samples_index = 0;
static uint16_t samples[NUM_SAMPLES] = {0};


void bsp_pressure_manage(void) {
    if (timestamp_is_expired(timestamp, bsp_timers_get_millis(), 100)) {

        samples[samples_index] = bsp_adc_get(BSP_ADC_PRESSURE);
        samples_index = (samples_index+1) % NUM_SAMPLES;

        timestamp = bsp_timers_get_millis();
    }
}


uint16_t bsp_pressure_get_adc(void) {
    uint32_t total = 0;
    for (size_t i = 0; i < NUM_SAMPLES; i++) {
        total += samples[i];
    }

    return (uint16_t)(total/NUM_SAMPLES);
}


uint16_t bsp_pressure_convert_to_millibar(uint16_t adc) {
    return adc;
}
