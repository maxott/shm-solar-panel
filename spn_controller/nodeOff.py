#!/usr/bin/python

#-------------------------------------------------------
# Node Off - Part
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import sys
import smbus
import time

#-------------------------------------------------------
# Declaration of Variables
#-------------------------------------------------------

bus = smbus.SMBus(1)
expander_address = 0x20

bit0 = 0b00000001
bit1 = 0b00000010
bit2 = 0b00000100
bit3 = 0b00001000
bit4 = 0b00010000
bit5 = 0b00100000
bit6 = 0b01000000
bit7 = 0b10000000

#-------------------------------------------------------
# Functions
#-------------------------------------------------------

def nodeOff():			#Function for cutting off node power

	bus.write_byte_data(expander_address, 0x00, 0)          #Expander configuration lines
	bus.write_byte_data(expander_address, 0x01, bit1)	#Sets GPIO1 to output


	bus.write_byte_data(expander_address,0x09,bit1)		#Sets GPIO1 to true
	time.sleep(0.1)
	bus.write_byte_data(expander_address,0x09,0)		#Have to be cleared otherwise it's setting all the time
	return -1


