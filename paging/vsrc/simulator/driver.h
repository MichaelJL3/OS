#ifndef DRIVER_H //define guard
#define DRIVER_H

#include <cstdlib>
#include <fstream>
#include <queue>
#include <vector>

#define A 0
#define B 1
#define C 2

#define FAULT 3
#define EVICT 4
#define TOTAL 5

#define MACH 0
#define PAGE 1
#define SIZE 2
#define REFS 3
#define MAXF 4
#define PROC 5

#define MAXINT 2147483647

#define Q 3

using namespace std;

struct Frame{
	int pageheld;
	int process;
	int loaded;
	int last;
};

class Simulate{
	unsigned int size;
	unsigned int page;
	unsigned int process;
	unsigned int machine;
	unsigned int refs;
	unsigned int maxframe;
	int pos;
	double** status;
	vector< vector<int> > framesheld; 
	fstream rd;
	queue<int> p;
	Frame* frames;
public:
	Simulate(unsigned int* global, double** paststatus);
	~Simulate();
	int randomOS();
	void driver(int type);
	void newframe(int maxframe, int curr, int pg, int cycle);
	void evictframe(int type, int pagen, int curr, int cycle);
	void addpage(int currframe, int cycle);
	int fifo();
	int lru();
	int random();
	void remove(int loc, int cycle, int curr, int pagen);
	int nxtref(int i, int w);
	int* findframe(int n, int pg);
};

#endif