#ifndef DRIVER_H //define guard
#define DRIVER_H

#include <cstdlib>
#include <fstream>
#include <queue>
#include <vector>

/*=================================================================================================*\
|  driver.h 																					   	|
|  Author: Michael Laucella 																		|
|																									|
|  Defines the MACROS, functions, and variables used for driver.cpp						          	|
|																									|
|  the MACROS apply to a specififc position in an integer* 											|
|  they are used with 2* a 1d* holding the program arguments to pass to the driver 					|
|  and a 2d* holding variables and resources for each process 										|
|																									|
|  the MACROS also apply to the basic quantum for RR and the MAXINT									|
|																									|
|  for brief summary of methods and variables read below											|
|  for more detailed information check out driver.cpp 												|
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

//MAXINT FOR USE WITH THE RANDOM NUMBER GENERATORS
#define MAXINT 2147483647

//QUANTUM FOR USE WITH THE DRIVER
#define Q 3

using namespace std;

//FRAME STRUCT
struct Frame{
	int pageheld;	//page held by the frame
	int process;	//process the page belongs to
	int loaded;		//when page was loaded
	int last;		//last reference made to frame
};

//
class Simulate{
	unsigned int size;		//size of process
	unsigned int page;		//page size
	unsigned int process;  	//number of processes
	unsigned int machine;	//machine size
	unsigned int refs;		//number of refrences per process
	unsigned int maxframe;	//number of frames
	double** status;		//status array for processes - for more information on status read main.cpp/.h
	vector< vector<int> > framesheld; //a list holding lists of all held frames of a process for all processes 
	fstream rd;		//file stream for opening and reading files 
	queue<int> p;	//queue holdinf the running processes
	Frame* frames;	//array of frames
public:
	//CONSTRUCTORS/DESTRUCTORS
	Simulate(unsigned int* global, double** paststatus);
	~Simulate();
	//FRAME FUNCTIONS
	void newframe(int maxframe, int curr, int pg, int cycle);
	void evictframe(int type, int pagen, int curr, int cycle);
	void remove(int loc, int cycle, int curr, int pagen);
	void referenced(int currframe, int cycle);
	int* findframe(int n, int pg);
	//EVICTION ALGORITHMS
	int fifo();
	int lru();
	int random();
	//BASIC DRIVER REFERENCING  
	void driver(int type);
	int randomOS();
	int nxtref(int i, int w);
};

#endif