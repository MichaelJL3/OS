#include "scheduler/scheduler.h"

/*
	First Come First Serve scheduling algorithm

	-the way this currently functions is that each cycle runs through the n
	processes to determine their states "blocked/running...etc" when it reaches 
	back to index 0 then a cycle has been completed. The scheduler determines whose
	next by keeping a list of how long each process has been ready the first process 
	encountered with the highest amount of ready waits is the next to run 
		see ready() in scheduler to get a clearer idea
*/

void FCFS(const vector<int*>& data, int v){		//--FCFS first come first serve
	int index=0;
	scheduler fc;					//initializing cheduler object
	fc.local=data;
	fc.size=fc.local.size();
	fc.setup();						//basic setup
	if(v)
		cout << "This detailed printout gives the state and remaining burst for each process\n" << endl;
	while(!fc.empty(index)){					
		fc.list=fc.local[index];	//grab next process
		if(v){						//verbose printing
			if(!index)
				cout << "before cycle: " << fc.cycle;
			fc.print_verbose(index);
		}
		else
			if(fc.state[index]==2)
				fc.local[index][7]++;
		if(fc.list[8]!=0)			//if a processes run total hasnt depleted
			fc.process(index);		//main workload
		index++;
		index%=fc.size;
		if(!index){					//end/start of cycle
			if(v)
				cout << endl;
			fc.ready();		//determine next ready
			fc.cycle++;		//when index is returnd to 0 through modulus calculations it begins a new cycle
		}
	}
	cout << "the scheduling algorithm used was first come first serve\n" << endl;
	fc.print_process();		//print summary
}