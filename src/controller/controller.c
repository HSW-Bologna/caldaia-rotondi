#include "adapters/modbus_server.h"
#include "controller.h"
#include "services/timestamp.h"
#include "bsp/timers.h"
#include "bsp/rs485.h"
#include "observer.h"
#include "model/model.h"


void controller_init(mut_model_t *model) {
    modbus_server_init();
    observer_init(model);
}


void controller_manage(mut_model_t *model) {
    static unsigned long ts = 0;

    modbus_server_manage(model);
    observer_manage(model);

    if (timestamp_is_expired(ts, bsp_timers_get_millis(), 1000)) {
        uint16_t pressure_adc = bsp_pressure_get_adc();
        model_set_pressure(model, pressure_adc, bsp_pressure_convert_to_millibar(pressure_adc));
        ts = bsp_timers_get_millis();
    }
}
