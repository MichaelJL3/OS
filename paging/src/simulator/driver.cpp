#include "driver.h"

/*=============================================================================================================*\
|  driver.cpp					 																				|
|  Author: Michael Laucella 																					|
| 																												|
|  primary function of this file is to simulate demand paging based on a set of given conditions 				|
|																												|
|   simulates demand paging by keeping track of all the frames a process holds 									|
|	and by keeping track of which page and the process that is held within the frames 							|
|	making it easy to search for a frame held by a process because it only searches through 					|
|	the number of frames held by that process rather then all the frames 										|
| 																												|
|	it uses three eviction algorithms the type is determined through cmd argument in main.cpp 					|
|		-LRU: evicts the least recently used page reference 													|
|		-FIFO: evicts the earliest referenced frame in the list 												|
|		-RANDOM: picks a random number and mods to find a location frame to swap 								|
|																												|
|   if rand file is not program will exit									  									|
|																												|
|   for more informtation read below for details on MACROS and quick function descriptions view driver.h        | 
\*_____________________________________________________________________________________________________________*/


///////////////////////////////////////////CONSTRUCTORS/DESTRUCTORS////////////////////////////////////////////////

Simulate::Simulate(unsigned int* global, double** paststatus){	//constructor
	/*
		setup basic class variables with the arguments passed in
	*/
	rd.open("rand");	//open rand file
	if(!rd.is_open()){	//if it doesn't open exit program after displaying appropriate warning 
		puts("Random Number File Not Found");
		exit(0);
	}

	size=global[SIZE];		//process size
	page=global[PAGE];		//page size
	process=global[PROC];	//number of processes
	machine=global[MACH];	//machine size 
	maxframe=global[MAXF];	//frame size
	refs=global[REFS];		//number of refrences per process

	status=paststatus;		//the 2d array setup in main

	frames=new Frame[maxframe];	 //allocate the frame number of frame objects
	for(int i=0; i<process; i++) //push every process onto a queue
		p.push(i);

	vector< vector<int> > temp(process); //setup a temporary vector of size process
	framesheld=temp;        //framesheld by process is now pointing to the newly allocated temporary vector
}

Simulate::~Simulate(){	//destructor delete allocated data
	delete frames;
}

////////////////////////////////////////////////FRAME FUNCTIONS////////////////////////////////////////////////////

void Simulate::newframe(int framepos, int curr, int pg, int cycle){
	//add a page to a new frame
	Frame fr = frames[framepos];	//load the frame
	fr.pageheld=pg;					//set the page it holds
	fr.process=curr;				//set the process holding it
	fr.last=cycle;					//set the new reference times
	fr.loaded=cycle;
	frames[framepos]=fr;			//save the changes
	framesheld[curr].push_back(framepos);	//push the frame onto the list of frames held by the process
}

void Simulate::evictframe(int type, int pagen, int curr, int cycle){
	//evict a frame based on the type of algorithm
	int loc=-1;
	switch(type){	//run algorithm return location of frame to evict
		case 0: loc=lru(); break;
		case 1: loc=fifo(); break;
		case 2: loc=random(); break;
	}
	remove(loc, cycle, curr, pagen);	//evict the frame
}

void Simulate::remove(int loc, int cycle, int curr, int pagen){
	//remove a page from a frame
	int remve=0;	
	int* temp;
	Frame fr=frames[loc];	//load the frame to be swaped out
	remve=fr.process;		//the process of the page being removed
	status[remve][TOTAL]+=(cycle-fr.loaded);	//calculate time in
	status[remve][EVICT]++;	//add to the processes eviction amount
	temp=findframe(remve, fr.pageheld); //find the frame in the process list to remove
	framesheld[remve].erase(framesheld[remve].begin()+temp[1]);	//remove the frame from the list
	framesheld[curr].push_back(loc);  //add to the list of the new process taking the frame
	fr.pageheld=pagen;	//set the frame to the new page
	fr.process=curr;	//set the frame to the new process
	fr.last=cycle;		//set the times in
	fr.loaded=cycle;
	frames[loc]=fr;		//save changes 
	delete temp;
}

void Simulate::referenced(int currframe, int cycle){	//add a page
	Frame fr=frames[currframe];	//at the frame update the last time it was accessed
	fr.last=cycle;
	frames[currframe]=fr;
}

int* Simulate::findframe(int n, int pg){	//find the frame with the desired page for a process
	//returns a list one is used for eviction which requires the location in the list
	//the other is the actual location in the frame table
	vector<int> temp=framesheld[n];	//load the list of frames for the process
	int size=temp.size();
	int* ret=new int[2];
	ret[0]=-1;	//in case it is not found
	ret[1]=-1;	
	for(int i=0; i<size; i++)	//loop through the list searching for the frame holding the page
		if(frames[temp[i]].pageheld==pg){
			ret[0]=temp[i];	//location in the frame table
			ret[1]=i;		//location in the list of frames held by the process
			return ret;
		}
	return ret;
}

//////////////////////////////////////////////EVICTION ALGORITHMS//////////////////////////////////////////////////

int Simulate::fifo(){
	//find the process that has been in the system the earliest
	int length=machine/page-1;	//check through all the frames
	int min=MAXINT;	//smallest 
	int loc=-1;
	int check=0;
	Frame fr;
	for(int i=length; i>=0; i--){	//check for the smallest process 
		fr = frames[i];
		check=fr.loaded;
		if(check<min){
			min=check;
			loc=i;	//save the location of the smallest
		}
	}
	return loc;	//return the location
}

int Simulate::lru(){
	//find the last process to enter or be referenced in the system
	int length=machine/page-1;	//check through all the frames
	int min=MAXINT;	//smallest
	int loc=-1;
	int check=0;
	Frame fr;
	for(int i=length; i>=0; i--){	//check for the smallest process 
		fr = frames[i];
		check=fr.last;	
		if(check<min){
			min=check;
			loc=i;	//save the location of the smallest
		}
	}
	return loc;	//return the location
}

int Simulate::random(){
	//generate a random number then mod to determine which frame it points to
	int loc=randomOS()%(machine/page);
	return loc;
}
 
///////////////////////////////////////////BASIC DRIVER REFERENCING////////////////////////////////////////////////

void Simulate::driver(int type){	
	int framenum=maxframe-1;	
	vector<int> list(framenum+1);
	int w[process];				//array of holding the current word for the process
	int refper=process*refs;	//size of the driver run = number of processes * references per process
	int curr=p.front();			//the current process
	int currframe=0;			//the currframe
	int pagen=0;				//the page being referenced
	int end=refper-(refper%Q*process);	//the remainder of the run in which processes will complete before the quantum
	int q=Q;					//the quantum
	int* temp;
	for (int i=0; i<process; i++)	//set up the starting words for each process
		w[i]=(111*(i+1))%size;
	for (int ref=0; ref<refper; ref++){	//main loop of the driver
		if(!(ref%q)&&ref){	//if the process has used all its quantum 
			if(ref==end)	//if it has reached a point where the quantum is larger then whats left of the processes change the quantum
				q=refper%Q;
			p.push(p.front());	//push the current process to the back of the queue
			p.pop();		//remove it from the front of the queue
			curr=p.front();	//take the new process from the front
		}
		pagen=w[curr]/page;	//the pagenumber of the word
		temp=findframe(curr, pagen); //find if the pagenumber is in the list of that processes frames
		currframe=temp[0];
		if(currframe==-1){	//if not
			if(framenum>=0){  //if there are free frames available
				newframe(framenum, curr, pagen, ref+1);	//access new frame
				framenum--;	//decrease avialble number of empty frames
			}
			else	//no frames are avialable evict one
				evictframe(type, pagen, curr, ref+1);
			status[curr][FAULT]++;	//a fault has occured
		}
		else	//if it is add to its last reference
			referenced(currframe, ref+1);
		w[curr]=nxtref(curr, w[curr]);	//determine next word for the process
		delete temp; 
	}
}

int Simulate::randomOS(){	//get the next large random int from the file
	unsigned int x;
	string s;
	getline(rd,s);	//read in the line
	x=atoi(s.c_str());	//convert to int
	return x;	//return 
}

int Simulate::nxtref(int i, int w){	  //the next reference word by modifying the last referenced word 
	double n=randomOS()/(MAXINT+1.0); //determine the probability to check for
	double a=status[i][A];	
	double b=status[i][B];
	double c=status[i][C];
	b+=a;
	c+=b;
	if(n<a)	//if probability is less then A 
		w=(w+1)%size;
	else if(n<b)	//if probability is less then B 
		w=(w-5+size)%size;
	else if(n<c)	//if probability is less then C 
		w=(w+4)%size;
	else	//else run the random word generator
		w=(randomOS())%size;
	return w;	//return the new word 
}