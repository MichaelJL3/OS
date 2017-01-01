#include "LCFS.h"

/*
	Last Come First Serve scheduling algorithm

	-the way this currently functions is that each cycle runs through the n
	processes to determine their states "blocked/running...etc" when it reaches 
	back to index 0 then a cycle has been completed. The scheduler determines whose
	next by keeping a list of how long each process has been ready the first process 
	encountered with the lowest amount of ready waits is the next to run 
		see lc_ready() below to get a clearer idea
*/

void LCFS(const vector<int*>& data, int v){
	int index=0;
	lc.local=data;
	lc.size=lc.local.size();
	lc.setup();		//setup basic variables
	if(v)
		cout << "This detailed printout gives the state and remaining burst for each process\n" << endl;
	while(!lc.empty(index)){
		lc.list=lc.local[index];	//load next index from list
		if(v){
			if(!index)				//prints verbose 
				cout << "before cycle: " << lc.cycle;
			lc.print_verbose(index);
		}
		else
			if(lc.state[index]==2)
				lc.local[index][7]++;
		if(lc.list[8]!=0)			//is a processes run total hasnt depleted
			lc.process(index);		//main workload
		index++;
		index%=lc.size;
		if(!index){					//start/end of cycle
			if(v)
				cout << endl;
			lc_ready();				//determine which process runs next
			lc.cycle++;				//new cycle only when the index has looped back to 0
		}
	}
	cout << "the scheduling algorithm used was last come first serve\n" << endl;
	lc.print_process();		//print summary
}

void lc_ready(){
	int lowest=1000000000;		//set abnormally high value for lowest
	int run_next=0;				
	if(!lc.run){
		for(int i=0; i<lc.size; i++){ 	//look through the list
			if(lc.ready_list[i]<lowest&&lc.ready_list[i]){	//if it is a lower number and is not 0
				run_next=i;
				lowest=lc.ready_list[i];
			}
		}
		if(lc.ready_list[run_next]){	//if the list actually found something
			lc.ready_list[run_next]=0;	//reset its ready
			lc.state[run_next]=1;		//change state to run
			lc.run=1;					//set run flag
			if(!lc.local[run_next][4])	//if cpu boost is 0 grab new one
				lc.local[run_next][4]=lc.randomOS(lc.local[run_next][1]);
		}
	}
}