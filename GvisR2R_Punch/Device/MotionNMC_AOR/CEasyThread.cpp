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
	m_bEnd = false;
	m_bStarted = TRUE;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	DoLoop();
	CoUninitialize();

	m_bEnd = true;
	return TRUE;
}

int CEasyThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

void CEasyThread::KillThread()
{
	m_bRun = false;
	if (m_bStarted)
	{
		DWORD nTick = GetTickCount();
		while (!m_bEnd)
		{

			if (GetTickCount() - nTick >= 3000)
				break;

			Sleep(0);
			//PumpMessage();
		}
	}
}

CEasyThread::CEasyThread()
{
	m_bStarted = 0;
	m_bRun = false;
	m_bEnd = false;
	m_bStop = 0;
}

CEasyThread::~CEasyThread()
{
}


void CEasyThread::DoLoop()
{
	while (m_bRun)
	{
		if (Running() == 0)
		{
			m_bRun = 0;
			break;
		}
#if USE_SSR_L_UL == USE
		if (g_bDestroy)
		{
			m_bRun = 0;
			break;
		}
#endif
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
