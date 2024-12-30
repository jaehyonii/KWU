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
	int idx = 0; // alp배열의 인덱스
public:
	void fileopen(const char *arr, const char *str) {
		/* 파일 입력 open*/
		if (!strcmp(str, "read")) {
			fin.open(arr);
			
			if (!fin)
				cout << "No file detected" << endl;
		}
		/* 파일 출력 open*/
		else if (!strcmp(str, "write")) {
			fout.open(arr);
			
			if (!fout)
				cout << "No file detected" << endl;
		}
		/* 입력이 잘못됐을 때*/
		else
			cout << "No file detected" << endl;
	}
	void fileread() {
		fin.getline(bin, 1024);
		
		int countZero = 0; // 0의 개수
		int i = 0;// 배열 bin 인덱스
		while (bin[i]) {
			if (bin[i] == '0')
				countZero++;
			/* 0을 지나다가 1을 만나면*/
			else if (bin[i] == '1') {
				alp[idx++] = 'a' + countZero; //0의 개수에 비례해서 알파벳의 크기가 커짐
				countZero = 0;
			}
			/* 0의 개수가 25개가 되면 알파벳 z라는 뜻*/
			if (countZero == 25) {
				alp[idx++] = 'z';
				countZero = 0;
			}
			i++;
		}
		alp[idx] = 0; // 널문자 삽입
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