#include<iostream>

using namespace std;

bool checkDup(char num, char* record);
int count(char num, char* arr);

int main()
{
	char fnum[100]; // ù��°�Է�
	char snum[100]; // �ι�° �Է�
	int i, j;

	char record[10] = { 0 }; // �ѹ� ���� ���ڵ� ���
	
	cout << "�� �� �Է�(�������� ����): ";
	cin >> fnum >> snum;
	
	/*fnum�� snum�� �̾���̴� ����*/
	for (i = 0; fnum[i] != 0; i++) {} //i�� fnum�� NULL���ڸ� ����Ű�� index�� ��
	for (j = 0; snum[j] != 0; j++) //snum�� ���ڵ��� �̾����
		fnum[i + j] = snum[j];
	fnum[i + j] = 0; // NULL���� ����

	for (int i = 0; fnum[i] != 0; i++)
	{
		if (checkDup(fnum[i], record)) // �ߺ��̸� pass
			continue;
		else
			cout << fnum[i] << count(fnum[i], fnum);
	}
	
	return 0;
}

/*�ߺ� �˻�*/
bool checkDup(char num, char* record)
{
	int i = 0;
	
	/*record ���� num�� ���� ���ڰ� �ִ��� �˻�*/
	while (record[i] != 0)
	{
		if (num == record[i]) // �ִٸ� �ߺ��Ǵ� ���̹Ƿ� true ��ȯ
			return true;
		i++;
	}
	record[i] = num; // ���ٸ� num�� record�� �����ϰ� false ��ȯ
	return false;
}

/*���� count*/
int count(char num, char* arr)
{
	int count = 0;

	/*�迭 ���� num�� ���� ���� � �ִ��� count*/
	for (int i = 0; arr[i] != 0; i++)
	{
		if (num == arr[i])
			count++;
	}
	return count;
}