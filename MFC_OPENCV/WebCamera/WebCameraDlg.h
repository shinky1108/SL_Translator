
// WebCameraDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CWebCameraDlg 대화 상자
class CWebCameraDlg : public CDialogEx
{
// 생성입니다.
public:
	CWebCameraDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEBCAMERA_DIALOG };
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
	CStatic m_picture;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	VideoCapture *capture;
	Mat tmpImg, frame;
	CImage cimage_mfc;

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEnd();
	
	afx_msg bool flag;
	int index = 0;
	int image_count = 1;
	char buf[256];
	afx_msg void OnBnClickedCancel();
};
