#include "Queue.h"

/* Node class*/
Node::Node() {
	m_pNext = nullptr;
	m_Data = 0;
}
Node::~Node(){}

void Node::SetData(int n) {
	m_Data = n;
}
void Node::SetNext(Node* pNext) {
	m_pNext = pNext;
}
int Node::GetData() {
	return m_Data;
}
Node* Node::GetNext() {
	return m_pNext;
}

/* Queue class*/
Queue::Queue() {
	m_pHead = nullptr;
	m_pTail = nullptr;
	m_Size = 0;
	m_NumElement = 0;
}
Queue::~Queue() {
	Node* pWalker = m_pHead;
	Node* pPre;
	/* Queue 객체의 모든 노드들을 삭제*/
	while (pWalker) {
		pPre = pWalker;
		pWalker = pWalker->GetNext();
		delete pPre;
	}
}

void Queue::SetSize(int n) {
	m_Size = n;
}
bool Queue::IsEmpty() {
	if (m_NumElement == 0)
		return true;
	else
		return false;
}
bool Queue::IsFull() {
	if (m_NumElement == m_Size)
		return true;
	else
		return false;
}
bool Queue::Push(Node* pNode) {
	if (!this->IsFull()) { // Queue의 size가 남아있을 때
		if (m_NumElement == 0) { // Queue에 노드가 하나도 없을 때
			m_pHead = pNode;
			m_pTail = pNode;
		}
		else { // Queue에 노드가 존재할 때
			m_pTail->SetNext(pNode);
			m_pTail = pNode;
		}
		m_NumElement++; // 개수 1 증가
		return true;
	}
	else // 이미 Queue가 가득 차 있을 때
		return false;
}
Node* Queue::Pop() {
	Node* tmp = m_pHead;

	if (!this->IsEmpty()) { // Queue에 노드가 존재할 때
		m_pHead = m_pHead->GetNext();
		if (m_NumElement == 1) // 노드 개수가 1이었다면
			m_pTail == nullptr; // pTail도 null이어야 함
		m_NumElement--; // 개수 1 감소
	}
	return tmp;
}
void Queue::PrintQueue() { // 노드를 모두 출력
	Node* pWalker = m_pHead;
	for (int i = 0; i < m_NumElement && i < m_Size; i++) {
		cout << pWalker->GetData() << " ";
		pWalker = pWalker->GetNext();
	}
}