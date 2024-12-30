#define _CRT_SECURE_NO_WARNINGS
#include "Person.h"

/* class Person*/
Person::Person() {
	age = 0;
}
Person::~Person() {}

int Person::getAge() {
	return age;
}
char* Person::getName() {
	return name;
}
void Person::setAge(int age) {
	this->age = age;
}
void Person::setName(char* name) {
	strncpy(this->name, name, 31);
	this->name[31] = 0; // null문자 삽입
}


/* class Professor*/
Professor::Professor(){}
Professor::~Professor(){}

char* Professor::getProfessorNum() {
	return professorNum;
}
char* Professor::getMajor() {
	return major;
}

void Professor::setProfessorNum(char* professorNum) {
	strncpy(this->professorNum, professorNum, 10);
	this->professorNum[10] = 0; // null문자 삽입
}
void Professor::setMajor(char* major) {
	strncpy(this->major, major, 31);
	this->major[31] = 0; // null문자 삽입
}

void Professor::Say() {
	cout << "I'm a professor of KW University." << endl;
	cout << "My name is " << name << "." << endl;
	cout << "I'm " << age << " years old" << endl;
	cout << "My Professor Number is " << professorNum << endl;
	cout << "I'm majoring in " << major << "." << endl;
}



/* class Student*/
Student::Student(){}
Student::~Student(){}

char* Student::getStudentNum() {
	return studentNum;
}
char* Student::getMajor() {
	return major;
}
int Student::getSchooYear() {
	return schoolYear;
}

void Student::setStudentNum(char* studentNum) {
	strncpy(this->studentNum, studentNum, 10);
	this->studentNum[10] = 0; // null문자 삽입
}
void Student::setMajor(char* major) {
	strncpy(this->major, major, 31);
	this->major[31] = 0; // null문자 삽입
}
void Student::setSchoolYear(int year) {
	schoolYear = year;
}

void Student::Say() {
	cout << "I'm a student of KW University." << endl;
	cout << "My name is " << name << "." << endl;
	/* 학년에 따라서 불리는 호칭이 다 다르므로 각 경우에 따라 출력을 달리 해준다.*/
	switch (schoolYear) {
	case 1:
		cout << "I'm " << age << " years old and I'm a Fresh man." << endl;
		break;
	case 2:
		cout << "I'm " << age << " years old and I'm a Sophomore." << endl;
		break;
	case 3:
		cout << "I'm " << age << " years old and I'm a Junior." << endl;
		break;
	case 4:
		cout << "I'm " << age << " years old and I'm a Senior." << endl;
		break;
	default:
		cout << "I'm " << age << " years old and I'm a Worker." << endl;
	}
	cout << "My Professor Number is " << studentNum << endl;
	cout << "I'm majoring in " << major << "." << endl;
}