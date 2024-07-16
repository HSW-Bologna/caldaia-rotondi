#include "adapters/modbus_server.h"
#include "controller.h"
#include "services/timestamp.h"
#include "bsp/timers.h"
#include "bsp/rs485.h"
#include "observer.h"
#include "model/model.h"


void controller_init(mut_model_t *pmodel) {
    modbus_server_init();
    observer_init(pmodel);
}


void controller_manage(mut_model_t *pmodel) {
    static unsigned long ts = 0;

    modbus_server_manage(pmodel);
    observer_manage(pmodel);

    if (timestamp_is_expired(ts, bsp_timers_get_millis(), 1000)) {
        ts = bsp_timers_get_millis();
    }
}
