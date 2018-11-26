#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int NUM_OF_PAGES = 15;

//Node structures used by prims min spanning tree///////////
struct primNode2{
	int vertex;
	int distance;
	primNode2* nextFollower;
};
struct primNode{
	int vertex;
	bool known;
	int distance;
	primNode2* firstFollower;
};
////////////////////////////////////////////////////////////

int FindNextVertex(primNode);
bool PrimsMinimumSpanningTree();


int main(){
	// error check input data with prims algorithm
	if(PrimsMinimumSpanningTree())
	{
		cout << "Graph is complete. No disconnected elements." << endl;
			return 0;
	}else{
		cout << "Error with input data. Disconnected or dangling websites detected" << endl;
		return 1;
	}




}


//////////////////////////////////////////////////////////////////////////////////////////////
// FindNextVertex function
//
// purpose: used by PrimsMinimumSpanningTree function
//          taken from CS340 algorithms page
//////////////////////////////////////////////////////////////////////////////////////////////
int FindNextVertex (primNode a[]){
	int nextVertex = -1;
	int shortestDistance = 50000;
	for (int i = 1; i <= NUM_OF_PAGES; i++)
	{
		if(a [i].distance < shortestDistance && a [i].known == false)
		{
			nextVertex = i;
			shortestDistance = a [i].distance;
		}
	}
	return nextVertex;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// PrimsMinimumSpanningTree function
//
// purpose: to create a minimum spanning tree using website input data
//          and confirm that there is no disconnected websites (nodes)
//
// output: false if there are disconnected elements, true if not
//////////////////////////////////////////////////////////////////////////////////////////////
bool PrimsMinimumSpanningTree(){
	int startVertex = 1;

	// open input file and error check
	ifstream inputs;
	inputs.open("websites.txt");
	if(!inputs)
	{
		cout << "Error opening input file websites.txt";
		return false;
	}

	/////////////code taken from CS340 algorithms//////////////////////
	primNode a[NUM_OF_PAGES + 1];
	for (int i = 1; i <= NUM_OF_PAGES;i++)
	{
		a [i].vertex = 0;
		a [i].known = false;
		a [i].distance = 50000;
		a [i].firstFollower = NULL;
	}

	primNode2* p;
	int x,y;
	inputs >> x;

	while (inputs)
	{
		bool cont = true;
		inputs >> y;
		p = a [x].firstFollower;
		while (p != NULL)
		{
			if (p -> vertex == y)
				cont = false;
			p = p -> nextFollower;
		}
		if (p == NULL && cont)
		{
			p = new primNode2;
			p -> vertex = y;
			p -> distance = 1;
			p -> nextFollower = a [x].firstFollower;
			a [x].firstFollower = p;
			p = new primNode2;
			p -> vertex = x;
			p -> distance = 1;
			p -> nextFollower = a [y].firstFollower;
			a [y].firstFollower = p;
		}
		inputs >> x;
	}

	a [startVertex].vertex = startVertex;
	a [startVertex].distance = 0;
	int i = startVertex;

	bool cont = true;
	while (cont)
	{
		a [i].known = true;
		p = a [i].firstFollower;
		while (p != NULL)
		{
			if (a [p -> vertex].known == false)
			{
				if(p -> distance < a [p -> vertex].distance)
				{
					a [p -> vertex].distance = p -> distance;
					a [p -> vertex].vertex = i;
				}
			}
			p = p -> nextFollower;
		}
		i = FindNextVertex (a);
		if (i == -1)
			cont = false;
	}

	/////////////////////////////////////////////////////////////////////


	// additional code to check integrety of graph. If disconnected
	// element is found, function will return false

	for (int i = 1; i <= NUM_OF_PAGES;i++)
	{
		if(a[i].distance == 50000)
		{
			return false;
		}
	}

	return true;
}