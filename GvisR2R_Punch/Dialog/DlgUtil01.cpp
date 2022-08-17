// DlgUtil01.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgUtil01.h"

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
// CDlgUtil01 dialog


CDlgUtil01::CDlgUtil01(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil01::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
//	m_pVision = NULL;
}

CDlgUtil01::~CDlgUtil01()
{
// 	if(m_pVision)
// 	{
// 		delete m_pVision;
// 		m_pVision = NULL;
// 	}
// 	Sleep(300);
// 
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgUtil01::Create()
{
	return CDialog::Create(CDlgUtil01::IDD);
}

void CDlgUtil01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil01, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil01)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil01 message handlers

void CDlgUtil01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 375;
		m_pRect->bottom += 375 + GetSystemMetrics(SM_CYSIZE);
		m_pRect->left = 3;
		m_pRect->right += 3;
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

void CDlgUtil01::AtDlgShow()
{

}

void CDlgUtil01::AtDlgHide()
{

}

BOOL CDlgUtil01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

// 	HWND hW = GetDlgItem(IDC_STC_VISION)->GetSafeHwnd();
// 	CRect rt;
// 	GetDlgItem(IDC_STC_VISION)->GetWindowRect(&rt);

// 	if(pView->m_pVision[0])
// 	{
// 		pView->m_pVision[0]->SelLive(hW, rt);
// 
// 		pView->m_pVision[0]->ClearOverlay();
// 		pView->m_pVision[0]->DrawCenterMark();
// 	}

	CString str;
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[0]);
	GetDlgItem(IDC_STC_DEF_0)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[1]);
	GetDlgItem(IDC_STC_DEF_1)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[2]);
	GetDlgItem(IDC_STC_DEF_2)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[3]);
	GetDlgItem(IDC_STC_DEF_3)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[4]);
	GetDlgItem(IDC_STC_DEF_4)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[5]);
	GetDlgItem(IDC_STC_DEF_5)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[6]);
	GetDlgItem(IDC_STC_DEF_6)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[7]);
	GetDlgItem(IDC_STC_DEF_7)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[8]);
	GetDlgItem(IDC_STC_DEF_8)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[9]);
	GetDlgItem(IDC_STC_DEF_9)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[10]);
	GetDlgItem(IDC_STC_DEF_10)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[11]);
	GetDlgItem(IDC_STC_DEF_11)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[12]);
	GetDlgItem(IDC_STC_DEF_12)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[13]);
	GetDlgItem(IDC_STC_DEF_13)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[14]);
	GetDlgItem(IDC_STC_DEF_14)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[15]);
	GetDlgItem(IDC_STC_DEF_15)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[16]);
	GetDlgItem(IDC_STC_DEF_16)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[17]);
	GetDlgItem(IDC_STC_DEF_17)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[18]);
	GetDlgItem(IDC_STC_DEF_18)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[19]);
	GetDlgItem(IDC_STC_DEF_19)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[20]);
	GetDlgItem(IDC_STC_DEF_20)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[21]);
	GetDlgItem(IDC_STC_DEF_21)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[22]);
	GetDlgItem(IDC_STC_DEF_22)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[23]);
	GetDlgItem(IDC_STC_DEF_23)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[24]);
	GetDlgItem(IDC_STC_DEF_24)->SetWindowText(str);
	str.Format(_T("[%c]"), pDoc->m_cBigDefCode[25]);
	GetDlgItem(IDC_STC_DEF_25)->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgUtil01::PreTranslateMessage(MSG* pMsg) 
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
