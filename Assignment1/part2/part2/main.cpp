/*
Assignment 1, Part 2
Binary Tree Average Depth problem

purpose: create 8,64,512, 4096 binary trees with 4,32, 256, 2048 nodes respectively, calculating the average depth after
each insert, and calculating a total depth after all trees have been made, and write all data to CSV file.

input: none (values of n are preprogrammed)
output: total averages for each value of n are written to a comma seperated file (trees.csv)


*/


// includes
#include "tree.h"
#include <fstream>

// includes for random number generator
#include <ctime>
#include <cstdlib> 

// function prototypes
void runTest(int n, ofstream& output);

//////////////////////////////main function//////////////////////////////////
int main(){

	srand(time(0));  // seed the random generator

	ofstream outfile;  // open output file
	outfile.open("trees.csv");

	// error check output file
	if(!outfile)
	{
		cerr << "Unable to open file... Exiting.";
		return 1;
	}


	// run tests for n = 4,32,256,2048
	cout << "Running Tests... ";
	runTest(4,outfile);
	runTest(32,outfile);
	runTest(256,outfile);
	runTest(2048,outfile);
	cout << "Tests Complete. ";

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// runTest function
//
// purpose: create 2n binary trees with n nodes, calculating the average depth after
// each insert, and calculating a total depth after all trees have been made, and write data to CSV file.
//
// input: n (int), output file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runTest(int n, ofstream& output){

	float totalAvg = 0.0; // create total averages 

	for(int i = 0; i < n*2; i++) // create 2n trees 
	{
		
		Tree testTree;  // create tree
		float avg = 0.0; // reset avg for this tree
		for(int j = 0; j < n;j++)   // input n random values
		{
			
			int val = rand() % (n+1);
			avg = avg + testTree.insert(val);
			
		}
		avg = avg / n;  // calclate avg for current tree
		totalAvg = totalAvg + avg;  // add to total average
		testTree.destroy_tree(); // clear current tree
	}

	totalAvg = totalAvg / (2 * n); // calculate total average
	output << totalAvg; // output data for current test
	if(n != 2048)
	output << ",";
}

