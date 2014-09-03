#!/usr/bin/env python
command = "/usr/bin/sudo /sbin/ifdown wlan0"
import subprocess
process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
#print output
