#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "LoanBookData.h"
#include <fstream>
#include <iostream>

class BpTree {
private:
	BpTreeNode* root;
	int			order;		// m children
	ofstream* fout;

	void deleteRecursive(BpTreeNode* cur) {
		if (cur->getMostLeftChild() != NULL) { // cur is IndexNode
			deleteRecursive(cur->getMostLeftChild());
			for (auto it = cur->getIndexMap()->begin(); it != cur->getIndexMap()->end(); it++) {
				deleteRecursive(it->second);
			}
		}
		delete cur;
	}
public:
	BpTree(ofstream *fout, int order = 3) {
		root = NULL;
		this->order = order;
		this->fout = fout;
	}
	~BpTree() {
		// delete all
		deleteRecursive(root);
	}
	/* essential */
	bool		Insert(LoanBookData* newData);
	bool		excessDataNode(BpTreeNode* pDataNode);
	bool		excessIndexNode(BpTreeNode* pIndexNode);
	void		splitDataNode(BpTreeNode* pDataNode);
	void		splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return root; }
	BpTreeNode* searchDataNode(string name);

	bool searchBook(string name);
	bool searchRange(string start, string end);
	bool print();
};

#endif
