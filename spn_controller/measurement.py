#!/usr/bin/python

import sys
import time
import smbus
import socket

from oml4py import OMLBase
from datetime import datetime

bus = smbus.SMBus(1)
ADC_Address = 0x4D
FG_Address = 0x55

#Return-Class conaining current and voltage measured by the fuelgauge
class FuelGaugeReturn:
	current = float(0)
	voltage = float(0)
	
#Class containing calculated power for measurements
class Power:
	nodepower = float(0)
	chargingpower = float(0)
	fgr = FuelGaugeReturn
	adc_res = float(0)

#Function measuring charging/discharging power and nodepower
def GetPower():
	fgr = FuelGaugeReturn
	power = Power
	adc_res = ADC_ReadCurrent() 
	fgr = ReadFuelGauge()
	power.chargingpower = fgr.current * fgr.voltage
	power.nodepower = adc_res * fgr.voltage
	power.fgr = fgr
	power.adc_res = adc_res
	#print "Current: %f" %fgr.current
	#print "Voltage: %f" %fgr.voltage
	return power

#Voltage FuelGauge
def ReadFuelGauge():
	fgr = FuelGaugeReturn
	volt = ((bus.read_word_data(FG_Address, 0x08))/1000.0)
	#print "Volt %f" %volt
	current = bus.read_word_data(FG_Address, 0x00)
	if current >= 0x800:
		current =  current - 0x10000
	#print "Current %d" %current
	current = float(current /1000.0)
	#print "current %f" %current
	fgr.voltage = volt;
	fgr.current = current;
	return fgr
	
#Nodecurrent - ADC	
def ADC_ReadCurrent():
	avrg = SampleAverage(5)
	amps = Result2Amps(avrg)
	return amps
#samples 50 times and calculates average
def ADC_Read():                                   #Function reads Sample Result from ADC
	avrg = SampleAverage(50)
	#print avrg
	volts = Result2Volts(avrg)
	amps = Result2Amps(avrg)
	return "Voltage (Volts) = %f Current (Amps): %f" % (volts, amps)
#samples ammount times from adc and returns average
def SampleAverage(ammount):
	sum = long(0)
	i = ammount
	for i in range(0,ammount):
		res = bus.read_word_data(ADC_Address,0)
		res_lsb = (res>>8)
		res_msb = (res<<8)&0xFF00
		res = res_msb | res_lsb
		res = (res >> 2)&0x3FF
		sum= sum + res
		#print sum
		i=i-1
	#print float(ammount)
	sum = sum  /float( ammount)
	#print sum
	return sum
#converts adc result to volts
def Result2Volts(result):
	return result *5.0 /0x400
#converts adcresult to amps
def Result2Amps(result):
	return result * 5.0 /(0x400 * 0.007 * 51.92)	#See formula in paper

"""	
power = Power
while 1:
	power = GetPower()
	print "Nodes: %f Battery: %f" %(power.nodepower, power.chargingpower)
	time.sleep(1)
	
"""
experiment = "SHM_SPN_%d_%02d_%02d_%02d_%02d" %(datetime.now().year, datetime.now().month, datetime.now().day, datetime.now().hour, datetime.now().minute)
print experiment
#oml=OMLBase("SHM-SPN", "Solar_Power_Measurement","RaspberryPi_X","tcp:minuet3.dynhost.nicta.com.au:3003")
#open oml server connection
oml=OMLBase("SHM_SPN", experiment,socket.gethostname(),"tcp:norbit.npc.nicta.com.au:3003")
#add measurement points
oml.addmp("Node_Power_Consumtion", "time:double power:double")
oml.addmp("Battery_Charging_Power", "time:double power:double")
oml.addmp("Node_Current", "time:double power:double")
oml.start()

power = Power
time_inc = float(1.0)		#Time between samples
time_cnt = float(0.0)		#Loop-Counter
time_max = float(60)		#Time to measure

while time_cnt < time_max:
	power = GetPower()
	print "Nodes: %f Battery: %f" %(power.nodepower, power.chargingpower)
	#injects data to measurement points
	oml.inject("Node_Power_Consumtion", (time_cnt, power.nodepower))
	oml.inject("Battery_Charging_Power", (time_cnt, power.chargingpower))
	oml.inject("Node_Current", (time_cnt, power.adc_res))
	time.sleep(time_inc)
	time_cnt = time_cnt + time_inc
#close oml server connection
oml.close()