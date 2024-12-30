#include "Score.h"

int main()
{
	StudentScoreList list;

	int num;
	cout << "Input the number of students: "; cin >> num;

	cout << "\n==========Input Math Score==========\n";
	cout << "->";
	double avr = 0;
	double score;
	for (int i = 0; i < num; i++) {
		cin >> score;
		avr += score;
	}
	avr /= num;
	cout << "average: " << avr << endl;
	Score* node = new Score;
	node->SetAvg(avr);
	list.Insert(node);


	cout << "\n==========Input English Score==========\n";
	cout << "->";
	avr = 0;
	for (int i = 0; i < num; i++) {
		cin >> score;
		avr += score;
	}
	avr /= num;
	cout << "average: " << avr << endl;
	node = new Score;
	node->SetAvg(avr);
	list.Insert(node);


	cout << "\n==========Input Science Score==========\n";
	cout << "->";
	avr = 0;
	for (int i = 0; i < num; i++) {
		cin >> score;
		avr += score;
	}
	avr /= num;
	cout << "average: " << avr << endl;
	node = new Score;
	node->SetAvg(avr);
	list.Insert(node);

	int isAscending;
	cout << "Ascending or Descending (1 or 0)"; cin >> isAscending;
	if(isAscending)
		list.PrintList(true);
	else
		list.PrintList(false);

	return 0;
}