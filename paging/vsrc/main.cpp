#include "main.h"

void output(int process){
	int evict=0;
	int faultsum=0;
	int count=0;
	short tot=0;
	double avg=0.0;
	double avgsum=0.0;
	puts("");
	for(int i=0; i<process; i++){
		faultsum+=status[i][FAULT];
		printf("process %d had %d faults", i+1, (int)status[i][FAULT]);
		evict=status[i][EVICT];
		if(evict){
			tot=1;
			count+=evict;
			avg=status[i][TOTAL]/evict;
			avgsum+=status[i][TOTAL];
			printf(" and %.1f average residency.\n", avg);
		}
		else
			puts(". With no evictions, the average residence is undefined.");
	}
	printf("\nThe total number of faults is %d", faultsum); 
	if(tot)
		printf(" and the overall average residency is %.1f.\n", avgsum/count);
	else
		puts(". With no evictions, the overall average residence is undefined.");
}

int algorithm(string s){
	int length=s.length();
	for(int i=0; i<length; i++)
		s[i]=tolower(s[i]);
	if(s.find("lru")!=string::npos)
		return 0;
	else if(s.find("fifo")!=string::npos)
		return 1;
	else
		return 2;
}

double** setup(int n){
	double** ret=new double*[n];
  	
  	for (int i=0; i<n; i++){
  		ret[i]=new double[6];
    	for (int k=0; k<6; k++)	
    		ret[i][k]=0;
  	}
	return ret;
}

int preprocess(int job){
	int p=0;
	switch(job){
		case 1: 
			p=1; 
			status=setup(p); 
			status[0][A]=1;
			break;
		case 2: 
			p=4; 
			status=setup(p);
			for(int i=0; i<p; i++)
				status[i][A]=1;
			break;
		case 3: 
			p=4; 
			status=setup(p); 
			break;
		default: 
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
	return p;
}

int main(int argv, char** argc){
	if(argv<7){		//less than 6 args provided assertion raised
		puts("ERROR: not enough arguments given\n\t example: paging.exe 10 10 20 1 10 10 lru");
		exit(0);
	}

	unsigned int job;
	unsigned int type;
	unsigned int process;
	unsigned int args[6];
	args[MACH]=atoi(argc[1]);		//machine size in words
	args[PAGE]=atoi(argc[2]);		//page size in words
	args[SIZE]=atoi(argc[3]);		//process size: virtual address references
	job=atoi(argc[4]);   			//the job mix
	args[REFS]=atoi(argc[5]);		//the number of references for each process
	string replacement=argc[6];		//the replacement algorithm

	type=algorithm(replacement);
	args[MAXF]=args[MACH]/args[PAGE];
	process=preprocess(job);
	args[PROC]=process;

	printf("The machine size is %d.\nThe page size is %d.\n"
		   "The process size is %d.\nThe job mix number is %d.\n"
		   "The number of references per process is %d.\n"
		   "The replacement algorithm is %s.\n\n", 
		   args[MACH], args[PAGE], args[SIZE], job, args[REFS], replacement.c_str());
	
	Simulate paging(args, status);
	paging.driver(type);
	output(process);
        
    for(int i=0; i<process; i++)
    	delete status[i];

	return 0;
}