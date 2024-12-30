#pragma once
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split(string data) {
    vector<string> spt;
    stringstream ss(data);
    string temp;

    while (getline(ss, temp, ' ')) {
        spt.push_back(temp);
    }

    return spt;
}
