#include<iostream>

using namespace std;

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define starting 4 // 출발지점에서는 진행방향이 없음

char** memory_alloc2D(int height, int width);
int memory_free2D(char** ppMemAllocated);
void maze(char** map, int i, int j, int* dest, int dir, int num);
bool isValue(char** map, int i, int j);

int main()
{
	/* 맵의 높이, 너비 입력*/
	int height, width;
	cin >> height >> width;

	/* 맵을 문자열로 입력 받을 때 null문자까지 고려해서 너비 1 크게 생성*/
	char** map = memory_alloc2D(height + 1, width + 1);
	/* 맵 작성*/
	for (int h = 0; h < height; h++)
		cin >> map[h];
	map[height] = 0;
	
	/* 출발, 도착 지점을 행, 열 순으로 입력*/
	int start[2];
	int dest[2];
	cin >> start[1] >> start[0] >> dest[1] >> dest[0];
	/* 출발, 도착 지점을 인덱스로 변경*/
	start[0] -= 1; start[1] -= 1;
	dest[0] -= 1; dest[1] -= 1;
	
	maze(map, start[0], start[1], dest, starting, 1); // 출발지점까지 경로길이에 포함하므로 num에 1 대입

	/* map 해제*/
	memory_free2D(map);
}

/* i, j는 현재 위치 인덱스, dest는 목적지 인덱스, dir는 직전에 진행한 방향, num은 경로의 길이*/
void maze(char**map, int i, int j, int*dest, int dir, int num) {
	if (isValue(map, i, j)) {
		/* 목적지에 도착했을 때*/
		if (i == dest[0] && j == dest[1])
			cout << num;

		/* 왔던 길을 다시 되돌아 가는 것을 막기 위한 조건*/
		if (dir != RIGHT)
			maze(map, i, j - 1, dest, LEFT, num + 1);
		if (dir != LEFT)
			maze(map, i, j + 1, dest, RIGHT, num + 1);
		if (dir != UP)
			maze(map, i + 1, j, dest, DOWN, num + 1);
		if (dir != DOWN)
			maze(map, i - 1, j, dest, UP, num + 1);
	}
}

/* 유효성 검사*/
bool isValue(char** map, int i, int j) {
	/* map의 마지막 행은 NULL이기 때문에 map[i]==0이면 맵을 벗어난 것.
	또한 map의 각 행의 마지막 열도 NULL이기 때문에 map[i][j]==0이면 맵을 벗어난 것.
	map[i][j]=='1'이면 벽이기에 유효하지 않다.*/
	if (i >= 0 && map[i] != 0 && j >= 0 && map[i][j] != 0 && map[i][j] == '0')
		return true;
	else
		return false;
}

/*메모리 할당*/
char** memory_alloc2D(int height, int width)
{
	char** ppMem2d = new char * [height];
	if (ppMem2d == 0)
		return 0;
	ppMem2d[0] = new char[height * width];
	if (ppMem2d[0] == 0)
	{
		delete[] ppMem2d;
		return 0;
	}

	for (int i = 1; i < height; i++)
		ppMem2d[i] = ppMem2d[i - 1] + width;
	return ppMem2d;
}

/*메모리 해제*/
int memory_free2D(char** ppMemAllocated)
{
	if (ppMemAllocated == 0)
		return -1;

	if (ppMemAllocated[0])
		delete[] ppMemAllocated[0];

	delete[] ppMemAllocated;
	return 0;
}