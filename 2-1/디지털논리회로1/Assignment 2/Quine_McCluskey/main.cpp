#include "val.h"
#include<iostream>
#include<string>
#include<fstream>

using namespace std;

int main()
{
	ifstream fin;
	string str;
	fin.open("input_minterm.txt");

	ofstream fout;
	fout.open("result.txt");

	getline(fin, str);
	/* �ԷµǴ� �������� ���̿� ���� �ִ� qm ���� �ܰ� ��*/
	int num = atoi(str.c_str());
	
	/* arr�� �ε����� �� �� �׷��� column �ܰ��*/
	val_manager* arr = new val_manager[num + 2]; // arr�� ������ �ε����� ������´�.

	
	while (!fin.eof()) {
		getline(fin, str);
		arr[0].setData(str.c_str());
	}

	val* current = arr[0].head;
	/* ��� �Է��� don't care minterm �� �� ����ó��*/
	while (current) {
		if (current->getStr()[0] == 'm')
			break;
		current = current->getNext();
	}
	if (current == NULL){
		for(int i = 0; i<num; i++)
			fout << '-';
		fout << endl << endl;
		fout << "Cost (# of transistors): 0";
		return 0;
	}

	arr[0].sortBy1();
	int i;
	for (i = 1; i <= num; i++) {
		arr[i].combining(arr[i - 1]);
		arr[i].deleteSame();
		if (arr[i].isEmpty())
			break;
	}
	
	/* PI table ũ��*/
	int height = 0;
	int width = 0;
	current = arr[0].head;
	// minterm ����
	while (current) {
		if (current->getStr()[0] == 'm')
			width++;
		current = current->getNext();
	}
	// PI ����
	for (int i = 0; !arr[i].isEmpty(); i++) {
		current = arr[i].head;
		while (current) {
			if (current->getStr()[1] == '*')
				height++;
			current = current->getNext();
		}
	}

	/* PI table ���� �� �ʱ�ȭ*/
	char** table = new char*[height];
	table[0] = new char[height * width];
	for (int i = 1; i < height; i++) {
		table[i] = table[i - 1] + width;
	}
	for (int i = 0; i < height * width; i++)
		table[0][i] = ' ';

	/* Pi table�� �������� ������ �迭*/
	char** PI = new char* [height];
	char** minterm = new char* [width];
	
	/* PI ����*/
	int idx = 0;
	for (int i = 0; !arr[i].isEmpty(); i++) {
		current = arr[i].head;
		while (current) {
			if (current->getStr()[1] == '*') {
				PI[idx] = new char[num + 1];
				strcpy(PI[idx], current->getStr() + 2);
				idx++;
			}
			current = current->getNext();
		}
	}
	/* minterm�� ����*/
	current = arr[0].head;
	idx = 0;
	for (; current;) {
		if (current->getStr()[0] == 'm')
		{
			minterm[idx] = new char[num + 1];
			strcpy(minterm[idx], current->getStr() + 2);
			idx++;
		}
		current = current->getNext();
	}
	
	/* PI�� minterm�� �����ϴ� ĭ�� 'X' ����*/
	for (int i = 0; i < height; i++) { // i�� table�� ��
		for (int j = 0; j < width; j++) { // j�� table�� ��
			int k;
			for (k = 0; k < num; k++) { // k�� ���ڿ��� �ε���
				if (PI[i][k] == '-')
					continue;
				else if (PI[i][k] != minterm[j][k])
					break;
			}
			/* PI�� minterm�� �����ϸ� 'X' ǥ��*/
			if (k == num) // k == num�� PI�� minterm�� �ٸ� ��Ʈ ���� ������ ���������� �ǹ�
				table[i][j] = 'X';
		}
	}


	val_manager minPIs; // minimum set of PIs�� ������ ����Ʈ
	/* x������ 1���� ���� PI(essential PI)�� ����*/
	for (int j = 0; j < width; j++) {
		int xNum = 0; // �� ���� x����
		for (int i = 0; i < height; i++) {
			if (table[i][j] == 'X')	{
				xNum++;
				idx = i; // 'X'�� �� �ε��� ����
			}
		}
		/*essential PI �߰�*/
		if (xNum == 1) {
			minPIs.setData(PI[idx], 0);
			PI[idx][0] = '@'; // �̹� ���� PI���� ǥ��

			for (int k = 0; k < width; k++) {
				if (table[idx][k] == 'X'){
					minterm[k][0] = '@'; //�̹� ���Ե� minterm���� ǥ��
					/* �̹� ���Ե� minterm�� ���� �ִ� ��� 'X' ����*/
					for (int h = 0; h < height; h++) {
						if(table[h][k] == 'X')
							table[h][k] = ' ';
					}
				}
			}
		}
	}
	for (int k = 0; k < height; k++) {
		if (PI[k][0] == '@')
			continue;
		/* ���� 'X'�� �����ϴ� ���� Ž��*/
		int xMax = 0; // ���� ���� 'X'�� ����
		for (int i = 0; i < height; i++) {
			if (PI[i][0] == '@')
				continue;

			int xNum = 0; // �� �࿡�� 'X'�� ����
			for (int j = 0; j < width; j++) {
				if (table[i][j] == 'X')
					xNum++;
			}
			/* ���� ���� 'X'�� ������ ���� ���ε��� ����*/
			if (xNum > xMax) {
				xMax = xNum;
				idx = i;
			}
		}
		if (xMax > 0) {
			minPIs.setData(PI[idx], 0);
			PI[idx][0] = '@'; // �̹� ���� PI���� ǥ��
			for (int k = 0; k < width; k++) {
				if (table[idx][k] == 'X') {
					minterm[k][0] = '@'; //�̹� ���Ե� minterm���� ǥ��
					/* �̹� ���Ե� minterm�� ���� �ִ� ��� 'X' ����*/
					for (int h = 0; h < height; h++) {
						if (table[h][k] == 'X')
							table[h][k] = ' ';
					}
				}
			}
		}
	}

	minPIs.sortBy1();

	/* ���Ͽ� ���*/
	current = minPIs.head;
	while (current) {
		fout << current->getStr() << endl;
		current = current->getNext();
	}
	
	/* Ʈ�������� ���� ���ϱ�*/
	int transistor = 0; // Ʈ�������� ����
	if (minPIs.valNum > 1) // �Է��� 1���� ������ ������ gate�� �ʿ����
		transistor = transistor + 2 * minPIs.valNum + 2; // OR gate�� ���� Ʈ�������� ��
	current = minPIs.head;
	str = current->getStr(); // �� �Է��� �ι��� ������ ������ ����
	int input = 0; // AND gate�� �Է� ����
	for (int i = 0; i<num; i++) {
		if (str[i] == '1' || str[i] == '0') // '-'�� �ƴϸ�
			input++;
	}
	if (input > 1) // �Է��� 1���� ������ ������ gate�� �ʿ����
		transistor = transistor + 2 * input + 2; // ù���� AND gate�� ���� Ʈ�������� ��
	current = current->getNext();
	while (current) {
		input = 0;
		for (int i = 0; i < num; i++) {
			if (current->getStr()[i] == '1' || current->getStr()[i] == '0') // '-'�� �ƴϸ�
				input++;
			if (current->getStr()[i] == '0')
				str[i] = '0';
		}
		if (input > 1) // �Է��� 1���� ������ ������ gate�� �ʿ����
			transistor = transistor + 2 * input + 2; // ������ And gate�� ���� Ʈ�������� ��
		current = current->getNext();
	}
	/* inverter���� Ʈ�������Ͱ� 2�� ����*/
	for (int i = 0; i < num; i++) {
		if (str[i] == '0') // SOP expression���� 0�� �ι��͸� �ʿ�� ��
			transistor += 2;
	}
	
	cout << endl << "Cost (# of transistor): " << transistor << endl;
	fout << endl << "Cost (# of transistor): " << transistor;
	
	fin.close();
	fout.close();
	/* �޸� ����*/
	for (int i = 0; i < width; i++)
		delete[]minterm[i];
	for (int i = 0; i < height; i++)
		delete[]PI[i];
	delete[]minterm;
	delete[]PI;
	delete[]table[0];
	delete[]table;
	delete[] arr;

	return 0;
}