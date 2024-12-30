#include "Manager.h"
#include <iomanip>

int main()
{
	Manager ds;	//Declare DS
	ds.run("command.txt");	//Run Program
	return 0;	//Return Program

	/*vector<pair<int, pair<int, int> > > E;
	E.push_back(make_pair(11, make_pair(1, 1)));
	E.push_back(make_pair(9, make_pair(1, 1)));
	E.push_back(make_pair(1, make_pair(1, 1)));
	E.push_back(make_pair(6, make_pair(1, 1)));
	E.push_back(make_pair(9, make_pair(1, 1)));
	E.push_back(make_pair(4, make_pair(1, 1)));
	E.push_back(make_pair(21, make_pair(1, 1)));
	E.push_back(make_pair(26, make_pair(1, 1)));

	E.push_back(make_pair(21, make_pair(1, 1)));
	E.push_back(make_pair(19, make_pair(1, 1)));


	insertion_sort(E, 0, E.size()-1);
	
	for (auto v : E)
		cout << v.first << ' ';

	return 0;*/
}
