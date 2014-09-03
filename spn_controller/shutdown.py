#!/usr/bin/python

import sys
import smbus
import time
import os

bus = smbus.SMBus(1)
expander_address = 0x20		#define address of port expander
expander = 0x20

ledcounter = 0


def writebyte(address,register,value):			#general write_i2c_byte function
	bus.write_byte_data(address,register,value)
	return -1

def setShutdownPiGpio():				#set the shutdown GPIO-0 on Expander - Cut-off from power can only be happen when this pin is set to 1 and the raspberry pi is halted
	bus.write_byte_data(expander_address,0x09,1)
	return -1

def shutdown():						#Function for shutdown
	
	os.system("sudo shutdown -h now")



bus.write_byte_data(expander_address, 0x00, 0)			#Expander configuration lines

bus.write_byte_data(expander_address, 0x01, 255)		

setShutdownPiGpio()	 				#GPIO-0 on Port-Expander is set to 1 - has to be resetted to 0 bei opening the program 


shutdown()						#Calling the Shutdown-function
