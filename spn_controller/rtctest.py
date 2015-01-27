#!/usr/bin/python

import sys
import smbus
import time

bus = smbus.SMBus(1)
rtc = 0x6f

def osziact():
	
	print bus.read_byte_data(rtc, 0x00)

	bus.write_byte_data(rtc, 0x00, 0x80)

	print bus.read_byte_data(rtc, 0x00)


def BCDtDEC(bcd):

        return (bcd&0x0f)+((bcd&0xf0)>>4)*10

def DECtBCD(dec):

        return (dec/10)*6 + dec

def run_script():
    ret_value = 2
    return ret_value

script_ret = run_script()

#osziact()

#time.sleep(3)

print BCDtDEC(bus.read_byte_data(rtc, 0x02)), ":", BCDtDEC(bus.read_byte_data(rtc, 0x01)),":", BCDtDEC(bus.read_byte_data(rtc, 0x00)-128)
print BCDtDEC(bus.read_byte_data(rtc, 0x0C))

print BCDtDEC(bus.read_byte_data(rtc, 0x0B))

print BCDtDEC(bus.read_byte_data(rtc, 0x13))

print BCDtDEC(bus.read_byte_data(rtc, 0x12))

