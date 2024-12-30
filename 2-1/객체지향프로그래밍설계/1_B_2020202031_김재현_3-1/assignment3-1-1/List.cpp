#include "List.h"
#include <iostream>

Node::Node() {
	pNext = nullptr;
	data = nullptr;
}
Node::Node(const char* str) {
	/* ��� ������ �� data ������ �����ϴ� ������*/
	int len = strlen(str);
	data = new char[len + 1];
	strcpy(data, str);

	pNext = nullptr;
}
Node::~Node() {
	delete[]data;
}

void Node::setData(char* str) {
	/* �Ű������� ���� �����ڸ� ȣ������ �� data�� ������ִ� �޼ҵ�*/
	int len = strlen(str);
	data = new char[len + 1];
	strcpy(data, str);
}
void Node::setNext(Node* pNext) {
	this->pNext = pNext;
}
char* Node::getData() {
	return data;
}
Node* Node::getNext() {
	return pNext;
}

void bubbleSort(Node** pHead, int size) {
	for (int i = size - 1; i > 0; i--) {
		Node* pCurrent = *pHead;
		Node* pPre = nullptr;
		Node* pNext = (*pHead)->getNext();
		
		for (int j = 0; j < i; j++) {
			char* str1 = pCurrent->getData();
			char* str2 = pNext->getData();
			if (compare(str1, str2))
			{
				/* ó�� �� ��带 switching �� ��*/
				if (pCurrent == *pHead) {
					pCurrent->setNext(pNext->getNext());
					pNext->setNext(pCurrent);
					*pHead = pNext;
				}
				/* �߰� �� ��带 switching �� ��*/
				else {
					pPre->setNext(pNext);
					pCurrent->setNext(pNext->getNext());
					pNext->setNext(pCurrent);					
				}
				/* switching�� �Ϸ�� ��, pPre, pNext, pCurrent ������ �����ǹǷ� pPre�� pNext�� ��ġ�� ����*/
				pPre = pNext;
				pNext = pCurrent->getNext();
				continue;
			}
			/* switching�� ���� ������ pPre, pCurrent, pNext�� ���� ��带 �����ϵ��� ��ġ ����*/
			pPre = pCurrent;
			pCurrent = pNext;
			pNext = pNext->getNext();
		}
		
	}
}


/* str1�� str2���� ũ�� true ������ false (��ҹ��� ���� X)*/
bool compare(char* str1, char* str2) {
	while (*str1 || *str2) {
		char ch1 = *str1;
		char ch2 = *str2;
		
		/* �ҹ��ڷ� ��ȯ*/
		if ('A' <= ch1 && ch1 <= 'Z')
			ch1 += 32;
		if ('A' <= ch2 && ch2 <= 'Z')
			ch2 += 32;
		
		/* str1 > str2�� true*/
		if (ch1 < ch2) {
			return false;
		}
		else if (ch1 > ch2) {
			return true;
		}
		
		str1++;
		str2++;
	}
	return false; // str1, str2 ũ�Ⱑ ������ false
}

/* p1, p2�� ���ĺ� ������������ ���ĵǾ� �ִٴ� �����Ͽ� ���*/
void Merge_List(Node* p1, Node* p2, Node* p3) {
	/* p3�� �ʱ���´� data�� ���� ������ ����̱� ������ ���� data�� set���ش�.*/
	if (compare(p1->getData(), p2->getData())) { // p1's data > p2's data
		p3->setData(p2->getData());
		Node* tmp = p2->getNext();
		delete p2; //p2�� data������ p3�� ���������Ƿ� ��� ����
		p2 = tmp;
	}
	else { // p1's data <= p2's data
		p3->setData(p1->getData());
		Node* tmp = p1->getNext();
		delete p1; //p1�� data������ p3�� ���������Ƿ� ��� ����
		p1 = tmp;
	}
	
	/*  p1, p2�� �����ϴ� �� p1, p2�� ũ�⸦ ���Ͽ� ���� ���� p3�� �̵���Ų��.*/
	Node* w3 = p3;
	while (p1 && p2) {
		if (compare(p1->getData(), p2->getData())) { // p1's data > p2's data
			w3->setNext(p2);
			w3 = w3->getNext();
			p2 = p2->getNext();
		}
		else { // p1's data <= p2's data
			w3->setNext(p1);
			w3 = w3->getNext();
			p1 = p1->getNext();
		}
	}
	/* p1�� �����ִٸ� p3�� ���� �̵�*/
	while (p1) {
		w3->setNext(p1);
		w3 = w3->getNext();
		p1 = p1->getNext();
	}
	/* p2�� �����ִٸ� p3�� ���� �̵�*/
	while (p2) {
		w3->setNext(p2);
		w3 = w3->getNext();
		p2 = p2->getNext();
	}
}