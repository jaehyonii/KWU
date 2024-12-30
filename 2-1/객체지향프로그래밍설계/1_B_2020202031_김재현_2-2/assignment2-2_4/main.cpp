#include <iostream>
#include "Clock.h"

int main() {
	char ch;
	int second;
	Clock clock;

	cout << "Set beggining time" << endl;
	cout << "Enter seconds: "; cin >> second;
	
	clock.setTime(second);
	while (1) {
		cout << "input among H, M, S, X: ";
		cin >> ch;
		switch (ch) {
		case 'H':
			clock.increaseHour();
			break;
		case 'M':
			clock.increaseMinute();
			break;
		case 'S':
			clock.increaseSecond();
			break;
		case 'X':
			return 0;
		default:
			cout << "unvalid input!" << endl;
		}
	}
	return 0;
}