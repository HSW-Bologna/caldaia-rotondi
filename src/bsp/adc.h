/*
 * adc.h
 *
 *  Created on: 11 Jul 2024
 *      Author: Maldus
 */

#ifndef BSP_ADC_H_
#define BSP_ADC_H_

typedef enum {
    BSP_ADC_R1 = 0,
    BSP_ADC_S,
    BSP_ADC_T,
    BSP_ADC_PRESSURE,
#define BSP_ADC_NUM 2
} bsp_adc_t;

void bsp_adc_init(void);
uint16_t bsp_adc_get(bsp_adc_t adc);

#endif /* BSP_ADC_H_ */
