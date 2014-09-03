#!/usr/bin/env python
from time import sleep
execfile("/home/pi/spn_controller/WLAN_off.py")
execfile("/home/pi/spn_controller/WLAN_on.py")
sleep(5*60)
