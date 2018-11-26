/*
		CS340 Assignment 6 Part 1
       	Replicated workers algorithm

		purpose: to create workers on simultanious threads to do
		fake work (simulated by random numbers and a delay function)



		Assignment instructions part g indicates:

		"The DoWork function should have some kind of mechanism for determining
		whether a new task should be created. However, the mechanism should also ensure 
		that the DoWork function eventually stops creating new tasks. In this way, the 
		computation will eventually terminate. I suggest you generate a random number for 
		each worker that indicates the maximum number of new tasks it will create."

		Note this suggests but does not necessarily require that the number of new work
		created should be random. This program will stop making tasks after a fixed 
		number (25) of tasks has been created.
*/


#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
using namespace std;


////////////////////////////////////////////////////////
// taskQueue Structure
//
// used to create work pools
// simple queue is tracked by head[workPoolID] and 
// tail[workPoolID] in replicated workers algorithm
////////////////////////////////////////////////////////
struct taskQueue{
	int fifo[3000];
};



//Most of the variables required should be defined as global variables so that they can be shared by all threads
// these variables are taken directly from CS340 inclass notes
const int NO_OF_WORK_POOLS = 3;
const int NO_OF_WORKERS = 5;
int t[NO_OF_WORK_POOLS + 1];
int d[(NO_OF_WORKERS * NO_OF_WORK_POOLS) + 1];
sem_t s[NO_OF_WORK_POOLS+1];
taskQueue w[NO_OF_WORK_POOLS+1];
int emptyWorkPools;
sem_t e;
sem_t printLock;
int head[NO_OF_WORK_POOLS+1];
int tail[NO_OF_WORK_POOLS+1];

int allWorkers = 1; // global worker counter, used to create incrementally tagged workers in 
                    // worker function
int allTasks = 1;  // global task counter, when this hits 25, workers will stop making new tasks

pthread_t pid;

pthread_t *tids;

// function prototypes
void PutWork(int,int,int&);
void Thread(int);
void InsertTask (int,int);
void *Worker(void*);
int GetWork(int,int&);
void DoWork(int,int,int&);
int RemoveTask(int);
void Init (sem_t *sem);
void Lock (sem_t *sem);
void Unlock (sem_t *sem);
void Destroy (sem_t *sem);

//ReplicatedWorkers (main) function /**********************************************************************
/*******************************taken from cs340 algorithms page****************************************/
int main(){

	//initialisation
	srand(time(NULL));
	int startTask = 1;
	Init(&e);
	Init(&printLock);
	for(int i = 1;i <= NO_OF_WORK_POOLS; i++){
		t[i] = 0;
		Init(&s[i]);
		for(int j = ((i - 1) * NO_OF_WORKERS + 1);j <= i * NO_OF_WORKERS; j++)
			d[j] = i % NO_OF_WORK_POOLS + 1;
		head[i] = 0;
		tail[i] = 0;
	}
	d [1] = 1;
	emptyWorkPools = 0;


	//create first task
	pid = pthread_self ();
	cout << "Pid: " << pid << " has started." << endl;
	int temp;
	PutWork (1,startTask,temp);  
	cout << "Pid: " << pid << " has created task 1 in workPoolID 1" << endl;

	//created replicated workers
	tids = new pthread_t[(NO_OF_WORKERS * NO_OF_WORK_POOLS) + 1];
		
	for(int i = 1; i <= (NO_OF_WORKERS * NO_OF_WORK_POOLS); i++)
	{
		
		pthread_create (&tids[i], NULL, &Worker, (void*)i);
	}
	
	for(int i = 1; i <= (NO_OF_WORKERS * NO_OF_WORK_POOLS); i++)
		pthread_join (tids[i], NULL);

    // clear semaphores
	for (int i = 1; i <= NO_OF_WORK_POOLS; i++)
	{
		Destroy(&s[i]); 
	}
	Destroy(&e);
	Destroy(&printLock);

	// end program
	cout << "pid: " << pid << " has terminated." << endl << endl;
	return 0;
}
/********************************************************************************************************
/*******************************************************************************************************/


///////////////////////////////////////////////////////////////////////
// PutWork function
//
// purpose: take specific task from a worker and put it into
//          a workpool
//
// input: workerID number, task number, workPoolVar variable
// 
// output: workPoolVar with the work pool that task was added
//         ( for printing purposes)
///////////////////////////////////////////////////////////////////////
void PutWork(int workerID,int task, int& workPoolVar){
	
	// determine workpool with d array
	int workPoolID = d[workerID];
	workPoolVar = workPoolID;
	
	// lock s and e
	Lock(&s[workPoolID]);
	Lock(&e);

	// update emptyworkpools global variable
	t[workPoolID]++;
	int idleWorkers = t [workPoolID];
	if(idleWorkers == (-(NO_OF_WORKERS) + 1))
		emptyWorkPools --;

	// unlock s and e
	Unlock (&s[workPoolID]);
	Unlock (&e);
	
	// insert task and update d
	InsertTask(workPoolID,task); 
	d [workerID] = workPoolID % NO_OF_WORK_POOLS + 1;
}




///////////////////////////////////////////////////////////////////////
// InsertTask function
//
// purpose: insert task into specific workpool queue
//
// input: workpool ID, task to be inserted
///////////////////////////////////////////////////////////////////////
void InsertTask (int workPoolID,int task){
		

	Lock (&s[workPoolID]);

	// add the task into fifo queue array
	w [workPoolID].fifo[tail[workPoolID]] = task;
	// modify tail variable
	tail[workPoolID] ++;

	
	Unlock (&s[workPoolID]);
}


///////////////////////////////////////////////////////////////////////
// Worker function
//
// purpose: to create a worker that is made on a new thread.
//          worker will collect tasks and do work as needed
//
// input: workerID
///////////////////////////////////////////////////////////////////////
void *Worker(void*){
	
	int workerID = allWorkers;
	allWorkers++;

	// grab first task
	int workPoolVar;
	int task = GetWork (workerID,workPoolVar);

	// continue to get tasks and do work
	while(task != -1)
	{
		DoWork (workerID, task,workPoolVar);
		task = GetWork (workerID,workPoolVar);
	}

	Lock(&printLock);
	cout << "workerID: " << workerID << " has terminated." << endl;
	Unlock(&printLock);
}


///////////////////////////////////////////////////////////////////////
// GetWork function
//
// purpose: check workpools for tasks to be done. if workpools are
//          all empty, then start inserting NULL tasks that will terminate
//         the program
//
// input: worker ID number, workPoolVar variable
//
// output: task that was removed, WorkPoolVar indicating which work
//         pool for printing purposes
///////////////////////////////////////////////////////////////////////
int GetWork(int workerID,int& workPoolVar){
		
	int workPoolID = ((workerID - 1) / NO_OF_WORKERS) + 1;
	Lock (&s[workPoolID]);
	Lock (&e);
	t [workPoolID] --;
	int idleWorkers = t [workPoolID];

	// check if work pool is empty
	if(idleWorkers == -(NO_OF_WORKERS))
	{
		emptyWorkPools ++;
		// if all work pools empty, insert null tasks
		if(emptyWorkPools == NO_OF_WORK_POOLS)
		{
			Unlock(&e);
			Unlock (&s[workPoolID]);
			for(int i = 1; i <= NO_OF_WORK_POOLS; i++)
			{
				for(int j = 1; j <= NO_OF_WORKERS; j++)
					InsertTask(i, -1);
			}
		}
		else
		{
			Unlock(&e);
			Unlock(&s[workPoolID]);
		}
	}
	else
	{
		Unlock(&e);
		Unlock(&s[workPoolID]);
	}

	//else get tasks from specific work pool and return it
	int task = RemoveTask (workPoolID);
	workPoolVar = workPoolID;
	return task;
}


///////////////////////////////////////////////////////////////////////
// DoWork function
//
// purpose: do fake work. Continue to add new work until
//          25 tasks have been added
//
// input: worker ID number, specific task number, workPoolVar variable
//
// output: WorkPoolVar indicating which work
//         pool for printing purposes
///////////////////////////////////////////////////////////////////////
void DoWork(int workerID,int task,int& workPoolVar){
	Lock(&printLock);	
	cout << "WorkerID " << workerID << " has started task " << task << " from workPoolID " << workPoolVar << endl;
	Unlock(&printLock);
	allTasks++;
	
	// generate random amount of time to similate work of varying length
	struct timespec randomTime;
	randomTime.tv_sec = rand() % 5;
	randomTime.tv_nsec = rand();
	nanosleep (&randomTime, NULL);
	
	int workPoolVar2;

	// make additional work until 25 total tasks have been added
	
	if(allTasks <= 25){
		PutWork(workerID,allTasks,workPoolVar2);
		Lock(&printLock);	
		cout << "WorkerID " << workerID << " has created task " << allTasks << " in workPoolID " << workPoolVar2 << endl;
		Unlock(&printLock);	
	}
	
	// generate random amount of time to similate work of varying length
	randomTime.tv_sec = rand() % 5;
	randomTime.tv_nsec = rand();
	nanosleep (&randomTime, NULL);
	
    Lock(&printLock);	
	cout << "WorkerID " << workerID << " has finished task " << task << " from workPoolID " << workPoolVar << endl;
	Unlock(&printLock);	
}


///////////////////////////////////////////////////////////////////////
// RemoveTask function
//
// purpose: get a task from a work pool queue. If
//          the queue is empty, keep trying until a task is found
//
// input: workPool ID number
//
// output: task that was removed
///////////////////////////////////////////////////////////////////////
int RemoveTask(int workPoolID){

	// keep trying to get work from workpool until one is found
	Lock (&s[workPoolID]);
	while(true){
		if(tail[workPoolID] == head[workPoolID])           
			Unlock (&s [workPoolID]);
		else
			goto loopbreak;
		Lock (&s [workPoolID]);
	}
loopbreak:true;


	// get task from queue and modify head variable
	int task = w [workPoolID].fifo[head [workPoolID]];// these are reveresed in algorithm?
	head [workPoolID] ++;                             // these are reveresed in algorithm?


	// clarify w [workPoolID][head [workPoolID]] = EMPTY line in algorithm page

	Unlock (&s [workPoolID]);
	return task;
}


///////////////////////// semaphore functions ////////////////////////////////
// taken from semaphoreFunctions.c sample code and modified for c++ cout//////
void Init (sem_t *sem)
{
    if (sem_init (sem, 0, 1) == -1)
    {
        cout << "Failed to initialize semaphore";
        exit (1);
    }
    return;
}

void Lock (sem_t *sem)
{
    while (sem_wait (sem) == -1)
    {
        if(errno != EINTR)
        {
            cout << "Thread failed to lock semaphore\n";
            exit (1);
        }
    }
    return;
}

void Unlock (sem_t *sem)
{
    if (sem_post (sem) == -1)
    {
        cout << "Thread failed to unlock semaphore\n";
        exit (1);
    }
    return;
}

void Destroy (sem_t *sem)
{
    if (sem_destroy (sem) == -1)
    {
        cout << "Failed to destroy semaphore";
        exit (1);
    }
    return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////