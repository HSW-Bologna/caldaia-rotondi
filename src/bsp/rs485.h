/*
 * rs485.h
 *
 *  Created on: 27 Jun 2024
 *      Author: Maldus
 */

#ifndef BSP_RS485_H_
#define BSP_RS485_H_

#define BSP_RS485_MAX_PACKET_LEN 255

void bsp_rs485_init(void);
void bsp_rs485_write(uint8_t *data, size_t len);
size_t bsp_rs485_read(uint8_t *data, size_t max_len);
void bsp_rs485_flush(void);

#endif /* BSP_RS485_H_ */
