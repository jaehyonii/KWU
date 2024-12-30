#include <iostream>

using namespace std;

class Node {
private:
	int value;
	Node* pNext;

public:
	Node() {
		value = 0;
		pNext = nullptr;
	}
	void setValue(int n) {
		value = n;
	}
	void setNext(Node* next) {
		pNext = next;
	}
	int getValue() {
		return value;
	}
	Node* getNext() {
		return pNext;
	}
};

void printList(Node* p) {
	Node* walker = p;

	while (walker) {
		cout << walker->getValue() << ", ";
		walker = walker->getNext();
	}
}

Node* insertionSort(Node* p) {
	/* insertion sort*/
	Node* current = p->getNext();
	Node* cpre = p;
	while (current) {
		/* current가 리스트 Head에 위치해야 할 때*/
		if (p->getValue() < current->getValue()) {
			cpre->setNext(current->getNext()); // current노드 리스트에서 제외
			current->setNext(p);
			p = current;

			current = cpre;
		}
		else {
			Node* walker = p->getNext();
			Node* wpre = p;
			while (walker != current) {
				if (walker->getValue() < current->getValue()) {
					cpre->setNext(current->getNext()); // current노드 리스트에서 제외

					/* current를 walker앞에 삽입*/
					wpre->setNext(current);
					current->setNext(walker);

					current = cpre;
					break;
				}
				wpre = walker;
				walker = walker->getNext();
			}
		}
		cpre = current;
		current = current->getNext();
	}
	
	return p;
}

Node* Binary_Search(Node* p, int n) {
	p = insertionSort(p);
	
	int L = 0;
	int H = 15;
	int M = (L + H) / 2;

	Node* walker;
	while (L <= H) {
		M = (L + H) / 2;
		walker = p;
		/* M번째 노드를 탐색*/
		for (int i = 0; i < M; i++)
			walker = walker->getNext();
		
		if (walker->getValue() > n) // n이 M번째 노드보다 작을 때
			L = M + 1;
		else if (walker->getValue() < n) // n이 M번째 노드보다 클 때
			H = M - 1;
		else
			return walker;
		
	}
	cout << "Search fail" << endl;
	
	/* Search 하려는 정수를 가진 노드가 존재하지 않을 때*/
	if (H < 0) // Search 하려는 정수가 존재하는 최소노드보다 작을 때
		return p; // 첫번째 노드 반환
	else if (L > 15) { // Search 하려는 정수가 존재하는 최대노드보다 클 때
		walker = p;
		/* 마지막 노드 탐색*/
		for (int i = 0; i < 15; i++)
			walker = walker->getNext();
		return walker; // 반환
	}
	/* 리스트 범위 내의 값이지만 해당 정수를 가지는 노드가 존재하지 않을 때*/
	else {
		walker = p;
		for (int i = 0; i < M - 1; i++)
			walker = walker->getNext();
		
		int diff1;
		int diff2;

		diff1 = walker->getValue() - n;
		diff2 = n - walker->getNext()->getValue();

		if (diff1 <= diff2)
			return walker;
		else
			return walker->getNext();
	}
}

int main()
{
	int num;

	cout << "정수 입력: "; cin >> num;
	Node* Head = new Node();
	Head->setValue(num);

	Node* pWalker = Head;
	for (int i = 0; i < 15; i++) {
		cin >> num;
		Node* tmp = new Node();
		tmp->setValue(num);
		pWalker->setNext(tmp);
		pWalker = tmp;
	}
	printList(Head);
	cout << endl;

	Head = insertionSort(Head);
	printList(Head);
	cout << endl;
	
	cout << "찾고자 하는 정수 입력: "; cin >> num;
	Node* found = Binary_Search(Head, num);
	cout << found->getValue() << endl;
}