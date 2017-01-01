#include "driver.h"

Simulate::~Simulate(){
	delete frames;
}

Simulate::Simulate(unsigned int* global, double** paststatus){
	rd.open("rand");
	if(!rd.is_open()){
		puts("Random Number File Not Found");
		exit(0);
	}

	size=global[SIZE];
	page=global[PAGE];
	process=global[PROC];
	machine=global[MACH];
	maxframe=global[MAXF];
	refs=global[REFS];

	status=paststatus;

	frames=new Frame[maxframe];
	for(int i=0; i<process; i++)
		p.push(i);

	vector< vector<int> > temp(process);
	framesheld=temp;
}

void Simulate::addpage(int currframe, int cycle){
	Frame fr = frames[currframe];
	fr.last=cycle;
	frames[currframe]=fr;
}

void Simulate::newframe(int framepos, int curr, int pg, int cycle){
	Frame fr = frames[framepos];
	fr.pageheld=pg;
	fr.process=curr;
	fr.last=cycle;
	fr.loaded=cycle;
	frames[framepos]=fr;
	framesheld[curr].push_back(framepos);
	pos=framepos;
}

void Simulate::remove(int loc, int cycle, int curr, int pagen){
	int remve=0;
	int* temp;
	Frame fr=frames[loc];
	remve=fr.process;
	status[remve][TOTAL]+=(cycle-fr.loaded);
	status[remve][EVICT]++;
	temp=findframe(remve, fr.pageheld);
	framesheld[remve].erase(framesheld[remve].begin()+temp[1]);
	framesheld[curr].push_back(loc);
	fr.pageheld=pagen;
	fr.process=curr;
	fr.last=cycle;
	fr.loaded=cycle;
	frames[loc]=fr;
	pos=loc;
	delete temp;
}

int Simulate::fifo(){
	int length=machine/page-1;
	int min=MAXINT;
	int loc=-1;
	int check=0;
	Frame fr;
	for(int i=length; i>=0; i--){
		fr = frames[i];
		check=fr.loaded;
		if(check<min){
			min=check;
			loc=i;
		}
	}
	return loc;
}

int Simulate::random(){
	int loc=randomOS()%(machine/page);
	return loc;
}

int Simulate::lru(){
	int length=machine/page-1;
	int min=MAXINT;
	int loc=-1;
	int check=0;
	Frame fr;
	for(int i=length; i>=0; i--){
		fr = frames[i];
		check=fr.last;
		if(check<min){
			min=check;
			loc=i;
		}
	}
	return loc;
}

void Simulate::evictframe(int type, int pagen, int curr, int cycle){
	printf(" evict ");
	int loc=-1;
	switch(type){
		case 0: loc=lru(); break;
		case 1: loc=fifo(); break;
		case 2: loc=random(); break;
	}
	remove(loc, cycle, curr, pagen);
}

int* Simulate::findframe(int n, int pg){
	vector<int> temp=framesheld[n];
	int size=temp.size();
	int* ret=new int[2];
	ret[0]=-1;
	ret[1]=-1;
	for(int i=0; i<size; i++)
		if(frames[temp[i]].pageheld==pg){
			ret[0]=temp[i];
			ret[1]=i;
			return ret;
		}
	return ret;
}

void Simulate::driver(int type){	
	int framenum=maxframe-1;
	vector<int> list(framenum+1);
	int w[process];
	int refper=process*refs;
	int curr=p.front();
	int currframe=0;
	int pagen=0;
	int swap=0;
	int end=refper-(refper%Q*process);
	int q=Q;
	int* temp;
	for (int i=0; i<process; i++)
		w[i]=(111*(i+1))%size;
	for (int ref=0; ref<refper; ref++){
		if(!(ref%q)&&ref){
			if(ref==end)
				q=refper%Q;
			swap=p.front();
			p.pop();
			p.push(swap);
			curr=p.front();
		}
		pagen=w[curr]/page;
		temp=findframe(curr, pagen);
		currframe=temp[0];
		pos=currframe;
		delete temp;
		if(currframe==-1){
			if(framenum>=0){
				newframe(framenum, curr, pagen, ref+1);
				printf(" fault ");
				framenum--;
			}
			else
				evictframe(type, pagen, curr, ref+1);
			status[curr][FAULT]++;
		}
		else
			addpage(currframe, ref+1);
		printf("%d reference word %d (page %d) at time %d: in frame %d\n", curr+1, w[curr], pagen, ref+1, pos); 
		w[curr]=nxtref(curr, w[curr]); 
	}
}

int Simulate::randomOS(){
	unsigned int x;
	string s;
	getline(rd,s);
	x=atoi(s.c_str());
	return x;
}

int Simulate::nxtref(int i, int w){
	double n=randomOS()/(MAXINT+1.0);
	double a=status[i][A];
	double b=status[i][B];
	double c=status[i][C];
	b+=a;
	c+=b;
	if(n<a)
		w=(w+1)%size;
	else if(n<b)
		w=(w-5+size)%size;
	else if(n<c)
		w=(w+4)%size;
	else
		w=(randomOS())%size;
	return w;
}