#pragma once

#include <vector>
#include <sstream>

using namespace std;

vector<string> split(string data, char delim) {
    vector<string> spt;
    stringstream ss(data);
    string temp;
    
    while (getline(ss, temp, delim)) {
        spt.push_back(temp);
    }

    return spt;
}
