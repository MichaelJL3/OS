#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <new>
#include "link.h"


module *m_p=&mods;

int main(){
	int size=0;
	int run=0;
	int set=0;
	int check=0;
	string err;

	size=reads();  //PARSING AND STORING DATA

	//READING SYMBOL TABLE
	cout << "Symbol Table" << endl;
	for(int i=0; i<symbnum.size(); i++){
		if(i<serr.size())
			err=serr.at(i);
		else 
			err="";
		cout << symb.at(i) << "=" << symbnum.at(i) << err << endl;
	}
	cout << "\nMemory Map" << endl;

	m_p=&mods;
	m_p=m_p->next;
	check=symb.size();

	for(int i=0; i<size; i++){	//RUN THROUGH ALL THE MODULES  
		mem_c=&m_p->mem;
		sect_c=&m_p->sect;
		cout << "+" << set << endl;
		use(check);	//USES
		map(set);	//REST OF MEMORY MAP
		set+=mem_c->getsize();
		m_p=m_p->next;
	}

	cout << endl;
	for(int i=0; i<used.size(); i++){	//SYMBOL NOT USED ERRORS
		if(used.at(i)==0)
			cout << "warning " << symb.at(i) << " is defined but not used" << endl;
	}

	return 0;
}

///////////////////////////////////////MAP PRINT FUNCTION///////////////////////////////////

//PRINTS MAP BUT ALSO PARSES FOR ANY ERRORS
void map(int set){
	int addr;
	int run=mem_c->getsize();
	string err;
	char state;
	for(int i=0; i<run; i++){
		err="";
		addr=mem_c->get_mem(i);
		state=mem_c->get_state(i)[0];
		if(state=='R'){
			if(addr%1000+set<777)
				addr+=set;
			else{ 
				addr=(addr/1000)*1000;
				mem_c->set_err(i, "\terror: module size too large converted to 0");
			}
			mem_c->set_mem(i, addr);
		}
		if(state=='E'&&!mem_c->get_is_used(i)){
			mem_c->set_state(i, "I");
			err="\terror: E not used has been converted to I";
		}
		else
			err=mem_c->get_err(i);
	
		cout << i << ": " << mem_c->get_state(i)+" " << mem_c->get_mem(i) << err << endl;
	}
}

///////////////////////////////////////FUNCTIONS FOR USES///////////////////////////////////

void use(int check){
	string str;	//symbol in use 
	int pos=0;	//position of symbol
	int loc=0;	//location of symbol / later becomes symbol num
	int run=sect_c->getsize();
	for(int i=0; i<run; i++){
		str=sect_c->get_use(i);
		pos=sect_c->get_use_num(i);
		loc=finduse(str, symb);
		if(loc!=check){
			used.at(loc)=1;
			loc=symbnum.at(loc);
			setuse(pos, loc, 1, str);
		}
		else
			setuse(pos, loc, 0, str);
	}
}

//IF IT IS A TRUE USE THAT VIOLATES NO ERRORS SET
void setuse(int n, int set, int check, string str){
	int mem = mem_c->get_mem(n);	//current memory address
	int tmp=0;	//temporary value
	int next=0; //next address
	int size=mem_c->getsize();
	while(1){
		if(mem_c->get_state(n)[0]!='E'){
			mem_c->set_state(n, "E");
			mem_c->set_err(n, "\terror: use not defined as E has been converted to E");
		}
		tmp = mem/1000;
		mem -= (tmp*1000);
		next=0;
		while(mem){
			next*=10;
			next+=mem%10;
			mem/=10;
		}
		mem=tmp*1000;
		if(next<=777)
			mem += set;
		mem_c->set_mem(n, mem);
		if(check==0){
			mem_c->set_mem(n, (mem_c->get_mem(n)/1000)*1000);
			mem_c->set_err(n, "\terror: use "+str+" not defined address converted to 0");
		}
		mem_c->set_is_used(n);
		if(next>=777){
			if(next>777)
				mem_c->set_err(n, "\terror: module size too large converted to 0");
			break;
		}
		if(size>=next)
			mem = mem_c->get_mem(next);
		else{
			mem_c->set_err(n, "\terror: module pointer exceeding module size terminating chain");
			break;
		}
		n=next;
	}
}

//MAKE SURE USE EXISTS AND RETURN LOCATION
int finduse(string s, vector<string> symb){
	int count=0;
	int cmp = symb.size()-1;
	string str=symb.at(count);
	while(str.compare(s)){
        count++;
        if(count>cmp)
        	return cmp+1;
    	str=symb.at(count);
    }
	return count;
}

////////////////////////////////////////////FUNCTIONS FOR READING//////////////////////////////////////

int reads(){
	int size=0;
	int run=0;
	int set=0;
	int num=0;
	int cmp=0;
	char c=' ';
	string str;
	while(cin){
		m_p=m_p->next = new module;
		mem_c=&m_p->mem;
		sym_c=&m_p->sym;
		sect_c=&m_p->sect;

		size++;
		for(int i=0; i<3; i++){
			switch(i){
				case 0: next_sym(c); break;
				case 1: next_use(c); break;
				default: next_map(c); break;
			}
		}	

		run=sym_c->getsize();
		cmp=mem_c->getsize()-1;
		for(int i=0; i<run; i++){
			num=sym_c->get_symb_num(i);
			str=sym_c->get_symb(i);
			if(!exists(str, symb)){
				symb.push_back(str);
				used.push_back(0);
				if(num>cmp){
					symbnum.push_back(set);
					serr.push_back(" error: "+str+" exceeds module size");
				}
				else
					symbnum.push_back(num+set);
			}
			else{
				serr.push_back(" error: multiple definitions of "+str+" first one used");
			}
		}	

		set+=mem_c->getsize();
	}
	return size;
}

//CHECK IF SYMBOL IS ALREADY DEFINED
int exists(string s, vector<string> symb){
	int run = symb.size();
	for(int i=0; i<run; i++){
		if(!symb.at(i).compare(s))
			return 1;
	}
	return 0;
}

//GET THE NEXT WORD
string next_word(char c){
	string s;
	while(isspace(c)&&cin)
		cin.get(c);
	while(!isspace(c)&&cin){
		s+=c;
		cin.get(c);
	}
	return s;
}

//SET THE NEXT SYMBOLS
void next_sym(char c){
	string s;
	int run=atoi(next_word(c).c_str());
	sym_c->setsize(run);

	for(int i=0; i<run; i++){
		s=next_word(c);
		sym_c->set_symb(s);
		s=next_word(c);
		sym_c->set_symb_num(atoi(s.c_str()));
	}
}

//SET THE NEXT USES
void next_use(char c){
	string s;
	int run=atoi(next_word(c).c_str());
	sect_c->setsize(run);

	for(int i=0; i<run; i++){
		s=next_word(c);
		sect_c->set_use(s);
		s=next_word(c);
		sect_c->set_use_num(atoi(s.c_str()));
	}
}

//SET THE NEXT ADDRESSES
void next_map(char c){
	string s;
	int run=atoi(next_word(c).c_str());
	mem_c->setsize(run);

	for(int i=0; i<run; i++){
		s=next_word(c);
		mem_c->set_state(s);
		s=next_word(c);
		mem_c->set_mem(atoi(s.c_str()));
	}
}
