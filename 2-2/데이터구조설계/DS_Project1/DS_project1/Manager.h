#pragma once
#include "MemberQueue.h"
#include "TermsList.h"
#include "TermsBST.h"
#include "NameBST.h"
#include <fstream>
#include <string>

using namespace std;

class Manager
{
private:

	ifstream	fcmd;
	ofstream	flog;
	MemberQueue queue;
	TermsLIST termsLIST;
	NameBST nameBST;

public:
	Manager();
	~Manager();

	void run(const char* command);
	
	void PrintSuccess(const char* cmd);
	void PrintErrorCode(int num);

	// LOAD
	void LOAD();
	// ADD
	void ADD(string str);
	// QPOP
	void QPOP();
	// SEARCH
	void SEARCH(string str);
	// PRINT
	void PRINT(string str);
	// DELETE
	void DELETE(string str);

	// DELETE nodes by date
	TermsBSTNode* DELETETerms(TermsBSTNode* cur, string& date);
};
