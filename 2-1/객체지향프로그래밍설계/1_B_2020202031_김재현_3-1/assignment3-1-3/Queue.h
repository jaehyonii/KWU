#include<iostream>

using namespace std;

class Node
{
private:
	Node* m_pNext;
	int m_Data;

public:
	Node();
	~Node();

	void SetData(int n);
	void SetNext(Node* pNext);
	int GetData();
	Node* GetNext();
};


class Queue
{
private:
	Node* m_pHead;
	Node* m_pTail;
	int m_Size;
	int m_NumElement;

public:
	Queue();
	~Queue();

	void SetSize(int n);
	bool IsEmpty();
	bool IsFull();
	bool Push(Node* pNode);
	Node* Pop();
	void PrintQueue();
};