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


timestamp_t bsp_timers_get_millis(void);
void bsp_timers_init(void);


#endif /* BSP_TIMERS_H_ */
