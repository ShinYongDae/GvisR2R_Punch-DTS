// ThreadTask.cpp: implementation of the CThreadTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Global/GlobalDefine.h"
#include "ThreadTask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CThreadTask::CThreadTask()
{
	m_hWnd = NULL;
	m_pThread = NULL;

	m_bPause = FALSE;
	m_hShutdownEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hPauseEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

CThreadTask::~CThreadTask()
{
	ShutdownThread();
	::CloseHandle(m_hShutdownEvent);
	::CloseHandle(m_hPauseEvent);
}

//+------------------------------------------------------
// Public method to pause a running thread
//+------------------------------------------------------
void CThreadTask::Pause()
{
	if( NULL != m_pThread )
	{
		m_pThread->SuspendThread();
		m_bPause = TRUE;
		//ResetEvent(m_hPauseEvent);
	}
}

//+------------------------------------------------------
// Public method to resume a suspended thread
//+------------------------------------------------------
void CThreadTask::Resume()
{
	if( NULL != m_pThread )
	{
		m_pThread->ResumeThread();
		m_bPause = FALSE;
		//SetEvent(m_hPauseEvent);
	}
}

//+------------------------------------------------------
// Public method to start a worker thread
//+------------------------------------------------------
HRESULT CThreadTask::Start( HWND hWnd, LPVOID lpContext,LPVOID lpThreadProc)
{
	if(lpThreadProc !=NULL)
	{
		m_pParentWnd = reinterpret_cast< CWnd* >( lpContext );
		m_pThreadProc = (AFX_THREADPROC)lpThreadProc;
	}
	else
	{
		m_pParentWnd = (CWnd*)this;
		m_pThreadProc = &ThreadProc;
	}

	HRESULT hr = S_OK;

	m_hWnd = hWnd;

	::ResetEvent(m_hShutdownEvent);
	::ResetEvent(m_hPauseEvent);


	m_bPause = FALSE;

	if( SUCCEEDED( hr = ShutdownThread() ) )
	{
        hr = CreateThread();
	}
	return hr;
}
	
//+------------------------------------------------------
// Public method to stop a running thread
//+------------------------------------------------------
HRESULT CThreadTask::Stop()
{
	m_hWnd = NULL;
	return ShutdownThread();
}

HRESULT CThreadTask::WaitThreadEnd(DWORD dwTimeOut) 
{
	TRACE(_T("WaitThreadEnd(0x%08x) Entering...\n"),m_pThread->m_hThread);
	if(dwTimeOut == 0)
		dwTimeOut = 1000*60*60*24; // max 1 day wait

	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode = STILL_ACTIVE;
	MSG message;

	while(GetExitCodeThread(m_pThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{
			// Time Out Check
			if (GetTickCount() >= (dwStartTick + dwTimeOut))
			{
				AfxMessageBox(_T("WaitThreadEnd() Time Out!!!"),NULL, MB_OK|MB_ICONSTOP);
				return E_FAIL;
			}
			if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
			Sleep(0);
			TRACE(_T("WaitThreadEnd(0x%08x) FInished...\n"),m_pThread->m_hThread);
	}

	
	return S_OK;
}

//+------------------------------------------------------
// Helper method to create the secondary thread
//+------------------------------------------------------
HRESULT CThreadTask::CreateThread()
{
    // Fire off the thread
	m_pThread = AfxBeginThread(m_pThreadProc,
							static_cast<LPVOID>(m_pParentWnd),
							THREAD_PRIORITY_NORMAL,
							0,
							CREATE_SUSPENDED);
	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

    return S_OK;
}

//+------------------------------------------------------
// Called by the secondary thread to get the shutdown event handle	//
//+------------------------------------------------------
HANDLE& CThreadTask::GetShutdownEvent()
{
	return m_hShutdownEvent;
}

HANDLE& CThreadTask::GetPauseEvent()
{
	return m_hPauseEvent;
}

BOOL CThreadTask::GetPauseStatus()
{
	return m_bPause;
}

//+------------------------------------------------------
// Called by the secondary thread to post a inc progress or 
// thread completed user defined message
//+------------------------------------------------------
void CThreadTask::NotifyUI( UINT uNotificationType )
{
	// Check if the hWnd is still valid before posting the message
	// Note: use PostMessage instead of SendMessage because PostMessage
	// performs an asynchronous post; whereas SendMessage sends synchronously
	// (sending synchronously would kind of defeat the purpose of using a
	// worker thread)
	if( ::IsWindow( m_pParentWnd->m_hWnd ) )
	{
		switch( uNotificationType )
		{
		case NOTIFY_INC_PROGRESS:
			::PostMessage( m_pParentWnd->m_hWnd, WM_USER_INC_PROGRESS, 0, 0 );
			break;

		case NOTIFY_TASK_COMPLETED:
			::PostMessage( m_pParentWnd->m_hWnd, WM_USER_TASK_COMPLETED, 0, 0 );
			break;

		case NOTIFY_TASK_THREAD_COMPLETED:
			::PostMessage( m_pParentWnd->m_hWnd, WM_USER_TASK_THREAD_COMPLETED, 0, 0 );
			break;
		default:
			ASSERT( 0 );
		}
	}
}

//+------------------------------------------------------
// Helper method to shut down the secondary thread.
// Uses a shutdown event to signal the thread to exit
//+------------------------------------------------------
HRESULT CThreadTask::ShutdownThread()
{
#ifdef _DEBUG
	DWORD EXIT_TIME = 6000;
#else
	DWORD EXIT_TIME = 6000;
#endif

	HRESULT hr = S_OK;
	
	// Close the worker thread
	if( NULL != m_pThread )
	{
		// Signal the thread to exit
		::SetEvent( m_hShutdownEvent );
		Sleep(30);

		// This thread may be suspended, so resume before shutting down
		m_pThread->ResumeThread();
		Sleep(30);

		// Wait for the thread to exit. If it doesn't shutdown on its own, force it closed with Terminate thread        
		if (WAIT_TIMEOUT == WaitForSingleObject( m_pThread->m_hThread, EXIT_TIME))	
		{
			::TerminateThread( m_pThread->m_hThread, -10000 ); // force terminate thread
			hr = S_FALSE; 
			Sleep(30);
		}

		DWORD dwExitCode;
// 		::GetExitCodeThread(m_pThread->m_hThread,&dwExitCode);
		while((GetExitCodeThread(m_pThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE))
		{
			Sleep(30);
		}

		delete m_pThread;
		m_pThread = NULL;
	}

	// Reset the shutdown event
	::ResetEvent( m_hShutdownEvent );

	return hr;

}

//+------------------------------------------------------
// Secondary thread procedure
//+------------------------------------------------------
UINT CThreadTask::ThreadProc( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CThreadTask* pThread = reinterpret_cast< CThreadTask* >( lpContext );
	pThread->m_csThreadProc.Lock();

	CString strMsg;

	DWORD dwTick = 0;
	DWORD dwTimePeriod = 10;

	do{
		if( WAIT_OBJECT_0 == WaitForSingleObject( pThread->GetShutdownEvent(), dwTimePeriod) )
		{
			break;
		}

		if(dwTick == 0)
		{
			strMsg = _T("Thread Task0 Message");
			pThread->NotifyUI(NOTIFY_INC_PROGRESS);
		}

		++dwTick;

		if(dwTick == dwTimePeriod/10)
			dwTick = 0;
		
	}while(1);

	// Send the thread completed message to the UI
	pThread->NotifyUI( NOTIFY_TASK_THREAD_COMPLETED );

	pThread->m_csThreadProc.Unlock();
	return 0;
}
