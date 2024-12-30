#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class val {
private:
	char* str; // input
	int count1; // 1�� ����
	val* pNext;
public:
	val(const char* str); // str�� this->str�� ����
	val(const char* str, int); // str�� this->str�� ����
	~val();
	char* getStr(); // str �ּ� ��ȯ
	int getCount1(); // 1�� ���� ��ȯ
	void setNext(val* next); // pNext�� next�� ����Ŵ
	val* getNext(); // pNext ��ȯ
};

class val_manager {
public:
	val* head;
	val* tail;
	val* current;
	int valNum; // ����� ����

public:
	val_manager();
	~val_manager();
	bool isEmpty();
	void setData(const char* str); // ���ο� ��� �߰�
	void setData(const char* str, int i); // ���ο� ��� �߰�
	void sortBy1(); // 1�� ������ ���� ����
	bool combining(val_manager& manager);
	void deleteSame(); // �ߺ��Ǵ� �Է��� delete.

	void printall(); // ����
};