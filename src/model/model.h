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
#include "pid.h"
#include "services/timestamp.h"


typedef struct {
    uint16_t adc_r1;
    uint16_t adc_s;
    uint16_t adc_t;
    uint16_t adc_pressure;
    uint8_t override_duty_cycle;
    uint8_t overridden_duty_cycle;
    uint8_t power;

    uint16_t pressure_setpoint_decibar;
    uint16_t pid_error;
    uint16_t pid_output;
    float pid_kp;
    float pid_ki;
    float pid_kd;
    pid_ctrl_t pid;

    timestamp_t last_communication_ts;
} mut_model_t ;

typedef const mut_model_t model_t;


void model_init(mut_model_t *p_model);
void model_set_pressure(mut_model_t *model, uint16_t pressure_millibar, uint16_t pressure_adc);
void model_tune_pid(mut_model_t *model);


#endif /* MODEL_MODEL_H_ */
