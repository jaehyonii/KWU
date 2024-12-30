#pragma once
#include <fstream>
#include "TermsBSTNode.h"
#include "NameBSTNode.h"

using namespace std;
class TermsBST
{
private:
	TermsBSTNode* root;

	// delete all nodes in destructor
	void postOrderDelete(TermsBSTNode* cur);
public:
	TermsBST();
	~TermsBST();

	TermsBSTNode* getRoot();
	void setRoot(TermsBSTNode* node);

	// insert
	void insert(TermsBSTNode* cur);

	// search
	TermsBSTNode* search(string date);

	// print
	void printAll(ofstream& flog);
	void inorderPrint(ofstream& flog, TermsBSTNode* cur);

	// delete only one node that has same name
	bool deleteNode(NameBSTNode* cur);
};