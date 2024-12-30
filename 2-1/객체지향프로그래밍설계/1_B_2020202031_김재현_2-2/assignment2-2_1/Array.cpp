#include "Array.h"

/* List*/
List::List()
	:pNext(NULL), pDown(NULL), str(NULL) {}

void List::addNext() {
	pNext = new List();
}

void List::addDown() {
	pDown = new List();
}

void List::addStr(const char* str) {
	this->str = new char[strlen(str) + 1];
	strcpy(this->str, str);
}

List* List::getNext() {
	return pNext;
}
List* List::getDown() {
	return pDown;
}
char* List::getStr() {
	return str;
}
void List::print() {
	cout << str;
}
List::~List() {
	delete[]str;
}

/* List Manager*/

List_Manager::List_Manager(char** spt) {
	this->spt = spt;
}

void List_Manager::makeList(List* list) {
	/* 재귀가 실행될 때마다 비어있는 spt를 건너뛰기 위함*/
	while (**spt == NULL)
		spt++;
	/* spt가 "["면 아래로 노드를 확장*/
	if (!strcmp(*spt, "[")) {
		list->addDown();
		spt++;
		makeList(list->getDown());
	}

	/* spt가 마지막 인덱스를 가리키고 있을 때 잘못 된 참조를 방지*/
	if (*spt == NULL)
		return;

	/* spt가 "]"면 재귀 종료*/
	if (!strcmp(*spt, "]")) {
		spt++;
		/* spt가 마지막 인덱스를 가리키고 있을 때 잘못 된 참조를 방지*/
		if (*spt == NULL)
			return;
		/* 재귀 종료 후 spt가 비어있는 인덱스를 참조하는 것을 방지*/
		while (**spt == NULL)
			spt++;
		return;
	}
	/* spt가 일반적인 string일 경우*/
	else
		/*아래노드가 없으면*/
	{
		/* 마지막 재귀 탈출 후 불필요한 실행을 방지*/
		if (*spt == NULL)
			return;

		if (!list->getDown())
		{
			list->addStr(*spt); // 현재노드에 문자열을 복사
			spt++;
			list->addNext(); // 다음노드 생성
			makeList(list->getNext()); // 다음노드로 재귀
		}
		/* 아래노드가 있으면 다음노드를 생성하고 생성한 노드로 재귀*/
		else
		{
			list->addNext();
			makeList(list->getNext());
		}
	}
}

void List_Manager::printArray(List* list) {
	if (list->getDown()) {
		cout << "[";
		printArray(list->getDown());
	}
	if (!list->getNext()) {
		cout << "]";
		return;
	}
	if (list->getStr()) {
		list->print();
		if (list->getNext()->getStr())
			cout << ", ";
	}
	if (list->getNext())
		printArray(list->getNext());
}

void List_Manager::print(List* list, const char* condition) {
	List* ptr = list;
	/* 입력이 "arr"일 때*/
	if (!strcmp(condition, "arr"))
		printArray(list);
	/* 입력이 arr[..........] 일 때*/
	else if (!strncmp(condition, "arr", 3)) {
		for (int i = 4; condition[i];) {
			ptr = list->getDown();
			/* 숫자가 나오면 인덱스로 판단*/
			if (condition[i] < '0' || '9' < condition[i]) {
				int num = atoi(condition + i); // 문자로 된 숫자를 정수로 저장
				/* 입력된 숫자만큼의 인덱스를 참조*/
				for (int j = 0; j < num + 1; j++) {
					ptr = list->getNext();
				}
				/* 한번 참조한 숫자는 건너뜀*/
				while ('0' <= condition[i] && condition[i] <= '9')
					i++;
			}
			i++;
		}
		if (!ptr->getDown())
			cout << ptr->getStr() << endl;
		else
			printArray(ptr);
	}
	else
		cout << "Invalid Input" << endl;
}

/* 재귀를 통해 마지막 노드부터 처음노드까지 delete해준다.*/
void List_Manager::List_delete(List* list) {
	if (list->getDown())
		List_delete(list->getDown());
	if (list->getNext())
		List_delete(list->getNext());
	if (!list->getDown() && !list->getNext()) {
		delete list;
	}
}