#pragma once

#define TOP 8
#define BOTTOM 4
#define LEFT 2
#define RIGHT 1


class Cell {
private:
	int index; // �� ������ ������ ������ ��ȣ
	int preIdx; // ���� ã�� �� �ڽ��� �� Cell�� index�� ����
	char wall; // ������ 4��Ʈ 2������ ǥ�� MSB���� TOP, BOTTOM, LEFT, RIGHT ����, 1�� ��
	bool isVisited;
	
public:
	Cell();
	void setIndex(int num);
	int getIndex();
	void setPre(int num);
	int getPre();
	void breakWall(int dir);
	char getWall();
	bool getVisited();
	void visited();
	void unvisited();
};


class Stack {
private:
	Cell* arr[900];
	int last;
	
public:
	Stack();
	void push(Cell* data);
	Cell* pop();
	int size();
	Cell* top();
    bool empty();
};


class CircularQueue {

private:
	Cell* arr[900] = { 0, };
	int front;
	int rear;
	const int maxQueueSize = 900;

public:
	CircularQueue();
	bool isEmpty();
	bool isFull();
	void enQueue(Cell* data);
	Cell* deQueue();

};
Cell** memory_alloc2D(int row, int column);
int memory_free2D(Cell** map);
void map_generate(Cell** map, int row, int column);