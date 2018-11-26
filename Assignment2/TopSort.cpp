/*
TopSort.cpp

Purpose: create a dynamic linked list topological sorting algorithm  using
the pseudocode provided on the CS340 Algorithms web page,
with the addional ability to print items after the input phase, and to
list items that are concurrent on the same line.

data type: strings

input type: CS_Prerequisites.txt file with string values delimied by spaces and endlines


*/


#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Function and structure prototypes
struct Leader;
struct Follower;
string Read(ifstream&);
Leader* buildList(string,Leader*&,Leader*&,int&);
void topologicalSort(ifstream&);

// Follower node taken from in-class notes/////////////////
struct Follower
{
	Leader* adjecentLeader;
	Follower* nextFollower;
};
///////////////////////////////////////////////////////////


/////Struct node taken from in-class notes/////////////////
struct Leader
{
	string element;
	int inDegree;
	Leader* nextLeader;
	Follower* firstFollower;
};
///////////////////////////////////////////////////////////




/*****************************************************************************************************
*****************************************************************************************************/
int main(){

	// declare input stream and open file
	ifstream myReadFile;
	myReadFile.open("CS_Prerequisites.txt");

	// perform error check on file and use top sort function
	if(myReadFile)
	topologicalSort(myReadFile);
	else
		cout << "Error opening CS_Prerequisites.txt file";

	return 0;
}

/*****************************************************************************************************
*****************************************************************************************************/


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Read function
//
// Purpose: read a string value from input file
//
// input: input stream
//
// Output: string from file (if END_OF_INPUT then end of file was reached)
/////////////////////////////////////////////////////////////////////////////////////////////////////
string Read(ifstream& input){

	string temp;
	input >> temp;

	if(input)
		return temp;
	else
		return "END_OF_INPUT";

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// buildList function
//
// Purpose: used by topologicalSort to build dynamic list
//          taken from CS340 Algorithms page
//
// input: string value, head and tail nodes, element count (int)
//
// Output: Leader node to be assigned to p or q
/////////////////////////////////////////////////////////////////////////////////////////////////////
Leader* buildList(string w,Leader*& head,Leader*& tail,int& i){
	
	Leader* h = head;
	tail -> element = w;
	while (h -> element != w)
		h = h -> nextLeader;
	if (h == tail)
	{
		tail = new Leader;
		i++;
		h -> inDegree = 0;
		h -> firstFollower = NULL;
		h -> nextLeader = tail;
	}
	return h;
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// topologicalSort function
//
// Purpose: used to create a dynamic topological sorted list
//          from input file, and print items in order. Must print concurrent
//          items on the same line
//          taken from CS340 Algorithms page (and edited)
//
// input: input stream
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void topologicalSort(ifstream& input){

	int linearCount = 1;
	string x,y;
	Follower* f;
	Leader* p;
	Leader* q;
	Leader* r;
	Leader* head = new Leader;
	Leader* tail = head;
	int elementCount = 0;
	x = Read(input);
	while (x != "END_OF_INPUT")
	{
		y = Read(input);
		p = buildList(x, head, tail, elementCount);
		q = buildList (y, head, tail, elementCount);
		f = new Follower;
		f -> adjecentLeader = q;
		f -> nextFollower = p -> firstFollower;
		p -> firstFollower = f;
		q -> inDegree ++;
		x = Read(input);
	}
	///////Additional algorithm by student to print after input phase////////////////
	
	Leader* temp1 = head;
	while(temp1 != tail)
	{
		// print leader number and element
		cout << "Leader " << linearCount << endl;
		cout <<  "  element = " << temp1 -> element << endl;
		//print indegree and next leader
		cout << "  inDegree = " << temp1 -> inDegree << endl;
		if(temp1->nextLeader != tail)
		cout << "  nextLeader = " << temp1 -> nextLeader->element << endl;
		else
			cout << "  nextLeader = N/A" << endl;

		Follower* temp = temp1->firstFollower;
		int tempFollow = 1;
		//cycle through all followers of this node
		while(temp != NULL)
		{
			// print off the followers of this node
			cout << "  Follower " << tempFollow << " = " << temp->adjecentLeader->element << endl;
			tempFollow++;
			temp = temp->nextFollower;
		}
		cout << endl;
		// increment temp pointer and Leader count
		temp1 = temp1->nextLeader;
		linearCount++;
	}
	
	//////////////////////////////////////////////////////////////////////////



	p = head;
	head = NULL;
	r = tail;
	int i = 0;

	while (p != r)
	{
		q = p;
		p = p -> nextLeader;
		if(q -> inDegree == 0)
		{
			q -> nextLeader = head;
			head = q;
			i ++;
			if(i == 1)
				tail = q;
		}
	}
	q = head;
	r = tail;

	bool changelines = false;
	
	while(q != NULL)
	{
		
		cout <<  q -> element << " ";

		///// Additional code by student to//////////////////////////////
		///// change lines when a new 'tier' of classes is reached///////
		if(changelines)
			cout << endl;
	    changelines = true;
		/////////////////////////////////////////////////////////////////

		elementCount --;
		f = q -> firstFollower;
		q = q -> nextLeader;


		while(f != NULL)
		{
			p = f -> adjecentLeader;
			p -> inDegree --;
			if(p -> inDegree == 0)
			{
				p -> nextLeader = NULL;
				r -> nextLeader = p;
				r = p;
				if(q == NULL)
					q = r;
			}
			
			f = f -> nextFollower;
			//////////////Additional code by student///////////////////////////
			///////////////capture point for NOT changing lines //////////////
			if(f == NULL)
			{
				// if this code is not reached, then move to new line
				// in printing phase
				changelines = false;
			}
			///////////////////////////////////////////////////////////////////
		}
	}
	if (elementCount != 0)
		cout << "ERROR - SET NOT PARTIALLY ORDERED" << endl;
		
}
	