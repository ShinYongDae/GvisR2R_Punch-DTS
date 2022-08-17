
// GvisR2R_Punch.h : GvisR2R_Punch ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "SingleInstance.h"


// CGvisR2R_PunchApp:
// �� Ŭ������ ������ ���ؼ��� GvisR2R_Punch.cpp�� �����Ͻʽÿ�.
//

class CGvisR2R_PunchApp : public CWinApp
{
	CString m_strHostAddress, m_strHostPort;

	void wsaStartup();
	void wsaEndup();

public:
	CGvisR2R_PunchApp();
	~CGvisR2R_PunchApp();

	CSingleInstance m_singleInstance;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();

	CString GetHostAddress();
	CString GetHostPort();
	void SetHostAddress(CString sAddr);
	void SetHostPort(CString sPort);

	DECLARE_MESSAGE_MAP()
};

extern CGvisR2R_PunchApp theApp;
