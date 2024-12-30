#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

char** split(const char* str, char ch);
bool Qcompare(const char* data, const char* cdt);
int memory_free2D(char** ppMemAllocated);
void print(const char* data, char* cdt);

int main()
{
	char fname[100];	
	int num;
	
	
	cout << "Input file name : ";
	cin >> fname;
	cin >> num;
	
	/*2���� �迭 �����Ҵ��ϰ� ���ǰ� ����*/
	char** cdt = new char* [num + 1];
	cdt[num] = 0;
	for (int i = 0; i < num; i++)
	{
		cdt[i] = new char[100];
		cin >> cdt[i];
	}
	
	cout << endl;

	/*�Է¹��� ���ǵ��� �ϳ��� �˻�*/
	for (int i = 0; cdt[i]; i++) {
		ifstream fin;
		/* file open*/
		fin.open(fname);

		if (!fin) {
			cout << "no" << endl;
		}
		
		/* ���Ͽ��� ���پ� �о�鿩 ������ �����ϴ� ���� ���*/
		char data[100];
		for (int j = 0; !fin.eof(); j++) {
			fin.getline(data, 100);
			print(data, cdt[i]);
		}
		/* file close*/
		fin.close();
	}

	
	/*�����Ҵ� ����*/
	memory_free2D(cdt);

	return 0;
}

/* �Ѱ��� ���ǿ� �´� �ؽ�Ʈ�� ���*/
void print(const char* data, char* cdt)
{
	char** spt = split(cdt, '*'); // ������ '*�� split (*�ڸ��� NULL ����)
	const char* pdata = data; // pdata�� data�� ��ҵ��� ����

	/* '*'�� �ƴ�ù��° ������ ���ڿ��� ��ġ���� ������ ����.*/
	if ((*(spt[0]) != 0 && !Qcompare(pdata, spt[0])))
		return;
	int i;
	/* ������ ���Ǻ��� ���ڿ��� ��ġ�ϴ��� �ϳ��� �˻�*/
	for (i = 0; spt[i] != 0; i++) {
		/* ��ġ�ϸ� ��ġ�ϴ� �κ� �ǳʶٱ�*/
		if (Qcompare(pdata, spt[i]))
			pdata += strlen(spt[i]);
		/* ������ '*'�� ��� ���� ���� ����*/
		else if (*(spt[i]) == 0){}
		/* ������ ���ڿ��� ��ġ���� ���� ��*/
		else {
			while (*pdata != 0) {
				/* ���ǰ� ���ڿ��� ��ġ�� ������ ���ڿ� �˻�*/
				if (!Qcompare(pdata, spt[i]))
					pdata++;
				/* ��ġ�ϸ� ��ġ�ϴ� �κ� �ǳʶٱ�*/
				else {
					pdata += strlen(spt[i]);
					break;
				}
			}
		}
		/* ���ڿ��� ������ �����ϸ� �ݺ��� Ż��*/
		if (*pdata == 0)
			break;
	}
	
	/* ���ڿ��� ���� ���������� ������ ���� �������� �� �ֱ� ������ for������ �˻�*/
	for (; spt[i] !=0 ; i++) {
		/* ���ǿ� '*'�� �������� �ʾ��� ��� spt[i]�� �״�� �����ϱ� ������ ���ܷ� �д�.*/
		if (spt[1] == 0)
			break;
		/* �����ִ� ���ǵ��� '*'�� �ƴ� ��� ����.*/
		if (*(spt[i]) != 0)
			return;
	}
	cout << data << endl; // data ���
	
	memory_free2D(spt);
}

/* '*'�� split�� �� ���ǿ� data�� �����ϴ��� �˻� */
bool Qcompare(const char* data, const char* cdt)
{	
	for (int i = 0; cdt[i]; i++) {
		if (cdt[i] == '?')
			continue;
		else if (data[i] != cdt[i])
			return false;
	}
	return true;
}

/*str�� 'ch'�� �����Ͽ� 2�����迭 ����*/
char** split(const char* str, char ch) // �����Ҵ� ���� ����!
{
	char copy[100];
	/*str�� copy�� ����*/
	int k = 0;
	for (k = 0; str[k] != 0; k++)
		copy[k] = str[k];
	copy[k] = 0;

	int num = 0;
	/*ch ���� ���ֱ�, ch�� ��ġ�� �ε����� �ι��� �ֱ�*/
	for (int i = 0; copy[i] != 0; i++) {
		if (copy[i] == ch) {
			copy[i] = 0;
			num++;
		}
	}
	/* 'ch'�� ���еǴ� ���ڿ��� ����+1��ŭ �Ҵ� �� �������� �ι��� ����*/
	char** arr = new char* [num + 2];
	arr[num + 1] = 0;

	/*arr �� �κе鿡 ���е� ���ڿ��� ���̸�ŭ �����Ҵ� �� ���ڿ� ����*/
	char* ptr = copy;
	for (int i = 0; i < num + 1; i++) {
		int len = strlen(ptr);
		arr[i] = new char[len + 1];
		strcpy(arr[i], ptr);
		/* ptr�� ���� ���ڿ��� ���۵Ǵ� �κ��� Ž��*/
		while (*(ptr++) != 0) {}
	}

	return arr;
}

int memory_free2D(char** ppMemAllocated)
{
	if (ppMemAllocated == 0)
		return -1;

	for (int i = 0; ppMemAllocated[i] != 0; i++)
		delete[] ppMemAllocated[i];

	delete[] ppMemAllocated;
	return 0;
}

