#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class MemberQueueNode
{
private:
	string name;
	int age;
	string startInfoCollection;
	char termType;
	
public:
	MemberQueueNode(string line) {
		stringstream ss(line);
		
		ss >> name >> age >> startInfoCollection >> termType;
	}
	~MemberQueueNode() {}

	// get name
	string getName() {
		return name;
	}
	// get age
	int getAge() {
		return age;
	}
	// get starting point
	string getStartInfoCollection() {
		return startInfoCollection;
	}
	// get term type
	char getTermType() {
		return termType;
	}

	void printInfo(ofstream& stream) {
		stream << name << '/' << age << '/'
			<< startInfoCollection << '/' << termType << endl;
	}
};