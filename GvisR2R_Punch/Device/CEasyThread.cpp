#include "stdafx.h"
#include "CEasyThread.h"


IMPLEMENT_DYNCREATE(CEasyThread, CWinThread)
BEGIN_MESSAGE_MAP(CEasyThread, CWinThread)
	//{{AFX_MSG_MAP(CEquipEvtThread)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CEasyThread::InitInstance()
{
	m_bRun = true;
	//m_bEnd = false;
	m_bStarted = TRUE;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//DoLoop();
	while (m_bRun && !m_bStop)
	{
		if (Running() == 0)
		{
			m_bRun = 0;
			break;
		}

		Sleep(0);
	}
	CoUninitialize();

	//m_bEnd = true;
	//return TRUE; // Next is Run().
	return FALSE;
}

int CEasyThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


CEasyThread::CEasyThread()
{
	m_bStarted = 0;
	m_bRun = false;
	//m_bEnd = false;
	m_bStop = 0;
}

CEasyThread::~CEasyThread()
{


}


void CEasyThread::DoLoop()
{
	while (m_bRun && !m_bStop)
	{
		if (Running() == 0)
		{
			m_bRun = 0;
			break;
		}

		Sleep(0);
	}
}

int CEasyThread::Running()
{

	return TRUE;
}

BOOL CEasyThread::PrepareThread()
{
	return CreateThread(CREATE_SUSPENDED);
}


int CEasyThread::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinThread::Run();
}
