#pragma once
#include "TermsBST.h"
#include "MemberQueueNode.h"

class TermsListNode
{
private:
	char termType;
	int members; // number of members
	TermsBST* termsBST;
	TermsListNode* next;

public:
	TermsListNode(MemberQueueNode* tmp) 
	:next(nullptr)
	{
		termType = tmp->getTermType();
		members = 0;
		termsBST = new TermsBST();
	}
	~TermsListNode() {
		delete termsBST;
	}

	bool isEmpty() {
		if (!termsBST->getRoot())
			return true;
		else
			return false;
	}

	char getTermType() { return termType; }
	int getMembers() { return members; }
	TermsBST* getTermsBST() { return termsBST; }

	void increaseMembers() { members++; }
	void decreaseMembers() { members--; }

	TermsListNode* getNext() { return next; }
	void setNext(TermsListNode* next)	 { this->next = next; }
};