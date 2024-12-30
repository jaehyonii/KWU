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
	
	/*2차원 배열 동적할당하고 조건값 저장*/
	char** cdt = new char* [num + 1];
	cdt[num] = 0;
	for (int i = 0; i < num; i++)
	{
		cdt[i] = new char[100];
		cin >> cdt[i];
	}
	
	cout << endl;

	/*입력받은 조건들을 하나씩 검사*/
	for (int i = 0; cdt[i]; i++) {
		ifstream fin;
		/* file open*/
		fin.open(fname);

		if (!fin) {
			cout << "no" << endl;
		}
		
		/* 파일에서 한줄씩 읽어들여 조건을 부합하는 것은 출력*/
		char data[100];
		for (int j = 0; !fin.eof(); j++) {
			fin.getline(data, 100);
			print(data, cdt[i]);
		}
		/* file close*/
		fin.close();
	}

	
	/*동적할당 해제*/
	memory_free2D(cdt);

	return 0;
}

/* 한개의 조건에 맞는 텍스트들 출력*/
void print(const char* data, char* cdt)
{
	char** spt = split(cdt, '*'); // 조건을 '*로 split (*자리엔 NULL 대입)
	const char* pdata = data; // pdata가 data의 요소들을 참조

	/* '*'이 아닌첫번째 조건이 문자열과 일치하지 않으면 종료.*/
	if ((*(spt[0]) != 0 && !Qcompare(pdata, spt[0])))
		return;
	int i;
	/* 앞쪽의 조건부터 문자열과 일치하는지 하나씩 검사*/
	for (i = 0; spt[i] != 0; i++) {
		/* 일치하면 일치하는 부분 건너뛰기*/
		if (Qcompare(pdata, spt[i]))
			pdata += strlen(spt[i]);
		/* 조건이 '*'일 경우 다음 조건 참조*/
		else if (*(spt[i]) == 0){}
		/* 조건이 문자열과 일치하지 않을 때*/
		else {
			while (*pdata != 0) {
				/* 조건과 문자열이 일치할 때까지 문자열 검사*/
				if (!Qcompare(pdata, spt[i]))
					pdata++;
				/* 일치하면 일치하는 부분 건너뛰기*/
				else {
					pdata += strlen(spt[i]);
					break;
				}
			}
		}
		/* 문자열의 끝까지 도달하면 반복문 탈출*/
		if (*pdata == 0)
			break;
	}
	
	/* 문자열의 끝에 도달했지만 조건은 아직 남아있을 수 있기 때문에 for문으로 검사*/
	for (; spt[i] !=0 ; i++) {
		/* 조건에 '*'을 포함하지 않았을 경우 spt[i]가 그대로 존재하기 때문에 예외로 둔다.*/
		if (spt[1] == 0)
			break;
		/* 남아있는 조건들이 '*'가 아닐 경우 종료.*/
		if (*(spt[i]) != 0)
			return;
	}
	cout << data << endl; // data 출력
	
	memory_free2D(spt);
}

/* '*'로 split된 각 조건에 data가 부합하는지 검사 */
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

/*str을 'ch'로 구분하여 2차원배열 생성*/
char** split(const char* str, char ch) // 동적할당 해제 주의!
{
	char copy[100];
	/*str을 copy에 복사*/
	int k = 0;
	for (k = 0; str[k] != 0; k++)
		copy[k] = str[k];
	copy[k] = 0;

	int num = 0;
	/*ch 개수 세주기, ch가 위치한 인덱스에 널문자 넣기*/
	for (int i = 0; copy[i] != 0; i++) {
		if (copy[i] == ch) {
			copy[i] = 0;
			num++;
		}
	}
	/* 'ch'로 구분되는 문자열의 개수+1만큼 할당 후 마지막에 널문자 대입*/
	char** arr = new char* [num + 2];
	arr[num + 1] = 0;

	/*arr 각 부분들에 구분된 문자열의 길이만큼 동적할당 후 문자열 복사*/
	char* ptr = copy;
	for (int i = 0; i < num + 1; i++) {
		int len = strlen(ptr);
		arr[i] = new char[len + 1];
		strcpy(arr[i], ptr);
		/* ptr이 다음 문자열이 시작되는 부분을 탐색*/
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

