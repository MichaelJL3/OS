#include "scheduler.h"

/*
	holds all the bulk of the scheduling algorithms
	
	main process include
		-process
		-blocked
		-running
		-arriving
		-ready

	for more information ready below or look at scheduler.h for preview and basic description
	of all
*/

scheduler::scheduler(){		//sets up basic variables and connects to rand file
	rd.open("main/rand/rand.txt");
	if(!rd.is_open()){
		cout << "Random Number File Not Found" << endl;
		assert(0);
	}
	q=0;
	cycle=0;
	run=0;
}

scheduler::~scheduler(){	//deletes all the allocated content and closes the file
	delete(state);
	delete(block);
	delete(ready_list);
	rd.close();
}

void scheduler::print_process(){	//prints the summary
	double cpu_util=0, io_util=0;
	double wait=0, turn=0;
	double throughput=100*size;
	throughput/=--cycle;
	for(int i=0; i<size; i++){
		cout << "Process: " << i << endl << "\t(A,B,C,IO) = (";//print process
		for(int j=0; j<4; j++){
			if(j)
				cout << ",";
			cout << local[i][j];
		}
		cout << ")" << endl		//then print its basic specifications
			<< "\tFinishing time: " << local[i][5] << endl 
			<< "\tTurnaround time: " << local[i][5]-local[i][0] << endl 
			<< "\tI/O time: " << local[i][6] << endl
			<< "\tWaiting time: " << local[i][7] << endl << endl;
		wait+=local[i][7];		//take the sum of these specs for further calculation and averaging
		turn+=local[i][5]-local[i][0];
		cpu_util+=local[i][2];
		io_util+=local[i][6];
	}	
	cout <<"Summary Data:" << endl 	//calculate and display the averages
		<< "\tFinishing time: " << cycle << endl
		<< "\tCPU Utilization: " << cpu_util/cycle << endl
		<< "\tI/O Utilization: " << io_util/cycle << endl
		<< "\tThroughput: " << throughput << " processes per hundred cycles" << endl
		<< "\tAverage turnaround time: " << turn/size << endl
		<< "\tAverage waiting time: " << wait/size << endl << endl 
		<< "======================================================" << endl;
}

void scheduler::print_verbose(int index){	//verbose print
	if(index)	//dont tab the 0th member of the cycle "looks better"
		cout << "\t"; 
	switch(state[index]){	//print the state of the process
		case 0:  cout << " blocked " << block[index]; break;
		case 1:  
			//if it is running it requires a separate case for RR because it prints in terms of the quantum
			if(!q||list[4]==1) cout << " running " << list[4]; 
			else cout << " running " << q-run+1;
				break;
		case 2:  cout << " ready 0 "; local[index][7]++; break;	//add to wait as well
		case 3:  cout << " unstarted 0"; break;
		default: cout << " terminated 0 "; break;
	}
}

int scheduler::randomOS(int n){		//returns number from file
	unsigned int x=0;
	string s;
	getline(rd,s);
	x=atoi(s.c_str());
	return (x%n+1);
}

void scheduler::blocked(int index){		//check if process is blocked
	if(block[index])
		block[index]--;					//if it is decrement
	if(!block[index]&&!state[index])	//if it isnt and ist state was blocked set to ready
		state[index]=2;
}

void scheduler::running(int index){
	int io=0;
	list[4]--;	//decrement the cpu burst
	list[8]--;	//decrement the total run
	list[9]++;	//add to the cpu burst run

 	if(!list[4]){					//if cpu burst is complete
 		run=0;						//set run flag to 0
		if(list[8]){				//if run hasnt depleted as well
			io=randomOS(list[3]);	//generate io block
			block[index]=io;		//set block
			list[6]+=io;			//add up io time "for analytical purposes later"
			state[index]=0;			//set state to blocked
		}
		else{						//if finished
			list[5]=cycle;			//total cpu time
			state[index]=4;			//set state to temrinated
		}
	}
	else if(!list[8]){		//else if run is depleted but not cpu
		list[4]=0;			//set burst to 0
		run=0;				//set run to 0
		list[5]=cycle;		//total cpu time
		state[index]=4;		//set to terminated
	}

	//================================RR ONLY=====================================
	else if(run==q){			//else if its run the amount of time as quantum
		run=0;					//set run to 0
		state[index]=2;			//set state to ready
		ready_list[index]++;	//add to ready waits
	}
	else						//else add up the run count
		run++;
}

void scheduler::arriving(int index){	//process incoming process
	if(local[index][0]<=cycle){			//if delay is smaller then the cycle
		ready_list[index]++;			//add to its ready wait
		state[index]=2;					//set to ready
	}
}

void scheduler::ready(){		//determine which process runs next
	int highest=0;
	int run_next=0;
	if(!run){
		for(int i=0; i<size; i++){
			if(ready_list[i]>highest){		//search for longest waiting
				run_next=i;
				highest=ready_list[i];
			}
		}
		if(ready_list[run_next]){		//if it exists
			ready_list[run_next]=0;		//set waiting record to 0
			state[run_next]=1;			//set is state to run
			run=1;						//set run flag
			if(!local[run_next][4])		//if cpu burst is 0 generate new burst
				local[run_next][4]=randomOS(local[run_next][1]);
		}
	}
}

void scheduler::process(int index){
	blocked(index);		//check if its blocked 
	//important note it is outside of the switch because if blocked switches the state to
	//ready then it has to run the other case 2
	switch(state[index]){
		case 1: running(index); break;	//if 1 run running
		case 3: arriving(index); break; //if 3 run arriving only happens per process until its in the system, state 3 can never be reobtained
		case 2: ready_list[index]++; break; //if its ready add to its ready wait
		default: break;
	}
}

int scheduler::empty(int index){	//check for all run terminations plus that it has cycled back to 0 for printing purposes
	if(size>1&&index)				//if it consists of more then one element and its not on index 0 return false 
		return 0;
	for (int i=0; i<size; ++i){		//else check the list if a process still hasnt depleted return 0
		list=local[i];
		if(list[8])
			return 0;
	}
	return 1;						//else it is empty
}

void scheduler::setup(){			//resets items to use in schedule
	ready_list=new int[size];
	state=new int[size];
	block=new int[size];			//initiates and fills appropriately
	for(int i=0; i<size; i++){
		local[i][8]=local[i][2];	//resets the run depletions
		local[i][5]=0;				//total run time
		local[i][6]=0;				//total io time
		local[i][7]=0;				//total wait time
		local[i][9]=0;				//total run time
		block[i]=0;					//no process is blocked
		state[i]=3;					//all are unstarted
		ready_list[i]=0;			//none have been ready
	}
	//list[5] and list[9] look like the same thing but the difference is that one is an 
	//immediate sum list[5]=cycle when its run has depleted where as list[9] increments
	//per run for use in HPRN
}