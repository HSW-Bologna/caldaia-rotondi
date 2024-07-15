#include <stdint.h>
#include "hal_data.h"


static int adc_sample_ad_proc(uint16_t *const p_adc_data);


static volatile uint8_t adc_end_flg = 0;


void bsp_pressure_manage(void) {
    return;
    uint16_t adc_data = 0;
    adc_sample_ad_proc(&adc_data);
    __NOP();
    __NOP();
    __NOP();
}


static int adc_sample_ad_proc(uint16_t *const p_adc_data) {
    fsp_err_t ret = FSP_SUCCESS;

    ret = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    if (FSP_SUCCESS == ret) {
        ret = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
        if (FSP_SUCCESS == ret) {
            /* triggers by calling R_ADC_ScanStart(). */
            (void) R_ADC_ScanStart(&g_adc0_ctrl);

            /* Wait for conversion to complete. */
            while (!adc_end_flg) {
                /* wait A/D conversion end */
            }
            adc_end_flg = 0;

            /* Read converted data. */
            ret = R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_10, p_adc_data);
        }
    }

    /* ADC driver end processing */
    R_ADC_Close(&g_adc0_ctrl);

    if (FSP_SUCCESS == ret) {
        return 0;
    } else {
        return -1;
    }
}

/*
void adc_sample_callback(adc_callback_args_t *p_arg) {
    (void)(p_arg);
    adc_end_flg = true;

    return;
}
*/
