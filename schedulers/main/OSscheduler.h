#ifndef OS_H
#define OS_H

#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include "algorithms/FCFS.cpp"
#include "algorithms/RR.cpp"
#include "algorithms/LCFS.cpp"
#include "algorithms/HPRN.cpp"

using namespace std;

/*
	OSscheduler holds the main function it reads in the user inputed data
	then grabs the file for running and activates the scheduling algorithms

	-note if no file is provided a default input1 is used if it cannot locate
	the file an assert(0) is raised

	-also for detailed input use --verbose as the first cmd argument
*/

vector <int*> data;		//vector of int* each int* is a set (A,B,C,D) + other sections for storing information to analyze
int trck=0;				//keep track of where in the input string you are

//void priority(string input);	//look in OSscheduler.cpp for more detail on this extra function
string read_in(string file);	//reads input from file
int interpret(string input);	//analyzes the input
int nextnumber(string input);	//parses next number in the string for the set
void sort(int size);			//crude sorting algorithm "deemed less essential in the scope of things"
void print_vec();				//prints vector 

#endif