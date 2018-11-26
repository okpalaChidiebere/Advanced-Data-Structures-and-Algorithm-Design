#include"PrintQueue.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Job overloaded < operator function
//
// Purpose: compare filesizes and prioritise lower file sizes
// if priority levels are the same. Used by Printqueue class
// 
// input: 2 Job structures
//
// output: true if left job has lower priority or lower filesize (when priority is tied),
// otherwise false
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool operator < (const Job& a, const Job& b){
	if (a.priority < b.priority)
		return true;
	else if(a.priority == b.priority && a.fileSize < b.fileSize)  // check the file sizes if priority matches
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Job overloaded << operator function
//
// Purpose: Prints Job structure
//
// Input: Job to be printed
//
// output: Prints all data of Job to screen. If dummy job is given (jobNo -1) then print
// a blank field.
/////////////////////////////////////////////////////////////////////////////////////////////////////

ostream& operator << (ostream& ous, const Job& a)
{

		ous<<"Job Number: "<<a.jobNo<<endl<<"File Name: "<<a.filename<<endl<<"File Size: "<<a.fileSize<<endl<<"Priority: "<<a.priority<<endl;

	return ous;
}




/************************************************************************************************************************************
************************************************************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////////
// buildHeap private helper function
//
// Purpose: rebuild heap order property of PrintQueue after item has
// been removed or priority is changed
/////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintQueue::buildHeap(){


	/////Code taken from CS340 algorithms/////////////////////
	int j = elementCount / 2;
	int i = j;
	Job temp;
	int child;
	while(i > 0)
	{
		temp = a[i];
		while(i * 2 <= elementCount)
		{
			child = i * 2;
			if(child != elementCount && a[child + 1] < a[child])
				child ++;
			if(a[child] < temp)
				a[i] = a[child];
			else
				break;
			i = child;
		}
		a[i] = temp;
		j--;    
		i = j;
	}
	/////////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintQueue Constructor function
/////////////////////////////////////////////////////////////////////////////////////////////////////

PrintQueue::PrintQueue(){

	elementCount = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// list function
//
// Purpose: List all jobs in queue in order that they will be printed
//
// Output: Jobs lists in order to the screen
/////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintQueue::list(){
	int oldCount = elementCount;

	// create temp job and job array
	Job tempArr[HEAP_SIZE];
	Job temp;
	cout << "All jobs in order that they will be printed:" << endl;

	// release all jobs from print queue in order, print and move to temp array
	for(int i = 0; i < oldCount; i++)
	{

		temp = release();
		cout << temp << endl;
		tempArr[i] = temp;
	}

	// put all elements back into print queue 
	for(int j = 0; j < oldCount; j++)
	{
		insert(tempArr[j]);
	}

}





/////////////////////////////////////////////////////////////////////////////////////////////////////
// insert function
//
// Purpose: insert new job into print queue
// 
// Input: Job to be inserted
//
// Output: True if sucessful, false if unsuccessful (print queue is full)
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool PrintQueue::insert(Job x) 
{
	// check if queue is full
	if(elementCount < 15)
	{
		/////////// insert algorithm from CS340 webpage////////////
		elementCount++;
		int i = elementCount;
		while ((i > 1) && (x < a[i/2]))
		{
			a[i] = a[i / 2];
			i = i / 2;
		}
		a[i] = x;
		/////////////////////////////////////////////////////////////
		return true; // insert successful
	}else{
		return false; // insert unsuccessful
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// remove function
//
// Purpose: remove specific print job from the queue
//
// Input: job number of the job to be deleted
//
// Output: true if successful, otherwise false (empty queue or job not found)
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool PrintQueue::remove(int target){
	if(elementCount == 0)
		return false;  // empty queue early exit


	// cycle through queue and find the job number
	for(int i = 0;i < elementCount; i++)
	{
		if(a[i+1].jobNo == target) // target found
		{
			a[i+1] = a[elementCount];   // put last value in its place and decrement element count
			elementCount--;
			buildHeap(); //rebuild heap order property
			return true;  // exit function successfully
		}
	}

	return false; // return false if target was not found
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// release function
//
// Purpose: remove highest priority job from print queue
//
// Output: Job number of released Job. If operation fails (empty print queue) then
// returns -1
/////////////////////////////////////////////////////////////////////////////////////////////////////
Job PrintQueue::release(){
	if(elementCount > 0) // continue if Queue is not empty
	{
		/////////////// deleteMin code from CS340 website///////////////////
		Job x = a [1];
		a[1] = a [elementCount];
		elementCount --;
		Job temp = a[1];
		int i = 1;
		int child = 0;
		while((i*2) <= elementCount)
		{
			child = i * 2;
			if(child != elementCount && a[child + 1] < a[child])
				child ++;
			if (a[child] < temp)
				a[i] = a[child];
			else
				break;
			i = child;
		}
		a[i] = temp;
		return x;
		//////////////////////////////////////////////////////////////////////
	}else{
		Job badJob;  // return dummy job if the queue was empty
		badJob.jobNo = -1;
		return badJob;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// changePriority function
//
// Purpose: change priority of a specific job
//
// input: int value of the jobNum, and int value of the new desired priority
//
// output: true if the change was successful, false if not (empty or target not found)
//
// precondition: priority input value is error checked to 1-3 only
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool PrintQueue::changePriority(int target,int value){
	if(elementCount == 0)
		return false;  // empty queue early exit

	// cycle through queue and find the job number
	for(int i = 0;i < elementCount; i++)
	{
		if(a[i+1].jobNo == target) // target found
		{
			a[i+1].priority = value;
			buildHeap(); //rebuild heap order property
			return true;  // exit function successfully
		}
	}

	return false; // return false if target was not found
}