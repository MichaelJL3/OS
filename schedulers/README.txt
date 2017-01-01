/*===================================================================*/

	Author: michael laucella 
	language c++

/*===================================================================*/

The file structure is very intertwined the main process the basic file
input and information and calls 4 scheduling algorithm functions
which themselves instatiate a scheduler object

The scheduler object is a class holding several variables and 
functions that all 4 algorithms run with the exception of 2 ready functions
one for HPRN and one for LCFS 

The algorithms themselves are practically scheduler objects making most of
their function calls and variable through the scheduler object however it looked 
cleaner and simpler to understand by splintering them into different files

the schedule algorithms are:
	FCFS - first come first serve
	RR	 - round robbin
	LCFS - last come first serve
	HPRN - high penaly ratio next

the file lists + directories are as follows
	-main: OSscheduler.h/.cpp
		-rand: rand.txt
		-algorithms: FCFS.cpp HPRN.cpp/.h LCFS.h/.cpp RR.cpp
			-scheduler: scheduler.h/.cpp

the rand.txt holds about 4000 numbers

for compiling with the makefile simply type make cmp
for running the file it compiles as sched so ./sched or sched.exe
if no args are provided an assertion 0 will be raised

for running on windows system double click the batch file
for unix systems double click the shell script
there are two options print/file
	the script will first prompt for input file with no extension
	then prompt for a response for verbose flag as either 1 or 0

	verbose 1 means print verbose or else default will go to non verbose

	note that although 1 and 0 are used for verbose flag it takes an arg of
	--verbose which is exactly what the script itself will pass 

	The script will display the results as CMD and pipe them to a file in the
	output folder

	the operating procedure is to enter a number for the file so for example:
	input1 = 1 input2 = 2 it automatically places the input when using the number
	later there is also a run all script but only for windows systems
	

each script checks for an compiles the file if it is non existent

in worst case scenario there is also a makefile type make cmp to compile

The file runs all 4 algorithms so in order to run a singular one say FCFS
you need to go in to OSscheduler and comment out the other 3 

There are plenty of comments within every file to help guide your understanding
