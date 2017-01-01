#include <string>
#include "redefine.h"

/* BASIC INSTANTIATION AND SETUP */

redefine::redefine(){
	size=0;
}

int redefine::getsize(){
	return size;
}

void redefine::setsize(int n){
	use_num=new int[n];
	use=new string[n];
}

/* SET & GET USES */

void redefine::set_use(string s){
	use[size]=s;
}

void redefine::set_use_num(int n){
	use_num[size]=n;
	size++;
}

string redefine::get_use(int i){
	return use[i];
}

int redefine::get_use_num(int i){
	return use_num[i];
}

///////////////EOF///////////////