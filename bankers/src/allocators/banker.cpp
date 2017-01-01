#include "banker.h"

/*=================================================================================*\
|  banker.cpp																		|																		
|  Author: Michael Laucella															|
|																					|
|  runs banker resource allocator													|
|  takes 4 paramaters upon instantiation 											|
|	  p = number of processes														|
|	  r = number of resources														|
|	  *u = list containing number of units per resource 							|
|	  vector< vector<int*> > i = the structured list of instructions 				|
|	   	 brokendown by process -> resource -> instructions for line      			|
|																					|
|  uses a 2d and 3d array to track all processes information and changes 			|
|  uses a runlist list which removes and inserts processes based						|
|  on if they are considered safe or not 											|
|	   																				|										|
|  for basic overview of class methods and MACROS check the banker.h file 	        |
|																					|
\*__________________________________________________________________________________*/

///////////////////////////CONSTRUCTOR////////////////////////////////////////////

safe::safe(int p, int r, int* u, vector< vector<int*> > i){
	/*
		constructor for class opt:

		creates a 3d and 2d array one tracking the status of a process
		and one keeping track of elements the process contains per resource
		all initially set to 0 
	*/
	running=p;
	process=p;
	resource=r;
	units=u;
	inp=i;
	cycle=0;
	data=new int**[process];
	status=new int*[process];

  	for (int i=0; i<process; i++){	//creating the 2d and 3d arrays
  		runlist.push_back(i);
    	data[i]=new int*[resource];
    	status[i]=new int[5];
    	for (int k=0; k<5; k++)	//the five are run, cycle, wait, block, resource
    		status[i][k]=0;
		for (int j=0; j<resource; j++){
			data[i][j]=new int[3];
			for (int k=0; k<3; k++) //the three are allocated, released, claim which are all resource dependent
    			data[i][j][k]=0;
    	}
  	}
}

safe::~safe(){	//destructor delete allocated data
	int* temp;
	for (int i=0; i<process; i++){	
    	delete status[i];
		for (int j=0; j<resource; j++){
			temp=data[i][j];
			delete temp;
    	}
  	}
}

////////////////////////////CONTROL FLOW//////////////////////////////////////////

void safe::manager(){
	/*
		manager runs the system while there are still unfinished tasks
		1: runs all runlist and capable canditates 
		2: it releases any units released during the cycle
	*/
	int size=0;
	while(running){
		offset=0;
		size=runlist.size();
		for(int i=0; i<size; i++)
			run(i);
		release();
		cycle++;
	}
	output();
}

void safe::state(int *line, int indx, int pos){
	/*
		runs the line and determines outcome 
		type - indicates one of the following
		0:    initialize
		1:    request
		2:    release
		else: terminated
	*/
	int resrc=line[RESOURCE]-1;	//the resource the line specifies
	int type=line[TYPE];		//which type of action for the switch statement
	int amount=line[AMOUNT];	 //the amount to used
	switch(type){
		case 0:	//initialized
			status[pos][RUN]++;
			if(amount>units[resrc]){	//the claim exceeds the max available abort it
				abort(pos);	
				runlist.erase(runlist.begin()+indx);
			}
			else //succeeded
				data[pos][resrc][CLAIM]=amount;				//set the processes claim 
			break;
		case 1:  //request
			if(!issafe(pos)){	//request not safe: not granted
				status[pos][WAIT]++;						//increase the time process has waited
				runlist.erase(runlist.begin()+indx);		//remove the block process from its location in the list
				runlist.insert(runlist.begin()+offset, pos);//blocked becomes head of list but back of block list hence the offset
				offset++;									//track where the next block process should go
			}
			else{	//request is safe: granted 
				status[pos][RUN]++;							//set next line to run
				data[pos][resrc][ALLOCATED]+=amount;		//add to the amount of the resource allocated to the process
				data[pos][resrc][CLAIM]-=amount;			//subtract the amount requested from the processes claim of that resource
				units[resrc]-=amount;						//subtract the amount requested from the resource
				if(data[pos][resrc][CLAIM]<0){				//if the process has exceeded its claim abort it
					abort(pos);								//reset all the processes stats set to aborted
					runlist.erase(runlist.begin()+indx);	//remove that process from the list
				}
			}
			break;
		case 2:		//released 
			data[pos][resrc][RELEASE]+=amount;	 //sets amount to release at end of cycle
			status[pos][RUN]++;					 //set next line to run
			break;
		default: 	//terminated
			status[pos][RUN]=TERMINATED;			//set process as complete
			status[pos][CYCLE]=cycle;				//set the cycle of termination
			runlist.erase(runlist.begin()+indx);	//remove process from the list
			running--;	//decrement number of running processes
			break;
	}
}

void safe::run(int indx){
	/*
		checks which process is running
		0: if delayed it adds to the cycle use and moves to the next
		1: if not delayed it processes the processes line 
	*/
	int pos=runlist[indx];
	int index=status[pos][RUN]; //which line the process is on
	int *list=inp[pos][index];  //list of instructions for the line
	if(list[DELAY])	     	    //delayed
		list[DELAY]--;
	else						//processes line
		state(list, indx, pos);
}

///////////////////////////OUTPUT////////////////////////////////////////////

void safe::output(){
	/*
		prints the end data 
		0: if process was aborted print aborted
		1: else print processes summary 
		2: then display averages of the data
	*/
	int t_cycle=0;
	int t_wait=0;
	int abrt=0;
	printf("\n\tBanker\n");
	for(int i=0; i<process; i++){
		printf("Task %d \t ", i+1);
		if(status[i][BLOCK]==TERMINATED)	//if task is aborted
			puts("aborted");
		else{
			printf("%d   %d   %3.0f%%\n", status[i][CYCLE], status[i][WAIT], (((double)status[i][WAIT]/(double)status[i][CYCLE])*100)); 
			t_cycle+=status[i][CYCLE];
			t_wait+=status[i][WAIT];
		}
	}
	printf("total\t %d   %d   %3.0f%%\n", t_cycle, t_wait, (((double)t_wait/(double)t_cycle)*100)); 
}

////////////////////////////RELEASING DATA////////////////////////////////////////

void safe::release(){
	/*
		cycle through the list if any unit was released then add it back to the 
		resource it was taken from and subtract from the processes allocated amount
	*/
	int rel=0;
	for(int r=0; r<resource; r++){
		for(int p=0; p<process; p++){
			rel=data[p][r][RELEASE];
			units[r]+=rel;
			data[p][r][RELEASE]=0;
			data[p][r][CLAIM]+=rel;		//add released data back to the claimable
		}
	}
}

////////////////////////////ABORTING PROCESSES////////////////////////////////////

void safe::abort(int pos){
	/*
		aborts process
		0: release all of its allocated data per resource
		1: set process state to terminated
	*/
	for(int r=0; r<resource; r++){
		units[r]+=data[pos][r][ALLOCATED];
		data[pos][r][ALLOCATED]=0;
	}
	status[pos][CYCLE]=cycle;
	status[pos][BLOCK]=TERMINATED;  //block set to aborted
	status[pos][RUN]=TERMINATED;	//run set to complete
	running--; 						//decrement number of running processes
}

////////////////////////////BANKERS SAFE//////////////////////////////////////////

short safe::issafe(int pos){	//is the request safe
	/*
		logic: if a request has been granted it is safe and can complete the process
		however if a new process also has its request granted it can also complete with
		what is available so the state is still safe as one of the 2 is gauranteed to 
		currently finish and the return is determined to complete the first request 

		check if per resource request is safe else return unsafe
	*/
	for(int r=0; r<resource; r++)	//check per resource of requestee
		if(data[pos][r][CLAIM]>units[r])  //if the amount of claim the process still has left is greater then the units of that resource state is unsafe
			return 0;
	return 1;	//state is safe
}