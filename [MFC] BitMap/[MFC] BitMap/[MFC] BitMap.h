
// [MFC] BitMap.h : [MFC] BitMap ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFCBitMapApp:
// �� Ŭ������ ������ ���ؼ��� [MFC] BitMap.cpp�� �����Ͻʽÿ�.
//

class CMFCBitMapApp : public CWinApp
{
public:
	CMFCBitMapApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCBitMapApp theApp;
