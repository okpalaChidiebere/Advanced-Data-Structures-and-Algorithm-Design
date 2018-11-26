#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int NUM_OF_PAGES = 15;


// simplified node structure for directed graph and search//
struct node{
	int val;
	node* follower;
};
////////////////////////////////////////////////////////////



void createDirectedGraph(node*);
void create2dArray(node*,float[NUM_OF_PAGES][NUM_OF_PAGES]);
void save2dArray(float[NUM_OF_PAGES][NUM_OF_PAGES]);

/******************************************************************************************************************************************************************
******************************************************************************************************************************************************************/
int main(){

	
	// create graph array and create directed graph
	node a[NUM_OF_PAGES + 1];
	createDirectedGraph(a);

	// initialize data array and populate
	float theArray[NUM_OF_PAGES][NUM_OF_PAGES];
	for(int i = 0; i < NUM_OF_PAGES;i++)
	{
		for(int j = 0; j < NUM_OF_PAGES;j++)
		{
			theArray[i][j] = 0;
		}
	}
	create2dArray(a,theArray);

	// output data to file
	save2dArray(theArray);


	return 0;
}
/******************************************************************************************************************************************************************
******************************************************************************************************************************************************************/




//////////////////////////////////////////////////////////////////////////////////////////////
// createDirectedGraph function
//
// purpose: create a simplified directed graph based off algorithms on the CS340 website
//          will read data from websites.txt
//
// input: empty directed graph
//
// output: completed directed graph
//
// assumptions: input file has already been error checked by minspanningtree.cpp
//////////////////////////////////////////////////////////////////////////////////////////////
void createDirectedGraph(node* a){

	// open input file
	ifstream inputs;
	inputs.open("websites.txt");

	// initialise array for the directed graph
	for (int i = 1; i <= NUM_OF_PAGES;i++)
	{
		a [i].follower = false;
	}


	node* p;
	int x,y;
	inputs >> x;

	// create all follower nodes and attach to array
	while (inputs)
	{
		bool cont = true;
		inputs >> y;
		p = a [x].follower;
		while (p != NULL)
		{
			if (p -> val == y)   // check for duplicate followers
				cont = false;
			p = p -> follower;
		}
		if (p == NULL && cont)
		{
			p = new node;
			p -> val = y;
			p -> follower = a [x].follower;
			a [x].follower = p;
		}
		inputs >> x;
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////
// create2dArray function
//
// purpose: create a 2d array with website scores, to be used later to calculate
//          page ranks
//
// input: directed graph, empty float 2d array
//
// output: float 2d array with values inserted from graph
//////////////////////////////////////////////////////////////////////////////////////////////
void create2dArray(node* a,float b[NUM_OF_PAGES][NUM_OF_PAGES] ){

	for (int i = 1; i <= NUM_OF_PAGES;i++)
	{
		node* p;
		node* q;
		float count = 0;     // use count to calculate the page weight given

		q = a[i].follower;
		p = a[i].follower;
		while(q != NULL)  // get counts
		{
			count++;
			q = q->follower;
		}
		
		while(p != NULL) 
		{
			b[p->val - 1][i-1] = 1/count;    // set appropriate array values
			p = p->follower;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////
// save2dArray function
//
// purpose: output all 2d array data to a text file 
//
// input: 2d data array with incomplete pageranks
//
// output: matrix.txt
//////////////////////////////////////////////////////////////////////////////////////////////
void save2dArray(float a[NUM_OF_PAGES][NUM_OF_PAGES]){
	ofstream outputs;
	outputs.open("matrix.txt");

		for(int i = 0; i < NUM_OF_PAGES;i++)
	{
		for(int j = 0; j < NUM_OF_PAGES;j++)
		{
			outputs << setw(10) << a[i][j] << " ";
		}
		outputs << endl;

	}


}