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

	/* ��� ���� �Է�*/
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

	delete[] input; // input �޸� ����
	/* spt �޸�����*/
	for (int i = 0; spt[i]; i++) {
		delete[]spt[i];
	}
	delete[]spt;
	manager.List_delete(list);
	return 0;
}

/* '[',']',string�� ����*/
char** split(const char* str, char ch1, char ch2) // �����Ҵ� ���� ����!
{
	char* copy = new char[100001];
	/*str�� copy�� ����*/
	int k = 0;
	for (k = 0; str[k] != 0; k++)
		copy[k] = str[k];
	copy[k] = 0;

	int num = 0;
	/*ch ���� ���ֱ�, ch�� ��ġ�� �ε����� �ι��� �ֱ�*/
	for (int i = 0; copy[i] != 0; i++) {
		if (copy[i] == ch1 || copy[i] == ch2) {
			copy[i] = 0;
			num++;
		}
		/* ���ȣ ������ num�� �����ش�.*/
		if (copy[i] == '[' || copy[i] == ']')
			num++;
	}
	/* 'ch'�� ���еǴ� ���ڿ��� ����+1��ŭ �Ҵ� �� �������� �ι��� ����*/
	char** arr = new char* [num + 2];
	arr[num + 1] = 0;

	/*arr �� �κе鿡 ���е� ���ڿ��� ���̸�ŭ �����Ҵ� �� ���ڿ� ����*/
	char* ptr = copy;
	for (int i = 0; i < num + 1; i++) {
		/* '['�� �پ��ִ� ���ڿ��� ��� '['���� �����Ҵ� ó��*/
		while (*ptr == '[') {
			arr[i] = new char[2];
			strcpy(arr[i], "[");
			i++;
			ptr++;
		}
		/* �Ϲ� ���ڿ��� ���*/
		if (*(ptr + strlen(ptr) - 1) != ']') {
			int len = strlen(ptr);
			arr[i] = new char[len + 1];
			strcpy(arr[i], ptr);
		}
		/* ���� ']'�� �پ��ִ� ���ڿ��� ���*/
		else {
			char* ptr2 = ptr;
			int len = 0;
			/* ']' �ٷ� ���� �ε��������� ���̸� ����*/
			while (*ptr2 != ']')
			{
				ptr2++;
				len++;
			}
			/* ���ڿ� ���� �� ����*/
			arr[i] = new char[len + 1];
			strncpy(arr[i], ptr, len);
			arr[i][len] = NULL;
			i++;
			
			ptr = ptr2; //�ε����� ']'�� ��ġ
			/* ']'�� �����Ҵ� �� ����*/
			while (*ptr == ']') {
				arr[i] = new char[2];
				strcpy(arr[i], "]");
				i++;
				ptr++;
			}
			i--;
		}

		/* ptr�� ���� ���ڿ��� ���۵Ǵ� �κ��� Ž��*/
		while (*(ptr++) != 0) {}
	}

	delete[] copy;
	return arr;
}