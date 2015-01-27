#!/usr/bin/python

import smbus
bus = smbus.SMBus(1)
FG_Address = 0x55


def RemainingCapacity():

	return bus.read_word_data(FG_Address, 0x04)	#Reads 2 bytes from remaining-capacity-register of fuelgauge

def StateOfCharge():
	
	return bus.read_word_data(FG_Address, 0x02)	#Reads 2 bytes from remaining-capacity-register of fuelgauge	

