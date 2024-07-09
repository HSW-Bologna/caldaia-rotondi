#define LIGHTMODBUS_SLAVE_FULL
#define LIGHTMODBUS_DEBUG
#define LIGHTMODBUS_IMPL
#include <lightmodbus/lightmodbus.h>
#include <stdint.h>
#include <assert.h>
#include "bsp/rs485.h"

static ModbusSlave slave;

ModbusError registerCallback(const ModbusSlave *slave,
                             const ModbusRegisterCallbackArgs *args,
                             ModbusRegisterCallbackResult *result) {
    switch (args->query) {
        // Pretend to allow all access
        case MODBUS_REGQ_R_CHECK:
        case MODBUS_REGQ_W_CHECK:
            result->exceptionCode = MODBUS_EXCEP_NONE;
            break;

            // Return 7 when reading
        case MODBUS_REGQ_R:
            result->value = 7;
            break;

        default:
            break;
    }

    return MODBUS_OK;
}

ModbusError exceptionCallback(const ModbusSlave *slave, uint8_t function,
                              ModbusExceptionCode code) {
    return MODBUS_OK;
}

void modbus_server_init(void) {
    // Read the data from stdin
    ModbusErrorInfo err = modbusSlaveInit(&slave, registerCallback,
                                          exceptionCallback,
                                          modbusDefaultAllocator,
                                          modbusSlaveDefaultFunctions,
                                          modbusSlaveDefaultFunctionCount);

    // Check for errors
    assert(modbusIsOk(err) && "modbusSlaveInit() failed!");
}

void modbus_server_manage(void) {
    uint8_t data[BSP_RS485_MAX_PACKET_LEN] = { 0 };
    size_t length = bsp_rs485_read(data, sizeof(data));

    if (length > 0) {
        ModbusErrorInfo err = modbusParseRequestPDU(&slave, data, length);
        if (modbusIsOk(err)) {
            bsp_rs485_flush();
        } else if (err.error == MODBUS_ERROR_LENGTH) {
            // Wait if more bytes are received
        } else {
            bsp_rs485_flush();
        }
    }
}
