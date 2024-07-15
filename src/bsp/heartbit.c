#include <services/timestamp.h>
#include "heartbit.h"
#include "hal_data.h"
#include "timers.h"

void bsp_heartbit_manage(void) {
    static timestamp_t timestamp = 0;
    static uint8_t blink = 0;

    if (blink) {
        if (timestamp_is_expired(timestamp, bsp_timers_get_millis(), 50)) {
            g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN,
                                     BSP_IO_LEVEL_LOW);
            blink = !blink;
            timestamp = bsp_timers_get_millis();
        }
    } else {
        if (timestamp_is_expired(timestamp, bsp_timers_get_millis(), 2000)) {
            g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_PIN_RUN,
                                     BSP_IO_LEVEL_HIGH);
            blink = !blink;
            timestamp = bsp_timers_get_millis();
        }
    }
}
