#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>

using namespace std;

#define IMG_HEIGHT 512
#define IMG_WIDTH 512

uint8_t** memory_alloc2D(uint32_t height, uint32_t width);
int memory_free2D(uint8_t** ppMemAllocated);
int atoi(char* cdt);
char* itoa(int num);
void sort(int(*cdt)[2]);
void crop(char* IMG_NAME);
void horizontal(char* IMG_NAME);
void vertical(char* IMG_NAME);

int main()
{
	char IMG_NAME[100];
	cin >> IMG_NAME;

	crop(IMG_NAME);
	horizontal(IMG_NAME);
	vertical(IMG_NAME);
	
	return 0;
}

/*���ڷ� �̷���� ���ڿ��� ������ �ٲٱ�*/
int atoi(char* cdt)
{
	int num = 0;
	
	for (int i = 0; cdt[i] != 0; i++) {
		num *= 10;
		num += cdt[i] - '0';
	}
	
	return num;
}

/*������ ���ڿ� ���ڷ� �ٲٱ�*/
char* itoa(int num)
{
	int n = num;
	int cnt = 0;

	/*num�� �ڸ��� count*/
	while (n) {
		n /= 10;
		cnt++;
	}
	/*�ڸ���+1��ŭ �����Ҵ� �� ������ �ε����� �ι��� �־��ֱ�*/
	char* arr = new char[cnt + 1];
	arr[cnt] = 0;
	/*���ڿ��� ���� ä���*/
	for (int i = cnt - 1; i >= 0; i--) {
		arr[i] = num % 10 + '0';
		num /= 10;
	}
	return arr;
}

/*�»��, ���ϴ� ��ǥ�� �����ϱ�*/
void sort(int(*cdt)[2])
{
	/* x1ũ�Ⱑ x2���� ũ�� swqp*/
	if (cdt[0][0] > cdt[1][0]) {
		int tmp = cdt[0][0];
		cdt[0][0] = cdt[1][0];
		cdt[1][0] = tmp;
	}
	/* y1ũ�Ⱑ y2���� ũ�� swap*/
	if (cdt[0][1] > cdt[1][1]) {
		int tmp = cdt[0][1];
		cdt[0][1] = cdt[1][1];
		cdt[1][1] = tmp;
	}
}

void crop(char*IMG_NAME)
{
	char cdtx1[100]; char cdty1[100];
	char cdtx2[100]; char cdty2[100];
	
	cout << "1st coordinate : "; cin >> cdtx1; cin >> cdty1;
	cout << "2nd coordinate : "; cin >> cdtx2; cin >> cdty2;
	/*���й��� ',' ����*/
	cdtx1[strlen(cdtx1) - 1] = 0;
	cdtx2[strlen(cdtx2) - 1] = 0;
	/*�Է¹��� ��ǥ�� �»�� ��ǥ�� ���ϴ� ��ǥ�� ��ȯ�Ѵ�.(������)*/
	int cdt[2][2];
	cdt[0][0] = atoi(cdtx1); cdt[0][1] = atoi(cdty1);
	cdt[1][0] = atoi(cdtx2); cdt[1][1] = atoi(cdty2);
	sort(cdt);
	/*crop�� ũ��*/
	int height = cdt[1][1] - cdt[0][1] + 1;
	int width = cdt[1][0] - cdt[0][0] + 1;
	
	FILE* fpInputImage = 0;
	FILE* fpOutputImage = 0;
	uint8_t** ppInputImageBuffer = 0;
	/*input file open*/
	fpInputImage = fopen(IMG_NAME, "rb");
	/*memory allocation*/
	ppInputImageBuffer = memory_alloc2D(IMG_HEIGHT, IMG_WIDTH);
	/*input file read to memory from the file*/
	for (int i = 0; i < IMG_HEIGHT; i++)
		fread((uint8_t*)ppInputImageBuffer[i], sizeof(uint8_t), IMG_WIDTH, fpInputImage);

	/*crop �̹����� ������ ���� �����Ҵ�*/
	uint8_t** cropped = memory_alloc2D(height, width);
	/*crop �̹��� ����*/
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			cropped[i][j] = ppInputImageBuffer[i + cdt[0][1]][j + cdt[0][0]];
	}
	
	char fname[100];
	strcpy(fname, IMG_NAME);
	/*Ȯ���ڸ� ����� _cropped_(width)x(height).raw ���̱�*/
	int i = strlen(fname) - 1;
	for (; fname[i] != '.'; i--) {}
	fname[i] = 0;
	strcat(fname, "_cropped_");
	char* arr;
	arr = itoa(width); strcat(fname, arr);
	strcat(fname, "x");
	delete[] arr;
	arr = itoa(height); strcat(fname, arr);
	delete[] arr;
	strcat(fname, ".raw");
	/*output fileopen*/
	fpOutputImage = fopen(fname, "wb");
	/*write the file*/
	for (int i = 0; i < height; i++)
		fwrite((uint8_t*)cropped[i], sizeof(uint8_t), width, fpOutputImage);
	memory_free2D(cropped);
	memory_free2D(ppInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);
}

void horizontal(char* IMG_NAME)
{
	FILE* fpInputImage = 0;
	FILE* fpOutputImage = 0;
	uint8_t** ppInputImageBuffer = 0;
	
	/*input file open*/
	fpInputImage = fopen(IMG_NAME, "rb");
	/*memory allocation*/
	ppInputImageBuffer = memory_alloc2D(IMG_HEIGHT, IMG_WIDTH);
	/*input file read to memory from the file*/
	for (int i = 0; i < IMG_HEIGHT; i++)
		fread((uint8_t*)ppInputImageBuffer[i], sizeof(uint8_t), IMG_WIDTH, fpInputImage);

	/*reverse horizontally*/
	for (int i = 0; i<IMG_HEIGHT; i++) {
		for (int j = 0; j < IMG_WIDTH / 2; j++) {
			uint8_t tmp = ppInputImageBuffer[i][j];
			ppInputImageBuffer[i][j] = ppInputImageBuffer[i][IMG_WIDTH - j - 1];
			ppInputImageBuffer[i][IMG_WIDTH - j - 1] = tmp;
		}
	}
	
	char fname[100];
	strcpy(fname, IMG_NAME);
	/*Ȯ���ڸ� ����� _horizontalflip.raw ���̱�*/
	int i = strlen(fname) - 1;
	for (; fname[i] != '.'; i--) {}
	fname[i] = 0;
	strcat(fname, "_horizontalflip.raw");
	/*output fileopen*/
	fpOutputImage = fopen(fname, "wb");
	/*write the file*/
	for (i = 0; i < IMG_HEIGHT; i++)
		fwrite((uint8_t*)ppInputImageBuffer[i], sizeof(uint8_t), IMG_WIDTH, fpOutputImage);
	memory_free2D(ppInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);
}

void vertical(char* IMG_NAME)
{
	FILE* fpInputImage = 0;
	FILE* fpOutputImage = 0;
	uint8_t** ppInputImageBuffer = 0;

	/*input file open*/
	fpInputImage = fopen(IMG_NAME, "rb");
	/*memory allocation*/
	ppInputImageBuffer = memory_alloc2D(IMG_HEIGHT, IMG_WIDTH);
	/*input file read to memory from the file*/
	for (int i = 0; i < IMG_HEIGHT; i++)
		fread((uint8_t*)ppInputImageBuffer[i], sizeof(uint8_t), IMG_WIDTH, fpInputImage);

	/*reverse vertically*/
	for (int i = 0; i < IMG_WIDTH; i++) {
		for (int j = 0; j < IMG_HEIGHT / 2; j++) {
			uint8_t tmp = ppInputImageBuffer[j][i];
			ppInputImageBuffer[j][i] = ppInputImageBuffer[IMG_HEIGHT - j - 1][i];
			ppInputImageBuffer[IMG_HEIGHT - j - 1][i] = tmp;
		}
	}

	char fname[100];
	strcpy(fname, IMG_NAME);
	/*Ȯ���ڸ� ����� _verticalflip.raw ���̱�*/
	int i = strlen(fname) - 1;
	for (; fname[i] != '.'; i--) {}
	fname[i] = 0;
	strcat(fname, "_verticalflip.raw");
	/*output fileopen*/
	fpOutputImage = fopen(fname, "wb");
	/*write the file*/
	for (int i = 0; i < IMG_HEIGHT; i++)
		fwrite((uint8_t*)ppInputImageBuffer[i], sizeof(uint8_t), IMG_WIDTH, fpOutputImage);
	memory_free2D(ppInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);
}

uint8_t** memory_alloc2D(uint32_t height, uint32_t width)
{
	uint8_t** ppMem2d = new uint8_t * [height];
	if (ppMem2d == 0)
		return 0;
	ppMem2d[0] = new uint8_t[height * width];
	if (ppMem2d[0] == 0)
	{
		delete[] ppMem2d;
		return 0;
	}

	for (int i = 1; i < height; i++)
		ppMem2d[i] = ppMem2d[i - 1] + width;
	return ppMem2d;
}

int memory_free2D(uint8_t** ppMemAllocated)
{
	if (ppMemAllocated == 0)
		return -1;
	
	if (ppMemAllocated[0])
		delete[] ppMemAllocated[0];

	delete[] ppMemAllocated;
	return 0;
}