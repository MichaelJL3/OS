#include <string>
#include "symbol.h"

/* BASIC INSTANTIATION AND SETUP */

symbol::symbol(){
	size=0;
}

int symbol::getsize(){
	return size;
}

void symbol::setsize(int n){
	symb_num=new int[n];
	symb=new string[n];
}

/* SET & GET USES */

void symbol::set_symb(string s){
	symb[size]=s;
}

void symbol::set_symb_num(int n){
	symb_num[size]=n;
	size++;
}

string symbol::get_symb(int i){
	return symb[i];
}

int symbol::get_symb_num(int i){
	return symb_num[i];
}


///////////////EOF///////////////