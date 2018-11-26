/*
Assignment 6 part 2
Programmable determanistic Turing machine

Objective: Create a program that will read data from a txt file, and
create an initial state, an action table, a tape, and a pointer

After reading file and creating necessary components, program will
execute turing machine and output results


*/
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

const int MAX_STATES = 10;
const int MAX_SYMBOLS = 3;

/////////////////////////////////////////////////////////
// Action structure
//
// contained in a 2d array (the action table)
// will contain instructions for the turing machine
// to implement
//////////////////////////////////////////////////////////
struct action{
	int newState;
	char newChar;
	char move;
};

bool getData(string&,int&,action actionTable[MAX_STATES][MAX_SYMBOLS],int&);
bool nextComputation(string&,int&,action actionTable[MAX_STATES][MAX_SYMBOLS],int&);
int conversion(char);

/********************************************************************************************************************
********************************************************************************************************************/
int main(){

	//initialise data for turing machine
	struct timespec fixedTime;
	fixedTime.tv_sec = 0;
	fixedTime.tv_nsec = 10000000;
	string tape = "";
	int state;
	action actionTable[MAX_STATES][MAX_SYMBOLS];
	int pointer;

	// get user data with getData function
	if(!getData(tape,state,actionTable,pointer))
		return 1;

	// run nextComputation function until turing machine halts and print results
	bool loopTest;
	bool firstState = true;
	do{
		loopTest = nextComputation(tape,state,actionTable,pointer);
		cout << tape;
		if(firstState)
		{
			cout << " <--- Initial State";
			firstState= false;
		}
		if(loopTest)
			cout << endl;
		nanosleep (&fixedTime, NULL);
	}while(loopTest);
	cout << " <--- End State" << endl << endl;
	

	return 0;
}
/********************************************************************************************************************
********************************************************************************************************************/


////////////////////////////////////////////////////////////////////
// conversion function
//
// purpose: convert character from tape into an int value
//          
// input: char from tape
//
// output: int useful for table lookup
////////////////////////////////////////////////////////////////////
int conversion(char x){
	if(x == '0')
		return 0;
	else if(x == '1')
		return 1;
	else
		return 2;
}


////////////////////////////////////////////////////////////////////
// getData function
//
// purpose: read text file and get all data
//
// input: empty tape, action table, state, and pointer
//
// output: filled tape, action table, state, and pointer
//         boolean true/false depending on file opening
//         or not
////////////////////////////////////////////////////////////////////
bool getData(string& tape,int& state,action actionTable[MAX_STATES][MAX_SYMBOLS],int& pointer){

	// get input file from user and error check
	string theFile;
	cout << "Please enter name of input file: ";
	cin >> theFile;
	ifstream inputs;
	inputs.open(theFile.c_str());
	if(!inputs){
		cout << "Error opening file. Exiting." << endl;
		return false;
	}

	// get initial state
	inputs >> state;

	// throw away rest of line
	string junk;
	getline(inputs,junk);

	// get tape from file
	char temp;
	do{
		inputs.get(temp);

		// if value is good, put it in the tape
		if(temp != ' ' && temp != '/')
			tape += temp;

	}while(temp != '/');

	// throw away rest of line
	getline(inputs,junk);

	//temp variables for action table construction
	string linedata;
	int row;
	int col;

	// get values from action table
	do{
		// get row number for action table
		inputs >> row;
		if(row != -1)
		{
			// get column number for action table
			inputs >> temp;
			col = conversion(temp);

			// save inputs to action table
			inputs >> actionTable[row][col].newState;
			inputs >> actionTable[row][col].newChar;
			inputs >> actionTable[row][col].move;
		}
		// throw away rest of line
		getline(inputs,junk);
	}while(row != -1);

	// get pointer value from file
	inputs >> pointer;
	pointer--; // adjust pointer for array indexing

	return true;
}


////////////////////////////////////////////////////////////////////
// nextComputation function
//
// purpose: perform table lookup and modify tape, state,
//          and pointer depending on table results
//
// input:  tape, action table, state, and pointer (in current state)
//
// output: ftape, action table, state, and pointer ( in next state)
//         boolean true/false depending if program
//         must continue or halt
////////////////////////////////////////////////////////////////////
bool nextComputation(string& tape,int& state,action actionTable[MAX_STATES][MAX_SYMBOLS],int& pointer){

	// initialise row and column for table lookup
	int row = state;
	int col = conversion(tape[pointer]);
	
	// perform lookup
	action temp = actionTable[row][col];

	// assign new values
	state = temp.newState;
	tape[pointer] = temp.newChar;

	// change pointer value
	if(temp.move == 'L')
		pointer--;
	else if(temp.move == 'R')
		pointer++;
	else if(temp.move == 'H')
		return false;

	
	return true;
}