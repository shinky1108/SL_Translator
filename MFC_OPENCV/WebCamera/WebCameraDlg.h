
// WebCameraDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CWebCameraDlg ��ȭ ����
class CWebCameraDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CWebCameraDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEBCAMERA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
