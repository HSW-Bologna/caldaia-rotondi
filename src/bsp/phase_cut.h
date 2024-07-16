/*
 * phase_cut.h
 *
 *  Created on: 16 Jul 2024
 *      Author: Maldo
 */

#ifndef BSP_PHASE_CUT_H_
#define BSP_PHASE_CUT_H_


typedef enum {
    BSP_PHASE_CUT_PHASE_R1 = 0,
    BSP_PHASE_CUT_PHASE_S ,
    BSP_PHASE_CUT_PHASE_T ,
#define BSP_PHASE_CUT_PHASE_NUM 3
} bsp_phase_cut_phase_t;


void bsp_phase_cut_prime(bsp_phase_cut_phase_t phase);
void bsp_phase_cut_set_percentage(uint8_t percentage);

#endif /* BSP_PHASE_CUT_H_ */
