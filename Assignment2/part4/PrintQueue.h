/*
PrintQueue Class

Purpose: Create a priority queue (binary heap) that will be used to solve the print
job queue problem in assignment 2 part 4

data type: Struct (Job) Array

features: 

a.	List all the jobs in the order they will be printed.

b.	Add a new job (i.e., an insert).

c.	Delete a job (i.e., requires a linear scan of the array).

d.	Release the next job (i.e., the highest priority job).

e.	Increase the priority of a job (requires a linear scan of the array).

f.	Decrease the priority of a job (requires a linear scan of the array).


*/

#include<iostream>
#include<string>
using namespace std;

const int HEAP_SIZE = 16;


// Job structure to contain data for each
//print job (priority, job number, filename and filesize)
struct Job
{
	int priority;
	int jobNo;
	string filename;
	int fileSize;

};
bool operator < (const Job&,const Job&);
ostream& operator << (ostream&, const Job&);

class PrintQueue
{
private:  
	//private data members
	int elementCount;
	Job a[HEAP_SIZE];
	void buildHeap();

public:
	// constructor
	PrintQueue(); 

	//public functions (features of queue)
	void list();
	bool insert(Job);
	bool remove(int);
	Job release();
	bool changePriority(int,int);

};