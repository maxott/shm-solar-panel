#!/usr/bin/python

#-------------------------------------------------------
# Reset Alarm - Part
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
rtc_address = 0x6f
	
class RtcResetAlarm:			#Class for reset alarm

	def ResetAlarm(self):		#Function for reset alarm
	
		bus.write_byte_data(rtc_address, 0x0D, 0b11110111 & bus.read_byte_data(rtc_address, 0x0D))	#Resets the Alarm-0-Register-bit which is set by hardware after an alarm to 0
		bus.write_byte_data(rtc_address, 0x14, 0b11110111 & bus.read_byte_data(rtc_address, 0x14))	#Resets the Alarm-1-Register-bit which is set by hardware after an alarm to 0


RtcResetAlarm = RtcResetAlarm()
RtcResetAlarm.ResetAlarm()
