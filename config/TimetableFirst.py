#!/usr/bin/python

import sys
import time
import pickle

timetable = [0,8,30,0,9,15,30,0,10,30,0,11,30,45,0,12,10,20,30,40,50,0,13,10,20,30,40,50,0,14,0,15,30,0,16,30,45,0,17,30,0,18,30,0,19,0,20,0,22,0,25]		#default timetable



i = len(timetable)
for x in range (0,i):

	print "Register[",x,"]:",timetable[x]


pickle.dump(timetable, open("/home/pi/make/timetableUpdate.p","wb"))
