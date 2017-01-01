#include "main.h"

/*=============================================================================================================*\
|  main.cpp					 																					|
|  Author: Michael Laucella 																					|
| 																												|
|  primary function of this file is to setup the necessary conditions and variable for the 						|
|  paging simulator using one of the three specified eviction algorithms										|
|     -LRU 																										|
|     -FIFO 																									|
|	  -RANDOM 																									|
|																												|
|	the process data and probilities for next word run are setup as a 2d array called status 					|
|																												|
|																												|
|   if not enough args are passed or the random file is not found the code will exit 							|
|																												|
|   for more informtation read below and view main.h for a look at the MACROS and their definitions             | 
\*_____________________________________________________________________________________________________________*/

////////////////////////////////////////////SETUP//////////////////////////////////////////

double** setup(int n){	//allocate a 2d array of size n
	double** ret=new double*[n];
  	
  	for (int i=0; i<n; i++){
  		ret[i]=new double[6];	//each array holds 6 fields: A,B,C,EVICT,FAULT,TOTAL
    	for (int k=0; k<6; k++)	
    		ret[i][k]=0;		//initialize all as zero
  	}
	return ret;
}

int preprocess(int job){
	/*
		takes the job mix number and allocate for the number of processes
		then distributes the probabilities given for the job mix
	*/
	int p=0;
	switch(job){	//set up process arrays
		case 1: 	//1 processes
			p=1; 
			status=setup(p); 
			status[0][A]=1;
			break;
		case 2: 	//4 processes
			p=4; 
			status=setup(p);
			for(int i=0; i<p; i++)
				status[i][A]=1;
			break;
		case 3: 	//4 processes
			p=4; 
			status=setup(p); 
			break;
		default: 	//4 processes
			p=4; 
			status=setup(p);
			for(int i=0; i<3; i++)
				status[i][A]=0.75;
			for(int i=2; i<4; i++){
				status[i][B]=0.125;
				status[i][C]=0.125;
			}
			status[0][B]=0.25;
			status[1][C]=0.25;
			status[3][A]=0.5;
			break;
	}
	return p;	//return number of processes
}

int algorithm(string s){	//determine which eviction algorithm to use
	int length=s.length();
	for(int i=0; i<length; i++)	//turn string provided to lower case
		s[i]=tolower(s[i]);
	if(s.find("lru")!=string::npos)	//if lru set to 0
		return 0;
	else if(s.find("fifo")!=string::npos) //if fifo set to 1
		return 1;
	else					//else assume random set to 2
		return 2;
}

////////////////////////////////////////////SUMMARY DATA//////////////////////////////////////////

void output(int process){
	/*
		displays the faults per process and average residency
		if no evictions occured no residency is displayed
		the residency is the the time a process was in divided by the number of eviction
	*/
	int evict=0;
	int faultsum=0;		//sum of the faults
	int count=0;
	short tot=0;
	double avg=0.0;		//local average
	double avgsum=0.0;	//final average
	puts("");
	for(int i=0; i<process; i++){
		faultsum+=status[i][FAULT];	 //sum of the faults
		printf("process %d had %d faults", i+1, (int)status[i][FAULT]);
		evict=status[i][EVICT];
		if(evict){	  //if there has been an eviction for the process
			tot=1;						//flag tells final average there has been at least one eviction
			count+=evict;				//keep a count of the process with evictions for final average
			avg=status[i][TOTAL]/evict;	//average residency
			avgsum+=status[i][TOTAL];	//add up the averages
			printf(" and %.2f average residency.\n", avg);
		}
		else	//if there hasn't been an eviction for the process
			puts(". With no evictions, the average residence is undefined.");
	}
	printf("\nThe total number of faults is %d", faultsum); 
	if(tot)	//if there has been at least one eviction among all the processes
		printf(" and the overall average residency is %.2f.\n", avgsum/count);
	else	//if there hasn't been a single eviction
		puts(". With no evictions, the overall average residence is undefined.");
}

//////////////////////////////////////////////////MAIN///////////////////////////////////////////////

int main(int argv, char** argc){
	/*
		takes the arguments provided and places them into an integer array
			to pass to the simulator
		sets up a 2d status array for the processes based on the job mixes
			process numbers and probabilities
		creates a simulator object
		runs the simulator
		displays the output
		deletes allocated data
	*/
	if(argv<7){		//less than 6 args provided exit program display proper warning
		puts("ERROR: not enough arguments given\n\t example: paging.exe 10 10 20 1 10 10 lru");
		exit(0);
	}

	unsigned int job;			
	unsigned int type;			
	unsigned int process;		
	unsigned int args[6];		//an array holding arguments for the driver
	args[MACH]=atoi(argc[1]);	//machine size in words
	args[PAGE]=atoi(argc[2]);	//page size in words
	args[SIZE]=atoi(argc[3]);	//process size: virtual address references
	job=atoi(argc[4]);   		//the job mix
	args[REFS]=atoi(argc[5]);	//the number of references for each process
	string replacement=argc[6];	//the replacement algorithm

	type=algorithm(replacement); //type of eviction algorithm to use
	args[MAXF]=args[MACH]/args[PAGE];	//number of frames
	process=preprocess(job);	 //number of processes
	args[PROC]=process;

	printf("The machine size is %d.\nThe page size is %d.\n"
		   "The process size is %d.\nThe job mix number is %d.\n"
		   "The number of references per process is %d.\n"
		   "The replacement algorithm is %s.\n", 
		   args[MACH], args[PAGE], args[SIZE], job, args[REFS], replacement.c_str());
	
	Simulate paging(args, status);  //setup simulator with proper arguments
	paging.driver(type); //simulate demand paging with the eviction algorithm given
	output(process);	 //print summary results of the paging 
        
    for(int i=0; i<process; i++)  //clear out allocated data
    	delete status[i];

	return 0;
}