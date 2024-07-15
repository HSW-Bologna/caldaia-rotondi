#include "model.h"


void model_init(mut_model_t *p_model) {
    p_model->adc_r1 = 0;
    p_model->adc_s = 0;
    p_model->adc_t = 0;
    p_model->override_duty_cycle = 0;
    p_model->overridden_duty_cycle = 0;
}
