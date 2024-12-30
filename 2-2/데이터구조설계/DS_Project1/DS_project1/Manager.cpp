#include "Manager.h"
#include "StrSplit.h"

Manager::Manager()
{

}
Manager::~Manager()
{
	
}

void Manager::run(const char* command)
{
	string str;

	// Open command & log file
	fcmd.open(command);
	flog.open("log.txt");
	if (!fcmd)
	{
		flog << "Fail to open command file" << endl;
		exit(-1);
	}

	// Run command
	while (getline(fcmd, str)) {
		if (!str.compare("LOAD")) {
			LOAD();
		}
		else if (!str.substr(0, 3).compare("ADD")) {
			ADD(str.substr(4));
		}
		else if (!str.compare("QPOP")) {
			QPOP();
		}
		else if (!str.substr(0, 6).compare("SEARCH")) {
			SEARCH(str.substr(7));
		}
		else if (!str.substr(0, 5).compare("PRINT")) {
			PRINT(str.substr(6));
		}
		else if (!str.substr(0, 6).compare("DELETE")) {
			DELETE(str.substr(7));
		}
		else if (!str.compare("EXIT")) {
			PrintSuccess("EXIT");
			break;
		}
		else
			PrintErrorCode(1000);
	}

	fcmd.close();
	flog.close();
	return;
}

void Manager::PrintSuccess(const char* cmd)
{
	flog << "===== " << cmd << " =====" << endl;
	flog << "Success" << endl;
	flog << "===============" << endl << endl;
}
void Manager::PrintErrorCode(int num)
{
	flog << "===== ERROR =====" << endl;
	flog << num << endl;
	flog << "===============" << endl << endl;
}

// LOAD
void Manager::LOAD()
{
	ifstream fdata;
	string str;

	fdata.open("data.txt");
	/* Error */
	// if textfile doesn't exist or data already exist in queue
	if (!fdata || !queue.empty()) {
		PrintErrorCode(100);
		return;
	}

	/* LOAD */
	flog << "===== LOAD =====" << endl;
	while (getline(fdata, str)) {
		MemberQueueNode* cur = new MemberQueueNode(str);
		queue.push(cur);
		cur->printInfo(flog);
	}
	flog << "===============" << endl << endl;

	fdata.close();
}

// ADD
void Manager::ADD(string str) {
	vector<string> spt = split(str);

	// if even one element is lacking
	if (spt.size() != 4) {
		PrintErrorCode(200);
		return;
	}

	MemberQueueNode* cur = new MemberQueueNode(str);
	queue.push(cur);

	flog << "===== ADD =====" << endl;
	cur->printInfo(flog);
	flog << "===============" << endl << endl;
}

// QPOP
void Manager::QPOP() {
	// if there's no data in MemberQueue
	if (queue.empty()) {
		PrintErrorCode(300);
		return;
	}

	while (!queue.empty()) {
		MemberQueueNode* mq = queue.pop();
		
		/* TermsList & TermsBST */

		// find termsListNode that corresponds to memberQueueNode
		TermsListNode* cur = termsLIST.searchByTerm(mq->getTermType());

		// if termsListNode not exists
		if (!cur) {
			// insert termsListNode
			TermsListNode* node = new TermsListNode(mq);
			termsLIST.insertTerm(node);
		}

		// termsListNode now exists or already exists
		// insert TermsBSTNode
		cur = termsLIST.searchByTerm(mq->getTermType());
		TermsBSTNode* Tnode = new TermsBSTNode(mq);

		cur->getTermsBST()->insert(Tnode);
		cur->increaseMembers();


		/* NameBST */

		// insert NameBSTNode
		NameBSTNode* Nnode = new NameBSTNode(mq);
		nameBST.insert(Nnode);

		/* delete queueNode */
		delete mq;
	}

	PrintSuccess("QPOP");
}

// SEARCH
void Manager::SEARCH(string str) {
	NameBSTNode* cur = nameBST.search(str);
	if (!cur) {
		PrintErrorCode(400);
		return;
	}

	flog << "===== " << "SEARCH" << " =====" << endl;
	cur->printInfo(flog);
	flog << "===============" << endl << endl;
}

// PRINT
void Manager::PRINT(string opt) {
	/* That termsLIST is empty means there's no data in BSTs*/
	if (!termsLIST.getHead()) {
		PrintErrorCode(500);
		return;
	}

	if (!opt.compare("A")) {
		if (!termsLIST.searchByTerm('A')) {
			PrintErrorCode(500);
			return;
		}
		flog << "===== " << "PRINT" << " =====" << endl;
		flog << "Terms_BST " << 'A' << endl;
		termsLIST.searchByTerm('A')->getTermsBST()->printAll(flog);
		flog << "===============" << endl << endl;
	}
	else if (!opt.compare("B"))	{
		if (!termsLIST.searchByTerm('B')) {
			PrintErrorCode(500);
			return;
		}
		flog << "===== " << "PRINT" << " =====" << endl;
		flog << "Terms_BST " << 'B' << endl;
		termsLIST.searchByTerm('B')->getTermsBST()->printAll(flog);
		flog << "===============" << endl << endl;
	}
	else if (!opt.compare("C"))	{
		if (!termsLIST.searchByTerm('C')) {
			PrintErrorCode(500);
			return;
		}
		flog << "===== " << "PRINT" << " =====" << endl;
		flog << "Terms_BST " << 'C' << endl;
		termsLIST.searchByTerm('C')->getTermsBST()->printAll(flog);
		flog << "===============" << endl << endl;
	}
	else if (!opt.compare("D")) {
		if (!termsLIST.searchByTerm('D')) {
			PrintErrorCode(500);
			return;
		}
		flog << "===== " << "PRINT" << " =====" << endl;
		flog << "Terms_BST " << 'D' << endl;
		termsLIST.searchByTerm('D')->getTermsBST()->printAll(flog);
		flog << "===============" << endl << endl;
	}
	else if (!opt.compare("NAME")) {
		flog << "===== " << "PRINT" << " =====" << endl;
		flog << "Name_BST " << endl;
		nameBST.printAll(flog);
		flog << "===============" << endl << endl;
	}
	else { // command isn't proper
		PrintErrorCode(500);
		return;
	}
}

// DELETE
void Manager::DELETE(string str) {
	if (!str.substr(0, 4).compare("DATE")) {
		string date = str.substr(5);
		for (int i = 0; i < 4; i++) {
			TermsListNode* cur = termsLIST.searchByTerm('A' + i);
			if(cur)	{
				TermsBSTNode* root = cur->getTermsBST()->getRoot();
				cur->getTermsBST()->setRoot(DELETETerms(root, date));
			}
		}
	}
	else if (!str.substr(0, 4).compare("NAME")) {
		string name = str.substr(5);
		NameBSTNode* Nnode = nameBST.deleteNode(name);
		if (!Nnode) {
			PrintErrorCode(600); // there's no one who has name
			return;
		}

		TermsListNode* TLnode = termsLIST.searchByTerm(Nnode->getTermType());
		if (TLnode->getTermsBST()->deleteNode(Nnode))
			TLnode->decreaseMembers();
		else {
			PrintErrorCode(600); // there's no one who has name
			return;
		}
		delete Nnode;
	}
	else {
		PrintErrorCode(600); // command isn't proper
		return;
	}

	for (int i = 0; i < 4; i++) {
		TermsListNode* cur = termsLIST.searchByTerm('A' + i);
		if (cur && cur->isEmpty())
			termsLIST.deleteListNode(cur);
	}
	PrintSuccess("DELETE");
}

// delete nodes that is earlier than date
TermsBSTNode* Manager::DELETETerms(TermsBSTNode* cur, string& date) {
	if (!cur)
		return nullptr;
	cur->setLeft(DELETETerms(cur->getLeft(), date));
	cur->setRight(DELETETerms(cur->getRight(), date));
	if (cur->getEndInfoCollection() < date) {
		nameBST.deleteNode(cur->getName());
		TermsBSTNode* tmp = cur->getRight();
		delete cur;
		return tmp;
	}
	else {
		return cur;
	}
}
