/*===================================================================*/

	Author: michael laucella 
	language c++

/*===================================================================*/


The file structure is split into 4 files, 2 headers and 2 cpp files
the hierarchy: "the -> indicates file within a directory and the tab indicates
a directory within a directory"

             goes src -> main.cpp/h
			      simulator -> driver.h/cpp 

The simulator has a class holding several variables and 
functions each one documented and commented on within their respective files

the eviction algorithms are:
	LRU
	FIFO
	RANDOM

the algorithms are not their own classes or objects but functions that are
part of the simulator class in driver.cpp/h. Seeing as the eviction algorithm just chooses which frame to evict it they were fairl easy to implement as small functions

there is also a second folder and exe known as vsrc and vpaging.exe
the v=verbose, its a non commented out and slightly different version
but it is useful in debugging and comparing the output, however note the 
files are not the same as in the regular src (the changes are rather small
just optimizations i made to the src after all the inputs passed) 

for compiling with the makefile simply type 
	make comp - for regular src
	make compv - for verbose src
	make compall - to compile both

there are also two compile scripts which compile both when run 
	comp.bat and comp.sh
	these will run when double clicked

for running the file it compiles as paging so ./paging or paging.exe

if file rand is not found an exit(0) will be raised
if not enough args are given a message will prompt with a proper example
	of the arguments and then an exit(0) will occur

there is a python script which is used to run all the examples given when
running type "python runall.py" to run however if you pass a cmd arg of V or v
"python runall.py v" it will run the verbose version. the script will prompt for an input number to run if the number x is 0<x<17 it will run one of the 16 available inputs, else it will run all of them. The python script will display the data unless told not to otherwise with a flag r or R. in which case all the files in the output folder will be updated 

the result of the program is directly printed as IO however the python script also redirects the output to a file so you can view them afterwards in the output folder 
*NOTE: the running the program regularly will present IO not file output only the scripts will do that

in worst case scenario there is also a makefile type make comp to compile the generic src

There are plenty of comments within every file to help guide your understanding

Read from main first then to the headers before the actual driver/simulator it will help your understanding much better, there are a lot of MACROS defined in the headers which are used for indexing multidimensional arrays, and the MACROS make it easier to understand what information is being accessed
