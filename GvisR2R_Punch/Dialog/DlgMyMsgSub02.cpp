// DlgMyMsgSub02.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgMyMsg.h"
#include "DlgMyMsgSub02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub02 dialog


CDlgMyMsgSub02::CDlgMyMsgSub02(CWnd* pParent, int nIDD)
	: CDlgMyMsg(pParent, nIDD)
{
	//{{AFX_DATA_INIT(CDlgMyMsgSub02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hParentWnd = NULL;
	m_pParent = pParent;
	m_pRect = NULL;
	m_bLoadImg = FALSE;
	
	StartThread();	
}

CDlgMyMsgSub02::~CDlgMyMsgSub02()
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

BOOL CDlgMyMsgSub02::Create()
{
	return CDialog::Create(CDlgMyMsgSub02::IDD);
}

void CDlgMyMsgSub02::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CDlgMyMsgSub02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMyMsgSub02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMyMsgSub02, CDialog)
	//{{AFX_MSG_MAP(CDlgMyMsgSub02)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_MESSAGE(WM_MYMSG02, OnMyMsg02)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub02 message handlers

BOOL CDlgMyMsgSub02::PreTranslateMessage(MSG* pMsg) 
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

BOOL CDlgMyMsgSub02::CloseMsgDlg() 
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


LRESULT CDlgMyMsgSub02::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_BTN_00: // Cancel...
		break;
	}
	return 0L;
}


LRESULT CDlgMyMsgSub02::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_BTN_00:
		SetRtnVal(IDOK);
		if(myBtn00.m_hParentWnd)
			myBtn00.Refresh();
		OnOK();
		break;
	}

	::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);
	return 0L;
}

void CDlgMyMsgSub02::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgMyMsgSub02::AtDlgShow()
{
	LoadImg();
}

void CDlgMyMsgSub02::AtDlgHide()
{
	DelImg();
}

void CDlgMyMsgSub02::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	myBtn00.LoadBkImage(MSG_SUB02_BTN_BK_UP, BTN_IMG_UP);
	myBtn00.LoadBkImage(MSG_SUB02_BTN_BK_DN, BTN_IMG_DN);
 	myBtn00.LoadImage(ICO_SUB02_BTN_EXIT, BTN_IMG_UP, CSize(40,40), BTN_POS_LEFT);
 	myBtn00.LoadImage(ICO_SUB02_BTN_EXIT, BTN_IMG_DN, CSize(40,40), BTN_POS_LEFT);
}

void CDlgMyMsgSub02::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	myBtn00.DelImgList();
}

BOOL CDlgMyMsgSub02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitPic();
	InitEdit();
	InitBtn();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMyMsgSub02::InitPic()
{
	myPic.Load(MSG_SUB02_BTN_BK_DN);
}

void CDlgMyMsgSub02::InitBtn()
{
	myBtn00.SetFont(_T("굴림체"),24,TRUE);
	myBtn00.SetTextColor(RGB_BLACK);
	myBtn00.SubclassDlgItem(IDC_BTN_00, this);
	myBtn00.SetHwnd(this->GetSafeHwnd(), IDC_BTN_00);
}

void CDlgMyMsgSub02::InitEdit()
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

void CDlgMyMsgSub02::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);
	myPic.Show(&dc,rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgMyMsgSub02::KillFocus(int nID)
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

void CDlgMyMsgSub02::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMyMsgSub02::OnMyMsg02(WPARAM wPara, LPARAM lPara)
{
	if(!this->IsWindowVisible())
		this->ShowWindow(SW_SHOW);
	return 0L;
}


void CDlgMyMsgSub02::StartThread() 
{
	m_ThreadTask.Start( GetSafeHwnd(),this,ThreadProc);// Start the thread
}

void CDlgMyMsgSub02::StopThread() 
{
	long hResult = (long)m_ThreadTask.Stop();// Stop the thread
}

// Home thread body
BOOL CDlgMyMsgSub02::ThreadProc( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CDlgMyMsgSub02* pThread = reinterpret_cast< CDlgMyMsgSub02* >( lpContext );

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
