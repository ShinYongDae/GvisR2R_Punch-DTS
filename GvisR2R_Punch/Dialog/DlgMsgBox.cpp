// DlgMsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMsgBox.h"

#include "../GvisR2R_Punch.h"
#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgBox dialog
CDlgMsgBox::CDlgMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMsgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgMsgBox::CDlgMsgBox(CFormView* pView,CString strTitle,CString strMsg,DWORD dwInMilliSeconds,COLORREF color)
{
	m_pView = pView;
	m_pMsgBoxThread = NULL;
	m_bModify = FALSE;
	m_TextColor = color;
	m_strTitle = strTitle;
	m_strMsg = strMsg;
	m_DispTime = dwInMilliSeconds;
	m_pRect = NULL;
	m_pRectView = NULL;
}

CDlgMsgBox::~CDlgMsgBox()
{
	if(m_pRect)	
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if(!m_pRectView)
	{
		delete m_pRectView;
		m_pRectView = NULL;
	}
}

BOOL CDlgMsgBox::Create()
{
	return CDialog::Create(CDlgMsgBox::IDD);
}

void CDlgMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsgBox)
	DDX_Control(pDX, IDS_TITLE, m_lbTitleMsg);
	DDX_Control(pDX, IDS_TIME, m_lbRemainTime);
	DDX_Control(pDX, IDS_MSG, m_lbMsg);
	DDX_Control(pDX, IDS_LOGO, m_lbLogo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgMsgBox, CDialog)
	//{{AFX_MSG_MAP(CDlgMsgBox)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDS_TIME, OnTime)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgBox message handlers
void CDlgMsgBox::Close()
{
	//if(m_pView != NULL)	
	//	pVrsView->PostMessage( WM_QUIT_DLG_MSG_BOX, IDOK);
	PostMessage(WM_CLOSE);
}

void CDlgMsgBox::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
	
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if(m_pRectView)
	{
		delete m_pRectView;
		m_pRectView = NULL;
	}

	pView->OnQuitDispMsg(0,0);
	if(m_hBkColor)
		::DeleteObject(m_hBkColor);
}

BOOL CDlgMsgBox::DestroyWindow() 
{
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if(m_pRectView)
	{
		delete m_pRectView;
		m_pRectView = NULL;
	}

	StopThread();
	if(m_hBkColor)
		::DeleteObject(m_hBkColor);
	return CDialog::DestroyWindow();
}

BOOL CDlgMsgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rect;

	#define WS_EX_LAYERED           0x00080000
	#define LWA_COLORKEY            0x00000001
	#define LWA_ALPHA               0x00000002
	#define ULW_COLORKEY            0x00000001
	#define ULW_ALPHA               0x00000002
	#define ULW_OPAQUE              0x00000004
	typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

	SLWA pSetLayeredWindowAttributes = NULL;
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");

	HWND hwnd = this->m_hWnd;
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * 100) / 100, LWA_ALPHA);
    
	// 다이얼로그 윈도우의 사이즈를 얻는다. 
	this->GetWindowRect( &rect );
	int nSizeX = rect.right - rect.left;
	int nSizeY = rect.bottom - rect.top;

	// 스트링의 크기에 따라 윈도우 크기를 변경한다.
	// 이벤트 윈도우를 메인윈도우의 중앙에 위치 시킨다.
	AfxGetMainWnd()->GetWindowRect( &rect );
	
	MoveWindow( rect.left + ( rect.right - rect.left )/2 - nSizeX/2,
	            rect.bottom - ( rect.bottom - rect.top )/2 - nSizeY*3/4,
				nSizeX,nSizeY);	
	
	m_lbTitleMsg.SetBkColor(RGB_BLACK);
	m_lbTitleMsg.SetTextColor(m_TextColor);
	m_lbTitleMsg.SetFontName(_T("굴림"));
	m_lbTitleMsg.SetFontSize(24);
	m_lbTitleMsg.SetFontBold(TRUE);
	m_lbTitleMsg.SetText(m_strTitle);

	TansText();

	m_lbMsg.SetBkColor(RGB_BLACK);
	m_lbMsg.SetTextColor(RGB_WHITE);
	m_lbMsg.SetFontName(_T("굴림"));
	m_lbMsg.SetFontSize(12);
	m_lbMsg.SetFontBold(TRUE);
	m_lbMsg.SetText(m_strMsg);

	m_lbRemainTime.SetBkColor(RGB_BLACK);
	m_lbRemainTime.SetTextColor(RGB_GREEN);
	m_lbRemainTime.SetFontSize(15);
	m_lbRemainTime.SetFontBold(TRUE);
	m_lbRemainTime.SetText(_T(""));

	GetDlgItem(IDOK) ->SetWindowText(_T("확 인 (Exit)"));
	UpdateData(FALSE);

	m_hBkColor = CreateSolidBrush(RGB_CLOUDBLUE); 

	//StartThread();
	m_dwStartTick = GetTickCount();
	m_dwEndTick = m_dwStartTick + m_DispTime;
	SetTimer(0, 100, NULL);


	FreeLibrary(hmodUSER32); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgMsgBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;
	
	// Custom control(buttons..), must have owner draw style selected
//	switch (pWnd->GetDlgCtrlID())  
//	{     
//	case IDOK:
//		VERIFY(SetCtrlColor(IDOK,"Space Bar",RGB_LTBLUE,RGB_BLACK));
//		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//		;
//	}  

	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
//		pDC->SetBkMode(TRANSPARENT);
//	   	hbr = CreateSolidBrush(RGB_CLOUDBLUE); 

		return m_hBkColor;
		break;
	case CTLCOLOR_STATIC:
	case CTLCOLOR_EDIT:
	case CTLCOLOR_BTN:
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_MSGBOX:
	case CTLCOLOR_SCROLLBAR:
	default:
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgMsgBox::SetCtrlColor(const UINT nID, CString strText, COLORREF bgColor, COLORREF fgColor)
{
	RECT rect;
	CWnd* wndCtrl = NULL;
    CString str;

	wndCtrl = this->GetDescendantWindow(nID);
	// Get rectangular boundary of Control window
	// and reset coordinates to client space
	wndCtrl->GetWindowRect(&rect);
	wndCtrl->ScreenToClient(&rect);  
	// Get device context of button window
	CDC* dc = wndCtrl->GetDC();
	dc->FillSolidRect(&rect,bgColor);
	dc->SetTextColor(fgColor);
	wndCtrl->GetWindowText(str);
	dc->DrawText(strText,&rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc->DrawEdge(&rect, BDR_RAISEDINNER | BDR_RAISEDOUTER, BF_RECT);
	// Release device context each time
	ReleaseDC(dc);
	return TRUE;
}

void CDlgMsgBox::GetDispMsg(CString &strMsg, CString &strTitle)
{
	strMsg = m_strMsg;
	strTitle = m_strTitle;
}

void CDlgMsgBox::SetDispMsg(CString strMsg, CString strTitle, DWORD dwInMilliSeconds, COLORREF color)
{
	m_TextColor = color;
	m_strMsg = strMsg;
	m_strTitle = strTitle;
	m_lbTitleMsg.SetBkColor(RGB_BLACK);
	m_lbTitleMsg.SetTextColor(m_TextColor);
	m_lbTitleMsg.SetFontName(_T("굴림"));
	m_lbTitleMsg.SetFontSize(24);
	m_lbTitleMsg.SetFontBold(TRUE);
	m_lbTitleMsg.SetText(m_strTitle);

	m_lbMsg.SetBkColor(RGB_BLACK);
	m_lbMsg.SetTextColor(RGB_WHITE);
	m_lbMsg.SetFontName(_T("굴림"));
	m_lbMsg.SetFontSize(14);
	m_lbMsg.SetFontBold(TRUE);
	m_lbMsg.SetText(m_strMsg);
	
	m_lbRemainTime.SetBkColor(RGB_BLACK);
	m_lbRemainTime.SetTextColor(RGB_GREEN);
	m_lbLogo.SetBkColor(RGB_BLUE);
	m_lbLogo.SetTextColor(RGB_WHITE);

	m_DispTime = dwInMilliSeconds;
	m_bModify = TRUE;
	GetDlgItem(IDOK) ->SetWindowText(_T("확 인 (Exit)"));
}

void CDlgMsgBox::StartThread() // Worker Thread 구동관련 Step8
{
	if(m_pMsgBoxThread == NULL)
	{
		m_bModify = TRUE;
		m_evtMsgBoxThread.ResetEvent();
		m_pMsgBoxThread = AfxBeginThread(DispMsgThread, this);
		if(m_pMsgBoxThread)
			m_hMsgBoxThread = m_pMsgBoxThread->m_hThread;
	}
}

void CDlgMsgBox::StopThread() // Worker Thread 구동관련 Step9
{
	if(m_pMsgBoxThread != NULL)
	{	
		m_evtMsgBoxThread.SetEvent();
		WaitUntilThreadEnd(m_hMsgBoxThread);
	}
	m_pMsgBoxThread = NULL;
	m_bModify = FALSE;
}

void CDlgMsgBox::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
{
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Entering\n",hThread);
	MSG message;
	const DWORD dwTimeOut = 500000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while(GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE && m_bAlive) {
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut)) 
		{
			//pView->MsgBox("WaitUntilThreadEnd() Time Out!!!");
 			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!",NULL, MB_OK|MB_ICONSTOP));
			return;
		}
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Exit\n",hThread);
}

UINT CDlgMsgBox::DispMsgThread(LPVOID pParam)
{
	CDlgMsgBox* pDlg = (CDlgMsgBox*)pParam;
	DWORD dwMilliseconds = 10;
	CString strTemp;

	static int dHour,dMin,dSec,dSubSec;

	DWORD dwCurTick;
	DWORD dwTimeOut;
	pDlg->m_bAlive = TRUE;
	MSG message;
	while(WAIT_OBJECT_0 != ::WaitForSingleObject(pDlg->m_evtMsgBoxThread,dwMilliseconds))
	{
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		
		// Time Out Update
		if(pDlg->m_bModify)
		{
			dwTimeOut = pDlg->m_DispTime + GetTickCount();
			pDlg->m_bModify = FALSE;
		}
		// Time Out Check
		dwCurTick = GetTickCount();
		if (dwCurTick >= (dwTimeOut))
		{
			pDlg->Close();
			break;
		}
		else
		{
			HWND hWnd = pDlg->GetSafeHwnd();
			if(hWnd)
			{
				dHour = ((dwTimeOut-dwCurTick)/(1000*60*60))%24;
				dMin = ((dwTimeOut-dwCurTick)/(1000*60))%60;
				dSec = ((dwTimeOut-dwCurTick)/1000)%60;
				//dSubSec = (dwTimeOut-dwCurTick)%100;
				strTemp.Format(_T("Exit: %02d:%02d:%02d"),dHour,dMin,dSec);
				pDlg->m_lbRemainTime.SetText(strTemp);
			}
		}
		
	}
	pDlg->m_bAlive = FALSE;
	return 0;
}

void CDlgMsgBox::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class

	OnClose();
}

BOOL CDlgMsgBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)	
		{
		case VK_SPACE:
			int kk =1;
			return 0;
		break;
		}
		// PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
	}			
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMsgBox::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}

void CDlgMsgBox::TansText()
{
	m_lbLogo.SetBkColor(RGB_BLUE);
	m_lbLogo.SetTextColor(m_TextColor);
	m_lbLogo.SetFontName(_T("굴림"));
	m_lbLogo.SetFontSize(16);
	m_lbLogo.SetFontBold(TRUE);
#if LANGUAGE == KOREAN
	m_lbLogo.SetText(_T("기가비스 주식회사   www.gigavis.com"));
#elif LANGUAGE == ENGLISH
	m_lbLogo.SetText(_T("GIGAVIS CO.,LTD.    www.gigavis.com"));
#elif LANGUAGE == JAPANESE
	m_lbLogo.SetText(_T("GIGAVIS CO.,LTD.    www.gigavis.com"));
#endif
}

void CDlgMsgBox::OnTime() 
{
	// TODO: Add your message handler code here

	Close();	
}

void CDlgMsgBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow)
	{
		if(!m_pRect)
		{
			m_pRect = new CRect;
			this->GetClientRect(m_pRect);
			
// 			m_pRect->top = 740;
// 			m_pRect->bottom += 740;
// 			m_pRect->left = 350;
// 			m_pRect->right += 350;

			m_pRectView = new CRect;			
			pView->GetClientRect(m_pRectView);
			m_pRect->top = (m_pRectView->bottom+m_pRectView->top)/2 - (m_pRect->bottom-m_pRect->top)*3/4;
			m_pRect->bottom += m_pRect->top;
			m_pRect->left = (m_pRectView->right+m_pRectView->left)/2 - (m_pRect->right-m_pRect->left)/2;
			m_pRect->right += m_pRect->left;

			this->MoveWindow(m_pRect, TRUE);
			this->SetForegroundWindow();
		}
	}
}

void CDlgMsgBox::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString strTemp;

	static int dHour,dMin,dSec,dSubSec;

	DWORD dwCurTick;

	if(nIDEvent==0)
	{
		if(!this->IsWindowVisible() || this != GetForegroundWindow())
		{
			this->ShowWindow(SW_SHOW);
			this->SetForegroundWindow();
		}

		// Time Out Check
		dwCurTick = GetTickCount();
		if (dwCurTick >= m_dwEndTick)
		{
			KillTimer(0);
			this->Close();
		}
		else
		{
			dHour = ((m_dwEndTick-dwCurTick)/(1000*60*60))%24;
			dMin = ((m_dwEndTick-dwCurTick)/(1000*60))%60;
			dSec = ((m_dwEndTick-dwCurTick)/1000)%60;

			strTemp.Format(_T("Exit: %02d:%02d:%02d"),dHour,dMin,dSec);
			m_lbRemainTime.SetText(strTemp);
		}

	}
	
	CDialog::OnTimer(nIDEvent);
}
