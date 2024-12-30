
// OOPMFCProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OOPMFCProject.h"
#include "OOPMFCProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COOPMFCProjectDlg 대화 상자



COOPMFCProjectDlg::COOPMFCProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OOPMFCPROJECT_DIALOG, pParent)
	, m_row(0)
	, m_column(0)
	, m_radio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COOPMFCProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_row);
	DDX_Text(pDX, IDC_EDIT2, m_column);
	DDX_Control(pDX, IDC_CHECK1, m_animate1);
	DDX_Control(pDX, IDC_CHECK2, m_animate2);
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	DDX_Control(pDX, IDC_STATIC_CANVAS, m_staticCanvas);
}

BEGIN_MESSAGE_MAP(COOPMFCProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &COOPMFCProjectDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &COOPMFCProjectDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_SOLVE, &COOPMFCProjectDlg::OnBnClickedButtonSolve)
END_MESSAGE_MAP()


// COOPMFCProjectDlg 메시지 처리기

BOOL COOPMFCProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COOPMFCProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COOPMFCProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COOPMFCProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COOPMFCProjectDlg::OnBnClickedButtonGenerate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE); // row column 값 읽기

	/* 맵 크기 입력 시 4 x 4 ~ 30 x 30 범위 설정*/
	if (4 <= m_row && m_row <= 30 && 4 <= m_column && m_column <= 30) {
		m_staticCanvas.setRow(m_row);
		m_staticCanvas.setCol(m_column);

		/* 출발 도착 준비 상태 변환*/
		m_staticCanvas.isStartReady = false;
		m_staticCanvas.isDestReady = false;

		/* 맵 그리기*/
		if(m_animate1.GetCheck())
			m_staticCanvas.DrawMazeOnGeneration(20); // Sleep(20)
		else
			m_staticCanvas.DrawMazeOnGeneration(0); // Sleep(0)
	}
	else {
		AfxMessageBox(_T("4부터 30까지의 범위 내에서 입력하세요"));
	}

}


// 실험 중
void COOPMFCProjectDlg::OnBnClickedButtonSolve()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE); // radio 값 읽기
	
	m_staticCanvas.method = m_radio;
	/* m_radio = 0 -> DFS, m_radio = 1 -> BFS */
	if (m_animate2.GetCheck())
		m_staticCanvas.SolvingMaze(m_radio, 20); // Sleep(20)
	else
		m_staticCanvas.SolvingMaze(m_radio, 0); // Sleep(0)



}


void COOPMFCProjectDlg::OnBnClickedButtonExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxGetMainWnd()->PostMessageW(WM_CLOSE); // 프로그램 종료
}