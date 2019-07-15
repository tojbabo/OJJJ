// MFC 헤더파일

// CLIENTDlg.h: 헤더 파일
//

#pragma once
#define BUF 1024
#include"CV2.h"

// CCLIENTDlg 대화 상자
class CCLIENTDlg : public CDialogEx
{
// 생성입니다.
public:
	CCLIENTDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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
public:
	afx_msg void OnBnClickedEnter();
	CStatic picture_my;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Send_Picture(HDC dc);
	void BitInfo_init(Mat mat, RECT rect, BITMAPINFO * bitInfo);
	VideoCapture* capture;
	Mat mat;
	CImage frame;
	CStatic picture_user1;
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN send_adr;
	bool isConnected = false;
	SOCKADDR_IN from_adr;
	CStatic static_debug;
	CAM cam;
	Mat user_frame;
};
