#pragma once
#include "map.h"


// StaticCanvasCtrl

class StaticCanvasCtrl : public CStatic
{
	DECLARE_DYNAMIC(StaticCanvasCtrl)

public:
	StaticCanvasCtrl();
	virtual ~StaticCanvasCtrl();

protected:
	DECLARE_MESSAGE_MAP()
		
private:
	CRect m_rect_window;
	int row;
	int column;

	CPen m_white_pen;
	CBrush m_white_brush;
	CPen m_blue_pen;
	CBrush m_blue_brush;
	CPen m_black_pen;
	const int margin;
	
	bool isMapReady;
	int rect_size;
	int maze_height;
	int maze_width;
	CPoint current_mouse_point;
	CPoint start;
	CPoint dest;
	
	Cell** map;
	CPoint map_start;

public:
	bool isStartReady; // 출발지점 선택 여부
	bool isDestReady; // 도착지점 선택 여부
	bool method;

	afx_msg void OnPaint();
	void setRow(int row);
	void setCol(int col);
	void DrawMazeOnGeneration(int speed); // 맵 제작, 출력
	void DrawMaze(); // 맵 출력
	void SolvingMaze(BOOL method, int speed);
	void Interaction();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


