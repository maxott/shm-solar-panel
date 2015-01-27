#!/usr/bin/python

import sys
import time
import pickle

timetable = [0,7,10,20,30,40,50,0,9,10,20,30,40,50,0,10,10,20,30,40,50,0,11,10,20,30,40,50,0,12,10,20,30,40,50,0,13,10,20,30,40,50,0,14,10,20,30,40,50,0,15,10,20,30,40,50,0,16,10,20,30,40,50,0,17,10,20,30,40,50,0,18,0,20,0,25]		#default timetable



i = len(timetable)
for x in range (0,i):

	print "Register[",x,"]:",timetable[x]


pickle.dump(timetable, open("/home/pi/spn_controller/timetableUpdate.p","wb"))
