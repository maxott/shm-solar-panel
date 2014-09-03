5) Run Config Programs:

	At first you have to connect the raspberry to the SPN_PCB -> Take care that you put the connection cable in the right way to the PCB!
	
	Run rtcConfigFirst.py with: (You are configuring the right time to the RTC on the PCB and setting certain bits, which needs to be set for running the spn_controller
	$ sudo python /home/pi/config/rtcConfigFirst

	Run TimetableFirst.py for initialise a default timetable:
	$ sudo python /home/pi/config/TimetableFirst.py

	Now test if the RTC and the oscillator is running: (Execute this file several times and look if the time is changing - when the time is not changing the oscillator on the PCB is probably damaged)
	$ sudo python /home/pi/config/rtctest.py