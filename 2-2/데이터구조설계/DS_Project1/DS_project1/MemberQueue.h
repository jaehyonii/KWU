#pragma once
#include "MemberQueueNode.h"

class MemberQueue
{
private:
	MemberQueueNode* queue[100];
	int capacity;
	int head, tail;
	bool lastOperationIsPush;
	
public:
	MemberQueue();
	~MemberQueue();

	bool empty();
	bool full();
	void push(MemberQueueNode* cur);
	MemberQueueNode* pop();
	MemberQueueNode* front();
};

