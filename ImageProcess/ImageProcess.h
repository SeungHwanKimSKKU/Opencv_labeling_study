
// ImageProcess.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CImageProcessApp:
// �� Ŭ������ ������ ���ؼ��� ImageProcess.cpp�� �����Ͻʽÿ�.
//

class CImageProcessApp : public CWinApp
{
public:
	CImageProcessApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CImageProcessApp theApp;