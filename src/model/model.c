#include "model.h"
#include "pid.h"
#include "config/app_config.h"
#include "services/timestamp.h"
#include "bsp/timers.h"


void model_init(mut_model_t *model) {
    model->adc_r1 = 0;
    model->adc_s = 0;
    model->adc_t = 0;
    model->adc_pressure = 0;

    model->power = 0;
    model->override_duty_cycle = 0;
    model->overridden_duty_cycle = 0;

    model->pressure_setpoint_decibar = 0;
    model->pid_kp = APP_CONFIG_DEFAULT_PID_KP;
    model->pid_ki = APP_CONFIG_DEFAULT_PID_KI;
    model->pid_kd = APP_CONFIG_DEFAULT_PID_KD;

    pid_init(&model->pid);
    model_tune_pid(model);
}


void model_set_pressure(mut_model_t *model, uint16_t pressure_millibar, uint16_t pressure_adc) {
    model->adc_pressure = pressure_adc;
    pid_add_input(&model->pid, pressure_millibar/100);
    model->pid_error = pid_current_error(&model->pid);
    model->pid_output = pid_compute(&model->pid);
}


void model_tune_pid(mut_model_t *model) {
    pid_ctrl_tune(&model->pid, model->pid_kp, model->pid_ki, model->pid_kd);
}


uint16_t model_get_output_percentage(model_t *model) {
    if (model->power) {
        if (timestamp_is_expired(model->last_communication_ts, bsp_timers_get_millis(), 4000)) {
            if (model->override_duty_cycle) {
                return model->overridden_duty_cycle;
            } else {
                return model->pid_output;
            }
        } else {
                return 0;
        }
    } else{
       return 0;
    }
}


void model_communication_ping(mut_model_t *model) {
    model->last_communication_ts = bsp_timers_get_millis();
}
