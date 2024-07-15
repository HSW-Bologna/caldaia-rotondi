/*
 * model.h
 *
 *  Created on: 12 Jul 2024
 *      Author: Maldus
 */

#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_


#include <stdint.h>
#include <stdlib.h>


typedef struct {
    uint16_t adc_r1;
    uint16_t adc_s;
    uint16_t adc_t;
    uint8_t override_duty_cycle;
    uint8_t overridden_duty_cycle;
} mut_model_t ;

typedef const mut_model_t model_t;


void model_init(mut_model_t *p_model);


#endif /* MODEL_MODEL_H_ */
