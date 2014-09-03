#!/usr/bin/python

import sys
import time
import pickle
import smbus

#-------------------------------------------------------
# Declaration of Variables
#-------------------------------------------------------

bus = smbus.SMBus(1)
rtc_address = 0x6f

bit0 = 0b00000001
bit1 = 0b00000010
bit2 = 0b00000100
bit3 = 0b00001000
bit4 = 0b00010000
bit5 = 0b00100000
bit6 = 0b01000000
bit7 = 0b10000000

alarm_input = 0

timetable = pickle.load( open( "/home/pi/make/timetableUpdate.p", "rb" ))	#getting update

pickle.dump(timetable, open("/home/pi/make/timetable.p","wb"))
timetable = pickle.load( open( "/home/pi/make/timetable.p", "rb" ))

i = len(timetable)-2

hour = 0
minute = 0

i = len(timetable)-2

#--------------------------------------------------------
# Functions
#-------------------------------------------------------

def BCDtDEC(bcd):

	return (bcd&0x0f)+((bcd&0xf0)>>4)*10

def DECtBCD(dec):

	return (dec/10)*6 + dec

#-----------------------------------------------------------
# Main Program
#-----------------------------------------------------------

hour = BCDtDEC(bus.read_byte_data(rtc_address, 0x02))		#catches the current hour
minute = BCDtDEC(bus.read_byte_data(rtc_address, 0x01))		#catches the current minute

if minute < 55:				#Making sure that the system don't skip an alarm - that the alarm is not one hour delayed or even a whole day

	minute = minute + 4
else:
	hour = hour + 1
	minute = 1

bus.write_byte_data(rtc_address, 0x07, bit4) 

x = 0

while x < i:

	if (timetable[x] >= hour) & (timetable[x-1] == 0) & (timetable[x+1] == 0) & (timetable[x+2] != 25):
		#0,hour,0,nextalarmhour
		print "here"
		bus.write_byte_data(rtc_address, 0x0D , bit7 | bit5)
		bus.write_byte_data(rtc_address, 0x0C , DECtBCD(int(timetable[x+2])))
		print timetable[x+2]
		x = i - 1	

	if (timetable[x] <=  hour) & (timetable[x+2] == 25) & (timetable[x+1] == 0):
		#passedhour,0,25! -- 0,morninghour alarm
		print "here7"
		bus.write_byte_data(rtc_address, 0x0D , bit7 | bit5)
		bus.write_byte_data(rtc_address, 0x0C , DECtBCD(int(timetable[1])))
		x = i - 1

	if (timetable[x] >= hour) & (timetable[x-1] == 0) & (timetable[x+1] != 0):
		
		y = x
	
		if timetable[x] > hour:
		
			#hour or minute,0,nextalarmhour (normally shouldn't enter this path - but you need it for not overjumping an hour!
	                bus.write_byte_data(rtc_address, 0x0D , bit7 | bit5)
			bus.write_byte_data(rtc_address, 0x0C , DECtBCD(int(timetable[x])))
			y = i
                
		x = i - 1
		
                ifcheck = 0

		while y < i: 
			
			if (timetable[y] < minute) & (timetable[y+1] > minute) & (timetable[y] != 0) & (ifcheck == 0):
				#passedminute,alarmminute
				bus.write_byte_data(rtc_address, 0x0D , bit7 | bit4)
				bus.write_byte_data(rtc_address, 0x0B , DECtBCD(int(timetable[y+1])))
				y = i - 1
				ifcheck = 1 

			if (timetable[y] < minute) & (timetable[y+1] == 0) & (timetable[y+2] != 25) & (ifcheck == 0):
				#passedminute,0,alarmhour
				bus.write_byte_data(rtc_address, 0x0D , bit7 | bit5)
				bus.write_byte_data(rtc_address, 0x0C , DECtBCD(int(timetable[y+2])))
				y = i - 1
				ifcheck = 1
				
			if (timetable[y] > minute) & (timetable[y-2] == 0) & (ifcheck == 0):
				#0,hour,alarmminute
				bus.write_byte_data(rtc_address, 0x0D , bit7 | bit4)
				bus.write_byte_data(rtc_address, 0x0B , DECtBCD(int(timetable[y])))
				y = i - 1
				ifcheck = 1
				
			if (timetable[y] < minute) & (timetable[y+1] == 0) & (timetable[y+2] == 25) & (ifcheck == 0):
				#passedminute,0,25! -- 0,morninghour alarm
				bus.write_byte_data(rtc_address, 0x0D , bit7 | bit5)
				bus.write_byte_data(rtc_address, 0x0C , DECtBCD(int(timetable[1])))
				y = i - 1
				ifcheck = 1
				
			y = y + 1
		
	x = x + 1	
				
