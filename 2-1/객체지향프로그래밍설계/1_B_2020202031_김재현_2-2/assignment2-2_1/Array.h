#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>
#include<stdlib.h>

using namespace std;

class List {
private:
	List* pNext;
	List* pDown;
	char* str;

public:
	List();

	void addNext();
	
	void addDown();

	void addStr(const char* str);
	
	List* getNext();
	List* getDown();
	char* getStr();
	
	void print();
	~List();
};

class List_Manager {
private:
	char** spt;
public:
	List_Manager(char** spt);

	void makeList(List* list);

	void printArray(List* list);

	void print(List* list, const char* condition);

	void List_delete(List* list);
};