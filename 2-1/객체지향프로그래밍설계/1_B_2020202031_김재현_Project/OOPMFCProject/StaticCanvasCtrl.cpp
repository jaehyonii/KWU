// StaticCanvasCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "StaticCanvasCtrl.h"



// StaticCanvasCtrl

IMPLEMENT_DYNAMIC(StaticCanvasCtrl, CStatic)

StaticCanvasCtrl::StaticCanvasCtrl()
	: margin(30)
	, m_black_pen(PS_SOLID, 2, RGB(0, 0, 0))
	, m_white_pen(PS_SOLID, 2, RGB(255, 255, 255))
	, m_white_brush(RGB(255, 255, 255))
	, m_blue_pen(PS_SOLID, 2, RGB(153, 204, 255))
	, m_blue_brush(RGB(153, 204, 255))

	, isMapReady(false)
	, isStartReady(false)
	, isDestReady(false)
	, method(false)
	, rect_size(0)
	, maze_height(0)
	, maze_width(0)
	, current_mouse_point(-1, -1)
	, map(nullptr)
	, map_start(-1, -1)
	, start(-1, -1)
	, dest(-1, -1)
{}

StaticCanvasCtrl::~StaticCanvasCtrl()
{
	memory_free2D(map);
}


BEGIN_MESSAGE_MAP(StaticCanvasCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// StaticCanvasCtrl 메시지 처리기

void StaticCanvasCtrl::setRow(int row) {
	this->row = row;
}
void StaticCanvasCtrl::setCol(int col) {
	this->column = col;
}

void StaticCanvasCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.

	CRect rectWindow;
	GetClientRect(&rectWindow);
	CBrush whiteBrush(RGB(255, 255, 255));
	dc.FillRect(&rectWindow, &whiteBrush);
}

void StaticCanvasCtrl::DrawMazeOnGeneration(int speed) {
	Invalidate();

	memory_free2D(map); // 기존의 맵 삭제
	map = memory_alloc2D(row, column); // 맵의 틀 제작

	CPaintDC dc(this);
	GetClientRect(&m_rect_window);


	/* canvas의 x, y 길이 저장*/
	int board_sizeX = this->m_rect_window.Width() - (this->margin << 1);
	int board_sizeY = this->m_rect_window.Height() - (this->margin << 1);
	rect_size = min(board_sizeX / column, board_sizeY / row); // 맵 한칸(정사각형) 한 변의 길이

	maze_height = rect_size * row;
	maze_width = rect_size * column;
	map_start.x = m_rect_window.Width() / 2 - maze_width / 2;
	map_start.y = m_rect_window.Height() / 2 - maze_height / 2;

	CPen* old_pen = dc.SelectObject(&this->m_black_pen);
	CBrush* old_brush = dc.SelectObject(&this->m_white_brush);

	dc.FillRect(&m_rect_window, old_brush); // 흰 바탕 채우기


	/* 맵의 틀 그리기*/
	for (int y = map_start.y; y < map_start.y + maze_height; y += rect_size) {
		for (int x = map_start.x; x < map_start.x + maze_width; x += rect_size)
			dc.Rectangle(x, y, x + rect_size, y + rect_size);
	}


	dc.SelectObject(&this->m_white_pen);

	dc.SelectObject(&this->m_blue_pen);
	dc.SelectObject(&this->m_blue_brush);

	/* 맵의 각 셀에 인덱스 매기기*/
	for (int i = 0; i < row * column; i++) {
		map[i / column][i % column].setIndex(i);
	}

	Stack stack;
	Cell* current = nullptr;
	Cell* chosen = nullptr;
	int r, c;
	srand((unsigned int)time(NULL));

	r = rand() % row;
	c = rand() % column;

	map[r][c].visited();
	stack.push(&map[r][c]);

	CRect d(map_start.x + c * rect_size + 1, map_start.y + r * rect_size + 1,
		map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
	dc.FillRect(&d, &m_blue_brush);
	Sleep(speed);

	while (!stack.empty()) {
		current = stack.pop();
		r = current->getIndex() / column;
		c = current->getIndex() % column;

		char unvisitedNeighbor = 0; // 방문하지 않은 이웃을 4비트 2진수로 표현

		/* 방문하지 않은 이웃 기록*/
			/* map의 좌측 맨 끝 부분이 아닐 경우*/
		if (c != 0) {
			/* 좌측 Cell이 방문되지 않았을 때*/
			if (!map[r][c - 1].getVisited()) {
				unvisitedNeighbor ^= LEFT;
			}
		}
		/* map의 우측 맨 끝 부분이 아닐 경우*/
		if (c != column - 1) {
			/* 우측 Cell이 방문되지 않았을 때*/
			if (!map[r][c + 1].getVisited()) {
				unvisitedNeighbor ^= RIGHT;
			}
		}
		/* map의 상단 맨 끝 부분이 아닐 경우*/
		if (r != 0) {
			/* 상단 Cell이 방문되지 않았을 때*/
			if (!map[r - 1][c].getVisited()) {
				unvisitedNeighbor ^= TOP;
			}
		}
		/* map의 하단 맨 끝 부분이 아닐 경우*/
		if (r != row - 1) {
			/* 하단 Cell이 방문되지 않았을 때*/
			if (!map[r + 1][c].getVisited()) {
				unvisitedNeighbor ^= BOTTOM;
			}
		}
		/* 기록 끝*/

		/*방문하지 않은 이웃이 존재한다면*/
		if (unvisitedNeighbor != 0) {
			stack.push(current); // current 다시 스택으로

			bool isChosen = false; // 뽑기 성공 여부


			/* 랜덤으로 뽑기*/
			while (!isChosen)
			{
				switch (rand() % 4) {
				case 0:
					/* 위쪽 이웃 choose*/
					if ((unvisitedNeighbor & TOP) == TOP) {
						chosen = &map[r - 1][c];
						isChosen = true; // 뽑기 성공
						chosen->visited(); // 방문표시
						/* 벽 부수기*/
						chosen->breakWall(BOTTOM);
						current->breakWall(TOP);
						/* animate*/
						CRect d(map_start.x + c * rect_size + 1, map_start.y + (r - 1) * rect_size + 1,
							map_start.x + (c + 1) * rect_size - 2, map_start.y + r * rect_size + 1);
						dc.FillRect(&d, &m_blue_brush);
						Sleep(speed);
					}
					break;
				case 1:
					/* 아래쪽 이웃 choose*/
					if ((unvisitedNeighbor & BOTTOM) == BOTTOM) {
						chosen = &map[r + 1][c];
						isChosen = true; // 뽑기 성공
						chosen->visited(); // 방문표시
						/* 벽 부수기*/
						chosen->breakWall(TOP);
						current->breakWall(BOTTOM);
						/* animate*/
						CRect d(map_start.x + c * rect_size + 1, map_start.y + (r + 1) * rect_size - 2,
							map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 2) * rect_size - 2);
						dc.FillRect(&d, &m_blue_brush);
						Sleep(speed);
					}
					break;
				case 2:
					/* 왼쪽 이웃 choose*/
					if ((unvisitedNeighbor & LEFT) == LEFT) {
						chosen = &map[r][c - 1];
						isChosen = true; // 뽑기 성공
						chosen->visited(); // 방문표시
						/* 벽 부수기*/
						chosen->breakWall(RIGHT);
						current->breakWall(LEFT);
						/* animate*/
						CRect d(map_start.x + (c - 1) * rect_size + 1, map_start.y + r * rect_size + 1,
							map_start.x + c * rect_size + 1, map_start.y + (r + 1) * rect_size - 2);
						dc.FillRect(&d, &m_blue_brush);
						Sleep(speed);
					}
					break;
				case 3:
					/* 오른쪽 이웃 choose*/
					if ((unvisitedNeighbor & RIGHT) == RIGHT) {
						chosen = &map[r][c + 1];
						isChosen = true; // 뽑기 성공
						chosen->visited(); // 방문표시
						/* 벽 부수기*/
						chosen->breakWall(LEFT);
						current->breakWall(RIGHT);
						/* animate*/
						CRect d(map_start.x + (c + 1) * rect_size - 2, map_start.y + r * rect_size + 1,
							map_start.x + (c + 2) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
						dc.FillRect(&d, &m_blue_brush);
						Sleep(speed);
					}
					break;
				}
			}
			stack.push(chosen); // 다시 스택으로
		}
	}

	DrawMaze();

	isMapReady = true;
	UpdateWindow();
}

void StaticCanvasCtrl::DrawMaze() {
	Invalidate();

	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, m_rect_window.Width(), m_rect_window.Height());
	memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, m_rect_window.Width(), m_rect_window.Height(), WHITENESS);
	

	/* 맵의 틀 다시 그리기*/
	memDC.SelectObject(&this->m_white_brush);
	memDC.SelectObject(&this->m_black_pen);
	for (int y = map_start.y; y < map_start.y + maze_height; y += rect_size) {
		for (int x = map_start.x; x < map_start.x + maze_width; x += rect_size)
			memDC.Rectangle(x, y, x + rect_size, y + rect_size);
	}
	/* 벽 지우기*/
	memDC.SelectObject(&this->m_white_pen);
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < column; x++) {
			/* 위쪽 벽이 존재하지 않으면 흰색으로 위쪽 벽을 색칠*/
			if ((map[y][x].getWall() & TOP) != TOP) {
				CRect r(map_start.x + x * rect_size + 1, map_start.y + y * rect_size - 2, map_start.x + (x + 1) * rect_size - 2, map_start.y + y * rect_size + 1);
				memDC.FillRect(&r, &m_white_brush);
			}
			/* 오른쪽 벽이 존재하지 않으면 흰색으로 오른쪽 벽을 색칠*/
			if ((map[y][x].getWall() & RIGHT) != RIGHT) {
				CRect r(map_start.x + (x + 1) * rect_size - 2, map_start.y + y * rect_size + 1, map_start.x + (x + 1) * rect_size + 1, map_start.y + (y + 1) * rect_size - 2);
				memDC.FillRect(&r, &m_white_brush);
			}
		}
	}

	dc.BitBlt(0, 0, m_rect_window.Width(), m_rect_window.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	
	UpdateWindow();
}


void StaticCanvasCtrl::SolvingMaze(BOOL method, int speed) { // method 0 -> DFS, 1 -> BFS
	DrawMaze();
	
	if (isStartReady && isDestReady) {
		/* 방문기록 초기화*/
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < column; j++)
				map[i][j].unvisited();
		}

		Invalidate();
		CPaintDC dc(this);

		/* canvas의 x, y 길이 저장*/
		int board_sizeX = this->m_rect_window.Width() - (this->margin << 1);
		int board_sizeY = this->m_rect_window.Height() - (this->margin << 1);

		CPen* old_pen = dc.SelectObject(&this->m_blue_pen);
		CBrush* old_brush = dc.SelectObject(&this->m_blue_brush);


		int r, c;
		r = (start.y - map_start.y) / rect_size; // 출발지의 행 인덱스
		c = (start.x - map_start.x) / rect_size; // 출발지의 열 인덱스

		if (!method) { // DFS
			Stack stack;
			Cell* current = nullptr;

			dc.SelectObject(&m_blue_brush);

			current = &map[r][c];
			current->visited();
			stack.push(current);

			while ((dest.x - map_start.x) / rect_size != c || (dest.y - map_start.y) / rect_size != r) {
				if (stack.empty()) // current가 돌고돌아 시작점을 가리킨다는 뜻
					stack.push(current); // current를 stack에 다시 넣어준다
				
				r = current->getIndex() / column;
				c = current->getIndex() % column;


				/* 위쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				if ((~current->getWall() & TOP) == TOP && !map[r - 1][c].getVisited()) {
					map[r - 1][c].setPre(current->getIndex());
					current = &map[r - 1][c];
					stack.push(current);
					current->visited();
					CRect d(map_start.x + c * rect_size + 1, map_start.y + (r)*rect_size - 2,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 왼쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				else if ((~current->getWall() & LEFT) == LEFT && !map[r][c - 1].getVisited()) {
					map[r][c - 1].setPre(current->getIndex());
					current = &map[r][c - 1];
					stack.push(current);
					current->visited();
					CRect d(map_start.x + c * rect_size - 2, map_start.y + r * rect_size + 1,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 오른쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				else if ((~current->getWall() & RIGHT) == RIGHT && !map[r][c + 1].getVisited()) {
					map[r][c + 1].setPre(current->getIndex());
					current = &map[r][c + 1];
					stack.push(current);
					current->visited();
					CRect d(map_start.x + (c)*rect_size + 1, map_start.y + r * rect_size + 1,
						map_start.x + (c + 1) * rect_size + 1, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 아래쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				else if ((~current->getWall() & BOTTOM) == BOTTOM && !map[r + 1][c].getVisited()) {
					map[r + 1][c].setPre(current->getIndex());
					current = &map[r + 1][c];
					stack.push(current);
					current->visited();
					CRect d(map_start.x + c * rect_size + 1, map_start.y + (r)*rect_size + 1,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size + 1);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 막다른 길일 경우*/
				else {
					CRect d(map_start.x + c * rect_size + 1, map_start.y + (r)*rect_size + 1,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					current = stack.pop();
				}
				/* 출발지 계속 찍어주기 */
				CBrush red_brush(RGB(255, 0, 0));
				dc.SelectObject(&red_brush);
				dc.Ellipse(map_start.x + ((start.x - map_start.x) / rect_size) * rect_size + 1, map_start.y + ((start.y - map_start.y) / rect_size) * rect_size + 1,
					map_start.x + ((start.x - map_start.x) / rect_size + 1) * rect_size - 2, map_start.y + ((start.y - map_start.y) / rect_size + 1) * rect_size - 2);
				/* 도착지 계속 찍어주기 */
				CBrush blue_brush(RGB(0, 0, 255));
				dc.SelectObject(&blue_brush);
				dc.Ellipse(map_start.x + ((dest.x - map_start.x) / rect_size) * rect_size + 1, map_start.y + ((dest.y - map_start.y) / rect_size) * rect_size + 1,
					map_start.x + ((dest.x - map_start.x) / rect_size + 1) * rect_size - 2, map_start.y + ((dest.y - map_start.y) / rect_size + 1) * rect_size - 2);
			}
		}
		else { // bfs
			CircularQueue queue;
			Cell* current = nullptr;

			current = &map[r][c];
			current->visited();
			queue.enQueue(current);
			while ((dest.x - map_start.x) / rect_size != c || (dest.y - map_start.y) / rect_size != r) {
				current = queue.deQueue();
				r = current->getIndex() / column;
				c = current->getIndex() % column;

				/* 위쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				if ((~current->getWall() & TOP) == TOP && !map[r - 1][c].getVisited()) {
					map[r - 1][c].setPre(current->getIndex());
					queue.enQueue(&map[r - 1][c]);
					map[r - 1][c].visited();
					CRect d(map_start.x + c * rect_size + 1, map_start.y + (r)*rect_size - 2,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 왼쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				if ((~current->getWall() & LEFT) == LEFT && !map[r][c - 1].getVisited()) {
					map[r][c - 1].setPre(current->getIndex());
					queue.enQueue(&map[r][c - 1]);
					map[r][c - 1].visited();
					CRect d(map_start.x + c * rect_size - 2, map_start.y + r * rect_size + 1,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 오른쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				if ((~current->getWall() & RIGHT) == RIGHT && !map[r][c + 1].getVisited()) {
					map[r][c + 1].setPre(current->getIndex());
					queue.enQueue(&map[r][c + 1]);
					map[r][c + 1].visited();
					CRect d(map_start.x + (c)*rect_size + 1, map_start.y + r * rect_size + 1,
						map_start.x + (c + 1) * rect_size + 1, map_start.y + (r + 1) * rect_size - 2);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 아래쪽 벽이 없을 경우 + 방문하지 않았을 경우*/
				if ((~current->getWall() & BOTTOM) == BOTTOM && !map[r + 1][c].getVisited()) {
					map[r + 1][c].setPre(current->getIndex());
					queue.enQueue(&map[r + 1][c]);
					map[r + 1][c].visited();
					CRect d(map_start.x + c * rect_size + 1, map_start.y + (r)*rect_size + 1,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size + 1);
					dc.FillRect(&d, &m_blue_brush);
					Sleep(speed);
				}
				/* 막다른 길일 경우*/
				else {
					CRect d(map_start.x + c * rect_size + 1, map_start.y + (r)*rect_size + 1,
						map_start.x + (c + 1) * rect_size - 2, map_start.y + (r + 1) * rect_size - 2);
					Sleep(speed);
					dc.FillRect(&d, &m_blue_brush);
				}
				/* 출발지 계속 찍어주기 */
				CBrush red_brush(RGB(255, 0, 0));
				dc.SelectObject(&red_brush);
				dc.Ellipse(map_start.x + ((start.x - map_start.x) / rect_size) * rect_size + 1, map_start.y + ((start.y - map_start.y) / rect_size) * rect_size + 1,
					map_start.x + ((start.x - map_start.x) / rect_size + 1) * rect_size - 2, map_start.y + ((start.y - map_start.y) / rect_size + 1) * rect_size - 2);
				/* 도착지 계속 찍어주기 */
				CBrush blue_brush(RGB(0, 0, 255));
				dc.SelectObject(&blue_brush);
				dc.Ellipse(map_start.x + ((dest.x - map_start.x) / rect_size) * rect_size + 1, map_start.y + ((dest.y - map_start.y) / rect_size) * rect_size + 1,
					map_start.x + ((dest.x - map_start.x) / rect_size + 1) * rect_size - 2, map_start.y + ((dest.y - map_start.y) / rect_size + 1) * rect_size - 2);
			}
		}

		/* 경로 표시*/
		CPen red_pen(PS_SOLID, 8, RGB(255, 0, 0));
		dc.SelectObject(&red_pen);
		r = (dest.y - map_start.y) / rect_size; // 출발지 행
		c = (dest.x - map_start.x) / rect_size; // 출발지 열
		while (r != (start.y - map_start.y) / rect_size || c != (start.x - map_start.x) / rect_size) {
			dc.MoveTo(map_start.x + c * rect_size + rect_size / 2, map_start.y + r * rect_size + rect_size / 2);
			Cell* current = &map[r][c];
			r = current->getPre() / column;
			c = current->getPre() % column;
			dc.LineTo(map_start.x + c * rect_size + rect_size / 2, map_start.y + r * rect_size + rect_size / 2);
		}
		UpdateWindow();
	}
}


void StaticCanvasCtrl::Interaction() {
	Invalidate();

	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, m_rect_window.Width(), m_rect_window.Height());
	memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, m_rect_window.Width(), m_rect_window.Height(), WHITENESS);

	for (int y = map_start.y; y < map_start.y + maze_height; y += this->rect_size) {
		for (int x = map_start.x; x < map_start.x + maze_width; x += this->rect_size) {
			memDC.SelectObject(&this->m_black_pen);
			memDC.SelectObject(&this->m_white_brush);
			memDC.Rectangle(x, y, x + rect_size, y + rect_size);

			CRect r(x + 1, y + 1, x + this->rect_size - 2, y + this->rect_size - 2);
			if (x <= this->current_mouse_point.x && this->current_mouse_point.x < x + this->rect_size
				&& y <= this->current_mouse_point.y && this->current_mouse_point.y < y + this->rect_size) {
				CBrush yellow_brush(RGB(255, 255, 0));
				CBrush* oldBrush = memDC.SelectObject(&yellow_brush);
				memDC.FillRect(&r, &yellow_brush);

				memDC.SelectObject(oldBrush);
			}
			/* 출발지*/
			if (x <= start.x && start.x < x + rect_size && y <= start.y && start.y < y + rect_size && isStartReady) {
				CBrush red_brush(RGB(255, 0, 0));
				memDC.SelectObject(&red_brush);
				memDC.Ellipse(x + 1, y + 1, x + this->rect_size - 2, y + this->rect_size - 2);
			}
			/* 도착지*/
			if (x <= dest.x && dest.x < x + rect_size && y <= dest.y && dest.y < y + rect_size && isDestReady) {
				CBrush blue_brush(RGB(0, 0, 255));
				memDC.SelectObject(&blue_brush);
				memDC.Ellipse(x + 1, y + 1, x + this->rect_size - 2, y + this->rect_size - 2);
			}
		}
	}

	memDC.SelectObject(&this->m_white_pen);
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < column; x++) {
			/* 위쪽 벽이 존재하지 않으면 흰색으로 위쪽 벽을 색칠*/
			if ((map[y][x].getWall() & TOP) != TOP) {
				CRect r(map_start.x + x * rect_size + 1, map_start.y + y * rect_size - 2, map_start.x + (x + 1) * rect_size - 2, map_start.y + y * rect_size + 1);
				memDC.FillRect(&r, &m_white_brush);
			}
			/* 오른쪽 벽이 존재하지 않으면 흰색으로 오른쪽 벽을 색칠*/
			if ((map[y][x].getWall() & RIGHT) != RIGHT) {
				CRect r(map_start.x + (x + 1) * rect_size - 2, map_start.y + y * rect_size + 1, map_start.x + (x + 1) * rect_size + 1, map_start.y + (y + 1) * rect_size - 2);
				memDC.FillRect(&r, &m_white_brush);
			}
		}
	}


	dc.BitBlt(0, 0, m_rect_window.Width(), m_rect_window.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	UpdateWindow();
}

void StaticCanvasCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	this->current_mouse_point = point;
	if (isMapReady)
	{
		this->Interaction();
	}
	CStatic::OnMouseMove(nFlags, point);
}


void StaticCanvasCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (isMapReady) {
		if (map_start.x <= point.x && point.x < map_start.x + column * rect_size && map_start.y <= point.y && point.y < map_start.y + row * rect_size) {
			start.x = point.x;
			start.y = point.y;
			isStartReady = true;
		}
		else
		{
			isStartReady = false;
		}
		this->Interaction();
	}
	CStatic::OnLButtonDown(nFlags, point);
}


void StaticCanvasCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (isMapReady) {
		if (map_start.x <= point.x && point.x < map_start.x + column * rect_size && map_start.y <= point.y && point.y < map_start.y + row * rect_size) {
			dest.x = point.x;
			dest.y = point.y;
			isDestReady = true;
		}
		else
		{
			isDestReady = false;
		}
		this->Interaction();
	}
	CStatic::OnRButtonDown(nFlags, point);
}
