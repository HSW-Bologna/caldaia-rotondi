#include "adapters/modbus_server.h"
#include "controller.h"

void controller_init(void) {
    modbus_server_init();
}

void controller_manage(void) {
    modbus_server_manage();
}
