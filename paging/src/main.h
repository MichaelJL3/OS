#ifndef MAIN_H //define guard
#define MAIN_H

#include <cstdlib>
#include <string>
#include "simulator/driver.cpp"

/*=================================================================================================*\
|  main.h 																					    	|
|  Author: Michael Laucella 																		|
|																									|
|  Defines the MACROS, functions, and variables used for main.cpp						          	|
|																									|
|  the MACROS only apply to a specififc position in an integer* 									|
|  they are used with 2* a 1d* holding the program arguments to pass to the driver 					|
|  and a 2d* holding variables and resources for each process 										|
|																									|
|  for brief summary of methods and variables read below											|
|  for more detailed information check out main.cpp 												|
\*_________________________________________________________________________________________________*/

//FOR USE WITH THE STATUS ARRAY
#define A 0			//A 
#define B 1			//B ~ as specified in the pdf and readme (pertain to word selection and probabilities for a process)
#define C 2			//C
#define FAULT 3		//tracks the number of faults a process has
#define EVICT 4		//tracks the number of evictions a process has
#define TOTAL 5		//tracks the total time each page is in before removal

//FOR USE WITH THE ARGS ARRAY
#define MACH 0		//references the location of the machine size 
#define PAGE 1		//references the location of the page size 
#define SIZE 2		//references the location of the process size 
#define REFS 3		//references the location of the references per process 
#define MAXF 4		//references the location of the size of the frametable
#define PROC 5		//references the location of the number of processes

using namespace std;

double** status;	//2d array tracking the evictions, faults, and other information per process

double** setup(int n);		//allocates data for the status array based on how many processes are running
int preprocess(int job);	//determines the number of processes running and sets up basic probabilities 
int algorithm(string s);	//determines which type of eviction method to use based on the cmd args
void output(int process);	//displays the summary data

#endif