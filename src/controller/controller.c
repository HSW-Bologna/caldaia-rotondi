#include "adapters/modbus_server.h"
#include "controller.h"
#include "services/timestamp.h"
#include "bsp/timers.h"
#include "bsp/rs485.h"

void controller_init(void) {
    modbus_server_init();
}

void controller_manage(mut_model_t *p_model) {
    static unsigned long ts = 0;

    modbus_server_manage(p_model);

    if (timestamp_is_expired(ts, bsp_timers_get_millis(), 1000)) {
        ts = bsp_timers_get_millis();
    }
}
