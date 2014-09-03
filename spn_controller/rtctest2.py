#!/usr/bin/env python

import smbus

bus = smbus.SMBus(1)

rtc_address = 0x6f

print "%x" %bus.read_byte_data(rtc_address, 0x07)
