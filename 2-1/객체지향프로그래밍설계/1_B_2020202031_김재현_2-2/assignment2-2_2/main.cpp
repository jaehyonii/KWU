#include<iostream>
#include"A.h"

int main() {
	double num;
	/*객체의 생성자에 전달할 실수 입력받기*/
	cin >> num;

	A obj(num); //객체 생성과 동시에 생성자 실행

	return 0; // main 종료와 동시에 소멸자 실행
}