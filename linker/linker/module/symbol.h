#ifndef SYMBOL_H
#define SYMBOL_H

using namespace std;

//SYMBOL CLASS SUB MEMBER OF MODULE STRUCT
//	-symbol = symbol table
class symbol{
	string *symb;
	int *symb_num;
	int size;
public:
	symbol();
	int getsize();

	void setsize(int n);
	void set_symb(string s);
	void set_symb_num(int n);

	int get_symb_num(int i);
	string get_symb(int i);
};

#endif