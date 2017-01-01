#ifndef REDEFINE_H
#define REDEFINE_H

using namespace std;

//REDEFINE CLASS SUB MEMBER OF MODULE STRUCT
//	-redefine = uses for memory map
class redefine{
	string *use;
	int *use_num;
	int size;
public:
	redefine();
	int getsize();

	void setsize(int n);
	void set_use(string s);
	void set_use_num(int n);

	int get_use_num(int i);
	string get_use(int i);
};

#endif