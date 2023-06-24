#!/usr/bin/python3

#import time
import board, sys
from datetime import datetime
import adafruit_tca9548a
from adafruit_ina219 import ADCResolution, BusVoltageRange, INA219


i2c = board.I2C()  # uses board.SCL and board.SDA
tca = adafruit_tca9548a.TCA9548A(i2c)

sensor1 = INA219(tca[0])
sensor2 = INA219(tca[1])

# optional : change configuration to use 32 samples averaging for both bus voltage and shunt voltage
sensor1.bus_adc_resolution = ADCResolution.ADCRES_12BIT_32S
sensor1.shunt_adc_resolution = ADCResolution.ADCRES_12BIT_32S
sensor2.bus_adc_resolution = ADCResolution.ADCRES_12BIT_32S
sensor2.shunt_adc_resolution = ADCResolution.ADCRES_12BIT_32S

# optional : change voltage range to 16V
sensor1.bus_voltage_range = BusVoltageRange.RANGE_16V
sensor2.bus_voltage_range = BusVoltageRange.RANGE_16V

# measure and display loop
#shunt_voltage = ina219.shunt_voltage  # voltage between V+ and V- across the shunt
bus_voltage1 = sensor1.bus_voltage  # voltage on V- (load side)
bus_voltage2 = sensor2.bus_voltage  # voltage on V- (load side)
shunt_voltage1 = sensor1.shunt_voltage  # voltage between V+ and V- across the shunt
shunt_voltage2 = sensor2.shunt_voltage  # voltage between V+ and V- across the shunt
current1 = sensor1.current  # current in mA
current2 = sensor2.current  # current in mA
power1 = sensor1.power  # power in watts
power2 = sensor2.power  # power in watts

ntime = datetime.now().strftime("%H:%M ")

# INA219 measure bus voltage on the load side. So PSU voltage = bus_voltage + shunt_voltage

if((len(sys.argv) == 2) and sys.argv[1] == "-l"):
	print("Voltage/Current at %s" % ntime)
	print("--DEW----------------------------------")
	print("Voltage (VIN+) : {:6.3f}   V".format(bus_voltage1 + shunt_voltage1))
	print("Shunt Current  : {:7.4f}  A".format(current1 / 1000))
	print("Power Calc.    : {:8.5f} W".format(bus_voltage1 * (current1 / 1000)))
	print("Power Register : {:6.3f}   W".format(power1))
	print("--PWR----------------------------------")
	print("Voltage (VIN+) : {:6.3f}   V".format(bus_voltage2 + shunt_voltage2))
	print("Shunt Current  : {:7.4f}  A".format(current2 / 1000))
	print("Power Calc.    : {:8.5f} W".format(bus_voltage2 * (current2 / 1000)))
	print("Power Register : {:6.3f}   W".format(power2))
	print("--Total--------------------------------")
	print("Voltage (VIN+) : {:6.3f}   V".format((bus_voltage1 + shunt_voltage1 +bus_voltage2 + shunt_voltage2) /2))
	print("Shunt Current  : {:7.4f}  A".format((current1 + current2) / 1000))
	print("Power Calc.    : {:8.5f} W".format(bus_voltage1 * (current1 / 1000) + bus_voltage2 * (current2 / 1000)))
	print("Power Register : {:6.3f}   W".format(power1 + power2))
	print("")

else:
	print ("%4.1f,%4.2f,%4.1f,%4.1f,%4.2f,%4.1f" % ( 
		(bus_voltage1 + shunt_voltage1),
		(current1 / 1000),
		(bus_voltage1 * (current1 / 1000)),
		(bus_voltage2 + shunt_voltage2),
		(current2 / 1000),
		(bus_voltage2 * (current2 / 1000))
		))

# Check internal calculations haven't overflowed (doesn't detect ADC overflows)
if sensor1.overflow:
        print("Internal Math Sensor1 Overflow Detected!")
        print("")

if sensor2.overflow:
        print("Internal Math Sensor2 Overflow Detected!")
        print("")

