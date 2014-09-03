following includes a daemon which should automatically start at system-boot.
just paste file spn_controller to the directory /etc/init.d/ and run following commands to add them to the startup-list.

$ sudo update-rc.d spn_controller defaults

Note: /home/pi/spn_controller/Software_SPN.py must be executable!