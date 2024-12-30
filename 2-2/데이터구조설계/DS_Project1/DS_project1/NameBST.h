#pragma once
#include "NameBSTNode.h"

class NameBST
{
private:
	NameBSTNode* root;

	// delete all nodes in destructor
	void postOrderDel(NameBSTNode* t);
public:
	NameBST();
	~NameBST();

	NameBSTNode* getRoot();

	// insert
	void insert(NameBSTNode* cur);
	// search by name
	NameBSTNode* search(string name);
	// print
	void printAll(ofstream& flog);
	void inorderPrint(ofstream& flog, NameBSTNode* cur);
	// delete by name
	NameBSTNode* deleteNode(string name);
};