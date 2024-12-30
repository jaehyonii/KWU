#include "SelectionTree.h"
#include <map>
#include <queue>

bool SelectionTree::Insert(LoanBookData* newData) {
	SelectionTreeNode* cur = leafNode[(newData->getCode()) / 100];
	cur->getHeap()->Insert(newData);
	
	cur->setBookData(cur->getHeap()->getRoot()->getBookData());

	// bubbling up
	while (cur != root) {
		SelectionTreeNode* sibling;
		sibling = cur == cur->getParent()->getLeftChild() ?
			cur->getParent()->getRightChild() : cur->getParent()->getLeftChild();
			
		// initialization
		if (!sibling->getBookData())
			cur->getParent()->setBookData(cur->getBookData());
		// comparing
		else if (cur->getBookData()->getName() < sibling->getBookData()->getName())
			cur->getParent()->setBookData(cur->getBookData());
		else {
			cur->getParent()->setBookData(sibling->getBookData());
			break;
		}
		cur = cur->getParent();
	}

	return true;
}

bool SelectionTree::Delete() {
	if (root == NULL)
		return false;

	SelectionTreeNode* cur = root;
	SelectionTreeNode* child;
	LoanBookData* lbd = root->getBookData();

	// erase all path nodes that same with root
	while (!cur->getHeap()) {
		if (cur->getLeftChild()->getBookData() && lbd->getName() == cur->getLeftChild()->getBookData()->getName())
			child = cur->getLeftChild();
		else if (cur->getRightChild()->getBookData() && lbd->getName() == cur->getRightChild()->getBookData()->getName())
			child = cur->getRightChild();
		else
			break;
		
		cur->setBookData(NULL);
		cur = child;
	}

	cur->setBookData(NULL);
	cur->getHeap()->heapifyDown(cur->getHeap()->getRoot());
	
	// rematch start
	cur->setBookData(cur->getHeap()->getRoot()->getBookData());
	// bubbling up
	while (cur != root) {
		SelectionTreeNode* sibling;
		sibling = cur == cur->getParent()->getLeftChild() ?
			cur->getParent()->getRightChild() : cur->getParent()->getLeftChild();

		// initialization
		if (sibling->getBookData() == NULL)
			cur->getParent()->setBookData(cur->getBookData());
		// comparing
		else if (cur->getBookData()->getName() < sibling->getBookData()->getName())
			cur->getParent()->setBookData(cur->getBookData());
		else
			cur->getParent()->setBookData(sibling->getBookData());
		
		cur = cur->getParent();
	}

	return true;
}

bool SelectionTree::printBookData(int bookCode) {
	// cur is code 100 heap's root
	LoanBookHeapNode* cur = leafNode[bookCode / 100]->getHeap()->getRoot();
	queue<LoanBookHeapNode*> Queue;

	if (cur == NULL)
		return false;
	
	map<string, LoanBookData*> lbd;
	Queue.push(cur);
	while (!Queue.empty()) {
		cur = Queue.front();
		Queue.pop();
		lbd[cur->getBookData()->getName()] = cur->getBookData();
		if (cur->getLeftChild())
			Queue.push(cur->getLeftChild());
		if (cur->getRightChild())
			Queue.push(cur->getRightChild());
	}

	(*fout) << "========PRINT_ST========" << endl;
	for(map<string, LoanBookData*>::iterator it = lbd.begin(); it!=lbd.end(); it++) {
		(*fout) << it->second->getName() << '/'
				<< it->second->getCode()/100 << "00" << '/'
				<< it->second->getAuthor() << '/'
				<< it->second->getYear() << '/'
				<< it->second->getLoanCount()
				<< endl;
	}
	(*fout) << "=========================" << endl << endl;

	return true;
}