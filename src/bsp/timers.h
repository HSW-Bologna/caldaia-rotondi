/*
 * timers.h
 *
 *  Created on: 27 Jun 2024
 *      Author: Maldus
 */

#ifndef BSP_TIMERS_H_
#define BSP_TIMERS_H_


#include <stdint.h>
#include "services/timestamp.h"


#define TIMERS_MAX_TIMERS 6


timestamp_t bsp_timers_get_millis(void);
void bsp_timers_init(void);
void bsp_timers_set_callback(uint16_t timer_index, uint16_t counter, void (*callback)(void* arg), void*arg);


#endif /* BSP_TIMERS_H_ */
