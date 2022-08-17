// ThreadTask0.h: interface for the CThreadTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADTASK_H__947140DF_748A_4B8D_A964_38B01ED19261__INCLUDED_)
#define AFX_THREADTASK_H__947140DF_748A_4B8D_A964_38B01ED19261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThreadTask  
{
public:
	CThreadTask();
	virtual ~CThreadTask();
	AFX_THREADPROC m_pThreadProc;
	CCriticalSection m_csThreadProc;
protected:
	// Determines which type of user defined message to post
	enum	NotificationTypes { NOTIFY_INC_PROGRESS,NOTIFY_TASK_COMPLETED ,NOTIFY_TASK_THREAD_COMPLETED };

// Public interface methods
public:

	void Pause(); // Public method to pause a running thread
	void Resume();// Public method to resume a suspended thread
	HRESULT Start(HWND hWnd, LPVOID lpContext,LPVOID lpThreadProc=NULL);// Public method to start a worker thread
	HRESULT Stop();// Public method to stop a running thread
	HRESULT WaitThreadEnd(DWORD dwTimeOut=0);
	
// Implementation
//private:
    HRESULT CreateThread();// Helper method to create the secondary thread
	HANDLE& GetShutdownEvent();// Called by the secondary thread to get the shutdown event handle
	HANDLE& GetPauseEvent();
	BOOL GetPauseStatus();

	void NotifyUI(UINT uNotificationType);// Called by the secondary thread to post a inc progress or thread completed user defined message 
	HRESULT ShutdownThread();// Helper method to shut down the secondary thread. Uses a shutdown event to signal the thread to exit
	static UINT ThreadProc(LPVOID lpContext); // Secondary thread procedure



// Private fields
private:
	HWND	m_hWnd;				// Window handle to the UI dialog (used to
								// post progress and completed messages)
    HANDLE	m_hShutdownEvent;	// Shutdown Event handle (causes thread to exit)
	HANDLE	m_hPauseEvent;		// Pause Event handle (causes thread to suspend)
	BOOL	m_bPause;

	CWinThread *m_pThread;
	CWnd*	m_pParentWnd;

};
#endif // !defined(AFX_THREADTASK_H__947140DF_748A_4B8D_A964_38B01ED19261__INCLUDED_)
