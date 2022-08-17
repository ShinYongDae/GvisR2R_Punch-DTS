// DlgMyPassword.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgMyPassword.h"

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
// CDlgMyPassword dialog


CDlgMyPassword::CDlgMyPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMyPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMyPassword)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_nSel = 0;
	pView->m_bShowModalMyPassword = TRUE;
	m_nRtnVal = 0;
	m_strMsg = _T("");
	m_nCtrlId = 0;
	m_bTIM_DLG_MY_PASSWORD_Lock = FALSE;

	m_bFont = FALSE;
}

CDlgMyPassword::~CDlgMyPassword()
{
	AtDlgHide();

	if(m_pRect)	
	{
		delete m_pRect;
		m_pRect = NULL;
	}

    if(m_bFont)
	{
		if(m_Font.DeleteObject())
			m_bFont = FALSE;
	}

	pView->m_bShowModalMyPassword = FALSE;
}


void CDlgMyPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMyPassword)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMyPassword, CDialog)
	//{{AFX_MSG_MAP(CDlgMyPassword)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SYM00, OnButtonSym00)
	ON_BN_CLICKED(IDC_BUTTON_SYM01, OnButtonSym01)
	ON_BN_CLICKED(IDC_BUTTON_SYM02, OnButtonSym02)
	ON_BN_CLICKED(IDC_BUTTON_SYM03, OnButtonSym03)
	ON_BN_CLICKED(IDC_BUTTON_SYM04, OnButtonSym04)
	ON_BN_CLICKED(IDC_BUTTON_SYM05, OnButtonSym05)
	ON_BN_CLICKED(IDC_BUTTON_SYM06, OnButtonSym06)
	ON_BN_CLICKED(IDC_BUTTON_SYM07, OnButtonSym07)
	ON_BN_CLICKED(IDC_BUTTON_SYM08, OnButtonSym08)
	ON_BN_CLICKED(IDC_BUTTON_SYM09, OnButtonSym09)
	ON_BN_CLICKED(IDC_BUTTON_SYM10, OnButtonSym10)
	ON_BN_CLICKED(IDC_BUTTON_SYM11, OnButtonSym11)
	ON_BN_CLICKED(IDC_BUTTON_NUM1, OnButtonNum1)
	ON_BN_CLICKED(IDC_BUTTON_NUM2, OnButtonNum2)
	ON_BN_CLICKED(IDC_BUTTON_NUM3, OnButtonNum3)
	ON_BN_CLICKED(IDC_BUTTON_NUM4, OnButtonNum4)
	ON_BN_CLICKED(IDC_BUTTON_NUM5, OnButtonNum5)
	ON_BN_CLICKED(IDC_BUTTON_NUM6, OnButtonNum6)
	ON_BN_CLICKED(IDC_BUTTON_NUM7, OnButtonNum7)
	ON_BN_CLICKED(IDC_BUTTON_NUM8, OnButtonNum8)
	ON_BN_CLICKED(IDC_BUTTON_NUM9, OnButtonNum9)
	ON_BN_CLICKED(IDC_BUTTON_NUM0, OnButtonNum0)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, OnButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_EQUAL, OnButtonEqual)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_Q, OnButtonCharQ)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_W, OnButtonCharW)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_E, OnButtonCharE)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_R, OnButtonCharR)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_T, OnButtonCharT)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_Y, OnButtonCharY)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_U, OnButtonCharU)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_I, OnButtonCharI)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_O, OnButtonCharO)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_P, OnButtonCharP)
	ON_BN_CLICKED(IDC_BUTTON_ESC, OnButtonEsc)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_A, OnButtonCharA)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_S, OnButtonCharS)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_D, OnButtonCharD)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_F, OnButtonCharF)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_G, OnButtonCharG)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_H, OnButtonCharH)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_J, OnButtonCharJ)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_K, OnButtonCharK)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_L, OnButtonCharL)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_Z, OnButtonCharZ)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_X, OnButtonCharX)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_C, OnButtonCharC)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_V, OnButtonCharV)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_B, OnButtonCharB)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_N, OnButtonCharN)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_M, OnButtonCharM)
	ON_BN_CLICKED(IDC_BUTTON_COMMA, OnButtonComma)
	ON_BN_CLICKED(IDC_BUTTON_DOT, OnButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, OnButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SPACE, OnButtonSpace)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_BS, OnButtonBs)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_EN_SETFOCUS(IDC_EDIT_11, OnSetfocusEdit11)
	ON_EN_SETFOCUS(IDC_EDIT_21, OnSetfocusEdit21)
	ON_BN_CLICKED(IDC_BTN_00, OnBtn00)
	ON_EN_CHANGE(IDC_EDIT_11, OnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT_21, OnChangeEdit21)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_SHIFT, OnCheckShift)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_PW, OnMyPw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMyPassword message handlers

BOOL CDlgMyPassword::Create()
{
	return CDialog::Create(CDlgMyPassword::IDD);
}

void CDlgMyPassword::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgMyPassword::AtDlgShow()
{
	LoadImg();
}

void CDlgMyPassword::AtDlgHide()
{
	m_bTIM_DLG_MY_PASSWORD = FALSE;
	DelImg();
}

void CDlgMyPassword::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	myBtn.LoadBkImage(IMG_BTN_UP_DlgMyPassword, BTN_IMG_UP);
	myBtn.LoadBkImage(IMG_BTN_DN_DlgMyPassword, BTN_IMG_DN);
	
	myLabel.LoadBkImage(IMG_BK_LT_YELLOW);
}

void CDlgMyPassword::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	myBtn.DelImgList();
	myLabel.DelImgList();
}

BOOL CDlgMyPassword::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	CString sData;

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			GetDlgItem(IDC_EDIT_21)->GetWindowText(sData);
			if(sData == pDoc->m_sPassword)
				m_nRtnVal = 1;

			OnOK();
// 			else
// 			{
// 				if(m_nCtrlId == IDC_STATIC_08)
// 					OnOK();
// 			}
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgMyPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetFont(_T("±¼¸²Ã¼"),24,TRUE);

	InitPic();
	InitLabel();
	InitEdit();
	InitGroup();
	InitBtn();
	

	m_nSel = 0;
	GetDlgItem(IDC_EDIT_11)->SetFocus();

	m_bTIM_DLG_MY_PASSWORD = TRUE;
	SetTimer(TIM_WATCHDOG_MYPASSWORD, 30, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMyPassword::InitLabel()
{
	myLabel.SubclassDlgItem(IDC_STC_20, this);
	myLabel.SetTextColor(RGB_ORANGE);

	myLabel.SetFontName(_T("Arial"));
	myLabel.SetFontSize(28);
	myLabel.SetFontBold(TRUE);
	myLabel.SetBkColor(RGB_WHITE);
	myLabel.DrawBoarder();
}

void CDlgMyPassword::InitEdit()
{
	myEdit[0].SubclassDlgItem(IDC_EDIT_11, this);
	myEdit[1].SubclassDlgItem(IDC_EDIT_21, this);

	myEdit[0].SetFontName(_T("Arial"));
	myEdit[0].SetFontSize(FONT_SIZE_PW+LINE_SPACE_PW);
	myEdit[0].SetFontBold(TRUE);
	myEdit[0].SetTextColor(RGB_RED);
	myEdit[0].SetBkColor(RGB_LTYELLOW);
	myEdit[0].SetText(_T(""));
	myEdit[0].SetFont((CFont*)&myEdit[0].m_font, TRUE);
	KillFocus(IDC_EDIT_11);

	myEdit[1].SetFontName(_T("Arial"));
	myEdit[1].SetFontSize(32);
	myEdit[1].SetFontBold(TRUE);
	myEdit[1].SetTextColor(RGB_BLACK);
	myEdit[1].SetBkColor(RGB_WHITE);
	myEdit[1].SetFont((CFont*)&myEdit[1].m_font, TRUE);

}

void CDlgMyPassword::InitGroup()
{
	myGroup.SubclassDlgItem(IDC_GROUP_00, this);
	myGroup.SetHwnd(this->GetSafeHwnd(), IDC_GROUP_00);
	myGroup.SetBkColor(RGB_BLACK);
}

void CDlgMyPassword::InitBtn()
{
	myBtn.SubclassDlgItem(IDC_BTN_00, this);
	myBtn.SetHwnd(this->GetSafeHwnd(), IDC_BTN_00);
	myBtn.SetFont(_T("±¼¸²Ã¼"),24,TRUE);
	myBtn.SetTextColor(RGB_BLACK);

	GetDlgItem(IDC_BUTTON_SYM00)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM01)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM02)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM03)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM04)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM05)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM06)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM07)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM08)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM09)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM10)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM11)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM1)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM2)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM3)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM4)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM5)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM6)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM7)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM8)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM9)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM0)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_MINUS)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_EQUAL)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_Q)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_W)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_E)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_R)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_T)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_Y)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_U)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_I)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_O)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_P)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_A)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_S)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_D)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_F)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_G)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_H)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_J)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_K)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_L)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_Z)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_X)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_C)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_V)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_B)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_N)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_M)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_ESC)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_ENTER)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_DOT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_CHECK_SHIFT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SPACE)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_LEFT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_RIGHT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_BS)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_DEL)->SetFont((CFont*)&m_Font, TRUE);
}

void CDlgMyPassword::InitPic()
{
	myPic.Load(IMG_BK_LT_RED);
//	myPic.Load(BK_NOR_RED);
}

void CDlgMyPassword::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);
	myPic.Show(&dc,rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CDlgMyPassword::OnButtonSym00() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x21,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x21,0);
}

void CDlgMyPassword::OnButtonSym01() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x40,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x40,0);
}

void CDlgMyPassword::OnButtonSym02() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x23,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x23,0);
}

void CDlgMyPassword::OnButtonSym03() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x24,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x24,0);	
}

void CDlgMyPassword::OnButtonSym04() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x25,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x25,0);	
}

void CDlgMyPassword::OnButtonSym05() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x5E,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x5E,0);	
}

void CDlgMyPassword::OnButtonSym06() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x26,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x26,0);	
}

void CDlgMyPassword::OnButtonSym07() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2A,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2A,0);	
}

void CDlgMyPassword::OnButtonSym08() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x28,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x28,0);	
}

void CDlgMyPassword::OnButtonSym09() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x29,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x29,0);	
}

void CDlgMyPassword::OnButtonSym10() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x7B,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x7B,0);	
}

void CDlgMyPassword::OnButtonSym11() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x7D,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x7D,0);	
}

void CDlgMyPassword::OnButtonEqual() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x3D,0);		
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x3D,0);		
}

void CDlgMyPassword::OnButtonCharQ() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x51+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x51,0);
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x51+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x51,0);
	}
}

void CDlgMyPassword::OnButtonCharW() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x57+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x57,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x57+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x57,0);	
	}
}

void CDlgMyPassword::OnButtonCharE() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x45+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x45,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x45+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x45,0);	
	}
}

void CDlgMyPassword::OnButtonCharR() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x52+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x52,0);			
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x52+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x52,0);			
	}
}

void CDlgMyPassword::OnButtonCharT() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x54+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x54,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x54+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x54,0);	
	}
}

void CDlgMyPassword::OnButtonCharY() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x59+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x59,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x59+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x59,0);		
	}
}

void CDlgMyPassword::OnButtonCharU() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x55+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x55,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x55+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x55,0);		
	}
}

void CDlgMyPassword::OnButtonCharI() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x49+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x49,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x49+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x49,0);		
	}
}

void CDlgMyPassword::OnButtonCharO() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4F+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4F,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4F+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4F,0);	
	}
}

void CDlgMyPassword::OnButtonCharP() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x50+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x50,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x50+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x50,0);		
	}
}

void CDlgMyPassword::OnButtonCharA() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x41+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x41,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x41+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x41,0);	
	}
}

void CDlgMyPassword::OnButtonCharS() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x53+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x53,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x53+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x53,0);	
	}
}

void CDlgMyPassword::OnButtonCharD() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x44+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x44,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x44+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x44,0);		
	}
}

void CDlgMyPassword::OnButtonCharF() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x46+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x46,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x46+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x46,0);	
	}
}

void CDlgMyPassword::OnButtonCharG() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x47+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x47,0);			
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x47+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x47,0);			
	}
}

void CDlgMyPassword::OnButtonCharH() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x48+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x48,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x48+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x48,0);		
	}
}

void CDlgMyPassword::OnButtonCharJ() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4A,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4A,0);	
	}
}

void CDlgMyPassword::OnButtonCharK() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4B+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4B,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4B+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4B,0);		
	}
}

void CDlgMyPassword::OnButtonCharL() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4C+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4C,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4C+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4C,0);		
	}
}

void CDlgMyPassword::OnButtonCharZ() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x5A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x5A,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x5A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x5A,0);	
	}
}

void CDlgMyPassword::OnButtonCharX() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x58+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x58,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x58+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x58,0);	
	}
}

void CDlgMyPassword::OnButtonCharC() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x43+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x43,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x43+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x43,0);	
	}
}

void CDlgMyPassword::OnButtonCharV() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x56+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x56,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x56+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x56,0);	
	}
}

void CDlgMyPassword::OnButtonCharB() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x42+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x42,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x42+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x42,0);	
	}
}

void CDlgMyPassword::OnButtonCharN() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4E + 0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4E,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4E + 0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4E,0);	
	}
}

void CDlgMyPassword::OnButtonCharM() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4D+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4D,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4D+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4D,0);	
	}
}

void CDlgMyPassword::OnButtonComma() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2C,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2C,0);	
}

void CDlgMyPassword::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	if(m_nSel == 0)
		GetDlgItem(IDC_EDIT_11)->SetWindowText(_T(""));
	else if(m_nSel == 1)
		GetDlgItem(IDC_EDIT_21)->SetWindowText(_T(""));
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
}

void CDlgMyPassword::OnButtonSpace() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x20,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x20,0);	
}

void CDlgMyPassword::OnButtonBs() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,VK_BACK,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,VK_BACK,0);
}

void CDlgMyPassword::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_KEYDOWN,VK_DELETE,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_KEYDOWN,VK_DELETE,0);
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
}

void CDlgMyPassword::OnButtonDot() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2E,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2E,0);
}

void CDlgMyPassword::OnButtonEnter() 
{
	// TODO: Add your control notification handler code here
	CString sData;
	GetDlgItem(IDC_EDIT_21)->GetWindowText(sData);
	if(sData == pDoc->m_sPassword)
		m_nRtnVal = 1;

	OnOK();
// 	else
// 	{
// 		if(m_nCtrlId == IDC_STATIC_08)
// 			OnOK();
// 	}
}

void CDlgMyPassword::OnButtonLeft() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_KEYDOWN,VK_LEFT,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_KEYDOWN,VK_LEFT,0);
}

void CDlgMyPassword::OnButtonMinus() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2D,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2D,0);
}

void CDlgMyPassword::OnButtonNum0() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x30,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x30,0);
}

void CDlgMyPassword::OnButtonNum1() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x31,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x31,0);
}

void CDlgMyPassword::OnButtonNum2() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x32,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x32,0);
}

void CDlgMyPassword::OnButtonNum3() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x33,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x33,0);
}

void CDlgMyPassword::OnButtonNum4() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x34,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x34,0);
}

void CDlgMyPassword::OnButtonNum5() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x35,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x35,0);
}

void CDlgMyPassword::OnButtonNum6() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x36,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x36,0);
}

void CDlgMyPassword::OnButtonNum7() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x37,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x37,0);
}

void CDlgMyPassword::OnButtonNum8() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x38,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x38,0);
}

void CDlgMyPassword::OnButtonNum9() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x39,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x39,0);
}

void CDlgMyPassword::OnButtonRight() 
{
	// TODO: Add your control notification handler code here
	// GetDlgItem(IDC_EDIT_11)->SetFocus();
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_KEYDOWN,VK_RIGHT,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_KEYDOWN,VK_RIGHT,0);
}

void CDlgMyPassword::OnSetfocusEdit11() 
{
	// TODO: Add your control notification handler code here
	m_nSel = 0;
}

void CDlgMyPassword::OnSetfocusEdit21() 
{
	// TODO: Add your control notification handler code here
	m_nSel = 1;	
}

void CDlgMyPassword::OnButtonEsc() 
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

void CDlgMyPassword::OnBtn00() 
{
	// TODO: Add your control notification handler code here
	CString sData;
	GetDlgItem(IDC_EDIT_21)->GetWindowText(sData);
	if(sData == pDoc->m_sPassword)
		m_nRtnVal = 1;

	OnOK();
// 	else
// 	{
// 		if(m_nCtrlId == IDC_STATIC_08)
// 			OnOK();
// 	}

}

void CDlgMyPassword::OnChangeEdit11() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	CString str;
	GetDlgItem(IDC_EDIT_11)->GetWindowText(str);
}

void CDlgMyPassword::OnChangeEdit21() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	CString str;
	GetDlgItem(IDC_EDIT_21)->GetWindowText(str);
}

void CDlgMyPassword::SetMsg(CString strMsg, int nCtrlId)
{
	m_strMsg = strMsg;
	m_nCtrlId = nCtrlId;
}


void CDlgMyPassword::KillFocus(int nID)
{
	SetTimer(TIM_KILL_FOCUS, 50, NULL);
}


void CDlgMyPassword::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

// 	if(nIDEvent == TIM_WATCHDOG_MYPASSWORD)
// 	{
// 		KillTimer(TIM_WATCHDOG_MYPASSWORD);
// 
// 		if(pView->m_bShowModalMyPassword)
// 		{
// 			if(!this->IsWindowVisible())
// 				this->ShowWindow(SW_SHOW);
// 				
// 			Sleep(10);
// 			SetTimer(TIM_WATCHDOG_MYPASSWORD, 300, NULL);
// 		}
// 	}

	if(nIDEvent == TIM_WATCHDOG_MYPASSWORD)
	{
		KillTimer(TIM_WATCHDOG_MYPASSWORD);
// 		if(!m_bTIM_DLG_MY_PASSWORD_Lock)
// 		{
// 			m_bTIM_DLG_MY_PASSWORD_Lock = TRUE;
// 			this->PostMessage(WM_MY_PW,0,0);
// 			//DispPw();
// 		}
		if(!this->IsWindowVisible())
			this->ShowWindow(SW_SHOW);

		if(m_bTIM_DLG_MY_PASSWORD)
			SetTimer(TIM_WATCHDOG_MYPASSWORD, 500, NULL);
	}


	if(nIDEvent == TIM_KILL_FOCUS)
	{
		KillTimer(TIM_KILL_FOCUS);

		myEdit[0].SetFontSize(FONT_SIZE_PW);
		myEdit[0].SetText(m_strMsg);
		GetDlgItem(IDC_BTN_00)->SetFocus();
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT  CDlgMyPassword::OnMyPw(WPARAM wPara, LPARAM lPara)
{
	if(!this->IsWindowVisible())
		this->ShowWindow(SW_SHOW);

	m_bTIM_DLG_MY_PASSWORD_Lock = FALSE;
	return 0L;
}


void CDlgMyPassword::SetFont(CString srtFntName, int nSize, BOOL bBold)
{	
    if(m_bFont)
	{
		if(m_Font.DeleteObject())
			m_bFont = FALSE;
	}

	LOGFONT lfCtrl = {0};
	lfCtrl.lfOrientation	= 0 ;
	lfCtrl.lfEscapement		= 0 ;

	lfCtrl.lfHeight			= nSize;
	lfCtrl.lfWeight			= bBold ? FW_BOLD : FW_NORMAL;

	lfCtrl.lfItalic			= FALSE;
	lfCtrl.lfUnderline		= FALSE;
	lfCtrl.lfStrikeOut		= FALSE;

	lfCtrl.lfCharSet		= DEFAULT_CHARSET;
	lfCtrl.lfQuality		= DEFAULT_QUALITY;
	lfCtrl.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lfCtrl.lfPitchAndFamily	= DEFAULT_PITCH;
	_tcscpy(lfCtrl.lfFaceName, srtFntName);

	if(!m_bFont)
	{
		BOOL bCr = m_Font.CreateFontIndirect(&lfCtrl);
		if(bCr)
			m_bFont = TRUE;
	}
}
void CDlgMyPassword::OnCheckShift() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck();
	if(bOn)
	{
		GetDlgItem(IDC_BUTTON_CHAR_Q)->SetWindowText(_T("q"));
		GetDlgItem(IDC_BUTTON_CHAR_W)->SetWindowText(_T("w"));
		GetDlgItem(IDC_BUTTON_CHAR_E)->SetWindowText(_T("e"));
		GetDlgItem(IDC_BUTTON_CHAR_R)->SetWindowText(_T("r"));
		GetDlgItem(IDC_BUTTON_CHAR_T)->SetWindowText(_T("t"));
		GetDlgItem(IDC_BUTTON_CHAR_Y)->SetWindowText(_T("y"));
		GetDlgItem(IDC_BUTTON_CHAR_U)->SetWindowText(_T("u"));
		GetDlgItem(IDC_BUTTON_CHAR_I)->SetWindowText(_T("i"));
		GetDlgItem(IDC_BUTTON_CHAR_O)->SetWindowText(_T("o"));
		GetDlgItem(IDC_BUTTON_CHAR_P)->SetWindowText(_T("p"));
		GetDlgItem(IDC_BUTTON_CHAR_A)->SetWindowText(_T("a"));
		GetDlgItem(IDC_BUTTON_CHAR_S)->SetWindowText(_T("s"));
		GetDlgItem(IDC_BUTTON_CHAR_D)->SetWindowText(_T("d"));
		GetDlgItem(IDC_BUTTON_CHAR_F)->SetWindowText(_T("f"));
		GetDlgItem(IDC_BUTTON_CHAR_G)->SetWindowText(_T("g"));
		GetDlgItem(IDC_BUTTON_CHAR_H)->SetWindowText(_T("h"));
		GetDlgItem(IDC_BUTTON_CHAR_J)->SetWindowText(_T("j"));
		GetDlgItem(IDC_BUTTON_CHAR_K)->SetWindowText(_T("k"));
		GetDlgItem(IDC_BUTTON_CHAR_L)->SetWindowText(_T("l"));
		GetDlgItem(IDC_BUTTON_CHAR_Z)->SetWindowText(_T("z"));
		GetDlgItem(IDC_BUTTON_CHAR_X)->SetWindowText(_T("x"));
		GetDlgItem(IDC_BUTTON_CHAR_C)->SetWindowText(_T("c"));
		GetDlgItem(IDC_BUTTON_CHAR_V)->SetWindowText(_T("v"));
		GetDlgItem(IDC_BUTTON_CHAR_B)->SetWindowText(_T("b"));
		GetDlgItem(IDC_BUTTON_CHAR_N)->SetWindowText(_T("n"));
		GetDlgItem(IDC_BUTTON_CHAR_M)->SetWindowText(_T("m"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CHAR_Q)->SetWindowText(_T("Q"));
		GetDlgItem(IDC_BUTTON_CHAR_W)->SetWindowText(_T("W"));
		GetDlgItem(IDC_BUTTON_CHAR_E)->SetWindowText(_T("E"));
		GetDlgItem(IDC_BUTTON_CHAR_R)->SetWindowText(_T("R"));
		GetDlgItem(IDC_BUTTON_CHAR_T)->SetWindowText(_T("T"));
		GetDlgItem(IDC_BUTTON_CHAR_Y)->SetWindowText(_T("Y"));
		GetDlgItem(IDC_BUTTON_CHAR_U)->SetWindowText(_T("U"));
		GetDlgItem(IDC_BUTTON_CHAR_I)->SetWindowText(_T("I"));
		GetDlgItem(IDC_BUTTON_CHAR_O)->SetWindowText(_T("O"));
		GetDlgItem(IDC_BUTTON_CHAR_P)->SetWindowText(_T("P"));
		GetDlgItem(IDC_BUTTON_CHAR_A)->SetWindowText(_T("A"));
		GetDlgItem(IDC_BUTTON_CHAR_S)->SetWindowText(_T("S"));
		GetDlgItem(IDC_BUTTON_CHAR_D)->SetWindowText(_T("D"));
		GetDlgItem(IDC_BUTTON_CHAR_F)->SetWindowText(_T("F"));
		GetDlgItem(IDC_BUTTON_CHAR_G)->SetWindowText(_T("G"));
		GetDlgItem(IDC_BUTTON_CHAR_H)->SetWindowText(_T("H"));
		GetDlgItem(IDC_BUTTON_CHAR_J)->SetWindowText(_T("J"));
		GetDlgItem(IDC_BUTTON_CHAR_K)->SetWindowText(_T("K"));
		GetDlgItem(IDC_BUTTON_CHAR_L)->SetWindowText(_T("L"));
		GetDlgItem(IDC_BUTTON_CHAR_Z)->SetWindowText(_T("Z"));
		GetDlgItem(IDC_BUTTON_CHAR_X)->SetWindowText(_T("X"));
		GetDlgItem(IDC_BUTTON_CHAR_C)->SetWindowText(_T("C"));
		GetDlgItem(IDC_BUTTON_CHAR_V)->SetWindowText(_T("V"));
		GetDlgItem(IDC_BUTTON_CHAR_B)->SetWindowText(_T("B"));
		GetDlgItem(IDC_BUTTON_CHAR_N)->SetWindowText(_T("N"));
		GetDlgItem(IDC_BUTTON_CHAR_M)->SetWindowText(_T("M"));
	}
}
