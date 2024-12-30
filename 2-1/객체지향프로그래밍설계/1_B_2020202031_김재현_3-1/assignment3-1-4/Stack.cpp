#include "Stack.h"

/* Node class*/
Node::Node() {
	m_pNext = nullptr;
	m_Data = 0;
}
Node::~Node() {}

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
Stack::Stack() {
	m_pHead = nullptr;
	m_Size = 0;
	m_NumElement = 0;
}
Stack::~Stack() {
	Node* pWalker = m_pHead;
	Node* pPre;
	/* Queue ��ü�� ��� ������ ����*/
	while (pWalker) {
		pPre = pWalker;
		pWalker = pWalker->GetNext();
		delete pPre;
	}
}

void Stack::SetSize(int n) {
	m_Size = n;
}
bool Stack::IsEmpty() {
	if (m_NumElement == 0)
		return true;
	else
		return false;
}
bool Stack::IsFull() {
	if (m_NumElement == m_Size)
		return true;
	else
		return false;
}
bool Stack::Push(Node* pNode) {
	if (!this->IsFull()) { // Queue�� size�� �������� ��
		pNode->SetNext(m_pHead);
		m_pHead = pNode;
		m_NumElement++;

		return true;
	}
	else // �̹� Queue�� ���� �� ���� ��
		return false;
}
Node* Stack::Pop() {
	Node* tmp = m_pHead;

	if (!this->IsEmpty()) { // Queue�� ��尡 ������ ��
		m_pHead = m_pHead->GetNext();
		m_NumElement--; // ���� 1 ����
	}
	return tmp;
}
void Stack::PrintStack() { // ��带 ��� ���
	Node* pWalker = m_pHead;
	for (int i = 0; i < m_NumElement && i < m_Size; i++) {
		cout << pWalker->GetData() << " ";
		pWalker = pWalker->GetNext();
	}
}