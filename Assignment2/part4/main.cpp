/*
PrintQueue driver program

Purpose: implement and test the Print Queue class defined in PrintQueue.h

*/

#include "PrintQueue.h"

int main(){

	// create new print queue
	PrintQueue theQueue;
	cout << "Print Queue created. Enter command:" << endl;
	int command;
	int jobs = 1;
	// repeat commands until user exits
	do{
		cout << "1 - List all jobs" << endl;
		cout << "2 - Add a new job" << endl;
		cout << "3 - Delete a job" << endl;
		cout << "4 - Release next job" << endl;
		cout << "5 - Change priority of a job" << endl;
		cout << "6 - Exit" << endl;

		cin >> command; // get user command


		// if-else block to perform desired commands
		if(command == 1)
		{
			theQueue.list();
		}
		else if(command == 2)
		{
			// create temp job and set job number (incremented)
			Job temp;
			int priority;

			// prompt user for remaining data
			cout << "Enter job name: ";
			cin >> temp.filename;
			cout << "Enter file size (kb): ";
			cin >> temp.fileSize;
			cout << "Enter priority (1-3): ";
			cin >> priority;

			if(priority < 4 && priority > 0)
			{  // error checking priority value
				temp.priority = priority;
				temp.jobNo = jobs;
				jobs++;
				// insert the job and display results
				if(theQueue.insert(temp))
					cout << "Job " << temp.jobNo << " added successfully" << endl;
				else
					cout << "Unable to add job. Queue is full"<< endl;
			}else{
				cout << "Bad priority value." << endl;
			}
		}
		else if(command == 3)
		{
			// prompt user for job number
			int temp;
			cout << "Enter job number to be deleted: ";
			cin >> temp;

			// attempt removal and display the results
			if(theQueue.remove(temp))
				cout << "Job " << temp << " successfully removed" << endl;
			else
				cout << "Failed to locate job" << endl;

		}
		else if(command == 4)
		{
			// release next job into temp job structure
			Job temp = theQueue.release();

			//if no job was release (-1 job number) print error message
			if(temp.jobNo == -1)
				cout << "Print Queue is empty." << endl;
			else
			{
				cout << "This job has been released to printer:" << endl;
				cout << temp;
			}

		}
		else if(command == 5)
		{
			// make temp values and get user data
			int temp,priority;
			cout << "Enter job number: ";
			cin >> temp;
			cout << "Enter new priority: ";
			cin >> priority;
			if(priority < 4 && priority > 0)
			{  // error checking priority value

				// attempt perform change priorty and display results
				if(theQueue.changePriority(temp,priority))
					cout << "Job " << temp << " successfully changed to priority " << priority << endl;
				else
					cout << "Failed to locate job"<< endl;
			}
			else
			{
				cout << "Bad priority value." << endl;
			}
		}


		cout << endl;
	}while(command != 6); // exit from loop when user exits

	return 0;
}