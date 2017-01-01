#ifndef MEMORY_H
#define MEMORY_H

using namespace std;

//MEMORY SUBCLASS OF MODULE STRUCT
//	-memory = addresses for memory map

class memory{
	int *mem;
	int *used_mem;
	int size;
	string *mem_state;
	string *err;
public:
	memory();
	int getsize();

	int get_mem(int i);
	void setsize(int n);
	void set_mem(int i, int n);
	void set_mem(int n);

	string get_state(int i);
	void set_state(int i, string s);
	void set_state(string s);

	string get_err(int i);
	void set_err(int i, string s);

	int get_is_used(int n);
	void set_is_used(int n);
};

#endif