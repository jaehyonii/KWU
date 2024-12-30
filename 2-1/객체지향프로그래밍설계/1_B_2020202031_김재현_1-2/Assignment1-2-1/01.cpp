#include<iostream>

using namespace std;

bool checkDup(char num, char* record);
int count(char num, char* arr);

int main()
{
	char fnum[100]; // 첫번째입력
	char snum[100]; // 두번째 입력
	int i, j;

	char record[10] = { 0 }; // 한번 나온 숫자들 기록
	
	cout << "두 수 입력(공백으로 구분): ";
	cin >> fnum >> snum;
	
	/*fnum과 snum을 이어붙이는 과정*/
	for (i = 0; fnum[i] != 0; i++) {} //i가 fnum의 NULL문자를 가리키는 index가 됨
	for (j = 0; snum[j] != 0; j++) //snum의 문자들을 이어붙힘
		fnum[i + j] = snum[j];
	fnum[i + j] = 0; // NULL문자 삽입

	for (int i = 0; fnum[i] != 0; i++)
	{
		if (checkDup(fnum[i], record)) // 중복이면 pass
			continue;
		else
			cout << fnum[i] << count(fnum[i], fnum);
	}
	
	return 0;
}

/*중복 검사*/
bool checkDup(char num, char* record)
{
	int i = 0;
	
	/*record 내에 num과 같은 숫자가 있는지 검사*/
	while (record[i] != 0)
	{
		if (num == record[i]) // 있다면 중복되는 수이므로 true 반환
			return true;
		i++;
	}
	record[i] = num; // 없다면 num을 record에 저장하고 false 반환
	return false;
}

/*개수 count*/
int count(char num, char* arr)
{
	int count = 0;

	/*배열 내에 num과 같은 수가 몇개 있는지 count*/
	for (int i = 0; arr[i] != 0; i++)
	{
		if (num == arr[i])
			count++;
	}
	return count;
}