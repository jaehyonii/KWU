#include "List.h"

int main()
{
	Polynomial p1;
	Polynomial p2;
	Polynomial p3;

	Term* tmp=nullptr;
	srand((unsigned int)time(NULL));
	for (int i = 5; i > -3; i--) {
		tmp = new Term();
		tmp->SetCoeff((rand()-rand()) % 10);
		tmp->SetExponent(i);
		p1.Insert(tmp);
	}

	for (int i = 5; i > -3; i--) {
		tmp = new Term();
		tmp->SetCoeff((rand() - rand()) % 10);
		tmp->SetExponent(i);
		p2.Insert(tmp);
	}

	for (int i = 5; i > -3; i--) {
		tmp = new Term();
		tmp->SetCoeff((rand() - rand()) % 10);
		tmp->SetExponent(i);
		p3.Insert(tmp);
	}
	cout << "p1: ";	p1.PrintList();	cout << endl;
	
	cout << "p2: ";	p2.PrintList();	cout << endl;

	cout << "p3: ";	p3.PrintList();	cout << endl;

	cout << "p1 + p2: ";
	p1.Add(p2); p1.PrintList();
	cout << endl;

	cout << "p2 - p3: ";
	p2.Sub(p3);	p2.PrintList();
	cout << endl << endl;
	
	return 0;
}