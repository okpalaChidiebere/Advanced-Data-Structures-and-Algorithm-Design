/*
Tree class

purpose: Create a basic binary tree class
that will be used to solve the average depth problem in assignment 1, part 2

data type: INT

features: create a binary tree, insert values (and return depth the inserted value)
			and destroy a tree


*/
#include <iostream>
using namespace std;


// structure used for tree nodes
struct node{
	int value;
	node *left;
	node *right;
};


class Tree
{
public:
	Tree();
	void destroy_tree();
	int insert(int key);
	

private:
	 void destroy_tree(node *leaf);
	int insert(int key, node *leaf);
	

	node *root;

};