#include "Person.h"

int main()
{
	char name[32];
	int age;
	char num[11];
	char major[32];
	int schoolyear;
	
	Professor pro;
	cout << "==========Insert Professor Info==========\n";
	cout << "name: "; cin.getline(name, 32);
	cout << "age: "; cin >> age;
	cin.ignore(); // cin으로 age를 입력 받은 후 버퍼에 남아있는 개행을 없애준다.
	cout << "ProfessorNum: "; cin.getline(num, 11);
	cout << "major: "; cin.getline(major, 32);
	
	/* 입력 받은 정보를 pro객체에 저장*/
	pro.setName(name);
	pro.setAge(age);
	pro.setProfessorNum(num);
	pro.setMajor(major);
	
	
	Student stu;
	cout << "\n==========Insert Student Info==========\n";
	cout << "name: "; cin.getline(name, 32);
	cout << "age: "; cin >> age;
	cin.ignore(); // cin으로 age를 입력 받은 후 버퍼에 남아있는 개행을 없애준다.
	cout << "StudentNum: "; cin.getline(num, 11);
	cout << "major: "; cin.getline(major, 32);
	cout << "Grade: "; cin >> schoolyear;

	/* 입력 받은 정보를 stu객체에 저장*/
	stu.setName(name);
	stu.setAge(age);
	stu.setStudentNum(num);
	stu.setMajor(major);
	stu.setSchoolYear(schoolyear);
	
	cout << "\n==========Professor Info==========\n";
	pro.Say(); // 교수 정보 출력

	cout << "\n==========Student Info==========\n";
	stu.Say(); // 학생 정보 출력
}