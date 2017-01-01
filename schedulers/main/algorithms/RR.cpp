#include "scheduler/scheduler.h"

/*
	Round Robbin scheduling algorithm

	-the way this currently functions is that each cycle runs through the n
	processes to determine their states "blocked/running...etc" when it reaches 
	back to index 0 then a cycle has been completed. The scheduler determines whose
	next by keeping a list of how long each process has been ready the first process 
	encountered with the highest amount of ready waits is the next to run 
		see ready() in scheduler to get a clearer idea

	-only difference between this set up and FCFS is that it prempts so it will
	 stop running store how far its gone and resume when it gets its chance again
	 to understand the main differences check out the bottom of running() in scheduler
	 the section pertaining to q
*/

void RR(const vector<int*>& data, int v){
	int index=0;
	scheduler rr;		//initialize object
	rr.local=data;		
	rr.size=rr.local.size();
	rr.qincr=0;			//round robbin has extra variables that are within scheduler
	rr.q=2;				//this is because the basic ready is q/!q compatible 
	rr.setup();			//setup basic variables
	if(v)
		cout << "This detailed printout gives the state and remaining burst for each process\n" << endl;
	while(!rr.empty(index)){
		rr.list=rr.local[index];	//load next index from list 
		if(v){
			if(!index)				//prints verbose 
				cout << "before cycle: " << rr.cycle;
			rr.print_verbose(index);
		}
		else
			if(rr.state[index]==2)
				rr.local[index][7]++;
		if(rr.list[8]!=0)			//is a processes run total hasnt depleted
			rr.process(index);		//main workload
		index++;
		index%=rr.size;
		if(!index){					//start/end of cycle
			if(v)
				cout << endl;
			rr.ready();				//determine which process runs next
			rr.cycle++;				//new cycle only when the index has looped back to 0
		}
	}
	cout << "the scheduling algorithm used was round robbin\n" << endl;
	rr.print_process();		//print processes summary 
}