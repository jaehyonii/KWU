#include "List.h"
#include <iostream>
#include <string.h>

int main() {
	char arr[100];
	Node* p1 = nullptr; // Node�� Head�� ����
	int size1 = 0; // p1��� ����
	Node* p2 = nullptr; // Node�� Head�� ����
	int size2 = 0; // p2 ��� ����
	Node* p3 = nullptr; // Node�� Head�� ����
	
	Node* pWalker;
	
	cout << "Input>>" << endl;
	
	/* Input List 1 */
	cout << "Input list 1: ";
	cin.getline(arr, 100);
	char* ptr;
	if (ptr = strtok(arr, " ")) {
		p1 = new Node(ptr); // Head ����
		size1++;

		/* list ����*/
		ptr = strtok(NULL, " ");
		pWalker = p1;
		while (ptr) {
			pWalker->setNext(new Node(ptr));
			pWalker = pWalker->getNext();
			ptr = strtok(NULL, " ");
			size1++;
		}
	}
	
	bubbleSort(&p1, size1); // ���ĺ� ������������ ����
	

	/* Input List 2 */
	cout << "Input List 2: ";
	cin.getline(arr, 100);
	if (ptr = strtok(arr, " ")) {
		p2 = new Node(ptr); // Head ����
		size2++;

		/* list ����*/
		ptr = strtok(NULL, " ");
		pWalker = p2;
		while (ptr) {
			pWalker->setNext(new Node(ptr));
			pWalker = pWalker->getNext();
			ptr = strtok(NULL, " ");
			size2++;
		}
	}
	bubbleSort(&p2, size2); // ���ĺ� ������������ ����
	
	p3 = new Node();
	Merge_List(p1, p2, p3); // ���ĵ� p1, p2�� p3�� ����
	
	/* p3�� ����԰� ���ÿ� �޸� ����*/
	while (p3) {
		pWalker = p3->getNext();
		cout << p3->getData() << " ";
		delete p3;
		p3 = pWalker;
	}
	cout << endl;
	
	return 0;
}

// Well done is better than well said
// Blaze with the fire that is never extinguished