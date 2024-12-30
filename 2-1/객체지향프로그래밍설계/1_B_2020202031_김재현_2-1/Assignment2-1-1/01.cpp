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
	int stack[128]; // ����
	int topIndex = 0; // ������ top index
	char input[10]; // �Է¹��� ��ɾ ������ �迭
	int num; // �Է¹��� ������ ������ ����
	/*���� ����*/
	while (1) {
		cin >> input; // ��ɾ� �Է�
		
		/* push��ɾ� -> stack�� value �߰�*/
		if (!strcmp(input, "push")) {
			cin >> num;
			push(stack, topIndex, num);
		}
		/* pop��ɾ� -> stack���� top value ��� �� ����*/
		else if (!strcmp(input, "pop")) {
			pop(stack, topIndex);
		}
		/* top��ɾ� -> stack���� top value ���*/
		else if (!strcmp(input, "top")) {
			top(stack, topIndex);
		}
		/* print��ɾ� -> stack�� bottom to top value ���*/
		else if (!strcmp(input, "print")) {
			print(stack, topIndex);
		}
		/* empty��ɾ� -> stack�� value ������ ���� 0 or 1*/
		else if (!strcmp(input, "empty")) {
			empty(topIndex);
		}
		/* exit��ɾ� -> �ݺ��� Ż��*/
		else if (!strcmp(input, "exit"))
			break;
		/* �������� ������ ��ɾ� ���� ��ɾ �Էµ� ��*/
		else
			cout << "Invalid Input!" << endl;
	}
	return 0;
}

void push(int stack[], int& topIndex, int num) {
	/* ������ ���� á�� �� ����ó��*/
	if (topIndex == 128) {
		cout << "stack is full" << endl;
		return;
	}

	stack[topIndex++] = num; // top�� value �߰��ϰ� top 1����
}

void pop(int stack[], int& topIndex) {
	/* stack�� ������� �� ����ó��*/
	if (topIndex == 0) {
		cout << "There's No value" << endl;
		return;
	}
	cout << stack[topIndex - 1] << endl; // top value ���
	stack[topIndex - 1] = 0; // top value�� 0���� �ʱ�ȭ
	topIndex--; // top 1 ����
}

void top(int stack[], int topIndex) {
	/* stack�� ������� �� ����ó��*/
	if (topIndex == 0) {
		cout << "There's No value" << endl;
		return;
	}
	cout << stack[topIndex - 1] << endl; // top value ���
}

void print(int stack[], int topIndex) {
	/* stack�� ������� �� ����ó��*/
	if (topIndex == 0) {
		cout << "There's No value" << endl;
		return;
	}
	/* stack�� ��� value ���*/
	for (int i = 0; i < topIndex; i++)
		cout << stack[i] << " ";
	cout << endl;
}

void empty(int topIndex) {
	/* stack�� ��������� 1*/
	if (topIndex == 0)
		cout << "1" << endl;
	/* stack�� ä���������� 0*/
	else
		cout << "0" << endl;
}