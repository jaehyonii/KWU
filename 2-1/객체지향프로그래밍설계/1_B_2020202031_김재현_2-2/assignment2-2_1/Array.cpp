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
	/* ��Ͱ� ����� ������ ����ִ� spt�� �ǳʶٱ� ����*/
	while (**spt == NULL)
		spt++;
	/* spt�� "["�� �Ʒ��� ��带 Ȯ��*/
	if (!strcmp(*spt, "[")) {
		list->addDown();
		spt++;
		makeList(list->getDown());
	}

	/* spt�� ������ �ε����� ����Ű�� ���� �� �߸� �� ������ ����*/
	if (*spt == NULL)
		return;

	/* spt�� "]"�� ��� ����*/
	if (!strcmp(*spt, "]")) {
		spt++;
		/* spt�� ������ �ε����� ����Ű�� ���� �� �߸� �� ������ ����*/
		if (*spt == NULL)
			return;
		/* ��� ���� �� spt�� ����ִ� �ε����� �����ϴ� ���� ����*/
		while (**spt == NULL)
			spt++;
		return;
	}
	/* spt�� �Ϲ����� string�� ���*/
	else
		/*�Ʒ���尡 ������*/
	{
		/* ������ ��� Ż�� �� ���ʿ��� ������ ����*/
		if (*spt == NULL)
			return;

		if (!list->getDown())
		{
			list->addStr(*spt); // �����忡 ���ڿ��� ����
			spt++;
			list->addNext(); // ������� ����
			makeList(list->getNext()); // �������� ���
		}
		/* �Ʒ���尡 ������ ������带 �����ϰ� ������ ���� ���*/
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
	/* �Է��� "arr"�� ��*/
	if (!strcmp(condition, "arr"))
		printArray(list);
	/* �Է��� arr[..........] �� ��*/
	else if (!strncmp(condition, "arr", 3)) {
		for (int i = 4; condition[i];) {
			ptr = list->getDown();
			/* ���ڰ� ������ �ε����� �Ǵ�*/
			if (condition[i] < '0' || '9' < condition[i]) {
				int num = atoi(condition + i); // ���ڷ� �� ���ڸ� ������ ����
				/* �Էµ� ���ڸ�ŭ�� �ε����� ����*/
				for (int j = 0; j < num + 1; j++) {
					ptr = list->getNext();
				}
				/* �ѹ� ������ ���ڴ� �ǳʶ�*/
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

/* ��͸� ���� ������ ������ ó�������� delete���ش�.*/
void List_Manager::List_delete(List* list) {
	if (list->getDown())
		List_delete(list->getDown());
	if (list->getNext())
		List_delete(list->getNext());
	if (!list->getDown() && !list->getNext()) {
		delete list;
	}
}