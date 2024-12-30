#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>

using namespace std;

class Student {
private:
	char* name;
	int age;
	char* class_name;
public:
	Student(char* name, int age, char* class_name);
	~Student();
	void print() {
		cout << "Name: " << name << endl;
		cout << "Age: " << age << endl;
		cout << "Class: " << class_name << endl;
		cout << "----------" << endl;
	}
	char* getName();
	char* getClass_name();
};

class School {
private:
	class Student* student_list[100];
	int size = 0;
public:
	void new_student(char* name, int age, char* class_name);
	void sort_by_name();
	void print_all();
	void print_class(char* class_name);
};