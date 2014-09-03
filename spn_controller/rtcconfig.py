#!usr/bin/python

#-------------------------------------------------------
# RTC-Config Part
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import sys						#import Libaries
import time
import smbus

#-------------------------------------------------------
# Declaration of Variables
#-------------------------------------------------------

bus = smbus.SMBus(1)					#define Variables
rtc_address = 0x6f

year = 0						
month = 0
date = 0
day = 0
hour = 0
minute = 0
second = 0
counter = 0
registersave = 0

#-------------------------------------------------------
# Functions
#-------------------------------------------------------

def BCDtDEC(bcd):

	return (bcd&0x0f)+((bcd&0xf0)>>4)*10

def DECtBCD(dec):

	return (dec/10)*6 + dec

#-------------------------------------------------------
#-------------------------------------------------------
# Main Program
#-------------------------------------------------------
#-------------------------------------------------------

bus.write_byte_data(rtc_address, 0x00, 0)     		#turn off oscillator

#-------------------------------------------------------
# Input Years
#-------------------------------------------------------

counter = 0
while counter < 1:

	year =  raw_input("Enter the year(0-99 valid): ");	#input string year
	year = int(year)					#convert string to integer
	
	if (year <= 99) & (year >= 0):				#checks if the input is valid
	
		bus.write_byte_data(rtc_address, 0x06, DECtBCD(year))    #write the input to year register in BCD
		counter = counter + 1
		print "Year: %d" %BCDtDEC(bus.read_byte_data(rtc_address, 0x06))
		
	else:
		
		print "\nyour input is not valid - try again\n"	# Error Message when input is not valid

#-------------------------------------------------------
# Input Month
#-------------------------------------------------------

counter = 0

while counter < 1:

	month = raw_input("Enter the month(1-12 valid): ");	#input string month
	month = int(month)					#convert string to integer

	if (month <= 12) & (month > 0):				#checks if input is valid
		
		bus.write_byte_data(rtc_address, 0x05, DECtBCD(month))	#write the input to the month register
		counter = counter + 1
		print "Month: %d" %BCDtDEC(bus.read_byte_data(rtc_address, 0x05))
 	
	else:
		
		print "\nyour input is not valid - try again\n"	#Error Message 

#-------------------------------------------------------
# Input Date
#-------------------------------------------------------

counter = 0

while counter < 1:

	date = raw_input("Enter the date(1-31 valid): ");
	date = int(date)

	if (date <= 31) & (date > 0):
		
		bus.write_byte_data(rtc_address, 0x04, DECtBCD(date))
		counter = counter + 1
		print "Date: %d" %BCDtDEC(bus.read_byte_data(rtc_address, 0x04))

	else:
		
		print "\nyour input is not valid - please try again\n"

#-------------------------------------------------------
# Input Day
#-------------------------------------------------------

counter = 0

while counter < 1:

	day = raw_input("Enter the day(1-7 valid): ");
	day = int(day)

	if (day <= 7) & (day > 0):
		
		registersave = bus.read_byte_data(rtc_address, 0x03)
		registersave = registersave & 0b00111000
		
		day = day + 0x08
		day = day | registersave

		bus.write_byte_data(rtc_address, 0x03, day)
		counter = counter + 1
		day = BCDtDEC(bus.read_byte_data(rtc_address, 0x03))
		
		day = day & 0b00000111
		print "Day: %d" %day
	
	else:

		print "\nyour input is not valid - please try again\n"

#-------------------------------------------------------
# Input Hour
#-------------------------------------------------------

counter = 0

while counter < 1:

	hour = raw_input("Enter the hour(1-23 valid): ");
	hour = int(hour)

	if (hour <= 23) & (hour >= 0):
	
		bus.write_byte_data(rtc_address, 0x02, DECtBCD(hour))
		counter = counter + 1
		print "Hour: %d" %BCDtDEC(bus.read_byte_data(rtc_address, 0x02))
	
	else:

		print "\nyour input is not valid - please try again\n"

#-------------------------------------------------------
# Input Minute
#-------------------------------------------------------

counter = 0

while counter < 1:

	minute = raw_input("Enter the minute(0-59 valid): ");
	minute = int(minute)

	if (minute <= 59) & (minute >= 0):

		bus.write_byte_data(rtc_address, 0x01, DECtBCD(minute))
		counter = counter + 1
		print "Minute: %d" %BCDtDEC(bus.read_byte_data(rtc_address, 0x01))

	else:

		print "\nyour input is not valid - please try again\n"

#-------------------------------------------------------
# Input Second
#-------------------------------------------------------

counter = 0

while counter < 1:

        second = raw_input("Enter the seconds(0-59 valid): ");
        second = int(second)

        if (second <= 59) & (second >= 0):

                bus.write_byte_data(rtc_address, 0x00, DECtBCD(second))
                counter = counter + 1
                print "Second: %d" %BCDtDEC(bus.read_byte_data(rtc_address, 0x00))

        else:

                print "\nyour input is not valid - please try again\n"


secondread = bus.read_byte_data(rtc_address, 0x00)	 #copies the second data to the var: secondread
secondread = secondread + 0x80				 #sets the bit for running the osci without altering the information about the seconds
bus.write_byte_data(rtc_address, 0x00, secondread)
