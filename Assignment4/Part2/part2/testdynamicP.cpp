


/***************************************************************************************************************
										Test Driver for dynamicP class
***************************************************************************************************************/
#include "dynamicP.h"

int main()
{
	string word1 = " ";
	string word2 = " ";
	string temp1, temp2;
	cout << "Enter the first string: ";
	cin>>temp1;
	cout << "Enter the second string: ";
	cin>>temp2;

	word1 = word1 + temp1;
	word2 = word2 + temp2;

	dynamicPro_array twoD_array(word1, word2);
	twoD_array.LongestCommonSubsequence(word1, word2);
	cout << "The longest common subsequence word is \"";
	twoD_array.Traverse(word1, word1.length()-1, word2.length()-1);
	cout << "\"" << endl;


	
	return 0;
}