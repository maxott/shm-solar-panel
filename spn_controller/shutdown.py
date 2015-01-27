#!/usr/bin/python

import sys
import smbus
import time
import os

bus = smbus.SMBus(1)
expander_address = 0x20		#define address of port expander
expander = 0x20

def shutdown():						#Function for shutdown
	
	
	bus.write_byte_data(expander_address, 0x00, 0)                  #Expander confi$

	bus.write_byte_data(expander_address, 0x01, 255)


	bus.write_byte_data(expander_address,0x09,1)	#set shutdown-pin at expander

		
	os.system("sudo shutdown -h now")
