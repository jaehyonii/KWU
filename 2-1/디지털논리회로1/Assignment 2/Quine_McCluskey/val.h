#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class val {
private:
	char* str; // input
	int count1; // 1의 개수
	val* pNext;
public:
	val(const char* str); // str을 this->str에 복사
	val(const char* str, int); // str을 this->str에 복사
	~val();
	char* getStr(); // str 주소 반환
	int getCount1(); // 1의 개수 반환
	void setNext(val* next); // pNext가 next를 가리킴
	val* getNext(); // pNext 반환
};

class val_manager {
public:
	val* head;
	val* tail;
	val* current;
	int valNum; // 노드의 개수

public:
	val_manager();
	~val_manager();
	bool isEmpty();
	void setData(const char* str); // 새로운 노드 추가
	void setData(const char* str, int i); // 새로운 노드 추가
	void sortBy1(); // 1의 개수에 따라 정렬
	bool combining(val_manager& manager);
	void deleteSame(); // 중복되는 입력을 delete.

	void printall(); // 실험
};