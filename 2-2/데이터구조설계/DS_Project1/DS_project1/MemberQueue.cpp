#include "MemberQueue.h"

MemberQueue::MemberQueue()
{
	capacity = 100;
	head = 0;
	tail = 0;
	lastOperationIsPush = false;
}
MemberQueue::~MemberQueue()
{
	// queue have queuenode
	if(head!=tail)
	{
		for (int i = head + 1; i != tail; i = (i + 1) % capacity)
			delete queue[i];
		delete queue[tail];
	}
}

bool MemberQueue::empty()
{
	if (head == tail && !lastOperationIsPush) // 마지막 연산이 pop일 때
		return true;
	else
		return false;
}
bool MemberQueue::full()
{
	if (head == tail && lastOperationIsPush) // 마지막 연산이 push일 때
		return true;
	else
		return false;
}
void MemberQueue::push(MemberQueueNode* cur)
{
	if (full())
		exit(1);
	tail = (tail + 1) % capacity; // moving tail clockwise
	queue[tail] = cur;

	lastOperationIsPush = true;
}
MemberQueueNode* MemberQueue::pop()
{
	if (empty())
		exit(1);

	head = (head + 1) % capacity; // moving head clockwise
	MemberQueueNode* tmp = queue[head];

	lastOperationIsPush = false;

	return tmp;
}
MemberQueueNode* MemberQueue::front()
{
	return queue[head + 1];
}