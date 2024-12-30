#pragma once
#include<iostream>

using namespace std;

class Person
{
protected:
	int age;
	char name[32];

public:
	Person();
	~Person();
	virtual void Say() = 0;

	int getAge();
	char* getName();
	void setAge(int age);
	void setName(char* name);
};

class Professor :public Person
{
protected:
	char professorNum[11];
	char major[32];

public:
	Professor();
	~Professor();
	char* getProfessorNum();
	char* getMajor();
	void setProfessorNum(char* professorNum);
	void setMajor(char* major);
	void Say();
};

class Student :public Person
{
protected:
	char studentNum[11];
	char major[32];
	int schoolYear;

public:
	Student();
	~Student();
	char* getStudentNum();
	char* getMajor();
	int getSchooYear();
	void setStudentNum(char* studentNum);
	void setMajor(char* major);
	void setSchoolYear(int year);
	void Say();
};