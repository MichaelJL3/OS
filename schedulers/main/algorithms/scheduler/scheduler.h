#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include <fstream>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
	Scheduler file holds the main work load for each process, because they are such
	interconnected structures all the algorithms create a scheduler object and use
	its processes with any small deviation taken care of in the algorithm rather then
	the schedulers functions

	note default ready function only applies to FCFS and RR it is built to have quantum 
	capability

	for HPRN and LCFS they have their own forms of a ready function using variables from 
	the scheduler
*/

class scheduler{
public:
	int q, qincr, cycle, size;	//basic variables 
	int *list;				//Holds currently loaded int* from vector
	int *ready_list;		//Stores number of cycles process has been ready
	int *block;				//Stores processes blocked count
	int *state;				//Stores processes state
	vector<int*> local;		//Points to original vector
	short run;				//Run flag
	ifstream rd;			//File reader
public:
	scheduler();					//Instantiates random file reader
	~scheduler();					//Erase allocated objects
	int randomOS(int n);			//Generates random int from next line in file
	int empty(int index);			//Checks if all processes have finished running
	void print_process();			//Prints ending summary
	void print_verbose(int index);	//Prints verbose running information
	void process(int index);		//Main control unit decide where a process is going based on its state
	void blocked(int index);		//Blocked process
	void running(int index);		//Running process
	void arriving(int index);		//Arriving process unless gets called for states of delay
	void ready();					//Ready process determines at end of cycle who is eligible to run
	void setup();					//Reset changes to the list		
};

#endif