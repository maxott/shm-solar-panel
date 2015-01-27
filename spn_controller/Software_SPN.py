#!/usr/bin/env python

#-------------------------------------------------------
#-------------------------------------------------------
# Software for the Solar Powered Node
#-------------------------------------------------------
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import logging
import sys
from time import sleep
import rtcResetAlarm
import safetyAlarm
import mpptlow
import LM75
import nodeOn
import nodeOff
import rtcCatchTime
import rtcAlarmTimetable
import shutdown
import FuelGauge_RemainingCapacity

#-------------------------------------------------------
# Used Constants
#-------------------------------------------------------

if_fuelgauge_constant = 0
if_constant_bat = 1
if_constant_temp = 80
sleep_constant = 10
batteryCheck = 0
temperatureCheck = 0

#-------------------------------------------------------
# Main Program
#-------------------------------------------------------

logging.basicConfig(filename='spn_controller.log',format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p', level=logging.DEBUG)

try:
	sleep(sleep_constant)

	#Reset the alarm registers that a new alarm has effect
	rtcResetAlarm.ResetAlarm()
	logging.info('Alarm resetted')
  				
	#Sets an safety alarm to 12 am
	safetyAlarm.safety()
	logging.info('safety alarm is set')
	
	#batteryCheck = FuelGauge_RemainingCapacity.RemainingCapacity()				#Fuel Gauge is looking at the battery capacity status (in %)
	if batteryCheck == if_fuelgauge_constant:
	#if batteryCheck >= 20:									#When there are more then 20% left - we are turning on the nodes
		logging.info('battery okay - fuel gauge')
		
		batteryCheck = mpptlow.mpptlowbat()						#Checks Battery over Mppt (if fuel gauge fails)
	     	
		if batteryCheck == if_constant_bat:						#if we are allowed to turn on nodes
		
			logging.info('battery okay - mppt')
		
			temperatureCheck = LM75.readtemp()					#Get the temperature
		
			if float(temperatureCheck) <= if_constant_temp:				#Checks if the surrounding temperature is under 80 degrees
			
				logging.info('Temperature in case is okay')
			
				nodeOn.nodeOn()							#Runs the file nodeOn.py - Switch on Nodes
				logging.info('Turned on nodes')

				#-----------------------------------------------------------------------------------------------------------------------------------
				# 		Node Communication
				#print "I2C successfully written, System now sleeps!"
			
				#logging instead of printing!! warnings and errors - throw rest away
			
			
				#constants instead of "magic numbers"
				sleep(sleep_constant)	#Here should be placed execfile("/home/pi/spn_controller/howEverYouCallit.py") - python script or program for node communication => wait for shutdown-request
			
				#-----------------------------------------------------------------------------------------------------------------------------------
			
				#don't hard-code the path!!
				nodeOff.nodeOff()			#Runs the filde nodeOff.py -Switch off nodes	
				logging.info('Turn off nodes')

				#Here should the cell modem send the collected data to the server

				rtcCatchTime.catchtime()
				logging.info('rtc got new correct time')
			
				rtcAlarmTimetable.newAlarm()	#Programs the new alarm as it stand in the updated timetable
				logging.info('new alarm is programmed')
			else:
				logging.warning('Temperature in the case is over 80C')	#Returns that the Temp. is too high for running the system - maybe something is destroyed
		else:
		
			logging.warning('Mppt says that battery is too low!')			#Says that the MPPT has disconnected the battery from the load
	#else:
		#logging.warning('Fuel Gauge says that battery is too low!')
				
	logging.info('shutdown raspberry now')	
	shutdown.shutdown()				#After setting the Alarm is shutdowns itself and wait for a new interrupt of the RTC

except IOError, err:

	print err
	logging.error('I2C-bus failure!!-prove connection to PCB')

