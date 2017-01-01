/*===================================================================*/

	Author: michael laucella 
	language c++

/*===================================================================*/

The file structure is simple the main runs the program relying on 
a struct of objects which together make up a module

the program reads from standard input until nothing else is received
then parses the data into a set of data structures and analyzes them

the module classes are:
	memory - addresses
	redefine - uses
	symbol - symbols

which can be found in the linker/module folder
every file has its own subsequent header file

for running on windows system double click the batch file
for unix systems double click the shell script

each script checks for an compiles the file if non existent
it then asks for the name of a text file to pipe to the program
if nothing is entered it will enter keyboard standard input
program will then display info 

in worst case scenario there is also a makefile type make cmp to compile

if running from keyboard use # as an ending delimeter it must be used as 
the ending of the module meaning after the memory section ex: 0 0 0 # 
(x=sym, x=use, x=mem, #)
