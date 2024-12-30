#include "val.h"
#include<iostream>
#include<cstring>

using namespace std;


val::val(const char* str) {
	/* 입력을 그대로 저장*/
	int num = strlen(str);
	this->str = new char[num + 1];
	strcpy(this->str, str);
	/* prime implicant인지 여부를 파악하는 기호를 공백에 저장
	*: prime implicant, V: implicant*/
	(this->str)[1] = '*';
	
	/*1의 개수 카운트*/
	count1 = 0;
	for (int i = 2; str[i]; i++)
		if(str[i]=='1')
			count1++;

	pNext = NULL;
}
/* minimum set of PIs를 저장하기 위한 생성자*/
val::val(const char* str, int i) {
	/* 입력을 그대로 저장*/
	int num = strlen(str);
	this->str = new char[num + 1];
	strcpy(this->str, str);

	pNext = NULL;
}
val::~val() {
	delete[]str;
}
char* val::getStr() {
	return str;
}
int val::getCount1() {
	return count1;
}
/* pNext가 다음 노드(next)를 가리킴*/
void val::setNext(val* next) {
	pNext = next;
}
val* val::getNext() {
	return pNext;
}



/*section 별로 링크드 리스트*/
val_manager::val_manager() {
	head = NULL;
	tail = NULL;
	current = NULL;
	valNum = 0;
}
val_manager::~val_manager() {
	current = head;
	while (current) {
		current = head->getNext();
		delete head;
		head = current;
	}
}

bool::val_manager::isEmpty() {
	if (head == NULL)
		return true;
	else
		return false;
}
/* 마지막 노드의 pNext에 새 노드 추가*/
void val_manager::setData(const char* str) {
	current = new val(str);

	/* 리스트가 비어있으면 새 노드를 채워주고 head와 tail로 채워준다.*/
	if (head == NULL) {
		head = current;
		tail = current;
	}
	/* 리스트에 이미 노드가 존재한다면 마지막 노드의 pNext에 새 노드를 추가해주고 tail로 참조*/
	else {
		tail->setNext(current);
		tail = current;
	}
	valNum++; // 개수 증가
}
/* 추가 정보 없이 순수 변수만을 저장하기 위한 setData 오버로딩*/
void val_manager::setData(const char* str, int i) {
	current = new val(str, 0);

	/* 리스트가 비어있으면 새 노드를 채워주고 head와 tail로 채워준다.*/
	if (head == NULL) {
		head = current;
		tail = current;
	}
	/* 리스트에 이미 노드가 존재한다면 마지막 노드의 pNext에 새 노드를 추가해주고 tail로 참조*/
	else {
		tail->setNext(current);
		tail = current;
	}
	valNum++; // 개수 증가
}
/*1의 개수에 따라 정렬*/
void val_manager::sortBy1() {
	/* 노드가 비어있을 때*/
	if (head == NULL) {
		return;
	}
	
	/*노드의 개수가 1개일 때 실행 X*/
	if (head == tail)
		return;
	
	/*BubbleSort*/
	for (int i = valNum - 1; i > 0; i--) {
		/* 맨 앞 두 노드는 먼저 따로 비교*/
		current = head->getNext();
		if (head->getCount1() > current->getCount1()) {
			head->setNext(current->getNext());
			current->setNext(head);
			head = current;
		}

		/* 두 번째 노드부터 다음 노드와 비교*/
		val* prev = head;
		current = prev->getNext();
		val* next = current->getNext();
		for (int j = 1; j < i; j++) {
			if (current->getCount1() > next->getCount1()) {
				prev->setNext(next);
				current->setNext(next->getNext());
				next->setNext(current);
				/* sort 과정에서 next와 current의 자리가 바뀌어 원상복구 시키는 과정*/
				val* tmp = next;
				next = current;
				current = tmp;
			}
			/* 노드 참조 포인터들을 한칸씩 옮기기*/
			prev = current;
			current = next;
			next = next->getNext();
		}
	}
	/* tail 위치 찾기(마지막 노드 찾기)*/
	current = head;
	while (current->getNext() != NULL)
		current = current->getNext();
	tail = current;
	deleteSame();
}
/*정렬된 입력에서 중복된 것들을 삭제*/
void val_manager::deleteSame() {
	if (head == NULL || head==tail) {
		return;
	}

	current = head;
	while (current != tail && current != NULL) {
		val* pre = current;
		val* target = current->getNext();
		/* 1의 개수 같은것끼리 비교*/
		while (current->getCount1()==target->getCount1()) {
			if (!strcmp(current->getStr(), target->getStr())) {
				pre->setNext(target->getNext());
				delete target;
				valNum--;
			}
			else {
				pre = target;
			}
			target = pre->getNext();
			if (target == NULL)
				break;
		}
		current = current->getNext();
	}
	/* tail 위치 찾기(마지막 노드 찾기)*/
	current = head;
	while (current->getNext() != NULL)
		current = current->getNext();
	tail = current;
}

bool val_manager::combining(val_manager& manager) {
	/* 이전 column의 변수가 한개거나 비어있을 때*/
	if (manager.head == manager.tail || manager.head == NULL)
		return false;

	manager.current = manager.head;
	char* copy = new char[strlen(manager.current->getStr()) + 1];
	while (manager.current != manager.tail) {
		val* target = manager.current->getNext();
		/* 1의 개수가 n, n+1이거나 같은 것끼리 비교*/
		while (target->getCount1() - manager.current->getCount1() <= 1) {
			
			char* str1 = manager.current->getStr();
			char* str2 = target->getStr();
			int diff = 0;// 두 변수가 서로 다른 비트를 가지는 개수
			int idx = 0; // 두 변수가 서로 다른 비트를 가지는 인덱스
			for (int i = 2; str1[i]; i++) {
				if (str1[i] != str2[i])	{
					diff++;
					idx = i;
				}
			}
			/* 두 변수의 hamming distance가 1이라면*/
			if (diff == 1) {
				copy[0] = 'd';
				copy[1] = '*';
				strcpy(copy + 2, str1 + 2);
				copy[idx] = '-';
				setData(copy);
				
				/* 참조한 변수들 체크표시 해주기*/
				str1[1] = 'V';
				str2[1] = 'V';
			}

			
			/* targer이 마지막 노드를 가리키게 되면 다음 반복에서 존재하지 않는 노드를 참조하게 됨*/
			if (target == manager.tail)
				break;
			target = target->getNext();
		}
		manager.current = manager.current->getNext();
	}
	delete[] copy;
	return true;
}

void val_manager::printall() {
	if (head == NULL) {
		return;
	}

	current = head;
	while (current) {
		cout << current->getStr() << endl;
		current = current->getNext();
	}
}