#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>  // added for file output

using namespace std;

// function prototypes
void arrayanalyze(long, int, ofstream&);


// main function to call timing function 5 times with proper n and reps
int main()
{
	ofstream outfile;

	outfile.open("algorithm2.csv");

	//error checking the output file
	if(!outfile)
	{
		cout<<"unable to open the .csv file\n";
	}



	arrayanalyze(16,10,outfile);
	arrayanalyze(64,10,outfile);
	arrayanalyze(256,10,outfile);
	arrayanalyze(1024,10,outfile);
	arrayanalyze(4096,10,outfile);



	return 0;
}





// This program determines the run time of an algorithm whose life
// is very short. A measurement technique is used that determines
// the number of times the algorithm can run in one second. The
// elapsed time for one run of the algorithm can then be derived
// from this value.

void arrayanalyze(long n, int reps, ofstream& outfile)
{

	unsigned int i;
	int count;
	vector <long> iterations;
	time_t start;
	time_t finish;
	double timeWithAlgorithm;
	double timeWithoutAlgorithm;

	cout << "Algorithm Timing Started!" << endl;
	//initializing  the arrays used the the algorithm
	int* x = new int[n];
	
	for(int i=0;i<n;i++)
	{
		x[i] = 1;
	}

	int* arrayA = new int[n];
	int s = 0; 
	//initializing ends


	// Phase 1.

	iterations.reserve (reps);

	// Determine the number of iterations of the following while loop that
	// can be executed in one second (it includes the algorithm).

	while (iterations.size () < reps)
	{
		count = 0;
		start = time (0);
		do
		{
			++count;
			
			// Algorithm that you are timing goes here for this phase
			
			for( i=0; i<n-i;i++)
			{
				s=s+x[i];
				arrayA[i]=s/(i+1);
			}

			//
			finish = time (0);
			// Continue as long as time hasn't changed
		} while (finish == start);
		// Record the number of iterations
		iterations.push_back (count);
	}

	cout << "Iteration counts (with algorithm): " << endl;
	for (i = 0; i < iterations.size (); ++i) 
	{
		cout << iterations [i] << " ";
	}
	cout << endl;

	cout << "Sorted iteration counts (with algorithm): " << endl;
	sort (iterations.begin (), iterations.end ());
	for (i = 0; i < iterations.size (); ++i) 
	{
		cout << iterations [i] << " ";
	}
	cout << endl;

	cout << "Selected value (with algorithm): " << iterations [reps / 2] << endl;
	timeWithAlgorithm = 1000.0 / iterations [reps / 2];
	cout << "Elapsed time (with algorithm): " << timeWithAlgorithm << " ms" << endl;

	iterations.clear ();

	// Phase 2.

	iterations.reserve (reps);

	// Now determine the number of iterations of the following while loop that
	// can be executed in one second (it does not include the algorithm).

	while (iterations.size () < reps)
	{
		count = 0;
		start = time (0);
		do
		{
			++count;
			// Algorithm is removed for this phase

			//
			finish = time (0);
			// Continue as long as time hasn't changed
		} while (finish == start);
		// Record the number of iterations
		iterations.push_back (count);
	}

	cout << "Iteration counts (without algorithm): " << endl;
	for (i = 0; i < iterations.size (); ++i) 
	{
		cout << iterations [i] << " ";
	}
	cout << endl;

	cout << "Sorted iteration counts (without algorithm): " << endl;
	sort (iterations.begin (), iterations.end ());
	for (i = 0; i < iterations.size (); ++i) 
	{
		cout << iterations [i] << " ";
	}
	cout << endl;

	cout << "Selected value (without algorithm): " << iterations [reps / 2] << endl;
	timeWithoutAlgorithm = 1000.0 / iterations [reps / 2];
	cout << "Elapsed time (without algorithm): " << timeWithoutAlgorithm << " ms" << endl;

	// Phase 3.

	cout << "Algorithm elapsed time: " << timeWithAlgorithm - timeWithoutAlgorithm << " ms" << endl;
	outfile << fixed << timeWithAlgorithm - timeWithoutAlgorithm;
	if(n != 4096)
	outfile << ",";
	cout << "Algorithm Timing Ended!" << endl;


}

