#pragma once
#include <string>
#include "MemberQueueNode.h"

using namespace std;

class NameBSTNode
{
private:
	string name;
	int age;
	string startInfoCollection; // start date of private infomation collection
	string endInfoCollection; // expiration date of private infomation collection
	char termType; // term type

	NameBSTNode* left;
	NameBSTNode* right;
	

public:
	NameBSTNode(NameBSTNode& data) {
		name = data.name;
		age = data.age;
		startInfoCollection = data.startInfoCollection;
		endInfoCollection = data.endInfoCollection;
		termType = data.termType;
	}
	NameBSTNode(MemberQueueNode* data)
	:left(nullptr), right(nullptr), name(data->getName()), startInfoCollection(data->getStartInfoCollection())
		,endInfoCollection(data->getStartInfoCollection())
	{
		age = data->getAge();
		termType = data->getTermType();
		
		/* calculate expiration date */
		int year = stoi(endInfoCollection.substr(0, 4));
		int month = stoi(endInfoCollection.substr(5, 2));
		switch (termType) {
		case 'A':
			month = month + 6;
			if (month > 12) {
				year++;
				month = month - 12;
			}
			endInfoCollection.replace(0, 4, to_string(year));

			/* to make month to maintain two digits*/
			if(month<10)
				endInfoCollection.replace(5, 2, "0" + to_string(month));
			else
				endInfoCollection.replace(5, 2, to_string(month));
			
			break;
		case 'B':
			year++;
			endInfoCollection.replace(0, 4, to_string(year));
			break;
		case 'C':
			year = year + 2;
			endInfoCollection.replace(0, 4, to_string(year));
			break;
		case 'D':
			year = year + 3;
			endInfoCollection.replace(0, 4, to_string(year));
			break;
		default:
			break;
		}
	}
	~NameBSTNode() {}

	string getName() { return name; }
	string getEndInfoCollection() { return endInfoCollection; }
	char getTermType() { return termType; }
	
	void updateInfo(NameBSTNode* bst) {
		name = bst->name;
		age = bst->age;
		startInfoCollection = bst->startInfoCollection;
		endInfoCollection = bst->endInfoCollection;
		termType = bst->termType;
	}

	void printInfo(ofstream& stream) {
		stream << name << '/' << age << '/' 
			<< startInfoCollection << '/' << endInfoCollection << endl;
	}
	
	NameBSTNode* getLeft()	{ return left; }
	NameBSTNode* getRight()	{ return right; }

	void setLeft(NameBSTNode* left)		{ this->left = left; }
	void setRight(NameBSTNode* right)	{ this->right = right; }
};