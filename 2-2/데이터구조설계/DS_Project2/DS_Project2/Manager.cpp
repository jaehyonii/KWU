#include "Manager.h"
#include "StrSplit.h"

void Manager::run(const char* command) 
{
	fcmd.open(command);
	flog.open("log.txt", ios::app); // continue writing
	if(!fcmd)
	{
		flog << "File Open Error" << endl;
		return;
	}
	
	string str;
	while (!fcmd.eof())
	{
		getline(fcmd, str);
		if (!str.compare("LOAD")) {
			if (!LOAD())
				printErrorCode(100);
		}
		else if (!str.substr(0, 3).compare("ADD")) {
			if (!ADD(str.substr(4)))
				printErrorCode(200);
		}
		else if (!str.substr(0, 9).compare("SEARCH_BP")) {
			if(!SEARCH_BP(str.substr(10)))
				printErrorCode(300);
		}
		else if (!str.substr(0, 8).compare("PRINT_BP")) {
			if (!PRINT_BP())
				printErrorCode(400);
		}
		else if (!str.substr(0, 8).compare("PRINT_ST")) {
			if (!PRINT_ST(str.substr(9)))
				printErrorCode(500);
		}
		else if (!str.substr(0, 6).compare("DELETE")) {
			if (DELETE())
				printSuccessCode("DELETE");
			else
				printErrorCode(600);
		}
		else if (!str.compare("EXIT")) {
			printSuccessCode("EXIT");
			break;
		}
		else // wrong command
			printErrorCode(700);
	}

	fcmd.close();
	flog.close();
	return;
}

bool Manager::LOAD()
{
	ifstream fdata;
	string str;

	fdata.open("loan_book.txt");
	/* Error */
	if (!fdata || bptree->getRoot()) {
		return false;
	}

	/* LOAD */
	while (getline(fdata, str)) {
		vector<string> spt = split(str, '\t');
		LoanBookData* lbd = new LoanBookData();
		lbd->setBookData(spt[0], stoi(spt[1]), spt[2], stoi(spt[3]), stoi(spt[4]));
		
		bptree->Insert(lbd);
	}
	
	fdata.close();
	printSuccessCode("LOAD");

	return true;
}

bool Manager::ADD(string data)
{
	vector<string> spt = split(data, '\t');

	if (spt.size() != 4)
		return false;

	BpTreeNode* search = bptree->searchDataNode(spt[0]);
	if (!search) // there's no node in bptree
		return false;
	else if (search->getDataMap()->find(spt[0]) == search->getDataMap()->end()) { // there's no bookdata that we are searching
		// add new bookdata
		LoanBookData* lbd = new LoanBookData();
		lbd->setBookData(spt[0], stoi(spt[1]), spt[2], stoi(spt[3]));
		bptree->Insert(lbd);
	}
	else { // there's already bookdata
		// loan_book_count ++
		LoanBookData* mapData = search->getDataMap()->find(spt[0])->second;
		mapData->updateCount();
		
		switch (mapData->getCode()) {
		case 0:
		case 100:
		case 200:
			if (mapData->getLoanCount() == 3)
				stree->Insert(mapData);
			break;

		case 300:
		case 400:
			if (mapData->getLoanCount() == 4)
				stree->Insert(mapData);
			break;

		case 500:
		case 600:
		case 700:
			if (mapData->getLoanCount() == 2)
				stree->Insert(mapData);
			break;
		default:
			break;
		}
		(*search->getDataMap())[spt[0]] = NULL;
	}

	flog << "========ADD========" << endl;
	flog << spt[0] << '/' << spt[1] << '/' << spt[2] << '/' << spt[3] << endl;
	flog << "===================" << endl << endl;
	
	return true;
}

bool Manager::SEARCH_BP(string str)
{
	vector<string> spt = split(str, '\t');
	
	if (spt.size() == 1)
		return SEARCH_BP_BOOK(str);
	else
		return SEARCH_BP_RANGE(spt[0], spt[1]);
}

bool Manager::SEARCH_BP_BOOK(string book) 
{
	return bptree->searchBook(book);
}

bool Manager::SEARCH_BP_RANGE(string s, string e) 
{
	return bptree->searchRange(s, e);
}

bool Manager::PRINT_BP() 
{
	return bptree->print();
}

bool Manager::PRINT_ST(string code) 
{
	return stree->printBookData(stoi(code));	
}

bool Manager::DELETE() 
{
	return stree->Delete();
}

void Manager::printErrorCode(int num) { //ERROR CODE PRINT
	flog << "========ERROR========" << endl;
	flog << num << endl;
	flog << "======================" << endl << endl;
}

void Manager::printSuccessCode(string cmd) { //SUCCESS CODE PRINT 
	flog << "========" << cmd << "========" << endl;
	flog << "Success" << endl;
	flog << "==========================" << endl << endl;
}

