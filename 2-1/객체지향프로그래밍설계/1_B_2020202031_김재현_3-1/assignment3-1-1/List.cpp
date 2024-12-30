#include "List.h"
#include <iostream>

Node::Node() {
	pNext = nullptr;
	data = nullptr;
}
Node::Node(const char* str) {
	/* 노드 생성할 때 data 정보를 포함하는 생성자*/
	int len = strlen(str);
	data = new char[len + 1];
	strcpy(data, str);

	pNext = nullptr;
}
Node::~Node() {
	delete[]data;
}

void Node::setData(char* str) {
	/* 매개변수가 없는 생성자를 호출했을 때 data를 만들어주는 메소드*/
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
				/* 처음 두 노드를 switching 할 때*/
				if (pCurrent == *pHead) {
					pCurrent->setNext(pNext->getNext());
					pNext->setNext(pCurrent);
					*pHead = pNext;
				}
				/* 중간 두 노드를 switching 할 때*/
				else {
					pPre->setNext(pNext);
					pCurrent->setNext(pNext->getNext());
					pNext->setNext(pCurrent);					
				}
				/* switching이 완료된 후, pPre, pNext, pCurrent 순으로 나열되므로 pPre와 pNext의 위치를 수정*/
				pPre = pNext;
				pNext = pCurrent->getNext();
				continue;
			}
			/* switching을 하지 않으면 pPre, pCurrent, pNext가 다음 노드를 참조하도록 위치 수정*/
			pPre = pCurrent;
			pCurrent = pNext;
			pNext = pNext->getNext();
		}
		
	}
}


/* str1이 str2보다 크면 true 작으면 false (대소문자 구분 X)*/
bool compare(char* str1, char* str2) {
	while (*str1 || *str2) {
		char ch1 = *str1;
		char ch2 = *str2;
		
		/* 소문자로 변환*/
		if ('A' <= ch1 && ch1 <= 'Z')
			ch1 += 32;
		if ('A' <= ch2 && ch2 <= 'Z')
			ch2 += 32;
		
		/* str1 > str2면 true*/
		if (ch1 < ch2) {
			return false;
		}
		else if (ch1 > ch2) {
			return true;
		}
		
		str1++;
		str2++;
	}
	return false; // str1, str2 크기가 같으면 false
}

/* p1, p2가 알파벳 오름차순으로 정렬되어 있다는 가정하에 사용*/
void Merge_List(Node* p1, Node* p2, Node* p3) {
	/* p3의 초기상태는 data가 없는 상태의 노드이기 때문에 먼저 data를 set해준다.*/
	if (compare(p1->getData(), p2->getData())) { // p1's data > p2's data
		p3->setData(p2->getData());
		Node* tmp = p2->getNext();
		delete p2; //p2의 data정보를 p3에 저장했으므로 노드 삭제
		p2 = tmp;
	}
	else { // p1's data <= p2's data
		p3->setData(p1->getData());
		Node* tmp = p1->getNext();
		delete p1; //p1의 data정보를 p3에 저장했으므로 노드 삭제
		p1 = tmp;
	}
	
	/*  p1, p2가 존재하는 한 p1, p2의 크기를 비교하여 작은 쪽을 p3로 이동시킨다.*/
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
	/* p1이 남아있다면 p3로 전부 이동*/
	while (p1) {
		w3->setNext(p1);
		w3 = w3->getNext();
		p1 = p1->getNext();
	}
	/* p2가 남아있다면 p3로 전부 이동*/
	while (p2) {
		w3->setNext(p2);
		w3 = w3->getNext();
		p2 = p2->getNext();
	}
}