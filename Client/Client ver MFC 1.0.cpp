// MFC 파일
// 메인 Dlg 파일
// UDP기반 기본 영상 에코잉 구현

// CLIENTDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CLIENT.h"
#include "CLIENTDlg.h"
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


// CCLIENTDlg 대화 상자



CCLIENTDlg::CCLIENTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCLIENTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PICTURE_MY, picture_my);
	DDX_Control(pDX, PICTURE_OTHER_1, picture_user1);
	DDX_Control(pDX, STATIC_DEBUG, static_debug);
}

BEGIN_MESSAGE_MAP(CCLIENTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_ENTER, &CCLIENTDlg::OnBnClickedEnter)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCLIENTDlg 메시지 처리기

BOOL CCLIENTDlg::OnInitDialog()
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

	capture = new VideoCapture(0);
	if (!capture->isOpened())
		MessageBox(_T("캠을 열 수 엄슴다\n"));

	capture->set(CAP_PROP_FRAME_WIDTH, 320);
	capture->set(CAP_PROP_FRAME_HEIGHT, 240);
	SetTimer(1000, 30, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCLIENTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCLIENTDlg::OnPaint()
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
HCURSOR CCLIENTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCLIENTDlg::OnBnClickedEnter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	WSADATA wsaData;
	SOCKADDR_IN adr_to;
	SOCKADDR_IN adr_from;
	SOCKET sock;
	char buf[100] = "helllllllo wordl";
	int adr_sz;

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
		// 에러
	}
	memset(&adr_to, 0, sizeof(adr_to));
	memset(&adr_from, 0, sizeof(adr_from));

	adr_to.sin_family = AF_INET;
	adr_to.sin_addr.s_addr = inet_addr("52.39.220.170");
	adr_to.sin_port = htons(atoi("9000"));
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == INVALID_SOCKET) {
		//eroror
	}
	for(int i = 0 ; i< 10;i++) {
		sendto(sock, buf, 100, 0, (struct sockaddr*)&adr_to, sizeof(adr_to));
		adr_sz = sizeof(adr_from);
		recvfrom(sock, buf, 100, 0, (struct sockaddr*)&adr_from, &adr_sz);
		
	}*/
	if (isConnected == false) {
		if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {}
		memset(&send_adr, 0, sizeof(send_adr));
		memset(&from_adr, 0, sizeof(from_adr));

		send_adr.sin_family = AF_INET;
		send_adr.sin_addr.s_addr = inet_addr("54.245.152.250");
		send_adr.sin_port = htons(atoi("9000"));
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock == INVALID_SOCKET) {}

		isConnected = true;
	}
	else
		isConnected = false;
}



void CCLIENTDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCLIENTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	capture->read(mat);

	CImage frame2;

	RECT rect;
	picture_my.GetClientRect(&rect);

	cv::Size winSize(rect.right, rect.bottom);
	frame2.Create(winSize.width, winSize.height, 24);


	BITMAPINFO *bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	BitInfo_init(mat, rect, bitInfo);

	SetDIBitsToDevice(frame2.GetDC(), 0, 0, mat.cols, mat.rows, 0, 0, 0, mat.rows, mat.data, bitInfo, DIB_RGB_COLORS);


	picture_my.MoveWindow(0, 100, mat.cols, mat.rows);
	CString get;
	get.Format(_T("%d - %d"), mat.cols, mat.rows); // 324 - 240
	static_debug.SetWindowTextW(get); // 320 240

	HDC dc = ::GetDC(picture_my.m_hWnd);
	Send_Picture(dc);
	frame2.BitBlt(dc, 0, 0); // 실제로 그리는 출력을 하는 코드
	::ReleaseDC(picture_my.m_hWnd, dc);

	frame2.ReleaseDC();
	frame2.Destroy();
	CDialogEx::OnTimer(nIDEvent);
}


void CCLIENTDlg::Send_Picture(HDC dc)
{
	// TODO: 여기에 구현 코드 추가.
	if (isConnected) {
		uchar* char_temp = cam.MtoC(mat);
		char* buf = (char*)char_temp;
		sendto(sock, buf, BUF, 0, (struct sockaddr*)&send_adr, sizeof(send_adr));
		int from_adr_sz = sizeof(from_adr);
		recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&from_adr, &from_adr_sz);
		user_frame = cam.CtoM((uchar*)buf);
		cam.stream_show(user_frame);

		/*
		CImage user_img;
		uchar* buffer = mat.data;
		char* buf = (char*)buffer;

		sendto(sock, buf, BUF, 0, (struct sockaddr*)&send_adr, sizeof(send_adr));

		int from_adr_sz = sizeof(from_adr);

		recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&from_adr, &from_adr_sz);

		Mat mat2 = Mat(240,320, CV_8UC3);
		mat2.data = (uchar*)buf;

		CImage user_frame;

		RECT rect;
		picture_user1.GetClientRect(&rect);

		cv::Size winSize(rect.right, rect.bottom);
		user_frame.Create(winSize.width, winSize.height, 24);


		BITMAPINFO *bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
		BitInfo_init(mat2, rect, bitInfo);

		SetDIBitsToDevice(user_frame.GetDC(), 0, 0, mat2.cols, mat2.rows, 0, 0, 0, mat2.rows, mat2.data, bitInfo, DIB_RGB_COLORS);


		picture_user1.MoveWindow(300, 100, mat2.cols, mat2.rows);

		HDC dc = ::GetDC(picture_user1.m_hWnd);
		Send_Picture(dc);
		user_frame.BitBlt(dc, 0, 0); // 실제로 그리는 출력을 하는 코드

		::ReleaseDC(picture_user1.m_hWnd, dc);

		user_frame.ReleaseDC();
		user_frame.Destroy();*/
	}
}


void CCLIENTDlg::BitInfo_init(Mat mat, RECT rect,BITMAPINFO* bitInfo)
{
	// TODO: 여기에 구현 코드 추가.

	int bpp = 8 * mat.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32)); // bpp 가 8 or 24 or 32가  아니라면 종료
	
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat.cols;
	bitInfo->bmiHeader.biHeight = -mat.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;
}
