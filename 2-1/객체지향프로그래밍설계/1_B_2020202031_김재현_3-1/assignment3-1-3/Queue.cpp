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
	/* Queue ��ü�� ��� ������ ����*/
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
	if (!this->IsFull()) { // Queue�� size�� �������� ��
		if (m_NumElement == 0) { // Queue�� ��尡 �ϳ��� ���� ��
			m_pHead = pNode;
			m_pTail = pNode;
		}
		else { // Queue�� ��尡 ������ ��
			m_pTail->SetNext(pNode);
			m_pTail = pNode;
		}
		m_NumElement++; // ���� 1 ����
		return true;
	}
	else // �̹� Queue�� ���� �� ���� ��
		return false;
}
Node* Queue::Pop() {
	Node* tmp = m_pHead;

	if (!this->IsEmpty()) { // Queue�� ��尡 ������ ��
		m_pHead = m_pHead->GetNext();
		if (m_NumElement == 1) // ��� ������ 1�̾��ٸ�
			m_pTail == nullptr; // pTail�� null�̾�� ��
		m_NumElement--; // ���� 1 ����
	}
	return tmp;
}
void Queue::PrintQueue() { // ��带 ��� ���
	Node* pWalker = m_pHead;
	for (int i = 0; i < m_NumElement && i < m_Size; i++) {
		cout << pWalker->GetData() << " ";
		pWalker = pWalker->GetNext();
	}
}