#!/usr/bin/env python

#-------------------------------------------------------
#-------------------------------------------------------
# Software for the Solar Powered Node
#-------------------------------------------------------
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import sys
from time import sleep
#-------------------------------------------------------
# Main Program
#-------------------------------------------------------

try:
	sleep(10)

	execfile("/home/pi/spn_controller/WLAN_on.py")

	execfile("/home/pi/spn_controller/rtcResetAlarm.py")     		     	#Reset the alarm registers that a new alarm has effect

	execfile("/home/pi/spn_controller/safetyAlarm.py")  				#Sets an safety alarm to 12 am

	#execfile ("/home/pi/Python-Programs/batterycritical.py")                       #This script asks if the battery is critical - fuel gauge

	script_locals = dict()
	execfile("/home/pi/spn_controller/mpptlow.py", dict(), script_locals)     	#Executes the mpptlow.py program

	if script_locals["scriptReturn"] == 1:				#Mppt checks if the battery is okey

		script_locals = dict()					
		execfile("/home/pi/spn_controller/LM75.py", dict(), script_locals)	#Executes the LM75.py program
		
		if float(script_locals["scriptReturn"]) <= 80.0:	#Checks if the surrounding temperature is under 80 degrees
		
			execfile("/home/pi/spn_controller/nodeOn.py")			#Runs the file nodeOn.py - Switch on Nodes

			#-----------------------------------------------------------------------------------------------------------------------------------
			# 		Node Communication
			print "I2C successfully written, System now sleeps!"
			
			#logging instead of printing!! warnings and errors - throw rest away
			
			
			#constants instead of "magic numbers"
			sleep(90)	#Here should be placed execfile("/home/pi/spn_controller/howEverYouCallit.py") - python script or program for node communication => wait for shutdown-request
			
			#-----------------------------------------------------------------------------------------------------------------------------------
			
			#don't hard-code the path!!
			execfile("/home/pi/spn_controller/nodeOff.py")			#Runs the filde nodeOff.py -Switch off nodes	

			#Here should the cell modem send the collected data to the server

			execfile("/home/pi/spn_controller/rtcCatchTime.py")		#The RTC catches the time from the PI
			
			execfile("/home/pi/spn_controller/rtcAlarmTimetable.py")	#Programs the new alarm as it stand in the updated timetable
			
		else:
			print "Temperature is over 80 degrees!!"	#Returns that the Temp. is too high for running the system - maybe something is destroyed
	else:

		print "Mppt says that battery is low"			#Says that the MPPT has disconnected the battery from the load
		
	execfile("/home/pi/spn_controller/shutdown.py")				#After setting the Alarm is shutdowns itself and wait for a new interrupt of the RTC

except IOError, err:

	print err
	print "I2C-Bus-Failure!! Exception-Handling: maybe write email to sb i2c bus not working"

