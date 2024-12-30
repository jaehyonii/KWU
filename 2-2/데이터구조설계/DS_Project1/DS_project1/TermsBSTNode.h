#pragma once
#include "MemberQueue.h"
class TermsBSTNode
{
private:
	TermsBSTNode* left;
	TermsBSTNode* right;

	string name;
	int age;
	string startInfoCollection; // start date of private infomation collection
	string endInfoCollection; // expiration date of private infomation collection

public:
	TermsBSTNode(MemberQueueNode* data)
	:left(nullptr), right(nullptr), name(data->getName()), startInfoCollection(data->getStartInfoCollection())
		, endInfoCollection(data->getStartInfoCollection())
	{
		age = data->getAge();
	}

	~TermsBSTNode() {}

	string getName() { return name; }
	int getAge() { return age; }
	string getStartInfoCollection() { return startInfoCollection; }
	string getEndInfoCollection() {	return endInfoCollection; }

	void updateInfo(TermsBSTNode* bst) {
		name = bst->name;
		age = bst->age;
		startInfoCollection = bst->startInfoCollection;
		endInfoCollection = bst->endInfoCollection;
	}

	void printInfo(ofstream& stream) {
		stream << name << '/' << age << '/'
			<< startInfoCollection << '/' << endInfoCollection << endl;
	}

	TermsBSTNode*	getLeft()	{ return left; }
	TermsBSTNode*	getRight()	{ return right; }

	void setLeft (TermsBSTNode* left)	{ this->left = left; }
	void setRight(TermsBSTNode* right)	{ this->right = right; }
};