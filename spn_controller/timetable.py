#!/usr/bin/python

#-------------------------------------------------------
# Update the timetable
#-------------------------------------------------------

#-------------------------------------------------------
# Import all needed Libaries
#-------------------------------------------------------

import sys
import time
import pickle

#-------------------------------------------------------
# Declaration of Variables
#-------------------------------------------------------

timetablesave = [0,0]

counter = 0

savelowregisteraddress = 0
savehighregisteraddress = 0

hourLow = 0
hourHigh = 0

hour = 0
minute = 0

initial = 0

timetable = pickle.load( open( "/home/pi/make/timetableUpdate.p", "rb" ))			#timetable is initialed as an array from the timetabelUpdate 

#-------------------------------------------------------
# Main Program
#-------------------------------------------------------

while counter < 2:
	
	if initial > 0:
		pickle.dump(timetable, open("/home/pi/make/timetableUpdate.p","wb"))		#When you come the second time into the loop it saves the written data to file 
		timetable = pickle.load( open( "/home/pi/make/timetableUpdate.p", "rb" )) 

	i = len(timetable)	
	for x in range (0,i-2):				#Prints every register exept the 0 registers and the last register,
										#which we need for the logic (last register = 25)
		if (timetable[x] != 0) & (timetable[x-1] == 0):
			print "Register[",x,"]:",timetable[x],": 00"
			hour = timetable[x]
		if (timetable[x] != 0) & (timetable[x-1] != 0):
			print "Register[",x,"]:",hour,":",timetable[x]
	
	while counter < 1:
		a = raw_input("\nDo you want to change something in the timetable?[y,n]:\n\n");	#Asks the user if he wants to alter something in the timetable
		if (a == 'y') | (a == 'n'):       #Proves if your input is valid
			counter = counter + 1
		else:
                   	print "\nError: Invalid input\n"
	counter = 0

	if a == 'y':
		
		initial = initial + 1	#Sets the variable for updating the file after finishing the loop
	
		b = raw_input("\nWhen you want to change one register type in 'c' otherwise when you type 'a' you want to add a register?[c,a]:\n\n");	#Asks you if you want to change or add something to the timetable
		

		#-------------------------------------------------------
		# Change Part
		#-------------------------------------------------------


		if b == 'c':	#Checks if you typed in c
#31			
			i = i - 1
			while counter < 1:
				
				print "\nWhich Register do you want to change?[ 1 -",i,"]:\n"	#Asks you if you which register you want to change - shows which are valid"
				register = raw_input();	
				register = int(register)
				
				if (register > 0) & (register <= i):				#Proves if your input is valid - otherwise a Error message will shown and you can try again
					counter = counter + 1
				else:
					print "\nError: Invalid Input!\n"
			counter = 0

			#-------------------------------------------------------
			# Hour - Change Part
			#-------------------------------------------------------


			if int(timetable[register - 1]) == 0: 					#Proves if you want to change an hour register
				
				k = register - 2
				j = register 
				
				while int(timetable[k]) > 0:					#Catches the alarmhour before your chosen register
					
					hourLow = int(timetable[k])
					k = k - 1 
				
				while timetable[j] > 0:						#Catches the alarmhour after your chosen register
					
					hourHigh = int(timetable[j+2])
					j = j + 1
#57				
				while counter < 1:
					
					print "\nTell me the hour you want to set an alarm here[valid:",hourLow,"-",hourHigh,"]:\n"	#Asks you to which hour you want to change the register
					hour = raw_input();										#shows two numbers - in between is valid
					 
					if (int(hour) > hourLow) & (int(hour) < hourHigh):	#Checks if input is valid
						
						timetable[register] = hour
						counter = counter + 1
					else:
						print "\nError: Invalid input\n"		
				counter = 0

			#-------------------------------------------------------
			# Minute - Change Part
			#-------------------------------------------------------

			else:									#Else block for minute registers
				print timetable[register]	
				if (int(timetable[register]) > 0):	#Checks if the register value is not zero
					
					if int(timetable[register - 2]) == 0:	#Checks if your register comes directly after an hour register
#75						
						minuteHigh = int(timetable[register + 1])

						if minuteHigh == 0:		#Checks if the register after that is an zero register (zero register are always before an hour register)
							minuteHigh = 59
						
						hour = timetable[register - 1]
						
						while counter < 1:		#Asks the minute you want to set an alarm - shows valid input in brackets
							
							print "\nTell me the minute you want to set the alarm at the hour",hour,"[valid:1 -",minuteHigh,"]:\n"
							minute = raw_input();
						
							if (int(minute) > 0) & (int(minute) < int(minuteHigh)): #Proves if the input is valid
#89															
								timetable[register] = int(minute)
								counter = counter + 1
							else:
								print "\nError: Invalid input\n"
						counter = 0

					else:
						if int(timetable[register + 1]) == 0:	#Checks if the register you entered is the last before a new hour
							
							l = register							

							while counter < 1:			#Catches the hour where the minute register belongs to
								if int(timetable[l]) == 0:
									hour = timetable[l+1]
									counter = counter + 1
								else:
									l = l - 1
									
							counter = 0
							minuteLow = int(timetable[register - 1])
							minuteHigh = 59
#110							
							while counter < 1:	#Asks the minute you want to set an alarm - shows valid input in brackets

								print "\nTell me the minute you want to set the alarm at the hour",hour,"[valid:",minuteLow,"- 59]:\n"	
								minute = raw_input();

								if (int(minute) > int(minuteLow)) & (int(minute) < int(minuteHigh)):	#Proves if the input is valid

									timetable[register] = int(minute)
									counter = counter + 1
								else:
									print "\nError: Invalid input\n"
							counter = 0

						else:
							
							if (int(timetable[register - 1]) > 0) & (int(timetable[register + 1]) < 60):	#Checks if the chosen minute register is between two minute registers
								
								l = register	

								while counter < 1:			#Catchs the hour belonging to the minute registers
									if int(timetable[l]) == 0:
										hour = int(timetable[l+1])
										counter = counter + 1
									else:
										l = l - 1

								counter = 0
								minuteLow = int(timetable[register - 1])	#Get the minute registers before and after the chosen one
								minuteHigh = int(timetable[register + 1])

								while counter < 1:	#Asks the minute you want to set an alarm - shows valid input in brackets

									print "\nTell me the minute you want to set the alarm at the hour",hour,"[valid:",minuteLow,"-",minuteHigh,"]:\n"
									minute = raw_input();

									if (int(minute) > int(minuteLow)) & (int(minute) < int(minuteHigh)):	#Proves if the input is valid

										timetable[register] = int(minute)
										counter = counter + 1
									else:
										print "\nError: Invalid input\n"
								counter = 0

							else:	
								print "\nInvalid Input -  you shouldn't be here - please try again!\n"		
				else:	
					print "\nYou cannot change Registers with value 0, because they are needed for the array-syntax\n"		

		#-------------------------------------------------------
		# Add Part
		#-------------------------------------------------------

		else:
			if b == 'a':	#Proves if you want to add something
				

				while counter < 1:	#Asks you if you want to add a hour- or a minute-alarm belonging to a hour
				
					decision = raw_input("\nWhat would you like to add to the timetable - hour alarm our minute alarm?[h/m]:\n\n");
						
					if (decision == 'h') | (decision == 'm'):	#Proves if your input is valid
						counter = counter + 1
					else:
						print "\nError: Invalid input\n"

					counter = 0
		
				#-------------------------------------------------------
				# Hour - Add Part
				#-------------------------------------------------------

		
				if decision == 'h':	#Proves if you want to add a hour-alarm
#191					
					while counter < 1:	#Asks you which hour would you like to add
		
						hour = raw_input("\nWhat hour would you like to add?\n\n");
						
						hour = int(hour)

						i = len(timetable)	
						
						if (hour > 0) & (hour < 25):	#Checks if you typed in an existing hour
		
							for x in range(0,i):	#Checks if your chosen hour is already in the table
								if timetable[x] == hour:
									if timetable[x-1] == 0:
										print"\nError this hour is already a alarm! - please try again\n"
										x = i + 1
										counter = 1
						else:
							print "\nError: Your hour is not existing! - please try again\n"
							counter = 1

						if counter == 1:
							counter = 0
						else:
							counter = 1
						
					counter = 0	#Logic for rowing the hour with the right syntax to the array	
					
					for x in range (0,i):	#Save the Register with the next coming hour after the chosen hour
								
						if timetable[x] > hour:	
							if (timetable[x-1] == 0) & (counter == 0):
								savehighregisteraddress = x
								print x
								counter = counter + 1
								x = i
					counter = 0

					if savehighregisteraddress != 0:	#For every hour but not when you want to add the last hour of a day
						
						registeradd = savehighregisteraddress - 1
						i = i - registeradd
						
						for x in range (0,i):		#save the timetable after the hour you typed in

							h = x + registeradd
							timetablesave.append(0)
							timetablesave[x] = timetable[h]

        	                 
						timetable.append(0)
						for x in range (0,i):		#add a new Register in the end - everything goes one back

							h = x + registeradd
							h = h +1
							timetable[h] = timetablesave[x]
						 
						timetable.append(0)
						for x in range (0,i):		#add a new register at the end  - everything goes one back

							h = x + registeradd
							h = h + 2
							timetable[h] = timetablesave[x]

						i = len(timetable)		#writes the typed in hour in the right position of the array
						for x in range (0,i):
			
							if timetable[x] == 0:
								if timetable[x+1] == 0:
									timetable[x+1] = hour
									x = i + 1
				
				#-------------------------------------------------------
				# Minute - Add Part
				#-------------------------------------------------------

				if decision == 'm':	#Checks if you want to add a minute
						
					while counter < 1:	#Asks you to which hour you want to add a minute

						hour = raw_input("\nTo which hour you want to add an addational minute alarm? [only existing hours valid]\n\n");
						hour = int(hour)
						
						i = len(timetable)
						
						for x in range (0,i):	#Checks if the hour is existing
							
							if (timetable[x] == hour) & (timetable[x-1] == 0):
								counter = counter + 1
								
						if counter == 0:
							print "\nError:Input invalid - Your hour is not existing in the table\n"	
        	                                      
					counter = 0

					while counter < 1: 		#Asks which minute you want to add to the hour you typed in

						minute = raw_input("\nTo which minute in this hour you want an alarm? [only non-existing minutes in this hour are valid]\n\n");
						minute = int(minute)

						i = len(timetable)

						if (minute > 0) & (minute < 60):	#Checks if the minute is existing
							for x in range (0,i):
								if (timetable[x] == hour) & (timetable[x-1] == 0):
									counter1 = x
									counter2 = 0
									while counter2 < 1:	#Controls if the minute is already existing at that hour and save the address to place the minute there
										counter1 = counter1 + 1
										#if (timetable[counter1] > minute) | (timetable[counter1] == 0) & (timetable[counter1 - 1] != minute) & (timetable[counter1 - 2] != 0):
										if (timetable[counter1] > minute) | (timetable[counter1] == 0) & (timetable[counter1 - 1] != minute):
											savehighregisteraddress = counter1
											counter = counter + 1
											counter2 = counter2 + 1
										else:
											if timetable[counter1 - 1] == minute:																								
												print"\nError: Invalid Input - Your minute alarm already exists in this hour\n"
												counter2 = counter2 + 1																													
						else:
							print"\nError: Invalid Input - Your typed in minute is not existing!\n"
                                                                
					counter = 0
					
					registeradd = savehighregisteraddress 	#Logic part for adding the minute the right register and everything after goeas one back
					i = i - registeradd

					for x in range (0,i):

						h = x + registeradd
						timetablesave.append(0)
						timetablesave[x] = timetable[h]


					timetable.append(0)
					for x in range (0,i):

						h = x + registeradd
						h = h +1
						timetable[h] = timetablesave[x]
					timetable[registeradd] = minute	
						
			else:
				print "\nError: Input invalid\n"
	else:
		
		counter = 2

		pickle.dump(timetable, open("/home/pi/make/timetableUpdate.p","wb"))

