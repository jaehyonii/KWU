#pragma once

#include "SelectionTree.h"
#include "BpTree.h"
#include <fstream>
#include <string>

class Manager
{
private:
	BpTree* bptree;
	SelectionTree* stree;
public:
	Manager(int bpOrder)	//constructor
	{
		/* You must fill here */
		bptree = new BpTree(&flog, bpOrder); // create bptree

		stree = new SelectionTree(&flog);// create stree
		
		
	}


	~Manager()//destructor
	{
		/* You must fill here */
		delete bptree;
		delete stree;
	}

	ifstream fcmd;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool ADD(string data);

	bool SEARCH_BP(string str);
	bool SEARCH_BP_BOOK(string book);
	bool SEARCH_BP_RANGE(string start, string end);

	bool PRINT_BP();
	bool PRINT_ST(string code);

	bool DELETE();

	void printErrorCode(int n);
	void printSuccessCode(string cmd);

};

