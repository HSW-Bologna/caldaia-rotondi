#include <stdint.h>
#include "hal_data.h"
#include "adc.h"
#include "phase_cut.h"


void bsp_adc_init(void) {
    fsp_err_t ret = FSP_SUCCESS;
    ret = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    assert(FSP_SUCCESS == ret);

    ret = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
    assert(FSP_SUCCESS == ret);

    R_BSP_IrqDisable(ADC0_SCAN_END_IRQn);

    /* triggers by calling R_ADC_ScanStart(). */
    (void) R_ADC_ScanStart(&g_adc0_ctrl);
}

uint16_t bsp_adc_get(bsp_adc_t adc) {
    adc_channel_t channels[BSP_ADC_NUM] = { ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_9, ADC_CHANNEL_10 };
    uint16_t adc_data = 0;
    R_ADC_Read(&g_adc0_ctrl, channels[adc], &adc_data);
    return adc_data;
}

void adc_sample_callback(adc_callback_args_t *p_arg) {
    switch (p_arg->event) {
        case ADC_EVENT_SCAN_COMPLETE: {
            //(void)R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_10, &adc_data);
            __NOP();
            break;
        }

        case ADC_EVENT_WINDOW_COMPARE_A: {
            R_BSP_IrqDisable(ADC0_WINDOW_A_IRQn);

            volatile adc_channel_t channel = p_arg->channel;
            uint16_t adc_data = 0;

            (void)R_ADC_Read(&g_adc0_ctrl, channel, &adc_data);
            uint8_t below = (g_adc0_ctrl.p_reg->ADCMPLR[0]
                    & (1 << channel)) == 0;
            // ADC reached 0
            if (below) {
                g_adc0_ctrl.p_reg->ADCMPLR[0] |= (uint16_t) ((1
                        << channel) & 0xFFFF);
            }
            // ADC rose up
            else {
                g_adc0_ctrl.p_reg->ADCMPLR[0] &= (uint16_t) (~(1U
                        << channel) & 0xFFFF);
                switch (channel) {
                    case ADC_CHANNEL_5:
                        bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_R1);
                        break;
                    case ADC_CHANNEL_6:
                        bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_S);
                        break;
                    case ADC_CHANNEL_9:
                        bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_T);
                        break;
                    default:
                        break;
                }
            }

            break;
        }

        case ADC_EVENT_WINDOW_COMPARE_B:
            __NOP();
            break;

        default:
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            break;
    }

    return;
}
