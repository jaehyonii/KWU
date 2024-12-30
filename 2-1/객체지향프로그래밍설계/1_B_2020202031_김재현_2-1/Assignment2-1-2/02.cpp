#include<iostream>

using namespace std;

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define starting 4 // ������������� ��������� ����

char** memory_alloc2D(int height, int width);
int memory_free2D(char** ppMemAllocated);
void maze(char** map, int i, int j, int* dest, int dir, int num);
bool isValue(char** map, int i, int j);

int main()
{
	/* ���� ����, �ʺ� �Է�*/
	int height, width;
	cin >> height >> width;

	/* ���� ���ڿ��� �Է� ���� �� null���ڱ��� ����ؼ� �ʺ� 1 ũ�� ����*/
	char** map = memory_alloc2D(height + 1, width + 1);
	/* �� �ۼ�*/
	for (int h = 0; h < height; h++)
		cin >> map[h];
	map[height] = 0;
	
	/* ���, ���� ������ ��, �� ������ �Է�*/
	int start[2];
	int dest[2];
	cin >> start[1] >> start[0] >> dest[1] >> dest[0];
	/* ���, ���� ������ �ε����� ����*/
	start[0] -= 1; start[1] -= 1;
	dest[0] -= 1; dest[1] -= 1;
	
	maze(map, start[0], start[1], dest, starting, 1); // ����������� ��α��̿� �����ϹǷ� num�� 1 ����

	/* map ����*/
	memory_free2D(map);
}

/* i, j�� ���� ��ġ �ε���, dest�� ������ �ε���, dir�� ������ ������ ����, num�� ����� ����*/
void maze(char**map, int i, int j, int*dest, int dir, int num) {
	if (isValue(map, i, j)) {
		/* �������� �������� ��*/
		if (i == dest[0] && j == dest[1])
			cout << num;

		/* �Դ� ���� �ٽ� �ǵ��� ���� ���� ���� ���� ����*/
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

/* ��ȿ�� �˻�*/
bool isValue(char** map, int i, int j) {
	/* map�� ������ ���� NULL�̱� ������ map[i]==0�̸� ���� ��� ��.
	���� map�� �� ���� ������ ���� NULL�̱� ������ map[i][j]==0�̸� ���� ��� ��.
	map[i][j]=='1'�̸� ���̱⿡ ��ȿ���� �ʴ�.*/
	if (i >= 0 && map[i] != 0 && j >= 0 && map[i][j] != 0 && map[i][j] == '0')
		return true;
	else
		return false;
}

/*�޸� �Ҵ�*/
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

/*�޸� ����*/
int memory_free2D(char** ppMemAllocated)
{
	if (ppMemAllocated == 0)
		return -1;

	if (ppMemAllocated[0])
		delete[] ppMemAllocated[0];

	delete[] ppMemAllocated;
	return 0;
}