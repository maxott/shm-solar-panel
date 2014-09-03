#!/usr/bin/env python
#Wifi up
from time import sleep

#google python coding style

command_dwn = "/usr/bin/sudo /sbin/ifdown wlan0"	#sudo and wlan0 => constant in header or config.py | setup.py!!!
command_up =  "/usr/bin/sudo /sbin/ifup wlan0"
import subprocess
process = subprocess.Popen(command_dwn.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
#print output
process = subprocess.Popen(command_up.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
#print output
process = subprocess.Popen(command_dwn.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
#print output
sleep(1)
process = subprocess.Popen(command_up.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
#print output

