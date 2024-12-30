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
		/* current�� ����Ʈ Head�� ��ġ�ؾ� �� ��*/
		if (p->getValue() < current->getValue()) {
			cpre->setNext(current->getNext()); // current��� ����Ʈ���� ����
			current->setNext(p);
			p = current;

			current = cpre;
		}
		else {
			Node* walker = p->getNext();
			Node* wpre = p;
			while (walker != current) {
				if (walker->getValue() < current->getValue()) {
					cpre->setNext(current->getNext()); // current��� ����Ʈ���� ����

					/* current�� walker�տ� ����*/
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
		/* M��° ��带 Ž��*/
		for (int i = 0; i < M; i++)
			walker = walker->getNext();
		
		if (walker->getValue() > n) // n�� M��° ��庸�� ���� ��
			L = M + 1;
		else if (walker->getValue() < n) // n�� M��° ��庸�� Ŭ ��
			H = M - 1;
		else
			return walker;
		
	}
	cout << "Search fail" << endl;
	
	/* Search �Ϸ��� ������ ���� ��尡 �������� ���� ��*/
	if (H < 0) // Search �Ϸ��� ������ �����ϴ� �ּҳ�庸�� ���� ��
		return p; // ù��° ��� ��ȯ
	else if (L > 15) { // Search �Ϸ��� ������ �����ϴ� �ִ��庸�� Ŭ ��
		walker = p;
		/* ������ ��� Ž��*/
		for (int i = 0; i < 15; i++)
			walker = walker->getNext();
		return walker; // ��ȯ
	}
	/* ����Ʈ ���� ���� �������� �ش� ������ ������ ��尡 �������� ���� ��*/
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

	cout << "���� �Է�: "; cin >> num;
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
	
	cout << "ã���� �ϴ� ���� �Է�: "; cin >> num;
	Node* found = Binary_Search(Head, num);
	cout << found->getValue() << endl;
}