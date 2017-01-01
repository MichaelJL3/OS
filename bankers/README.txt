/*===================================================================*/

	Author: michael laucella 
	language c++

/*===================================================================*/

The file structure is split into 5 files 2 headers and three cpp files
the hierarchy: "the -> indicates file within a directory and the tab indicates
a directory within a directory"

             goes src -> allocator.cpp
			      allocators -> banker.h/cpp optimistic.h/cpp

The allocators each have a class holding several variables and 
functions each one documented and commented on within their respective files

the Allocators are:
	Bankers
	Optimistic

for compiling with the makefile simply type make comp
for running the file it compiles as sched so ./alloc or alloc.exe
if no args are provided or the file is not found an assertion 0 will be raised

for running on windows system double click the batch file
their is also a run all batch for the windows which will run the 
provided sample input files 1-10 

the result of the program is directly printed as IO however the scripts also
redirect the output to a file so you can view them afterwards in the output folder
*NOTE: the running the program regularly will presemt IO not file output only the 
	scripts will do that

for unix systems double click the shell script

	the operating procedure for the single run script is to enter a number for the file so for example:
		input1 = 1 input2 = 2 it automatically places the input when using the number

each script checks for an compiles the file if it is non existent

in worst case scenario there is also a makefile type make comp to compile

The file runs both allocators so in order to run a singular one say Bankers
you need to go in to allocator.cpp and comment out

There are plenty of comments within every file to help guide your understanding

Read from allocator first then to the headers before the actual allocators it will help your understanding much better, there are a lot of MACROS defined in the headers which are used for indexing multidimensional arrays, and the MACROS make it easier to understand what information is being accessed
