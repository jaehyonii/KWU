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
	/* 입력되는 변수들의 길이에 따른 최대 qm 진행 단계 수*/
	int num = atoi(str.c_str());
	
	/* arr의 인덱스가 곧 그 그룹의 column 단계다*/
	val_manager* arr = new val_manager[num + 2]; // arr의 마지막 인덱스는 비워놓는다.

	
	while (!fin.eof()) {
		getline(fin, str);
		arr[0].setData(str.c_str());
	}

	val* current = arr[0].head;
	/* 모든 입력이 don't care minterm 일 때 예외처리*/
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
	
	/* PI table 크기*/
	int height = 0;
	int width = 0;
	current = arr[0].head;
	// minterm 개수
	while (current) {
		if (current->getStr()[0] == 'm')
			width++;
		current = current->getNext();
	}
	// PI 개수
	for (int i = 0; !arr[i].isEmpty(); i++) {
		current = arr[i].head;
		while (current) {
			if (current->getStr()[1] == '*')
				height++;
			current = current->getNext();
		}
	}

	/* PI table 구축 및 초기화*/
	char** table = new char*[height];
	table[0] = new char[height * width];
	for (int i = 1; i < height; i++) {
		table[i] = table[i - 1] + width;
	}
	for (int i = 0; i < height * width; i++)
		table[0][i] = ' ';

	/* Pi table의 변수들을 저장할 배열*/
	char** PI = new char* [height];
	char** minterm = new char* [width];
	
	/* PI 저장*/
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
	/* minterm을 저장*/
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
	
	/* PI가 minterm을 포함하는 칸에 'X' 저장*/
	for (int i = 0; i < height; i++) { // i는 table의 행
		for (int j = 0; j < width; j++) { // j는 table의 열
			int k;
			for (k = 0; k < num; k++) { // k는 문자열의 인덱스
				if (PI[i][k] == '-')
					continue;
				else if (PI[i][k] != minterm[j][k])
					break;
			}
			/* PI가 minterm을 포함하면 'X' 표시*/
			if (k == num) // k == num은 PI가 minterm과 다른 비트 없이 끝까지 진행했음을 의미
				table[i][j] = 'X';
		}
	}


	val_manager minPIs; // minimum set of PIs를 저장할 리스트
	/* x개수가 1개인 열의 PI(essential PI)를 저장*/
	for (int j = 0; j < width; j++) {
		int xNum = 0; // 한 열의 x개수
		for (int i = 0; i < height; i++) {
			if (table[i][j] == 'X')	{
				xNum++;
				idx = i; // 'X'의 행 인덱스 저장
			}
		}
		/*essential PI 발견*/
		if (xNum == 1) {
			minPIs.setData(PI[idx], 0);
			PI[idx][0] = '@'; // 이미 사용된 PI임을 표시

			for (int k = 0; k < width; k++) {
				if (table[idx][k] == 'X'){
					minterm[k][0] = '@'; //이미 포함된 minterm임을 표시
					/* 이미 포함된 minterm의 열에 있는 모든 'X' 삭제*/
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
		/* 많은 'X'를 포함하는 행을 탐색*/
		int xMax = 0; // 가장 많은 'X'의 개수
		for (int i = 0; i < height; i++) {
			if (PI[i][0] == '@')
				continue;

			int xNum = 0; // 한 행에서 'X'의 개수
			for (int j = 0; j < width; j++) {
				if (table[i][j] == 'X')
					xNum++;
			}
			/* 가장 많은 'X'의 개수를 가진 행인덱스 저장*/
			if (xNum > xMax) {
				xMax = xNum;
				idx = i;
			}
		}
		if (xMax > 0) {
			minPIs.setData(PI[idx], 0);
			PI[idx][0] = '@'; // 이미 사용된 PI임을 표시
			for (int k = 0; k < width; k++) {
				if (table[idx][k] == 'X') {
					minterm[k][0] = '@'; //이미 포함된 minterm임을 표시
					/* 이미 포함된 minterm의 열에 있는 모든 'X' 삭제*/
					for (int h = 0; h < height; h++) {
						if (table[h][k] == 'X')
							table[h][k] = ' ';
					}
				}
			}
		}
	}

	minPIs.sortBy1();

	/* 파일에 출력*/
	current = minPIs.head;
	while (current) {
		fout << current->getStr() << endl;
		current = current->getNext();
	}
	
	/* 트랜지스터 개수 구하기*/
	int transistor = 0; // 트랜지스터 개수
	if (minPIs.valNum > 1) // 입력이 1보다 적으면 논리연산 gate가 필요없음
		transistor = transistor + 2 * minPIs.valNum + 2; // OR gate에 들어가는 트랜지스터 수
	current = minPIs.head;
	str = current->getStr(); // 각 입력의 인버터 유무를 저장할 것임
	int input = 0; // AND gate의 입력 개수
	for (int i = 0; i<num; i++) {
		if (str[i] == '1' || str[i] == '0') // '-'이 아니면
			input++;
	}
	if (input > 1) // 입력이 1보다 적으면 논리연산 gate가 필요없음
		transistor = transistor + 2 * input + 2; // 첫번재 AND gate에 들어가는 트랜지스터 수
	current = current->getNext();
	while (current) {
		input = 0;
		for (int i = 0; i < num; i++) {
			if (current->getStr()[i] == '1' || current->getStr()[i] == '0') // '-'이 아니면
				input++;
			if (current->getStr()[i] == '0')
				str[i] = '0';
		}
		if (input > 1) // 입력이 1보다 적으면 논리연산 gate가 필요없음
			transistor = transistor + 2 * input + 2; // 나머지 And gate에 들어가는 트랜지스터 수
		current = current->getNext();
	}
	/* inverter에는 트랜지스터가 2개 포함*/
	for (int i = 0; i < num; i++) {
		if (str[i] == '0') // SOP expression에서 0은 인버터를 필요로 함
			transistor += 2;
	}
	
	cout << endl << "Cost (# of transistor): " << transistor << endl;
	fout << endl << "Cost (# of transistor): " << transistor;
	
	fin.close();
	fout.close();
	/* 메모리 해제*/
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