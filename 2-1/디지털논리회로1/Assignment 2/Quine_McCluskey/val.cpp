#include "val.h"
#include<iostream>
#include<cstring>

using namespace std;


val::val(const char* str) {
	/* �Է��� �״�� ����*/
	int num = strlen(str);
	this->str = new char[num + 1];
	strcpy(this->str, str);
	/* prime implicant���� ���θ� �ľ��ϴ� ��ȣ�� ���鿡 ����
	*: prime implicant, V: implicant*/
	(this->str)[1] = '*';
	
	/*1�� ���� ī��Ʈ*/
	count1 = 0;
	for (int i = 2; str[i]; i++)
		if(str[i]=='1')
			count1++;

	pNext = NULL;
}
/* minimum set of PIs�� �����ϱ� ���� ������*/
val::val(const char* str, int i) {
	/* �Է��� �״�� ����*/
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
/* pNext�� ���� ���(next)�� ����Ŵ*/
void val::setNext(val* next) {
	pNext = next;
}
val* val::getNext() {
	return pNext;
}



/*section ���� ��ũ�� ����Ʈ*/
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
/* ������ ����� pNext�� �� ��� �߰�*/
void val_manager::setData(const char* str) {
	current = new val(str);

	/* ����Ʈ�� ��������� �� ��带 ä���ְ� head�� tail�� ä���ش�.*/
	if (head == NULL) {
		head = current;
		tail = current;
	}
	/* ����Ʈ�� �̹� ��尡 �����Ѵٸ� ������ ����� pNext�� �� ��带 �߰����ְ� tail�� ����*/
	else {
		tail->setNext(current);
		tail = current;
	}
	valNum++; // ���� ����
}
/* �߰� ���� ���� ���� �������� �����ϱ� ���� setData �����ε�*/
void val_manager::setData(const char* str, int i) {
	current = new val(str, 0);

	/* ����Ʈ�� ��������� �� ��带 ä���ְ� head�� tail�� ä���ش�.*/
	if (head == NULL) {
		head = current;
		tail = current;
	}
	/* ����Ʈ�� �̹� ��尡 �����Ѵٸ� ������ ����� pNext�� �� ��带 �߰����ְ� tail�� ����*/
	else {
		tail->setNext(current);
		tail = current;
	}
	valNum++; // ���� ����
}
/*1�� ������ ���� ����*/
void val_manager::sortBy1() {
	/* ��尡 ������� ��*/
	if (head == NULL) {
		return;
	}
	
	/*����� ������ 1���� �� ���� X*/
	if (head == tail)
		return;
	
	/*BubbleSort*/
	for (int i = valNum - 1; i > 0; i--) {
		/* �� �� �� ���� ���� ���� ��*/
		current = head->getNext();
		if (head->getCount1() > current->getCount1()) {
			head->setNext(current->getNext());
			current->setNext(head);
			head = current;
		}

		/* �� ��° ������ ���� ���� ��*/
		val* prev = head;
		current = prev->getNext();
		val* next = current->getNext();
		for (int j = 1; j < i; j++) {
			if (current->getCount1() > next->getCount1()) {
				prev->setNext(next);
				current->setNext(next->getNext());
				next->setNext(current);
				/* sort �������� next�� current�� �ڸ��� �ٲ�� ���󺹱� ��Ű�� ����*/
				val* tmp = next;
				next = current;
				current = tmp;
			}
			/* ��� ���� �����͵��� ��ĭ�� �ű��*/
			prev = current;
			current = next;
			next = next->getNext();
		}
	}
	/* tail ��ġ ã��(������ ��� ã��)*/
	current = head;
	while (current->getNext() != NULL)
		current = current->getNext();
	tail = current;
	deleteSame();
}
/*���ĵ� �Է¿��� �ߺ��� �͵��� ����*/
void val_manager::deleteSame() {
	if (head == NULL || head==tail) {
		return;
	}

	current = head;
	while (current != tail && current != NULL) {
		val* pre = current;
		val* target = current->getNext();
		/* 1�� ���� �����ͳ��� ��*/
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
	/* tail ��ġ ã��(������ ��� ã��)*/
	current = head;
	while (current->getNext() != NULL)
		current = current->getNext();
	tail = current;
}

bool val_manager::combining(val_manager& manager) {
	/* ���� column�� ������ �Ѱ��ų� ������� ��*/
	if (manager.head == manager.tail || manager.head == NULL)
		return false;

	manager.current = manager.head;
	char* copy = new char[strlen(manager.current->getStr()) + 1];
	while (manager.current != manager.tail) {
		val* target = manager.current->getNext();
		/* 1�� ������ n, n+1�̰ų� ���� �ͳ��� ��*/
		while (target->getCount1() - manager.current->getCount1() <= 1) {
			
			char* str1 = manager.current->getStr();
			char* str2 = target->getStr();
			int diff = 0;// �� ������ ���� �ٸ� ��Ʈ�� ������ ����
			int idx = 0; // �� ������ ���� �ٸ� ��Ʈ�� ������ �ε���
			for (int i = 2; str1[i]; i++) {
				if (str1[i] != str2[i])	{
					diff++;
					idx = i;
				}
			}
			/* �� ������ hamming distance�� 1�̶��*/
			if (diff == 1) {
				copy[0] = 'd';
				copy[1] = '*';
				strcpy(copy + 2, str1 + 2);
				copy[idx] = '-';
				setData(copy);
				
				/* ������ ������ üũǥ�� ���ֱ�*/
				str1[1] = 'V';
				str2[1] = 'V';
			}

			
			/* targer�� ������ ��带 ����Ű�� �Ǹ� ���� �ݺ����� �������� �ʴ� ��带 �����ϰ� ��*/
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