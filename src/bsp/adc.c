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

    /* triggers by calling R_ADC_ScanStart(). */
    (void) R_ADC_ScanStart(&g_adc0_ctrl);

    // link timer to adc
    R_ELC_Open(&g_elc_ctrl, &g_elc_cfg);
    R_ELC_LinkSet(&g_elc_ctrl, ELC_PERIPHERAL_ADC0, ELC_EVENT_GPT4_COUNTER_OVERFLOW);
    R_ELC_Enable(&g_elc_ctrl);

}

uint16_t bsp_adc_get(bsp_adc_t adc) {
    adc_channel_t channels[BSP_ADC_NUM] = { ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_9, ADC_CHANNEL_10 };
    uint16_t adc_data = 0;
    R_ADC_Read(&g_adc0_ctrl, channels[adc], &adc_data);
    return adc_data;
}

#define NUM_SAMPLES 500
static volatile size_t voltage_samples_index = 0;
static volatile uint16_t voltage_samples[NUM_SAMPLES] = { 0 };

typedef enum
{
    det_search_FP,
    det_found_FP,
    det_search_FN,
    det_found_FN,
} e_detector_states;
e_detector_states L1_detector_state = det_search_FN;
e_detector_states L2_detector_state = det_search_FN;
e_detector_states L3_detector_state = det_search_FN;

#define MAINS_SAMPLES   8
uint16_t L1_history[MAINS_SAMPLES];
uint16_t L2_history[MAINS_SAMPLES];
uint16_t L3_history[MAINS_SAMPLES];
uint16_t mains_zero_th = 10;

int fl_stop_save=0;

void adc_sample_callback(adc_callback_args_t *p_arg) {
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_LOW);

    switch (p_arg->event) {
        case ADC_EVENT_SCAN_COMPLETE: {
            int i;

//            if (!fl_stop_save)
//            {
//                voltage_samples[voltage_samples_index] = bsp_adc_get(BSP_ADC_R);
//                voltage_samples_index = (voltage_samples_index + 1) % NUM_SAMPLES;
//            }

            // check for zero cross for L1
            // shift buffer
            for (i=MAINS_SAMPLES-1; i>0; i--)
            {
                L1_history[i] = L1_history[i-1];
            }
            L1_history[0] = bsp_adc_get(BSP_ADC_R);

            switch (L1_detector_state)
            {
                case det_search_FN:
                    if (L1_history[0] < mains_zero_th &&
                            L1_history[1] < mains_zero_th &&
                            L1_history[2] < mains_zero_th &&
                            L1_history[3] < mains_zero_th
                       )
                    {
                        L1_detector_state = det_found_FN;
                    }
                    break;
                case det_found_FN:
                    L1_detector_state = det_search_FP;
                    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_HIGH);
                    break;
                case det_search_FP:
                    if (L1_history[0] > mains_zero_th &&
                            L1_history[1] > mains_zero_th &&
                            L1_history[2] > mains_zero_th &&
                            L1_history[3] > mains_zero_th
                       )
                    {
                        L1_detector_state = det_found_FP;
                    }
                    break;
                case det_found_FP:
                    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_HIGH);
                    L1_detector_state = det_search_FN;
                    break;
            }

            // check for zero cross for L2
            for (i=MAINS_SAMPLES-1; i>0; i--)
            {
                L2_history[i] = L2_history[i-1];
            }
            L2_history[0] = bsp_adc_get(BSP_ADC_S);

            switch (L2_detector_state)
            {
                case det_search_FN:
                    if (L2_history[0] < mains_zero_th &&
                            L2_history[1] < mains_zero_th &&
                            L2_history[2] < mains_zero_th &&
                            L2_history[3] < mains_zero_th
                       )
                    {
                        L2_detector_state = det_found_FN;
                    }
                    break;
                case det_found_FN:
                    L2_detector_state = det_search_FP;
                    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_HIGH);
                    break;
                case det_search_FP:
                    if (L2_history[0] > mains_zero_th &&
                            L2_history[1] > mains_zero_th &&
                            L2_history[2] > mains_zero_th &&
                            L2_history[3] > mains_zero_th
                       )
                    {
                        L2_detector_state = det_found_FP;
                    }
                    break;
                case det_found_FP:
                    L2_detector_state = det_search_FN;
                    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_HIGH);
                    break;
            }

            // check for zero cross for L3
            for (i=MAINS_SAMPLES-1; i>0; i--)
            {
                L3_history[i] = L3_history[i-1];
            }
            L3_history[0] = bsp_adc_get(BSP_ADC_T);

            switch (L3_detector_state)
            {
                case det_search_FN:
                    if (L3_history[0] < mains_zero_th &&
                            L3_history[1] < mains_zero_th &&
                            L3_history[2] < mains_zero_th &&
                            L3_history[3] < mains_zero_th
                       )
                    {
                        L3_detector_state = det_found_FN;
                    }
                    break;
                case det_found_FN:
                    L3_detector_state = det_search_FP;
                    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_HIGH);
                    break;
                case det_search_FP:
                    if (L3_history[0] > mains_zero_th &&
                            L3_history[1] > mains_zero_th &&
                            L3_history[2] > mains_zero_th &&
                            L3_history[3] > mains_zero_th
                       )
                    {
                        L3_detector_state = det_found_FP;
                    }
                    break;
                case det_found_FP:
                    L3_detector_state = det_search_FN;
                    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_HIGH);
                    break;
            }

            if (L1_detector_state == det_found_FN)
            {
                bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_R);
            }
            if (L1_detector_state == det_found_FP)
            {
                bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_R);
            }

            if (L2_detector_state == det_found_FN)
            {
                bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_S);
            }
            if (L2_detector_state == det_found_FP)
            {
                bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_S);
            }

            if (L3_detector_state == det_found_FN)
            {
                bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_T);
            }
            if (L3_detector_state == det_found_FP)
            {
                bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_T);
            }

            timer_100us_callback(NULL);
            break;
        }

        case ADC_EVENT_WINDOW_COMPARE_A: {
//            volatile adc_channel_t channel = p_arg->channel;
//            uint16_t adc_data = 0;
//            (void)R_ADC_Read(&g_adc0_ctrl, channel, &adc_data);
//
//            uint8_t below = ((g_adc0_ctrl.p_reg->ADCMPLR[0] & (1 << channel)) == 0);
//            // ADC reached 0
//            if (below) {
//                g_adc0_ctrl.p_reg->ADCMPLR[0] |= (uint16_t) ((1 << channel) & 0xFFFF);
//            }
//            // ADC rose up
//            else {
//                g_adc0_ctrl.p_reg->ADCMPLR[0] &= (uint16_t) (~(1U << channel) & 0xFFFF);
//                switch (channel) {
//                    case ADC_CHANNEL_5:
//                        bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_R);
//                        break;
//                    case ADC_CHANNEL_6:
//                        bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_S);
//                        break;
//                    case ADC_CHANNEL_9:
//                        bsp_phase_cut_prime(BSP_PHASE_CUT_PHASE_T);
//                        break;
//                    default:
//                        break;
//                }
//            }
//
            __NOP();
            __NOP();
            break;
        }

        case ADC_EVENT_WINDOW_COMPARE_B:
            __NOP();
            __NOP();
            break;

        default:
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            break;
    }
//    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN, BSP_IO_LEVEL_LOW);

    return;
}
