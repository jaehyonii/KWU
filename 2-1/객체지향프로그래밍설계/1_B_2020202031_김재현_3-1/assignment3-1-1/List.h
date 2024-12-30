#pragma once

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

class Node
{
private:
	Node* pNext;
	char* data;

public:
	Node();
	Node(const char* str);
	~Node();

	void setData(char* str);
	void setNext(Node* pNext);
	char* getData();
	Node* getNext();
};

void bubbleSort(Node** pHead, int size);
bool compare(char* str1, char* str2);
void Merge_List(Node* p1, Node* p2, Node* p3);