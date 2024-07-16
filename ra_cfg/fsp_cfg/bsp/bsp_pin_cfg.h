/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define R1_AN (BSP_IO_PORT_00_PIN_10) /* Analog channel for the R line */
#define S_AN (BSP_IO_PORT_00_PIN_11) /* Analog channel for the S line */
#define T_AN (BSP_IO_PORT_00_PIN_14) /* Analog channel for the T line */
#define PRESSIONE (BSP_IO_PORT_00_PIN_15) /* Pressure sensor */
#define DIR (BSP_IO_PORT_01_PIN_02) /* RS485 direction line */
#define BSP_PIN_PWM3 (BSP_IO_PORT_01_PIN_03) /* Phase cut phase 3 */
#define IN2 (BSP_IO_PORT_01_PIN_09) /* Digital input 2 */
#define RELE1 (BSP_IO_PORT_01_PIN_10) /* On board relay */
#define BSP_PIN_PWM2 (BSP_IO_PORT_01_PIN_11) /* Phase cut phase 2 */
#define BSP_PIN_PWM1 (BSP_IO_PORT_01_PIN_12) /* Phase cut pin 1 */
#define IN1 (BSP_IO_PORT_02_PIN_00) /* Digital input 1 */
#define BSP_PIN_RUN (BSP_IO_PORT_02_PIN_01) /* On board LED */
extern const ioport_cfg_t g_bsp_pin_cfg; /* R7FA2E2A32DNK.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
