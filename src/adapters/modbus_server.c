#define LIGHTMODBUS_DEBUG
#define LIGHTMODBUS_IMPL
#define LIGHTMODBUS_F03S
#define LIGHTMODBUS_F06S
#define LIGHTMODBUS_F016S
#include <lightmodbus/lightmodbus.h>

#include "services/timestamp.h"
#include "services/app_config.h"
#include "modbus_swerver.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "bsp/rs485.h"

enum {
    MODBUS_HR_DEVICE_MODEL                   = 0,
    MODBUS_HR_FIRMWARE_VERSION       ,
    MODBUS_HR_INSTANT_ANALOG_VALUE_R1 ,
    MODBUS_HR_INSTANT_ANALOG_VALUE_S  ,
    MODBUS_HR_INSTANT_ANALOG_VALUE_T  ,
    MODBUS_HR_REQUIRED_DUTY_CYCLE     ,
};


ModbusError register_callback(const ModbusSlave *minion,
                             const ModbusRegisterCallbackArgs *args,
                             ModbusRegisterCallbackResult *result);
static ModbusError static_allocator(
    ModbusBuffer *buffer,
    uint16_t size,
    void *context);
static ModbusError exception_callback(const ModbusSlave *minion, uint8_t function,
                              ModbusExceptionCode code);


static ModbusSlave minion;


void modbus_server_init(void) {
    // Read the data from stdin
    ModbusErrorInfo err = modbusSlaveInit(&minion, register_callback,
                                          exception_callback,
                                          static_allocator,
                                          modbusSlaveDefaultFunctions,
                                          modbusSlaveDefaultFunctionCount);

    // Check for errors
    assert(modbusIsOk(err) && "modbusSlaveInit() failed!");
}


void modbus_server_manage(mut_model_t *p_model) {
    uint8_t request[BSP_RS485_MAX_PACKET_LEN] = { 0 };
    size_t request_length = bsp_rs485_read(request, sizeof(request));

    if (request_length > 0 && bsp_rs485_timed_out(50)) {
        modbusSlaveSetUserPointer(&minion, p_model);
        ModbusErrorInfo err = modbusParseRequestRTU(&minion, 1, request, (uint8_t)request_length);

        if (modbusIsOk(err)) {
            const uint8_t *response = modbusSlaveGetResponse(&minion);
            size_t response_length = modbusSlaveGetResponseLength(&minion);
            bsp_rs485_write((uint8_t*)response, response_length);
            bsp_rs485_flush();
        } else {
            bsp_rs485_flush();
        }
    } else {
        // No data
    }
}


static ModbusError static_allocator(
    ModbusBuffer *buffer,
    uint16_t size,
    void *context)
{
    (void)context;
    #define MAX_RESPONSE 256

    static uint8_t response[MAX_RESPONSE];

    if (size != 0) // Allocation reqest
    {
        if (size <= MAX_RESPONSE) // Allocation request is within bounds
        {
            buffer->data = response;
            return MODBUS_OK;
        }
        else // Allocation error
        {
            buffer->data = NULL;
            return MODBUS_ERROR_ALLOC;
        }
    }
    else // Free request
    {
        buffer->data = NULL;
        return MODBUS_OK;
    }
}


static ModbusError register_callback(const ModbusSlave *minion,
                             const ModbusRegisterCallbackArgs *args,
                             ModbusRegisterCallbackResult *result) {
    mut_model_t *p_model = modbusSlaveGetUserPointer(minion);

    switch (args->query) {
        // Pretend to allow all access
        case MODBUS_REGQ_R_CHECK:
            result->exceptionCode = MODBUS_EXCEP_NONE;
            break;

        case MODBUS_REGQ_W_CHECK:
            switch (args->index) {
                case MODBUS_HR_REQUIRED_DUTY_CYCLE:
                    result->exceptionCode = MODBUS_EXCEP_NONE;
                    break;
                default:
                    result->exceptionCode = MODBUS_EXCEP_ILLEGAL_FUNCTION;
                    break;
            }
            break;

            // Return 7 when reading
        case MODBUS_REGQ_R: {
            switch (args->index) {
                case MODBUS_HR_DEVICE_MODEL:
                    result->value = APP_CONFIG_DEVICE_MODEL;
                    break;

                case MODBUS_HR_FIRMWARE_VERSION:
                    result->value = ((APP_CONFIG_FIRMWARE_VERSION_MAJOR & 0x1F) << 11) |
                            ((APP_CONFIG_FIRMWARE_VERSION_MINOR & 0x1F) << 6) |
                            (APP_CONFIG_FIRMWARE_VERSION_MAJOR & 0x3F);
                    break;

                case MODBUS_HR_INSTANT_ANALOG_VALUE_R1:
                    result->value = p_model->adc_r1;
                    break;

                case MODBUS_HR_INSTANT_ANALOG_VALUE_S:
                    result->value = p_model->adc_s;
                    break;

                case MODBUS_HR_INSTANT_ANALOG_VALUE_T:
                    result->value = p_model->adc_t;
                    break;

                case MODBUS_HR_REQUIRED_DUTY_CYCLE:
                    result->value = ((p_model->override_duty_cycle > 0) << 15) | (p_model->overridden_duty_cycle & 0x7FFF);
                    break;

                default:
                    result->value = 0;
                    break;
            }
            break;
        }

        case MODBUS_REGQ_W: {
            switch (args->index) {
                case MODBUS_HR_REQUIRED_DUTY_CYCLE:
                    p_model->override_duty_cycle = (args->value & (1 << 15)) > 0;
                    p_model->overridden_duty_cycle = args->value & 0x7FFF;
                    break;

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }

    return MODBUS_OK;
}


static ModbusError exception_callback(const ModbusSlave *minion, uint8_t function,
                              ModbusExceptionCode code) {
    (void)minion;
    (void)function;
    (void)code;
    return MODBUS_OK;
}
