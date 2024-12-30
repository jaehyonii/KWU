#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

class CSVLoader {
private:
	float** data;
	int cols;
	int rows;
	
public:
	static constexpr unsigned int MAX_BUFFER_SIZE = 128;
	
private:
	char** split(const char* str, char ch);

public:
	CSVLoader();
	CSVLoader(const char* path);
	~CSVLoader();

	float** getData() const;
	int getRows() const;
	int getCols() const;

	void parse(const char* path);
	void print(void);
};