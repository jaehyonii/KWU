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

/*숫자로 이루어진 문자열을 정수로 바꾸기*/
int atoi(char* cdt)
{
	int num = 0;
	
	for (int i = 0; cdt[i] != 0; i++) {
		num *= 10;
		num += cdt[i] - '0';
	}
	
	return num;
}

/*정수를 문자열 숫자로 바꾸기*/
char* itoa(int num)
{
	int n = num;
	int cnt = 0;

	/*num의 자릿수 count*/
	while (n) {
		n /= 10;
		cnt++;
	}
	/*자리수+1만큼 동적할당 후 마지막 인덱스에 널문자 넣어주기*/
	char* arr = new char[cnt + 1];
	arr[cnt] = 0;
	/*문자열에 숫자 채우기*/
	for (int i = cnt - 1; i >= 0; i--) {
		arr[i] = num % 10 + '0';
		num /= 10;
	}
	return arr;
}

/*좌상단, 우하단 좌표로 정리하기*/
void sort(int(*cdt)[2])
{
	/* x1크기가 x2보다 크면 swqp*/
	if (cdt[0][0] > cdt[1][0]) {
		int tmp = cdt[0][0];
		cdt[0][0] = cdt[1][0];
		cdt[1][0] = tmp;
	}
	/* y1크기가 y2보다 크면 swap*/
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
	/*구분문자 ',' 제거*/
	cdtx1[strlen(cdtx1) - 1] = 0;
	cdtx2[strlen(cdtx2) - 1] = 0;
	/*입력받은 좌표를 좌상단 좌표와 우하단 좌표로 변환한다.(정수로)*/
	int cdt[2][2];
	cdt[0][0] = atoi(cdtx1); cdt[0][1] = atoi(cdty1);
	cdt[1][0] = atoi(cdtx2); cdt[1][1] = atoi(cdty2);
	sort(cdt);
	/*crop할 크기*/
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

	/*crop 이미지를 저장할 공간 동적할당*/
	uint8_t** cropped = memory_alloc2D(height, width);
	/*crop 이미지 저장*/
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			cropped[i][j] = ppInputImageBuffer[i + cdt[0][1]][j + cdt[0][0]];
	}
	
	char fname[100];
	strcpy(fname, IMG_NAME);
	/*확장자명 지우고 _cropped_(width)x(height).raw 붙이기*/
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
	/*확장자명 지우고 _horizontalflip.raw 붙이기*/
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
	/*확장자명 지우고 _verticalflip.raw 붙이기*/
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