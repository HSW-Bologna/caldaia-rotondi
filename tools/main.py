#!/usr/bin/env python3
import minimalmodbus

instrument = minimalmodbus.Instrument("COM3", 1)  # port name, slave address (in decimal)
instrument.serial.baudrate = 115200

## Read temperature (PV = ProcessValue) ##
temperature = instrument.read_register(1, functioncode=4)  # Registernumber, number of decimals
print(temperature)
