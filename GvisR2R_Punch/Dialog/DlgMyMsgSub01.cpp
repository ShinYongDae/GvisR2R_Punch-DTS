// DlgMyMsgSub01.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgMyMsg.h"
#include "DlgMyMsgSub01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub01 dialog


CDlgMyMsgSub01::CDlgMyMsgSub01(CWnd* pParent, int nIDD)
	: CDlgMyMsg(pParent, nIDD)
{
	//{{AFX_DATA_INIT(CDlgMyMsgSub01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hParentWnd = NULL;
	m_pParent = pParent;
	m_pRect = NULL;
	m_bLoadImg = FALSE;
	
	StartThread();	
}

CDlgMyMsgSub01::~CDlgMyMsgSub01()
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

BOOL CDlgMyMsgSub01::Create()
{
	return CDialog::Create(CDlgMyMsgSub01::IDD);
}

void CDlgMyMsgSub01::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CDlgMyMsgSub01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMyMsgSub01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMyMsgSub01, CDialog)
	//{{AFX_MSG_MAP(CDlgMyMsgSub01)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_MESSAGE(WM_MYMSG01, OnMyMsg01)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub01 message handlers

BOOL CDlgMyMsgSub01::PreTranslateMessage(MSG* pMsg) 
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
		case 'S':
		case 's':
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE; 
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);	
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CDlgMyMsgSub01::CloseMsgDlg() 
{
	//::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);

	if(m_pParent)
	{
		if(((CDlgMyMsg*)m_pParent)->m_bCloseMsg)
		{
			OnOK();
			return TRUE;
		}
	}
	else
		return TRUE;

	return FALSE;
}

LRESULT CDlgMyMsgSub01::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_BTN_00:
		break;
	case IDC_BTN_01:
		break;
	}
	return 0L;
}


LRESULT CDlgMyMsgSub01::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_BTN_00:
		SetRtnVal(IDYES);
		if(myBtn00.m_hParentWnd)
			myBtn00.Refresh();
		OnOK();
		break;
	case IDC_BTN_01:
		SetRtnVal(IDNO);
		if(myBtn00.m_hParentWnd)
			myBtn01.Refresh();
		OnCancel();
		break;
	}

	::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);
	return 0L;
}

void CDlgMyMsgSub01::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgMyMsgSub01::AtDlgShow()
{
	LoadImg();
}

void CDlgMyMsgSub01::AtDlgHide()
{
	DelImg();
}

void CDlgMyMsgSub01::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	myBtn00.LoadBkImage(MSG_SUB01_BTN_BK_UP, BTN_IMG_UP);
	myBtn00.LoadBkImage(MSG_SUB01_BTN_BK_DN, BTN_IMG_DN);
 	myBtn00.LoadImage(ICO_SUB01_BTN_EXIT, BTN_IMG_UP, CSize(40,40), BTN_POS_LEFT);
 	myBtn00.LoadImage(ICO_SUB01_BTN_EXIT, BTN_IMG_DN, CSize(40,40), BTN_POS_LEFT);

	myBtn01.LoadBkImage(MSG_SUB01_BTN_BK_UP, BTN_IMG_UP);
	myBtn01.LoadBkImage(MSG_SUB01_BTN_BK_DN, BTN_IMG_DN);
 	myBtn01.LoadImage(ICO_SUB01_BTN_CANCEL, BTN_IMG_UP, CSize(40,40), BTN_POS_LEFT);
 	myBtn01.LoadImage(ICO_SUB01_BTN_CANCEL, BTN_IMG_DN, CSize(40,40), BTN_POS_LEFT);
}

void CDlgMyMsgSub01::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	myBtn00.DelImgList();
	myBtn01.DelImgList();
}

BOOL CDlgMyMsgSub01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitPic();
	InitEdit();
	InitBtn();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMyMsgSub01::InitPic()
{
	myPic.Load(MSG_SUB01_BTN_BK_DN);
}

void CDlgMyMsgSub01::InitBtn()
{
	myBtn00.SetFont(_T("굴림체"),24,TRUE);
	myBtn00.SetTextColor(RGB_BLACK);
	myBtn00.SubclassDlgItem(IDC_BTN_00, this);
	myBtn00.SetHwnd(this->GetSafeHwnd(), IDC_BTN_00);

	myBtn01.SetFont(_T("굴림체"),24,TRUE);
	myBtn01.SetTextColor(RGB_BLACK);
	myBtn01.SubclassDlgItem(IDC_BTN_01, this);
	myBtn01.SetHwnd(this->GetSafeHwnd(), IDC_BTN_01);
}

void CDlgMyMsgSub01::InitEdit()
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

void CDlgMyMsgSub01::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);
	myPic.Show(&dc,rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgMyMsgSub01::KillFocus(int nID)
{
	CString sMsg[3];
	sMsg[0] = _T(""); sMsg[1] = _T(""); sMsg[2] = _T("");

	myEdit00.SetFontSize(FONT_SIZE);
	sMsg[0] = GetMsg();
	while(sMsg[0].GetLength() > 38)
	{
		sMsg[1] = sMsg[0].Left(38);
		sMsg[0].Delete(0,38);
		sMsg[2] += sMsg[1];
	}
	sMsg[2] += sMsg[0];
	myEdit00.SetText(sMsg[2]);
	GetDlgItem(IDC_BTN_00)->SetFocus();
}

void CDlgMyMsgSub01::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMyMsgSub01::OnMyMsg01(WPARAM wPara, LPARAM lPara)
{
	if(!this->IsWindowVisible())
		this->ShowWindow(SW_SHOW);

	return 0L;
}

void CDlgMyMsgSub01::StartThread() 
{
	m_ThreadTask.Start( GetSafeHwnd(),this,ThreadProc);// Start the thread
}

void CDlgMyMsgSub01::StopThread() 
{
	long hResult = (long)m_ThreadTask.Stop();// Stop the thread
}

// Home thread body
BOOL CDlgMyMsgSub01::ThreadProc( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CDlgMyMsgSub01* pThread = reinterpret_cast< CDlgMyMsgSub01* >( lpContext );

	BOOL bLock=FALSE;
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThreadAlive = TRUE;
	while( WAIT_OBJECT_0 != WaitForSingleObject( pThread->m_ThreadTask.GetShutdownEvent(), dwShutdownEventCheckPeriod) )
	{
		if(pThread->CloseMsgDlg())
			break;

		Sleep(100);
	}

	pThread->m_bThreadAlive = FALSE;

	return 0;
}
