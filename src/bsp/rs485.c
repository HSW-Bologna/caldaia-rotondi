#include <services/timestamp.h>
#include <stdint.h>
#include <stdlib.h>
#include "hal_data.h"
#include "timers.h"
#include "rs485.h"

static void check_timeout(void);

static size_t receive_index = 0;
static uint8_t receive_buffer[BSP_RS485_MAX_PACKET_LEN] = { 0 };
static uint32_t last_reception_timestamp = 0;

void bsp_rs485_init(void) {
    g_uart9.p_api->open(g_uart9.p_ctrl, g_uart9.p_cfg);
}

void bsp_rs485_write(uint8_t *data, size_t len) {
    g_uart9.p_api->write(g_uart9.p_ctrl, data, len);
}

size_t bsp_rs485_read(uint8_t *data, size_t max_len) {
    size_t total_bytes_read = receive_index > max_len ? max_len : receive_index;
    memcpy(data, receive_buffer, total_bytes_read);
    return total_bytes_read;
}

void bsp_rs485_flush(void) {
    receive_index = 0;
}

void rs485_callback(uart_callback_args_t *args) {
    switch (args->event) {
        case UART_EVENT_RX_COMPLETE:
            break;

        case UART_EVENT_RX_CHAR: {
            check_timeout();
            if (receive_index < BSP_RS485_MAX_PACKET_LEN) {
                receive_buffer[receive_index] = (uint8_t)args->data;
                receive_index++;
            }
            break;
        }

        case UART_EVENT_TX_DATA_EMPTY:
            break;

        default:
            break;
    }

}

static void check_timeout(void) {
    if (receive_index > 0
            && timestamp_is_expired(last_reception_timestamp,
                                    bsp_timers_get_millis(), 200)) {
        receive_index = 0;
    }

}
