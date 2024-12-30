#include<iostream>

using namespace std;

char* my_strtok(char* str) {
	static char* pstr = nullptr;
	static int idx = 0; // ���ڿ� str�� �ε���

	/* str�� ���ڿ� �迭�� �ԷµǸ� ���ڿ��� �ּҰ��� ����*/
	if (str){
		pstr = str;
	}
	
	int pre = idx;
	while (1) {
		if (pstr[idx] == '@' || pstr[idx] == '.') {
			pstr[idx] = NULL; // '@', '.'�� NULL�� ��ü
			idx++; // ���� �Լ� ȣ�⿡�� NULL ���� ���ڸ� ����ų �� �ֵ��� ��
			break;
		}
		else if (pstr[idx] == NULL) {
			break; //���ڿ��� ���̸� idx�� NULL�� ����Ű�� ��
		}
		idx++;
	}
	
	/* ���ڿ��� ���̸� idx�� ���������� ������� �ʾ� pre�� ����*/
	if (pre == idx)
		return NULL;
	else
		return pstr + pre;
}

int main()
{
	char arr[100];

	cin >> arr;
	char* ptr = my_strtok(arr);
	
	while (ptr) {
		cout << ptr << endl;
		ptr = my_strtok(NULL);
	}

	return 0;
}