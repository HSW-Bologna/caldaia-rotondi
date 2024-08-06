#ifndef BSP_PRESSURE_H_INCLUDED
#define BSP_PRESSURE_H_INCLUDED


#include <stdint.h>


void bsp_pressure_manage(void);
uint16_t bsp_pressure_get_adc(void);
uint16_t bsp_pressure_convert_to_millibar(uint16_t adc);


#endif
