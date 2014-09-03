#!/usr/bin/python

#-------------------------------------------------------
# Node On - Part
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
# Classes
#-------------------------------------------------------
#-------------------------------------------------------

#-----------------------------------------------------------------------
# Expander Class -
# Node on, Node off, Mppt Bat low, set shutdown pin, shutdown command
#-----------------------------------------------------------------------

class Expander:		
						#self.address = expander_address
	
	def nodeOn(self):			#Fumction for switch on nodes
	
		bus.write_byte_data(expander_address, 0x00, 0x00)	#Expander configuration lines
		bus.write_byte_data(expander_address, 0x01, bit2)	#Sets GPIO2 to output

		bus.write_byte_data(expander_address, 0x09, bit2)	#Sets bit GPIO2 to true
		time.sleep(0.1)
		bus.write_byte_data(expander_address, 0x09, 0x00)	#Have to be cleared otherwise it's setting all the time
		return -1

#-------------------------------------------------------
# Main Program
#-------------------------------------------------------

Expander = Expander()
Expander.nodeOn()			#Excutes the function nodeOn

