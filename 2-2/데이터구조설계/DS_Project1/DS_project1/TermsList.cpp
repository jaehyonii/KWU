#include "TermsList.h"

TermsLIST::TermsLIST(): head(nullptr)
{

}
TermsLIST::~TermsLIST()
{
	/* delete all TermsListNode*/
	while (head) {
		TermsListNode* tmp = head;
		head = head->getNext();
		delete tmp;
	}
}

TermsListNode* TermsLIST::getHead() { return head; }

// insert
void TermsLIST::insertTerm(TermsListNode* node) {
	/* if there's no Node in termsLIST*/
	if (head == nullptr) {
		head = node;
		return;
	}

	/* finding tail*/
	TermsListNode* cur = head, *pcur = nullptr;
	while (cur) {
		pcur = cur;
		cur = cur->getNext();
	}

	/* insert new TermsListNode*/
	pcur->setNext(node);
}

// search
TermsListNode* TermsLIST::searchByTerm(char termType){
	if (head == nullptr)
		return nullptr;
	TermsListNode* cur = head;
	
	while (cur && cur->getTermType() != termType) {
		cur = cur->getNext();
	}
	return cur;
};

// delete
void TermsLIST::deleteListNode(TermsListNode* cur) {
	if (cur == head) {
		head = cur->getNext();
		delete cur;
	}
	else {
		//find cur's parent
		TermsListNode* p = head, * pp = nullptr;
		while (p && p != cur) {
			pp = p;
			p = p->getNext();
		}
		pp->setNext(cur->getNext()); //insert
		delete cur; //delete
	}
}