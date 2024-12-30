#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>

using namespace std;

class Employee 
{
private:
	char* name;
	int age;
	char* country;
	char* job;

public:
	Employee();
	Employee(char* name, int age, char* country, char* job);
	Employee(Employee& A);
	~Employee();
	bool isNameCorret(char* name);
	void print();
	void change(char* name, int age, char* country, char* job);
};