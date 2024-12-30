#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Array.h"

char** split(const char* str, char ch1, char ch2);

int main() {
	char* input = new char[100001];
	cin.getline(input, 100001, '\n');

	char** spt = split(input, ',' , ' ');
	
	List* list = new List;
	List_Manager manager(spt);
	manager.makeList(list);

	/* 출력 조건 입력*/
	char condition[1024];
	while (1) {
		cin >> condition;
		
		if (strcmp(condition, "exit")){
			manager.print(list, condition);
			cout << endl;
		}
		else
			break;
	}

	delete[] input; // input 메모리 해제
	/* spt 메모리해제*/
	for (int i = 0; spt[i]; i++) {
		delete[]spt[i];
	}
	delete[]spt;
	manager.List_delete(list);
	return 0;
}

/* '[',']',string을 구분*/
char** split(const char* str, char ch1, char ch2) // 동적할당 해제 주의!
{
	char* copy = new char[100001];
	/*str을 copy에 복사*/
	int k = 0;
	for (k = 0; str[k] != 0; k++)
		copy[k] = str[k];
	copy[k] = 0;

	int num = 0;
	/*ch 개수 세주기, ch가 위치한 인덱스에 널문자 넣기*/
	for (int i = 0; copy[i] != 0; i++) {
		if (copy[i] == ch1 || copy[i] == ch2) {
			copy[i] = 0;
			num++;
		}
		/* 대괄호 개수도 num에 더해준다.*/
		if (copy[i] == '[' || copy[i] == ']')
			num++;
	}
	/* 'ch'로 구분되는 문자열의 개수+1만큼 할당 후 마지막에 널문자 대입*/
	char** arr = new char* [num + 2];
	arr[num + 1] = 0;

	/*arr 각 부분들에 구분된 문자열의 길이만큼 동적할당 후 문자열 복사*/
	char* ptr = copy;
	for (int i = 0; i < num + 1; i++) {
		/* '['가 붙어있는 문자열의 경우 '['먼저 동적할당 처리*/
		while (*ptr == '[') {
			arr[i] = new char[2];
			strcpy(arr[i], "[");
			i++;
			ptr++;
		}
		/* 일반 문자열일 경우*/
		if (*(ptr + strlen(ptr) - 1) != ']') {
			int len = strlen(ptr);
			arr[i] = new char[len + 1];
			strcpy(arr[i], ptr);
		}
		/* 끝에 ']'가 붙어있는 문자열의 경우*/
		else {
			char* ptr2 = ptr;
			int len = 0;
			/* ']' 바로 직전 인덱스까지의 길이를 측정*/
			while (*ptr2 != ']')
			{
				ptr2++;
				len++;
			}
			/* 문자열 생성 및 복사*/
			arr[i] = new char[len + 1];
			strncpy(arr[i], ptr, len);
			arr[i][len] = NULL;
			i++;
			
			ptr = ptr2; //인덱스를 ']'에 위치
			/* ']'를 동적할당 및 복사*/
			while (*ptr == ']') {
				arr[i] = new char[2];
				strcpy(arr[i], "]");
				i++;
				ptr++;
			}
			i--;
		}

		/* ptr이 다음 문자열이 시작되는 부분을 탐색*/
		while (*(ptr++) != 0) {}
	}

	delete[] copy;
	return arr;
}