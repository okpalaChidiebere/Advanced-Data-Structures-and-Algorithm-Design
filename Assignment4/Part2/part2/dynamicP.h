/*
dymaicPro_array class

purpose:
to find the longest common subsequence of two strings

data type:   INT 2D array, STRING 2D array

features: crate an array of INT 2D, get the length of the two strings, traverse the two 2D array created, finding the LongestCommonSubsequence, .

*/

#ifndef test_H

#include<iostream>
#include<string>

const int MAX_ROW_SIZE = 20;
const int MAX_COLUMN_SIZE = 20;


using namespace std;

class dynamicPro_array //: public stringOfchar
{
private:
	//datas
	int c[MAX_ROW_SIZE][MAX_COLUMN_SIZE];
	string b[MAX_ROW_SIZE][MAX_COLUMN_SIZE];
	int m, n;  //holds the length of the two words
public:

	dynamicPro_array(string x, string y);//constructor
	void LongestCommonSubsequence(string x, string y);
	void Traverse(string x, int i, int j);


};
#endif