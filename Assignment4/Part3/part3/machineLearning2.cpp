/*NaïveBayes algorithm
Overview:
-this program reads in test values from a file and stores them in arrays
-it calculates the probability of each value being a yes or no
- it reads in new data and compares it to the probabilities and selects the most likely one*/
#include<string>
#include<iostream>
#include <fstream>
using namespace std;
//data type for the A array, it holds strings as well as the number of times the strings appear
struct datum{
	string value;
	int count;
};
void NaiveBayesPreprocessor(string[1733][8], datum[1733][8], int[10][8][1733], float[10][8][1733], int&, int&);
void NaiveBayesLearner(string[1733][8], datum[1733][8], int[10][8][1733], float[10][8][1733], int&, int&);
string NaiveBayesClassifier(datum[1733][8], float[10][8][1733], string[8], int);
void BuildTables(string[1733][8], datum[1733][8], string, int, int);
int UniqueValues(datum[1733][8], int);
int XRefD(string[1733][8], datum[1733][8], int, int);
int XRefU(datum[1733][8], string[8], int);
/*Calls the pre processor to load all of the test data into an array then calls the learner to calculate the probability of yes or no for each element in the arrays
it then reads the unlabeled data from a file and uses the classifier function to determine what is likely for the input data*/
int main()
{
	//declare variables
	string D[1733][8];
	datum A[1733][8];
	int C[10][8][1733];
	float P[10][8][1733];
	string U[8];
	int t, n;
	string x;
	string temp, hold, c;
	int pos = 0;
	int i = 1;
	int person = 1;
	//load all test values into arrays
	NaiveBayesPreprocessor(D, A, C, P, t, n);
	//get the statistics from the test values and store them in an array
	NaiveBayesLearner(D, A, C, P, t, n);
	//open the unlabeled file
	ifstream ins;
	ins.open("car-unlabeled.txt");
	//get the first line in the file
	getline(ins, temp);
	//cycle through the line and save the first word into hold
	while (pos < temp.length() && temp[pos] != ',')
	{
		x = temp[pos];
		hold = hold + x;
		pos++;
	}
	//move the pointer to the
	pos++;
	//while not end of file
	while (ins)
	{
		//save hold into the next empty space in U and increment i
		U[i] = hold;
		i++;
		//empty hold
		hold = "";
		//if it'd the end of the line
		if (pos - 1 == temp.length())
		{
			//call the classifier to get the result for the line of data
			c = NaiveBayesClassifier(A, P, U, n);
			//get the next line from the file
			getline(ins, temp);
			// re-initialize the variables that reference adresses in the arrays
			pos = 0;
			i = 1;
			//output the results to the screen
			cout << "Person " << person << ": ";
			cout << c << endl;
			person++;
		}
		//cycle throught the next line and break off words to be stored into the U array
		while (pos < temp.length() && temp[pos] != ',')
		{
			x = temp[pos];
			hold = hold + x;
			pos++;
		}
		pos++;

	}

	//end program
	return 0;
}
/*It initializes all arrays to 0 or empty then it reads in test data and stores it in the A and the D arrays*/
void NaiveBayesPreprocessor(string D[1733][8], datum A[1733][8], int C[1733][8][1733], float P[1733][8][1733], int& t, int& n)
{
	//open test file
	ifstream infile;
	infile.open("car-labeled.txt");
	int i, j, k;
	char x = ' ';
	string hold;
	//cycle through the arrays
	for (i = 1; i < 1732; i++)
	{
		//initialize all tables to null or 0
		for (j = 1; j < 8; j++)
		{
			D[i][j] = " ";
			A[i][j].value = " ";
			A[i][j].count = 0;
			if (i < 10)
			  {
			    for (k = 1; k < 1732; k++)
			      {
				C[i][j][k] = 0;
				P[i][j][k] = 0.0;
				
			      }
			  }
		}
       		
	}
	//reset i and j to 1
	i = 1;
	j = 1;
	//read the first value to check if the file is empty or not
	string temp;
	unsigned int pos = 0;
	//remove the title line from the data for calculations
	getline(infile, temp);
	//get the first line of data
	getline(infile, temp);
	//get the first element in the line
	while (pos < temp.length() && temp[pos] != ',')
	{
		x = temp[pos];
		hold = hold + x;
		pos ++;
	}
	//move past the comma
	pos++;
	//keep reading in values till the end of the file
	while (infile)
	{
		//store  value in the arrays
		BuildTables(D, A, hold, i, j);
		hold = "";
		//if its the end of the line nove to the next line in the array
		if (pos - 1 == temp.length())
		{
			n = j;
			i++;
			j = 1;
			pos = 0;
			//get next line
			if (infile)
				getline(infile, temp);
		}
		//if not the end of the line then move to the next column in the array
		else
			j++;
		//read in the next value
		while (pos < temp.length() && temp[pos] != ',')
		{
			x = temp[pos];
			hold = hold + x;
			pos++;
		}
		pos++;
	}

	//save value into t
	t = i;
}
/*cycle through the arrays and store the ammount or times that each value appears
then save the percentage of yes' and no's for each value into the P array*/
void NaiveBayesLearner(string D[1733][8], datum A[1733][8], int C[1733][8][1733], float P[1733][8][1733], int& t, int& n)
{
	float thold = (float)t;        
	int i, j, k;
	//cycle throught the array
	for (int m = 1; m < t; m++)
	{
		//find the row in a certain column where the value in A matches the value in the corresponding position in D
		i = XRefD(D, A, m, n);
		//cout << i << " " << endl;
		for (j = 1; j < n; j++)
		{
			//find the row in a certain column where the value in A matches the value in the corresponding position in D
			k = XRefD(D, A, m, j);
			C[i][j][k] ++;
		}
	}
	//cycle throught the array
	for (i = 1; i < UniqueValues(A, n) + 1; i++)
	{
		//store the probability into the array P
		P[i][n][1] = A[i][n].count / thold;
		for (j = 1; j < n; j++)
		{
			for (k = 1; k < UniqueValues(A, j) + 1; k++)
			{
				P[i][j][k] = (float)C[i][j][k] / (float)A[i][n].count;
			}
		}
	}
}
/*cucle through the elements in U and get the probablility of each piece of data being a yes or no, then select the greater value and send the greater value back to
the main function*/
string NaiveBayesClassifier(datum A[1733][8], float P[1733][8][1733], string U[8], int n)
{
	double p = 0.0;
	double pTemp = 0.0;
	string c;
	int k;
	//cycle through the array
	for (int i = 1; i < UniqueValues(A, n) + 1; i++)
	{
		//store probability into a temp variable
		pTemp = P[i][n][1];
		//crossreference A with U
		for (int j = 1; j< n; j++)
		{
			k = XRefU(A, U, j);
			pTemp = pTemp * P[i][j][k];
		}
		//store the value in c
		if (pTemp > p)
		{
			p = pTemp;
			c = A[i][n].value;
		}
	}

	return c;
}
/*take values from the preprocesser and store the values in the correct positions in the arrays
in the case of A, save only one copy of every value and keep a counter of how many of each values there are*/
void BuildTables(string D[1733][8], datum A[1733][8], string hold, int i, int j)
{
	//save value into the data array
	D[i][j] = hold;

	int k = 1;
	//check if x is a repeated value in the array A
	while (A[k][j].value != " ")
	{
		//if there is then exit the array with the position of the repeared value
		if (hold== A[k][j].value)
			break;
		//else increment k and check the next row
		k++;
	}
	//save value x into the position of the first empty space in the array
	//or overwrite the value with itself
	A[k][j].value = hold;
	//increment the number of repeated elements there are for that position
	A[k][j].count++;
}
/*cycle through A and find out how many elements there are in each column and return the value to the caller*/
int UniqueValues(datum A[1733][8], int j)
{
	//cycle through the array and return the ammount of unique values in the array
	int k = 0;
	while (A[k + 1][j].value != " ")
		k++;
	return k;
}
//find the row where the D element is the same as the A element and return the position back to the caller
int XRefD(string D[1733][8], datum A[1733][8], int m, int j)
{
	//cycle through the array and return the position where the array values match
	int i;
	for (i = 1; i < UniqueValues(A, j) + 1; i++)
	{
		if (D[m][j] == A[i][j].value)
			return i;
	}
	return i;
}
//find the row where the U element is the same as the A element and return the position back to the caller
int XRefU(datum A[1733][8], string U[8], int j)
{
	int i;
	//cycle through the array and return the position where the array values match
	for (i = 1; i < UniqueValues(A, j) + 1; i++)
		if (U[j] == A[i][j].value)
			return i;
	return i;
}
