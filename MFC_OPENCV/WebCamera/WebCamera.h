
// WebCamera.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CWebCameraApp:
// �� Ŭ������ ������ ���ؼ��� WebCamera.cpp�� �����Ͻʽÿ�.
//

class CWebCameraApp : public CWinApp
{
public:
	CWebCameraApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CWebCameraApp theApp;