// DlgMyMsgSub00.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgMyMsg.h"
#include "DlgMyMsgSub00.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub00 dialog


CDlgMyMsgSub00::CDlgMyMsgSub00(CWnd* pParent, int nIDD)
	: CDlgMyMsg(pParent, nIDD)
{
	//{{AFX_DATA_INIT(CDlgMyMsgSub00)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	if (pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	m_pRect = NULL;
	m_bExit = TRUE;
	m_bLoadImg = FALSE;

	m_sMsg = _T("");
	m_sPrev = _T("");
	m_sPrevEdit = _T("");
	m_pStcTime = NULL;

	m_bResetTime = TRUE;
}

CDlgMyMsgSub00::~CDlgMyMsgSub00()
{
	StopThread();

	while(m_bThreadAlive)
	{
		Sleep(100);
	}

	AtDlgHide();

	if(m_pRect)	
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgMyMsgSub00::Create()
{
	return CDialog::Create(CDlgMyMsgSub00::IDD);
}

void CDlgMyMsgSub00::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CDlgMyMsgSub00::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMyMsgSub00)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMyMsgSub00, CDialog)
	//{{AFX_MSG_MAP(CDlgMyMsgSub00)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_MESSAGE(WM_MYMSG00, OnMyMsg00)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub00 message handlers

BOOL CDlgMyMsgSub00::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgMyMsgSub00::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_BTN_00: // Cancel...
		//CloseMsgDlg();
		m_nStTimer = GetTickCount() - m_nTimeOut;
		if(myBtn00.m_hParentWnd)
			myBtn00.Refresh();
		break;
	}
	return 0L;
}


LRESULT CDlgMyMsgSub00::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_BTN_00:
		if(myBtn00.m_hParentWnd)
			myBtn00.Refresh();
		break;
	}

	//::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);
	return 0L;
}

void CDlgMyMsgSub00::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		if(m_nPosX < -10000 || m_nPosY < -10000)
		{
			m_nPosX = 0;
			m_nPosY = 0;
		}
		
		m_pRect = new CRect;	
	
		CRect rect(0,0,0,0);
		GetClientRect(m_pRect);
		//ClientToScreen(m_pRect);
		int nHeight = m_pRect->Height();
		int nWidth = m_pRect->Width();
		m_pRect->top = (1024 - nHeight) *29/30 + m_nPosY;
		m_pRect->bottom = m_pRect->top + nHeight + m_nPosY;
		m_pRect->left = (1280 - nWidth) / 2 + m_nPosX;
		m_pRect->right = m_pRect->left + nWidth + m_nPosX;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMyMsgSub00::AtDlgShow()
{
	LoadImg();

	m_nStTimer = (int)GetTickCount();
	StartThread();	
	
	if(myBtn00.GetCheck())
		myBtn00.SetCheck(FALSE);
}

void CDlgMyMsgSub00::AtDlgHide()
{
	DelImg();
}

void CDlgMyMsgSub00::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	myLabel00.LoadBkImage(MSG_KEYPAD_UP);
	
	myBtn00.LoadBkImage(MSG_SUB00_BTN_BK_UP, BTN_IMG_UP);
	myBtn00.LoadBkImage(MSG_SUB00_BTN_BK_DN, BTN_IMG_DN);
 	myBtn00.LoadImage(ICO_SUB00_BTN_CANCEL, BTN_IMG_UP, CSize(30,30), BTN_POS_LEFT);
 	myBtn00.LoadImage(ICO_SUB00_BTN_CANCEL, BTN_IMG_DN, CSize(30,30), BTN_POS_LEFT);
}

void CDlgMyMsgSub00::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	myLabel00.DelImgList();
	myBtn00.DelImgList();
}

BOOL CDlgMyMsgSub00::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nTimeOut = GetTimeOut();

	InitPic();
	InitStatic();
	InitEdit();
	InitLabel();
	InitBtn();
	
	if(!m_bExit)
	{
		GetDlgItem(IDC_BTN_00)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMyMsgSub00::InitPic()
{
	myPic.Load(MSG_SUB00_BTN_BK_DN);
}

void CDlgMyMsgSub00::InitStatic()
{
	m_stc00.SubclassDlgItem(IDC_STATIC_01, this);
	m_stc00.SetBkColor(RGB_WHITE);
	m_stc00.SetTextColor(RGB_BLACK);
	m_stc00.SetFontName(_T("±¼¸²"));
	m_stc00.SetFontSize(20);
	m_stc00.SetFontBold(TRUE);

	m_hStcTime = ::GetDlgItem(this->GetSafeHwnd(), IDC_STATIC_01);
	m_pStcTime = GetDlgItem(IDC_STATIC_01);
}

void CDlgMyMsgSub00::InitBtn()
{
	myBtn00.SetFont(_T("±¼¸²Ã¼"),14,TRUE);
	myBtn00.SetTextColor(RGB_BLUE);
	myBtn00.SubclassDlgItem(IDC_BTN_00, this);
	myBtn00.SetHwnd(this->GetSafeHwnd(), IDC_BTN_00);
}

void CDlgMyMsgSub00::InitLabel()
{
	myLabel00.SubclassDlgItem(IDC_STATIC_00, this);
	myLabel00.SetFontName(_T("Arial"));
	myLabel00.SetFontSize(22);
	myLabel00.SetFontBold(TRUE);
	myLabel00.SetTextColor(RGB_BLUE);
	myLabel00.DrawBoarder(0,0);
}

void CDlgMyMsgSub00::InitEdit()
{
	myEdit00.SubclassDlgItem(IDC_EDIT1, this);
	myEdit00.SetFontName(_T("Arial"));
	myEdit00.SetFontSize(FONT_SIZE+LINE_SPACE);
	myEdit00.SetFontBold(TRUE);
	myEdit00.SetTextColor(RGB_BLUE);
	myEdit00.SetBkColor(RGB_LTYELLOW);
	myEdit00.SetText(_T(""));
	myEdit00.SetFont((CFont*)&myEdit00.m_font, TRUE);
	KillFocus(IDC_EDIT1);
	
}

void CDlgMyMsgSub00::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);
	myPic.Show(&dc,rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgMyMsgSub00::KillFocus(int nID)
{
	CString sMsg[3];
	sMsg[0] = _T(""); sMsg[1] = _T(""); sMsg[2] = _T("");

	if(myEdit00.m_hParentWnd)
		myEdit00.SetFontSize(FONT_SIZE);
	sMsg[0] = GetMsg();
	if(!sMsg[0].IsEmpty())
	{
		RemoveMsg();
		while(sMsg[0].GetLength() > 38)
		{
			sMsg[1] = sMsg[0].Left(38);
			sMsg[0].Delete(0,38);
			sMsg[2] += sMsg[1];
		}
		sMsg[2] += sMsg[0];

		if(m_sPrevEdit != sMsg[2])
		{
			m_sPrevEdit = sMsg[2];
			myEdit00.SetText(sMsg[2]);
			GetDlgItem(IDC_BTN_00)->SetFocus();
		}
	}
}

void CDlgMyMsgSub00::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

//void CDlgMyMsgSub00::DispTime(HWND hDlg) 
void CDlgMyMsgSub00::DispTime() 
{
	CString str;
	int nCurr = (int)GetTickCount();
	int nHour = (int((m_nTimeOut+m_nStTimer-nCurr) / (60*60*1000))) % 24;
	int nMin = (int((m_nTimeOut+m_nStTimer-nCurr) / (60*1000))) % 60;
	int nSec = (int((m_nTimeOut+m_nStTimer-nCurr) / (1000))) % 60;
	int nDot = (int((m_nTimeOut+m_nStTimer-nCurr) / (100))) % 10;
	str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);

	if(m_sPrev != str)
	{
		m_sPrev = str;
		if(m_stc00.GetSafeHwnd())
		{
			m_stc00.SetText(str);	
			m_stc00.Refresh();
		}
	}

	KillFocus(IDC_EDIT1);
	
}

void CDlgMyMsgSub00::Refresh() 
{
	m_nTimeOut = GetTimeOut();
	m_nStTimer = GetTickCount();
	myEdit00.SetText(GetMsg());
}

void CDlgMyMsgSub00::CloseMsgDlg() 
{
	::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);
}


LRESULT CDlgMyMsgSub00::OnMyMsg00(WPARAM wPara, LPARAM lPara)
{
	if(!this->IsWindowVisible())
		this->ShowWindow(SW_SHOW);

	return 0L;
}


void CDlgMyMsgSub00::StartThread() 
{
	m_ThreadTask.Start( GetSafeHwnd(),this,ThreadProc);// Start the thread
}

void CDlgMyMsgSub00::StopThread() 
{
	long hResult = (long)m_ThreadTask.Stop();// Stop the thread
}

// Home thread body
BOOL CDlgMyMsgSub00::ThreadProc( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CDlgMyMsgSub00* pThread = reinterpret_cast< CDlgMyMsgSub00* >( lpContext );

	BOOL bLock=FALSE;
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period
	int nStTime = pThread->m_nStTimer;
	int nTimeOut = pThread->m_nTimeOut;
	int nCurr = (int)GetTickCount();
	Sleep(100);

	pThread->m_bThreadAlive = TRUE;
	while( WAIT_OBJECT_0 != WaitForSingleObject( pThread->m_ThreadTask.GetShutdownEvent(), dwShutdownEventCheckPeriod) )
	{
		HWND hWnd = pThread->GetSafeHwnd();
		if(hWnd)
			pThread->DispTime();
		
		nStTime = pThread->m_nStTimer;
		nCurr = (int)GetTickCount();
		if(nCurr-nStTime > nTimeOut)
		{
			break;
		}
		Sleep(10);
	}

	pThread->CloseMsgDlg();
	pThread->m_bThreadAlive = FALSE;

	return 0;
}
