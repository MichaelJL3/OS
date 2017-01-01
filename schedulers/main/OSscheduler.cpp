#include "OSscheduler.h"

/*=============================================================================*\
|AUTHOR Michael Laucella														|
|NYU NETID ml4842																|
|																				|
|-scheduler algorithms 															|
|	runs FCFS 																	|
|	runs RR 																	|
|	runs LCFS																	|
|	runs HPRN 																	|
|																				|
|FOR MORE INFORMATION READ THE README FILE AND THE SEPARATE SCHEDULER CLASSES   |
\*=============================================================================*/


int main(int argc, char** argv){
	//MAIN FUNCTION WILL TAKE CMD ARGS AND RUN AND DISPLAY ALL 4 SCHEDULING ALGORITHMS
	//will raise an assert 0 if it cannot locate file
	string file="";
	string verbose="";
	int verbose_flag=0;

	if(argc>1)
		verbose=string(argv[1]);
	if(argc>2)
		file+=string(argv[2]);
	if(verbose.compare("--verbose")==0)
		verbose_flag=1;
	else
		file+=verbose;

	string line=read_in(file);
	int size=interpret(line);
	//scheduler sched(verbose_flag);
	//process all the data and initialize scheduler class

	cout << "The original input was: " << size << " ";		//original only printed once for obvious reason "its now sorted" 
	print_vec();
	//priority(line);
	sort(size);						//sort the list
	cout << "The (sorted) input is: " << size << " ";		//every schedule prints the sorted data  
	print_vec();
	cout << endl;
	FCFS(data, verbose_flag);				//FCFS

	cout << "The (sorted) input is: " << size << " ";
	print_vec();
	cout << endl;
	RR(data, verbose_flag);					//RR

	cout << "The (sorted) input is: " << size << " ";
	print_vec();
	cout << endl;
	LCFS(data, verbose_flag);				//LCFS

	cout << "The (sorted) input is: " << size << " ";
	print_vec();
	cout << endl;
	HPRN(data, verbose_flag);				//HPRN

	return 0;
}

void print_vec(){		//--prints vector 
	for(int i=0; i<data.size(); i++){
		cout <<"(";
    	for(int j=0; j<4; j++){
    		if(j)
    			cout << ",";
    		cout << data[i][j];
    	}
    	cout <<")";
	}
	cout << endl;
}

//######################################[ANALYZE DATA]######################################

int interpret(string input){		//enter in set data into vector
	int size=nextnumber(input);
	int* list;

	for(int i=0; i<size; i++){
		list=new int[10];	//create new int*
		list[0]=nextnumber(input);	//A
		list[1]=nextnumber(input);	//B
		list[2]=nextnumber(input);	//C
		list[3]=nextnumber(input);	//D
		list[4]=0; //cpu assignment - holds the random generated cpu value
		list[5]=0; //finish time - holds the termination point of a program
		list[6]=0; //io time - adds up the amount of io 
		list[7]=0; //wait time - adds up all waiting periods
		list[8]=list[2]; //store run to reset changes at end - restores B as it decrements on run
		list[9]=0; //holds run total for HPRN calculations
		data.push_back(list);
	}

	return size;
}

void sort(int size){		//sort the data in the vector based on delay 
	int smallest=1000000;
	int pos=0;
	int *temp;
	if(size){
		for(int i=0; i<size; i++){
			pos=i;
			for(int j=i; j<size; j++){
				if(data[j][0]<smallest){
					smallest=data[j][0];
					pos=j;
				}
			}
			temp=data[pos];
			data.erase(data.begin()+pos);
			data.insert(data.begin()+i, temp);
		}
	}
}

//##########################################[GATHER DATA]######################################

string read_in(string file){		//--open file
	string s;
	ifstream fd (file.c_str()); 
	
	if(fd.is_open()){
		getline(fd,s);
		fd.close();
	}
	else{
		cout << "ERROR: FILE NOT FOUND" << endl;
		assert(0);
	}
	return s;
}

int nextnumber(string input){		//--grabs the next number in the set
	int size=input.length();
	int ret=0;

	while(!isdigit(input[trck])){
		if(trck>size) break;
		trck++;
	}
	while(isdigit(input[trck])){
		if(trck>size) break;
		ret*=10;
		ret+=input[trck]-'0';
		trck++;
	}
	return ret;
}

//#############################################[EXTRA]#####################################

//WASNT SURE IF THE PRIORITY CAME FROM DELAY OR AS PARSED STRING I THOUGHT IT MADE MORE
//SENSE AS THE DELAY SO IN CASE IT IS THE STRING THIS FUNCTION WILL DO THAT TO AN EXTENT

/*void priority(string input){
	int* temp;
	int loc=0;
	int find=input.find("priority");
	if(find!=string::npos){
		find=input.find("last");
		if(find!=string::npos)
			loc=data.size()-1;
		find=input.find("second");
		if(find!=string::npos)
			loc=1;
		find=input.find("third");
		if(find!=string::npos)
			loc=2;
		find=input.find("fourth");
		if(find!=string::npos)
			loc=3;
	}
	if(loc){
		temp=data[loc];
		data.erase(data.begin()+loc);
		data.insert(data.begin(), temp);
	}
}*/