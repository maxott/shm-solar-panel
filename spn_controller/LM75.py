#!/usr/bin/python

#-------------------------------------------------------
# Read - LM75 Temperature Sensor
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

#-------------------------------------------------------
# Functions
#-------------------------------------------------------

def runScriptTemp():	#Function for returning the temperature 

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
	lm75 = 0x4f                     #address of LM75

	temp = 0                        

	def readtemp():			#Function for reading and returning temp
	
		bus = smbus.SMBus(1)
		temp = bus.read_word_data(lm75,0x00)

		if temp >= 0x8000:		#read_word puts the lsb in front of the msb
			temp = temp - 0x8000	#in the lsb bit0-6 is zero and only the bit 7 
			temp = temp << 1	#belongs to the temperature number
			temp = temp + 1
		else:
			temp = temp << 1
		

		if temp < 0x100:		#checking if temp is under zero
			temp = temp / 2.0	#calculating temperature 
			return temp		#For further information please have a look in the datasheet
		else:
		
			temp = temp - 0x192
			temp = temp / -2.0
			return temp

	return readtemp()

	
#-------------------------------------------------------
# Main Program
#-------------------------------------------------------

scriptReturn = runScriptTemp()			#Returns to the Executer the current temperature

