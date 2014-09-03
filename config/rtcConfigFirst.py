#!usr/bin/python

#-------------------------------------------------------
# RTC-First-Config Part
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import sys                                              #import Libari
import time
import smbus
import datetime
#-------------------------------------------------------
# Declaration of Variables
#-------------------------------------------------------

bus = smbus.SMBus(1)                                    #define Variables
rtc_address = 0x6f

year = datetime.datetime.now().year % 100
month = datetime.datetime.now().month
date = datetime.datetime.now().day
weekday = datetime.date.isoweekday(datetime.date.today())
hour = datetime.datetime.now().hour
minute = datetime.datetime.now().minute
second = datetime.datetime.now().second

registersave = 0

class RTC:

        def catchtime(self):

                bus.write_byte_data(rtc_address, 0x00, 0)               #turn off oscillator

                #-------------------------------------------------------
                # Initials all data to the RTC
                #-------------------------------------------------------

                WriteRtc(year, 0x06)
                WriteRtc(month, 0x05)
                WriteRtc(date, 0x04)

                day = int(weekday)
                registersave = bus.read_byte_data(rtc_address, 0x03)                    #saves the part of the register we should not overwrite
                registersave = registersave & 0b00111000
                day = day + 0x08                                                        #sets the VBATEN bit that the RTC gets power out of the battery when rest is switched off
                day = day | registersave                                                #adds the saved register
                bus.write_byte_data(rtc_address, 0x03, day)

                WriteRtc(hour, 0x02)
                WriteRtc(minute, 0x01)
                WriteRtc(second, 0x00)

                secondread = bus.read_byte_data(rtc_address, 0x00)                      #copies the second data to the var: secondread
                secondread = secondread + 0x80                                          #sets the bit for running the osci without altering the information about the seconds
                bus.write_byte_data(rtc_address, 0x00, secondread)


#-------------------------------------------------------
# Functions
#-------------------------------------------------------



def BCDtDEC(bcd):

        return (bcd&0x0f)+((bcd&0xf0)>>4)*10

def DECtBCD(dec):

        return (dec/10)*6 + dec

def WriteRtc(Var, Register):

        Var = int(Var)                                                                  #makes sure that time variable is an integer
        bus.write_byte_data(rtc_address, Register, DECtBCD(Var))                        #writes the initialised time to registers

#-------------------------------------------------------
# Main Program
#-------------------------------------------------------
#Define Class_Var
RTC = RTC()
#Execute Data
RTC.catchtime()

bus.write_byte_data(rtc_address, 0x03, 0b00001000 | bus.read_byte_data(rtc_address, 0x03))



