
// OOPMFCProjectDlg.h: 헤더 파일
//

#pragma once
#include "StaticCanvasCtrl.h"

// COOPMFCProjectDlg 대화 상자
class COOPMFCProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	COOPMFCProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OOPMFCPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	int m_row;
	int m_column;
public:
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonSolve();
private:
	CButton m_animate1;
	CButton m_animate2;
public:
	int m_radio;
	StaticCanvasCtrl m_staticCanvas;
};