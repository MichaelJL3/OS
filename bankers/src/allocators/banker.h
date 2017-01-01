
#ifndef BANKER_H //define guard
#define BANKER_H

#include <vector>
#include <iostream>
#include <string>
#include <assert.h>
#include <cstdlib>

/*=================================================================================================*\
|  banker.h 																				    |
|  Author: Michael Laucella 																		|
|																									|
|  Defines the MACROS, functions, and variables used for banker.cpp and the class safe           |
|																									|
|  for the MACROS they apply to 3 things a specififc integer* which is the current running line 	|
|  from the input known as "list" the 2d array keeping tracking of changes known as "status" and 	|
|  the 3d array keeping track of changes per resource known as "data"								|
|																									|
|  for brief summary of methods and variables read below											|
|  for more detailed information check out banker.cpp 											|
|																								    |
\*_________________________________________________________________________________________________*/


//USED IN STATUS FOR RUNNING AND BLOCK
#define TERMINATED -1   //used to signify process has completed or aborted 

//USED IN DATA TO TRACK ANY ALLOCATION AND RELEASE
#define ALLOCATED 0     //index to store size of allocated data
#define RELEASE 1       //index to store size of released data
#define CLAIM 2

//USED IN LIST TO DETERMINE BASIC INDEXING FOR INPUT LINE
#define TYPE 0			//index for type of request the task is making
#define DELAY 2         //index for the size of the delay
#define AMOUNT 4        //index for the amount of to be used 

//USED IN STATUS TO INDEX CHANGES
#define RUN 0   		//index to track which line a process is on
#define CYCLE 1         //index to track whats the cycle of the processe
#define BLOCK 2         //index to track how many units a process is blocked for
#define RESOURCE 3      //index to track which resource a process is blocked on
#define WAIT 4          //index to track how long a process has waited


using namespace std;

class safe{
	int running;		//number of running processes
	int process;		//number of processes
	int resource; 		//number of resources
	int offset;			//list offset when deleting
	int cycle;			//tracks the length of the run
	int* units;			//number of units per resource
	int** status;		//tracks status's of a process
	int*** data;		//tracks any allocation and relase of a process per resource
	vector<int> runlist;		//list of available processes
	vector< vector<int*> > inp; //the transformed input data from the file
public:
	safe(int p, int r, int* u, vector< vector<int*> > i);   //instatiation
	~safe();
	void state(int* line, int indx, int pos);				//process the next line in the list
	short issafe(int pos);  	//checks if request is safe
	void manager();				//maintains the loop until all process are complete
	void release();				//releases any unallocated data at the end of a cycle
	void run(int indx);			//runs the available process
	void abort(int pos);		//abort process whose allocations exceed their claims or require a higher claim then the resource has in general 
	void output();				//prints end data summary
	int find(int n); 			//finds where in list a process is
};

#endif /* BANKER_H */