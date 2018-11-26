#include "dynamicP.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dynamicPro_array constructor
//
// purpose: initializes the int 2D array to 0, and the string 2D array to EMPTY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

dynamicPro_array::dynamicPro_array(string x, string y)
{
	m = x.length(); 
	n = y.length();
	for (int i = 0; i <= m; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			c[i][j] = 0;
			b[i][j] = "";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dynamicPro_array LongestCommonSubsequence public function
//
//input: two array of strings
//
// purpose: to build the integer table and the string table accordingly to be able to find the longest common subsequence from any index in both stings
//NOTE: we use a character in the frist string to compare to the rest in the second string. and repeat for the rest of the characters in the first string
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dynamicPro_array::LongestCommonSubsequence(string x, string y)
{

	for (int i = 1; i <=m; i++)
	{
		for (int j = 1; j <=n; j++)
		{
			if (x[i] == y[j])  //when the two chatacter match
			{
				c[i][j] = c[i - 1][j - 1] + 1;  //increment the diagonal index at the top left by 1 and put the value at the current index
				b[i][j] = "\"";   //shows where we go the value from
			}
			else if (c[i - 1][j] >= c[i][j - 1]) //when the two characters dont match and the index on top of the current index in the 2D array is greater whe choose it and put in the current index
			{
				c[i][j] = c[i - 1][j];
				b[i][j] = "|";
			}
			else//when the two characters dont match and the index on left of the current index in the 2D array is greater whe choose it and put in the current index
			{
				c[i][j] = c[i][j - 1];
				b[i][j] = "-";
			}
		}
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dynamicPro_array LongestCommonSubsequence public function
//
//input: the first string and the index to start checking the longest common subsequence
//
// purpose: to traverse the sting 2D array we built
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dynamicPro_array::Traverse(string x, int i, int j)
{

	if (i == 0 || j == 0) //base case
		return;
	if (b[i][j] == "\"")
	{
		Traverse(x, i - 1, j - 1);
		cout << x[i];
	}
	else if (b[i][j] == "|")
		Traverse(x, i - 1, j);
	else
		Traverse(x, i, j - 1);

	return;
}