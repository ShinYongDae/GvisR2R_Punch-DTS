// DlgUtil02.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgUtil02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil02 dialog


CDlgUtil02::CDlgUtil02(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil02::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
}

CDlgUtil02::~CDlgUtil02()
{
	DelImg();

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgUtil02::Create()
{
	return CDialog::Create(CDlgUtil02::IDD);
}

void CDlgUtil02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil02, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil02)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil02 message handlers

void CDlgUtil02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
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

void CDlgUtil02::AtDlgShow()
{
	LoadImg();
}

void CDlgUtil02::AtDlgHide()
{
	DelImg();
}

void CDlgUtil02::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_UTIL02_BTN; i++)
	{
		if(i==1)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgUtil02, BTN_IMG_DN);
		}
		else if(i==2)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgUtil02, BTN_IMG_DN);
		}
		else if(i==6)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgUtil02, BTN_IMG_DN);
		}
		else if(i==7)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgUtil02, BTN_IMG_DN);
		}
		else if(i==8)
		{
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgUtil02, BTN_IMG_DN);
		}
		else if(i==9)
		{
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgUtil02, BTN_IMG_DN);
		}
		else
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgUtil02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgUtil02, BTN_IMG_DN);
		}
	}
}

void CDlgUtil02::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_UTIL02_BTN; i++)
		myBtn[i].DelImgList();
}

BOOL CDlgUtil02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUtil02::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_LT_ON, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LT_ON);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);
	myBtn[1].SubclassDlgItem(IDC_BTN_LT_UP, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_UP);
	myBtn[1].SetBoarder(FALSE);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);
	myBtn[2].SubclassDlgItem(IDC_BTN_LT_DN, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_DN);
	myBtn[2].SetBoarder(FALSE);
	myBtn[3].SubclassDlgItem(IDC_BTN_PIN_MOVE, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_MOVE);
	myBtn[4].SubclassDlgItem(IDC_BTN_PIN_SAVE, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_SAVE);
	myBtn[5].SubclassDlgItem(IDC_BTN_HOME_MOVE, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_HOME_MOVE);
	myBtn[6].SubclassDlgItem(IDC_BTN_JOG_UP, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_UP);
	myBtn[6].SetBoarder(FALSE);
	myBtn[7].SubclassDlgItem(IDC_BTN_JOG_DN, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_DN);
	myBtn[7].SetBoarder(FALSE);
	myBtn[8].SubclassDlgItem(IDC_BTN_JOG_LF, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_LF);
	myBtn[8].SetBoarder(FALSE);
	myBtn[9].SubclassDlgItem(IDC_BTN_JOG_RT, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_RT);
	myBtn[9].SetBoarder(FALSE);
	myBtn[10].SubclassDlgItem(IDC_CHK_JOG_VF, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VF);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);
	myBtn[11].SubclassDlgItem(IDC_CHK_JOG_F, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_F);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);
	myBtn[12].SubclassDlgItem(IDC_CHK_JOG_N, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_N);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);
	myBtn[13].SubclassDlgItem(IDC_CHK_JOG_S, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_S);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);
	myBtn[14].SubclassDlgItem(IDC_CHK_JOG_VS, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VS);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);
	myBtn[15].SubclassDlgItem(IDC_BTN_ALIGN, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN);

	int i;
	for(i=0; i<MAX_UTIL02_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"),16,TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
//		myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}
}

void CDlgUtil02::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgUtil02::InitStcTitle()
{
	myStcTitle[0].SubclassDlgItem(IDC_STC_JOG, this);

	for(int i=0; i<MAX_UTIL02_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgUtil02::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_LT_VAL, this);

	for(int i=0; i<MAX_UTIL02_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(14);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

BOOL CDlgUtil02::PreTranslateMessage(MSG* pMsg) 
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
