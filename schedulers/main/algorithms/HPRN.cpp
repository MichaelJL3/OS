#include "HPRN.h"

/*
	Highest Penalty Ratio Next scheduling algorithm

	-the way this currently functions is that each cycle runs through the n
	processes to determine their states "blocked/running...etc" when it reaches 
	back to index 0 then a cycle has been completed. The scheduler determines whose
	next by keeping a list of how long a process has been running and in the system
	the first process encountered with the highest penalty is the next to run 
		see hp_ready() in scheduler to get a clearer idea the penalty 
		formula is T/t where T = cycle - delay and t = total number of cpu bursts run
*/

void HPRN(const vector<int*>& data, int v){
	int index=0;
	hp.local=data;
	hp.size=hp.local.size();
	hp.setup();		//setup basic variables
	if(v)
		cout << "This detailed printout gives the state and remaining burst for each process\n" << endl;
	while(!hp.empty(index)){
		hp.list=hp.local[index];	//load next index from list
		if(v){
			if(!index)				//prints verbose 
				cout << "before cycle: " << hp.cycle;
			hp.print_verbose(index);
		}
		else
			if(hp.state[index]==2)
				hp.local[index][7]++;
		if(hp.list[8]!=0)			//is a processes run total hasnt depleted
			hp.process(index);		//main workload
		index++;
		index%=hp.size;
		if(!index){					//start/end of cycle
			if(v)
				cout << endl;
			hp_ready();				//determine which process runs next
			hp.cycle++;				//new cycle only when the index has looped back to 0
		}
	}
	cout << "the scheduling algorithm used was highest penalty ratio next\n" << endl;
	hp.print_process();			//print summary
}

void hp_ready(){		//HPRN ready function
	double highest=0;
	double penalty=0;	
	int run_next=0;
	int t=1;
	if(!hp.run){
		for(int i=0; i<hp.size; i++){
			if(hp.ready_list[i]){		//search for ready to run
				if(hp.local[i][9]>1)	//obtain total time run
					t=hp.local[i][9];
				else
					t=1;
				penalty=hp.cycle-hp.local[i][0];	//calculate penalty
				penalty/=t;
				if(penalty>highest){	//fin highest penalty
					run_next=i;
					highest=penalty;
				}
			}
		}
		if(hp.ready_list[run_next]){	//if highest penalty is found
			hp.ready_list[run_next]=0;	//reset ready to 0
			hp.state[run_next]=1;		//set state to run
			hp.run=1;					//set run flag
			if(!hp.local[run_next][4])	//if cpu burst is 0 generate new cpu burst
				hp.local[run_next][4]=hp.randomOS(hp.local[run_next][1]);
		}
	}
}