#include "CSVLoader.h"
#include<iostream>

int main() {
	CSVLoader csv("CSVLoader_Data.txt");
	
	cout << "\"CSVLoader_Data.txt\" has " << csv.getRows() << " rows and " << csv.getCols() << " cols" << endl << endl;
	csv.print();

	csv.parse("Experiment.txt");

	cout << endl<<endl<< "\"Experiment.txt\" has " << csv.getRows() << " rows and " << csv.getCols() << " cols" << endl << endl;
	csv.print();

	return 0;
}