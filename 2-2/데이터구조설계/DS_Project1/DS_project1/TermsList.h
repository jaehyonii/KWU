#pragma once
#include "TermsListNode.h"
#include "MemberQueueNode.h"

class TermsLIST
{
private:
	TermsListNode* head;

public:
	TermsLIST();
	~TermsLIST();

	TermsListNode* getHead();
	
	// insert
	void insertTerm(TermsListNode* cur);

	// search
	TermsListNode* searchByTerm(char termType);

	// delete
	void deleteListNode(TermsListNode* cur);
};
