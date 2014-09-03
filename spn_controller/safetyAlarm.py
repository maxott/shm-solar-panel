#!/usr/bin/python

#-------------------------------------------------------
# Alarm - Part
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import sys
import time
import smbus

#-------------------------------------------------------
# Declaration of Variables
#-------------------------------------------------------

bus = smbus.SMBus(1)
rtc_address = 0x6f

bit0 = 0b00000001
bit1 = 0b00000010
bit2 = 0b00000100
bit3 = 0b00001000
bit4 = 0b00010000
bit5 = 0b00100000
bit6 = 0b01000000
bit7 = 0b10000000

alarm_input = 0

#-------------------------------------------------------
# Class(es)
#-------------------------------------------------------

class RTC:

	def safteyAlarm(self):
		
	bus.write_byte_data(rtc_address, 0x07, bit5)		#Sets the Alarm 1 to active
	bus.write_byte_data(rtc_address, 0x14 , bit7 | bit5)	#Sets the Hour-Alarm-mode active
	bus.write_byte_data(rtc_address, 0x13, DECtBCD(12))	#Programs a 12 o'clock alarm
		
#-------------------------------------------------------
# Functions
#-------------------------------------------------------

#Think of default pattern!! => start(), stop(). find out if run as "python foo.py" or "python main.py" with "import foo.py"...
def BCDtDEC(bcd):	#Function - calculates from BCD to Decimal

	return (bcd&0x0f)+((bcd&0xf0)>>4)*10

def DECtBCD(dec):	#Function - calculates from Decimal to BCD

	return (dec/10)*6 + dec

RTC = RTC()
RTC.safteyAlarm()


