#!/usr/bin/python

def RemainingCapacity():
	import smbus
	bus = smbus.SMBus(1)
	FG_Address = 0x55
	return bus.readworddata(FG_Address, 0x04)	#Reads 2 bytes from remaining-capacity-register of fuelgauge
	
remaining_capacity = RemainingCapacity()