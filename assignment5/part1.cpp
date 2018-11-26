/*
CS 340 Assignment 5
Part 1

Purpose: To implement 4 algorithms for text processing
Brute force
Boyer-Moore
BoyerMoore with no character jump
Knuth-Morris-Pratt

Read input from text and pattern files
Count all instances of pattern with all 4 algorithms
Count all comparisons made in algorithms
output results


*/


#include<string>
#include<iostream>
#include<fstream>
#include<algorithm> 
#include<iomanip>
using namespace std;


int BruteForce(string,string,int&);
int BoyerMoore(string,string,int&);
int BoyerMooreNoJump(string,string,int&);
int Last(char,string,int&);
int KnuthMorrisPratt(string t,string p,int&);
void Failure(int*, string,int&);

/******************************************************************************************************
******************************************************************************************************/
int main(){
	string text = "";
	char temp;

	// open and error check text input
	ifstream inputs;
	inputs.open("text.txt");
	if(!inputs)
	{
		cout << "Error opening input file text.txt";
		return 1;
	}

	//get input text and save to text string
	do{
		inputs.get(temp);
		text += temp;
	}while(inputs);

	// open and error check patterns input
	ifstream inputs2;
	inputs2.open("patterns.txt");
	if(!inputs2)
	{
		cout << "Error opening input file patterns.txt";
		return 2;
	}

	int n = 0;
	string temp1;
	//count number of lines in patterns input
	do{
		getline(inputs2,temp1);
		if(temp1 != "")
			n++;
	}while(inputs2);

	// open patterns input again
	ifstream inputs3;
	inputs3.open("patterns.txt");

	// save all patterns into patterm array
	string* patterns = new string[n];
	for(int i = 0; i < 15; i++)
	{
		getline(inputs3,patterns[i]);
	}
	
	// output data for all 4 searching algorithms for every pattern
	for(int i = 0; i < 15; i++)
	{
		int found;
		int comparisons1 = BruteForce(text,patterns[i],found);
		int comparisons2 = BoyerMoore(text,patterns[i],found);
		int comparisons3 = BoyerMooreNoJump(text,patterns[i],found);
		int comparisons4 = KnuthMorrisPratt(text,patterns[i],found);
		cout << "=======================================================================" << endl;
		cout << "Search for: " << patterns[i] << endl;
		cout << "Found " << found << " times." << endl;
		cout << "Brute Force";
		cout << setw(13) << "Boyer-Moore";
		cout << setw(23) << "Boyer-Moore(No Jump)";
		cout << setw(22) << "Knuth-Morris-Pratt " << endl;
		cout << comparisons1;
		cout << setw(13) << comparisons2;
		cout << setw(14) << comparisons3;
		cout << setw(23) << comparisons4 << endl;
	}

	cout << "=======================================================================" << endl;
	system("PAUSE");
	return 0;
}
/******************************************************************************************************
******************************************************************************************************/

///////////////////////////////////////////////////////////////////////
// BruteForce function
//
// code taken from CS340 algorithms page, and modified
// to search for ALL instances of word, and count
// the total number of comparisons
//
// input: string t is the text to be searched
//        string p is the word that is being searched for
//
// output: total number of comparisons by algorithm
///////////////////////////////////////////////////////////////////////
int BruteForce(string t,string p, int& findCounter){
	int m = p.length();
	int n = t.length();
	int comparisonCounter = 0; // count number if time comparisons are made
	findCounter = 0; // count number of times word is found

	//comparisonCounter++; // increment counter for every comparison (for loop initial comparison)
	for(int i = 0; i <=  (n - m) ; i++)
	{
		//comparisonCounter++; // for loop comparison counter
		int j = 0;

		comparisonCounter++; // increment counter for every comparison (while loop intitial comparison)
		while(j < m && t[i + j] == p[j])
		{
			comparisonCounter++;// while loop comparison counter
			j ++;
		}

		//comparisonCounter++; // increment counter for every comparison (if statement)
		if(j == m)
			findCounter++;
	}
	return comparisonCounter; 
}

///////////////////////////////////////////////////////////////////////
// BoyerMoore function
// and Last Function
//
// code taken from CS340 algorithms page, and modified
// to search for ALL instances of word, and count
// the total number of comparisons
//
// input: string t is the text to be searched
//        string p is the word that is being searched for
//
// output: total number of comparisons by algorithm
///////////////////////////////////////////////////////////////////////
int BoyerMoore(string t,string p, int& findCounter){
	int m = p.length();
	int n = t.length();
	int i = m - 1;
	int j = m - 1;
	int comparisonCounter = 0; // count number if time comparisons are made
	findCounter = 0; // count number of times word is found

	do{
		comparisonCounter++; // increment counter for every comparison (if statement)
		if(p [j] == t [i])
		{
			//comparisonCounter++; // increment counter for every comparison (if statement)
			if(j == 0)
			{
				findCounter++;
				j = m - 1;  // dont stop but move ahead past found word and keep going
				i = i + m + (m-1);////////////
			}
			else
			{
				i --;
				j --;
			}
		}
		else
		{
			i = i + m - min(j, 1 + Last(t[i], p,comparisonCounter));
			j = m - 1;
		}

		//comparisonCounter++; // increment counter for every comparison (while loop comparison)
	}while(i < n);
	return comparisonCounter;
}
int Last (char c,string p,int& comparisonCounter){
	int m = p.length();
	//comparisonCounter++; // increment counter for every comparison (for loop initial comparison)
	for(int i = m - 1; i>=0; i--)
	{
		//comparisonCounter++; // increment counter for every comparison (if statement comparison)
		if(c == p [i])
			return i;

		//comparisonCounter++; // increment counter for every comparison(for loop comparison)
	}
	return -1;
}


///////////////////////////////////////////////////////////////////////
// BoyerMooreNoJump function
//
// BoyerMoore function with character jump heuristic removed
//
// input: string t is the text to be searched
//        string p is the word that is being searched for
//
// output: total number of comparisons by algorithm
///////////////////////////////////////////////////////////////////////
int BoyerMooreNoJump(string t,string p, int& findCounter){
	int m = p.length();
	int n = t.length();
	int i = m - 1;
	int j = m - 1;
	int comparisonCounter = 0; // count number if time comparisons are made
	findCounter = 0; // count number of times word is found

	do{
		comparisonCounter++; // increment counter for every comparison (if statement)
		if(p [j] == t [i])
		{
			//comparisonCounter++; // increment counter for every comparison (if statement)
			if(j == 0)
			{
				findCounter++;
				j = m - 1;  // dont stop but move ahead one character and keep searching
				i = i + m;
			}
			else
			{
				i --;
				j --;
			}
		}
		else
		{
			i = i + 1 + ((m-1)-j);
			j = m - 1;
		}
		//comparisonCounter++; // increment counter for every comparison (while loop comparison)
	}while(i < n);
	return comparisonCounter;
}


///////////////////////////////////////////////////////////////////////
// KnuthMorrisPratt function
// and Failure function
//
// code taken from CS340 algorithms page, and modified
// to search for ALL instances of word, and count
// the total number of comparisons
//
// input: string t is the text to be searched
//        string p is the word that is being searched for
//
// output: total number of comparisons by algorithm
///////////////////////////////////////////////////////////////////////
int KnuthMorrisPratt(string t,string p,int& findCounter){
	int m = p.length();
	int n = t.length();
	int* f = new int[m];
	int comparisonCounter = 0; // count number if time comparisons are made
	findCounter = 0; // count number of times word is found
	Failure (f, p,comparisonCounter);
	int i = 0;
	int	j = 0;
	//comparisonCounter++; // increment counter for every comparison (while loop initial comparison)
	while (i < n)
	{
		comparisonCounter++; // increment counter for every comparison (if statement)
		if( p [j] == t [i])
		{
			//comparisonCounter++; // increment counter for every comparison (if statement)
			if (j == m - 1)
			{
				findCounter++;
				i++;
				j = 0;
			}
			else{
				i ++;
				j ++;
			}
		}
		else if (j > 0)
			j = f [j - 1];
		else
			i ++;
	}
	return comparisonCounter;
}
void Failure (int* f,string p,int& comparisonCounter){
	int m = p.length();
	int i = 1;
	int j = 0;
	f [0] = 0;
	//comparisonCounter++; // increment counter for every comparison (while loop initial comparison)
	while(i < m)
	{
		//comparisonCounter++; // increment counter for every comparison (if statement)
		if(p [j] == p [i])
		{
			f [i] = j + 1;
			i ++;
			j ++;
		}
		else if(j > 0)
			j = f [j - 1];
		else
		{
			f [i] = 0;
			i ++;
		}
		//comparisonCounter++; // increment counter for every comparison (while loop comparison)
	}
}