#include "optimistic.h"

/*=================================================================================*\
|  optimistic.cpp																	|																		
|  Author: Michael Laucella															|
|																					|
|  runs optimistic resource allocator												|
|  takes 4 paramaters upon instantiation 											|
|	  p = number of processes														|
|	  r = number of resources														|
|	  *u = list containing number of units per resource 							|
|	  vector< vector<int*> > i = the structured list of instructions 				|
|	   	 brokendown by process -> resource -> instructions for line      			|
|																					|
|  uses a 2d and 3d array to track all processes information and changes 			|
|  uses a set of 2 lists block and ready  											|
|	  -block keeps track of the blocked processes and only runs a loop at  			|
|	   the end of a cycle   														|
|	  -ready are the available processes and run in the beginning of the   			|
|	   cycle 																		|
|  the two lists are swapped between as processes become blocked and unblocked 		|
|  for more detailed information read below 										|
| 																					|
|  for basic overview of class methods and MACROS check the optimistic.h file       |
|																					|
\*__________________________________________________________________________________*/


///////////////////////////CONSTRUCTOR////////////////////////////////////////////

opt::opt(int p, int r, int* u, vector< vector<int*> > i){
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
  		ready.push_back(i);
    	data[i]=new int*[resource];
    	status[i]=new int[5];
    	for (int k=0; k<5; k++)	//the five are run, cycle, wait, block, resource
    		status[i][k]=0;
		for (int j=0; j<resource; j++){		
			data[i][j]=new int[2];
			for (int k=0; k<2; k++) //keeps track of the allocated and released amount per process
    			data[i][j][k]=0;
    	}
  	}
}

opt::~opt(){	//destructor delete allocated data
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

void opt::manager(){
	/*
		manager runs the system while there are still unfinished tasks
		1: runs all ready and capable canditates 
		2: checks if all are blocked and aborts if they are
		3: it releases any units released during the cycle
		4: unblocks any process now capable of running
	*/
	int size=0;
	while(running){
		offset=0;
		size=ready.size();
		for(int i=0; i<size; i++)
			run(i);
		if(block.size()==running&&running)  //if it still has running processes and they are all blocked
			abort();
		release();
		unblock();
		cycle++;
	}
	output();
}

void opt::state(int *line, int indx, int pos){
	/*
		runs the line and determines outcome 
		type - indicates one of the following
		0:    initialize
		1:    request
		2:    release
		else: terminated
	*/
	int resrc=line[RESOURCE]-1;  //the resource the line specifies
	int type=line[TYPE];         //which type of action for the switch statement
	int amount=line[AMOUNT];     //the amount to used
	switch(type){
		case 0: //initialized
			status[pos][RUN]++;  //initialized set next line to run
			break;
		case 1: //request 
			if(amount>units[resrc]){  //not granted: it needs more then what is available block
				status[pos][BLOCK]+=amount;  			//set the processes to the amount blocked
				status[pos][RESOURCE]=resrc; 			//set which resource it is blocked for
				block.push_back(ready[indx-offset]);  	//add to blocked list
				ready.erase(ready.begin()+indx-offset); //remove from ready list
				offset++;  
			}
			else{  //granted 
				status[pos][RUN]++;					  //set next line to run
				data[pos][resrc][ALLOCATED]+=amount;  //increase amount allocated
				units[resrc]-=amount;				  //deduct amount needed from resource
			}
			break;
		case 2: //released 
			data[pos][resrc][RELEASE]+=amount;  //sets amount to release at end of cycle
			status[pos][RUN]++;					//set next line to run
			break;
		default: //terminated
			status[pos][CYCLE]=cycle;				 //set cycle upon process termination
			status[pos][RUN]=TERMINATED; 			 //set process as complete
			ready.erase(ready.begin()+indx-offset);  //remove process from the list
			offset++;
			running--;   //decrement number of running processes
			break;
	}
}

void opt::run(int indx){
	/*
		checks which process is running
		0: if delayed it adds to the cycle use and moves to the next
		1: if not delayed it processes the processes line 
	*/
	int pos=ready[indx-offset];
	int index=status[pos][RUN]; //which line the process is on
	int *list=inp[pos][index];  //list of instructions for the line
	if(list[DELAY])			//delayed
		list[DELAY]--;
	else 					//processes line
		state(list, indx, pos);
}

///////////////////////////OUTPUT////////////////////////////////////////////

void opt::output(){
	/*
		prints the end data 
		0: if process was aborted print aborted
		1: else print processes summary 
		2: then display averages of the data
	*/
	int t_cycle=0;
	int t_wait=0;
	int abrt=0;
	printf("\n\tFIFO\n");
	for(int i=0; i<process; i++){
		printf("Task %d \t ", i+1);
		if(status[i][BLOCK]==TERMINATED)
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

void opt::release(){
	/*
		cycle through the list if any unit was released then add it back to the 
		resource it was taken from and subtract from the processes allocated amount
	*/
	int rel=0;
	for(int r=0; r<resource; r++){
		for(int p=0; p<process; p++){
			rel=data[p][r][RELEASE];
			units[r]+=rel;
			data[p][r][ALLOCATED]-=rel;
			data[p][r][RELEASE]=0;
		}
	}
}

void opt::restore(int n){
	/*
		if process has been aborted restore any of its allocated data to the 
		proper resource
	*/
	for(int r=0; r<resource; r++){
		units[r]+=data[n][r][ALLOCATED];
		data[n][r][ALLOCATED]=0;
	}
}

////////////////////////////BLOCK RELATED FUNCTIONS///////////////////////////////

void opt::abort(){
	/*
		check if the amount needed by a process is less then the resources available
		if it is delete the process and continue checking further processes 
		if not then there is no longer a deadlock return 
	*/
	int need=0;
	int resrc=0;
	int index=0;
	int pos=0;
	for(int indx=0; indx<process; indx++){
		if(status[indx][RUN]!=TERMINATED){	//dont check completed processes
			resrc=status[indx][RESOURCE];   //of which resource it needs
			need=status[indx][BLOCK];       //how much it needs
			if(need<=units[resrc]&&units[resrc])  //is there enough units available
				break;
			else{
				running--;  					//decrement number of running processes
				restore(indx);  				//release any allocated data by the process
				status[indx][RUN]=TERMINATED;   //set run status to complete
				status[indx][BLOCK]=TERMINATED; //set block status to aborted
				status[indx][CYCLE]=cycle;
				pos=find(indx);					//find where in the blocked list the process is
				block.erase(block.begin()+pos); //delete from list
			}
		}
	}
}

void opt::unblock(){
	/*
		check if any process in the block list can now unblock

		all processes within the block list never run the main loop 
		until they are placed in the ready loop so cycle increase 
		per blocked item occurs within this function as well as wait increase

		does not break after first unblock keeps sum of the unblocked needs
		to allow for more processes to unblock
	*/
	int size=block.size();
	int need=0;
	int has=0;
	int index=0;
	int resrc=0;
	for(int incr=0; incr<size; incr++){
		index=block[incr];				//which process is blocked
		need=status[index][BLOCK];		//check how many units it needs
		resrc=status[index][RESOURCE];  //check for which resource
		status[index][WAIT]++;  		//increase processes wait
		if(has+need<=units[resrc]){
			has+=need;					
			status[index][BLOCK]=0;		//remove the block
			block.erase(block.begin()+incr);  //erase from the blocked list
			ready.insert(ready.begin(), index);  //add to start of ready list
			size--;  //offsetting the removed processes
			incr--;
		}
	}
}

/////////////////////////////OTHER METHODS////////////////////////////////

short opt::find(int n){		//locates process in block list
	int size=block.size();
	for(int i=0; i<size; i++){
		if(block[i]==n)		//if found 
			return i;
	}
	//assertion error major problems if process determined to be in the list is not in it
	printf("index error could not locate %d in list of blocked processes\n", n);
	assert(0);
}