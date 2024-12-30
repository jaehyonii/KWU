#include "CSVLoader.h"

/* public �޼���*/

CSVLoader::CSVLoader() 
:rows(0), cols(0), data(NULL){}

CSVLoader::CSVLoader(const char* path) 
:rows(0), cols(0), data(NULL)
{
	ifstream fin;
	char str[MAX_BUFFER_SIZE] = { 0 }; // ���ڿ� �о����

	fin.open(path);
	if (!fin)
		cout << "unvalid file" << endl;
	/* �� ���� count*/
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE);
		rows++;
	}
	data = new float* [rows]; // �� ������ŭ �����Ҵ�
	/* �� ���� count*/
	for (int i = 0; str[i]; i++) {
		if (str[i] == ',')
			cols++;
	}
	cols++; // �Ǽ��� ','����+1��
	/* �� ������ŭ �����Ҵ�*/
	for (int i = 0; i < rows; i++)
		data[i] = new float[cols];
	fin.close();

	/* ���� ó������ open*/
	fin.open(path);
	int i = 0;//�� �ε���
	char** spt = NULL; // ���ڿ� ','�� �����Ͽ� ���� ����
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE); // �� �྿ �Է�
		spt = split(str, ','); // ','�� �������� �� ����
		for (int j = 0; j < cols; j++)
			data[i][j] = atof(spt[j]);
		i++;
		/* split���� �����Ҵ�� �޸� ����*/
		for (int k = 0; spt[k]; k++)
			delete[]spt[k];
		delete[]spt;
	}
	fin.close();
}
CSVLoader::~CSVLoader() {
	/* data �޸� ����*/
	for (int i=0; i<rows; i++)
		delete[]data[i];
	delete[]data;
}

/* data ��ȯ*/
float** CSVLoader::getData() const {
	return data;
}
/* �� ���� ��ȯ*/
int CSVLoader::getRows() const {
	return rows;
}
/* �� ���� ��ȯ*/
int CSVLoader::getCols() const {
	return cols;
}

void CSVLoader::parse(const char* path) {
	/* ���� data�� �����Ѵٸ�*/
	if (data) {
		/* data �޸� ����*/
		for (int i = 0; i < rows; i++)
			delete[]data[i];
		delete[]data;
	}

	/* ��, �� ���� �ʱ�ȭ*/
	rows = 0;
	cols = 0;
	ifstream fin;
	char str[MAX_BUFFER_SIZE] = { 0 }; // ���ڿ� �о����

	fin.open(path);
	if (!fin)
		cout << "unvalid file" << endl;
	/* �� ���� count*/
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE);
		rows++;
	}
	data = new float* [rows]; // �� ������ŭ �����Ҵ�
	/* �� ���� count*/
	for (int i = 0; str[i]; i++) {
		if (str[i] == ',')
			cols++;
	}
	cols++; // �Ǽ��� ','����+1��
	/* �� ������ŭ �����Ҵ�*/
	for (int i = 0; i < rows; i++)
		data[i] = new float[cols];
	fin.close();

	/* ���� ó������ open*/
	fin.open(path);
	int i = 0;//�� �ε���
	char** spt = NULL; // ���ڿ� ','�� �����Ͽ� ���� ����
	while (!fin.eof()) {
		fin.getline(str, MAX_BUFFER_SIZE); // �� �྿ �Է�
		spt = split(str, ','); // ','�� �������� �� ����
		for (int j = 0; j < cols; j++)
			data[i][j] = atof(spt[j]);
		i++;
		/* split���� �����Ҵ�� �޸� ����*/
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

/* private �޼���*/

/*str�� 'ch'�� �����Ͽ� 2�����迭 ����*/
char** CSVLoader::split(const char* str, char ch) // �����Ҵ� ���� ����!
{
	char copy[128];
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