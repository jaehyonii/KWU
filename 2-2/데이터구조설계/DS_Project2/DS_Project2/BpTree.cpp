#include "BpTree.h"
#include <vector>

bool BpTree::Insert(LoanBookData* newData) {
	BpTreeNode* pCur = root;
	BpTreeNode* pDataNode = NULL;

	// call leftMostDataNode
	while (pCur) {
		pDataNode = pCur;
		pCur = pCur->getMostLeftChild();
	}

	if (pDataNode == NULL) { // if there's no node in Bptree
		root = new BpTreeDataNode();
		root->insertDataMap(newData->getName(), newData);
		return true;
	}

	// there's already nodes
	// find proper dataNode to insert data
	BpTreeNode* pp = pDataNode;
	while (pDataNode && newData->getName() > pDataNode->getDataMap()->begin()->first) {
		pp = pDataNode;
		pDataNode = pDataNode->getNext();
	}

	pp->insertDataMap(newData->getName(), newData);

	if (excessDataNode(pp))
		splitDataNode(pp);
	
	BpTreeNode* pIndexNode = pp->getParent();
	while (pIndexNode && excessIndexNode(pIndexNode)) {
		splitIndexNode(pIndexNode);
		pIndexNode = pIndexNode->getParent();
	}
	
	return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
	else return false;
}

// not touch root
void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	// delete first data from the splited node
	map <string, LoanBookData*>* origin = pDataNode->getDataMap();
	map <string, LoanBookData*>::iterator it = origin->begin();
	int sptIndex = ceil((order - 1) / 2.0 + 1);

	// create new node
	BpTreeNode* rear = new BpTreeDataNode();
	
	// find center iterator
	for (int i = 1; i < sptIndex; i++)
		it++;

	// split rear data
	for (; it != origin->end(); it++) {
		rear->insertDataMap(it->first, it->second);
	}
	for (it = rear->getDataMap()->begin(); it != rear->getDataMap()->end(); it++)
		pDataNode->deleteMap(it->first);

	// linking front, rear node
	rear->setPrev(pDataNode);
	rear->setNext(pDataNode->getNext());
	pDataNode->setNext(rear);
	if (pDataNode->getNext()) // if next node exist
		pDataNode->getNext()->setPrev(rear);
	
	// linking with parent
	it = rear->getDataMap()->begin();
	BpTreeNode* parent = pDataNode->getParent();
	if (!parent) { // if parent not exist
		parent = new BpTreeIndexNode();
		parent->setMostLeftChild(pDataNode);
		parent->insertIndexMap(it->first, rear);
		pDataNode->setParent(parent);
		rear->setParent(parent);

		root = parent;
	}
	else { // if parent exist
		parent->insertIndexMap(rear->getDataMap()->begin()->first, rear);
		rear->setParent(parent);
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	map <string, BpTreeNode*>* origin = pIndexNode->getIndexMap();
	map <string, BpTreeNode*>::iterator it = origin->begin();

	// create new node
	BpTreeNode* rear = new BpTreeIndexNode();
	
	int sptIndex = ceil((order - 1) / 2.0 + 1);
	// find center iterator
	for (int i = 1; i < sptIndex; i++)
		it++;

	BpTreeNode* parent = new BpTreeIndexNode();
	parent->insertIndexMap(it->first, rear);
	rear->setParent(parent);

	// split rear node
	rear->setMostLeftChild(it->second);
	it->second->setParent(rear);
	pIndexNode->deleteMap((it++)->first);
	for (; it != origin->end(); it++) {
		rear->insertIndexMap(it->first, it->second);
		it->second->setParent(rear);
	}
	for (it = rear->getIndexMap()->begin(); it != rear->getIndexMap()->end(); it++)
		pIndexNode->deleteMap(it->first);

	
	// linking with parent
	it = rear->getIndexMap()->begin();
	if (!pIndexNode->getParent()) { // if parent not exist
		parent->setMostLeftChild(pIndexNode);
		pIndexNode->setParent(parent);

		root = parent;
	}
	else { // if parent exist
		pIndexNode->getParent()->insertIndexMap(parent->getIndexMap()->begin()->first, parent->getIndexMap()->begin()->second);
		rear->setParent(pIndexNode->getParent());
	}
}

// return DataNode that bookData which has name should be located
// if there's no data in BpTree, return NULL
BpTreeNode* BpTree::searchDataNode(string name) {
	BpTreeNode* pCur = root;
	
	while (pCur->getMostLeftChild()) { // it's IndexNode?
		auto it =  pCur->getIndexMap()->begin();
		if (name < (it++)->first) {
			pCur = pCur->getMostLeftChild();
			continue;
		}
		for (; it != pCur->getIndexMap()->end(); it++) {
			if (name < it->first)
				break;
		}
		pCur = (--it)->second;
	}

	return pCur;
}

bool BpTree::searchBook(string name) {
	BpTreeNode* search = searchDataNode(name);

	// if there's no data in bpTree
	if (search == NULL)
		return false;
	// if there's no book that we are searching
	if (search->getDataMap()->find(name) == search->getDataMap()->end())
		return false;
	
	LoanBookData* bookData = search->getDataMap()->find(name)->second;
	

	if (bookData == NULL) {
		return false;
	}

	(*fout) << "========SEARCH_BP========" << endl;
	(*fout) << bookData->getName() << '/'
			<< bookData->getCode() / 100 << "00" << '/'
			<< bookData->getAuthor() << '/'
			<< bookData->getYear() << '/'
			<< bookData->getLoanCount() << endl;
	(*fout) << "==========================" << endl << endl;
	
	return true;
}

bool BpTree::searchRange(string start, string end) {
	vector<LoanBookData*> bookData; // save book names that belong in range
	BpTreeNode* pCur;

	// search proper DataNode
	pCur = searchDataNode(start);
	
	
	while (pCur) {
		map <string, LoanBookData*>* dataMap = pCur->getDataMap();
		for (auto it = dataMap->begin(); it != dataMap->end(); it++) {
			if (start[0] <= ((it->first)[0] | 32) && ((it->first)[0] | 32) <= end[0])
				if(it->second != NULL)
					bookData.push_back(it->second);
		}
		pCur = pCur->getNext();
	}
	
	// if there's no book that is belonged to range
	if (bookData.size() == 0)
		return false;

	(*fout) << "========SEARCH_BP========" << endl;
	for (vector<LoanBookData*>::iterator vt = bookData.begin(); vt != bookData.end(); vt++) {
		(*fout) << (*vt)->getName() << '/'
				<< (*vt)->getCode() / 100 << "00" << '/'
				<< (*vt)->getAuthor() << '/'
				<< (*vt)->getYear() << '/'
				<< (*vt)->getLoanCount() << endl;
	}
	(*fout) << "==========================" << endl << endl;
	
	return true;
}

bool BpTree::print() {
	if (!root)
		return false;

	BpTreeNode* pCur = root;
	BpTreeNode* dataNode = NULL;

	// call leftMostDataNode
	while (pCur) {
		dataNode = pCur;
		pCur = pCur->getMostLeftChild();
	}

	(*fout) << "========PRINT_BP========" << endl;
	while (dataNode) {
		map <string, LoanBookData*>* bookData = dataNode->getDataMap();
		for (auto it = bookData->begin(); it != bookData->end(); it++) {
			LoanBookData* lbd = it->second;
			if (lbd != NULL) {
				(*fout) << lbd->getName() << '/'
					<< lbd->getCode() / 100 << "00" << '/'
					<< lbd->getAuthor() << '/'
					<< lbd->getYear() << '/'
					<< lbd->getLoanCount() << endl;
			}
		}
		dataNode = dataNode->getNext();
	}
	(*fout) << "==========================" << endl << endl;

	return true;
}