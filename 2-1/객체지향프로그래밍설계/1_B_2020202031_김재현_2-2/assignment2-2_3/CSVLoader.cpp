#include "CSVLoader.h"

/* public 메서드*/

CSVLoader::CSVLoader() 
:rows(0), cols(0), data(NULL){}

CSVLoader::CSVLoader(const char* path) 
:rows(0), cols(0), data(NULL)
{
	ifstream fin;
	char str[MAX_BUFFER_SIZE] = { 0 }; // 문자열 읽어오기

	fin.open(path);
	if (!fin)
		cout << "unvalid file" << endl;
	/* 행 개수 count*/
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE);
		rows++;
	}
	data = new float* [rows]; // 행 개수만큼 동적할당
	/* 열 개수 count*/
	for (int i = 0; str[i]; i++) {
		if (str[i] == ',')
			cols++;
	}
	cols++; // 실수는 ','개수+1개
	/* 열 개수만큼 동적할당*/
	for (int i = 0; i < rows; i++)
		data[i] = new float[cols];
	fin.close();

	/* 새로 처음부터 open*/
	fin.open(path);
	int i = 0;//행 인덱스
	char** spt = NULL; // 문자열 ','로 구분하여 각각 저장
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE); // 한 행씩 입력
		spt = split(str, ','); // ','를 기준으로 열 구분
		for (int j = 0; j < cols; j++)
			data[i][j] = atof(spt[j]);
		i++;
		/* split으로 동적할당된 메모리 해제*/
		for (int k = 0; spt[k]; k++)
			delete[]spt[k];
		delete[]spt;
	}
	fin.close();
}
CSVLoader::~CSVLoader() {
	/* data 메모리 해제*/
	for (int i=0; i<rows; i++)
		delete[]data[i];
	delete[]data;
}

/* data 반환*/
float** CSVLoader::getData() const {
	return data;
}
/* 행 개수 반환*/
int CSVLoader::getRows() const {
	return rows;
}
/* 열 개수 반환*/
int CSVLoader::getCols() const {
	return cols;
}

void CSVLoader::parse(const char* path) {
	/* 기존 data가 존재한다면*/
	if (data) {
		/* data 메모리 해제*/
		for (int i = 0; i < rows; i++)
			delete[]data[i];
		delete[]data;
	}

	/* 행, 열 개수 초기화*/
	rows = 0;
	cols = 0;
	ifstream fin;
	char str[MAX_BUFFER_SIZE] = { 0 }; // 문자열 읽어오기

	fin.open(path);
	if (!fin)
		cout << "unvalid file" << endl;
	/* 행 개수 count*/
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE);
		rows++;
	}
	data = new float* [rows]; // 행 개수만큼 동적할당
	/* 열 개수 count*/
	for (int i = 0; str[i]; i++) {
		if (str[i] == ',')
			cols++;
	}
	cols++; // 실수는 ','개수+1개
	/* 열 개수만큼 동적할당*/
	for (int i = 0; i < rows; i++)
		data[i] = new float[cols];
	fin.close();

	/* 새로 처음부터 open*/
	fin.open(path);
	int i = 0;//행 인덱스
	char** spt = NULL; // 문자열 ','로 구분하여 각각 저장
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE); // 한 행씩 입력
		spt = split(str, ','); // ','를 기준으로 열 구분
		for (int j = 0; j < cols; j++)
			data[i][j] = atof(spt[j]);
		i++;
		/* split으로 동적할당된 메모리 해제*/
		for (int k = 0; spt[k]; k++)
			delete[]spt[k];
		delete[]spt;
	}
	fin.close();
}

void CSVLoader::print(void) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			cout << data[i][j] << "\t";
		cout << endl;
	}
}

/* private 메서드*/

/*str을 'ch'로 구분하여 2차원배열 생성*/
char** CSVLoader::split(const char* str, char ch) // 동적할당 해제 주의!
{
	char copy[128];
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