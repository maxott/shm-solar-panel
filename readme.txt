//-------------------------------------------
// User Instruction for using spn_controller
//-------------------------------------------

(after $ comes a command you should type in to your command shell on the raspberry pi)

1) Configure your raspberry for I2C using

	At first you open raspi-blacklist.conf with nano:
		$ sudo nano /etc/modprobe.d/raspi.blacklist.conf
	
		Then you enable the I2C by adding a # in front of the line: blacklist i2c-bcm2708
		blacklist i2c-bcm2708 -> #blacklist i2c-bcm2708
	
	Now you save it and enable kernel I2C module, by opening /etc/modules with nano
		$ sudo nano /etc/modules

		Then you have to add after the "snd-bcm2835" line a new one with "i2c-dev".

		snd-bcm2835 ->  snd-bcm2835
				i2c-dev

	There are some packages and libraries, which have to be installed to use I2C in python
		$ sudo apt-get install i2c-tools
		$ sudo apt-get install python-smbus
		$ sudo adduser pi i2c	

2) Activate your Watchdog-Deamon

	The watchdog is on the raspberry not used by default, so you have to activate it. 
		$ sudo modprobe bcm2708_wdog
		$ echo "bcm2708_wdog" | sudo tee -a /etc/modules

	With this two command lines your watchdog is ready for action. Then you have to install the watchdog-deamonv with following command.
		$ sudo apt-get install watchdog

	Then you open /etc/watchdog.conf and delete two hash tags in the following lines:
 		$ sudo nano /etc/watchdog.conf

		#watchdog-device	= /dev/watchdog		-> 	watchdog-device	= /dev/watchdog		
		#max-load-1		= 24			->	max-load-1		= 24

	After altering the file you only have to start the daemon with the next command and your wachdog will be rewritten every 10 seconds, if your software is running correctly. 
		$ sudo /etc/init.d/watchdog start
	You only have to do this once, because when you start your raspberry pi the next time, it will automatically start the watchdog. 
	
3) Reboot

	Here I recommend to reboot your raspberry pi with:
		$ sudo reboot		

4) Put the programs on your raspberry

	Copy the folders "spn_controller" and "config" and paste them to /home/pi/ 

5) Run Config Programs:

	At first you have to connect the raspberry to the SPN_PCB -> Take care that you put the connection cable in the right way to the PCB!
	
	Run rtcConfigFirst.py with: (You are configuring the right time to the RTC on the PCB and setting certain bits, which needs to be set for running the spn_controller
	$ sudo python /home/pi/config/rtcConfigFirst

	Run TimetableFirst.py for initialise a default timetable:
	$ sudo python /home/pi/config/TimetableFirst.py

	Now test if the RTC and the oscillator is running: (Execute this file several times and look if the time is changing - when the time is not changing the oscillator on the PCB is probably damaged)
	$ sudo python /home/pi/config/rtctest.py
	

6) Run spn_controller as a deamon when the raspberry starts:
	
	following includes a daemon which should automatically start at system-boot.
	just paste file spn_controller (path on git: /etc/inid.d/spn_controller) to the directory /etc/init.d/ on the raspberry and run following commands to add them to the startup-list.

	Note: /home/pi/spn_controller/Software_SPN.py must be executable! 

	$ sudo chmod +x /home/pi/spn_controller/Software_SPN.py 

	$ sudo update-rc.d spn_controller defaults

	

