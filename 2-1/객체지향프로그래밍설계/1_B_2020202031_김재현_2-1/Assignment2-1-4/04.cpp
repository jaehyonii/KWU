#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

class decode
{
private:
	ifstream fin;
	ofstream fout;
	char bin[1024] = { 0 };
	char alp[1024] = { 0 };
	int idx = 0; // alp�迭�� �ε���
public:
	void fileopen(const char *arr, const char *str) {
		/* ���� �Է� open*/
		if (!strcmp(str, "read")) {
			fin.open(arr);
			
			if (!fin)
				cout << "No file detected" << endl;
		}
		/* ���� ��� open*/
		else if (!strcmp(str, "write")) {
			fout.open(arr);
			
			if (!fout)
				cout << "No file detected" << endl;
		}
		/* �Է��� �߸����� ��*/
		else
			cout << "No file detected" << endl;
	}
	void fileread() {
		fin.getline(bin, 1024);
		
		int countZero = 0; // 0�� ����
		int i = 0;// �迭 bin �ε���
		while (bin[i]) {
			if (bin[i] == '0')
				countZero++;
			/* 0�� �����ٰ� 1�� ������*/
			else if (bin[i] == '1') {
				alp[idx++] = 'a' + countZero; //0�� ������ ����ؼ� ���ĺ��� ũ�Ⱑ Ŀ��
				countZero = 0;
			}
			/* 0�� ������ 25���� �Ǹ� ���ĺ� z��� ��*/
			if (countZero == 25) {
				alp[idx++] = 'z';
				countZero = 0;
			}
			i++;
		}
		alp[idx] = 0; // �ι��� ����
	}
	void filewrite() {
		fout << alp;
	}
	void fileclose() {
		fin.close();
		fout.close();
	}
};

int main()
{
	decode decorder;

	decorder.fileopen("binary.txt", "read");
	decorder.fileread();
	decorder.fileopen("alphabet.txt", "write");
	decorder.filewrite();
	decorder.fileclose();
	
	return 0;
}