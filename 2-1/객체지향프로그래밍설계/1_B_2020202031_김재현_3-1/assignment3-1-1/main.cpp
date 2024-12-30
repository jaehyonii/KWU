#include "List.h"
#include <iostream>
#include <string.h>

int main() {
	char arr[100];
	Node* p1 = nullptr; // Node의 Head를 저장
	int size1 = 0; // p1노드 개수
	Node* p2 = nullptr; // Node의 Head를 저장
	int size2 = 0; // p2 노드 개수
	Node* p3 = nullptr; // Node의 Head를 저장
	
	Node* pWalker;
	
	cout << "Input>>" << endl;
	
	/* Input List 1 */
	cout << "Input list 1: ";
	cin.getline(arr, 100);
	char* ptr;
	if (ptr = strtok(arr, " ")) {
		p1 = new Node(ptr); // Head 저장
		size1++;

		/* list 제작*/
		ptr = strtok(NULL, " ");
		pWalker = p1;
		while (ptr) {
			pWalker->setNext(new Node(ptr));
			pWalker = pWalker->getNext();
			ptr = strtok(NULL, " ");
			size1++;
		}
	}
	
	bubbleSort(&p1, size1); // 알파벳 오름차순으로 정렬
	

	/* Input List 2 */
	cout << "Input List 2: ";
	cin.getline(arr, 100);
	if (ptr = strtok(arr, " ")) {
		p2 = new Node(ptr); // Head 저장
		size2++;

		/* list 제작*/
		ptr = strtok(NULL, " ");
		pWalker = p2;
		while (ptr) {
			pWalker->setNext(new Node(ptr));
			pWalker = pWalker->getNext();
			ptr = strtok(NULL, " ");
			size2++;
		}
	}
	bubbleSort(&p2, size2); // 알파벳 오름차순으로 정렬
	
	p3 = new Node();
	Merge_List(p1, p2, p3); // 정렬된 p1, p2를 p3에 정렬
	
	/* p3를 출력함과 동시에 메모리 해제*/
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