#ifndef LINK_H
#define LINK_H

#include "module/memory.h"
#include "module/symbol.h"
#include "module/redefine.h"

//MODULE STRUCT ACTS AS SINGLE OBJECT
struct node{
	symbol sym;
	memory mem;
	redefine sect;
	struct node* next;
}typedef module;

//USED FOR READING
int reads();

//FOR ANALYZING DATA
void use(int check);
void map(int set);

//USED FOR PARSING THE DATA
string next_word(char c);

//USED FOR STORING THE DATA
void next_sym(char c);
void next_use(char c);
void next_map(char c);
void setuse(int n, int set, int check, string str);

//CHECKING AND GATHERING DATA
int exists(string s, vector<string> symb);
int finduse(string s, vector<string> symb);

//GLOBAL VARIABLES
vector<int> symbnum;
vector<string> symb;
vector<string> serr;
vector<int> used;

module mods;
memory *mem_c;
symbol *sym_c;
redefine *sect_c;

#endif