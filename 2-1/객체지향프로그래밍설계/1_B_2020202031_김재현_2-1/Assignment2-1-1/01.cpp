#include<iostream>
#include<string.h>

using namespace std;

void push(int stack[], int& topIndex, int num);
void pop(int stack[], int& topIndex);
void top(int stack[], int topIndex);
void print(int stack[], int topIndex);
void empty(int topIndex);

int main()
{
	int stack[128]; // 스택
	int topIndex = 0; // 스택의 top index
	char input[10]; // 입력받은 명령어를 저장할 배열
	int num; // 입력받은 정수를 저장할 변수
	/*무한 루프*/
	while (1) {
		cin >> input; // 명령어 입력
		
		/* push명령어 -> stack에 value 추가*/
		if (!strcmp(input, "push")) {
			cin >> num;
			push(stack, topIndex, num);
		}
		/* pop명령어 -> stack에서 top value 출력 및 제거*/
		else if (!strcmp(input, "pop")) {
			pop(stack, topIndex);
		}
		/* top명령어 -> stack에서 top value 출력*/
		else if (!strcmp(input, "top")) {
			top(stack, topIndex);
		}
		/* print명령어 -> stack의 bottom to top value 출력*/
		else if (!strcmp(input, "print")) {
			print(stack, topIndex);
		}
		/* empty명령어 -> stack내 value 유무에 따라 0 or 1*/
		else if (!strcmp(input, "empty")) {
			empty(topIndex);
		}
		/* exit명령어 -> 반복문 탈출*/
		else if (!strcmp(input, "exit"))
			break;
		/* 문제에서 제시한 명령어 외의 명령어가 입력될 시*/
		else
			cout << "Invalid Input!" << endl;
	}
	return 0;
}

void push(int stack[], int& topIndex, int num) {
	/* 스택이 가득 찼을 때 예외처리*/
	if (topIndex == 128) {
		cout << "stack is full" << endl;
		return;
	}

	stack[topIndex++] = num; // top에 value 추가하고 top 1증가
}

void pop(int stack[], int& topIndex) {
	/* stack이 비어있을 때 예외처리*/
	if (topIndex == 0) {
		cout << "There's No value" << endl;
		return;
	}
	cout << stack[topIndex - 1] << endl; // top value 출력
	stack[topIndex - 1] = 0; // top value를 0으로 초기화
	topIndex--; // top 1 감소
}

void top(int stack[], int topIndex) {
	/* stack이 비어있을 때 예외처리*/
	if (topIndex == 0) {
		cout << "There's No value" << endl;
		return;
	}
	cout << stack[topIndex - 1] << endl; // top value 출력
}

void print(int stack[], int topIndex) {
	/* stack이 비어있을 때 예외처리*/
	if (topIndex == 0) {
		cout << "There's No value" << endl;
		return;
	}
	/* stack의 모든 value 출력*/
	for (int i = 0; i < topIndex; i++)
		cout << stack[i] << " ";
	cout << endl;
}

void empty(int topIndex) {
	/* stack이 비어있으면 1*/
	if (topIndex == 0)
		cout << "1" << endl;
	/* stack이 채워져있으면 0*/
	else
		cout << "0" << endl;
}