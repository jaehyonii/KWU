#include "pch.h"
#include "map.h"

Cell::Cell()
	:wall(0b1111), isVisited(false)
{}
void Cell::setIndex(int num) {
	index = num;
}
int Cell::getIndex() {
	return index;
}
void Cell::setPre(int num) {
	preIdx = num;
}
int Cell::getPre() {
	return preIdx;
}
void Cell::breakWall(int dir) {
	switch (dir) {
	case TOP:
		wall ^= TOP;
		break;
	case BOTTOM:
		wall ^= BOTTOM;
		break;
	case LEFT:
		wall ^= LEFT;
		break;
	case RIGHT:
		wall ^= RIGHT;
		break;
	}
}
char Cell::getWall() {
	return wall;
}

bool Cell::getVisited() {
	return isVisited;
}
void Cell::visited() {
	isVisited = true;
}
void Cell::unvisited() {
	isVisited = false;
}


Stack::Stack() {
	last = -1;
}
void Stack::push(Cell* data) {
	arr[++last] = data;
}
bool Stack::empty() {
	if (last < 0) return true;
	else return false;
}
Cell* Stack::pop() {
	if (empty()) {
		return  nullptr;
	}
	return arr[last--];
}
int Stack::size() {
	return last + 1;
}
Cell* Stack::top() {
	if (empty()) {
		return nullptr;
	}
	return arr[last];
}


CircularQueue::CircularQueue()
{
	front = 0;
	rear = 0;
}

bool CircularQueue::isEmpty() {
	return (front == rear);
}

bool CircularQueue::isFull() {
	return (front == (rear + 1) % maxQueueSize);
}

void CircularQueue::enQueue(Cell* data) {
	if (isFull()) {
		return;
	}
	else {
		rear = ++rear % maxQueueSize;
		arr[rear] = data;
	}
}

Cell* CircularQueue::deQueue() {
	if (isEmpty()) {
		return nullptr;
	}
	else {
		return arr[++front];
	}
}


/* 동적할당*/
Cell** memory_alloc2D(int row, int column) {
	Cell** map = new Cell * [row];
	if (map == 0)
		return 0;
	map[0] = new Cell[row * column];
	if (map[0] == 0) {
		delete[] map;
		return 0;
	}

	for (int i = 1; i < row; i++)
		map[i] = map[i - 1] + column;
	return map;
}

/* 메모리 해제*/
int memory_free2D(Cell** map) {
	if (map == 0)
		return -1;

	if (map[0])
		delete[] map[0];

	delete[] map;
	return 0;
}


