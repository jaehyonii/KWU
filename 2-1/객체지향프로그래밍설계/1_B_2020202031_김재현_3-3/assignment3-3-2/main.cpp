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
	cin.ignore(); // cin���� age�� �Է� ���� �� ���ۿ� �����ִ� ������ �����ش�.
	cout << "ProfessorNum: "; cin.getline(num, 11);
	cout << "major: "; cin.getline(major, 32);
	
	/* �Է� ���� ������ pro��ü�� ����*/
	pro.setName(name);
	pro.setAge(age);
	pro.setProfessorNum(num);
	pro.setMajor(major);
	
	
	Student stu;
	cout << "\n==========Insert Student Info==========\n";
	cout << "name: "; cin.getline(name, 32);
	cout << "age: "; cin >> age;
	cin.ignore(); // cin���� age�� �Է� ���� �� ���ۿ� �����ִ� ������ �����ش�.
	cout << "StudentNum: "; cin.getline(num, 11);
	cout << "major: "; cin.getline(major, 32);
	cout << "Grade: "; cin >> schoolyear;

	/* �Է� ���� ������ stu��ü�� ����*/
	stu.setName(name);
	stu.setAge(age);
	stu.setStudentNum(num);
	stu.setMajor(major);
	stu.setSchoolYear(schoolyear);
	
	cout << "\n==========Professor Info==========\n";
	pro.Say(); // ���� ���� ���

	cout << "\n==========Student Info==========\n";
	stu.Say(); // �л� ���� ���
}