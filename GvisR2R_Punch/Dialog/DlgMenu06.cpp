// DlgMenu06.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgMenu06.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu06 dialog


CDlgMenu06::CDlgMenu06(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu06::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_pRect = NULL;
	m_pMyGL_Inner = NULL;
	m_pMyGL_Outer = NULL;
}

CDlgMenu06::~CDlgMenu06()
{
	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if (m_pMyGL_Inner)
	{
		delete m_pMyGL_Inner;
		m_pMyGL_Inner = NULL;
	}

	if (m_pMyGL_Outer)
	{
		delete m_pMyGL_Outer;
		m_pMyGL_Outer = NULL;
	}
}

BOOL CDlgMenu06::Create()
{
	return CDialog::Create(CDlgMenu06::IDD);
}

void CDlgMenu06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu06, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu06)
	ON_MESSAGE(WM_DRAW_REELMAP_INNER, OnDrawReelMapInner)
	ON_MESSAGE(WM_DRAW_REELMAP_OUTER, OnDrawReelMapOuter)
	ON_MESSAGE(WM_MYSTATIC_REDRAW, OnMyStaticRedraw)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu06 message handlers

void CDlgMenu06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (!m_pRect)
	{
		m_pRect = new CRect;

		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if (bShow)
	{
		//this->MoveWindow(m_pRect, TRUE);
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu06::AtDlgShow()
{
}

void CDlgMenu06::AtDlgHide()
{
}


BOOL CDlgMenu06::PreTranslateMessage(MSG* pMsg) 
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


BOOL CDlgMenu06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGL();
	InitStatic();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMenu06::InitStatic()
{
	myStcReelmapInner.SubclassDlgItem(IDC_STC_REELMAP_INNER, this);
	myStcReelmapInner.SetHwnd(this->GetSafeHwnd(), IDC_STC_REELMAP_INNER);

	myStcReelmapOuter.SubclassDlgItem(IDC_STC_REELMAP_OUTER, this);
	myStcReelmapOuter.SetHwnd(this->GetSafeHwnd(), IDC_STC_REELMAP_OUTER);
}
void CDlgMenu06::InitGL()
{
	if (!m_pMyGL_Inner)
	{
		m_pMyGL_Inner = new CMyGL(this);
		m_pMyGL_Inner->Init(IDC_STC_REELMAP_INNER);
	}
	m_pMyGL_Inner->SetRgn();
	m_pMyGL_Inner->SetPcsDef();

	if (!m_pMyGL_Outer)
	{
		m_pMyGL_Outer = new CMyGL(this);
		m_pMyGL_Outer->Init(IDC_STC_REELMAP_OUTER);
	}
	m_pMyGL_Outer->SetRgn();
	m_pMyGL_Outer->SetPcsDef();
}

void CDlgMenu06::ResetGL()
{
	if (m_pMyGL_Inner)
		m_pMyGL_Inner->ResetRgn();

	if (m_pMyGL_Outer)
		m_pMyGL_Outer->ResetRgn();
}

void CDlgMenu06::SetPnlNum()
{
	if (m_pMyGL_Inner)
		m_pMyGL_Inner->SetPnlNum();

	if (m_pMyGL_Outer)
		m_pMyGL_Outer->SetPnlNum();
}

void CDlgMenu06::SetPnlDefNum()
{
	if (m_pMyGL_Inner)
		m_pMyGL_Inner->SetPnlDefNum();

	if (m_pMyGL_Outer)
		m_pMyGL_Outer->SetPnlDefNum();
}

LRESULT CDlgMenu06::OnDrawReelMapInner(WPARAM wPara, LPARAM lPara)
{
	if (m_pMyGL_Inner)
	{
		m_pMyGL_Inner->Draw();
	}

	return 0L;
}

LRESULT CDlgMenu06::OnDrawReelMapOuter(WPARAM wPara, LPARAM lPara)
{
	if (m_pMyGL_Outer)
	{
		m_pMyGL_Outer->Draw();
	}

	return 0L;
}

LRESULT CDlgMenu06::OnMyStaticRedraw(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;

	switch (nCtrlID)
	{
	case IDC_STC_REELMAP_INNER:
		this->PostMessage(WM_DRAW_REELMAP_INNER, 0, 0);
		break;
	case IDC_STC_REELMAP_OUTER:
		this->PostMessage(WM_DRAW_REELMAP_OUTER, 0, 0);
		break;
	}

	return 0L;
}

void CDlgMenu06::SetRmapRgn()
{
	if (m_pMyGL_Inner)
	{
		m_pMyGL_Inner->SetRgn();
	}

	if (m_pMyGL_Outer)
	{
		m_pMyGL_Outer->SetRgn();
	}
}

void CDlgMenu06::DispReelmap(int nSerial, BOOL bDumy)
{
	if (nSerial <= 0)
		return;

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->Disp(nSerial, bDumy);
	SetPnlNum();
	SetPnlDefNum();
	myStcReelmapInner.Refresh();
	myStcReelmapOuter.Refresh();

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::RefreshRmap()
{
	myStcReelmapInner.Refresh();
	myStcReelmapOuter.Refresh();

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}



BOOL CDlgMenu06::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}
