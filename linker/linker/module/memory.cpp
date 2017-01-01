#include <string>
#include "memory.h"

/* INSTANTIATION AND BASIC SETUP */

memory::memory(){
	size=0;
}

int memory::getsize(){
	return size;
}

void memory::setsize(int n){
	mem=new int[n];
	used_mem=new int[n];
	mem_state=new string[n];
	err=new string[n];
}

/* SET & GET MEMORY ADDRESSES */
void memory::set_mem(int i, int n){
	mem[i]=n;
}

void memory::set_mem(int n){
	mem[size]=n;
	size++;
}

int memory::get_mem(int i){
	return mem[i];
}

/* SET & GET MEMORY STATE */

void memory::set_state(int i, string s){
	mem_state[i]=s;
}

void memory::set_state(string s){
	mem_state[size]=s;
}

string memory::get_state(int i){
	return mem_state[i];
}

/* SET & GET HAS BEEN USED */

int memory::get_is_used(int i){
	if(used_mem[i]==1)
		return 1;
	else 
		return 0;
}

void memory::set_is_used(int i){
	used_mem[i]=1;
}

/* ERRORS */
string memory::get_err(int i){
	return err[i];
}

void memory::set_err(int i, string s){
	err[i]=s;
}

///////////////EOF///////////////