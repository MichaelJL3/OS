#include <vector>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "allocators/optimistic.cpp"
#include "allocators/banker.cpp"

using namespace std;

/*=============================================================================================================*\
|  allocator.cpp					 																			|
|  Author: Michael Laucella 																					|
| 																												|
|  primary function of this file is to prepare the file data to be passed to the 								|
|   following allocators:  																						|
|     -Bankers 																									|
|     -Optimistic 																								|
| 																												|
|   the file is read into a vector 																				|
|   and then the data is split into a series of nested vectors  												|
|   a process list -> resource lists -> instruction lines pertaining to the resource and process 				|
|																												|
|   it also parses the beginning line for basic information such as 											|
|     -number of processes 																						|
|     -number of resources 																						|
|     -number of units per resource 																			|
|																												|
|   the data is handled dynamically so there is no default designed cap for number of resources or processes 	|
|																												|
|   if file is not found or one is not provided an assertion will be raised 									|
|																												|
|   for more informtation read below                                                                            | 
\*_____________________________________________________________________________________________________________*/


int tracker;	//[GLOBAL] tracker keeps track of where in input strings you have parsed to

vector<string> readfile(string file);  //reads the file
int* transform(string input);		   //changes the line of the file into an int*
int nextnum(string input);			   //parses the next number within the string 

int main(int argv, char** argc){
	/*
		main instantiates two objects an optimisitc manager and a banker
		it passes those objects the parsed data from the input file

		if no file is provided an assertion will raise
	*/
	string file="";
	if(argv>1)
		file=string(argc[1]);	//file name
	else{
		puts("[ASSERTION RAISED] no argument passed");	//no file provided assert
		assert(0);
	}

	vector<string> input=readfile(file);	//store lines of the file in a vector
	tracker=0;		

	int process=nextnum(input[0]);		//number of processes
	int resource=nextnum(input[0]);		//number of resources
	int* units=new int[resource];		
	
	for(int i=0; i<resource; i++)
		units[i]=nextnum(input[0]);		//store the number of units per resource
	
	input.erase(input.begin());			//delete the line processed above
	
	vector< vector<int*> > partition(process);
	int index=0;
	int size=input.size();
	int *list;
	for(int i=0; i<size; i++){
		tracker=0;
		list=transform(input[i]);	//transforms a string into a size 5 int array containing the running information
		partition[list[1]].push_back(list);		//push that list into the correct process list
	}

	opt optimistic(process, resource, units, partition);   	 //instantiating the optimistic allocator
	safe banker(process, resource, units, partition);        //instantiating the banker allocator
	optimistic.manager();		//run optimistic
	banker.manager();			//run banker

	size=partition.size();
	vector<int*> temp;
	int tsize=0;
	for(int i=0; i<size; i++){
		temp=partition[i];
		tsize=temp.size();
		for(int j=0; j<tsize; j++){
			delete temp[j];
		}
	}
	delete units;

	return 0;
}

int* transform(string input){
	/*
		changes a string into an array of 5 integers 
		0: the task example initiate
		1: the process
		2: the delay
		3: the resource
		4: the amount
	*/
	int* list=new int[5];
	int temp=0;
	if(input.find("request")!=string::npos) //parse for which task is being asked for by searching the string for keywords
		temp=1;
	else if(input.find("release")!=string::npos)
		temp=2;
	else if(input.find("initiate")!=string::npos)
		temp=0;
	else
		temp=3;

	list[0]=temp;
	for(int i=1; i<5; i++)
		list[i]=nextnum(input);		//fill 1-4 with the numbers in the string
	list[1]--;	//subtract one from the process number to use as an indexing method starting from 0
	
	return list;
}

///////////////////////////////////////////FILE READING FUNCTIONS////////////////////////////////////////////

vector<string> readfile(string file){	//reads the file line by line into a vector
	/*
		reads the file throws an assertion if it is not found
		reads line by line and stores into a vector
	*/
	ifstream fd (file.c_str()); 
	string s="";
	vector<string> arr;

	if(fd.is_open()){					//if file is open read it
		while(getline(fd,s))			//while it is not at the end of the file keep fetching lines
			if(!s.empty())
				arr.push_back(s);		//push the line to the end of the vector
		fd.close();
	}
	else{								//if file does not assert 0
		puts("[ASSERTION RAISED] file not found");
		assert(0);
	}

	return arr;
}

int nextnum(string input){	//gets the next number in the string
	/*
		parses string based on current tracker location in the string
	*/
	int size=input.length();
	int ret=0;

	while(!isdigit(input[tracker])&&tracker<=size)  //ignore any non digit characters
		tracker++;
	while(isdigit(input[tracker])&&tracker<=size){  //keep reading until character is non numeric 
		ret*=10;	//multiply by base 10 before adding the next number so that 11 is processed as 1*10+1 rather than 1+1
		ret+=input[tracker]-'0';	//add next digit to the number
		tracker++;
	}
	return ret;	
}