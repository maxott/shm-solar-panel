#!/usr/bin/python

#-------------------------------------------------------
# Node On - Part
#-------------------------------------------------------

#-------------------------------------------------------
# Function
#-------------------------------------------------------

def runScriptMppt():

	#-------------------------------------------------------
	# Declaration of needed Libraries
	#-------------------------------------------------------

	import sys
	import time
	import smbus

	#-------------------------------------------------------
	# Declaration of Variables
	#-------------------------------------------------------

	bus = smbus.SMBus(1)
	expander_address = 0x20              	#address of LM75

	temp = 0                 		#define variables

	bus.write_byte_data(expander_address,0x09,0x00)						
	def mpptlowbat():	              	#function returns 0 if battery is low and 1 when battery is okey

		batteryl = bus.read_byte_data(expander_address, 0x09)
		batteryl = batteryl & 0b00010000

		if batteryl == 0x10:                                    #When bit4 is true mpptlowbat returns 0 otherwiese 1
			batteryl = 0
		else:
			batteryl = 1
		return batteryl
	return mpptlowbat()
#-------------------------------------------------------
# Main Program
#-------------------------------------------------------

scriptReturn = runScriptMppt()
