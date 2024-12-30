#pragma once

#define TOP 8
#define BOTTOM 4
#define LEFT 2
#define RIGHT 1


class Cell {
private:
	int index; // 각 셀마다 가지는 고유한 번호
	int preIdx; // 길을 찾을 때 자신의 전 Cell의 index를 저장
	char wall; // 벽면을 4비트 2진수로 표현 MSB부터 TOP, BOTTOM, LEFT, RIGHT 순서, 1이 벽
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